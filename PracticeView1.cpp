
// PracticeView.cpp: CPracticeView 类的实现
//

#include "stdafx.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "Practice.h"
#endif

#include "PracticeDoc.h"
#include "PracticeView.h"
#include "DrawDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CPracticeView

IMPLEMENT_DYNCREATE(CPracticeView, CView)

BEGIN_MESSAGE_MAP(CPracticeView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CPracticeView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_COMMAND(ID_color, &CPracticeView::Oncolor)
	ON_COMMAND(ID_lineWidth, &CPracticeView::Onlinewidth)
	ON_COMMAND(ID_DDAline, &CPracticeView::OnDdaline)
	ON_COMMAND(ID_MIDline, &CPracticeView::OnMidline)
	ON_COMMAND(ID_BREline, &CPracticeView::OnBreline)
	ON_WM_LBUTTONDOWN()
	ON_COMMAND(ID_MidCircle, &CPracticeView::OnMidcircle)
	ON_COMMAND(ID_BreCircle, &CPracticeView::OnBrecircle)
	ON_WM_LBUTTONUP()
//	ON_WM_ERASEBKGND()
	ON_COMMAND(ID_delete, &CPracticeView::Ondelete)
	ON_COMMAND(ID_namedraw, &CPracticeView::Onnamedraw)
	ON_COMMAND(ID_polygon, &CPracticeView::Onpolygon)
	ON_COMMAND(ID_bagua, &CPracticeView::Onbagua)
	ON_COMMAND(MID4Circle, &CPracticeView::OnMid4circle)
END_MESSAGE_MAP()

// CPracticeView 构造/析构

CPracticeView::CPracticeView() noexcept
{
	// TODO: 在此处添加构造代码
	m_width = 0;
	m_color = RGB(0,0,0);
	sign = 0;
	i = 0;
}

CPracticeView::~CPracticeView()
{
}

BOOL CPracticeView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式
	cs.style |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_MAXIMIZE;
	return CView::PreCreateWindow(cs);
}

// CPracticeView 绘图

void CPracticeView::OnDraw(CDC* /*pDC*/)
{
	CPracticeDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	//设置当前绘图设备为OPENGL的设备情景对象
	if (m_hglrc) wglMakeCurrent(m_pDC->GetSafeHdc(), m_hglrc);
	else return;
	glDrawBuffer(GL_BACK);//指定在后台缓存中绘制
	glLoadIdentity();
	glClearColor(1.0, 1.0, 1.0,0.0f);
	//将颜色缓存设为glclearcolor命令所设置的颜色，即背景色
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	SwapBuffers(wglGetCurrentDC());//交换前后缓存
	glDrawBuffer(GL_FRONT);

}


// CPracticeView 打印


void CPracticeView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CPracticeView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CPracticeView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CPracticeView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

void CPracticeView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CPracticeView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CPracticeView 诊断

#ifdef _DEBUG
void CPracticeView::AssertValid() const
{
	CView::AssertValid();
}

void CPracticeView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CPracticeDoc* CPracticeView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPracticeDoc)));
	return (CPracticeDoc*)m_pDocument;
}
#endif //_DEBUG


// CPracticeView 消息处理程序

int CPracticeView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	int n;
	m_pDC = new CClientDC(this);
	ASSERT(m_pDC != NULL);
	//初始化像素格式
	static PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),  //结构大小
		1,
		PFD_DRAW_TO_WINDOW |             //窗口中绘图
		PFD_SUPPORT_OPENGL |             //支持OPENGL
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
	if ((pixelformat = ChoosePixelFormat(m_pDC->GetSafeHdc(), &pfd)) == 0)
	{
		MessageBox(L"选择像素格式失败！"); return -1;
	}
	//设置像素格式
	if (SetPixelFormat(m_pDC->GetSafeHdc(), pixelformat, &pfd) == FALSE)
	{
		MessageBox(L"设置像素格式失败！"); return -1;
	}
	n = ::GetPixelFormat(m_pDC->GetSafeHdc());
	::DescribePixelFormat(m_pDC->GetSafeHdc(), n, sizeof(pfd), &pfd);
	m_hglrc = wglCreateContext(m_pDC->GetSafeHdc());
	wglMakeCurrent(m_pDC->GetSafeHdc(), m_hglrc);



	return 0;
}


