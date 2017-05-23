void mydraw(IplImage *src_img, IplImage *dst_img);


void mydraw(IplImage *src_img, IplImage *dst_img){
  //二値化
  cvThreshold(src_img, dst_img,0,255,CV_THRESH_BINARY|CV_THRESH_OTSU);

  cv_Labelling(src_img,dst_img);
}

