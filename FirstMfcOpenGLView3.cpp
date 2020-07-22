// FirstMfcOpenGLView.cpp : implementation of the CFirstMfcOpenGLView class
//


#include "stdafx.h"
#include "FirstMfcOpenGL.h"

#include "FirstMfcOpenGLDoc.h"
#include "FirstMfcOpenGLView.h"
#include "SetWidth.h"
#include "SetHermiete.h"
#include "SetBezier.h"
#include "SetBLine_P.h"
#include "SetBLine_K.h"
#include <corecrt_math_defines.h>
#include <math.h>
#include <vector>
#include <iostream>
using namespace std;

using namespace std;
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFirstMfcOpenGLView

IMPLEMENT_DYNCREATE(CFirstMfcOpenGLView, CView)

BEGIN_MESSAGE_MAP(CFirstMfcOpenGLView, CView)
	//{{AFX_MSG_MAP(CFirstMfcOpenGLView)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()

	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
	ON_COMMAND(ID_linewidth, &CFirstMfcOpenGLView::Onlinewidth)
	ON_COMMAND(ID_linecolor, &CFirstMfcOpenGLView::Onlinecolor)
	ON_COMMAND(ID_Hermite_origin, &CFirstMfcOpenGLView::OnHermiteorigin)
	ON_COMMAND(ID_Hermite_set, &CFirstMfcOpenGLView::OnHermiteset)
ON_WM_LBUTTONDOWN()
ON_WM_LBUTTONUP()
//ON_WM_MOUSEMOVE()
ON_COMMAND(ID_clean, &CFirstMfcOpenGLView::Onclean)
ON_COMMAND(ID_Bezier, &CFirstMfcOpenGLView::OnBezier)
ON_COMMAND(ID_Bline_define, &CFirstMfcOpenGLView::OnBlinedefine)
ON_COMMAND(ID_Bline_change, &CFirstMfcOpenGLView::OnBlinechange)
ON_COMMAND(ID_BeizerFace, &CFirstMfcOpenGLView::OnBeizerface)
ON_COMMAND(ID_Bface, &CFirstMfcOpenGLView::OnBface)
ON_COMMAND(ID_start_bline, &CFirstMfcOpenGLView::Onstartbline)
ON_COMMAND(ID_end_bline, &CFirstMfcOpenGLView::Onendbline)
ON_COMMAND(ID_start_face, &CFirstMfcOpenGLView::Onstartface)
ON_COMMAND(ID_end_Bezierface, &CFirstMfcOpenGLView::OnendBezierface)
ON_COMMAND(ID_Bezier_dynamic, &CFirstMfcOpenGLView::OnBezierdynamic)
ON_COMMAND(ID_end_Bface, &CFirstMfcOpenGLView::OnendBface)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFirstMfcOpenGLView construction/destruction

CFirstMfcOpenGLView::CFirstMfcOpenGLView()
{
	// TODO: add construction code here

}

CFirstMfcOpenGLView::~CFirstMfcOpenGLView()
{
}

BOOL CFirstMfcOpenGLView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
    cs.lpszClass =AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW);
	cs.style |=WS_CLIPSIBLINGS|WS_CLIPCHILDREN|WS_MAXIMIZE;
	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CFirstMfcOpenGLView drawing

void CFirstMfcOpenGLView::OnDraw(CDC* pDC)
{
	CFirstMfcOpenGLDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
	if(!pDoc)
		return;

	//设置当前绘图设备为OPENGL的设备情景对象
   if(m_hglrc) wglMakeCurrent(m_pDC->GetSafeHdc(),m_hglrc);
	else return;
   glDrawBuffer(GL_BACK);//指定在后台缓存中绘制
   glLoadIdentity();
   glClearColor(1.0,1.0,1.0,0.7f);
	//将颜色缓存设为glclearcolor命令所设置的颜色，即背景色
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

SwapBuffers(wglGetCurrentDC());//交换前后缓存
glDrawBuffer(GL_FRONT);
      
   
}

/////////////////////////////////////////////////////////////////////////////
// CFirstMfcOpenGLView printing

BOOL CFirstMfcOpenGLView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CFirstMfcOpenGLView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CFirstMfcOpenGLView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CFirstMfcOpenGLView diagnostics

#ifdef _DEBUG
void CFirstMfcOpenGLView::AssertValid() const
{
	CView::AssertValid();
}

