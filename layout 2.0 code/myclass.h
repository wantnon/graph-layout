class Cvertex{
public:
	float v[3];
	float offset[3];
	Cvertex(){
		init3(v,0,0,0);
		init3(offset,0,0,0);
	}

};
class Cgraph{
public:
	int Nv;
	float xmin,xmax,ymin,ymax,zmin,zmax;
	vector<vector<bool> > mat;
	vector<Cvertex> vlist;
	//����-����ģ��
	float K;//��������
	float T;//��ǰ�¶�
	float Jf;//ǰһ�ε�Ŀ�꺯��ֵ
	float decay;
	//ģʽ
	bool dim2;//�Ƿ�Ϊ��ά
	vector<int> zlist;//���涥��zֵ
	Cgraph(){
		xmin=0;
		xmax=0;
		ymin=0;
		ymax=0;
		zmin=0;
		zmax=0;
		dim2=true;
		decay=0.85;
		Nv=0;
	}
	initSpace(float _xmin,float _xmax,float _ymin,float _ymax,float _zmin,float _zmax){//��ʼ����Χ��
		xmin=_xmin;
		xmax=_xmax;
		ymin=_ymin;
		ymax=_ymax;
		zmin=_zmin;
		zmax=_zmax;
	}
	shutLayoutProc(){//�رղ��ֹ���
		T=0;
	}
	initLayoutProc(){//��ʼ�����ֹ���
		//��ʼ����������
		if(dim2){
			float area=(xmax-xmin)*(ymax-ymin);
			K=pow(area/(int)vlist.size(),0.5);
		}else{
			float volume=(xmax-xmin)*(ymax-ymin)*(zmax-zmin);
			K=pow(volume/(int)vlist.size(),0.33);
		} 
		//��ʼ���¶�
		if(dim2){
			T=(xmax-xmin);
		}else{
			T=(xmax-xmin)*sqrt(3);
		}
		//��ʼ��Ŀ�꺯��ֵ
		Jf=inf;
		//��ʼ��˥��ϵ��
		decay=0.9;
	}
	void setT(float _T){
		T=_T;
	}
	void setDecay(float _decay){
		decay=_decay;
	}
	void setDim2(bool _dim2){
		if(dim2&&!_dim2){//�����2ά���3ά
			dim2=_dim2;
			//�ָ�zֵ
			for(int i=0;i<=(int)vlist.size()-1;i++){
				vlist[i].v[2]=zlist[i];
			}
		}else if(!dim2&&_dim2){//�����3ά���2ά
			dim2=_dim2;
			//����zֵ
			for(int i=0;i<=(int)vlist.size()-1;i++){
				zlist[i]=vlist[i].v[2];
			}
			//��vlist.v��zֵ�����zmax/2
			for(i=0;i<=(int)vlist.size()-1;i++){
				vlist[i].v[2]=zmax/2;
			}
			
		}
	
	}
	void setNv(int _Nv){
		Nv=_Nv;
	}
	initGraph(bool complete){//nΪ��������complete=trueΪ������ȫͼ�������������ͼ
		//�����ڽӾ���
		mat.resize(Nv);
		for(int i=0;i<=Nv-1;i++){
			mat[i].resize(Nv);
		}
		//����ڽӾ���
		for(i=0;i<=Nv-1;i++){
			for(int j=0;j<=i;j++){
				if(i==j){
					mat[i][j]=0;
				}else{
					if(complete)mat[i][j]=1;
					else mat[i][j]=rand()%2;
					mat[j][i]=mat[i][j];
				}
			}
		}
		//���ٶ����б�
		vlist.resize(Nv);
		//��䶥���б�
		for(i=0;i<=Nv-1;i++){
			float x=rand()%(int)(xmax-xmin);
			float y=rand()%(int)(ymax-ymin);
		    float z=rand()%(int)(zmax-zmin);
			init3(vlist[i].v,x,y,z);
		}
		//zֵ����zlist
		zlist.resize(Nv);
		for(i=0;i<=Nv-1;i++){
			zlist[i]=vlist[i].v[2];
		}
		if(dim2){
			for(i=0;i<=Nv-1;i++){
				vlist[i].v[2]=zmax/2;
			}
		}
	}
	void show(){
		if(dim2){
			//��ƽ��
			//����Χ��
			glColor3f(1,0,0);
			glBegin(GL_LINES);
			//   p0--p3
			//   |    |
			//   p1--p2
			float p[4][4];
			init3(p[0],xmin,ymax,zmax/2);
			init3(p[1],xmin,ymin,zmax/2);
			init3(p[2],xmax,ymin,zmax/2);
			init3(p[3],xmax,ymax,zmax/2);
			
			glVertex3fv(p[0]);
			glVertex3fv(p[1]);
			
			glVertex3fv(p[1]);
			glVertex3fv(p[2]);
			
			glVertex3fv(p[2]);
			glVertex3fv(p[3]);
			
			glVertex3fv(p[3]);
			glVertex3fv(p[0]);

			glEnd();
			glColor3f(1,1,1);
		}else{
			//����Χ��
			glColor3f(1,0,0);
			glBegin(GL_LINES);
			//����
			//   p0--p3
			//   |    |
			//   p1--p2
			float p[4][4];
			init3(p[0],xmin,ymax,zmin);
			init3(p[1],xmin,ymax,zmax);
			init3(p[2],xmax,ymax,zmax);
			init3(p[3],xmax,ymax,zmin);
			
			glVertex3fv(p[0]);
			glVertex3fv(p[1]);
			
			glVertex3fv(p[1]);
			glVertex3fv(p[2]);
			
			glVertex3fv(p[2]);
			glVertex3fv(p[3]);
			
			glVertex3fv(p[3]);
			glVertex3fv(p[0]);
			//����
			//   q0--q3
			//   |    |
			//   q1--q2
			float q[4][4];
			init3(q[0],xmin,ymin,zmin);
			init3(q[1],xmin,ymin,zmax);
			init3(q[2],xmax,ymin,zmax);
			init3(q[3],xmax,ymin,zmin);
			
			glVertex3fv(q[0]);
			glVertex3fv(q[1]);
			
			glVertex3fv(q[1]);
			glVertex3fv(q[2]);
			
			glVertex3fv(q[2]);
			glVertex3fv(q[3]);
			
			glVertex3fv(q[3]);
			glVertex3fv(q[0]);
			//������
			glVertex3fv(p[0]);
			glVertex3fv(q[0]);
			
			glVertex3fv(p[1]);
			glVertex3fv(q[1]);
			
			glVertex3fv(p[2]);
			glVertex3fv(q[2]);
			
			glVertex3fv(p[3]);
			glVertex3fv(q[3]);
			
			glEnd();
			glColor3f(1,1,1);
		}
		//����
		glColor3f(0,0,1);
		glBegin(GL_LINES);
		for(int i=0;i<=(int)mat.size()-1;i++){
			for(int j=0;j<i;j++){
				if(mat[i][j]){
					//����vlist[i].v-vlist[j].v
					glVertex3fv(vlist[i].v);
					glVertex3fv(vlist[j].v);
					
				}
			}
		}
		glEnd();
		glColor3f(1,1,1);
		//������
		glColor3f(0,1,0);
		glPointSize(5);
		glBegin(GL_POINTS);
		for(i=0;i<=(int)vlist.size()-1;i++){
			glVertex3fv(vlist[i].v);
		}
		glEnd();
		glPointSize(1);
		glColor3f(1,1,1);
	}

