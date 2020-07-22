
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

#include "BasicTran2D.h"
#include "Translate2D.h"
#include "Rotate2D.h"
#include "Scale2D.h"

#include "DrawSeal.h"
#include "Transform3D.h"
#include "house.h"
#include "Translate3D.h"
#include "Rotate3D.h"
#include "ScaleH.h"
#include "Symme3D.h"
#define _USE_MATH_DEFINES
#include <math.h>

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
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_COMMAND(ID_drawPoly, &CPracticeView::Ondrawpoly)
	ON_COMMAND(ID_givenChange, &CPracticeView::Ongivenchange)
	ON_COMMAND(ID_polyTranslate, &CPracticeView::Onpolytranslate)
	ON_COMMAND(ID_PolyRotate, &CPracticeView::OnPolyrotate)
	ON_COMMAND(ID_PolyScale, &CPracticeView::OnPolyscale)
	ON_COMMAND(ID_oldSeal, &CPracticeView::Onoldseal)
	ON_COMMAND(ID_newSeal, &CPracticeView::Onnewseal)
	ON_COMMAND(ID_drawline, &CPracticeView::Ondrawline)
	ON_COMMAND(ID_CStailor, &CPracticeView::OnCstailor)
	ON_COMMAND(ID_View, &CPracticeView::OnView)
	ON_COMMAND(ID_volkswagon, &CPracticeView::Onvolkswagon)
	ON_COMMAND(ID_house3D, &CPracticeView::Onhouse3d)
	ON_COMMAND(ID_TranHouse, &CPracticeView::OnTranhouse)
	ON_COMMAND(ID_RotaHouse, &CPracticeView::OnRotahouse)
	ON_COMMAND(ID_ScaleHouse, &CPracticeView::OnScalehouse)
	ON_COMMAND(ID_SymmeHouse, &CPracticeView::OnSymmehouse)
END_MESSAGE_MAP()

// CPracticeView 构造/析构


CPracticeView::CPracticeView()
{
	//m_Select = SEL_NONE;
}

CPracticeView::~CPracticeView()
{

}


BOOL CPracticeView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CPracticeView 绘图

void CPracticeView::OnDraw(CDC* /*pDC*/)
{
	CPracticeDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
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



BasicTran2D Basic2D;


//记录鼠标的点
void CPracticeView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CClientDC dc(this);
	CView::OnLButtonDown(nFlags, point);
	switch (flag)
	{
	case 1:
		
		Basic2D.T[Basic2D.num][0] = point.x;
		Basic2D.T[Basic2D.num][1] = point.y;
		if ((Basic2D.num != 0) && (Basic2D.T[Basic2D.num][0] == Basic2D.T[0][0]) && (Basic2D.T[Basic2D.num][1] == Basic2D.T[0][1]))
		{

		}
		else
		{
			dc.SetPixel(Basic2D.T[Basic2D.num][0], Basic2D.T[Basic2D.num][1], RGB(255, 0, 0));
			Basic2D.T[Basic2D.num][2] = 1;
			Basic2D.num++;

		}
		break;
	case 3:
		p[pp].x = point.x;
		p[pp].y = point.y;
		dc.SetPixel(p[pp], RGB(255, 0, 0));
		pp++;
		break;

	default:
		break;
	}
	
	
}


void CPracticeView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CClientDC dc(this);
	CPen pen1(PS_SOLID, 1, RGB(0, 0, 0));
	CPen pen2(PS_DASH, 1, RGB(100,0, 255));
	
	switch (flag) 
	{
	case 1:
		CView::OnLButtonUp(nFlags, point);
		dc.SelectObject(&pen1);
		if (Basic2D.num > 1)
		{
			dc.MoveTo(Basic2D.T[Basic2D.num - 2][0], Basic2D.T[Basic2D.num - 2][1]);
			dc.LineTo(Basic2D.T[Basic2D.num - 1][0], Basic2D.T[Basic2D.num - 1][1]);
		}
		break;
	case 3:

		if (pp == 2)
		{
			dc.SelectObject(&pen2);
			dc.MoveTo(p[0]);
			dc.LineTo(p[1]);
		}
	default:
		break;

	}
	
	
}


