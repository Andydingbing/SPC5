#include "StdAfx.h"
#include "TextClrListCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CTextClrListCtrl::CTextClrListCtrl(void)
{
}

CTextClrListCtrl::~CTextClrListCtrl(void)
{
}

BEGIN_MESSAGE_MAP(CTextClrListCtrl,CListCtrl)
 	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW,OnNMCustomdraw)
END_MESSAGE_MAP()

void CTextClrListCtrl::OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = CDRF_DODEFAULT;
	NMLVCUSTOMDRAW *lplvdr = (NMLVCUSTOMDRAW *)pNMHDR;
	NMCUSTOMDRAW &nmcd = lplvdr->nmcd;
	switch(lplvdr->nmcd.dwDrawStage) {
		case CDDS_PREPAINT:
			{
				*pResult = CDRF_NOTIFYITEMDRAW;
				break;
			}
		case CDDS_ITEMPREPAINT:
			{
				COLORREF crItem;
				DWORD itemspec = nmcd.dwItemSpec;
				if (m_ItemColor.Lookup(itemspec,crItem))
 				{
					lplvdr->clrText = crItem;
					*pResult = CDRF_DODEFAULT;
				}
			}
			break;
	}
}

void CTextClrListCtrl::SetItemColor(DWORD iItem,COLORREF color)
{
	m_ItemColor.SetAt(iItem,color);
	this->RedrawItems(iItem,0);
}