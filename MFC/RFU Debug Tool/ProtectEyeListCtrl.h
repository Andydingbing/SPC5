#pragma once

class CProtectEyeListCtrl : public CListCtrl
{
	DECLARE_DYNAMIC(CProtectEyeListCtrl)

public:
	CProtectEyeListCtrl();
	virtual ~CProtectEyeListCtrl();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult);
};