void CPracticeView::Ondrawpoly()
{
	// TODO: 在此添加命令处理程序代码
	flag = 1;
}


void CPracticeView::Ongivenchange()
{
	// TODO: 在此添加命令处理程序代码
	flag = 2;
	CClientDC dc(this);
	CPen pen(PS_SOLID, 1, RGB(255, 0, 0));
	dc.SelectObject(&pen);



	Basic2D.Translate(0, 100, Basic2D.T,Basic2D.num);//沿y轴平移100个单位
	
	

//找多边形的中点
	double midX = 0, midY = 0;
	for (int i = 0; i < Basic2D.num; i++)
	{
		midX += Basic2D.T[i][0];
		midY += Basic2D.T[i][1];
	}

	midX /= Basic2D.num;
	midY /= Basic2D.num;

	Basic2D.Translate(-midX, -midY, Basic2D.T, Basic2D.num);//中点移到原点
	Basic2D.Scale(0.5, 0, 0, 1, Basic2D.T, Basic2D.num);//沿x轴缩小50%
	Basic2D.Scale(1, 0, 0, 0.5, Basic2D.T, Basic2D.num);//沿y轴缩小50%

	Basic2D.Rotate(90, Basic2D.T, Basic2D.num);//绕中心逆时针旋转90°

	Basic2D.Translate(midX, midY, Basic2D.T, Basic2D.num);//中点还原

	//画出变换后的多边形
	for (int i = 1; i < Basic2D.num; i++)
	{
		dc.MoveTo(Basic2D.T[i - 1][0], Basic2D.T[i - 1][1]);
		dc.LineTo(Basic2D.T[i][0], Basic2D.T[i][1]);
	}




}


void CPracticeView::Onpolytranslate()
{
	// TODO: 在此添加命令处理程序代码
	flag = 2;
	double X, Y;
	Translate2D tDlg;
	if (IDOK == tDlg.DoModal())
	{
		X = tDlg.m_Tx;
		Y = tDlg.m_Ty;
	}

	CClientDC dc(this);
	CPen pen(PS_SOLID, 1, RGB(0, 255, 0));
	dc.SelectObject(&pen);
	
	Basic2D.Translate(X, Y, Basic2D.T, Basic2D.num);

	//画出变换后的多边形
	for (int i = 1; i < Basic2D.num; i++)
	{
		dc.MoveTo(Basic2D.T[i - 1][0], Basic2D.T[i - 1][1]);
		dc.LineTo(Basic2D.T[i][0], Basic2D.T[i][1]);
	}

		
	
}


void CPracticeView::OnPolyrotate()
{
	// TODO: 在此添加命令处理程序代码
	flag = 2;
	double Thta;
	Rotate2D rDlg;
	if (IDOK == rDlg.DoModal())
	{
		Thta = rDlg.m_thta;
	}

	flag = 2;
	CClientDC dc(this);
	CPen pen(PS_SOLID, 1, RGB(0, 0, 255));
	dc.SelectObject(&pen);

	//找多边形的中点
	double midX = 0, midY = 0;
	for (int i = 0; i < Basic2D.num; i++)
	{
		midX += Basic2D.T[i][0];
		midY += Basic2D.T[i][1];
	}

	midX /= Basic2D.num;
	midY /= Basic2D.num;

	Basic2D.Translate(-midX, -midY, Basic2D.T, Basic2D.num);//中点移到原点
	Basic2D.Rotate(Thta, Basic2D.T, Basic2D.num);
	Basic2D.Translate(midX, midY, Basic2D.T, Basic2D.num);//中点移到原点


	//画出变换后的多边形
	for (int i = 1; i < Basic2D.num; i++)
	{
		dc.MoveTo(Basic2D.T[i - 1][0], Basic2D.T[i - 1][1]);
		dc.LineTo(Basic2D.T[i][0], Basic2D.T[i][1]);
	}

}


