#include "stdafx.h"
#include "RFU_Debug_Tool.h"
#include "MainTabCtrl.h"
#include "InitDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define RF_R1A_DLG_BOTTOM 260

IMPLEMENT_DYNAMIC(CMainTabCtrl, CTabCtrl)

CMainTabCtrl::CMainTabCtrl()
{
	m_pSP3301 = &SP3301_0;
	m_pSP3501 = &SP3501;

	m_pRFR1ADlg = new CRFR1ADlg[MAX_RF];
	m_pRFR1AGuardDlg = new CRFR1AGuardDlg[1];
	m_pRFR1CDlg = new CRFR1CDlg[MAX_RF];
	m_pRFR1ADDlg = new CRFR1ADDlg[MAX_RF];
	m_pRFR1CDDlg = new CRFR1CDDlg[MAX_RF];
	m_pRFArbDlg = new CRFArbDlg[MAX_RF];
	m_pRFIQCapDlg = new CRFIQCaptureDlg[MAX_RF];

	m_pBBDlg = new CBBDlg[MAX_RF];
	m_pBBArbDlg = new CBBArbDlg[MAX_RF];
	m_pBBIQCapDlg = new CBBIQCaptureDlg[MAX_RF];

	
	m_pCalR1ATxLOLeakageDlg = new CCalR1ATxLOLeakageDlg[MAX_RF];
	m_pCalR1ATxSidebandDlg = new CCalR1ATxSideBandDlg[MAX_RF];
	m_pCalR1ATxAttDlg = new CCalR1ATxAttDlg[MAX_RF];
	m_pCalR1ATxPowerDlg = new CCalR1ATxPowerDlg[MAX_RF];
	m_pCalR1ARxSgpmmDlg = new CCalRxSgpmmDlg[1];
	m_pCalR1ARxRefDlg = new CCalR1ARxRefDlg[MAX_RF];

	m_pCalR1CDlg = new CCalR1CDlg[MAX_RF];
	m_pCalR1CTxLOLeakageDlg = new CCalR1CTxLOLeakageDlg[MAX_RF];
	m_pCalR1CTxSidebandDlg = new CCalR1CTxSidebandDlg[MAX_RF];
	m_pCalR1CTxFilterDlg = new CCalR1CTxFilterDlg[MAX_RF];
	m_pCalR1CTxPowerDlg = new CCalR1CTxPowerDlg[MAX_RF];
	m_pCalR1CTxAttDlg = new CCalR1CTxAttDlg[MAX_RF];
	m_pCalR1CTxFilOffDlg = new CCalR1CTxFilOffDlg[MAX_RF];
	m_pCalR1CRxFilterDlg = new CCalR1CRxFilterDlg[MAX_RF];
	m_pCalR1CRxRefDlg = new CCalR1CRxRefDlg[MAX_RF];
	m_pCalR1CRxAttDlg = new CCalR1CRxAttDlg[MAX_RF];
	m_pCalR1CRxFilOffDlg = new CCalR1CRxFilOffDlg[MAX_RF];

	m_pTestR1ASBFSDlg = new CTestR1ASBFSDlg[MAX_RF];
	m_pTestR1ATxFRDlg = new CTestR1ATxFRDlg[MAX_RF];
	m_pTestR1ARxFRDlg = new CTestR1ARxFRDlg[MAX_RF];
	m_pTestR1AX9119Dlg = new CTestR1AX9119Dlg[MAX_RF];
	m_pTestR1ATSDlg = new CTestR1ATSDlg[MAX_RF];

	m_pTestR1CTxFRDlg = new CTestR1CTxFRDlg[MAX_RF];
	m_pTestR1CRxFRDlg = new CTestR1CRxFRDlg[MAX_RF];
	m_pTestR1CBTRDlg = new CTestR1CBTRDlg[MAX_RFU];
	m_pTestR1CTTBDlg = new CTestR1CTTBDlg[MAX_RF];

	m_pDMADlg = new CDMADlg[2];
	m_pFPGADlg = new CFPGADlg[1];

	UpdateParamInChildDlg();
}