void CPracticeView::OnDestroy()
{
	// TODO: 在此处添加消息处理程序代码
	wglMakeCurrent(NULL, NULL); //将当前绘图情景对象赋空
	//删除当前绘图情景对象并释放所占内存
	if (m_hglrc) ::wglDeleteContext(m_hglrc);
	if (m_pDC) delete m_pDC;
	CView::OnDestroy();

}

void CPracticeView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if (m_hglrc) wglMakeCurrent(m_pDC->GetSafeHdc(), m_hglrc);
	else return;
	glViewport(0, 0, cx, cy);//设置视口与窗口大小相同
	glMatrixMode(GL_PROJECTION);//设置变换模式为投影变换
	glLoadIdentity();//初始化投影变换矩阵
	//根据窗口大小设置调整正射投影矩阵
	if (cx <= cy)
		glOrtho(-20.0, 20.0, -20.0*(GLfloat)cy / (GLfloat)cx, 20.0*(GLfloat)cy / (GLfloat)cx, -50.0, 50.0);
	else
		glOrtho(-20.0*(GLfloat)cx / (GLfloat)cy, 20.0*(GLfloat)cx / (GLfloat)cy, -20.0, 20.0, -50.0, 50.0);
	glMatrixMode(GL_MODELVIEW);//设置变换模式为变换模型
	glLoadIdentity();//初始化投影变换矩阵
}

//画直线的三种算法
//************DDA画线法*************
void CPracticeView::DDALine(CDC* pDC,CPoint p0,CPoint p1)
{
	int x;
	float dx, dy, y, k;
	CPen *pen = new CPen(PS_SOLID, m_width, m_color);
	CPen *oldpen = pDC->SelectObject(pen);
	pDC->MoveTo(p0);
	pDC->LineTo(p0);
	dx = p1.x - p0.x;
	dy = p1.y - p0.y;
	k = dy / dx;
	//if (k > 1 || k < -1)
	//	k = 1 / k;
	y = p0.y;
	for (x = p0.x; x <= p1.x; x++)
	{
		pDC->LineTo(x, int(y + 0.5));
		y = y + k;
	}
	pDC->SelectObject(oldpen);

}
//************中点画线法*************
void CPracticeView::Swap(int a, int b)
{
	int c;
	c = a;
	a = b;
	b = c;
}

void CPracticeView::MIDLine(CDC *pDC, CPoint p0, CPoint p1)
{
	int a, b, dt1, dt2, d, x, y, ystp = 1;
	CPen *pen = new CPen(PS_SOLID, m_width, m_color);
	CPen *oldpen = pDC->SelectObject(pen);
	pDC->MoveTo(p0);
	if (abs(p1.x - p0.x) > abs(p1.y - p0.y))
	{
		if (p0.x > p1.x)
		{
			Swap(p0.x, p1.x);
			Swap(p0.y, p1.y);
		}
		a = p0.y - p1.y;
		b = p1.x - p0.x;
		if (a > 0)
		{
			a = -a;
			ystp = -1;
		}
		else ystp = 1;
		d = (a << 1) + b;
		dt1 = a << 1;
		dt2 = (a + b) << 1;
		
		x = p0.x;
		y = p0.y;
		pDC->LineTo(p0);
		while (x < p1.x)
		{
			if (d < 0)
			{
				x++;
				y += ystp;
				d += dt2;
			}
			else
			{
				x++;
				d += dt1;
			}
			pDC->LineTo(x,y);
		}
		if (p0.y > p1.y)
		{
			x = p0.x;
			p0.x = p1.x;
			p1.x = x;
			y = p0.y;
			p0.y = p1.y;
			p1.y = y;
		}
		a = p0.x - p1.x;
		b = p1.y - p0.y;
		if (a > 0)
		{
			a = -a;
			ystp = -1;
		}
		else ystp = 1;
		d = (a << 1) + b;
		dt1 = a << 1;
		dt2 = (a + b) << 1;
		x = p0.x;
		y = p0.y;
		pDC->LineTo(x, y);
		while (y < p1.y)
		{
			if (d < 0)
			{
				y++;
				x += ystp;
				d += dt2;
			}
			else
			{
				y++;
				d += dt1;
			}
			pDC->LineTo(x, y);
		}
	}
	pDC->SelectObject(oldpen);
}