void CPracticeView::OnPolyscale()
{
	// TODO: 在此添加命令处理程序代码
	flag = 2;
	double Sx, Sy;
	Scale2D sDlg;
	if (IDOK == sDlg.DoModal())
	{
		Sx = sDlg.m_Sx;
		Sy = sDlg.m_Sy;
	}

	flag = 2;
	CClientDC dc(this);
	CPen pen(PS_SOLID, 1, RGB(0, 255, 255));
	dc.SelectObject(&pen);


	//找多边形的中点
	double midX = 0, midY = 0;
	for (int i = 0; i < Basic2D.num; i++)
	{
		midX += Basic2D.T[i][0];
		midY += Basic2D.T[i][1];
	}

	midX /= Basic2D.num;
	midY /= Basic2D.num;

	Basic2D.Translate(-midX, -midY, Basic2D.T, Basic2D.num);//中点移到原点
	Basic2D.Scale(Sx/100, 0, 0, Sy/100, Basic2D.T, Basic2D.num);
	Basic2D.Translate(midX, midY, Basic2D.T, Basic2D.num);//中点还原

	//画出变换后的多边形
	for (int i = 1; i < Basic2D.num; i++)
	{
		dc.MoveTo(Basic2D.T[i - 1][0], Basic2D.T[i - 1][1]);
		dc.LineTo(Basic2D.T[i][0], Basic2D.T[i][1]);
	}


}



DrawSeal seal;

void CPracticeView::Onoldseal()
{
	// TODO: 在此添加命令处理程序代码
	flag = 2;
	CClientDC *pDC = new CClientDC(this);

	CBrush brush1(RGB(255, 0, 0));
	CBrush brush2(RGB(255, 255, 255));
	CPen pen(PS_SOLID, 3, RGB(255, 255, 255));


	seal.Seal();
	pDC->SelectObject(brush1);
	pDC->Ellipse(seal.Pold[0][0], seal.Pold[0][1], seal.Pold[1][0], seal.Pold[1][1]);

	pDC->SelectObject(brush2);
	pDC->Ellipse(seal.Pold[2][0], seal.Pold[2][1], seal.Pold[3][0], seal.Pold[3][1]);

	pDC->SelectObject(brush1);
	pDC->Ellipse(seal.Pold[4][0], seal.Pold[4][1], seal.Pold[5][0], seal.Pold[5][1]);

	pDC->SelectObject(brush2);
	pDC->Ellipse(seal.Pold[6][0], seal.Pold[6][1], seal.Pold[7][0], seal.Pold[7][1]);


	pDC->SelectObject(brush1);
	pDC->Ellipse(seal.Pold[8][0], seal.Pold[8][1], seal.Pold[9][0], seal.Pold[9][1]);

	pDC->SelectObject(pen);
	pDC->MoveTo(seal.Pold[10][0], seal.Pold[10][1]);
	pDC->LineTo(seal.Pold[11][0], seal.Pold[11][1]);
	pDC->LineTo(seal.Pold[12][0], seal.Pold[12][1]);

	pDC->MoveTo(seal.Pold[13][0], seal.Pold[13][1]);
	pDC->LineTo(seal.Pold[14][0], seal.Pold[14][1]);

	pDC->MoveTo(seal.Pold[15][0], seal.Pold[15][1]);
	pDC->LineTo(seal.Pold[16][0], seal.Pold[16][1]);

	pDC->MoveTo(seal.Pold[17][0], seal.Pold[17][1]);
	pDC->LineTo(seal.Pold[18][0], seal.Pold[18][1]);

	pDC->MoveTo(seal.Pold[19][0], seal.Pold[19][1]);
	pDC->LineTo(seal.Pold[20][0], seal.Pold[20][1]);

	pDC->MoveTo(seal.Pold[22][0], seal.Pold[22][1]);
	pDC->LineTo(seal.Pold[21][0], seal.Pold[21][1]);

	pDC->MoveTo(seal.Pold[24][0], seal.Pold[24][1]);
	pDC->LineTo(seal.Pold[23][0], seal.Pold[23][1]);

	pDC->MoveTo(seal.Pold[25][0], seal.Pold[25][1]);
	pDC->LineTo(seal.Pold[26][0], seal.Pold[26][1]);

	pDC->MoveTo(seal.Pold[27][0], seal.Pold[27][1]);
	pDC->LineTo(seal.Pold[28][0], seal.Pold[28][1]);
	

}



