
// WICOView.cpp : CWICOView ���ʵ��
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

// CWICOView ����/����

CWICOView::CWICOView()
{
	// TODO: �ڴ˴���ӹ������
	m_pTabCtrl = new CMainTabCtrl;
}

CWICOView::~CWICOView()
{
	SAFE_DESTROY_DLG(m_pTabCtrl);
}

BOOL CWICOView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CWICOView ����

void CWICOView::OnDraw(CDC* /*pDC*/)
{
	CWICODoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
}


// CWICOView ���

#ifdef _DEBUG
void CWICOView::AssertValid() const
{
	CView::AssertValid();
}

void CWICOView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CWICODoc* CWICOView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CWICODoc)));
	return (CWICODoc*)m_pDocument;
}
#endif //_DEBUG


// CWICOView ��Ϣ�������

void CWICOView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: �ڴ����ר�ô����/����û���
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
