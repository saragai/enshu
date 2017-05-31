
#define TRUE 1
#define FALSE 0

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
CvScalar FilterContour(CvSeq *Contour, int Level, int *flag);//輪郭へのポインタ（この関数自体はTRUE FALSEで返す)
int ChoiceColor();

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

  
}

void labelling(IplImage *src_img, IplImage *dst_img, CvMemStorage *parent_storage,CvMemStorage *parent_storagepoly){
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
				 CV_RETR_CCOMP,
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
      if(c->v_next!=NULL){
	if(c->v_next->total==4){
	  int nearest_index = 0;
	  CvSeq* c_vnext=c->v_next;
	  
	  cvDrawContours(dst_img,c,CV_RGB(255,255,0),CV_RGB(200,255,255),0,CV_FILLED,8,cvPoint(0,0));
	  cvDrawContours(dst_img,c_vnext,CV_RGB(255,0,0),CV_RGB(0,255,255),0,CV_FILLED,8,cvPoint(0,0));
	}
      }
    }
  }
  

  /*
  if(contours !=NULL)
    cvSet(dst_img, CV_RGB(0,0,0), NULL);
    DrawNextContour(dst_img,contours,1);
  }
  */
  //cvReleaseSeq(&contours);
  cvReleaseMemStorage(&child_storage);
  cvReleaseMemStorage(&child_storagepoly);
}

void DrawNextContour(IplImage *img, //ラベリング結果を描画するIplImage（モノクロ）
		     CvSeq *Contour,//輪郭へのポインタ
		     int Level      //輪郭の階層
		     ){
  CvScalar ContoursColor;
  int flag=TRUE;
  ContoursColor =  FilterContour(Contour,Level,&flag);
  
  //輪郭の描写
  cvDrawContours(img,Contour,ContoursColor,ContoursColor,0,CV_FILLED,8,cvPoint(0,0));
  if (Contour->h_next != NULL)
    //次の輪郭がある場合は次の輪郭を描画
    DrawNextContour(img, Contour->h_next, Level);
  if (Contour->v_next != NULL && flag == TRUE)
    //子の輪郭がある場合は子の輪郭を描画
    DrawChildContour(img, Contour->v_next, Level + 1);
}
void DrawChildContour(IplImage *img, //ラベリング結果を描画するIplImage（モノクロ）
		     CvSeq *Contour,//輪郭へのポインタ
		     int Level      //輪郭の階層
		     ){
  CvScalar ContoursColor;
  int flag=TRUE;
  ContoursColor =  FilterContour(Contour,Level,&flag);
  
  //輪郭の描写
  cvDrawContours(img,Contour,ContoursColor,ContoursColor,0,CV_FILLED,8,cvPoint(0,0));
  if (Contour->h_next != NULL)
    //次の輪郭がある場合は次の輪郭を描画
    DrawNextContour(img, Contour->h_next, Level);
  if (Contour->v_next != NULL&&flag==TRUE)
    //子の輪郭がある場合は子の輪郭を描画
    DrawChildContour(img, Contour->v_next, Level + 1);
}

CvScalar FilterContour(CvSeq *Contour, int Level, int *flag){
  CvScalar ContoursColor;
  if (cvArcLength(Contour, CV_WHOLE_SEQ, -1)<100){
    *flag = FALSE;
  }
  if(*flag==TRUE){
    if((Level%2)==1){
      //白の輪郭ならば、赤で描写
      ContoursColor = CV_RGB(255,0,0);
    }else{
      //黒の輪郭ならば、青で
      ContoursColor = CV_RGB(0,0,255);
    }
  }else{
    ContoursColor = CV_RGB(0,255,0);
  }
  return ContoursColor;
}
