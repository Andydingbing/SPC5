#include "stdafx.h"
#include "MainTreeCtrl.h"
#include "RFU_Debug_Tool.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CMainTreeCtrl, CTreeCtrl)

CMainTreeCtrl::CMainTreeCtrl()
{
}

CMainTreeCtrl::~CMainTreeCtrl()
{
}


BEGIN_MESSAGE_MAP(CMainTreeCtrl, CTreeCtrl)
	ON_WM_DRAWITEM()
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, &CMainTreeCtrl::OnNMCustomdraw)
	ON_WM_CREATE()
END_MESSAGE_MAP()


void CMainTreeCtrl::OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	NMTVCUSTOMDRAW *plvoid = reinterpret_cast<NMTVCUSTOMDRAW*>(pNMHDR);
	
	if (CDDS_PREPAINT == pNMCD->dwDrawStage) {
		*pResult = CDRF_NOTIFYITEMDRAW;
	}
	else if (CDDS_ITEMPREPAINT == pNMCD->dwDrawStage) {
		if ((CDIS_SELECTED | CDIS_FOCUS) == pNMCD->uItemState || CDIS_SELECTED == pNMCD->uItemState || CDIS_FOCUS == pNMCD->uItemState) {
			plvoid->clrTextBk = GREEN;
			plvoid->clrText = BLACK;
			*pResult = CDRF_DODEFAULT;
		}
// 		if (plvoid->iLevel > 0) {
// 			CString strItem = g_DlgInfo[(pNMCD->lItemlParam)].m_strName;
// 			if (-1 != strItem.Find(_T("R1C")))
// 				plvoid->clrText = BLUE;
// 			*pResult = CDRF_DODEFAULT;
// 		}
	}
}

CString EnsureWidth(int32_t iWidth,const char *format,...)
{
	char szBuf[64] = {0};
	va_list ap;
	va_start(ap,format);
	vsprintf(szBuf,format,ap);
	CString strRes;
	strRes.Format("%-*s 0",iWidth - 1,szBuf);
	return _T(strRes);
}
