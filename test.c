
#include<stdio.h>
#include<cv.h>
#include<highgui.h>
#include<math.h>
#include<ctype.h>
#include<GL/glut.h>
#include<unistd.h>
#include"myfunction_mat.h"
#include"myfunction.h"
#include"myfunction_gl.h"

void display();


// デバック用
CvFont font;
char debug_text[256];
int debug_int[8];

IplImage *src_img=0,*tmp_img=0,*dst_img=0;
char *window_name = "Image";
CvCapture *capture=0;
CvMemStorage *storage, *storagepoly;

int flag=1;//現在のフレームでマーカーが検出されたかどうか。されていたら1になる

int frame_x[4],frame_y[4],start_x[4],start_y[4], goal_x[4],goal_y[4];

//ここから座標変換のため
double mkx[4],mky[4];
double mkx_in_cmr[3], mky_in_cmr[3], mkz_in_cmr[3];
double mk_scale=0;
double mat_mk_cmr[3][3];
GLuint texture[1];//テクスチャのIDを格納する配列

//ここから球の移動のため
double v[3]={0,0,0};
double pos[3]={0,0,0};
double pos_c[3];
//ライティングの情報
static const GLfloat light_position[] = { 5.0, 5.0, 5.0, 0.0 };
static const GLfloat light_ambient[] = {0.4, 0.4, 0.4, 1.0};
static const GLfloat light_diffuse[] = {1.0, 1.0, 1.0, 1.0};

static const GLfloat mat_default_color[] = { 1.0, 1.0, 1.0, 1.0 };
static const GLfloat mat_default_specular[] = { 1.0, 1.0, 1.0, 1.0 };
static const GLfloat mat_default_shininess[] = { 100.0 };
static const GLfloat mat_default_emission[] = {0.0, 0.0, 0.0, 0.0};
void init(){
  //OpenCV側
  printf("here\n");
  capture = cvCreateCameraCapture(0);
  src_img = cvQueryFrame(capture);
  
  tmp_img = cvCreateImage(cvGetSize (src_img), IPL_DEPTH_8U, 1);
  dst_img = cvCreateImage(cvGetSize (src_img), IPL_DEPTH_8U, 3);
  storage = cvCreateMemStorage(0);
  storagepoly = cvCreateMemStorage(0);

  //OpenGL側
  glutInitDisplayMode(GLUT_RGBA| GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowPosition(200,200);
  glutInitWindowSize(640,480);

  glGenTextures(1, &texture[0]);//テクスチャ
  
  //三次元用
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glClearDepth(1.0);
  // デプステストを行う
  glEnable( GL_DEPTH_TEST );
  glDepthFunc( GL_LESS );
  
  glShadeModel(GL_SMOOTH);
  // デフォルトライト
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);
  glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light_ambient);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);

  // デフォルトマテリアル
  glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_default_color);
  glMaterialfv(GL_FRONT, GL_AMBIENT, mat_default_color);
  glMaterialfv(GL_FRONT, GL_SPECULAR, mat_default_specular);
  glMaterialfv(GL_FRONT, GL_SHININESS, mat_default_shininess);
}



int main(int argc, char **argv){
  glutInit(&argc,argv);
  init();
  glutCreateWindow(window_name);
  
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(keyboard);
  glutSpecialFunc(specialkeydown);
  glutMainLoop();
  
  cvReleaseImage(&src_img);
  cvReleaseImage(&tmp_img);
  cvReleaseImage(&dst_img);
  cvReleaseCapture(&capture);
  return 0;
}

void display(){
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glColor3f(1.0,1.0,1.0);
  
  src_img = cvQueryFrame(capture);
  mydraw();
  background();
  glLoadIdentity();
  gluLookAt(0.0, 0.0, 5.0,//カメラ位置
	    0.0, 0.0, 0.0,//レンズを向ける方向
	    0.0, 1.0, 0.0);//上方向ベクトル
  glScalef(2.0, 2.0, 1.0);
  cmr_to_mk();
  mkframe();
  //mkxyz();
  mkcube();
  /*
  
  glPushMatrix();
  {
    glColor3f(1.0,0.0,0.0);
    glTranslated(pos_c[0],pos_c[1],pos_c[2]);
    glutSolidSphere(0.1,30,30);
  }
  glPopMatrix();
  */
  //printf("%d\n",flag);
  printf("%lf\n",mk_scale);
  glutPostRedisplay();
  
  glutSwapBuffers();
}