CMainTabCtrl::~CMainTabCtrl()
{
#define SAFE_DESTROY_WINDOW(wnd,num)	\
	for (int32_t i = 0;i < num;i ++) {	\
		if (wnd)						\
			wnd[i].DestroyWindow();		\
	}									\
	if (wnd) {							\
		delete []wnd;					\
		wnd = NULL;						\
	}

	SAFE_DESTROY_WINDOW(m_pRFR1ADlg,MAX_RF);
	SAFE_DESTROY_WINDOW(m_pRFR1AGuardDlg,1);
	SAFE_DESTROY_WINDOW(m_pRFR1CDlg,MAX_RF);
	SAFE_DESTROY_WINDOW(m_pRFR1ADDlg,MAX_RF);
	SAFE_DESTROY_WINDOW(m_pRFR1CDDlg,MAX_RF);
	SAFE_DESTROY_WINDOW(m_pRFArbDlg,MAX_RF);
	SAFE_DESTROY_WINDOW(m_pRFIQCapDlg,MAX_RF);

	SAFE_DESTROY_WINDOW(m_pBBDlg,MAX_RF);
	SAFE_DESTROY_WINDOW(m_pBBArbDlg,MAX_RF);
	SAFE_DESTROY_WINDOW(m_pBBIQCapDlg,MAX_RF);
	SAFE_DESTROY_WINDOW(m_pDMADlg,2);
	SAFE_DESTROY_WINDOW(m_pFPGADlg,1);

	SAFE_DESTROY_WINDOW(m_pCalR1ATxLOLeakageDlg,MAX_RF);
	SAFE_DESTROY_WINDOW(m_pCalR1ATxSidebandDlg,MAX_RF);
	SAFE_DESTROY_WINDOW(m_pCalR1ATxAttDlg,MAX_RF);
	SAFE_DESTROY_WINDOW(m_pCalR1ATxPowerDlg,MAX_RF);
	SAFE_DESTROY_WINDOW(m_pCalR1ARxSgpmmDlg,1);
	SAFE_DESTROY_WINDOW(m_pCalR1ARxRefDlg,MAX_RF);

	SAFE_DESTROY_WINDOW(m_pCalR1CDlg,MAX_RF);
	SAFE_DESTROY_WINDOW(m_pCalR1CTxLOLeakageDlg,MAX_RF);
	SAFE_DESTROY_WINDOW(m_pCalR1CTxSidebandDlg,MAX_RF);
	SAFE_DESTROY_WINDOW(m_pCalR1CTxFilterDlg,MAX_RF);
	SAFE_DESTROY_WINDOW(m_pCalR1CTxPowerDlg,MAX_RF);
	SAFE_DESTROY_WINDOW(m_pCalR1CTxAttDlg,MAX_RF);
	SAFE_DESTROY_WINDOW(m_pCalR1CTxFilOffDlg,MAX_RF);
	SAFE_DESTROY_WINDOW(m_pCalR1CRxFilterDlg,MAX_RF);
	SAFE_DESTROY_WINDOW(m_pCalR1CRxRefDlg,MAX_RF);
	SAFE_DESTROY_WINDOW(m_pCalR1CRxAttDlg,MAX_RF);
	SAFE_DESTROY_WINDOW(m_pCalR1CRxFilOffDlg,MAX_RF);

	SAFE_DESTROY_WINDOW(m_pTestR1ASBFSDlg,MAX_RF);
	SAFE_DESTROY_WINDOW(m_pTestR1ATxFRDlg,MAX_RF);
	SAFE_DESTROY_WINDOW(m_pTestR1ARxFRDlg,MAX_RF);
	SAFE_DESTROY_WINDOW(m_pTestR1AX9119Dlg,MAX_RF);
	SAFE_DESTROY_WINDOW(m_pTestR1ATSDlg,MAX_RF);

	SAFE_DESTROY_WINDOW(m_pTestR1CTxFRDlg,MAX_RF);
	SAFE_DESTROY_WINDOW(m_pTestR1CRxFRDlg,MAX_RF);
	SAFE_DESTROY_WINDOW(m_pTestR1CBTRDlg,MAX_RFU);
	SAFE_DESTROY_WINDOW(m_pTestR1CTTBDlg,MAX_RF);
}


BEGIN_MESSAGE_MAP(CMainTabCtrl, CTabCtrl)
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
END_MESSAGE_MAP()


void CMainTabCtrl::DrawItem(_In_ LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CBrush cbr_def;
	CBrush cbr_sel;
	cbr_sel.CreateSolidBrush(GREEN);

	//获取选项卡文字内容
	char szTabText[100];
	TC_ITEM tci;
	memset(szTabText,'\0',sizeof(szTabText));
	tci.mask = TCIF_TEXT;
	tci.pszText = szTabText;
	tci.cchTextMax = sizeof(szTabText) - 1;
	GetItem(lpDrawItemStruct->itemID,&tci);

	//填充选项卡背景
	CDC *dc = CDC::FromHandle(lpDrawItemStruct->hDC);
	cbr_def.CreateSolidBrush(dc->GetBkColor());
	if (lpDrawItemStruct->itemID == GetCurSel())
		dc->FillRect(&lpDrawItemStruct->rcItem,&cbr_sel);
	else {
		lpDrawItemStruct->rcItem.bottom += 2;
 		dc->FillRect(&lpDrawItemStruct->rcItem,&cbr_def);
	}

	//绘制选项卡文字
	if (lpDrawItemStruct->itemID == GetCurSel())
		dc->SetBkColor(GREEN);
 	else
 		dc->SetBkColor(dc->GetBkColor());
	RECT rc;
	rc = lpDrawItemStruct->rcItem;
	rc.top += 2;
	dc->DrawText(tci.pszText,lstrlen(tci.pszText),&rc,DT_CENTER);
}