void CPracticeView::Onnewseal()
{
	// TODO: 在此添加命令处理程序代码
	flag = 2;
	CClientDC *pDC = new CClientDC(this);
	CBrush brush1(RGB(255, 0, 0));
	CBrush brush2(RGB(255, 255, 255));
	CPen pen(PS_SOLID, 3, RGB(255, 255, 255));



	//对称轴 x=700
	for (int i = 0; i <= 28; i++)
	{
		seal.Pnew[i][0] = seal.Pold[i][0];
		seal.Pnew[i][1] = seal.Pold[i][1];
		seal.Pnew[i][2] = 1;
	}


	//对称轴通过原点
	Basic2D.Translate(-700, 0, seal.Pnew,29);


	//关于y轴对称
	Basic2D.Scale(-1, 0, 0, 1, seal.Pnew,29);

	//还原对称轴
	Basic2D.Translate(700, 0, seal.Pnew,29);

	

	pDC->SelectObject(brush1);
	pDC->Ellipse(seal.Pnew[0][0], seal.Pnew[0][1], seal.Pnew[1][0], seal.Pnew[1][1]);

	pDC->SelectObject(brush2);
	pDC->Ellipse(seal.Pnew[2][0], seal.Pnew[2][1], seal.Pnew[3][0], seal.Pnew[3][1]);
	pDC->SelectObject(brush1);
	pDC->Ellipse(seal.Pnew[4][0], seal.Pnew[4][1], seal.Pnew[5][0], seal.Pnew[5][1]);
	

	pDC->SelectObject(brush2);
	pDC->Ellipse(seal.Pnew[6][0], seal.Pnew[6][1], seal.Pnew[7][0], seal.Pnew[7][1]);
	pDC->SelectObject(brush1);
	pDC->Ellipse(seal.Pnew[8][0], seal.Pnew[8][1], seal.Pnew[9][0], seal.Pnew[9][1]);
	
	

	pDC->SelectObject(pen);
	pDC->MoveTo(seal.Pnew[10][0], seal.Pnew[10][1]);
	pDC->LineTo(seal.Pnew[11][0], seal.Pnew[11][1]);
	pDC->LineTo(seal.Pnew[12][0], seal.Pnew[12][1]);
	pDC->MoveTo(seal.Pnew[13][0], seal.Pnew[13][1]);
	pDC->LineTo(seal.Pnew[14][0], seal.Pnew[14][1]);
	pDC->MoveTo(seal.Pnew[15][0], seal.Pnew[15][1]);
	pDC->LineTo(seal.Pnew[16][0], seal.Pnew[16][1]);
	pDC->MoveTo(seal.Pnew[17][0], seal.Pnew[17][1]);
	pDC->LineTo(seal.Pnew[18][0], seal.Pnew[18][1]);
	pDC->MoveTo(seal.Pnew[19][0], seal.Pnew[19][1]);
	pDC->LineTo(seal.Pnew[20][0], seal.Pnew[20][1]);
	pDC->MoveTo(seal.Pnew[21][0], seal.Pnew[21][1]);
	pDC->LineTo(seal.Pnew[22][0], seal.Pnew[22][1]);
	pDC->MoveTo(seal.Pnew[23][0], seal.Pnew[23][1]);
	pDC->LineTo(seal.Pnew[24][0], seal.Pnew[24][1]);
	pDC->MoveTo(seal.Pnew[25][0], seal.Pnew[25][1]);
	pDC->LineTo(seal.Pnew[26][0], seal.Pnew[26][1]);
	pDC->MoveTo(seal.Pnew[27][0], seal.Pnew[27][1]);
	pDC->LineTo(seal.Pnew[28][0], seal.Pnew[28][1]);
	

}


