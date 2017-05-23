#include<stdio.h>
#include<cv.h>
#include<highgui.h>
#include<math.h>

int main(int argc, char **argv){
  int x,y,tmp;
  IplImage *src_img=0;
  char *window_name = "Image";
  if (argc!=2 || (src_img=cvLoadImage(argv[1], CV_LOAD_IMAGE_COLOR)) == 0){
    return -1;
  }
  cvNamedWindow (window_name, CV_WINDOW_AUTOSIZE);
  cvshowImage(window_name,src_img);
  cvWaitKey(0);
  cvDestroyWindow(window_name);
  cvReleaseImage(&src_img);
  return 0;
}

  
  
  