void CFirstMfcOpenGLView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CFirstMfcOpenGLDoc* CFirstMfcOpenGLView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CFirstMfcOpenGLDoc)));
	return (CFirstMfcOpenGLDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CFirstMfcOpenGLView message handlers

int CFirstMfcOpenGLView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	int n;
	m_pDC=new CClientDC(this);
	ASSERT(m_pDC != NULL);
	//初始化像素格式
	static PIXELFORMATDESCRIPTOR pfd=
	{
		sizeof(PIXELFORMATDESCRIPTOR),  //结构大小
		1,
		PFD_DRAW_TO_WINDOW|             //窗口中绘图
		PFD_SUPPORT_OPENGL|             //支持OPENGL
		PFD_DOUBLEBUFFER,               //双缓冲模式
		PFD_TYPE_RGBA,                 //RGBA颜色模式
		24,                           // 24位色彩深度
        0,0,0,0,0,0,                   //无颜色位
		0,                           // 无alpha 缓存
		0,                           //无移位
		0,                           //无加速缓存
		0,0,0,0,                     //无累积位
		32,                          //32位z缓存
		0,                           //无模板缓存
		0,                           //无辅助缓存
		PFD_MAIN_PLANE,              //PFD主平面
		0,                           //保留
		0,0,0                        //忽略主层掩模
	};
	//选择像素格式
	int pixelformat;
    if((pixelformat=ChoosePixelFormat(m_pDC->GetSafeHdc(),&pfd))==0)
	{
		MessageBox("选择像素格式失败！");return -1;
	}
	//设置像素格式
	if(SetPixelFormat(m_pDC->GetSafeHdc(),pixelformat,&pfd)==FALSE)
	{
        MessageBox("设置像素格式失败！");return -1;
	}
	n=::GetPixelFormat(m_pDC->GetSafeHdc());
	::DescribePixelFormat(m_pDC->GetSafeHdc(),n,sizeof(pfd),&pfd);
	m_hglrc=wglCreateContext(m_pDC->GetSafeHdc());
	wglMakeCurrent(m_pDC->GetSafeHdc(),m_hglrc);

		
	
	return 0;
}

void CFirstMfcOpenGLView::OnDestroy() 
{
	wglMakeCurrent(NULL,NULL); //将当前绘图情景对象赋空
	//删除当前绘图情景对象并释放所占内存
	if(m_hglrc) ::wglDeleteContext(m_hglrc);
	if(m_pDC) delete m_pDC;
	CView::OnDestroy();
	
	// TODO: Add your message handler code here
	
}

void CFirstMfcOpenGLView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
    if(m_hglrc) wglMakeCurrent(m_pDC->GetSafeHdc(),m_hglrc);
	else return;
	glViewport(0,0,cx,cy);//设置视口与窗口大小相同
	glMatrixMode(GL_PROJECTION);//设置变换模式为投影变换
	glLoadIdentity();//初始化投影变换矩阵
	//根据窗口大小设置调整正射投影矩阵
	if(cx<=cy)
		glOrtho(-20.0,20.0,-20.0*(GLfloat)cy/(GLfloat)cx,20.0*(GLfloat)cy/(GLfloat)cx,-50.0,50.0);
	else
        glOrtho(-20.0*(GLfloat)cx/(GLfloat)cy,20.0*(GLfloat)cx/(GLfloat)cy,-20.0,20.0,-50.0,50.0);
	glMatrixMode(GL_MODELVIEW);//设置变换模式为变换模型
    glLoadIdentity();//初始化投影变换矩阵
}









void CFirstMfcOpenGLView::DDALine(double x0,double y0,double x1,double y1)     
{  		
	   CDC *pDC=GetWindowDC();	
       CRect rect;
	   GetClientRect(&rect);
	   pDC->SetViewportOrg(500,300);
	   CPen pen(PS_SOLID, m_width, m_color);
	   pDC->SelectObject(&pen);
	   pDC->MoveTo(x0, y0);
	   double  dx, dy,x, y, k,xmax,xmin;					
	   dx=x1-x0;
	   dy=y1-y0;				
	   if(x0!=x1)
	   {
		   k=dy/dx;
		   if(x0<x1)
		   {
			   xmin=x0;
			   xmax=x1;
			   y=y0;
		   }
		   else
		   {
			   xmin=x1;
			   xmax=x0;
			   y=y1;
		   }
		   if(fabs(k)<=1)
			   for (x=xmin; x<=xmax; x++,y+=k)				
				   pDC->LineTo(int(x+0.5), int(y+0.5));
		   else
			   for (x=xmin; x<=xmax; x+=fabs(1/k))				
			   { 
				   pDC->LineTo(int(x + 0.5), int(y + 0.5));
				   if(k>0) 
					   y++;
				   else
					   y--;
			   }
	   }
	   else
           for (x=x0,y=(y0>y1?y1:y0); y<=(y0>y1?y0:y1); y++)				
			   pDC->LineTo(int(x + 0.5), int(y + 0.5));
}


