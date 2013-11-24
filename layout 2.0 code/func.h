void init3(float v[3],int x,int y,int z){
	v[0]=x;
	v[1]=y;
	v[2]=z;
}
float len3(float v[3]){
	float len=sqrt(v[0]*v[0]+v[1]*v[1]+v[2]*v[2]);
	if(len==0)len=0.001;
	return len;
}
float len3_2(float v[3]){
	return v[0]*v[0]+v[1]*v[1]+v[2]*v[2];
}
void sub3(float a[3],float b[3],float rs[3]){
	rs[0]=a[0]-b[0];
	rs[1]=a[1]-b[1];
	rs[2]=a[2]-b[2];
}
void add3(float a[3],float b[3],float rs[3]){
	rs[0]=a[0]+b[0];
	rs[1]=a[1]+b[1];
	rs[2]=a[2]+b[2];
}
void mul3(float k,float v[3],float rs[3]){
	rs[0]=k*v[0];
	rs[1]=k*v[1];
	rs[2]=k*v[2];
}