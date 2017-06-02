#include<stdio.h>
#include<cv.h>
#include<highgui.h>
#include<math.h>
#include<ctype.h>
#include<GL/glut.h> 
#include"myfunction.h"

void mapTexture();
void keyboard(unsigned char key, int x, int y);
void display();


// デバック用
CvFont font;
char debug_text[256];
int debug_int[8];

IplImage *src_img=0,*tmp_img=0,*dst_img=0;
char *window_name = "Image";
CvCapture *capture=0;
CvMemStorage *storage;
CvMemStorage *storagepoly;

GLuint texture[1];

int init(){
  //OpenCV側
  capture = cvCreateCameraCapture(0);
  src_img=cvQueryFrame(capture);
  tmp_img=cvCreateImage(cvGetSize (src_img), IPL_DEPTH_8U, 1);
  dst_img = cvCreateImage(cvGetSize (src_img), IPL_DEPTH_8U, 3);
  storage = cvCreateMemStorage(0);
  storagepoly = cvCreateMemStorage(0);

  //OpenGL側
  glutInitDisplayMode(GLUT_RGBA| GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowPosition(200,200);
  glutInitWindowSize(640,480);
  glShadeModel(GL_FLAT);
}
int LoadGLTextures(){
  IplImage *dbg_img=0;
  dbg_img=dst_img;
  cvFlip(dbg_img,NULL,0);
  
  glGenTextures(1, &texture[0]);
  glBindTexture(GL_TEXTURE_2D, texture[0]);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
  
  glTexImage2D(GL_TEXTURE_2D,
	       0,
	       GL_RGB,
	       dbg_img->width,
	       dbg_img->height,
	       0,GL_BGR_EXT,
	       GL_UNSIGNED_BYTE,
	       dbg_img->imageData);
  
  return 1;
}

int main(int argc, char **argv){
  glutInit(&argc,argv);
  init();
  glutCreateWindow(window_name);
  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);
  glutMainLoop();
  
  cvReleaseImage(&src_img);
  cvReleaseImage(&tmp_img);
  cvReleaseImage(&dst_img);
  cvReleaseCapture(&capture);
  return 0;
}

void display(){
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  src_img = cvQueryFrame(capture);
  mydraw(src_img,tmp_img,dst_img,storage,storagepoly);
  LoadGLTextures();
  
  glBegin(GL_QUADS);
  glColor3f(0.0,1.0,1.0);
  glVertex2f(debug_int[0]/320.0-1,debug_int[4]/240.0-1);
  glVertex2f(debug_int[1]/320.0-1,debug_int[5]/240.0-1);
  glVertex2f(debug_int[2]/320.0-1,debug_int[6]/240.0-1);
  glVertex2f(debug_int[3]/320.0-1,debug_int[7]/240.0-1);
  glEnd();
  
  glutPostRedisplay();
  mapTexture();
  
}

void mapTexture(){
  static const GLfloat color[] = {1.0, 1.0, 1.0, 1.0};
  glMaterialfv(GL_FRONT,GL_AMBIENT_AND_DIFFUSE,color);
  glEnable(GL_TEXTURE_2D);
  glNormal3d(0.0,0.0,1.0);
  glBegin(GL_QUADS);
  glColor3f(1.0,1.0,1.0);
  
  glTexCoord2d(0.0, 1.0);
  glVertex3d(-1,1,0.5);
  glTexCoord2d(0.0, 0.0);
  glVertex3d(-1,-1,0.5);
  glTexCoord2d(1.0, 0.0);
  glVertex3d(1,-1,0.5);
  glTexCoord2d(1.0, 1.0);
  glVertex3d(1,1,0.5);
  glEnd();
  glDisable(GL_TEXTURE_2D);
  glutSwapBuffers();
}
void keyboard(unsigned char key, int x, int y)
{
  switch (key) {
  case 'q':
  case 'Q':
  case '\033':  /* '\033' は ESC の ASCII コード */
    exit(0);
  default:
    break;
  }
}
