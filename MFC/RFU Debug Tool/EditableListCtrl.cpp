#include "stdafx.h"
#include "EditableListCtrl.h"
#include "RFU_Debug_Tool.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CEditableListCtrl, CListCtrl)

CEditableListCtrl::CEditableListCtrl()
{

}

CEditableListCtrl::~CEditableListCtrl()
{
}


BEGIN_MESSAGE_MAP(CEditableListCtrl, CListCtrl)
	ON_NOTIFY_REFLECT(NM_DBLCLK, &CEditableListCtrl::OnNMDblclk)
	ON_EN_KILLFOCUS(IDC_EDIT1, &CEditableListCtrl::OnEditEnd)
END_MESSAGE_MAP()


void CEditableListCtrl::OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	if (pNMItemActivate->iItem != -1) {
		CRect crItem;
		char szText[64] = {0};
		m_iRow = pNMItemActivate->iItem;
		m_iColumn = pNMItemActivate->iSubItem;
		this->GetSubItemRect(m_iRow,m_iColumn,LVIR_LABEL,crItem);
		crItem.left += 3;
		crItem.top += 3;
		crItem.right += 3;
		crItem.bottom += 3;
		this->GetItemText(m_iRow,m_iColumn,szText,64);

		if (!::IsWindow(m_Edit.GetSafeHwnd()))
			m_Edit.Create(ES_AUTOHSCROLL|WS_CHILD|ES_LEFT|ES_WANTRETURN|WS_BORDER,crItem,this,IDC_EDIT1);
		
		m_Edit.SetWindowText(szText);
		m_Edit.MoveWindow(&crItem);
		m_Edit.ShowWindow(SW_SHOW);
		m_Edit.SetFocus();
	}
	*pResult = 0;
}

void CEditableListCtrl::OnEditEnd()
{
	CString strText = "";
	m_Edit.GetWindowText(strText);
	this->SetItemText(m_iRow,m_iColumn,strText);
	m_Edit.ShowWindow(SW_HIDE);
}