
#define TRUE 1
#define FALSE 0

extern CvFont font;
extern char debug_text[256];
extern int debug_int[8];
extern int frame_x[4],frame_y[4],start_x[4],start_y[4], goal_x[4],goal_y[4];
extern IplImage *src_img,*tmp_img,*dst_img;
extern CvMemStorage *storage, *storagepoly;
extern GLuint texture[1];
extern int flag;

void mydraw();
int labelling();

void nearpoint(int tmp_x[4], int tmp_y[4],
	      int frame_x[4], int frmae_y[4],
	      int x, int y,
	      int number);
void mydraw(){
  //グレースケール化
  cvCvtColor(src_img,tmp_img,CV_BGR2GRAY);
  
  //ガウシアンフィルタ
  cvSmooth(tmp_img,tmp_img,CV_GAUSSIAN,3,0,0,0);
  //二値化
  cvThreshold(tmp_img, tmp_img,0,255,CV_THRESH_BINARY|CV_THRESH_OTSU);

  cvNot(tmp_img, tmp_img);
  //cvCanny(tmp_img,tmp_img,30,80,3);
  //ラベリング
  flag = labelling(tmp_img, dst_img, storage,storagepoly);

  
  //デバック用の文字出力
  cvInitFont(&font, CV_FONT_HERSHEY_PLAIN, 1.0, 1.0, 0,1,8);
  sprintf(debug_text,"1(%d, %d), 2(%d, %d), 3(%d, %d), (%d, %d)",
	  frame_x[0],frame_y[0],
	  frame_x[1],frame_y[1],
	  frame_x[2],frame_y[2],
	  frame_x[3],frame_y[3]
	  );
  cvPutText(dst_img, debug_text, cvPoint(15, 30), &font, CV_RGB(255,255,255));

  
}

int labelling(){
  CvMemStorage *child_storage = cvCreateChildMemStorage(storage);
  CvMemStorage *child_storagepoly = cvCreateChildMemStorage(storagepoly);
  CvSeq *contours = 0;
  CvSeq *polycontour=NULL;
  int contourCount=0;
  contourCount = cvFindContours(tmp_img,
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
  
  
  CvPoint *point;//ポリゴン点の座標取得に用いる。
  int tmp_x[4],tmp_y[4];
  int i,num; //numはスタート頂点の番号frame[0]=tmp[num]となるようにする

  for(CvSeq* c = polycontour;c!=NULL;c=c->h_next){
    if((cvContourPerimeter(c)<2000)&&(cvContourPerimeter(c)>60)&&(c->total==4)){
      //四角形の中に四角形があればマーカーとする。
      if(c->v_next!=NULL&&c->v_next->v_next!=NULL){	
	cvSet(dst_img, CV_RGB(0,0,0), NULL);
	//全体の四角形の頂点座標取得（ランダム）ー＞スタート、ゴールの角の座標を取得したい
	for(i=0;i<4;i++){
	  point = CV_GET_SEQ_ELEM(CvPoint,c,i);
	  tmp_x[i]=point->x;
	  tmp_y[i]=point->y;
	}
	flag = 1;
	//int nearest_index = 0;
	CvSeq* c_v2=c->v_next;
	CvSeq* c_v3=c_v2->v_next;
	
	cvDrawContours(dst_img,c,CV_RGB(255,255,0),CV_RGB(200,255,255),0,CV_FILLED,8,cvPoint(0,0));
	cvDrawContours(dst_img,c_v2,CV_RGB(255,255,255),CV_RGB(0,255,255),0,CV_FILLED,8,cvPoint(0,0));

	//中にある四角形の色塗り
	while(TRUE){
	  cvDrawContours(dst_img,c_v3,CV_RGB(255,0,0),CV_RGB(0,255,255),0,CV_FILLED,8,cvPoint(0,0));
	  
	  //四角形にさらに穴が開いていたらスタート、さもなくばゴール
	  if(c_v3->total == 4){
	    if(c_v3->v_next != NULL){
	      
	      for(i=0;i<4;i++){
		point = CV_GET_SEQ_ELEM(CvPoint,c_v3,i);
		start_x[i] = point->x;
		start_y[i] = point->y;
	      }
	      //スタートに一番近いframeの点を決める
	      nearpoint(tmp_x,tmp_y,frame_x,frame_y,start_x[0],start_y[0],0);
	    }else{
	      
	      for(i=0;i<4;i++){
		point = CV_GET_SEQ_ELEM(CvPoint,c_v3,i);
		goal_x[i]=point->x;
		goal_y[i]=point->y;
	      }
	      //ゴールに一番近いフレーム野店を決める
	      nearpoint(tmp_x,tmp_y,frame_x,frame_y,goal_x[0],goal_y[0],3);
	    }
	  }
	  if(c_v3->h_next == NULL) {break;}else{c_v3=c_v3->h_next;}

	}
	//スタートの方の頂点に近ければframe[1]になる
	nearpoint(tmp_x,tmp_y,frame_x,frame_y,frame_x[0],frame_y[0],1);
	nearpoint(tmp_x,tmp_y,frame_x,frame_y,frame_x[3],frame_y[3],2);
	
	cvReleaseMemStorage(&child_storage);
	cvReleaseMemStorage(&child_storagepoly);
        return 1;
      }
    }
    
  }
  
  //cvReleaseSeq(&contours);
  cvReleaseMemStorage(&child_storage);
  cvReleaseMemStorage(&child_storagepoly);
  return 0;
}

void nearpoint(int tmp_x[4], int tmp_y[4],int frame_x[4], int frmae_y[4], int x, int y, int number){
  int i, num=0;
  int min=pow(tmp_x[0]-x,2)+pow(tmp_y[0]-y,2);
  for(i=1; i<4; i++){
    if(min > pow(tmp_x[i]-x, 2) + pow(tmp_y[i]-y, 2) ){
      num = i;
      min = pow(tmp_x[i]-x, 2) + pow(tmp_y[i]-y, 2);
    }
  }
  frame_x[number]=tmp_x[num];
  frame_y[number]=tmp_y[num];
  tmp_x[num]=10000;
  tmp_y[num]=10000;
}
  
    