void CFirstMfcOpenGLView::Hermite(double p[4][3])                                  //Hermite:根据断点位置矢量和切实量化曲线
{
	
	vector<Point> pts;	
	int count1 = 500;
	double t = 0, dt = 1.0 / count1;
    while ( t<=1)
	{ 
		Point q;
		double qz;
		qz = (p[0][2] * (2 * pow(t, 3) - 3 * pow(t, 2) + 1) + p[1][2] * (-2 * pow(t, 3) + 3 * pow(t, 2)) + p[2][2] * (pow(t, 3) - 2 * pow(t, 2) + t) + p[3][2] *(pow(t, 3) - pow(t, 2)));
		q.x = (p[0][0] * (2 * pow(t, 3) - 3 * pow(t, 2) + 1) + p[1][0] * (-2 * pow(t, 3) + 3 * pow(t, 2)) + p[2][0] * (pow(t, 3) - 2 * pow(t, 2) + t) + p[3][0] * (pow(t, 3) - pow(t, 2))) - qz * cos(M_PI / 4);
        q.y= (p[0][1] * (2 * pow(t, 3) - 3 * pow(t, 2) + 1) + p[1][1] * (-2 * pow(t, 3) + 3 * pow(t, 2)) + p[2][1] * (pow(t, 3) - 2 * pow(t, 2) + t) + p[3][1] * (pow(t, 3) - pow(t, 2))) - qz * sin(M_PI / 4);
        pts.push_back(q);
		t+=dt;
	}
    while (pts.size()>1)
	{
		Point  p=pts.back();
		pts.pop_back();
		DDALine(p.x,p.y,pts.back().x,pts.back().y);
	}
}	   


 
double CFirstMfcOpenGLView::jiecheng(int n)                                             //阶乘的求解
{
	if (n == 1 || n == 0)
	{
		return 1;
	}
	else
	{
		return n * jiecheng(n - 1);
	}
}

double CFirstMfcOpenGLView::C_n_k(int n, int k)
{
	return jiecheng(n) / (jiecheng(k)*jiecheng(n - k));
}

   
void  CFirstMfcOpenGLView::Bezier()                                                    //画 Bezier曲线
{

	CDC *pDC = GetWindowDC();
	CPen pen(PS_SOLID, m_width, m_color);
	pDC->SelectObject(&pen);
	int count2 = 500;
	double t = 0, dt = 1.0 / count2;
	CPoint lastq = P_Bezier[0];
	while (t <= 1)
	{
		CPoint q;
		q.x = 0;
		q.y = 0;
		for (int i = 0; i <= B_n; i++)
		{
			q.x += P_Bezier[i].x*C_n_k(B_n, i)*pow(t, i)*pow(1 - t, B_n - i);
			q.y += P_Bezier[i].y*C_n_k(B_n, i)*pow(t, i)*pow(1 - t, B_n - i);
		}
	//	pDC->SetPixel(q,m_color);
		pDC->MoveTo(lastq);
		pDC->LineTo(q);
		lastq = q;
		t += dt;
	}

}




void CFirstMfcOpenGLView::BSplineToPoints()                                        // 计算bspline曲线上各点坐标值
{
	// TODO: 在此处添加实现代码.
	CDC *pDC = GetWindowDC();
	CPen pen(PS_SOLID, m_width, m_color);
	pDC->SelectObject(&pen); 
	double *knots = new double[N + K+1];
	for (int i = 0; i < N + K+1 ; i++)
	{
		knots[i] = i;
	}
	double u = knots[K - 1];
	double delt = (knots[N +1] - knots[K-1]) / (double)(nPoints);

	for (int i = 0; i < nPoints + 1; i++)
	{
		CPoint p=BSpline(knots, N + K+1 , u);
		if (i == 0)
		{
			pDC->MoveTo(p);
		}
		else
		{
			pDC->LineTo(p);
		}

		u += delt;
	}
}



CPoint CFirstMfcOpenGLView::BSpline(double knot[], int num, double u)                               // 计算B样条曲线在参数为u的坐标值
{
	// TODO: 在此处添加实现代码.
	CPoint ret = CPoint(0, 0);
	double temp;
	for (int i = 0; i < N+1; i++)
	{
		temp = Base(i, K, knot, num, u);
		ret.x += pointList[i].x * temp;
		ret.y += pointList[i].y * temp;
	}
	return ret;
}



