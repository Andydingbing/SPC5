#pragma once

class CMainTreeCtrl : public CTreeCtrl
{
	DECLARE_DYNAMIC(CMainTreeCtrl)

public:
	CMainTreeCtrl();
	virtual ~CMainTreeCtrl();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult);
};

CString EnsureWidth(int32_t iWidth,const char *format,...);

