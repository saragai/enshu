#include<stdio.h>
#include<cv.h>
#include<highgui.h>
#include<math.h>
#include<ctype.h>
#include"myfunction.h"
<<<<<<< HEAD

=======
>>>>>>> e7e744aa4b55c5e2dbc22eea22a7c7935173c71b
#define camera 1

int main(int argc, char **argv){
  int x,y,tmp;
<<<<<<< HEAD
  IplImage *src_img=0,*bg_img=0,*tmp_img=0,*dst_img=0;
  CvCapture *capture=0;int c=0;
  char *window_name = "Image";
  
  CvMemStorage *storage = cvCreateMemStorage(0);

=======
  IplImage *src_img=0,*bg_img=0,*src_img_gray=0,*dst_img=0;
  CvCapture *capture=0;int c=0;
  char *window_name = "Image";
>>>>>>> e7e744aa4b55c5e2dbc22eea22a7c7935173c71b
  cvNamedWindow (window_name, CV_WINDOW_AUTOSIZE);
  
  if(camera ==1){
    capture = cvCreateCameraCapture(0);
    src_img = cvQueryFrame(capture);
    
<<<<<<< HEAD
    tmp_img=cvCreateImage(cvGetSize (src_img), IPL_DEPTH_8U, 1);
    bg_img=cvCreateImage(cvGetSize (src_img), IPL_DEPTH_8U, 3);
    bg_img=cvQueryFrame(capture);
    dst_img = cvCreateImage(cvGetSize (src_img), IPL_DEPTH_8U, 3);
=======
    src_img_gray=cvCreateImage(cvGetSize (src_img), IPL_DEPTH_8U, 1);
    bg_img = cvCreateImage(cvGetSize (src_img), IPL_DEPTH_8U, 1);
    bg_img=cvQueryFrame(capture);
    dst_img = cvCreateImage(cvGetSize (src_img), IPL_DEPTH_8U, 1);
>>>>>>> e7e744aa4b55c5e2dbc22eea22a7c7935173c71b
    
    while(1){
      src_img = cvQueryFrame(capture);
      if(c=='s')bg_img=cvQueryFrame(capture);
<<<<<<< HEAD
      
      mydraw(src_img,tmp_img,dst_img,storage);
      
=======
      cvCvtColor(src_img,src_img_gray,CV_BGR2GRAY);
      mydraw(src_img_gray,dst_img);
>>>>>>> e7e744aa4b55c5e2dbc22eea22a7c7935173c71b
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
<<<<<<< HEAD
    tmp_img=cvCreateImage(cvGetSize (src_img), IPL_DEPTH_8U, 3);
    dst_img = cvCreateImage(cvGetSize (src_img), IPL_DEPTH_8U, 3);
    mydraw(src_img,tmp_img,dst_img,storage);
=======
    src_img_gray=cvCreateImage(cvGetSize (src_img), IPL_DEPTH_8U, 1);
    dst_img = cvCreateImage(cvGetSize (src_img), IPL_DEPTH_8U, 1);
    mydraw(src_img_gray,dst_img);
>>>>>>> e7e744aa4b55c5e2dbc22eea22a7c7935173c71b
    cvShowImage(window_name,dst_img);
    cvWaitKey(0);
  }
  
  //ここまで画像の取得

  
  
  
  
  //cvCanny (src_img_gray, dst_img, 50.0,200.0,3);
  
  //ここから画像の描画
  
  cvDestroyWindow(window_name);
  
  cvReleaseCapture(&capture);
  cvReleaseImage(&src_img);
<<<<<<< HEAD
  cvReleaseImage(&tmp_img);
  cvReleaseImage(&bg_img);
  cvReleaseImage(&dst_img);
  cvReleaseMemStorage(&storage);
=======
  cvReleaseImage(&src_img_gray);
  cvReleaseImage(&dst_img);
>>>>>>> e7e744aa4b55c5e2dbc22eea22a7c7935173c71b
  return 0;
}
