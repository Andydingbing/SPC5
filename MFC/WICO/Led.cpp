#include "stdafx.h"
#include "WICO.h"
#include "Led.h"

IMPLEMENT_DYNAMIC(CLed, CWnd)

CLed::CLed(CWnd* pParent /*=NULL*/)
{
	m_Clr = RGB(255,0,0);
}

CLed::~CLed()
{
}

void CLed::DoDataExchange(CDataExchange* pDX)
{
	CWnd::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CLed, CWnd)
	ON_WM_PAINT()
	ON_WM_DRAWITEM()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


BOOL CLed::RegisterWndClass(HINSTANCE hInstance)
{
	LPCTSTR className = _T("CLed");
	WNDCLASS windowclass;
	if (hInstance)
		hInstance = AfxGetInstanceHandle();
	if (!(::GetClassInfo(hInstance,className,&windowclass))) {
		windowclass.style = CS_DBLCLKS;
		windowclass.lpfnWndProc = ::DefWindowProc;
		windowclass.cbClsExtra = windowclass.cbWndExtra = 0;
		windowclass.hInstance = hInstance;
		windowclass.hIcon = NULL;
		windowclass.hCursor = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
		windowclass.hbrBackground = (HBRUSH)::GetStockObject(WHITE_BRUSH);
		windowclass.lpszMenuName = NULL;
		windowclass.lpszClassName = className;
		
		if (!AfxRegisterClass(&windowclass)) {
			AfxThrowResourceException();
			return FALSE;
		}
	}
	return TRUE;
}

void CLed::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CWnd::OnPaint()

	CRect rect;
	this->GetClientRect(&rect);

	Gdiplus::Color Clr;
	Clr.SetFromCOLORREF(m_Clr);

// 	HDC hMemDC = CreateCompatibleDC(this->GetDC()->m_hDC);
// 	HBITMAP hMemBitmap = CreateCompatibleBitmap(this->GetDC()->m_hDC,rect.Width(),rect.Height());
// 	SelectObject(hMemDC,hMemBitmap);
// 
// 	Graphics graphics(hMemDC);
// 	graphics.SetSmoothingMode(SmoothingModeHighQuality);
// 	Gdiplus::Pen pen(Clr);
// 	Gdiplus::SolidBrush brush(Clr);
// 	graphics.FillEllipse(&brush,rect.left,rect.top,rect.Width() - 4,rect.Width() - 4);
// 	graphics.DrawEllipse(&pen,rect.left,rect.top,rect.Width() - 4,rect.Width() - 4);
// 	DeleteObject(hMemBitmap);
// 	BitBlt(this->GetDC()->m_hDC,rect.left,rect.top,rect.Width(),rect.Height(),hMemDC,0,0,SRCCOPY);
// 	DeleteDC(hMemDC);

	CBrush newBrush(m_Clr);
	CBrush *oldBrush = dc.SelectObject(&newBrush);
	CPen newPen(PS_SOLID,1,m_Clr);
	CPen *oldPen = dc.SelectObject(&newPen);
	dc.Rectangle(&rect);
	dc.SelectObject(oldPen);
	dc.SelectObject(oldBrush);
}

void CLed::SetClr(COLORREF clr)
{
	m_Clr = clr;
	RedrawWindow();
}