void CPracticeView::Ondrawline()
{
	// TODO: 在此添加命令处理程序代码
	CClientDC *pDC = new CClientDC(this);
	CPen pen(PS_SOLID, 2, RGB(100, 200, 100));
	pDC->SelectObject(pen);
	pDC->Rectangle(Xmin, Ymin, Xmax, Ymax);

	flag = 3;
	
	

}

int CPracticeView::Code(CPoint p0)//计算端点代码
{
	int code = 0;
	if (p0.x < Xmin)
		code |= LEFT;
	
	if (p0.x > Xmax)
		code |= RIGHT;

	if (p0.y < Ymin)
		code |= BOTTOM;

	else if (p0.y > Ymax)
		code |= TOP;

	return code;
}

CPoint CPracticeView::Boundary(int code)//线段与窗口是否有交点处理
{
	CPoint pp;
	if ((LEFT&code) != 0)
	{
		pp.x = Xmin;
		pp.y = p[1].y + (p[1].y - p[0].y)*(Xmin - p[1].x) / (p[1].x - p[0].x);
	}
	else if ((RIGHT&code) != 0)
	{
		pp.x = Xmax;
		pp.y = p[1].y + (p[1].y - p[0].y)*(Xmax - p[1].x) / (p[1].x - p[0].x);
	}
	else if ((BOTTOM&code) != 0)
	{ 
		pp.y = Ymin;
		pp.x = p[1].x + (p[1].x - p[0].x)*(Ymin - p[1].y) / (p[1].y - p[0].y);
	}
	else if ((TOP&code) != 0)
	{
		pp.y = Ymax;
		pp.x = p[1].x + (p[1].x - p[0].x)*(Ymax - p[1].y) / (p[1].y - p[0].y);
	}

	return pp;
}


void CPracticeView::CohenSutherland()
{
	int code_0;
	int code_1;
	int i = 1;

	do
	{
		code_0 = Code(p[0]);
		code_1 = Code(p[1]);

		if ((code_0 | code_1 )== 0)
		{
			i = 0;//线段在窗口内
		}
		else if ((code_0&code_1) != 0)
		{
			p[0].x = 0;
			p[0].y = 0;
			p[1].x = 0;
			p[1].y = 0;
			i = 0;//线段在窗口外
		}
		else
		{
			if (code_0 == 0)
			{
				int code3;
				CPoint P;
				P = p[1];
				p[1] = p[0];
				p[0] = P;


				code3 = code_0;
				code_0 = code_1;
				code_1 = code3;
				
			}

			p[0] = Boundary(code_0);
		}

	} while (i);
	


}



void CPracticeView::OnCstailor()
{
	// TODO: 在此添加命令处理程序代码
	CClientDC *pDC = new CClientDC(this);
	flag = 2;

	CohenSutherland();



	CPen pen(PS_SOLID, 1, RGB(0, 200, 100));
	pDC->SelectObject(pen);
	pDC->MoveTo(p[0]);
	pDC->LineTo(p[1]);

}




