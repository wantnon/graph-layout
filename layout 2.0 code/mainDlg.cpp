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
//------------------------------ȫ�ֱ���
//����
const double inf=100000000;
const float pi=3.1415926;
//2D������
#define SCREEN_COOR 0
#define MATH_COOR 1
//timer��ʱ����
int timerspan=100;
//��ת�ӽ�
float A=0;//ˮƽ��ת��(��Z��нǣ���Zת��XΪ����)
float dA=10*pi/180;//A������
float B=0;//��ֱ��ת��(��XZƽ��н�)
float dB=10*pi/180;//B������
//�ӿڳߴ�
float width;
float height;
//���λ��
float xx;
float yy;
float zz;
//�������������ԭ��ľ���
float r;
//���
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
//ͼ
Cgraph graph;
#include"setNvDlg.h"
//---------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
// �������ظ�ʽ����
int CmainDlg::MySetPixelFormat(HDC hdc)
{
	PIXELFORMATDESCRIPTOR pfd = { 
	    sizeof(PIXELFORMATDESCRIPTOR),    // pfd�ṹ�Ĵ�С 
	    1,                                // �汾�� 
	    PFD_DRAW_TO_WINDOW |              // ֧���ڴ����л�ͼ 
	    PFD_SUPPORT_OPENGL |              // ֧�� OpenGL 
	    PFD_DOUBLEBUFFER,                 // ˫����ģʽ 
	    PFD_TYPE_RGBA,                    // RGBA ��ɫģʽ 
	    24,                               // 24 λ��ɫ��� 
	    0, 0, 0, 0, 0, 0,                 // ������ɫλ 
	    0,                                // û�з�͸���Ȼ��� 
	    0,                                // ������λλ 
	    0,                                // ���ۼӻ��� 
	    0, 0, 0, 0,                       // �����ۼ�λ 
	    32,                               // 32 λ��Ȼ���     
	    0,                                // ��ģ�建�� 
	    0,                                // �޸������� 
	    PFD_MAIN_PLANE,                   // ���� 
	    0,                                // ���� 
	    0, 0, 0                           // ���Բ�,�ɼ��Ժ������ģ 
	}; 
	
	int  iPixelFormat; 
 
	// Ϊ�豸������õ���ƥ������ظ�ʽ 
	if((iPixelFormat = ChoosePixelFormat(hdc, &pfd)) == 0)
	{
		MessageBox("ChoosePixelFormat Failed", NULL, MB_OK);
		return 0;
	}
	 
	// ������ƥ������ظ�ʽΪ��ǰ�����ظ�ʽ 
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
	// ���õ�ǰ�Ļ�ͼ���ظ�ʽ
	MySetPixelFormat(::GetDC(m_hWnd));
	// ��û�ͼ������
	hdc = ::GetDC(m_hWnd);
	// ������Ⱦ������
	hglrc = wglCreateContext(hdc);
	// ʹ��ͼ������Ϊ��ǰ�����̵ֳĵ�ǰ��ͼ������ 
	wglMakeCurrent(hdc, hglrc);	
	////-----------------------------------------------------------
	this->SetTimer( 1,timerspan,NULL);
	//select clearing(background) color
	glClearColor(0.0,0.0,0.0,0.0);
	//initialize viewing values
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glEnable(GL_DEPTH_TEST);
	//-------------------------------------------����-------------------------------------------------------------------------- 
	//----ѡ����������
	//������GL_FLAT��GL_SMOOTH
	glShadeModel(GL_FLAT );
	//----���ò���
	//���ԶԲ�ͬ����GL_FRONT,GL_BACK,GL_FRONT_AND_BACK����������Ŀ��
	//GL_AMBIENT(������ɫ),GL_DIFFUSE(ɢ����ɫ),GL_AMBIENT_AND_DIFFUSE,GL_SPECULAR(������ɫ),GL_SHININESS(����ָ��),
	//GL_EMISSION,GL_COLOR_INDEXES
	//����ֻ��GL_SPECULAR��GL_SHININESS�������ã�������Ĭ��ֵ
	GLfloat mat_specular[]={0,0,0,1};
	glMaterialfv(GL_FRONT,GL_SPECULAR,mat_specular);//������ɫ
	GLfloat mat_shininess[]={10};
	glMaterialfv(GL_FRONT,GL_SHININESS,mat_shininess);//����ָ��
	//----���ù�ԴLIGHT0
	//����8����ԴLIGHT0~LIGHT7
	//��������GL_AMBIENT,GL_DIFFUSE,GL_SPECULAR,GL_POSITION,GL_SPOT_DIRECTION,GL_SPOT_EXPONENT,GL_SPOT_CUTOFF,
	//GL_CONSTANT_ATTENUATION,GL_LINEAR_ATTENUATION,GL_QUADRATIC_ATTENUATION
	//����ֻ��GL_DIFFUSE��GL_SPECULAR�������ã�������Ĭ��ֵ
	GLfloat white_light[]={1,1,1,1};
    glLightfv(GL_LIGHT0,GL_DIFFUSE,white_light);//������
	glLightfv(GL_LIGHT0,GL_SPECULAR,white_light);//�����
	//----���ù���ģ��
	//����GL_LIGHT_MODEL_AMBIENT,GL_LIGHT_MODEL_LOCAL_VIEWR,GL_LIGHT_MODEL_TWO_SIDE,GL_LIGHT_MODEL_COLOR_CONTROL
	//����ֻ��GL_LIGHT_MODEL_AMBIENT����һ�����ã�������Ĭ��ֵ
	GLfloat lmodel_ambient[]={0.5,0.5,0.5,1.0};
   	glLightModelfv(GL_LIGHT_MODEL_AMBIENT,lmodel_ambient);
	//----����
	glEnable(GL_LIGHTING);//��������
	glEnable(GL_LIGHT0);//����LIGHT0
	glEnable(GL_NORMALIZE);//������������λ��
	glEnable(GL_COLOR_MATERIAL);//������ɫ����ģʽ(ʹ��ɫ������Զ����)
	//--------------------------------------------------------------------------------------------------------------------------	
	//����
	glEnable(GL_TEXTURE_2D);
	glClearDepth(1.0f);
	glDepthFunc(GL_LEQUAL);
	//����ӿڴ�С
	GLint viewport[4];//4��Ԫ������Ϊx,y,width,height
	glGetIntegerv(GL_VIEWPORT,viewport);
	width=viewport[2];
	height=viewport[3];
	//�����
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
	//----��ͶӰ����Ĳ���-----------
    glMatrixMode(GL_PROJECTION);//�л���ͶӰģʽ
    glLoadIdentity();//��һ��
	//������׶
    gluPerspective(45,//�ӽ�
		width*1.0/height, //�ӿں��ݱ�
		0.1,//������
		5000);//Զ���� 
	//----�� ģ��-��ͼ ��ջ�Ĳ���----
	glMatrixMode(GL_MODELVIEW);//�л��� ģ��-��ͼ ģʽ
	glLoadIdentity();//��һ��

	//�����ע��
	float cx=(graph.xmax-graph.xmin)/2;
	float cy=(graph.ymax-graph.ymin)/2;
	float cz=(graph.zmax-graph.zmin)/2;
	//�������λ��
	xx=r*cos(B)*sin(A)+cx;
	yy=r*sin(B)+cy;
	zz=r*cos(B)*cos(A)+cz;	
	//��gluLookAt����ջ����ͼ����

    gluLookAt(xx,yy,zz,//���λ��
	    cx,cy,cz,//��ע��
		0,1,0);//������Ϸ���
	GLfloat light_position[]={10,30,30,0};
	glLightfv(GL_LIGHT0,GL_POSITION, light_position);
	//��ʼ��ͼ�ͱ任
	graph.show();
	//2d����
	Begin2D(SCREEN_COOR);
	float x=width-120;
	float y=30;
	//ģʽ(2d/3d)
	if(graph.dim2){	
		Text("mode: 2d",x,y);
	}else{
		Text("mode: 3d",x,y);
	}
	//������
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
	this->Invalidate (false);//�ػ�
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
		//���Լ�ֵ
	//	CString str;str.Format("%d",nChar);MessageBox(str);
		switch (nChar)
		{
		case 38://��
			break;
		case 40://��
			break;
		case 37://��
			break;
		case 39://��
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
	//ע�⣬�˴���Ϊ����if��������if-elseif
	if(pMsg->message == WM_KEYUP){
		UINT nChar = (UINT)pMsg->wParam;
		switch (nChar)
		{
		case 38://��
			break;
		case 40://��
			break;
		case 37://��
			break;
		case 39://��
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
	this->KillTimer(1);//��ͣ
	MessageBox(str.c_str(),"���� layout");
	this->SetTimer( 1,timerspan,NULL);//�ָ�
	
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
		//��A�任��[0,2*pi)
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
	ShellExecute( 0,"open","˵��.txt",0,0,SW_SHOW);
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
    str+="    ����϶�: ��ת   \r\n";
	str+="    ������: ����   \r\n";
	this->KillTimer(1);//��ͣ
	MessageBox(str.c_str(),"����");
	this->SetTimer( 1,timerspan,NULL);//�ָ�
	
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
	int Nvf=graph.Nv;//�����Nvֵ
	setNvDlg.m_Nv=graph.Nv;
	if(setNvDlg.DoModal()==IDOK){
		int Nv=setNvDlg.m_Nv;
		if(Nv!=Nvf){//�б������
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
