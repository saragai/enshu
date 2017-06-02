
#define TRUE 1
#define FALSE 0

extern CvFont font;
extern char debug_text[256];
extern int debug_int[8];


void mydraw(IplImage *src_img,
	    IplImage *tmp_img,
	    IplImage *dst_img,
	    CvMemStorage *parent_storage,
	    CvMemStorage *parent_storagepoly
	    );
void labelling(IplImage *src_img,
	       IplImage *dst_img,
	       CvMemStorage *parent_storage,
	       CvMemStorage *parent_storagepoly);
//label function
void DrawNextContour(IplImage *img, //ラベリング結果を描画するIplImage（モノクロ）
		     CvSeq *Contour,//輪郭へのポインタ
		     int Level      //輪郭の階層
		     );
void DrawChildContour(IplImage *img,    //ラベリング結果を描画するIplImage(8Bit3chカラー）
		      CvSeq *Contour, //輪郭へのポインタ
		      int Level//輪郭のレベル（階層）
		      );


void mydraw(IplImage *src_img,//Input
	    IplImage *tmp_img,
	    IplImage *dst_img,//Output
	    CvMemStorage *parent_storage,
	    CvMemStorage *parent_storagepoly
	    ){

  
  //グレースケール化
  cvCvtColor(src_img,tmp_img,CV_BGR2GRAY);
  
  //ガウシアンフィルタ
  cvSmooth(tmp_img,tmp_img,CV_GAUSSIAN,3,0,0,0);
  //二値化
  cvThreshold(tmp_img, tmp_img,0,255,CV_THRESH_BINARY|CV_THRESH_OTSU);

  cvNot(tmp_img, tmp_img);
  //cvCanny(tmp_img,tmp_img,30,80,3);
  //ラベリング
  labelling(tmp_img, dst_img, parent_storage,parent_storagepoly);

  //デバック用の文字出力
  cvInitFont(&font, CV_FONT_HERSHEY_PLAIN, 1.0, 1.0, 0,1,8);
  sprintf(debug_text,"1(%d, %d), 2(%d, %d), 3(%d, %d), (%d, %d)",
	  debug_int[0],debug_int[4],debug_int[1],debug_int[5],
	  debug_int[2],debug_int[6],debug_int[3],debug_int[7]);
  cvPutText(dst_img, debug_text, cvPoint(15, 30), &font, CV_RGB(255,255,255));
}

void labelling(IplImage *src_img,
	       IplImage *dst_img,
	       CvMemStorage *parent_storage,
	       CvMemStorage *parent_storagepoly){
  CvMemStorage *child_storage = cvCreateChildMemStorage(parent_storage);
  CvMemStorage *child_storagepoly = cvCreateChildMemStorage(parent_storagepoly);
  CvSeq *contours = 0;
  CvSeq *polycontour=NULL;
  int contourCount=0;
  cvSet(dst_img, CV_RGB(0,0,0), NULL);
  contourCount = cvFindContours(src_img,
				child_storage,
				&contours,
				sizeof(CvContour),
				CV_RETR_TREE,
				CV_CHAIN_APPROX_SIMPLE,
				cvPoint(0,0));
  
  polycontour = cvApproxPoly(contours,
			     sizeof(CvContour),
			     child_storagepoly,
			     CV_POLY_APPROX_DP,
			     10,
			     1
			     );
  
  
  for(CvSeq* c = polycontour;c!=NULL;c=c->h_next){
    if((cvContourPerimeter(c)<2000)&&(cvContourPerimeter(c)>60)&&(c->total==4)){
      //四角形の中に四角形があればマーカーとする。
      if(c->v_next!=NULL&&c->v_next->v_next!=NULL){
	CvPoint *point;
	for(int i=0;i<4;i++){
	  point =CV_GET_SEQ_ELEM(CvPoint,c,i);
	  debug_int[i]=point->x;
	  debug_int[i+4]=point->y;
	}
	int nearest_index = 0;
	CvSeq* c_v2=c->v_next;
	CvSeq* c_v3=c_v2->v_next;
	
	cvDrawContours(dst_img,c,CV_RGB(255,255,0),CV_RGB(200,255,255),0,CV_FILLED,8,cvPoint(0,0));
	cvDrawContours(dst_img,c_v2,CV_RGB(255,255,255),CV_RGB(0,255,255),0,CV_FILLED,8,cvPoint(0,0));
	while(TRUE){
	  cvDrawContours(dst_img,c_v3,CV_RGB(255,0,0),CV_RGB(0,255,255),0,CV_FILLED,8,cvPoint(0,0));
	  
	  if(c_v3->h_next == NULL) {break;}else{c_v3=c_v3->h_next;}
	  
	}
	
      }
    }
  }
  
  //cvReleaseSeq(&contours);
  cvReleaseMemStorage(&child_storage);
  cvReleaseMemStorage(&child_storagepoly);
}
