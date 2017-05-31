#include<stdio.h>
#include<cv.h>
#include<highgui.h>
#include<math.h>
#include<ctype.h>
#include"myfunction.h"

#define camera 1

int main(int argc, char **argv){
  int x,y,tmp;
  
  IplImage *src_img=0,*bg_img=0,*tmp_img=0,*dst_img=0;
  CvCapture *capture=0;int c=0;
  char *window_name = "Image";
  
  CvMemStorage *storage = cvCreateMemStorage(0);
  CvMemStorage *storagepoly = cvCreateMemStorage(0);
  
  cvNamedWindow (window_name, CV_WINDOW_AUTOSIZE);
  
  capture = cvCreateCameraCapture(0);
  src_img = cvQueryFrame(capture);
  
  tmp_img=cvCreateImage(cvGetSize (src_img), IPL_DEPTH_8U, 1);
  bg_img=cvCreateImage(cvGetSize (src_img), IPL_DEPTH_8U, 3);
  bg_img=cvQueryFrame(capture);
  dst_img = cvCreateImage(cvGetSize (src_img), IPL_DEPTH_8U, 3);
  
  while(1){
    src_img = cvQueryFrame(capture);
    if(c=='s')bg_img=cvQueryFrame(capture);
    
    mydraw(src_img,tmp_img,dst_img,storage,storagepoly);
    cvShowImage(window_name,dst_img);
    
    void* life = cvGetWindowHandle(window_name);
    c=cvWaitKey(100);
    
    if(c=='\x1b'||NULL==life){break;}
  }
  
  cvDestroyWindow(window_name);
  
  cvReleaseCapture(&capture);
  cvReleaseImage(&src_img);
  cvReleaseImage(&tmp_img);
  cvReleaseImage(&bg_img);
  cvReleaseImage(&dst_img);
  cvReleaseMemStorage(&storage);
  cvReleaseMemStorage(&storagepoly);
  return 0;
}
