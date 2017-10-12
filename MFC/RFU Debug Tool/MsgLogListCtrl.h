#pragma once
#include "afxcmn.h"

class CMsgLogListCtrl : public CListCtrl
{
public:
	CMsgLogListCtrl(void);
	~CMsgLogListCtrl(void);
protected:
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnNMCustomdraw(NMHDR*, LRESULT*);
};