void CPracticeView::OnView()
{
	// TODO: 在此添加命令处理程序代码
	int i,j;
	Transform3D tran;

	//变换矩阵清零
	for (i = 1; i <= 4; ++i)
	{
		for (j = 1; j <= 4; ++j)
		{
			tran.A[i][j] = 0;
			tran.A1[i][j] = 0;
			tran.Ah[i][j] = 0;
			tran.Aw[i][j] = 0;
		}
	}


	tran.theta_y = 45;
	tran.phi_x = 125;
	//给正等测变换矩阵赋值
	tran.A[1][1] = (float)cos(tran.theta_y*M_PI / 180);
	tran.A[1][2] = (float)sin(tran.theta_y*M_PI / 180)*(float)sin(tran.phi_x*M_PI / 180);
	tran.A[2][2] = (float)cos(tran.phi_x*M_PI / 180);
	tran.A[3][1] = (float)sin(tran.theta_y*M_PI / 180);
	tran.A[3][2] = (float)-cos(tran.theta_y*M_PI / 180)*(float)sin(tran.phi_x*M_PI / 180);
	tran.A[4][4] = 1;


	//给主视图变换矩阵赋值
	tran.A1[1][1] = 1;
	tran.A1[3][3] = 1;
	tran.A1[4][4] = 1;

	//俯视图
	tran.Ah[1][1] = 1;
	tran.Ah[2][3] = -1;
	tran.Ah[4][4] = 1;


	//侧视图
	tran.Aw[2][1] = -1;
	tran.Aw[3][3] = 1;
	tran.Aw[4][4] = 1;

	tran.Display();
}


void CPracticeView::drawlogo(CDC* pDC,double logo1[][3])
{

	CBrush brush(RGB(255, 255, 255));
	CPen pen(PS_SOLID, 1, RGB(255, 255, 255));
	CPoint L[6];

	for (int i = 0; i < 5; i++)
	{
		L[i].x = logo1[i][0];
		L[i].y = logo1[i][1];
	}

	L[5].x = L[0].x;
	L[5].y = L[0].y;

	pDC->SelectObject(brush);
	pDC->Polygon(L, 5);
	pDC->SelectObject(pen);
	pDC->Polyline(L, 6);

	CPoint L2[8];
	for (int i = 0; i < 7; i++)
	{
		L2[i].x = logo1[5+i][0];
		L2[i].y = logo1[5+i][1];
	}
	
	L2[7].x = L2[0].x;
	L2[7].y = L2[0].y;

	pDC->SelectObject(brush);
	pDC->Polygon(L2, 7);
	pDC->SelectObject(pen);
	pDC->Polyline(L2, 8);
}


void CPracticeView::Onvolkswagon()
{
	// TODO: 在此添加命令处理程序代码

	CClientDC *pDC = new CClientDC(this);

	CBrush brush(RGB(255, 255, 255));
	CBrush brush2(RGB(54, 100, 139));
	CPen pen(PS_SOLID, 1, RGB(255, 255, 255));
	
	pDC->SelectObject(brush2);
	pDC->Rectangle(150, 150, 850, 850);

	pDC->SelectObject(brush);
	pDC->Ellipse(200, 200, 800, 800);
	pDC->SelectObject(pen);
	pDC->Ellipse(200, 200, 800, 800);

	pDC->SelectObject(brush2);
	pDC->Ellipse(255, 255, 745, 745);
	pDC->SelectObject(pen);
	pDC->Ellipse(255, 255, 745, 745);

	logo[0][0] = 354;
	logo[0][1] = 275;
	logo[0][2] = 1;

	logo[1][0] = 430;
	logo[1][1] = 263;
	logo[1][2] = 1;

	logo[2][0] = 500;
	logo[2][1] = 420;
	logo[2][2] = 1;

	logo[3][0] = 500;
	logo[3][1] = 490;
	logo[3][2] = 1;

	logo[4][0] = 450;
	logo[4][1] = 490;
	logo[4][2] = 1;


	logo[5][0] = 450;
	logo[5][1] = 510;
	logo[5][2] = 1;

	logo[6][0] = 500;
	logo[6][1] = 510;
	logo[6][2] = 1;

	logo[7][0] = 500;
	logo[7][1] = 580;
	logo[7][2] = 1;

	logo[8][0] = 420;
	logo[8][1] = 759;
	logo[8][2] = 1;

	logo[9][0] = 265;
	logo[9][1] = 412;
	logo[9][2] = 1;

	logo[10][0] = 301;
	logo[10][1] = 311;
	logo[10][2] = 1;

	logo[11][0] = 420;
	logo[11][1] = 577;
	logo[11][2] = 1;


	drawlogo(pDC, logo);

	//对称变换得到右半边

	for (int i = 0; i < 12; i++)
	{
		logo2[i][0] = logo[i][0];
		logo2[i][1] = logo[i][1];
		logo2[i][2] = logo[i][2];
	}

	//对称轴x=500通过原点
	Basic2D.Translate(-500, 0, logo2, 12);


	//关于y轴对称
	Basic2D.Scale(-1, 0, 0, 1, logo2, 12);

	//还原对称轴
	Basic2D.Translate(500, 0, logo2, 12);

	drawlogo(pDC, logo2);
}



