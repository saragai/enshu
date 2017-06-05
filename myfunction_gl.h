/*
extern IplImage *src_img, *dst_img;

extern double x[4],y[4];
*/
extern double mkx_in_cmr[3], mky_in_cmr[3], mkz_in_cmr[3];
extern double mkx[4],mky[4];
extern double mat_mk_cmr[3][3];
extern double mk_scale;
extern double v[3],pos[3],pos_c[3];
void cvt_mk2cmr(double a[3],double b[3]);

int LoadGLTextures(){
  IplImage *dbg_img=0;
  dbg_img=src_img;//出力画像の設定
  cvFlip(dbg_img,NULL,0);
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

void background(){
  float bgh=4.5,bgw=6,bgdepth=-15;
  LoadGLTextures();
  glEnable(GL_TEXTURE_2D);
  glBegin(GL_QUADS);
  glTexCoord2f(0,0); glVertex3f(-1 * bgw, -1 * bgh, bgdepth);
  glTexCoord2f(0,1); glVertex3f(-1 * bgw, bgh     , bgdepth);
  glTexCoord2f(1,1); glVertex3f(bgw,      bgh     , bgdepth);
  glTexCoord2f(1,0); glVertex3f(bgw,      -1 * bgh, bgdepth);
  glEnd();
}


void reshape (int w, int h){
  GLsizei glw = (GLsizei)w, glh = (GLsizei)h;
  if(w<h){//高さのほうが大きい
    glViewport(0, (glh - glw)/2, glw, glw);
  }else{//幅のほうが大きい
    glViewport((glw - glh)/2, 0, glh, glh);
  }
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glFrustum(-1.0,1.0,-1.0,1.0,
	    3.0,25.0);
  glMatrixMode(GL_MODELVIEW);
}


void keyboard(unsigned char key, int x, int y)
{
  switch (key) {
  case 'q':
  case 'Q':
  case '\033':  // '\033' は ESC の ASCII コード
    exit(0);

  default:
    break;
  }
}

//マーカーフレームを-1~1に直す
void mkframe(){
  int i;
  for(i=0;i<4;i++){
    mkx[i]=(frame_x[i]-320)/320.0;
    mky[i]=(240-frame_y[i])/240.0;
  }
}

void cmr_to_mk(){
  //x軸のもと
  
  double v3to0[3]={mkx[0]-mkx[3],mky[0]-mky[3],0};
  double v0toC[3]={-mkx[0],-mky[0],5};
  double h30C[3];
  double v1to2[3]={mkx[2]-mkx[1],mky[2]-mky[1],0};
  double v2toC[3]={-mkx[2],-mky[2],5};
  double h12C[3];
  //y軸のもと
  double v0to1[3]={mkx[1]-mkx[0],mky[1]-mky[0],0};
  double v1toC[3]={-mkx[1],-mky[1],5};
  double h01C[3];
  double v2to3[3]={mkx[3]-mkx[2],mky[3]-mky[2],0};
  double v3toC[3]={-mkx[3],-mky[3],5};
  double h23C[3];

  //正規化前
  double tmp_x[3], tmp_y[3], tmp_z[3];
  //x軸
  vec_cp(v3to0,v0toC,h30C);
  vec_cp(v1to2,v2toC,h12C);
  vec_cp(v0to1,v1toC,h01C);
  vec_cp(v2to3,v3toC,h23C);
  
  vec_cp(h30C,h12C,tmp_x);
  vec_cp(h01C,h23C,tmp_y);
  vec_cp(tmp_x,tmp_y,tmp_z);
  mk_scale=pow(vec_abs(tmp_z), 0.15);

  /*
  //ここで、tmp_x,tmp_yが直行していない恐れがあるので無理やり直す
  vec_cp(tmp_z,tmp_x,tmp_y);
  vec_cp(tmp_x,tmp_y,tmp_z);
  vec_cp(tmp_y,tmp_z,tmp_x);
  */
  
  vec_nrm(tmp_x , mkx_in_cmr);
  vec_nrm(tmp_y , mky_in_cmr);
  vec_nrm(tmp_z , mkz_in_cmr);

  //SetMatrix
  int i;
  for(i=0;i<3;i++){
    mat_mk_cmr[0][i]=mkx_in_cmr[i];
    mat_mk_cmr[1][i]=mky_in_cmr[i];
    mat_mk_cmr[2][i]=mkz_in_cmr[i];
  }
}
void mkxyz(){
  glBegin(GL_LINES);
  glColor3f(0.0,1.0,1.0);
  glVertex3d(0.0,0.0,0.0);
  glVertex3dv(mkx_in_cmr);
  
  glColor3f(1.0,0.0,1.0);
  glVertex3d(0.0,0.0,0.0);
  glVertex3dv(mky_in_cmr);
  
  glColor3f(1.0,1.0,0.0);
  glVertex3d(0.0,0.0,0.0);
  glVertex3dv(mkz_in_cmr);
  glEnd();
}
void mkcube(){
  double l=0.3,r=-0.3,d=0.6;
  double e[6][4][3]={{{r,r, 0},
		      {r,l, 0},
		      {l,l, 0},
		      {l,r, 0}},
		     {{r,r, d},
		      {r,l, d},
		      {l,l, d},
		      {l,r, d}},
		     {{r,r, 0},
		      {r,r, d},
		      {l,r, d},
		      {l,r, 0}},
		     {{r,r, d},
		      {r,l, d},
		      {r,l, 0},
		      {r,r, 0}},
		     {{l,l, d},
		      {r,l, d},
		      {r,l, 0},
		      {l,l, 0}},
		     {{l,l, d},
		      {l,r, d},
		      {l,r, 0},
		      {l,l, 0}}};
  int i,j,k;
  double E[6][4][3];
  vec_add(pos,v,pos);
  cvt_mk2cmr(pos,pos_c);
  v[0]=v[1]=v[2]=0;
  
  for(j=0;j<6;j++){
    for(i=0;i<4;i++){
      cvt_mk2cmr(e[j][i],E[j][i]);
      for(k=0;k<3;k++){
	E[j][i][k]+=pos_c[k];
      }
    }
  }
  glBegin(GL_QUADS);
  glColor3f(0.0,1.0,1.0);
  for(i=0;i<4;i++){
    glVertex3dv(E[0][i]);
  }
  glEnd();
  
  glBegin(GL_QUADS);
  glColor3f(1.0,0.0,1.0);
  for(i=0;i<4;i++){
    glVertex3dv(E[1][i]);
  }
  glEnd();

  glBegin(GL_QUADS);
  glColor3f(0.5,1.0,1.0);
  for(i=0;i<4;i++){
    glVertex3dv(E[2][i]);
  }
  glEnd();glBegin(GL_QUADS);
  glColor3f(1.0,0.5,1.0);
  for(i=0;i<4;i++){
    glVertex3dv(E[3][i]);
  }
  glEnd();glBegin(GL_QUADS);
  glColor3f(1.0,1.0,0.5);
  for(i=0;i<4;i++){
    glVertex3dv(E[4][i]);
  }
  glEnd();glBegin(GL_QUADS);
  glColor3f(1.0,1.0,0.0);
  for(i=0;i<4;i++){
    glVertex3dv(E[5][i]);
  }
  glEnd();
}
void cvt_mk2cmr(double a[3],double b[3]){
  double tmp[3],tmp2[3];
  double ori[3]={(mkx[0]+mkx[1]+mkx[2]+mkx[3])/4.0,
		 (mky[0]+mky[1]+mky[2]+mky[3])/4.0,
		 0};
  
  double mat_scale[3][3]={{mk_scale,0,0},
			  {0,mk_scale,0},
			  {0,0,mk_scale}};
  vec_mat(a,mat_mk_cmr,tmp);
  tmp[2]*=4.0/3.0;
  vec_mat(tmp,mat_scale, tmp2);
  vec_add(tmp2,ori,b);
}

void specialkeydown(int key, int x, int y)
{
  double vel=0.05;
     if( key == GLUT_KEY_UP )//矢印「上」
     {
       v[1]+=vel;
     }

     if( key == GLUT_KEY_DOWN )//矢印「下」
     {
       v[1]+=(-1)*vel;
     }

     if( key == GLUT_KEY_LEFT )
     {
       v[0]+=(-1)*vel;
     }

     if( key == GLUT_KEY_RIGHT )
     {
       v[0]+=vel;
     }

}