double CFirstMfcOpenGLView::Base(int i, int k, double knot[], int num, double u)                             // 计算B样条基函数
{
	// TODO: 在此处添加实现代码.
	double temp1 = 0;
	double temp2 = 0;
	if (k == 1)
	{
		if (i == (num - 2))
		{
			if ((u >= knot[i]) && (u <= knot[i + 1])) return 1;
			else return 0;
		}
		else
		{
			if ((u >= knot[i]) && (u < knot[i + 1])) return 1;
			else return 0;
		}
	}
	else if (k > 1)
	{
		if (knot[i + k - 1] != knot[i])
		{
			temp1 = Base(i, k - 1, knot, num, u);
			temp1 = (u - knot[i])*temp1 / (knot[i + k - 1] - knot[i]);
		}
		if (knot[i + k] != knot[i + 1])
		{
			temp2 = Base(i + 1, k - 1, knot, num, u);
			temp2 = (knot[i + k] - u) * temp2 / (knot[i + k] - knot[i + 1]);
		}
		return temp1 + temp2;
	}
	return 0;
}




double CFirstMfcOpenGLView::F_k_n(int k,int n,double t)                                 //B样条曲面基函数
{
	double F = 0;
	for (int i = 0; i <= n - k; i++)
	{
		F += pow(-1, i)*C_n_k(n + 1, i)*pow(t + n - k - i, n);
	}
	F = F / jiecheng(n);
	return F;
}


CPoint CFirstMfcOpenGLView::P_u_v(double u,double v)                                     //Bezier曲面的矩阵表示
{
	CPoint P;
	P.x = 0;
	P.y = 0;
	double B_i_n[1][4];
	for (int i = 0; i < 4; i++)
	{
		B_i_n[0][i] = C_n_k(3, i)*pow(v, i)*pow(1 - v, 3 - i);
	}
	double B_i_m[4][1];
	for (int i = 0; i < 4; i++)
	{
		B_i_m[i][0] = C_n_k(3, i)*pow(u, i)*pow(1 - u, 3 - i);
	}

	double P1[1][4] = { 0,0,0,0 };
	for(int i=0;i<1;i++)
		for(int j=0;j<4;j++)
			for (int k = 0; k < 4; k++)
			{
				P1[i][j] += B_i_n[i][k] * facepoint[k][j].x;
			}

	for (int i = 0; i < 1; i++)
		for (int j = 0; j < 1; j++)
			for (int k = 0; k < 4; k++)
			{
				P.x += P1[i][k] * B_i_m[k][j];
			}
	double P2[1][4] = { 0,0,0,0 };
	for (int i = 0; i < 1; i++)
		for (int j = 0; j < 4; j++)
			for (int k = 0; k < 4; k++)
			{
				P2[i][j] += B_i_n[i][k] * facepoint[k][j].y;
			}

	for (int i = 0; i < 1; i++)
		for (int j = 0; j < 1; j++)
			for (int k = 0; k < 4; k++)
			{
				P.y += P2[i][k] * B_i_m[k][j];
			}

	return P;


}


void CFirstMfcOpenGLView::BezierFace()                                       //画Bezier曲面
{
	CDC *pDC = GetWindowDC();

	int count2 = 100;
	double u , v ;
	double dt = 1.0 / count2;
	for (u = 0; u <= 1; u += dt)
	{
		
		for (v = 0; v <= 1; v += dt)
		{
			CPoint q;
			q = P_u_v(u, v);
			pDC->SetPixel(q, m_color);
		}
	}
	
}

CPoint CFirstMfcOpenGLView::P_u_v_BFace(double u, double v)                                     //B样条曲面的矩阵表示
{
	CPoint P;
	P.x = 0;
	P.y = 0;
	for(int i=0;i<=3;i++)
		for (int j = 0; j <= 3; j++)
		{
			P.x += F_k_n(i, 3, u)*F_k_n(j, 3, v)*facepoint[i][j].x;
			P.y += F_k_n(i, 3, u)*F_k_n(j, 3, v)*facepoint[i][j].y;
		}
	return P;


}

void CFirstMfcOpenGLView::BFace()                                          //画B样条曲面
{
	CDC *pDC = GetWindowDC();

	int count2 = 100;
	double u, v;
	double dt = 1.0 / count2;
	for (u = 0; u <= 1; u += dt)
	{
		for (v = 0; v <= 1; v += dt)
		{
			CPoint q;
			q = P_u_v_BFace(u, v);
			pDC->SetPixel(q, m_color);
		}
	}
}






