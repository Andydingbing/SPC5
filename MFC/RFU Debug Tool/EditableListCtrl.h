#pragma once

class CEditableListCtrl : public CListCtrl
{
	DECLARE_DYNAMIC(CEditableListCtrl)

public:
	CEditableListCtrl();
	virtual ~CEditableListCtrl();

protected:
	DECLARE_MESSAGE_MAP()
public:
	int m_iRow;
	int m_iColumn;
	CEdit m_Edit;
	afx_msg void OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEditEnd();
};