BOOL CMainTabCtrl::OnEraseBkgnd(CDC* pDC)
{
//	CRect rect;
//	GetClientRect(&rect);

	//创建画刷
// 	CBrush brush;
// 	brush.CreateSolidBrush(TabCtrl_BkCol_Def);
// 
// 	//填充控件背景
// 	pDC->FillRect(&rect,&brush);

	CTabCtrl::OnEraseBkgnd(pDC);
	return true;
}

void CMainTabCtrl::OnSize(UINT nType, int cx, int cy)
{
	CTabCtrl::OnSize(nType, cx, cy);

	CRect crRFR1A,crRFR1AGuard,crNormalChild;
	crRFR1A.top    = 30;
	crRFR1A.bottom = RF_R1A_DLG_BOTTOM;
	crRFR1A.left   = 8;
	crRFR1A.right  = cx - 8;

	crRFR1AGuard.top	= crRFR1A.bottom;
	crRFR1AGuard.bottom = cy - 8;
	crRFR1AGuard.left	= 8;
	crRFR1AGuard.right	= cx - 8;

	crNormalChild.top    = 30;
	crNormalChild.bottom = cy - 8;
	crNormalChild.left   = 8;
	crNormalChild.right  = cx - 8;

#define DEFER_WIND_POS(wnd,num,rect)				\
	if (wnd) {										\
		for (int32_t i = 0;i < num;i ++) {			\
			if (::IsWindow(wnd[i].GetSafeHwnd()))	\
				wnd[i].MoveWindow(rect);			\
		}											\
	}

	DEFER_WIND_POS(m_pRFR1ADlg,MAX_RF,crRFR1A);
	DEFER_WIND_POS(m_pRFR1AGuardDlg,MAX_RF,crRFR1AGuard);
	DEFER_WIND_POS(m_pRFR1CDlg,MAX_RF,crNormalChild);
	DEFER_WIND_POS(m_pRFR1ADDlg,MAX_RF,crNormalChild);
	DEFER_WIND_POS(m_pRFR1CDDlg,MAX_RF,crNormalChild);
	DEFER_WIND_POS(m_pRFArbDlg,MAX_RF,crNormalChild);
	DEFER_WIND_POS(m_pRFIQCapDlg,MAX_RF,crNormalChild);

	DEFER_WIND_POS(m_pBBDlg,MAX_RF,crNormalChild);
	DEFER_WIND_POS(m_pBBArbDlg,MAX_RF,crNormalChild);
	DEFER_WIND_POS(m_pBBIQCapDlg,MAX_RF,crNormalChild);
	DEFER_WIND_POS(m_pDMADlg,2,crNormalChild);
	DEFER_WIND_POS(m_pFPGADlg,1,crNormalChild);

	DEFER_WIND_POS(m_pCalR1ATxLOLeakageDlg,MAX_RF,crNormalChild);
	DEFER_WIND_POS(m_pCalR1ATxSidebandDlg,MAX_RF,crNormalChild);
	DEFER_WIND_POS(m_pCalR1ATxAttDlg,MAX_RF,crNormalChild);
	DEFER_WIND_POS(m_pCalR1ATxPowerDlg,MAX_RF,crNormalChild);
	DEFER_WIND_POS(m_pCalR1ARxSgpmmDlg,1,crNormalChild);
	DEFER_WIND_POS(m_pCalR1ARxRefDlg,MAX_RF,crNormalChild);

	DEFER_WIND_POS(m_pCalR1CDlg,MAX_RF,crNormalChild);
	DEFER_WIND_POS(m_pCalR1CTxLOLeakageDlg,MAX_RF,crNormalChild);
	DEFER_WIND_POS(m_pCalR1CTxSidebandDlg,MAX_RF,crNormalChild);
	DEFER_WIND_POS(m_pCalR1CTxFilterDlg,MAX_RF,crNormalChild);
	DEFER_WIND_POS(m_pCalR1CTxPowerDlg,MAX_RF,crNormalChild);
	DEFER_WIND_POS(m_pCalR1CTxAttDlg,MAX_RF,crNormalChild);
	DEFER_WIND_POS(m_pCalR1CTxFilOffDlg,MAX_RF,crNormalChild);
	DEFER_WIND_POS(m_pCalR1CRxFilterDlg,MAX_RF,crNormalChild);
	DEFER_WIND_POS(m_pCalR1CRxRefDlg,MAX_RF,crNormalChild);
	DEFER_WIND_POS(m_pCalR1CRxAttDlg,MAX_RF,crNormalChild);
	DEFER_WIND_POS(m_pCalR1CRxFilOffDlg,MAX_RF,crNormalChild);

	DEFER_WIND_POS(m_pTestR1ASBFSDlg,MAX_RF,crNormalChild);
	DEFER_WIND_POS(m_pTestR1ATxFRDlg,MAX_RF,crNormalChild);
	DEFER_WIND_POS(m_pTestR1ARxFRDlg,MAX_RF,crNormalChild);
	DEFER_WIND_POS(m_pTestR1AX9119Dlg,MAX_RF,crNormalChild);
	DEFER_WIND_POS(m_pTestR1ATSDlg,MAX_RF,crNormalChild);

	DEFER_WIND_POS(m_pTestR1CTxFRDlg,MAX_RF,crNormalChild);
	DEFER_WIND_POS(m_pTestR1CRxFRDlg,MAX_RF,crNormalChild);
	DEFER_WIND_POS(m_pTestR1CBTRDlg,MAX_RFU,crNormalChild);
	DEFER_WIND_POS(m_pTestR1CTTBDlg,MAX_RF,crNormalChild);
}