house House;

void CPracticeView::Onhouse3d()
{
	CClientDC *pDC = new CClientDC(this);
	CPen pen(PS_SOLID, 1, RGB(0, 255, 0));
	pDC->SelectObject(&pen);
	House.draw(pDC);

}


void CPracticeView::OnTranhouse()
{
	// TODO: 在此添加命令处理程序代码
	CClientDC *pDC = new CClientDC(this);
	int x, y, z;
	Translate3D tDlg;
	if (IDOK == tDlg.DoModal())
	{
		x = -tDlg.transX;
		y = -tDlg.transY;
		z = -tDlg.transZ;
	}

	CPen pen(PS_SOLID, 1, RGB(255, 0, 0));
	pDC->SelectObject(&pen);

	House.Translate3D(x, y,z, House.T, House.num);

	House.draw(pDC);


}


void CPracticeView::OnRotahouse()
{
	// TODO: 在此添加命令处理程序代码

	CClientDC *pDC = new CClientDC(this);
	CString A;
	double thta;
	Rotate3D tDlg;
	if (IDOK == tDlg.DoModal())
	{
		A = tDlg.rotateA;
		thta = -tDlg.rotatethta;
	}

	CPen pen(PS_SOLID, 1, RGB(0, 0,255));
	pDC->SelectObject(&pen);

	House.Rotate3D(thta,A, House.T, House.num);

	House.draw(pDC);



}


void CPracticeView::OnScalehouse()
{
	// TODO: 在此添加命令处理程序代码
	CClientDC *pDC = new CClientDC(this);
	double h;
	ScaleH tDlg;
	if (IDOK == tDlg.DoModal())
	{
		h = tDlg.Scaleh;
	}

	CPen pen(PS_SOLID, 1, RGB(0, 255, 255));
	pDC->SelectObject(&pen);

	House.Scale3D(h, h, h, House.T, House.num);

	House.draw(pDC);

}


void CPracticeView::OnSymmehouse()
{
	// TODO: 在此添加命令处理程序代码

	CClientDC *pDC = new CClientDC(this);
	CString str1;
	Symme3D sDlg;
	if (IDOK == sDlg.DoModal())
	{
		str1 = sDlg.str;
	}

	CPen pen(PS_SOLID, 1, RGB(255,0 , 255));
	pDC->SelectObject(&pen);


	if (str1 == XOY)
	{
		House.Scale3D(1, 1, -1, House.T, House.num);
		House.Translate3D(200, 0, 500, House.T, House.num);
		House.draw(pDC);
	}
	else if (str1 == YOZ)
	{
		House.Scale3D(-1, 1, 1, House.T, House.num);
		House.draw(pDC);
	}
	else if (str1 == ZOX)
	{
		House.Scale3D(1, -1, 1, House.T, House.num);
		House.draw(pDC);
	}

	

	

	
}
