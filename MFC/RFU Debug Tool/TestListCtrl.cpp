#include "stdafx.h"
#include "RFU_Debug_Tool.h"
#include "TestListCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CHeaderCtrlEx, CHeaderCtrl)

CHeaderCtrlEx::CHeaderCtrlEx()
{
}

CHeaderCtrlEx::~CHeaderCtrlEx()
{
}


BEGIN_MESSAGE_MAP(CHeaderCtrlEx, CHeaderCtrl)
END_MESSAGE_MAP()


BOOL CHeaderCtrlEx::OnChildNotify(UINT msg, WPARAM wp, LPARAM lp, LRESULT* pRes)
{
	NMHDR &nmh = *(NMHDR*)lp;
	if (nmh.code == HDN_BEGINTRACK || nmh.code == HDN_BEGINTRACKW)
		return (BOOL)(*pRes = TRUE);
	else if (nmh.code == HDN_DIVIDERDBLCLICK || nmh.code == HDN_DIVIDERDBLCLICKW)
		return (BOOL)(*pRes = TRUE);
	return CHeaderCtrl::OnChildNotify(msg,wp,lp,pRes);
}


IMPLEMENT_DYNAMIC(CTestListCtrl, CTextClrListCtrl)

CTestListCtrl::CTestListCtrl()
{
}

CTestListCtrl::~CTestListCtrl()
{
}


BEGIN_MESSAGE_MAP(CTestListCtrl, CListCtrl)
END_MESSAGE_MAP()


void CTestListCtrl::PreSubclassWindow()
{
	CListCtrl::PreSubclassWindow();
	m_ctrlHeader.SubclassWindow(::GetDlgItem(m_hWnd,0));
}