void CMainTabCtrl::CreateAllDlg()
{
	CRect crInitDlg;
	int iScreenWidth = GetSystemMetrics(SM_CXSCREEN);
	int iScreenHeight = GetSystemMetrics(SM_CYSCREEN);

	CInitDlg *pInitDlg = new CInitDlg;
	pInitDlg->Create(CInitDlg::IDD);
	pInitDlg->GetWindowRect(&crInitDlg);
	pInitDlg->MoveWindow((iScreenWidth - crInitDlg.Width()) / 2,(iScreenHeight - crInitDlg.Height()) / 2,crInitDlg.Width(),crInitDlg.Height(),TRUE);
	pInitDlg->ShowWindow(SW_NORMAL);

	CRect crTabCtrl,crRFR1A,crRFR1AGuard;
	this->GetClientRect(&crTabCtrl);
	crTabCtrl.top    += 30;
	crTabCtrl.bottom -= 8;
	crTabCtrl.left   += 8;
	crTabCtrl.right  -= 8;

	crRFR1A = crTabCtrl;
	crRFR1A.bottom = RF_R1A_DLG_BOTTOM;

	for (int32_t i = 0;i < MAX_RF;i ++) {
		m_pRFR1ADlg[i].Create(CRFR1ADlg::IDD,this);
		m_pRFR1ADlg[i].ShowWindow(SW_HIDE);
		m_pRFR1ADlg[i].MoveWindow(&crRFR1A);

		m_pRFR1CDlg[i].Create(CRFR1CDlg::IDD,this);
		m_pRFR1CDlg[i].ShowWindow(SW_HIDE);
		m_pRFR1CDlg[i].MoveWindow(&crTabCtrl);

		m_pRFR1ADDlg[i].Create(CRFR1ADDlg::IDD,this);
		m_pRFR1ADDlg[i].ShowWindow(SW_HIDE);
		m_pRFR1ADDlg[i].MoveWindow(&crTabCtrl);

		m_pRFR1CDDlg[i].Create(CRFR1CDDlg::IDD,this);
		m_pRFR1CDDlg[i].ShowWindow(SW_HIDE);
		m_pRFR1CDDlg[i].MoveWindow(&crTabCtrl);

		m_pRFArbDlg[i].Create(CRFArbDlg::IDD,this);
		m_pRFArbDlg[i].ShowWindow(SW_HIDE);
		m_pRFArbDlg[i].MoveWindow(&crTabCtrl);

		m_pRFIQCapDlg[i].Create(CRFIQCaptureDlg::IDD,this);
		m_pRFIQCapDlg[i].ShowWindow(SW_HIDE);
		m_pRFIQCapDlg[i].MoveWindow(&crTabCtrl);
	
		m_pBBDlg[i].Create(CBBDlg::IDD,this);
		m_pBBDlg[i].ShowWindow(SW_HIDE);
		m_pBBDlg[i].MoveWindow(&crTabCtrl);

		m_pBBArbDlg[i].Create(CBBArbDlg::IDD,this);
		m_pBBArbDlg[i].ShowWindow(SW_HIDE);
		m_pBBArbDlg[i].MoveWindow(&crTabCtrl);

		m_pBBIQCapDlg[i].Create(CBBIQCaptureDlg::IDD,this);
		m_pBBIQCapDlg[i].ShowWindow(SW_HIDE);
		m_pBBIQCapDlg[i].MoveWindow(&crTabCtrl);

		m_pCalR1ATxLOLeakageDlg[i].Create(CCalR1ATxLOLeakageDlg::IDD,this);
		m_pCalR1ATxLOLeakageDlg[i].ShowWindow(SW_HIDE);
		m_pCalR1ATxLOLeakageDlg[i].MoveWindow(&crTabCtrl);

		m_pCalR1ATxSidebandDlg[i].Create(CCalR1ATxSideBandDlg::IDD,this);
		m_pCalR1ATxSidebandDlg[i].ShowWindow(SW_HIDE);
		m_pCalR1ATxSidebandDlg[i].MoveWindow(&crTabCtrl);

		m_pCalR1ATxAttDlg[i].Create(CCalR1ATxAttDlg::IDD,this);
		m_pCalR1ATxAttDlg[i].ShowWindow(SW_HIDE);
		m_pCalR1ATxAttDlg[i].MoveWindow(&crTabCtrl);

		m_pCalR1ATxPowerDlg[i].Create(CCalR1ATxPowerDlg::IDD,this);
		m_pCalR1ATxPowerDlg[i].ShowWindow(SW_HIDE);
		m_pCalR1ATxPowerDlg[i].MoveWindow(&crTabCtrl);

		m_pCalR1ARxRefDlg[i].Create(CCalR1ARxRefDlg::IDD,this);
		m_pCalR1ARxRefDlg[i].ShowWindow(SW_HIDE);
		m_pCalR1ARxRefDlg[i].MoveWindow(&crTabCtrl);

		m_pCalR1CDlg[i].Create(CCalR1CDlg::IDD,this);
		m_pCalR1CDlg[i].ShowWindow(SW_HIDE);
		m_pCalR1CDlg[i].MoveWindow(&crTabCtrl);

		m_pCalR1CTxLOLeakageDlg[i].Create(CCalR1CTxLOLeakageDlg::IDD,this);
		m_pCalR1CTxLOLeakageDlg[i].ShowWindow(SW_HIDE);
		m_pCalR1CTxLOLeakageDlg[i].MoveWindow(&crTabCtrl);

		m_pCalR1CTxSidebandDlg[i].Create(CCalR1CTxSidebandDlg::IDD,this);
		m_pCalR1CTxSidebandDlg[i].ShowWindow(SW_HIDE);
		m_pCalR1CTxSidebandDlg[i].MoveWindow(&crTabCtrl);

		m_pCalR1CTxFilterDlg[i].Create(CCalR1CTxFilterDlg::IDD,this);
		m_pCalR1CTxFilterDlg[i].ShowWindow(SW_HIDE);
		m_pCalR1CTxFilterDlg[i].MoveWindow(&crTabCtrl);

		m_pCalR1CTxPowerDlg[i].Create(CCalR1CTxPowerDlg::IDD,this);
		m_pCalR1CTxPowerDlg[i].ShowWindow(SW_HIDE);
		m_pCalR1CTxPowerDlg[i].MoveWindow(&crTabCtrl);

		m_pCalR1CTxAttDlg[i].Create(CCalR1CTxAttDlg::IDD,this);
		m_pCalR1CTxAttDlg[i].ShowWindow(SW_HIDE);
		m_pCalR1CTxAttDlg[i].MoveWindow(&crTabCtrl);

		m_pCalR1CTxFilOffDlg[i].Create(CCalR1CTxFilOffDlg::IDD,this);
		m_pCalR1CTxFilOffDlg[i].ShowWindow(SW_HIDE);
		m_pCalR1CTxFilOffDlg[i].MoveWindow(&crTabCtrl);
		
		m_pCalR1CRxFilterDlg[i].Create(CCalR1CRxFilterDlg::IDD,this);
		m_pCalR1CRxFilterDlg[i].ShowWindow(SW_HIDE);
		m_pCalR1CRxFilterDlg[i].MoveWindow(&crTabCtrl);

		m_pCalR1CRxRefDlg[i].Create(CCalR1CRxRefDlg::IDD,this);
		m_pCalR1CRxRefDlg[i].ShowWindow(SW_HIDE);
		m_pCalR1CRxRefDlg[i].MoveWindow(&crTabCtrl);

		m_pCalR1CRxAttDlg[i].Create(CCalR1CRxAttDlg::IDD,this);
		m_pCalR1CRxAttDlg[i].ShowWindow(SW_HIDE);
		m_pCalR1CRxAttDlg[i].MoveWindow(&crTabCtrl);

		m_pCalR1CRxFilOffDlg[i].Create(CCalR1CRxFilOffDlg::IDD,this);
		m_pCalR1CRxFilOffDlg[i].ShowWindow(SW_HIDE);
		m_pCalR1CRxFilOffDlg[i].MoveWindow(&crTabCtrl);

		m_pTestR1ASBFSDlg[i].Create(CTestR1ASBFSDlg::IDD,this);
		m_pTestR1ASBFSDlg[i].ShowWindow(SW_HIDE);
		m_pTestR1ASBFSDlg[i].MoveWindow(&crTabCtrl);

		m_pTestR1ATxFRDlg[i].Create(CTestR1ATxFRDlg::IDD,this);
		m_pTestR1ATxFRDlg[i].ShowWindow(SW_HIDE);
		m_pTestR1ATxFRDlg[i].MoveWindow(&crTabCtrl);

		m_pTestR1ARxFRDlg[i].Create(CTestR1ARxFRDlg::IDD,this);
		m_pTestR1ARxFRDlg[i].ShowWindow(SW_HIDE);
		m_pTestR1ARxFRDlg[i].MoveWindow(&crTabCtrl);

		m_pTestR1AX9119Dlg[i].Create(CTestR1AX9119Dlg::IDD,this);
		m_pTestR1AX9119Dlg[i].ShowWindow(SW_HIDE);
		m_pTestR1AX9119Dlg[i].MoveWindow(&crTabCtrl);

		m_pTestR1ATSDlg[i].Create(CTestR1ATSDlg::IDD,this);
		m_pTestR1ATSDlg[i].ShowWindow(SW_HIDE);
		m_pTestR1ATSDlg[i].MoveWindow(&crTabCtrl);

		m_pTestR1CTxFRDlg[i].Create(CTestR1CTxFRDlg::IDD,this);
		m_pTestR1CTxFRDlg[i].ShowWindow(SW_HIDE);
		m_pTestR1CTxFRDlg[i].MoveWindow(&crTabCtrl);

		m_pTestR1CRxFRDlg[i].Create(CTestR1CRxFRDlg::IDD,this);
		m_pTestR1CRxFRDlg[i].ShowWindow(SW_HIDE);
		m_pTestR1CRxFRDlg[i].MoveWindow(&crTabCtrl);

		m_pTestR1CTTBDlg[i].Create(CTestR1CTTBDlg::IDD,this);
		m_pTestR1CTTBDlg[i].ShowWindow(SW_HIDE);
		m_pTestR1CTTBDlg[i].MoveWindow(&crTabCtrl);
	}

	for (int32_t i = 0;i < MAX_RFU;i ++) {
		m_pTestR1CBTRDlg[i].Create(CTestR1CBTRDlg::IDD,this);
		m_pTestR1CBTRDlg[i].ShowWindow(SW_HIDE);
		m_pTestR1CBTRDlg[i].MoveWindow(&crTabCtrl);
	}

	for (int32_t i = 0;i < 2;i ++) {
		m_pDMADlg[i].Create(CDMADlg::IDD,this);
		m_pDMADlg[i].ShowWindow(SW_HIDE);
		m_pDMADlg[i].MoveWindow(&crTabCtrl);
	}
	
	m_pFPGADlg[0].Create(CFPGADlg::IDD,this);
	m_pFPGADlg[0].ShowWindow(SW_HIDE);
	m_pFPGADlg[0].MoveWindow(&crTabCtrl);

	m_pCalR1ARxSgpmmDlg[0].Create(CCalRxSgpmmDlg::IDD,this);
	m_pCalR1ARxSgpmmDlg[0].ShowWindow(SW_HIDE);
	m_pCalR1ARxSgpmmDlg[0].MoveWindow(&crTabCtrl);

	m_pRFR1ADlg[0].GetWindowRect(&crRFR1A);
	ScreenToClient(&crRFR1A);
	crRFR1AGuard.top	= crRFR1A.bottom;
	crRFR1AGuard.bottom = crTabCtrl.bottom;
	crRFR1AGuard.left   = crRFR1A.left;
	crRFR1AGuard.right  = crRFR1A.right;

	m_pRFR1AGuardDlg[0].Create(CRFR1AGuardDlg::IDD,this);
	m_pRFR1AGuardDlg[0].ShowWindow(SW_NORMAL);
	m_pRFR1AGuardDlg[0].MoveWindow(&crRFR1AGuard);

	m_pRFR1ADlg[0].ShowWindow(SW_NORMAL);

	SAFE_DESTROY_DLG(pInitDlg);
}

