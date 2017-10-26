#include "stdafx.h"
#include "RFU_Debug_Tool.h"
#include "RFU_Debug_ToolDoc.h"
#include "RFU_Debug_ToolView.h"
#include "CheckDlg.h"
#include "IQCaptureThread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CCSEView, CView)

BEGIN_MESSAGE_MAP(CCSEView, CView)
	ON_WM_CREATE()
	ON_COMMAND(ID_ARB_WV, &CCSEView::OnOpenIQChartDlg)
	ON_COMMAND(ID_TRANS_TO_WV, &CCSEView::OnOpenWaveVisionDlg)
	ON_COMMAND(ID_ALL_IQCAP_START, &CCSEView::OnStartAllIQCapture)
	ON_COMMAND(ID_ALL_IQCAP_STOP, &CCSEView::OnStopAllIQCapture)
	ON_COMMAND(ID_INSTR_CTRL, &CCSEView::OnOpenInstrDlg)
	ON_COMMAND(ID_LOG_CONFIG, &CCSEView::OnOpenLogConfigDlg)
	ON_COMMAND(ID_CALC, &CCSEView::OnOpenCalc)
	ON_COMMAND(ID_NIIOTRACE, &CCSEView::OnOpenNIIOTrace)
	ON_COMMAND(ID_DEVICE_INIT, &CCSEView::SP3301Init)
	ON_COMMAND(ID_DEVICE_REBOOT, &CCSEView::OnDeviceReboot)
	ON_COMMAND(ID_DEVICE_SHUTDOWN, &CCSEView::OnDeviceShutdown)
	ON_COMMAND(ID_PCIE_RESTART, &CCSEView::OnPCIERestart)
	ON_MESSAGE(WM_CSE_IQ_CHART, &CCSEView::SendMessageToIQChartDlg)
	ON_COMMAND(ID_SP3301_0, &CCSEView::OnSelSP33010)
	ON_UPDATE_COMMAND_UI(ID_SP3301_0, &CCSEView::OnUpdateSP33010)
	ON_COMMAND(ID_SP3301_1, &CCSEView::OnSelSP33011)
	ON_UPDATE_COMMAND_UI(ID_SP3301_1, &CCSEView::OnUpdateSP33011)
	ON_COMMAND(ID_SP3301_2, &CCSEView::OnSelSP33012)
	ON_UPDATE_COMMAND_UI(ID_SP3301_2, &CCSEView::OnUpdateSP33012)
	ON_COMMAND(ID_SP3301_3, &CCSEView::OnSelSP33013)
	ON_UPDATE_COMMAND_UI(ID_SP3301_3, &CCSEView::OnUpdateSP33013)
	ON_COMMAND(ID_SP3301_4, &CCSEView::OnSelSP33014)
	ON_UPDATE_COMMAND_UI(ID_SP3301_4, &CCSEView::OnUpdateSP33014)
	ON_WM_SIZE()
	ON_COMMAND(ID_TUNE_VCXO, &CCSEView::OnTuneVcxo)
	ON_COMMAND(ID_TEST_ALGORITHM_5355, &CCSEView::OnTestAlgorithm5355)
	ON_COMMAND(ID_LOG_VIEWER, &CCSEView::OnOpenLogViewerDlg)
	ON_COMMAND(ID_TEST_PROGRAMBIT, &CCSEView::OnTestProgrambit)
	ON_COMMAND(ID_TEST_FITTINGINTERPOLATION, &CCSEView::OnTestFittingInterp)
END_MESSAGE_MAP()


CCSEView::CCSEView()
{
	g_pMainDlg = NULL;
	g_pADF5355Dlg = NULL;
	g_pHMC1197Dlg = NULL;
	g_pLogDlg = NULL;
	g_pLogViewerDlg = NULL;
	g_pTestCMPBDlg = NULL;
	g_pTestAlgo5355Dlg = NULL;
	g_pTestFittingInterpDlg = NULL;
}

