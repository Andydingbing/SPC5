#pragma once
#include "afxcmn.h"

class CTextClrListCtrl : public CListCtrl
{
public:
	CTextClrListCtrl(void);
	~CTextClrListCtrl(void);
protected:
	DECLARE_MESSAGE_MAP()
public:
	void SetItemColor(DWORD iItem,COLORREF color);
public:
	afx_msg void OnNMCustomdraw(NMHDR*, LRESULT*);
	CMap<DWORD,DWORD&,COLORREF,COLORREF&> m_ItemColor;
};
