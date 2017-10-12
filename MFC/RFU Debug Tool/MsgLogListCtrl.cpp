#include "StdAfx.h"
#include "RFU_Debug_Tool.h"
#include "MsgLogListCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CMsgLogListCtrl::CMsgLogListCtrl(void)
{
}

CMsgLogListCtrl::~CMsgLogListCtrl(void)
{
}

BEGIN_MESSAGE_MAP(CMsgLogListCtrl,CListCtrl)
 	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW,OnNMCustomdraw)
END_MESSAGE_MAP()

void CMsgLogListCtrl::OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	NMTVCUSTOMDRAW *pLVCD = reinterpret_cast<NMTVCUSTOMDRAW*>(pNMHDR);
	*pResult = 0;
	if (CDDS_PREPAINT == pNMCD->dwDrawStage) {
		*pResult = CDRF_NOTIFYITEMDRAW;
	}
	else {
		int iItem = static_cast<int>(pNMCD->dwItemSpec);
		if (GetItemState(iItem,LVIS_SELECTED)) {
			pLVCD->clrText = WHITE;
			pLVCD->clrTextBk = RGB(0,160,255);
			::SetTextColor(pLVCD->nmcd.hdc,WHITE);
			*pResult = CDRF_NEWFONT;
		}
		else {
			pLVCD->clrText = BLACK;
			pLVCD->clrTextBk = PROTECT_EYE;
			*pResult = CDRF_DODEFAULT;
		}
	}
}