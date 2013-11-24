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
	//引力-斥力模型
	float K;//引力常数
	float T;//当前温度
	float Jf;//前一次的目标函数值
	float decay;
	//模式
	bool dim2;//是否为二维
	vector<int> zlist;//保存顶点z值
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
	initSpace(float _xmin,float _xmax,float _ymin,float _ymax,float _zmin,float _zmax){//初始化包围盒
		xmin=_xmin;
		xmax=_xmax;
		ymin=_ymin;
		ymax=_ymax;
		zmin=_zmin;
		zmax=_zmax;
	}
	shutLayoutProc(){//关闭布局过程
		T=0;
	}
	initLayoutProc(){//初始化布局过程
		//初始化引力常数
		if(dim2){
			float area=(xmax-xmin)*(ymax-ymin);
			K=pow(area/(int)vlist.size(),0.5);
		}else{
			float volume=(xmax-xmin)*(ymax-ymin)*(zmax-zmin);
			K=pow(volume/(int)vlist.size(),0.33);
		} 
		//初始化温度
		if(dim2){
			T=(xmax-xmin);
		}else{
			T=(xmax-xmin)*sqrt(3);
		}
		//初始化目标函数值
		Jf=inf;
		//初始化衰减系数
		decay=0.9;
	}
	void setT(float _T){
		T=_T;
	}
	void setDecay(float _decay){
		decay=_decay;
	}
	void setDim2(bool _dim2){
		if(dim2&&!_dim2){//如果是2维变成3维
			dim2=_dim2;
			//恢复z值
			for(int i=0;i<=(int)vlist.size()-1;i++){
				vlist[i].v[2]=zlist[i];
			}
		}else if(!dim2&&_dim2){//如果是3维变成2维
			dim2=_dim2;
			//保持z值
			for(int i=0;i<=(int)vlist.size()-1;i++){
				zlist[i]=vlist[i].v[2];
			}
			//将vlist.v的z值都变成zmax/2
			for(i=0;i<=(int)vlist.size()-1;i++){
				vlist[i].v[2]=zmax/2;
			}
			
		}
	
	}
	void setNv(int _Nv){
		Nv=_Nv;
	}
	initGraph(bool complete){//n为顶点数，complete=true为生成完全图，否则生成随机图
		//开辟邻接矩阵
		mat.resize(Nv);
		for(int i=0;i<=Nv-1;i++){
			mat[i].resize(Nv);
		}
		//填充邻接矩阵
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
		//开辟顶点列表
		vlist.resize(Nv);
		//填充顶点列表
		for(i=0;i<=Nv-1;i++){
			float x=rand()%(int)(xmax-xmin);
			float y=rand()%(int)(ymax-ymin);
		    float z=rand()%(int)(zmax-zmin);
			init3(vlist[i].v,x,y,z);
		}
		//z值存入zlist
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
			//画平面
			//画包围盒
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
			//画包围盒
			glColor3f(1,0,0);
			glBegin(GL_LINES);
			//上面
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
			//下面
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
			//四条棱
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
		//画边
		glColor3f(0,0,1);
		glBegin(GL_LINES);
		for(int i=0;i<=(int)mat.size()-1;i++){
			for(int j=0;j<i;j++){
				if(mat[i][j]){
					//画边vlist[i].v-vlist[j].v
					glVertex3fv(vlist[i].v);
					glVertex3fv(vlist[j].v);
					
				}
			}
		}
		glEnd();
		glColor3f(1,1,1);
		//画顶点
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

	void layout(){//进行一次布局
		if(T<=1){	
			//此时布局部已完成，进行平移和缩放
			if(dim2){//如果是2维情况
				//求外接矩形
				float left=inf;
				float right=-inf;
				float up=-inf;
				float down=inf;
				for(int i=0;i<=Nv-1;i++){
					if(vlist[i].v[0]<left)left=vlist[i].v[0];
					if(vlist[i].v[0]>right)right=vlist[i].v[0];
					if(vlist[i].v[1]>up)up=vlist[i].v[1];
					if(vlist[i].v[1]<down)down=vlist[i].v[1];
				}//得到left,right,up,down
				//求外接矩形中心
				float _cx=(left+right)/2;
				float _cy=(up+down)/2;
				//求画板中心
				float cx=(xmin+xmax)/2;
				float cy=(ymin+ymax)/2;
				//由外接矩形中心向画板中心的平移向量
				float vec[2]={cx-_cx,cy-_cy};
				//按平移向量对图形进行平移，但只移若干分之一，而不一步到位，为的是动画效果
				for(i=0;i<=Nv-1;i++){
					vlist[i].v[0]+=vec[0]/10;
					vlist[i].v[1]+=vec[1]/10;
				}
				//求外接矩形的最大边心距
				float r=max((xmax-xmin)/2,(ymax-ymin)/2);
				//求边心距的最大可增加量
				float dr=min((xmax-xmin)/2-(right-left)/2,(ymax-ymin)/2-(up-down)/2);
				dr=dr-(xmax-xmin)/15;//防止顶天
				float _dr=dr/1000;//只取最大可增加量的若干分之一，为的是动画效果
				//求缩放系数
				float k=(r+dr)/r;
				//按k对图形进行缩放，
				for(i=0;i<=Nv-1;i++){
					vlist[i].v[0]=(vlist[i].v[0]-_cx)*k+_cx;
					vlist[i].v[1]=(vlist[i].v[1]-_cy)*k+_cy;
				}
			}else{//如果是3维情况
				//求外接矩形
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
				}//得到left,right,up,down,front,back
				//求外接立方体中心
				float _cx=(left+right)/2;
				float _cy=(up+down)/2;
				float _cz=(front+back)/2;
				//求空间中心
				float cx=(xmin+xmax)/2;
				float cy=(ymin+ymax)/2;
				float cz=(zmin+zmax)/2;
				//由外接立方体中心向空间中心的平移向量
				float vec[3]={cx-_cx,cy-_cy,cz-_cz};
				//按平移向量对图形进行平移，但只移若干分之一，而不一步到位，为的是动画效果
				for(i=0;i<=Nv-1;i++){
					vlist[i].v[0]+=vec[0]/10;
					vlist[i].v[1]+=vec[1]/10;
					vlist[i].v[2]+=vec[2]/10;
				}
				//求外接立方体的最大边心距
				float r=max(max((xmax-xmin)/2,(ymax-ymin)/2),(zmax-zmin)/2);
				//求边心距的最大可增加量
				float dr=min(min((xmax-xmin)/2-(right-left)/2,(ymax-ymin)/2-(up-down)/2),(zmax-zmin)/2-(front-back)/2);
				dr=dr-(xmax-xmin)/20;//防止顶天
				float _dr=dr/1000;//只取最大可增加量的若干分之一，为的是动画效果
				//求缩放系数
				float k=(r+dr)/r;
				//按k对图形进行缩放，
				for(i=0;i<=Nv-1;i++){
					vlist[i].v[0]=(vlist[i].v[0]-_cx)*k+_cx;
					vlist[i].v[1]=(vlist[i].v[1]-_cy)*k+_cy;
					vlist[i].v[2]=(vlist[i].v[2]-_cz)*k+_cz;
				}
			}
			return;
		}
		//将vlist[i]拷贝一份，以备恢复
		vector<Cvertex> _vlist=vlist;
		//将各顶点的offset恢复为0
		for(int i=0;i<=(int)vlist.size()-1;i++){
			init3(vlist[i].offset,0,0,0);
		}
		//在斥力作用下位移
		for(i=0;i<=(int)vlist.size()-1;i++){
			for(int j=0;j<i;j++){
				//计算位移向量offset和_offset
				float diff[3];
				sub3(vlist[i].v,vlist[j].v,diff);
				float len_diff=len3(diff);//|diff|
				float e_diff[3];//diff/|diff|
				mul3(1.0/len_diff,diff,e_diff);
				float offset[3];//(diff/|diff|)*fr(|diff|)
				mul3(fr(len_diff),e_diff,offset);
				float _offset[3];//-offset
				mul3(-1,offset,_offset);
				//累计位移量
				add3(vlist[i].offset,offset,vlist[i].offset);
				add3(vlist[j].offset,_offset,vlist[j].offset);
			}
		}
		//在引力作用下位移
		for(i=0;i<=(int)vlist.size()-1;i++){
			for(int j=0;j<i;j++){
				if(mat[i][j]){
					//计算位移向量offset和_offset
					float diff[3];
					sub3(vlist[i].v,vlist[j].v,diff);
					float len_diff=len3(diff);//|diff|
					float e_diff[3];//diff/|diff|
					mul3(1.0/len_diff,diff,e_diff);
					float offset[3];//(diff/|diff|)*fr(|diff|)
					mul3(fa(len_diff),e_diff,offset);
					float _offset[3];//-offset
					mul3(-1,offset,_offset);
					//累计位移量
					add3(vlist[i].offset,_offset,vlist[i].offset);
					add3(vlist[j].offset,offset,vlist[j].offset);
				}
			}
		}
		//进行位移(限制移动距离不超过T)
		for(i=0;i<=(int)vlist.size()-1;i++){
			float len_offset=len3(vlist[i].offset);//位移量的模
			float e_offset[3];//位移量的方向
			mul3(1.0/len_offset,vlist[i].offset,e_offset);//得到e_offset
			len_offset=min(len_offset,T);//得到len_offset
			float offset[3];//位移量
			mul3(len_offset,e_offset,offset);//得到offset
			add3(vlist[i].v,offset,vlist[i].v);
			if(dim2){
				vlist[i].v[2]=zmax/2;
			}
		}
		//降温
    	T=decay*T;
	}

	float fa(float d){//引力位移
		return d*d/K;
	}
	float fr(float d){//斥力位移
		return K*K/d;
	}
	



};