CCSEView::~CCSEView()
{
	SAFE_DESTROY_DLG(g_pMainDlg);
	SAFE_DESTROY_DLG(g_pADF5355Dlg);
	SAFE_DESTROY_DLG(g_pHMC1197Dlg);
	SAFE_DESTROY_DLG(g_pLogDlg);
	SAFE_DESTROY_DLG(g_pLogViewerDlg);
	SAFE_DESTROY_DLG(g_pTestCMPBDlg);
	SAFE_DESTROY_DLG(g_pTestAlgo5355Dlg);
	SAFE_DESTROY_DLG(g_pTestFittingInterpDlg);
	m_InstrumentDlg.DestroyWindow();
	m_ArbIQChartDlg.DestroyWindow();
}

BOOL CCSEView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式
	return CView::PreCreateWindow(cs);
}

// CCSEView 绘制

void CCSEView::OnDraw(CDC* /*pDC*/)
{
	CCSEDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}


// CCSEView 诊断

#ifdef _DEBUG
void CCSEView::AssertValid() const
{
	CView::AssertValid();
}

void CCSEView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CCSEDoc* CCSEView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCSEDoc)));
	return (CCSEDoc*)m_pDocument;
}
#endif //_DEBUG


int CCSEView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	return 0;
}

void CCSEView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	g_pMainDlg = new CMainDlg;
	g_pMainDlg->Create(CMainDlg::IDD,this);
	g_pMainDlg->MoveWindow(0,0,MAIN_DLG_INITIAL_WIDTH,MAIN_DLG_INITIAL_HEIGHT,TRUE);
	g_pMainDlg->ShowWindow(SW_NORMAL);

	g_pADF5355Dlg = new CADF5355Dlg;
	g_pADF5355Dlg->Create(CADF5355Dlg::IDD,GetDesktopWindow());
	g_pADF5355Dlg->ShowWindow(SW_HIDE);

	g_pHMC1197Dlg = new CHMC1197Dlg;
	g_pHMC1197Dlg->Create(CHMC1197Dlg::IDD,GetDesktopWindow());
	g_pHMC1197Dlg->ShowWindow(SW_HIDE);

	g_pLogDlg = new CLogDlg;
	g_pLogDlg->Create(CLogDlg::IDD,this);
	g_pLogDlg->MoveWindow(0,MAIN_DLG_INITIAL_HEIGHT,MAIN_DLG_INITIAL_WIDTH,MAIN_DLG_INITIAL_HEIGHT / MAIN_LOG_DLG_HEIGHT_RATIO,TRUE);
	g_pLogDlg->ShowWindow(SW_NORMAL);

	g_pLogViewerDlg = new CLogViewerDlg;
	g_pLogViewerDlg->Create(CLogViewerDlg::IDD,GetDesktopWindow());
	g_pLogViewerDlg->ShowWindow(SW_HIDE);

	g_pTestCMPBDlg = new CTestCMPBDlg;
	g_pTestCMPBDlg->Create(CTestCMPBDlg::IDD,GetDesktopWindow());
	g_pTestCMPBDlg->ShowWindow(SW_HIDE);

	g_pTestAlgo5355Dlg = new CTestAlgo5355Dlg;
	g_pTestAlgo5355Dlg->Create(CTestAlgo5355Dlg::IDD,GetDesktopWindow());
	g_pTestAlgo5355Dlg->ShowWindow(SW_HIDE);

	g_pTestFittingInterpDlg = new CTestFittingInterpDlg;
	g_pTestFittingInterpDlg->Create(CTestFittingInterpDlg::IDD,GetDesktopWindow());
	g_pTestFittingInterpDlg->ShowWindow(SW_HIDE);

	AfxGetMainWnd()->MoveWindow(0,0,MAIN_DLG_INITIAL_WIDTH + 20, MAIN_DLG_INITIAL_HEIGHT + MAIN_DLG_INITIAL_HEIGHT / MAIN_LOG_DLG_HEIGHT_RATIO + 80,TRUE);

	vector<HWND> hWnd;
	hWnd.push_back(g_pLogDlg->GetSafeHwnd());
	hWnd.push_back(g_pLogViewerDlg->GetSafeHwnd());
	Log.Init(hWnd);
}