//设置线宽
void CFirstMfcOpenGLView::Onlinewidth()
{
	// TODO: 在此添加命令处理程序代码
	SetWidth dlg;
	dlg.width_Dlg = m_width;
	UpdateData(FALSE);
	if (IDOK == dlg.DoModal())
		m_width = dlg.width_Dlg;
}


//设置线条颜色
void CFirstMfcOpenGLView::Onlinecolor()
{
	// TODO: 在此添加命令处理程序代码

	CColorDialog dlg(m_color, CC_FULLOPEN | CC_RGBINIT);

	if (IDOK == dlg.DoModal())
		m_color = dlg.GetColor();
}



//原始hermite曲线
void CFirstMfcOpenGLView::OnHermiteorigin()
{
	// TODO: 在此添加命令处理程序代码
	glClear(GL_COLOR_BUFFER_BIT);
	Hermite(p_origin);
}


//自己画hermite曲线
void CFirstMfcOpenGLView::OnHermiteset()
{
	// TODO: 在此添加命令处理程序代码
	SetHermiete dlg;

	 dlg.p0_x = p[0][0];
	 dlg.p0_y = p[0][1];
	dlg.p0_z = p[0][2];
	dlg.p1_x = p[1][0];
	dlg.p1_y = p[1][1];
	dlg.p1_z = p[1][2];


	dlg.p0_xx= p[2][0];
	dlg.p0_yy = p[2][1];
	dlg.p0_zz = p[2][2];

	dlg.p1_xx = p[3][0];
	dlg.p1_yy = p[3][1];
	dlg.p1_zz = p[3][2];

	UpdateData(FALSE);



	if (IDOK == dlg.DoModal())
	{
		p[0][0] = dlg.p0_x;
		p[0][1] = dlg.p0_y;
		p[0][2] = dlg.p0_z;
		p[1][0] = dlg.p1_x;
		p[1][1] = dlg.p1_y;
		p[1][2] = dlg.p1_z;

		p[2][0] = dlg.p0_xx;
		p[2][1] = dlg.p0_yy;
		p[2][2] = dlg.p0_zz;
		p[3][0] = dlg.p1_xx;
		p[3][1] = dlg.p1_yy;
		p[3][2] = dlg.p1_zz;
	}
	glClear(GL_COLOR_BUFFER_BIT);
	Hermite(p);

}



void CFirstMfcOpenGLView::DynamicBezier()
{

	CDC *pDC = GetWindowDC();
	CPen pen1(PS_SOLID, 2, RGB(255, 0, 0));
	CPen pen2(PS_SOLID, 2, RGB(0, 255, 0));
	CPen pen3(PS_SOLID, 2, RGB(0, 0, 255));
	CPen pen4(PS_SOLID, 2, RGB(255, 255, 255));
	CPen pen5(PS_SOLID, 2, RGB(0, 0, 0));
	CPen pen6(PS_SOLID, 2, RGB(0, 255, 0));
	int num = 0;
	while (true)
	{
		CPoint p[6], lastp;
		lastp = p_dyn[0];
		for (double t = 0; t <= 1; t += 0.05)
		{
			for (int i = 0; i < 3; i++)
			{
				p[i].x = p_dyn[i].x * (1 - t) + p_dyn[i + 1].x * t;
				p[i].y = p_dyn[i].y * (1 - t) + p_dyn[i + 1].y * t;
			}
			for (int i = 0; i < 2; i++)
			{
				p[i + 3].x = p[i].x*(1 - t) + p[i + 1].x*t;
				p[i + 3].y = p[i].y*(1 - t) + p[i + 1].y*t;
			}

			p[5].x = p[3].x*(1 - t) + p[4].x*t;
			p[5].y = p[3].y*(1 - t) + p[4].y*t;

			pDC->SelectObject(&pen2);
			pDC->MoveTo(p[0]);
			pDC->LineTo(p[1]);

			pDC->MoveTo(p[1]);
			pDC->LineTo(p[2]);

			pDC->SelectObject(&pen3);
			pDC->MoveTo(p[3]);
			pDC->LineTo(p[4]);

			if (num % 2 == 0)
			{
				pDC->SelectObject(&pen1);
			}
			else
			{
				pDC->SelectObject(&pen6);
			}

			pDC->MoveTo(lastp);
			pDC->LineTo(p[5]);
			CString s;
			s.Format(_T("%2f"), t);
			pDC->TextOutA(p_dyn[3].x + 40, p_dyn[3].y + 20, s);
			_sleep(50);

			pDC->SelectObject(&pen4);
			pDC->MoveTo(p[0]);
			pDC->LineTo(p[1]);

			pDC->MoveTo(p[1]);
			pDC->LineTo(p[2]);

			pDC->MoveTo(p[3]);
			pDC->LineTo(p[4]);
			if (num % 2 == 0)
			{
				pDC->SelectObject(&pen1);
			}
			else
			{
				pDC->SelectObject(&pen6);
			}
			pDC->MoveTo(lastp);
			pDC->LineTo(p[5]);
			lastp = p[5];

		
			pDC->SelectObject(&pen5);
			for (int i = 0; i < 3; i++)
			{
				pDC->MoveTo(p_dyn[i]);
				pDC->LineTo(p_dyn[i + 1]);
			}
			

	}
		num++;

	}



	
}