//************Brensenham画线法*************
void CPracticeView::BresenhamLine(CDC* pDC, CPoint p1, CPoint p2)
{
	int iTag = 0;
	int dx, dy, tx, ty, inc1, inc2, d, curx, cury;
	CPen *pen = new CPen(PS_SOLID, m_width, m_color);
	CPen *oldpen = pDC->SelectObject(pen);
	pDC->MoveTo(p1);
	pDC->LineTo(p1);
	if (p1.x == p2.x && p1.y == p2.y)
	{
		return;
	}
	dx = abs(p2.x - p1.x);
	dy = abs(p2.y - p1.y);
	if (dx < dy)
	{
		iTag = 1;
		Swap(p1.x, p1.y);
		Swap(p2.x, p2.y);
		Swap(dx, dy);
	}
	tx = (p2.x - p1.x) > 0 ? 1 : -1;
	ty = (p2.y - p1.y) > 0 ? 1 : -1;
	curx = p1.x;
	cury = p1.y;
	inc1 = 2 * dy;
	inc2 = 2 * (dy - dx);
	d = inc1 - dx;
	while (curx != p2.x)
	{
		curx += tx;
		if (d < 0)
			d += inc1;
		else
		{
			cury += ty;
			d += inc2;
		}
		if (iTag)
			pDC->LineTo(cury, curx);
		else
			pDC->LineTo(curx, cury);
	}
	pDC->SelectObject(oldpen);
}

//************1/8中点画圆*************
void CPracticeView::CirPot(CDC *pDC,CPoint p0, int x,int y)
{
	pDC->SetPixel(p0.x + x, p0.y + y,m_color);
	pDC->SetPixel(p0.x + y, p0.y + x, m_color);
	pDC->SetPixel(p0.x + y, p0.y - x, m_color);
	pDC->SetPixel(p0.x + x, p0.y - y, m_color);
	pDC->SetPixel(p0.x - x, p0.y - y, m_color);
	pDC->SetPixel(p0.x - y, p0.y - x, m_color);
	pDC->SetPixel(p0.x - y, p0.y + x, m_color);
	pDC->SetPixel(p0.x - x, p0.y + y, m_color);
}

void CPracticeView::MidCircle(CDC *pDC,CPoint p, int r)
{
	int x, y, d;
	x = 0;
	y = r;
	d = 1 - r;
	CirPot(pDC,p, x, y);
	while (x < y)
	{
		if (d < 0)
		{
			d += 2*x+3;
		}
		else
		{
			d += 2*(x-y)+5;
			y--;
		}
		x++;
		CirPot(pDC,p, x, y);
	}
}

void CPracticeView::BresenCircle(CDC *pDC,CPoint p0, int r)
{
	int x, y, d;
	x = 0;
	y = r;
	d = 3 - 2 * r;
	while (x < y)
	{
		CirPot(pDC,p0, x, y);
		if (d < 0)
			d += 4 * x + 6;
		else
		{
			d += 4 * (x - y) + 10;
			y--;
		}
			x++;
	}
	if (x == y)
			CirPot(pDC,p0, x, y);
}




//调色盘
void CPracticeView::Oncolor()
{
	// TODO: 在此添加命令处理程序代码
	CColorDialog dlg(m_color, CC_FULLOPEN | CC_RGBINIT);
	if (IDOK == dlg.DoModal())
		m_color = dlg.GetColor();
}


void CPracticeView::Onlinewidth()
{
	// TODO: 在此添加命令处理程序代码
	DrawDlg dlg;
	if (IDOK == dlg.DoModal())
	m_width = dlg.Dlg_Width;
}


void CPracticeView::OnDdaline()
{
	// TODO: 在此添加命令处理程序代码
	CDC *pDC = GetWindowDC();
	sign = 1;

}




void CPracticeView::OnMidline()
{
	// TODO: 在此添加命令处理程序代码
	CDC *pDC = GetWindowDC();
	sign = 2;

}


void CPracticeView::OnBreline()
{
	// TODO: 在此添加命令处理程序代码
	CDC *pDC = GetWindowDC();
	sign = 3;

}

void CPracticeView::OnMidcircle()
{
	// TODO: 在此添加命令处理程序代码
	CDC *pDC = GetWindowDC();
	sign = 4;
	
}



void CPracticeView::OnBrecircle()
{
	// TODO: 在此添加命令处理程序代码
	CDC *pDC = GetWindowDC();
	sign = 5;

}