void CMainTabCtrl::UpdateParamInChildDlg()
{
	m_pRFR1AGuardDlg[0].m_pSP3501 = m_pSP3501;
	for (int i = 0;i < MAX_RF;i ++) {
		m_pRFR1ADlg[i].m_pSP1401   = m_pSP3301->m_pSP1401R1A[i];
		m_pRFR1ADDlg[i].m_pSP1401  = m_pSP3301->m_pSP1401R1A[i];
		m_pRFR1CDlg[i].m_pSP1401   = m_pSP3301->m_pSP1401R1C[i];
		m_pRFR1CDlg[i].m_pSP3501   = m_pSP3501;
		m_pRFR1CDDlg[i].m_pSP1401  = m_pSP3301->m_pSP1401R1C[i];
		m_pRFArbDlg[i].m_pSP1401   = m_pSP3301->m_pSP1401[i];
		m_pRFArbDlg[i].m_pSP3301   = m_pSP3301;
		m_pRFIQCapDlg[i].m_pSP1401 = m_pSP3301->m_pSP1401[i];
		m_pRFIQCapDlg[i].m_pSP2401 = m_pSP3301->m_pSP2401[i];

		m_pBBDlg[i].m_pSP1401	   = m_pSP3301->m_pSP1401R1A[i];
		m_pBBDlg[i].m_pSP2401	   = m_pSP3301->m_pSP2401[i];
		m_pBBArbDlg[i].m_pSP1401   = m_pSP3301->m_pSP1401[i];
		m_pBBIQCapDlg[i].m_pSP1401 = m_pSP3301->m_pSP1401[i];
		m_pBBIQCapDlg[i].m_pSP2401 = m_pSP3301->m_pSP2401[i];

		m_pCalR1ATxSidebandDlg[i].m_pSP1401	 = m_pSP3301->m_pSP1401R1A[i];
		m_pCalR1ATxSidebandDlg[i].m_pSP2401	 = m_pSP3301->m_pSP2401[i];
		m_pCalR1ATxLOLeakageDlg[i].m_pSP1401 = m_pSP3301->m_pSP1401R1A[i];
		m_pCalR1ATxLOLeakageDlg[i].m_pSP2401 = m_pSP3301->m_pSP2401[i];
		m_pCalR1ATxLOLeakageDlg[i].m_pSP3501 = m_pSP3501;
		m_pCalR1ATxAttDlg[i].m_pSP1401		 = m_pSP3301->m_pSP1401R1A[i];
		m_pCalR1ATxAttDlg[i].m_pSP2401		 = m_pSP3301->m_pSP2401[i];
		m_pCalR1ATxPowerDlg[i].m_pSP1401     = m_pSP3301->m_pSP1401R1A[i];
		m_pCalR1ATxPowerDlg[i].m_pSP2401	 = m_pSP3301->m_pSP2401[i];
		m_pCalR1ARxRefDlg[i].m_pSP1401		 = m_pSP3301->m_pSP1401R1A[i];
		m_pCalR1ARxRefDlg[i].m_pSP2401		 = m_pSP3301->m_pSP2401[i];

		m_pTestR1ASBFSDlg[i].m_pSP1401  = m_pSP3301->m_pSP1401R1A[i];
		m_pTestR1ASBFSDlg[i].m_pSP3301  = m_pSP3301;
		m_pTestR1ASBFSDlg[i].m_S6	    = &(m_pSP3301->m_S6);
		m_pTestR1ATxFRDlg[i].m_pSP1401  = m_pSP3301->m_pSP1401R1A[i];
		m_pTestR1ATxFRDlg[i].m_pSP2401  = m_pSP3301->m_pSP2401[i];
		m_pTestR1ARxFRDlg[i].m_pSP1401  = m_pSP3301->m_pSP1401R1A[i];
		m_pTestR1ARxFRDlg[i].m_pSP2401  = m_pSP3301->m_pSP2401[i];
		m_pTestR1AX9119Dlg[i].m_pSP1401 = m_pSP3301->m_pSP1401R1A[i];
		m_pTestR1ATSDlg[i].m_pSP1401    = m_pSP3301->m_pSP1401R1A[i];
		m_pTestR1ATSDlg[i].m_pSP2401    = m_pSP3301->m_pSP2401[i];

		m_pCalR1CDlg[i].m_pSP1401				= m_pSP3301->m_pSP1401R1C[i];
		m_pCalR1CTxLOLeakageDlg[i].m_pSP1401	= m_pSP3301->m_pSP1401R1C[i];
		m_pCalR1CTxLOLeakageDlg[i].m_pSP2401	= m_pSP3301->m_pSP2401[i];
		m_pCalR1CTxLOLeakageDlg[i].m_pSP3501	= m_pSP3501;
		m_pCalR1CTxSidebandDlg[i].m_pSP1401		= m_pSP3301->m_pSP1401R1C[i];
		m_pCalR1CTxSidebandDlg[i].m_pSP2401		= m_pSP3301->m_pSP2401[i];
		m_pCalR1CTxFilterDlg[i].m_pSP1401		= m_pSP3301->m_pSP1401R1C[i];
		m_pCalR1CTxFilterDlg[i].m_pSP2401		= m_pSP3301->m_pSP2401[i];
		m_pCalR1CTxPowerDlg[i].m_pSP1401		= m_pSP3301->m_pSP1401R1C[i];
		m_pCalR1CTxPowerDlg[i].m_pSP2401		= m_pSP3301->m_pSP2401[i];
		m_pCalR1CTxAttDlg[i].m_pSP1401			= m_pSP3301->m_pSP1401R1C[i];
		m_pCalR1CTxAttDlg[i].m_pSP2401			= m_pSP3301->m_pSP2401[i];
		m_pCalR1CTxFilOffDlg[i].m_pSP1401		= m_pSP3301->m_pSP1401R1C[i];
		m_pCalR1CTxFilOffDlg[i].m_pSP2401		= m_pSP3301->m_pSP2401[i];
		m_pCalR1CRxFilterDlg[i].m_pSP1401		= m_pSP3301->m_pSP1401R1C[i];
		m_pCalR1CRxFilterDlg[i].m_pSP2401		= m_pSP3301->m_pSP2401[i];
		m_pCalR1CRxRefDlg[i].m_pSP1401			= m_pSP3301->m_pSP1401R1C[i];
		m_pCalR1CRxRefDlg[i].m_pSP2401			= m_pSP3301->m_pSP2401[i];
		m_pCalR1CRxAttDlg[i].m_pSP1401			= m_pSP3301->m_pSP1401R1C[i];
		m_pCalR1CRxAttDlg[i].m_pSP2401			= m_pSP3301->m_pSP2401[i];
		m_pCalR1CRxFilOffDlg[i].m_pSP1401		= m_pSP3301->m_pSP1401R1C[i];
		m_pCalR1CRxFilOffDlg[i].m_pSP2401		= m_pSP3301->m_pSP2401[i];

		m_pTestR1CTxFRDlg[i].m_pSP1401 = m_pSP3301->m_pSP1401R1C[i];
		m_pTestR1CTxFRDlg[i].m_pSP2401 = m_pSP3301->m_pSP2401[i];
		m_pTestR1CRxFRDlg[i].m_pSP1401 = m_pSP3301->m_pSP1401R1C[i];
		m_pTestR1CRxFRDlg[i].m_pSP2401 = m_pSP3301->m_pSP2401[i];
		m_pTestR1CTTBDlg[i].m_pSP1401  = m_pSP3301->m_pSP1401R1C[i];
		m_pTestR1CTTBDlg[i].m_pSP2401  = m_pSP3301->m_pSP2401[i];
	}

	m_pRFR1AGuardDlg[0].m_pSP1401.clear();
	for (int32_t i = 0;i < MAX_RF;i ++)
		m_pRFR1AGuardDlg[0].m_pSP1401.push_back(m_pSP3301->m_pSP1401R1A[i]);
	m_pDMADlg[0].m_pSP1401 = m_pSP3301->m_pSP1401[2];
	m_pDMADlg[1].m_pSP1401 = m_pSP3301->m_pSP1401[0];
	m_pFPGADlg[0].m_K7_0   = &m_pSP3301->m_K7_0;
	m_pFPGADlg[0].m_K7_1   = &m_pSP3301->m_K7_1;
	m_pFPGADlg[0].m_S6	   = &m_pSP3301->m_S6;
	m_pFPGADlg[0].m_pSP2401_0 = m_pSP3301->m_pSP2401[0];
	m_pFPGADlg[0].m_pSP2401_2 = m_pSP3301->m_pSP2401[2];

	::PostMessage(m_pRFR1AGuardDlg[0].GetSafeHwnd(),WM_CSE_SP3301_CHANGE,0,0);
	for (int i = 0;i < MAX_RF;i ++) {
		::PostMessage(m_pCalR1ATxSidebandDlg[i].GetSafeHwnd(),WM_CSE_SP3301_CHANGE,0,0);
		::PostMessage(m_pCalR1ATxLOLeakageDlg[i].GetSafeHwnd(),WM_CSE_SP3301_CHANGE,0,0);
		::PostMessage(m_pCalR1ATxAttDlg[i].GetSafeHwnd(),WM_CSE_SP3301_CHANGE,0,0);
		::PostMessage(m_pCalR1ATxPowerDlg[i].GetSafeHwnd(),WM_CSE_SP3301_CHANGE,0,0);
		::PostMessage(m_pCalR1ARxRefDlg[i].GetSafeHwnd(),WM_CSE_SP3301_CHANGE,0,0);
	}
}