void CFirstMfcOpenGLView::OnLButtonDown(UINT nFlags, CPoint point)                          //鼠标左键被按下
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CDC *pDC = GetWindowDC();
	CPen pen1(PS_DASH, 1, RGB(0, 0, 0));
	CBrush brush1(RGB(255, 0, 0));
	CBrush brush2(RGB(0, 0, 0));
	switch (drawtype)
	{
	case 1://bezier
		if (count <= B_n+1)
		{
			P_Bezier[count] = point;
			pDC->Ellipse(P_Bezier[count].x - 4, P_Bezier[count].y - 4, P_Bezier[count].x + 4, P_Bezier[count].y + 4);
			count++;
		}
		break;
	case 2://三次B样条
		if (count < N+1)
		{
			startPoint = point;
			endPoint = point;
			pointList[count] = point;
			pDC->Ellipse(pointList[count].x - 4, pointList[count].y - 4, pointList[count].x + 4, pointList[count].y + 4);
			count++; 
		}
		break;
	case 3:
		if (count < 16)
		{
			facepoint[count%4][count/4] = point;
			pDC->Ellipse(facepoint[count % 4][count / 4].x - 4, facepoint[count % 4][count / 4].y - 4, facepoint[count % 4][count / 4].x + 4, facepoint[count % 4][count / 4].y + 4);
			count++;
		}
		break;

	case 4:
		if (count < 16)
		{
			facepoint[count % 4][count / 4] = point;
			pDC->Ellipse(facepoint[count % 4][count / 4].x - 4, facepoint[count % 4][count / 4].y - 4, facepoint[count % 4][count / 4].x + 4, facepoint[count % 4][count / 4].y + 4);
			count++;
		}
		break;

	case 5://改变控制点
		pDC->SelectObject(&brush1);
		for(int i=0;i<4;i++)
			for (int j = 0; j < 4; j++){
				if ((point.x>=facepoint[i][j].x-4)&&(point.x<=facepoint[i][j].x+4))
				{
					if ((point.y >= facepoint[i][j].y - 4) && (point.y <= facepoint[i][j].y + 4))
					{
						pDC->Ellipse(facepoint[i][j].x - 4, facepoint[i][j].y - 4, facepoint[i][j].x + 4, facepoint[i][j].y + 4);
						ii = i;
						jj = j;
						return;
					}				
				}
			}
		break;

	case 7:
		pDC->SelectObject(&brush1);
		for (int i = 0; i < N+1; i++)
		{
			if ((point.x >= pointList[i].x - 4) && (point.x <= pointList[i].x + 4))
			{
				if ((point.y >= pointList[i].y - 4) && (point.y <= pointList[i].y + 4))
				{
					pDC->Ellipse(pointList[i].x - 4, pointList[i].y - 4, pointList[i].x + 4, pointList[i].y + 4);
					ii = i;
					break;
				}
			}
		}
		break;

	case 8:
		pDC->SelectObject(&brush2);
		if (count < 4)
		{
			p_dyn[count] = point;
			pDC->Ellipse(p_dyn[count].x - 4, p_dyn[count].y - 4, p_dyn[count].x + 4, p_dyn[count].y + 4);
			count++;
		}
		break;




	}
	CView::OnLButtonDown(nFlags, point);
}


