#include "stdafx.h"
#include "RFU_Debug_Tool.h"
#include "MainDlg.h"
#include "MainFrm.h"
#include "InitDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define TREE_ITEM_WIDTH 50
#define TREE_CTRL_WIDTH 219

IMPLEMENT_DYNAMIC(CMainDlg, CDialog)

CMainDlg::CMainDlg(CWnd* pParent /*=NULL*/)
		: CAttachThreadDlg(CMainDlg::IDD, pParent)
{
	m_pTreeCtrl = NULL;
	m_pTabCtrl = NULL;
	m_pShowingDlg = NULL;
	m_iTabIdxRF  = 0;
	m_iTabIdxDMA = 0;
	m_iTabIdxRFU = 0;
}

CMainDlg::~CMainDlg()
{
	if (m_pTreeCtrl) {
		m_pTreeCtrl->DestroyWindow();
		delete m_pTreeCtrl;
		m_pTreeCtrl = NULL;
	}
	if (m_pTabCtrl) {
		m_pTabCtrl->DestroyWindow();
		delete m_pTabCtrl;
		m_pTabCtrl = NULL;
	}
}

void CMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMainDlg, CDialog)
	ON_NOTIFY(TCN_SELCHANGE, IDC_CARD_SELECT_TAB, &CMainDlg::OnTcnSelchangeCardSelTab)
	ON_NOTIFY(NM_DBLCLK, IDC_FUNCTION_TREE, &CMainDlg::OnNMDblclkFunctionTree)
	ON_NOTIFY(NM_CLICK, IDC_FUNCTION_TREE, &CMainDlg::OnNMClickFunctionTree)
	ON_MESSAGE(WM_CSE_SP3301_INIT, &CMainDlg::Boot)
	ON_MESSAGE(WM_CSE_SINGLE_THREAD_DONE, &CMainDlg::BootDone)
	ON_MESSAGE(WM_CSE_SP3301_CHANGE, &CMainDlg::SP3301Change)
	ON_WM_DRAWITEM()
	ON_WM_SIZE()
END_MESSAGE_MAP()


