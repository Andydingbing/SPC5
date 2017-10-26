
// WICOView.cpp : CWICOView 类的实现
//

#include "stdafx.h"
#include "WICO.h"

#include "WICODoc.h"
#include "WICOView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CWICOView

IMPLEMENT_DYNCREATE(CWICOView, CView)

BEGIN_MESSAGE_MAP(CWICOView, CView)
	ON_WM_SIZE()
END_MESSAGE_MAP()

// CWICOView 构造/析构

CWICOView::CWICOView()
{
	// TODO: 在此处添加构造代码
	m_pTabCtrl = new CMainTabCtrl;
}

CWICOView::~CWICOView()
{
	SAFE_DESTROY_DLG(m_pTabCtrl);
}

BOOL CWICOView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CWICOView 绘制

void CWICOView::OnDraw(CDC* /*pDC*/)
{
	CWICODoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}


// CWICOView 诊断

#ifdef _DEBUG
void CWICOView::AssertValid() const
{
	CView::AssertValid();
}

void CWICOView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CWICODoc* CWICOView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CWICODoc)));
	return (CWICODoc*)m_pDocument;
}
#endif //_DEBUG


// CWICOView 消息处理程序

void CWICOView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: 在此添加专用代码和/或调用基类
	CRect crChild;

	CFont cfTabCtrl;
	LOGFONT logFont;

	logFont.lfHeight = 10;
	logFont.lfWidth = 10;
	logFont.lfEscapement = 10;
	logFont.lfOrientation = 10;
	logFont.lfWeight = FW_NORMAL;
	logFont.lfItalic = 0;
	logFont.lfUnderline = 0;
	logFont.lfStrikeOut = 0;
	logFont.lfCharSet = DEFAULT_CHARSET;
	logFont.lfOutPrecision = OUT_DEFAULT_PRECIS;
	logFont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	logFont.lfQuality = PROOF_QUALITY;
	logFont.lfPitchAndFamily = VARIABLE_PITCH | FF_ROMAN;
	cfTabCtrl.CreateFontIndirect(&logFont);

	m_pTabCtrl->Create(WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | TCS_OWNERDRAWFIXED | TCS_FIXEDWIDTH,CRect(0,0,500,400),this,IDC_TRX_TAB);
	m_pTabCtrl->ShowWindow(SW_NORMAL);
	m_pTabCtrl->SetFont(&cfTabCtrl);
	m_pTabCtrl->InsertItem(0,_T("TX"));
	m_pTabCtrl->InsertItem(1,_T("RX"));
	m_pTabCtrl->SetCurSel(0);
	m_pTabCtrl->CreateAllDlg();

	AfxGetMainWnd()->MoveWindow(0,0,crChild.Width(),crChild.Height());
}

void CWICOView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	if (::IsWindowVisible(m_pTabCtrl->GetSafeHwnd())) {
		m_pTabCtrl->MoveWindow(0,0,cx,cy);
	}
}