void CPracticeView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CDC *pDC = GetWindowDC();
	if (sign != 0)
	{
		i++;
		m_StartP=point;
	}

	CView::OnLButtonDown(nFlags, point);
}





void CPracticeView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CDC *pDC = GetWindowDC();
	m_EndP = point;
	if (i == 1)
	{
		i = 0;
		switch (sign)
		{
		case 1:
			DDALine(pDC,m_StartP,m_EndP);
			break;
		case 2:
			MIDLine(pDC, m_StartP, m_EndP);
			break;
		case 3:
			BresenhamLine(pDC, m_StartP, m_EndP);
			break;
		case 4:
			r = sqrt((m_StartP.x - m_EndP.x)*(m_StartP.x - m_EndP.x) + (m_StartP.y - m_EndP.y)*(m_StartP.y - m_EndP.y));
			MidCircle(pDC, m_StartP, r);
			break;
		case 5:
			r = sqrt((m_StartP.x - m_EndP.x)*(m_StartP.x - m_EndP.x) + (m_StartP.y - m_EndP.y)*(m_StartP.y - m_EndP.y));
			BresenCircle(pDC, m_StartP, r);
			break;
		case 6:
			r = sqrt((m_StartP.x - m_EndP.x)*(m_StartP.x - m_EndP.x) + (m_StartP.y - m_EndP.y)*(m_StartP.y - m_EndP.y));
			Mid4Circle(pDC, m_StartP, r);
			break;

		}
	}
	CView::OnLButtonUp(nFlags, point);
}





void CPracticeView::Ondelete()
{
	// TODO: 在此添加命令处理程序代码
	Invalidate(TRUE);
}


//***************多边形扫描线算法***********************

struct ET
{
	int x;
	double dx;
	int ymax;
	ET *next;
}AET,NET;




//定义多边形点集
struct point
{
	double x;
	double y;
}poly[4] = { {110,30} ,{220,90} ,{90,150},{60,90}};



void CPracticeView::POLYGON(CDC *pDC)
{
	CPen pen(PS_SOLID, 2, RGB(0, 0, 0));
	pDC->SelectObject(pen);
	//画出多边形
	pDC->MoveTo(60, 90);
	pDC->LineTo(110, 30);
	pDC->LineTo(220, 90);
	pDC->LineTo(90, 150);
	pDC->LineTo(60, 90);

}


void CPracticeView::ScanFill(CDC *pDC)
{

	//确定扫描线范围
	int j,ii=0;
	double YMAX = poly[0].y;
	//double YMIN = poly[0].y;
	for (j = 1; j < 4; j++)
	{
		if (poly[j].y > YMAX)
			YMAX = poly[j].y;
	//	if (poly[j].y < YMIN)
	//		YMIN = poly[j].y;
	}
	
	ET *NET[1000];
	ET *AET;
	for (int m = 0; m < YMAX; m++)
	{
		NET[m] = new ET;
		NET[m]->next = nullptr;
	}
	AET = new ET;
	AET->next = nullptr;

	for (ii = 0; ii <= YMAX; ii++)
	{
		for (int j = 0; j < 4; j++)
			if (poly[j].y == ii)
			{
				//一个点跟前面的一个点形成一条线段，跟后面的点也形成线段
				if (poly[(j - 1 + 4) % 4].y > poly[j].y)
				{
					ET *p = new ET;
					p->x = poly[j].x;
					p->ymax = poly[(j - 1 + 4) % 4].y;
					p->dx = (poly[(j - 1 + 4) % 4].x - poly[j].x) / (poly[(j - 1 + 4) % 4].y - poly[j].y);
					p->next = NET[ii]->next;
					NET[ii]->next = p;
				}
				if (poly[(j + 1 + 4) % 4].y > poly[j].y)
				{
					ET *p = new ET;
					p->x = poly[j].x;
					p->ymax = poly[(j + 1 + 4) % 4].y;
					p->dx = (poly[(j + 1 + 4) % 4].x - poly[j].x) / (poly[(j + 1 + 4) % 4].y - poly[j].y);
					p->next = NET[i]->next;
					NET[i]->next = p;
				}
			}
	}

	for (ii= 0; ii <= YMAX; ii++)
	{
		//计算新的交点x,更新AET
		ET *p = AET->next;
		while (p)
		{
			p->x = p->x + p->dx;
			p = p->next;
		}

		ET *tq = new ET;
		p = AET->next;
		tq->next = NULL;
		while (p)
		{
			while (tq->next && p->x >= tq->next->x)
				tq = tq->next;
			ET *s = p->next;
			p->next = tq->next;
			tq->next = p;
			p = s;
			tq = AET;
		}
		ET *q = AET;
		p = q->next;
		while (p)
		{
			if (p->ymax == i)
			{
				q->next = p->next;
				delete p;
				p = q->next;
			}
			else
			{
				q = q->next;
				p = q->next;
			}
		}
		//将NET中的新点加入AET,并用插入法按X值递增排序
		p = NET[i]->next;
		q = AET;
		while (p)
		{
			while (q->next && p->x >= q->next->x)
				q = q->next;
			ET *s = p->next;
			p->next = q->next;
			q->next = p;
			p = s;
			q = AET;
		}

		p = AET->next;
		while (p && p->next)
		{
			for (float j = p->x; j <= p->next->x; j++)
				pDC->SetPixel(j, ii, RGB(255, 255, 0));
			p = p->next->next;//考虑端点情况
		}
	}
}