BOOL CMainDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	this->GetClientRect(&m_crWnd);

	CFont cfTreeCtrl;
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

	m_pTabCtrl = new CMainTabCtrl;
	m_pTabCtrl->Create(WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | TCS_OWNERDRAWFIXED | TCS_FIXEDWIDTH,CRect(TREE_CTRL_WIDTH,0,TREE_CTRL_WIDTH + 828,423),this,IDC_CARD_SELECT_TAB);
	m_pTabCtrl->ShowWindow(SW_NORMAL);
	m_pTabCtrl->SetFont(&cfTabCtrl);
	m_pTabCtrl->InsertItem(0,_T("RF_0"));
	m_pTabCtrl->InsertItem(1,_T("RF_1"));
	m_pTabCtrl->InsertItem(2,_T("RF_2"));
	m_pTabCtrl->InsertItem(3,_T("RF_3"));
	m_pTabCtrl->SetCurSel(0);
	m_pTabCtrl->CreateAllDlg();
	m_pShowingDlg = dynamic_cast<CDialog *>(&m_pTabCtrl->m_pRFR1ADlg[0]);

	logFont.lfHeight = 14;
	cfTreeCtrl.CreateFontIndirect(&logFont);

	m_pTreeCtrl = new CMainTreeCtrl;
	m_pTreeCtrl->Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP | TCS_FOCUSNEVER | TCS_RAGGEDRIGHT | TCS_FORCELABELLEFT | TCS_VERTICAL | TCS_SCROLLOPPOSITE | TCS_BOTTOM | TCS_MULTISELECT,CRect(0,0,TREE_CTRL_WIDTH,423),this,IDC_FUNCTION_TREE);
	m_pTreeCtrl->ShowWindow(SW_NORMAL);
	m_pTreeCtrl->SetFont(&cfTreeCtrl);
	m_pTreeCtrl->SetBkColor(RGB(179,217,255));

	HTREEITEM hRoot  = NULL;
	HTREEITEM hChild = NULL;
	HTREEITEM hChild2 = NULL;
	CString strRoot = " ";
	int32_t iWidth = TREE_ITEM_WIDTH;

	hRoot = m_pTreeCtrl->InsertItem(_T("RF Debug"),TVI_ROOT);
	hChild = m_pTreeCtrl->InsertItem(_T("R1A/B"),hRoot);
	hChild2 = m_pTreeCtrl->InsertItem(EnsureWidth(iWidth,"OverView%25sR1A/B",""),hChild);
	m_pTreeCtrl->SelectItem(hChild2);
	hChild2 = m_pTreeCtrl->InsertItem(EnsureWidth(iWidth,"Advance%35sR1A/B",""),hChild);
	m_pTreeCtrl->Expand(hChild,TVE_EXPAND);

	hChild = m_pTreeCtrl->InsertItem(_T("R1C/D"),hRoot);
	hChild2 = m_pTreeCtrl->InsertItem(EnsureWidth(iWidth,"OverView%25sR1C/D",""),hChild);	
	hChild2 = m_pTreeCtrl->InsertItem(EnsureWidth(iWidth,"Advance%35sR1C/D",""),hChild);
	m_pTreeCtrl->Expand(hChild,TVE_EXPAND);

	hChild = m_pTreeCtrl->InsertItem(EnsureWidth(iWidth,"ARB%40sR1A/B",""),hRoot);
	hChild = m_pTreeCtrl->InsertItem(EnsureWidth(iWidth,"IQ Capture%35sR1A/B",""),hRoot);
	m_pTreeCtrl->Expand(hRoot,TVE_EXPAND);

	hRoot = m_pTreeCtrl->InsertItem(_T("BB Debug"),TVI_ROOT);
	hChild = m_pTreeCtrl->InsertItem(EnsureWidth(iWidth,"OverView%35sR1A/B/C/D",""),hRoot);
	hChild = m_pTreeCtrl->InsertItem(EnsureWidth(iWidth,"ARB%40sR1A/B/C/D",""),hRoot);
	hChild = m_pTreeCtrl->InsertItem(EnsureWidth(iWidth,"IQ Capture%35sR1A/B/C/D",""),hRoot);
	hChild = m_pTreeCtrl->InsertItem(EnsureWidth(iWidth,"DMA"),hRoot);
	hChild = m_pTreeCtrl->InsertItem(EnsureWidth(iWidth,"FPGA"),hRoot);
	m_pTreeCtrl->Expand(hRoot,TVE_EXPAND);

	hRoot = m_pTreeCtrl->InsertItem(_T("Calibration[R1A/B]"),TVI_ROOT);
	hChild = m_pTreeCtrl->InsertItem(EnsureWidth(iWidth,"发本振泄漏%25sR1A/B",""),hRoot);
	hChild = m_pTreeCtrl->InsertItem(EnsureWidth(iWidth,"发边带抑制%25sR1A/B",""),hRoot);
	hChild = m_pTreeCtrl->InsertItem(EnsureWidth(iWidth,"发衰减器误差%25sR1A/B",""),hRoot);
	hChild = m_pTreeCtrl->InsertItem(EnsureWidth(iWidth,"发基准功率%25sR1A/B",""),hRoot);
	hChild = m_pTreeCtrl->InsertItem(EnsureWidth(iWidth,"收SGPMM建表%25sR1A/B",""),hRoot);
	hChild = m_pTreeCtrl->InsertItem(EnsureWidth(iWidth,"收参考电平%25sR1A/B",""),hRoot);
	m_pTreeCtrl->Expand(hRoot,TVE_EXPAND);

	hRoot = m_pTreeCtrl->InsertItem(_T("Calibration[R1C/D]"),TVI_ROOT);
	hChild = m_pTreeCtrl->InsertItem(EnsureWidth(iWidth,"发本振泄漏%25sR1C/D",""),hRoot);
	hChild = m_pTreeCtrl->InsertItem(EnsureWidth(iWidth,"发边带抑制%25sR1C/D",""),hRoot);
	hChild = m_pTreeCtrl->InsertItem(EnsureWidth(iWidth,"发补偿滤波器%25sR1C/D",""),hRoot);
	hChild = m_pTreeCtrl->InsertItem(EnsureWidth(iWidth,"发基准功率%25sR1C/D",""),hRoot);
	hChild = m_pTreeCtrl->InsertItem(EnsureWidth(iWidth,"发衰减器误差%25sR1C/D",""),hRoot);
	hChild = m_pTreeCtrl->InsertItem(EnsureWidth(iWidth,"发滤波器误差%25sR1C/D",""),hRoot);
	hChild = m_pTreeCtrl->InsertItem(EnsureWidth(iWidth,"收补偿滤波器%25sR1C/D",""),hRoot);
	hChild = m_pTreeCtrl->InsertItem(EnsureWidth(iWidth,"收参考电平%25sR1C/D",""),hRoot);
	hChild = m_pTreeCtrl->InsertItem(EnsureWidth(iWidth,"收衰减器误差%25sR1C/D",""),hRoot);
	hChild = m_pTreeCtrl->InsertItem(EnsureWidth(iWidth,"收滤波器误差%25sR1C/D",""),hRoot);
	m_pTreeCtrl->Expand(hRoot,TVE_EXPAND);

	hRoot = m_pTreeCtrl->InsertItem(_T("Test[R1A/B]"),TVI_ROOT);
	hChild = m_pTreeCtrl->InsertItem(EnsureWidth(iWidth,"单板稳定性%25sR1A/B",""),hRoot);
	hChild = m_pTreeCtrl->InsertItem(EnsureWidth(iWidth,"发频率响应%25sR1A/B",""),hRoot);
	hChild = m_pTreeCtrl->InsertItem(EnsureWidth(iWidth,"收频率响应%25sR1A/B",""),hRoot);	
	hChild = m_pTreeCtrl->InsertItem(EnsureWidth(iWidth,"OCXO电压曲线%35sR1A/B",""),hRoot);	
	hChild = m_pTreeCtrl->InsertItem(EnsureWidth(iWidth,"温度特性%35sR1A/B",""),hRoot);
	m_pTreeCtrl->Expand(hRoot,TVE_EXPAND);

	hRoot = m_pTreeCtrl->InsertItem(_T("Test[R1C/D]"),TVI_ROOT);
	hChild = m_pTreeCtrl->InsertItem(EnsureWidth(iWidth,"发频率响应%25sR1C/D",""),hRoot);
	hChild = m_pTreeCtrl->InsertItem(EnsureWidth(iWidth,"收频率响应%25sR1C/D",""),hRoot);
	hChild = m_pTreeCtrl->InsertItem(EnsureWidth(iWidth,"风扇转速vs整机温度%25sR1C/D",""),hRoot);
	hChild = m_pTreeCtrl->InsertItem(EnsureWidth(iWidth,"发基准功率vs射频温度%25sR1C/D",""),hRoot);
	m_pTreeCtrl->Expand(hRoot,TVE_EXPAND);

	m_pTreeCtrl->PostMessage(WM_VSCROLL,SB_TOP,0);

	return TRUE;
}

void CMainDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	if (::IsWindowVisible(m_pTreeCtrl->GetSafeHwnd()))
		m_pTreeCtrl->MoveWindow(0,0,TREE_CTRL_WIDTH,cy);
	if (::IsWindowVisible(m_pTabCtrl->GetSafeHwnd()))
		m_pTabCtrl->MoveWindow(TREE_CTRL_WIDTH,0,cx - TREE_CTRL_WIDTH,cy);
}

void CMainDlg::ModifyTabCtrl()
{
	HTREEITEM hTreeCurItem = m_pTreeCtrl->GetSelectedItem();
	CString strTreeCurItem = m_pTreeCtrl->GetItemText(hTreeCurItem);

	if (strTreeCurItem == EnsureWidth(TREE_ITEM_WIDTH,"OverView%25sR1A/B","")
	 || strTreeCurItem == EnsureWidth(TREE_ITEM_WIDTH,"OverView%25sR1C/D","")
	 || strTreeCurItem == EnsureWidth(TREE_ITEM_WIDTH,"Advance%35sR1A/B","")
	 || strTreeCurItem == EnsureWidth(TREE_ITEM_WIDTH,"Advance%35sR1C/D","")
	 || strTreeCurItem == EnsureWidth(TREE_ITEM_WIDTH,"ARB%40sR1A/B","")
	 || strTreeCurItem == EnsureWidth(TREE_ITEM_WIDTH,"IQ Capture%35sR1A/B","")
	 || strTreeCurItem == _T("Calibration[R1A/B]")
	 || strTreeCurItem == EnsureWidth(TREE_ITEM_WIDTH,"发本振泄漏%25sR1A/B","")
	 || strTreeCurItem == EnsureWidth(TREE_ITEM_WIDTH,"发边带抑制%25sR1A/B","")
	 || strTreeCurItem == EnsureWidth(TREE_ITEM_WIDTH,"发衰减器误差%25sR1A/B","")
	 || strTreeCurItem == EnsureWidth(TREE_ITEM_WIDTH,"发基准功率%25sR1A/B","")
	 || strTreeCurItem == EnsureWidth(TREE_ITEM_WIDTH,"收参考电平%25sR1A/B","")
	 || strTreeCurItem == _T("Calibration[R1C/D]")
	 || strTreeCurItem == EnsureWidth(TREE_ITEM_WIDTH,"发本振泄漏%25sR1C/D","")
	 || strTreeCurItem == EnsureWidth(TREE_ITEM_WIDTH,"发边带抑制%25sR1C/D","")
	 || strTreeCurItem == EnsureWidth(TREE_ITEM_WIDTH,"发补偿滤波器%25sR1C/D","")
	 || strTreeCurItem == EnsureWidth(TREE_ITEM_WIDTH,"发基准功率%25sR1C/D","")
	 || strTreeCurItem == EnsureWidth(TREE_ITEM_WIDTH,"发衰减器误差%25sR1C/D","")
	 || strTreeCurItem == EnsureWidth(TREE_ITEM_WIDTH,"发滤波器误差%25sR1C/D","")
	 || strTreeCurItem == EnsureWidth(TREE_ITEM_WIDTH,"收补偿滤波器%25sR1C/D","")
	 || strTreeCurItem == EnsureWidth(TREE_ITEM_WIDTH,"收参考电平%25sR1C/D","")
	 || strTreeCurItem == EnsureWidth(TREE_ITEM_WIDTH,"收衰减器误差%25sR1C/D","")
	 || strTreeCurItem == EnsureWidth(TREE_ITEM_WIDTH,"收滤波器误差%25sR1C/D","")

	 || strTreeCurItem == EnsureWidth(TREE_ITEM_WIDTH,"单板稳定性%25sR1A/B","")
	 || strTreeCurItem == EnsureWidth(TREE_ITEM_WIDTH,"发频率响应%25sR1A/B","")
	 || strTreeCurItem == EnsureWidth(TREE_ITEM_WIDTH,"收频率响应%25sR1A/B","")
	 || strTreeCurItem == EnsureWidth(TREE_ITEM_WIDTH,"OCXO电压曲线%35sR1A/B","")
	 || strTreeCurItem == EnsureWidth(TREE_ITEM_WIDTH,"温度特性%35sR1A/B","")

	 || strTreeCurItem == EnsureWidth(TREE_ITEM_WIDTH,"发频率响应%25sR1C/D","")
	 || strTreeCurItem == EnsureWidth(TREE_ITEM_WIDTH,"收频率响应%25sR1C/D","")
	 || strTreeCurItem == EnsureWidth(TREE_ITEM_WIDTH,"发基准功率vs射频温度%25sR1C/D","")) {
		 m_pTabCtrl->DeleteAllItems();
		 m_pTabCtrl->InsertItem(0,_T("RF_0"));
		 m_pTabCtrl->InsertItem(1,_T("RF_1"));
		 m_pTabCtrl->InsertItem(2,_T("RF_2"));
		 m_pTabCtrl->InsertItem(3,_T("RF_3"));
		 m_pTabCtrl->SetCurSel(m_iTabIdxRF);
	}
	else if (strTreeCurItem == EnsureWidth(TREE_ITEM_WIDTH,"OverView%35sR1A/B/C/D","")
		  || strTreeCurItem == EnsureWidth(TREE_ITEM_WIDTH,"ARB%40sR1A/B/C/D","")
		  || strTreeCurItem == EnsureWidth(TREE_ITEM_WIDTH,"IQ Capture%35sR1A/B/C/D","")) {
			  m_pTabCtrl->DeleteAllItems();
			  m_pTabCtrl->InsertItem(0,_T("k7-1--->RF0"));
			  m_pTabCtrl->InsertItem(1,_T("k7-1--->RF1"));
			  m_pTabCtrl->InsertItem(2,_T("k7-0--->RF2"));
			  m_pTabCtrl->InsertItem(3,_T("k7-0--->RF3"));
			  m_pTabCtrl->SetCurSel(m_iTabIdxRF);
	}
	else if (strTreeCurItem == EnsureWidth(TREE_ITEM_WIDTH,"DMA")) {
			m_pTabCtrl->DeleteAllItems();
			m_pTabCtrl->InsertItem(0,_T("k7-0"));
			m_pTabCtrl->InsertItem(1,_T("k7-1"));
			m_pTabCtrl->SetCurSel(m_iTabIdxDMA);
	}
	else if (strTreeCurItem == EnsureWidth(TREE_ITEM_WIDTH,"FPGA")) {
			m_pTabCtrl->DeleteAllItems();
			m_pTabCtrl->InsertItem(0,_T("FPGA"));
			m_pTabCtrl->SetCurSel(0);
	}
	else if (strTreeCurItem == EnsureWidth(TREE_ITEM_WIDTH,"收SGPMM建表%25sR1A/B","")) {
			m_pTabCtrl->DeleteAllItems();
			m_pTabCtrl->InsertItem(0,_T("SGPMM"));
			m_pTabCtrl->SetCurSel(0);
	}
	else if (strTreeCurItem == EnsureWidth(TREE_ITEM_WIDTH,"风扇转速vs整机温度%25sR1C/D","")) {
			m_pTabCtrl->DeleteAllItems();
			m_pTabCtrl->InsertItem(0,_T("RFU_0"));
			m_pTabCtrl->InsertItem(1,_T("RFU_1"));
			m_pTabCtrl->InsertItem(2,_T("RFU_2"));
			m_pTabCtrl->InsertItem(3,_T("RFU_3"));
			m_pTabCtrl->InsertItem(4,_T("RFU_4"));
			m_pTabCtrl->SetCurSel(m_iTabIdxRFU);
	}
}

