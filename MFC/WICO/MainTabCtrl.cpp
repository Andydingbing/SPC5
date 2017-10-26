#include "stdafx.h"
#include "WICO.h"
#include "MainTabCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CMainTabCtrl, CTabCtrl)

CMainTabCtrl::CMainTabCtrl()
{
	g_pTxDlg = new CTxDlg;
	g_pRxDlg = new CRxDlg;
}

CMainTabCtrl::~CMainTabCtrl()
{
	SAFE_DESTROY_DLG(g_pTxDlg);
	SAFE_DESTROY_DLG(g_pRxDlg);
}


BEGIN_MESSAGE_MAP(CMainTabCtrl, CTabCtrl)
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_NOTIFY_REFLECT(TCN_SELCHANGE, &CMainTabCtrl::OnTcnSelchange)
END_MESSAGE_MAP()


void CMainTabCtrl::DrawItem(_In_ LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CBrush cbr_def;
	CBrush cbr_sel;
	cbr_sel.CreateSolidBrush(RGB(0,255,0));

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
		dc->SetBkColor(RGB(0,255,0));
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

void CMainTabCtrl::OnTcnSelchange(NMHDR *pNMHDR, LRESULT *pResult)
{
	int iTabIdx = GetCurSel();
	if (0 == iTabIdx) {
		if (::IsWindow(g_pTxDlg->GetSafeHwnd()))
			g_pTxDlg->ShowWindow(SW_NORMAL);
		if (::IsWindow(g_pRxDlg->GetSafeHwnd()))
			g_pRxDlg->ShowWindow(SW_HIDE);
	}
	else if (1 == iTabIdx) {
		if (::IsWindow(g_pTxDlg->GetSafeHwnd()))
			g_pTxDlg->ShowWindow(SW_HIDE);
		if (::IsWindow(g_pRxDlg->GetSafeHwnd()))
			g_pRxDlg->ShowWindow(SW_NORMAL);
	}
}

void CMainTabCtrl::OnSize(UINT nType, int cx, int cy)
{
	CTabCtrl::OnSize(nType, cx, cy);

	CRect crChild;
	crChild.top    = 30;
	crChild.bottom = cy - 8;
	crChild.left   = 8;
	crChild.right  = cx - 8;

	if (g_pTxDlg && ::IsWindow(g_pTxDlg->GetSafeHwnd()))
		g_pTxDlg->MoveWindow(&crChild);
	if (g_pRxDlg && ::IsWindow(g_pRxDlg->GetSafeHwnd()))
		g_pRxDlg->MoveWindow(&crChild);
}

void CMainTabCtrl::CreateAllDlg()
{
	CRect crTabCtrl,crRFR1A,crRFR1AGuard;
	this->GetClientRect(&crTabCtrl);
	crTabCtrl.top    += 30;
	crTabCtrl.bottom -= 8;
	crTabCtrl.left   += 8;
	crTabCtrl.right  -= 8;

	g_pTxDlg->Create(CTxDlg::IDD,this);
	g_pTxDlg->MoveWindow(&crTabCtrl);
	g_pTxDlg->ShowWindow(SW_NORMAL);

	g_pRxDlg->Create(CRxDlg::IDD,this);
	g_pRxDlg->MoveWindow(&crTabCtrl);
	g_pRxDlg->ShowWindow(SW_HIDE);
}