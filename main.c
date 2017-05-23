#include<stdio.h>
#include<cv.h>
#include<highgui.h>
#include<math.h>
#include<ctype.h>
#include"myfunction.h"
#define camera 1

int main(int argc, char **argv){
  int x,y,tmp;
  IplImage *src_img=0,*bg_img=0,*src_img_gray=0,*dst_img=0;
  CvCapture *capture=0;int c=0;
  char *window_name = "Image";
  cvNamedWindow (window_name, CV_WINDOW_AUTOSIZE);
  
  if(camera ==1){
    capture = cvCreateCameraCapture(0);
    src_img = cvQueryFrame(capture);
    
    src_img_gray=cvCreateImage(cvGetSize (src_img), IPL_DEPTH_8U, 1);
    bg_img = cvCreateImage(cvGetSize (src_img), IPL_DEPTH_8U, 1);
    bg_img=cvQueryFrame(capture);
    dst_img = cvCreateImage(cvGetSize (src_img), IPL_DEPTH_8U, 1);
    
    while(1){
      src_img = cvQueryFrame(capture);
      if(c=='s')bg_img=cvQueryFrame(capture);
      cvCvtColor(src_img,src_img_gray,CV_BGR2GRAY);
      mydraw(src_img_gray,dst_img);
      cvShowImage(window_name,dst_img);
      void* life = cvGetWindowHandle(window_name);
      c=cvWaitKey(10);
      
      if(c=='\x1b'||NULL==life){break;}
    }
  }
  else{
    if (argc!=2 || (src_img = cvLoadImage(argv[1], CV_LOAD_IMAGE_COLOR)) == 0){
      return -1;
    }
    src_img_gray=cvCreateImage(cvGetSize (src_img), IPL_DEPTH_8U, 1);
    dst_img = cvCreateImage(cvGetSize (src_img), IPL_DEPTH_8U, 1);
    mydraw(src_img_gray,dst_img);
    cvShowImage(window_name,dst_img);
    cvWaitKey(0);
  }
  
  //ここまで画像の取得

  
  
  
  
  //cvCanny (src_img_gray, dst_img, 50.0,200.0,3);
  
  //ここから画像の描画
  
  cvDestroyWindow(window_name);
  
  cvReleaseCapture(&capture);
  cvReleaseImage(&src_img);
  cvReleaseImage(&src_img_gray);
  cvReleaseImage(&dst_img);
  return 0;
}
