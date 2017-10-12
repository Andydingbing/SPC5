#pragma once
#include "afxcmn.h"

class CRegLogListCtrl : public CListCtrl
{
public:
	CRegLogListCtrl(void);
	~CRegLogListCtrl(void);
protected:
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnNMCustomdraw(NMHDR*, LRESULT*);
};
