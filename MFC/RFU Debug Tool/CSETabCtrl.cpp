#include "stdafx.h"
#include "RFU_Debug_Tool.h"
#include "CSETabCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CCSETabCtrl, CTabCtrl)

CCSETabCtrl::CCSETabCtrl()
{
}

CCSETabCtrl::~CCSETabCtrl()
{
}


BEGIN_MESSAGE_MAP(CCSETabCtrl, CTabCtrl)
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
END_MESSAGE_MAP()


void CCSETabCtrl::DrawItem(_In_ LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CBrush cbr_def;
	CBrush cbr_sel;
	cbr_sel.CreateSolidBrush(GREEN);

	//��ȡѡ���������
	char szTabText[100];
	TC_ITEM tci;
	memset(szTabText,'\0',sizeof(szTabText));
	tci.mask = TCIF_TEXT;
	tci.pszText = szTabText;
	tci.cchTextMax = sizeof(szTabText) - 1;
	GetItem(lpDrawItemStruct->itemID,&tci);

	//���ѡ�����
	CDC *dc = CDC::FromHandle(lpDrawItemStruct->hDC);
	cbr_def.CreateSolidBrush(dc->GetBkColor());
	if (lpDrawItemStruct->itemID == GetCurSel())
		dc->FillRect(&lpDrawItemStruct->rcItem,&cbr_sel);
	else {
		lpDrawItemStruct->rcItem.bottom += 2;
 		dc->FillRect(&lpDrawItemStruct->rcItem,&cbr_def);
	}

	//����ѡ�����
	if (lpDrawItemStruct->itemID == GetCurSel())
		dc->SetBkColor(GREEN);
 	else
 		dc->SetBkColor(dc->GetBkColor());
	RECT rc;
	rc = lpDrawItemStruct->rcItem;
	rc.top += 4;
	dc->DrawText(tci.pszText,lstrlen(tci.pszText),&rc,DT_CENTER);
}

BOOL CCSETabCtrl::OnEraseBkgnd(CDC* pDC)
{
//	CRect rect;
//	GetClientRect(&rect);

	//������ˢ
// 	CBrush brush;
// 	brush.CreateSolidBrush(TabCtrl_BkCol_Def);
// 
// 	//���ؼ�����
// 	pDC->FillRect(&rect,&brush);

	CTabCtrl::OnEraseBkgnd(pDC);
	return true;
}