void CPracticeView::SeedFill4(HDC hdc,int x, int y, COLORREF oldcolor, COLORREF newcolor)
{
	if (GetPixel(hdc,x, y) == oldcolor)
	{
		SetPixel(hdc,x, y, newcolor);
		SeedFill4(hdc,x, y + 1, oldcolor, newcolor);
		SeedFill4(hdc,x, y - 1, oldcolor, newcolor);
		SeedFill4(hdc,x-1, y, oldcolor, newcolor);
		SeedFill4(hdc,x+1, y, oldcolor, newcolor);
		

	}
	

}

//名字设计

void CPracticeView::Onnamedraw()
{
	CDC*pDC = GetDC();//得到绘图类指针  
	RedrawWindow();//重绘窗口

	//刘
	CPen *pen1 = new CPen(PS_SOLID, 4, RGB(72,61,139));//DarkSlateBlue
	CPen *pen2 = new CPen(PS_SOLID, 2, RGB(132, 112, 255));//LinghtSlateBlue
	CPen *pen3 = new CPen(PS_SOLID, 3, RGB(0, 0, 255));//Blue
	CPen *pen4 = new CPen(PS_SOLID, 3, RGB(30, 144, 255));//DodgerBlue
	CPen *pen5 = new CPen(PS_DASH, 1, RGB(70, 130, 180));//SteelBlue
	CPen *pen6 = new CPen(PS_DASH, 1, RGB(0, 206, 209));//DarkTurquoise
	pDC->SelectObject(pen1);
	CPoint p0 = { 350,100 };
	CPoint p01 = { 350,50 };
	CPoint p1 = { 200,200 };
	CPoint p2 = { 500,200 };
	int x, y, d;
	x = 0;
	y = 50;//r=50
	d = 1 - y;
	pDC->MoveTo(p0.x + x, p0.y - y);
	pDC->LineTo(p0.x + x, p0.y - y);
	pDC->MoveTo(p0.x + y, p0.y - x);
	pDC->LineTo(p0.x + y, p0.y - x);
	pDC->MoveTo(p0.x - y, p0.y - x);
	pDC->LineTo(p0.x - y, p0.y - x);
	pDC->MoveTo(p0.x - x, p0.y - y);
	pDC->LineTo(p0.x - x, p0.y - y);
	while (x < y)
	{
		if (d < 0)
		{
			d += 2 * x + 3;
		}
		else
		{
			d += 2 * (x - y) + 5;
			y--;
		}
		x++;
		pDC->MoveTo(p0.x + x, p0.y - y);
		pDC->LineTo(p0.x + x, p0.y - y);
		pDC->MoveTo(p0.x + y, p0.y - x);
		pDC->LineTo(p0.x + y, p0.y - x);
		pDC->MoveTo(p0.x - y, p0.y - x);
		pDC->LineTo(p0.x - y, p0.y - x);
		pDC->MoveTo(p0.x - x, p0.y - y);
		pDC->LineTo(p0.x - x, p0.y - y);
	}

	pDC->SelectObject(pen2);
	x = 0;
	y = 150;//r=150
	d = 1 - y;
	pDC->MoveTo(p01.x + x, p01.y + y);
	pDC->LineTo(p01.x + x, p01.y + y);
	pDC->MoveTo(p01.x + y, p01.y + x);
	pDC->LineTo(p01.x + y, p01.y + x);
	pDC->MoveTo(p01.x - y, p01.y + x);
	pDC->LineTo(p01.x - y, p01.y + x);
	pDC->MoveTo(p01.x - x, p01.y + y);
	pDC->LineTo(p01.x - x, p01.y + y);
	while (x < y)
	{
		if (d < 0)
		{
			d += 2 * x + 3;
		}
		else
		{
			d += 2 * (x - y) + 5;
			y--;
		}
		x++;
		pDC->MoveTo(p01.x + x, p01.y + y);
		pDC->LineTo(p01.x + x, p01.y + y);
		pDC->MoveTo(p01.x + y, p01.y + x);
		pDC->LineTo(p01.x + y, p01.y + x);
		pDC->MoveTo(p01.x - y, p01.y + x);
		pDC->LineTo(p01.x - y, p01.y + x);
		pDC->MoveTo(p01.x - x, p01.y + y);
		pDC->LineTo(p01.x - x, p01.y + y);
	}


	pDC->SelectObject(pen3);
	x = 0;
	y = 200;//r=200
	d = 1 - y;
	pDC->MoveTo(p1.x + x, p1.y + y);
	pDC->LineTo(p1.x + x, p1.y + y);
	pDC->MoveTo(p1.x + y, p1.y + x);
	pDC->LineTo(p1.x + y, p1.y + x);
	
	while (x < y)
	{
		if (d < 0)
		{
			d += 2 * x + 3;
		}
		else
		{
			d += 2 * (x - y) + 5;
			y--;
		}
		x++;
		pDC->MoveTo(p1.x + x, p1.y + y);
		pDC->LineTo(p1.x + x, p1.y + y);
		pDC->MoveTo(p1.x + y, p1.y + x);
		pDC->LineTo(p1.x + y, p1.y + x);
	}

	pDC->SelectObject(pen4);
	x = 0;
	y = 200;//r=200
	d = 1 - y;
	pDC->MoveTo(p2.x - y, p2.y + x);
	pDC->LineTo(p2.x - y, p2.y + x);
	pDC->MoveTo(p2.x - x, p2.y + y);
	pDC->LineTo(p2.x - x, p2.y + y);

	while (x < y)
	{
		if (d < 0)
		{
			d += 2 * x + 3;
		}
		else
		{
			d += 2 * (x - y) + 5;
			y--;
		}
		x++;
		pDC->MoveTo(p2.x - y, p2.y + x);
		pDC->LineTo(p2.x - y, p2.y + x);
		pDC->MoveTo(p2.x - x, p2.y + y);
		pDC->LineTo(p2.x - x, p2.y + y);
	}
	pDC->SelectObject(pen5);
	CPoint p3 = { 500,250 };
	CPoint p4 = { 500,400 };
	pDC->MoveTo(p3);
	pDC->LineTo(p4);

	pDC->SelectObject(pen6);
	CPoint p5 = { 550,50 };
	CPoint p6 = { 550,500 };
	pDC->MoveTo(p5);
	pDC->LineTo(p6);


	//琳
	CPen *pen7 = new CPen(PS_SOLID, 5, RGB(102, 205, 170));//MediumAquamarine
	CPen *pen8 = new CPen(PS_SOLID, 3, RGB(0,100, 0));//DarkGreen
	CPen *pen9 = new CPen(PS_SOLID, 4, RGB(46, 139, 87));//SeaGreen
	CPen *pen10 = new CPen(PS_SOLID, 4, RGB(32, 178, 170));//LightSeaGreen
	CPen *pen11 = new CPen(PS_DASH, 6, RGB(189, 183, 107));//DarkKhaki
	CPen *pen12 = new CPen(PS_DASH, 6, RGB(218, 165, 32));//goldenrod
	CPoint p7 = { 750,200 };
	CPoint p8 = { 700,250 };
	CPoint p9 = { 800,250 };
	CPoint p10 = { 750,300 };
	CPoint p11 = { 750,150 };
	CPoint p12 = { 750,350 };
	pDC->SelectObject(pen7);
	x = 0;
	y = 50;//r=50
	d = 1 - y;
	pDC->MoveTo(p7.x + x, p7.y - y);
	pDC->LineTo(p7.x + x, p7.y - y);
	pDC->MoveTo(p7.x + y, p7.y - x);
	pDC->LineTo(p7.x + y, p7.y - x);
	pDC->MoveTo(p7.x - y, p7.y - x);
	pDC->LineTo(p7.x - y, p7.y - x);
	pDC->MoveTo(p7.x - x, p7.y - y);
	pDC->LineTo(p7.x - x, p7.y - y);
	while (x < y)
	{
		if (d < 0)
		{
			d += 2 * x + 3;
		}
		else
		{
			d += 2 * (x - y) + 5;
			y--;
		}
		x++;
		pDC->MoveTo(p7.x + x, p7.y - y);
		pDC->LineTo(p7.x + x, p7.y - y);
		pDC->MoveTo(p7.x + y, p7.y - x);
		pDC->LineTo(p7.x + y, p7.y - x);
		pDC->MoveTo(p7.x - y, p7.y - x);
		pDC->LineTo(p7.x - y, p7.y - x);
		pDC->MoveTo(p7.x - x, p7.y - y);
		pDC->LineTo(p7.x - x, p7.y - y);
	}
	pDC->SelectObject(pen8);
	pDC->MoveTo(p8);
	pDC->LineTo(p9);
	pDC->SelectObject(pen9);
	pDC->MoveTo(p11);
	pDC->LineTo(p12);

	pDC->SelectObject(pen10);
	x = 0;
	y = 50;//r=50
	d = 1 - y;
	pDC->MoveTo(p10.x + x, p10.y + y);
	pDC->LineTo(p10.x + x, p10.y + y);
	pDC->MoveTo(p10.x + y, p10.y + x);
	pDC->LineTo(p10.x + y, p10.y + x);
	pDC->MoveTo(p10.x - y, p10.y + x);
	pDC->LineTo(p10.x - y, p10.y + x);
	pDC->MoveTo(p10.x - x, p10.y + y);
	pDC->LineTo(p10.x - x, p10.y + y);
	while (x < y)
	{
		if (d < 0)
		{
			d += 2 * x + 3;
		}
		else
		{
			d += 2 * (x - y) + 5;
			y--;
		}
		x++;
		pDC->MoveTo(p10.x + x, p10.y + y);
		pDC->LineTo(p10.x + x, p10.y + y);
		pDC->MoveTo(p10.x + y, p10.y + x);
		pDC->LineTo(p10.x + y, p10.y + x);
		pDC->MoveTo(p10.x - y, p10.y + x);
		pDC->LineTo(p10.x - y, p10.y + x);
		pDC->MoveTo(p10.x - x, p10.y + y);
		pDC->LineTo(p10.x - x, p10.y + y);
	}

	CPoint p13 = { 850,200 };
	CPoint p14 = { 1250,200 };
	CPoint p15 = { 950,50 };
	CPoint p16 = { 950,500 };
	CPoint p17 = { 1150,50 };
	CPoint p18 = { 1150,500 };
	CPoint p19 = { 950,200 };//*
	CPoint p20 = { 850,450 };
	CPoint p21 = { 1050,450 };//*
	CPoint p22 = { 1150,200 };
	CPoint p23 = { 1250,450 };
	pDC->SelectObject(pen8);
	pDC->MoveTo(p13);
	pDC->LineTo(p14);
	pDC->SelectObject(pen9);
	pDC->MoveTo(p15);
	pDC->LineTo(p16);
	pDC->SelectObject(pen10);
	pDC->MoveTo(p17);
	pDC->LineTo(p18);
	pDC->SelectObject(pen11);
	pDC->MoveTo(p19);
	pDC->LineTo(p20);
	pDC->MoveTo(p19);
	pDC->LineTo(p21);
	pDC->SelectObject(pen12);
	pDC->MoveTo(p22);
	pDC->LineTo(p21);
	pDC->MoveTo(p22);
	pDC->LineTo(p23);



}



