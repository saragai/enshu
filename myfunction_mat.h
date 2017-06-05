void vec_cpy(double a[3],double b[3]){
  int i;
  for(i=0;i<3;i++){
    b[i]=a[i];
  }
}
//外積
void vec_cp(double a[3],double b[3],double c[3]){
  int i;
  for(i=0; i<3; i++){
    c[i]=a[(i+1)%3] * b[(i+2)%3] - b[(i+1)%3] * a[(i+2)%3];
  }
}
//ベクトルの絶対値
double vec_abs(double a[3]){
  return sqrt(a[0]*a[0]+a[1]*a[1]+a[2]*a[2]);
}
//正規化
void vec_nrm(double a[3],double b[3]){
  double vol = vec_abs(a);
  int i;
  for(i=0;i<3;i++){
    b[i]=a[i]/vol;
  }
}
//ベクトルの足し算
void vec_add(double a[3], double b[3], double c[3]){
  int i;
  for(i=0;i<3;i++){
    c[i]=a[i]+b[i];
  }
}

//ベクトル×行列
void vec_mat(double a[3], double A[3][3], double b[3]){
  int i,j;

  for(i=0;i<3;i++){
    b[i]=0;
    for(j=0;j<3;j++){
      b[i]+=a[j]*A[j][i];
    }
  }
}