void CCSEView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	int iHeightLogDlg = cy / (MAIN_LOG_DLG_HEIGHT_RATIO + 1);
	int iHeightMainDlg = cy - iHeightLogDlg;

	if (::IsWindowVisible(g_pMainDlg->GetSafeHwnd()))
		g_pMainDlg->MoveWindow(0,0,cx,iHeightMainDlg);
	if (::IsWindowVisible(g_pLogDlg->GetSafeHwnd()))
		g_pLogDlg->MoveWindow(0,iHeightMainDlg,cx,iHeightLogDlg);
}

void CCSEView::OnOpenIQChartDlg()
{
	if (!::IsWindow(m_ArbIQChartDlg.GetSafeHwnd()))
		m_ArbIQChartDlg.Create(CArbIQChartDlg::IDD, GetDesktopWindow());
	m_ArbIQChartDlg.ShowWindow(SW_NORMAL);
	m_ArbIQChartDlg.SetFocus();
}

void CCSEView::OnOpenWaveVisionDlg()
{
	CWaveVisionDlg *pWVDlg = new CWaveVisionDlg;
	pWVDlg->Create(CWaveVisionDlg::IDD,this);
	pWVDlg->ShowWindow(SW_NORMAL);
}

void CCSEView::OnStartAllIQCapture()
{
	TEST_THREAD_EXISTED_CHKV();
	CAttachThreadDlg::m_pThread = new CIQCaptureThread(this);
	CAttachThreadDlg::m_pThread->CreateThread();
}

void CCSEView::OnStopAllIQCapture()
{
	CCSEWinThread::m_bRunning = FALSE;
}

void CCSEView::OnOpenInstrDlg()
{
	if (!::IsWindow(m_InstrumentDlg.GetSafeHwnd()))
		m_InstrumentDlg.Create(CInstrumentDlg::IDD,GetDesktopWindow());
	m_InstrumentDlg.ShowWindow(SW_NORMAL);
	m_InstrumentDlg.SetFocus();
// 	m_InstrumentDlg.SetWindowPos(&CWnd::wndTopMost,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);		//置顶对话框
}

void CCSEView::OnOpenLogConfigDlg()
{
	if (!::IsWindow(m_LogConfigDlg.GetSafeHwnd()))
		m_LogConfigDlg.Create(CLogConfigDlg::IDD,this);
	m_LogConfigDlg.ShowWindow(SW_NORMAL);
	m_LogConfigDlg.SetFocus();
}

void CCSEView::OnOpenCalc()
{
 	system("start /b CALC");
}

void CCSEView::OnOpenNIIOTrace()
{
	system("start /b C:\\\"Program Files (x86)\"\\\"National Instruments\"\\\"NI Spy\"\\\"NI Spy.exe\"");
}

void CCSEView::SP3301Init()
{
	if (::IsWindowVisible(g_pMainDlg->GetSafeHwnd()))
		::PostMessage(g_pMainDlg->GetSafeHwnd(),WM_CSE_SP3301_INIT,0,0);
}

void CCSEView::OnDeviceReboot()
{
	CCheckDlg CheckDlg("Restart CXU");
	if (CheckDlg.DoModal() == IDOK)
		system("shutdown -r -t 1");
}

void CCSEView::OnDeviceShutdown()
{
	CCheckDlg CheckDlg("Shutdown CXU");
	if (CheckDlg.DoModal() == IDOK)
		system("shutdown -s -t 1");
}

void CCSEView::OnPCIERestart()
{
	system("devcon_x64.exe restart \"PCI\\VEN_10EE&DEV_7021\"");
	Sleep(1000);
}

LRESULT CCSEView::SendMessageToIQChartDlg(WPARAM wParam,LPARAM lParam)
{
	if (m_ArbIQChartDlg.IsWindowVisible())
		return ::PostMessage(m_ArbIQChartDlg.GetSafeHwnd(),WM_PASTE,wParam,lParam);
	return 0;
}

void CCSEView::OnSelSP33010()
{
	CCSEDoc *pDoc = GetDocument();
	pDoc->m_bSP33010Enable = true;
	pDoc->m_bSP33011Enable = false;
	pDoc->m_bSP33012Enable = false;
	pDoc->m_bSP33013Enable = false;
	pDoc->m_bSP33014Enable = false;
	::SendMessage(g_pMainDlg->GetSafeHwnd(),WM_CSE_SP3301_CHANGE,0,0);
}