void CPracticeView::Onpolygon()
{
	// TODO: 在此添加命令处理程序代码
	CDC *pDC = GetWindowDC();
	POLYGON(pDC);
	ScanFill(pDC);

}


void CPracticeView::Onbagua()
{
	// TODO: 在此添加命令处理程序代码
	CDC *pDC = GetWindowDC();
	HDC hdc = pDC->GetSafeHdc();
	//画八卦圆
	CPen *pen = new CPen(PS_DASH, 1, m_color);
	pDC->SelectObject(pen);

	CPoint p0 = { 55,55 };
	CPoint p1 = { 55,35 };
	CPoint p2 = { 55,75 };
	int x, y, d;
	x = 0;
	y = 20;//r=200
	d = 1 - y;
	pDC->SetPixel(p2.x - x, p2.y - y, m_color);
	pDC->SetPixel(p2.x - y, p2.y - x, m_color);
	pDC->SetPixel(p2.x - y, p2.y + x, m_color);
	pDC->SetPixel(p2.x - x, p2.y + y, m_color);
	while (x < y)
	{
		if (d < 0)
		{
			d += 2 * x + 3;
		}
		else
		{
			d += 2 * (x - y) + 5;
			y--;
		}
		x++;
		pDC->SetPixel(p2.x - x, p2.y - y, m_color);
		pDC->SetPixel(p2.x - y, p2.y - x, m_color);
		pDC->SetPixel(p2.x - y, p2.y + x, m_color);
		pDC->SetPixel(p2.x - x, p2.y + y, m_color);
	}
		x = 0;
		y = 20;//r=200
		d = 1 - y;
		pDC->SetPixel(p1.x + x, p1.y + y, m_color);
		pDC->SetPixel(p1.x + y, p1.y + x, m_color);
		pDC->SetPixel(p1.x + y, p1.y - x, m_color);
		pDC->SetPixel(p1.x + x, p1.y - y, m_color);
	while (x < y)
	{
		if (d < 0)
		{
			d += 2 * x + 3;
		}
		else
		{
			d += 2 * (x - y) + 5;
			y--;
		}
		x++;
		pDC->SetPixel(p1.x + x, p1.y + y, m_color);
		pDC->SetPixel(p1.x + y, p1.y + x, m_color);
		pDC->SetPixel(p1.x + y, p1.y - x, m_color);
		pDC->SetPixel(p1.x + x, p1.y - y, m_color);
	}

	BresenCircle(pDC, p0, 40);
	BresenCircle(pDC, p1, 5);
	BresenCircle(pDC, p2, 5);
	
	SeedFill4(hdc, p1.x, p1.y, RGB(255, 255, 255), RGB(0, 0, 0));
	SeedFill4(hdc, 60, 55, RGB(255, 255, 255), RGB(0, 0, 0));

}

