// mainDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GLmfc.h"
#include "mainDlg.h"

using namespace std;
#include<vector>
#include<string>
#pragma comment (lib,"glaux.lib")
#include <stdlib.h>
#include <windows.h>
#include <gl\gl.h>
#include <gl\glu.h>
#include <gl\glut.h>
#include <gl\glaux.h>
#include <math.h>
//------------------------------全局变量
//常数
const double inf=100000000;
const float pi=3.1415926;
//2D坐标风格
#define SCREEN_COOR 0
#define MATH_COOR 1
//timer的时间间隔
int timerspan=100;
//旋转视角
float A=0;//水平旋转角(与Z轴夹角，由Z转向X为正角)
float dA=10*pi/180;//A的增量
float B=0;//竖直旋转角(与XZ平面夹角)
float dB=10*pi/180;//B的增量
//视口尺寸
float width;
float height;
//相机位置
float xx;
float yy;
float zz;
//相机到世界坐标原点的距离
float r;
//鼠标
float mosx=0;
float mosy=0;
float mosxf=0;
float mosyf=0;
bool leftDown=false;
bool rightDown=false;
//
#include"text.h"
#include"func.h"
#include"myclass.h"
//图
Cgraph graph;
#include"setNvDlg.h"
//---------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
// 设置像素格式函数
int CmainDlg::MySetPixelFormat(HDC hdc)
{
	PIXELFORMATDESCRIPTOR pfd = { 
	    sizeof(PIXELFORMATDESCRIPTOR),    // pfd结构的大小 
	    1,                                // 版本号 
	    PFD_DRAW_TO_WINDOW |              // 支持在窗口中绘图 
	    PFD_SUPPORT_OPENGL |              // 支持 OpenGL 
	    PFD_DOUBLEBUFFER,                 // 双缓存模式 
	    PFD_TYPE_RGBA,                    // RGBA 颜色模式 
	    24,                               // 24 位颜色深度 
	    0, 0, 0, 0, 0, 0,                 // 忽略颜色位 
	    0,                                // 没有非透明度缓存 
	    0,                                // 忽略移位位 
	    0,                                // 无累加缓存 
	    0, 0, 0, 0,                       // 忽略累加位 
	    32,                               // 32 位深度缓存     
	    0,                                // 无模板缓存 
	    0,                                // 无辅助缓存 
	    PFD_MAIN_PLANE,                   // 主层 
	    0,                                // 保留 
	    0, 0, 0                           // 忽略层,可见性和损毁掩模 
	}; 
	
	int  iPixelFormat; 
 
	// 为设备描述表得到最匹配的像素格式 
	if((iPixelFormat = ChoosePixelFormat(hdc, &pfd)) == 0)
	{
		MessageBox("ChoosePixelFormat Failed", NULL, MB_OK);
		return 0;
	}
	 
	// 设置最匹配的像素格式为当前的像素格式 
	if(SetPixelFormat(hdc, iPixelFormat, &pfd) == FALSE)
	{
		MessageBox("SetPixelFormat Failed", NULL, MB_OK);
		return 0;
	}

	return 1;
}
/////////////////////////////////////////////////////////////////////////////
// CmainDlg dialog


CmainDlg::CmainDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CmainDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CmainDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CmainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CmainDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CmainDlg, CDialog)
	//{{AFX_MSG_MAP(CmainDlg)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_KEYDOWN()
	ON_COMMAND(ID_about, Onabout)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEWHEEL()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_COMMAND(ID_instruction, Oninstruction)
	ON_COMMAND(ID_rand, Onrand)
	ON_COMMAND(ID_complete, Oncomplete)
	ON_COMMAND(ID_control, Oncontrol)
	ON_COMMAND(ID_dim2, Ondim2)
	ON_COMMAND(ID_setNv, OnsetNv)
	ON_COMMAND(ID_layout_hot, Onlayouthot)
	ON_COMMAND(ID_layout_warm, Onlayoutwarm)
	ON_COMMAND(ID_layout_cool, Onlayoutcool)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CmainDlg message handlers

int CmainDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	// 设置当前的绘图像素格式
	MySetPixelFormat(::GetDC(m_hWnd));
	// 获得绘图描述表
	hdc = ::GetDC(m_hWnd);
	// 创建渲染描述表
	hglrc = wglCreateContext(hdc);
	// 使绘图描述表为当前调用现程的当前绘图描述表 
	wglMakeCurrent(hdc, hglrc);	
	////-----------------------------------------------------------
	this->SetTimer( 1,timerspan,NULL);
	//select clearing(background) color
	glClearColor(0.0,0.0,0.0,0.0);
	//initialize viewing values
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glEnable(GL_DEPTH_TEST);
	//-------------------------------------------光照-------------------------------------------------------------------------- 
	//----选择明暗技术
	//可以用GL_FLAT或GL_SMOOTH
	glShadeModel(GL_FLAT );
	//----设置材质
	//可以对不同的面GL_FRONT,GL_BACK,GL_FRONT_AND_BACK设置如下项目：
	//GL_AMBIENT(环境颜色),GL_DIFFUSE(散射颜色),GL_AMBIENT_AND_DIFFUSE,GL_SPECULAR(镜面颜色),GL_SHININESS(镜面指数),
	//GL_EMISSION,GL_COLOR_INDEXES
	//这里只对GL_SPECULAR和GL_SHININESS作了设置，其余用默认值
	GLfloat mat_specular[]={0,0,0,1};
	glMaterialfv(GL_FRONT,GL_SPECULAR,mat_specular);//镜面颜色
	GLfloat mat_shininess[]={10};
	glMaterialfv(GL_FRONT,GL_SHININESS,mat_shininess);//镜面指数
	//----设置光源LIGHT0
	//共有8个光源LIGHT0~LIGHT7
	//设置项有GL_AMBIENT,GL_DIFFUSE,GL_SPECULAR,GL_POSITION,GL_SPOT_DIRECTION,GL_SPOT_EXPONENT,GL_SPOT_CUTOFF,
	//GL_CONSTANT_ATTENUATION,GL_LINEAR_ATTENUATION,GL_QUADRATIC_ATTENUATION
	//这里只对GL_DIFFUSE和GL_SPECULAR进行设置，其余用默认值
	GLfloat white_light[]={1,1,1,1};
    glLightfv(GL_LIGHT0,GL_DIFFUSE,white_light);//漫反射
	glLightfv(GL_LIGHT0,GL_SPECULAR,white_light);//镜面光
	//----设置光照模型
	//包括GL_LIGHT_MODEL_AMBIENT,GL_LIGHT_MODEL_LOCAL_VIEWR,GL_LIGHT_MODEL_TWO_SIDE,GL_LIGHT_MODEL_COLOR_CONTROL
	//这里只对GL_LIGHT_MODEL_AMBIENT进行一下设置，其余用默认值
	GLfloat lmodel_ambient[]={0.5,0.5,0.5,1.0};
   	glLightModelfv(GL_LIGHT_MODEL_AMBIENT,lmodel_ambient);
	//----启用
	glEnable(GL_LIGHTING);//开启光照
	glEnable(GL_LIGHT0);//开启LIGHT0
	glEnable(GL_NORMALIZE);//开启法向量单位化
	glEnable(GL_COLOR_MATERIAL);//开启颜色材质模式(使颜色与材质自动混合)
	//--------------------------------------------------------------------------------------------------------------------------	
	//纹理
	glEnable(GL_TEXTURE_2D);
	glClearDepth(1.0f);
	glDepthFunc(GL_LEQUAL);
	//获得视口大小
	GLint viewport[4];//4个元素依次为x,y,width,height
	glGetIntegerv(GL_VIEWPORT,viewport);
	width=viewport[2];
	height=viewport[3];
	//随机数
	srand(time(0));
	//----------------------
	r=400;
	graph.initSpace(0,200,0,200,0,200);
	graph.setNv(10);
	graph.setDim2(true);
	graph.initGraph(true);

	return 0;
}