void CMainDlg::ShowDesiredDlg()
{
	HTREEITEM hTreeCurItem = m_pTreeCtrl->GetSelectedItem();
	CString strTreeCurItem = m_pTreeCtrl->GetItemText(hTreeCurItem);
	int iTabIdx = m_pTabCtrl->GetCurSel();

#define SHOW_MAPPED_DLG(tree_item,dlg,tab_idx)								\
	if (tree_item == strTreeCurItem) {										\
		m_pShowingDlg->ShowWindow(SW_HIDE);									\
		m_pTabCtrl->dlg[iTabIdx].ShowWindow(SW_NORMAL);						\
		m_pShowingDlg = dynamic_cast<CDialog *>(&m_pTabCtrl->dlg[iTabIdx]);	\
		tab_idx = iTabIdx;													\
		return;																\
	}

	CDialog *pDlg = NULL;
	if (EnsureWidth(TREE_ITEM_WIDTH,"OverView%25sR1A/B","") == strTreeCurItem) {
		m_pShowingDlg->ShowWindow(SW_HIDE);
		m_pTabCtrl->m_pRFR1ADlg[iTabIdx].ShowWindow(SW_NORMAL);
		m_pTabCtrl->m_pRFR1AGuardDlg[0].ShowWindow(SW_NORMAL);
		m_pShowingDlg = dynamic_cast<CDialog *>(&m_pTabCtrl->m_pRFR1ADlg[iTabIdx]); 
		m_iTabIdxRF = iTabIdx;
		return;
	}
	else {
		m_pTabCtrl->m_pRFR1AGuardDlg[0].ShowWindow(SW_HIDE);
	}

	SHOW_MAPPED_DLG(EnsureWidth(TREE_ITEM_WIDTH,"OverView%25sR1C/D",""),m_pRFR1CDlg,m_iTabIdxRF);
	SHOW_MAPPED_DLG(EnsureWidth(TREE_ITEM_WIDTH,"Advance%35sR1A/B",""),m_pRFR1ADDlg,m_iTabIdxRF);
	SHOW_MAPPED_DLG(EnsureWidth(TREE_ITEM_WIDTH,"Advance%35sR1C/D",""),m_pRFR1CDDlg,m_iTabIdxRF);
	SHOW_MAPPED_DLG(EnsureWidth(TREE_ITEM_WIDTH,"ARB%40sR1A/B",""),m_pRFArbDlg,m_iTabIdxRF);
	SHOW_MAPPED_DLG(EnsureWidth(TREE_ITEM_WIDTH,"IQ Capture%35sR1A/B",""),m_pRFIQCapDlg,m_iTabIdxRF);

	SHOW_MAPPED_DLG(EnsureWidth(TREE_ITEM_WIDTH,"OverView%35sR1A/B/C/D",""),m_pBBDlg,m_iTabIdxRF);
	SHOW_MAPPED_DLG(EnsureWidth(TREE_ITEM_WIDTH,"ARB%40sR1A/B/C/D",""),m_pBBArbDlg,m_iTabIdxRF);
	SHOW_MAPPED_DLG(EnsureWidth(TREE_ITEM_WIDTH,"IQ Capture%35sR1A/B/C/D",""),m_pBBIQCapDlg,m_iTabIdxRF);
	SHOW_MAPPED_DLG(EnsureWidth(TREE_ITEM_WIDTH,"DMA"),m_pDMADlg,m_iTabIdxDMA);
	SHOW_MAPPED_DLG(EnsureWidth(TREE_ITEM_WIDTH,"FPGA"),m_pFPGADlg,iTabIdx);
	
	SHOW_MAPPED_DLG(EnsureWidth(TREE_ITEM_WIDTH,"发本振泄漏%25sR1A/B",""),m_pCalR1ATxLOLeakageDlg,m_iTabIdxRF);
	SHOW_MAPPED_DLG(EnsureWidth(TREE_ITEM_WIDTH,"发边带抑制%25sR1A/B",""),m_pCalR1ATxSidebandDlg,m_iTabIdxRF);
	SHOW_MAPPED_DLG(EnsureWidth(TREE_ITEM_WIDTH,"发衰减器误差%25sR1A/B",""),m_pCalR1ATxAttDlg,m_iTabIdxRF);
	SHOW_MAPPED_DLG(EnsureWidth(TREE_ITEM_WIDTH,"发基准功率%25sR1A/B",""),m_pCalR1ATxPowerDlg,m_iTabIdxRF);
	SHOW_MAPPED_DLG(EnsureWidth(TREE_ITEM_WIDTH,"收SGPMM建表%25sR1A/B",""),m_pCalR1ARxSgpmmDlg,iTabIdx);
	SHOW_MAPPED_DLG(EnsureWidth(TREE_ITEM_WIDTH,"收参考电平%25sR1A/B",""),m_pCalR1ARxRefDlg,m_iTabIdxRF);

	SHOW_MAPPED_DLG(_T("Calibration[R1C/D]"),m_pCalR1CDlg,m_iTabIdxRF);
	SHOW_MAPPED_DLG(EnsureWidth(TREE_ITEM_WIDTH,"发本振泄漏%25sR1C/D",""),m_pCalR1CTxLOLeakageDlg,m_iTabIdxRF);
	SHOW_MAPPED_DLG(EnsureWidth(TREE_ITEM_WIDTH,"发边带抑制%25sR1C/D",""),m_pCalR1CTxSidebandDlg,m_iTabIdxRF);
	SHOW_MAPPED_DLG(EnsureWidth(TREE_ITEM_WIDTH,"发补偿滤波器%25sR1C/D",""),m_pCalR1CTxFilterDlg,m_iTabIdxRF);
	SHOW_MAPPED_DLG(EnsureWidth(TREE_ITEM_WIDTH,"发基准功率%25sR1C/D",""),m_pCalR1CTxPowerDlg,m_iTabIdxRF);
	SHOW_MAPPED_DLG(EnsureWidth(TREE_ITEM_WIDTH,"发衰减器误差%25sR1C/D",""),m_pCalR1CTxAttDlg,m_iTabIdxRF);
	SHOW_MAPPED_DLG(EnsureWidth(TREE_ITEM_WIDTH,"发滤波器误差%25sR1C/D",""),m_pCalR1CTxFilOffDlg,m_iTabIdxRF);
	SHOW_MAPPED_DLG(EnsureWidth(TREE_ITEM_WIDTH,"收补偿滤波器%25sR1C/D",""),m_pCalR1CRxFilterDlg,m_iTabIdxRF);
	SHOW_MAPPED_DLG(EnsureWidth(TREE_ITEM_WIDTH,"收参考电平%25sR1C/D",""),m_pCalR1CRxRefDlg,m_iTabIdxRF);
	SHOW_MAPPED_DLG(EnsureWidth(TREE_ITEM_WIDTH,"收衰减器误差%25sR1C/D",""),m_pCalR1CRxAttDlg,m_iTabIdxRF);
	SHOW_MAPPED_DLG(EnsureWidth(TREE_ITEM_WIDTH,"收滤波器误差%25sR1C/D",""),m_pCalR1CRxFilOffDlg,m_iTabIdxRF);

	SHOW_MAPPED_DLG(EnsureWidth(TREE_ITEM_WIDTH,"单板稳定性%25sR1A/B",""),m_pTestR1ASBFSDlg,m_iTabIdxRF);
	SHOW_MAPPED_DLG(EnsureWidth(TREE_ITEM_WIDTH,"发频率响应%25sR1A/B",""),m_pTestR1ATxFRDlg,m_iTabIdxRF);
	SHOW_MAPPED_DLG(EnsureWidth(TREE_ITEM_WIDTH,"收频率响应%25sR1A/B",""),m_pTestR1ARxFRDlg,m_iTabIdxRF);
	SHOW_MAPPED_DLG(EnsureWidth(TREE_ITEM_WIDTH,"OCXO电压曲线%35sR1A/B",""),m_pTestR1AX9119Dlg,m_iTabIdxRF);
	SHOW_MAPPED_DLG(EnsureWidth(TREE_ITEM_WIDTH,"温度特性%35sR1A/B",""),m_pTestR1ATSDlg,m_iTabIdxRF);

	SHOW_MAPPED_DLG(EnsureWidth(TREE_ITEM_WIDTH,"发频率响应%25sR1C/D",""),m_pTestR1CTxFRDlg,m_iTabIdxRF);
	SHOW_MAPPED_DLG(EnsureWidth(TREE_ITEM_WIDTH,"收频率响应%25sR1C/D",""),m_pTestR1CRxFRDlg,m_iTabIdxRF);
	SHOW_MAPPED_DLG(EnsureWidth(TREE_ITEM_WIDTH,"风扇转速vs整机温度%25sR1C/D",""),m_pTestR1CBTRDlg,m_iTabIdxRFU);
	SHOW_MAPPED_DLG(EnsureWidth(TREE_ITEM_WIDTH,"发基准功率vs射频温度%25sR1C/D",""),m_pTestR1CTTBDlg,m_iTabIdxRF);
}

