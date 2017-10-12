#include "stdafx.h"
#include "RFU_Debug_Tool.h"
#include "MainFrm.h"
#include "MainThread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_MESSAGE(WM_CSE_SWHW_VERSION,OnShowVer)
	ON_WM_GETMINMAXINFO()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 状态行指示器
	ID_INDICATOR_K7_0_VERSION,
	ID_INDICATOR_K7_1_VERSION,
	ID_INDICATOR_S6_VERSION,
	ID_INDICATOR_DRIVER_VERSION,
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

CMainFrame::CMainFrame()
{
	// TODO: 在此添加成员初始化代码
}

CMainFrame::~CMainFrame()
{
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("未能创建状态栏\n");
		return -1;      // 未能创建
	}
	return 0;
}

LRESULT CMainFrame::OnShowVer(WPARAM wParam,LPARAM lParam)
{
	CSP3301::RFUInfo Ver = *(CSP3301::RFUInfo *)lParam;
	char *pDriVer = (char *)wParam;
	CString strVer = "";

	strVer.Format("K7_0:%#010x",Ver.m_uiK7_0_Ver);
	m_wndStatusBar.SetPaneText(1,strVer);

	strVer.Format("K7_1:%#010x",Ver.m_uiK7_1_Ver);
	m_wndStatusBar.SetPaneText(2,strVer);

	strVer.Format("S6:%#010x",Ver.m_uiS6_Ver);
	m_wndStatusBar.SetPaneText(3,strVer);

	strVer.Format("Driver:%s",pDriVer);
	m_wndStatusBar.SetPaneText(4,strVer);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
// 	cs.style &= ~WS_MAXIMIZEBOX;	//禁止最大化
// 	cs.style &= ~WS_THICKFRAME;		//禁止改变窗口大小
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return TRUE;
}


// CMainFrame 诊断

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