void CFirstMfcOpenGLView::OnLButtonUp(UINT nFlags, CPoint point)                        //鼠标左键松开
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CDC *pDC = GetWindowDC();
	CPen pen1(PS_DASH, 1, RGB(0, 0, 0));
	CPen pen2(PS_DASH, 1, RGB(0, 255, 0));
	CPen pen3(PS_SOLID, 2, RGB(0, 0, 0));
	CBrush brush1(RGB(255, 0, 0));
	pDC->SelectObject(&pen1);
	switch (drawtype)
	{
	case 1://Bezier
		if (count > 1 && count <= B_n)
		{
			pDC->MoveTo(P_Bezier[count - 2]);
			pDC->LineTo(P_Bezier[count - 1]);
		}
		if (count == B_n + 1)
		{

			pDC->MoveTo(P_Bezier[count - 2]);
			pDC->LineTo(P_Bezier[count - 1]);
			Bezier();
		}

		break;
	case 2://三次B样条
		if (count > 1 && count <= N+1)
		{
			pDC->MoveTo(pointList[count - 2]);
			pDC->LineTo(pointList[count - 1]);
		}
		if (count ==N+1)
		{
			BSplineToPoints();
		}
		break;
	case 3://Bezier曲面
		if (count % 4 != 1&&count<16)
		{
			pDC->MoveTo(facepoint[(count - 2) % 4][(count - 2) / 4]);
			pDC->LineTo(facepoint[(count - 1) % 4][(count - 1) / 4]);
		}
		if (count == 16)
		{
			pDC->MoveTo(facepoint[(count - 2) % 4][(count - 2) / 4]);
			pDC->LineTo(facepoint[(count - 1) % 4][(count - 1) / 4]);
			BezierFace();
		}
		break;
	case 4:
		if (count % 4 != 1&&count<16)
		{
			pDC->MoveTo(facepoint[(count - 2) % 4][(count - 2) / 4]);
			pDC->LineTo(facepoint[(count - 1) % 4][(count - 1) / 4]);
		}
		if (count == 16)
		{
			pDC->MoveTo(facepoint[(count - 2) % 4][(count - 2) / 4]);
			pDC->LineTo(facepoint[(count - 1) % 4][(count - 1) / 4]);
			BFace();
		}
		break;
	case 5://改变控制点
		pDC->SelectObject(&pen2);
		facepoint[ii][jj] = point;
		pDC->Ellipse(point.x - 4, point.y - 4, point.x + 4, point.y + 4);
		for (int i = 0; i < 16; i++)
		{
			if (i % 4 != 1 && i < 16)
			{
				pDC->MoveTo(facepoint[(i - 2) % 4][(i - 2) / 4]);
				pDC->LineTo(facepoint[(i - 1) % 4][(i - 1) / 4]);
			}
			if (i == 16)
			{
				pDC->MoveTo(facepoint[(i - 2) % 4][(i - 2) / 4]);
				pDC->LineTo(facepoint[(i - 1) % 4][(i - 1) / 4]);
			}
		}
		break;
	case 7:
		pDC->SelectObject(&pen2);
		pointList[ii] = point;
		pDC->Ellipse(point.x - 4, point.y - 4, point.x + 4, point.y + 4);
		for (int i = 0; i < N; i++)
		{
				pDC->MoveTo(pointList[i]);
				pDC->LineTo(pointList[i+1]);
		}
		break;
	case 8:
		pDC->SelectObject(&pen3);
		if (count > 1 && count <=4)
		{
			pDC->MoveTo(p_dyn[count - 2]);
			pDC->LineTo(p_dyn[count - 1]);
		}
		if (count == 4)
		{
			pDC->TextOutA(p_dyn[0].x - 20, p_dyn[0].y,"P0");
			pDC->TextOutA(p_dyn[1].x - 20, p_dyn[1].y, "P1");
			pDC->TextOutA(p_dyn[2].x - 20, p_dyn[2].y, "P2");
			pDC->TextOutA(p_dyn[3].x - 20, p_dyn[3].y, "P3");
			pDC->TextOutA(p_dyn[3].x + 20, p_dyn[3].y + 20, "t=");
			DynamicBezier();
		}
		break;



	}
	
	CView::OnLButtonUp(nFlags, point);
}





void CFirstMfcOpenGLView::Onclean()//清屏
{
	// TODO: 在此添加命令处理程序代码
	Invalidate();
}


void CFirstMfcOpenGLView::OnBezier()
{
	// TODO: 在此添加命令处理程序代码
	SetBezier dlg;
	if (IDOK == dlg.DoModal())
	{
		B_n = dlg.num;
	}
	glClear(GL_COLOR_BUFFER_BIT);
	drawtype = 1;
	count = 0;
}


void CFirstMfcOpenGLView::OnBlinedefine()
{
	// TODO: 在此添加命令处理程序代码
	SetBLine_P dlg;
	if (IDOK == dlg.DoModal())
	{
		N = dlg.P_num_dlg-1;
	}
	glClear(GL_COLOR_BUFFER_BIT);
	K = 4;
	drawtype = 2;
	count = 0;



}


