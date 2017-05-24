
#define TRUE 1
#define FALSE 0

void mydraw(IplImage *src_img,IplImage *tmp_img, IplImage *dst_img, CvMemStorage *parent_storage);
void labelling(IplImage *src_img, IplImage *dst_img, CvMemStorage *parent_storage);
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
	    CvMemStorage *parent_storage){
  
  //グレースケール化
  cvCvtColor(src_img,tmp_img,CV_BGR2GRAY);
  
  //ガウシアンフィルタ
  cvSmooth(tmp_img,tmp_img,CV_GAUSSIAN,5,0,0,0);
  //二値化
  cvThreshold(tmp_img, tmp_img,0,255,CV_THRESH_BINARY|CV_THRESH_OTSU);
  
  //cvCanny(tmp_img,tmp_img,30,80,3);
  //ラベリング
  labelling(tmp_img, dst_img, parent_storage);

  
}

void labelling(IplImage *src_img, IplImage *dst_img, CvMemStorage *parent_storage){
  CvMemStorage *child_storage = cvCreateChildMemStorage(parent_storage);
  CvSeq *contours = 0;
  
  cvFindContours(src_img,
		 child_storage,
		 &contours,
		 sizeof(CvContour),
		 CV_RETR_TREE,
		 CV_CHAIN_APPROX_SIMPLE,
		 cvPoint(0,0));
  
  if(contours !=NULL){
    cvSet(dst_img, CV_RGB(0,0,0), NULL);
    DrawNextContour(dst_img,contours,1);
  }
  //cvReleaseSeq(&contours);
  cvReleaseMemStorage(&child_storage);
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