void CMainDlg::OnTcnSelchangeCardSelTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	ShowDesiredDlg();
	*pResult = 0;
}

void CMainDlg::OnNMDblclkFunctionTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	UINT uFlags = 0;
	CPoint cpHitPoint = GetCurrentMessage()->pt;
	m_pTreeCtrl->ScreenToClient(&cpHitPoint);
	HTREEITEM hItem = m_pTreeCtrl->HitTest(cpHitPoint,&uFlags);
	if ((hItem != NULL) && (TVHT_ONITEM & uFlags))
		m_pTreeCtrl->SelectItem(hItem);
	ModifyTabCtrl();
	ShowDesiredDlg();
	*pResult = 0;
}

void CMainDlg::OnNMClickFunctionTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	OnNMDblclkFunctionTree(pNMHDR,pResult);
	*pResult = 0;
}

BOOL CMainDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
		return TRUE;
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
		return TRUE;
	else
		return CDialog::PreTranslateMessage(pMsg);
}

LRESULT CMainDlg::Boot(WPARAM wParam,LPARAM lParam)
{
	THREAD_EXISTED_CHK();

	m_pThread = new CMainThread(this,0);
	m_pThread->CreateThread();
	return 0;
}

LRESULT CMainDlg::BootDone(WPARAM wParam,LPARAM lParam)
{
	CCSEWinThread::m_bRunning = FALSE;
	m_pTabCtrl->UpdateParamInChildDlg();
	return 0;
}

LRESULT CMainDlg::SP3301Change(WPARAM wParam,LPARAM lParam)
{
	switch (wParam) {
		case 0 : {m_pTabCtrl->m_pSP3301 = &SP3301_0;break;}
		case 1 : {m_pTabCtrl->m_pSP3301 = &SP3301_1;break;}
		case 2 : {m_pTabCtrl->m_pSP3301 = &SP3301_2;break;}
		case 3 : {m_pTabCtrl->m_pSP3301 = &SP3301_3;break;}
		case 4 : {m_pTabCtrl->m_pSP3301 = &SP3301_4;break;}
		default:break;
	}
	m_pTabCtrl->UpdateParamInChildDlg();
	Log->AddMsgList(0,"working on rfu%d",wParam);
	return 0;
}