void CCSEView::OnUpdateSP33010(CCmdUI *pCmdUI)
{
	CCSEDoc *pDoc = GetDocument();
 	pCmdUI->SetCheck(pDoc->m_bSP33010Enable);
}

void CCSEView::OnSelSP33011()
{
	CCSEDoc *pDoc = GetDocument();
	pDoc->m_bSP33010Enable = false;
	pDoc->m_bSP33011Enable = true;
	pDoc->m_bSP33012Enable = false;
	pDoc->m_bSP33013Enable = false;
	pDoc->m_bSP33014Enable = false;
	::SendMessage(g_pMainDlg->GetSafeHwnd(),WM_CSE_SP3301_CHANGE,1,0);
}

void CCSEView::OnUpdateSP33011(CCmdUI *pCmdUI)
{
	CCSEDoc *pDoc = GetDocument();
	pCmdUI->SetCheck(pDoc->m_bSP33011Enable);
}

void CCSEView::OnSelSP33012()
{
	CCSEDoc *pDoc = GetDocument();
	pDoc->m_bSP33010Enable = false;
	pDoc->m_bSP33011Enable = false;
	pDoc->m_bSP33012Enable = true;
	pDoc->m_bSP33013Enable = false;
	pDoc->m_bSP33014Enable = false;
	::SendMessage(g_pMainDlg->GetSafeHwnd(),WM_CSE_SP3301_CHANGE,2,0);
}

void CCSEView::OnUpdateSP33012(CCmdUI *pCmdUI)
{
	CCSEDoc *pDoc = GetDocument();
	pCmdUI->SetCheck(pDoc->m_bSP33012Enable);
}

void CCSEView::OnSelSP33013()
{
	CCSEDoc *pDoc = GetDocument();
	pDoc->m_bSP33010Enable = false;
	pDoc->m_bSP33011Enable = false;
	pDoc->m_bSP33012Enable = false;
	pDoc->m_bSP33013Enable = true;
	pDoc->m_bSP33014Enable = false;
	::SendMessage(g_pMainDlg->GetSafeHwnd(),WM_CSE_SP3301_CHANGE,3,0);
}

void CCSEView::OnUpdateSP33013(CCmdUI *pCmdUI)
{
	CCSEDoc *pDoc = GetDocument();
	pCmdUI->SetCheck(pDoc->m_bSP33013Enable);
}

void CCSEView::OnSelSP33014()
{
	CCSEDoc *pDoc = GetDocument();
	pDoc->m_bSP33010Enable = false;
	pDoc->m_bSP33011Enable = false;
	pDoc->m_bSP33012Enable = false;
	pDoc->m_bSP33013Enable = false;
	pDoc->m_bSP33014Enable = true;
	::SendMessage(g_pMainDlg->GetSafeHwnd(),WM_CSE_SP3301_CHANGE,4,0);
}

void CCSEView::OnUpdateSP33014(CCmdUI *pCmdUI)
{
	CCSEDoc *pDoc = GetDocument();
	pCmdUI->SetCheck(pDoc->m_bSP33014Enable);
}

void CCSEView::OnTuneVcxo()
{
	if (!::IsWindow(m_X9119Dlg.GetSafeHwnd()))
		m_X9119Dlg.Create(CX9119Dlg::IDD,this);
	m_X9119Dlg.ShowWindow(SW_NORMAL);
	m_X9119Dlg.SetFocus();
}

void CCSEView::OnTestProgrambit()
{
	g_pTestCMPBDlg->ShowWindow(SW_NORMAL);
	g_pTestCMPBDlg->SetActiveWindow();
}

void CCSEView::OnTestAlgorithm5355()
{
	g_pTestAlgo5355Dlg->ShowWindow(SW_NORMAL);
	g_pTestAlgo5355Dlg->SetActiveWindow();
}

void CCSEView::OnTestFittingInterp()
{
	g_pTestFittingInterpDlg->ShowWindow(SW_NORMAL);
	g_pTestFittingInterpDlg->SetActiveWindow();
}

void CCSEView::OnOpenLogViewerDlg()
{
	g_pLogViewerDlg->ShowWindow(SW_NORMAL);
}