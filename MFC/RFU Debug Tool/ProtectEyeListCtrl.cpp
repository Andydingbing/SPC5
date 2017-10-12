#include "stdafx.h"
#include "ProtectEyeListCtrl.h"
#include "RFU_Debug_Tool.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CProtectEyeListCtrl, CListCtrl)

CProtectEyeListCtrl::CProtectEyeListCtrl()
{

}

CProtectEyeListCtrl::~CProtectEyeListCtrl()
{
}


BEGIN_MESSAGE_MAP(CProtectEyeListCtrl, CListCtrl)
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, &CProtectEyeListCtrl::OnNMCustomdraw)
END_MESSAGE_MAP()


void CProtectEyeListCtrl::OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	NMTVCUSTOMDRAW *pLVCD = reinterpret_cast<NMTVCUSTOMDRAW*>(pNMHDR);
	*pResult = 0;
	if (CDDS_PREPAINT == pNMCD->dwDrawStage) {
		*pResult = CDRF_NOTIFYITEMDRAW;
	}
	else {
 		int iItem = static_cast<int>(pNMCD->dwItemSpec);
		if (GetItemState(iItem,LVIS_SELECTED) == LVIS_SELECTED) {
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