void CmainDlg::draw(){
	//clear all pixels
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); 
	//----对投影矩阵的操作-----------
    glMatrixMode(GL_PROJECTION);//切换到投影模式
    glLoadIdentity();//归一化
	//设置视锥
    gluPerspective(45,//视角
		width*1.0/height, //视口横纵比
		0.1,//近截面
		5000);//远截面 
	//----对 模型-视图 堆栈的操作----
	glMatrixMode(GL_MODELVIEW);//切换到 模型-视图 模式
	glLoadIdentity();//归一化

	//计算关注点
	float cx=(graph.xmax-graph.xmin)/2;
	float cy=(graph.ymax-graph.ymin)/2;
	float cz=(graph.zmax-graph.zmin)/2;
	//计算相机位置
	xx=r*cos(B)*sin(A)+cx;
	yy=r*sin(B)+cy;
	zz=r*cos(B)*cos(A)+cz;	
	//用gluLookAt设置栈底视图矩阵

    gluLookAt(xx,yy,zz,//相机位置
	    cx,cy,cz,//关注点
		0,1,0);//向机向上方向
	GLfloat light_position[]={10,30,30,0};
	glLightfv(GL_LIGHT0,GL_POSITION, light_position);
	//开始绘图和变换
	graph.show();
	//2d文字
	Begin2D(SCREEN_COOR);
	float x=width-120;
	float y=30;
	//模式(2d/3d)
	if(graph.dim2){	
		Text("mode: 2d",x,y);
	}else{
		Text("mode: 3d",x,y);
	}
	//顶点数
	y+=20;
	CString str;
	str.Format("vertex: %d",graph.Nv);
	Text(str,x,y);

	End2D();

		
	
	
}
void CmainDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	draw();
	//
    SwapBuffers(hdc);
	
	// Do not call CDialog::OnPaint() for painting messages
}

void CmainDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	graph.layout();
	//------------------------
	this->Invalidate (false);//重画
	//
	CDialog::OnTimer(nIDEvent);
}

void CmainDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
}

BOOL CmainDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN)
	{
		UINT nChar = (UINT)pMsg->wParam;
		//测试键值
	//	CString str;str.Format("%d",nChar);MessageBox(str);
		switch (nChar)
		{
		case 38://上
			break;
		case 40://下
			break;
		case 37://左
			break;
		case 39://右
			break;
		case 65://a
			break;
		case 80://p
			break;
		case 83://s
			break;
		case 70://f
			break;
		case 69://e
			break;
		case 68://d
			break;
		case 72://h
			break;
		case 86://v
			break;
		case 66://b
			break;
		case 87://w
			break;
		case 35:
			break;
		case 32:
			break;
		case 82:
			break;
		case 34:
			break;
		case 81:
			break;
		case 93:
			break;
		}
	}
	//注意，此处用为并列if，不能用if-elseif
	if(pMsg->message == WM_KEYUP){
		UINT nChar = (UINT)pMsg->wParam;
		switch (nChar)
		{
		case 38://上
			break;
		case 40://下
			break;
		case 37://左
			break;
		case 39://右
			break;
		case 83://s
			break;
		case 66://b
			break;
		case 65:
			break;
		case 70://f
			break;
		case 69://e
			break;
		case 68://d
			break;
		case 87://w
			break;
		case 35:
			break;
		case 32:
			break;
		case 82:
			break;
		case 34:
			break;
		case 81:
			break;
		case 93:
			break;
		case 72://h
			break;
		}
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}
void CmainDlg::Onabout() 
{
	// TODO: Add your command handler code here
	string str;
    str+="    layout 2.0  \r\n";
	str+="    by wantnon  \r\n";
	str+="    2011-6-1   \r\n";
	this->KillTimer(1);//暂停
	MessageBox(str.c_str(),"关于 layout");
	this->SetTimer( 1,timerspan,NULL);//恢复
	
}

void CmainDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	leftDown=true;
	mosx=point.x;
	mosy=point.y;
	mosxf=point.x;
	mosyf=point.y;
	
	CDialog::OnLButtonDown(nFlags, point);
}

void CmainDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(leftDown){
		mosxf=mosx;
		mosyf=mosy;
		mosx=point.x;
		mosy=point.y;
		A-=(mosx-mosxf)*0.01;
		//将A变换到[0,2*pi)
		if(A>=360*pi/180)A-=360*pi/180;
		if(A<0)A+=360*pi/180;
		//
		B+=(mosy-mosyf)*0.01;
		if(fabs(B-pi/2)<=0.1||fabs(B-(-pi/2))<=0.1){
			B-=(mosy-mosyf)*0.01;
		}
	}


	CDialog::OnMouseMove(nFlags, point);
}

void CmainDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	leftDown=false;

	CDialog::OnLButtonUp(nFlags, point);
}

BOOL CmainDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	// TODO: Add your message handler code here and/or call default
	if(zDelta<0){
		r+=20;
	}else{
		r-=20;
	}
	return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}

void CmainDlg::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	rightDown=true;
	CDialog::OnRButtonDown(nFlags, point);
}

void CmainDlg::OnRButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	rightDown=false;
	
	CDialog::OnRButtonUp(nFlags, point);
}

void CmainDlg::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnLButtonDblClk(nFlags, point);
}

void CmainDlg::Oninstruction() 
{
	// TODO: Add your command handler code here
	ShellExecute( 0,"open","说明.txt",0,0,SW_SHOW);
}

void CmainDlg::Onrand() 
{
	// TODO: Add your command handler code here
	graph.initGraph(false);
	graph.shutLayoutProc();
		
}

void CmainDlg::Oncomplete() 
{
	// TODO: Add your command handler code here
	graph.initGraph(true);
	graph.shutLayoutProc();
	
}


void CmainDlg::Oncontrol() 
{
	// TODO: Add your command handler code here
	string str;
    str+="    鼠标拖动: 旋转   \r\n";
	str+="    鼠标滚轮: 缩放   \r\n";
	this->KillTimer(1);//暂停
	MessageBox(str.c_str(),"控制");
	this->SetTimer( 1,timerspan,NULL);//恢复
	
}

void CmainDlg::Ondim2() 
{
	// TODO: Add your command handler code here
	graph.setDim2(!graph.dim2);
	
}



void CmainDlg::OnsetNv() 
{
	// TODO: Add your command handler code here
	CsetNvDlg setNvDlg;
	int Nvf=graph.Nv;//保存旧Nv值
	setNvDlg.m_Nv=graph.Nv;
	if(setNvDlg.DoModal()==IDOK){
		int Nv=setNvDlg.m_Nv;
		if(Nv!=Nvf){//有变则更新
			graph.setNv(Nv);
			graph.initGraph(true);
		}
	}


	
}

void CmainDlg::Onlayouthot() 
{
	// TODO: Add your command handler code here
	graph.initLayoutProc();
	
}

void CmainDlg::Onlayoutwarm() 
{
	// TODO: Add your command handler code here
	graph.initLayoutProc();
	float _T;
	if(graph.dim2){
		_T=(graph.xmax-graph.xmin)/6;
	}else{
		_T=(graph.xmax-graph.xmin)*sqrt(3)/6;
	}
	graph.setDecay(0.9);
	graph.setT(_T);
}

void CmainDlg::Onlayoutcool() 
{
	// TODO: Add your command handler code here
	graph.initLayoutProc();
	float _T;
	if(graph.dim2){
		_T=(graph.xmax-graph.xmin)/14;
	}else{
		_T=(graph.xmax-graph.xmin)*sqrt(3)/14;
	}
	graph.setDecay(0.9);
	graph.setT(_T);
	
}