void CPracticeView::CirPot4(CDC *pDC,CPoint p,int x,int y)
{
	pDC->SetPixel(p.x + x, p.y + y, m_color);
	pDC->SetPixel(p.x + x, p.y - y, m_color);
	pDC->SetPixel(p.x + y, p.y + x, m_color);
	pDC->SetPixel(p.x - y, p.y + x, m_color);
	/*
	pDC->SetPixel(p.x + x, p.y + y, m_color);
	pDC->SetPixel(p.x + x, p.y - y, m_color);
	pDC->SetPixel(p.x - x, p.y - y, m_color);
	pDC->SetPixel(p.x - x, p.y + y, m_color);
	*/
}

void CPracticeView::Mid4Circle(CDC *pDC, CPoint p, int r)
{
	int x, y, d;
	int Y[10000];
	int flag = 0,i=0;
	int X;
	x =X= (r*sqrt(2))/2;
	y = (r*sqrt(2)) / 2;
	d = 2.5-r*(sqrt(2));
	CirPot4(pDC, p, x, y);
	while (x>=-X)
	{
		if (x > 0)
		{
		   if (d < 0)
		   {
		    	d += 4*(y-x)+10;
		    	y++;
		   }
		   else
		   {
			d += 6-4*x;
		   }

		Y[i] = y;
		i++;
		}
		else
		{
			i--;
			y = Y[i];
		}

		x--;
		if (x == 0)
		{
			flag = 1;
		}

		CirPot4(pDC, p, x, y);
	}
	/*
	int x, y, d;
	x = 0;
	y = r;
	d = 1 - r;
	CirPot4(pDC, p, x, y);
	while (x <=r)
	{
		if (d < 0)
		{
			d += 2 * x + 3;
		if(x>=y)
		{
			x++;
		}
		}
		else
		{
			d += 2 * (x - y) + 5;
			if (x < y)
			{
				y--;
			}
		}
		if (x < y)
		{
			x++;
		}
		else
		{
			y--;
		}
		CirPot4(pDC, p, x, y);
	}
	*/
}




void CPracticeView::OnMid4circle()
{
	// TODO: 在此添加命令处理程序代码
	CDC *pDC = GetWindowDC();
	sign = 6;
}