	void layout(){//����һ�β���
		if(T<=1){	
			//��ʱ���ֲ�����ɣ�����ƽ�ƺ�����
			if(dim2){//�����2ά���
				//����Ӿ���
				float left=inf;
				float right=-inf;
				float up=-inf;
				float down=inf;
				for(int i=0;i<=Nv-1;i++){
					if(vlist[i].v[0]<left)left=vlist[i].v[0];
					if(vlist[i].v[0]>right)right=vlist[i].v[0];
					if(vlist[i].v[1]>up)up=vlist[i].v[1];
					if(vlist[i].v[1]<down)down=vlist[i].v[1];
				}//�õ�left,right,up,down
				//����Ӿ�������
				float _cx=(left+right)/2;
				float _cy=(up+down)/2;
				//�󻭰�����
				float cx=(xmin+xmax)/2;
				float cy=(ymin+ymax)/2;
				//����Ӿ��������򻭰����ĵ�ƽ������
				float vec[2]={cx-_cx,cy-_cy};
				//��ƽ��������ͼ�ν���ƽ�ƣ���ֻ�����ɷ�֮һ������һ����λ��Ϊ���Ƕ���Ч��
				for(i=0;i<=Nv-1;i++){
					vlist[i].v[0]+=vec[0]/10;
					vlist[i].v[1]+=vec[1]/10;
				}
				//����Ӿ��ε������ľ�
				float r=max((xmax-xmin)/2,(ymax-ymin)/2);
				//����ľ������������
				float dr=min((xmax-xmin)/2-(right-left)/2,(ymax-ymin)/2-(up-down)/2);
				dr=dr-(xmax-xmin)/15;//��ֹ����
				float _dr=dr/1000;//ֻȡ���������������ɷ�֮һ��Ϊ���Ƕ���Ч��
				//������ϵ��
				float k=(r+dr)/r;
				//��k��ͼ�ν������ţ�
				for(i=0;i<=Nv-1;i++){
					vlist[i].v[0]=(vlist[i].v[0]-_cx)*k+_cx;
					vlist[i].v[1]=(vlist[i].v[1]-_cy)*k+_cy;
				}
			}else{//�����3ά���
				//����Ӿ���
				float left=inf;
				float right=-inf;
				float up=-inf;
				float down=inf;
				float front=-inf;
				float back=inf;
				for(int i=0;i<=Nv-1;i++){
					if(vlist[i].v[0]<left)left=vlist[i].v[0];
					if(vlist[i].v[0]>right)right=vlist[i].v[0];
					if(vlist[i].v[1]>up)up=vlist[i].v[1];
					if(vlist[i].v[1]<down)down=vlist[i].v[1];
					if(vlist[i].v[2]>front)front=vlist[i].v[2];
					if(vlist[i].v[2]<back)back=vlist[i].v[2];
				}//�õ�left,right,up,down,front,back
				//���������������
				float _cx=(left+right)/2;
				float _cy=(up+down)/2;
				float _cz=(front+back)/2;
				//��ռ�����
				float cx=(xmin+xmax)/2;
				float cy=(ymin+ymax)/2;
				float cz=(zmin+zmax)/2;
				//�����������������ռ����ĵ�ƽ������
				float vec[3]={cx-_cx,cy-_cy,cz-_cz};
				//��ƽ��������ͼ�ν���ƽ�ƣ���ֻ�����ɷ�֮һ������һ����λ��Ϊ���Ƕ���Ч��
				for(i=0;i<=Nv-1;i++){
					vlist[i].v[0]+=vec[0]/10;
					vlist[i].v[1]+=vec[1]/10;
					vlist[i].v[2]+=vec[2]/10;
				}
				//�����������������ľ�
				float r=max(max((xmax-xmin)/2,(ymax-ymin)/2),(zmax-zmin)/2);
				//����ľ������������
				float dr=min(min((xmax-xmin)/2-(right-left)/2,(ymax-ymin)/2-(up-down)/2),(zmax-zmin)/2-(front-back)/2);
				dr=dr-(xmax-xmin)/20;//��ֹ����
				float _dr=dr/1000;//ֻȡ���������������ɷ�֮һ��Ϊ���Ƕ���Ч��
				//������ϵ��
				float k=(r+dr)/r;
				//��k��ͼ�ν������ţ�
				for(i=0;i<=Nv-1;i++){
					vlist[i].v[0]=(vlist[i].v[0]-_cx)*k+_cx;
					vlist[i].v[1]=(vlist[i].v[1]-_cy)*k+_cy;
					vlist[i].v[2]=(vlist[i].v[2]-_cz)*k+_cz;
				}
			}
			return;
		}
		//��vlist[i]����һ�ݣ��Ա��ָ�
		vector<Cvertex> _vlist=vlist;
		//���������offset�ָ�Ϊ0
		for(int i=0;i<=(int)vlist.size()-1;i++){
			init3(vlist[i].offset,0,0,0);
		}
		//�ڳ���������λ��
		for(i=0;i<=(int)vlist.size()-1;i++){
			for(int j=0;j<i;j++){
				//����λ������offset��_offset
				float diff[3];
				sub3(vlist[i].v,vlist[j].v,diff);
				float len_diff=len3(diff);//|diff|
				float e_diff[3];//diff/|diff|
				mul3(1.0/len_diff,diff,e_diff);
				float offset[3];//(diff/|diff|)*fr(|diff|)
				mul3(fr(len_diff),e_diff,offset);
				float _offset[3];//-offset
				mul3(-1,offset,_offset);
				//�ۼ�λ����
				add3(vlist[i].offset,offset,vlist[i].offset);
				add3(vlist[j].offset,_offset,vlist[j].offset);
			}
		}
		//������������λ��
		for(i=0;i<=(int)vlist.size()-1;i++){
			for(int j=0;j<i;j++){
				if(mat[i][j]){
					//����λ������offset��_offset
					float diff[3];
					sub3(vlist[i].v,vlist[j].v,diff);
					float len_diff=len3(diff);//|diff|
					float e_diff[3];//diff/|diff|
					mul3(1.0/len_diff,diff,e_diff);
					float offset[3];//(diff/|diff|)*fr(|diff|)
					mul3(fa(len_diff),e_diff,offset);
					float _offset[3];//-offset
					mul3(-1,offset,_offset);
					//�ۼ�λ����
					add3(vlist[i].offset,_offset,vlist[i].offset);
					add3(vlist[j].offset,offset,vlist[j].offset);
				}
			}
		}
		//����λ��(�����ƶ����벻����T)
		for(i=0;i<=(int)vlist.size()-1;i++){
			float len_offset=len3(vlist[i].offset);//λ������ģ
			float e_offset[3];//λ�����ķ���
			mul3(1.0/len_offset,vlist[i].offset,e_offset);//�õ�e_offset
			len_offset=min(len_offset,T);//�õ�len_offset
			float offset[3];//λ����
			mul3(len_offset,e_offset,offset);//�õ�offset
			add3(vlist[i].v,offset,vlist[i].v);
			if(dim2){
				vlist[i].v[2]=zmax/2;
			}
		}
		//����
    	T=decay*T;
	}

	float fa(float d){//����λ��
		return d*d/K;
	}
	float fr(float d){//����λ��
		return K*K/d;
	}
	



};