void CFirstMfcOpenGLView::OnBlinechange()
{
	// TODO: 在此添加命令处理程序代码
	SetBLine_K dlg;
	if (IDOK == dlg.DoModal())
	{
		K = dlg.setK_dlg+1;
	}
	glClear(GL_COLOR_BUFFER_BIT);
	N = 3;
	drawtype = 2;
	count = 0;

}


void CFirstMfcOpenGLView::OnBeizerface()
{
	// TODO: 在此添加命令处理程序代码
	drawtype = 3;
	count = 0;

}


void CFirstMfcOpenGLView::OnBface()
{
	// TODO: 在此添加命令处理程序代码
	drawtype = 4;
	count = 0;
}


void CFirstMfcOpenGLView::Onstartbline()                //开始调整B样条曲线的控制点
{
	// TODO: 在此添加命令处理程序代码
	drawtype = 7;
}


void CFirstMfcOpenGLView::Onendbline()
{
	// TODO: 在此添加命令处理程序代码
	drawtype = 6;
	CDC *pDC = GetWindowDC();
	CBrush brush(RGB(255, 255, 255));
	CPen pen1(PS_DASH, 1, RGB(0,0,0));
	pDC->SelectObject(&brush);
	CRect rect;
	GetClientRect(&rect);
	pDC->Rectangle(rect);
	
	for (int i = 0; i < N+1; i++)
	{
		pDC->Ellipse(pointList[i].x - 4, pointList[i].y - 4, pointList[i].x + 4, pointList[i].y + 4);
	}
	pDC->SelectObject(&pen1);
	for (int i = 0; i < N ; i++)
	{
		pDC->MoveTo(pointList[i]);
		pDC->LineTo(pointList[i + 1]);
	}
	BSplineToPoints();

}


void CFirstMfcOpenGLView::Onstartface()
{
	// TODO: 在此添加命令处理程序代码
	drawtype = 5;
}


void CFirstMfcOpenGLView::OnendBezierface()
{
	// TODO: 在此添加命令处理程序代码
	drawtype = 6;
	CDC *pDC = GetWindowDC();
	CBrush brush(RGB(255, 255, 255));
	CPen pen1(PS_DASH,1, RGB(0,0,0));

	pDC->SelectObject(&brush);
	CRect rect;
	GetClientRect(&rect);
	pDC->Rectangle(rect);
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			pDC->Ellipse(facepoint[i][j].x - 4, facepoint[i][j].y - 4, facepoint[i][j].x + 4, facepoint[i][j].y + 4);
		}

	}
	pDC->SelectObject(&pen1);
	for (int i = 0; i < 16; i++)
	{
		if (i % 4 != 1 && i < 16)
		{
			pDC->MoveTo(facepoint[(i - 2) % 4][(i - 2) / 4]);
			pDC->LineTo(facepoint[(i - 1) % 4][(i - 1) / 4]);
		}
		if (i == 16)
		{
			pDC->MoveTo(facepoint[(i - 2) % 4][(i - 2) / 4]);
			pDC->LineTo(facepoint[(i - 1) % 4][(i - 1) / 4]);
		}
	}
	BezierFace();
}


void CFirstMfcOpenGLView::OnBezierdynamic()                                   //三次Bezier曲线动态生成图
{
	// TODO: 在此添加命令处理程序代码
	drawtype = 8;
	count = 0;

}


void CFirstMfcOpenGLView::OnendBface()
{
	// TODO: 在此添加命令处理程序代码
	drawtype = 6;
	CDC *pDC = GetWindowDC();
	CBrush brush(RGB(255, 255, 255));
	pDC->SelectObject(&brush);
	CRect rect;
	GetClientRect(&rect);
	pDC->Rectangle(rect);
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			pDC->Ellipse(facepoint[i][j].x - 4, facepoint[i][j].y - 4, facepoint[i][j].x + 4, facepoint[i][j].y + 4);
		}

	}

	for (int i = 0; i < 16; i++)
	{
		if (i % 4 != 1 && i < 16)
		{
			pDC->MoveTo(facepoint[(i - 2) % 4][(i - 2) / 4]);
			pDC->LineTo(facepoint[(i - 1) % 4][(i - 1) / 4]);
		}
		if (i == 16)
		{
			pDC->MoveTo(facepoint[(i - 2) % 4][(i - 2) / 4]);
			pDC->LineTo(facepoint[(i - 1) % 4][(i - 1) / 4]);
		}
	}
	BFace();
}
