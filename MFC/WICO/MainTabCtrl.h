#pragma once

class CMainTabCtrl : public CTabCtrl
{
	DECLARE_DYNAMIC(CMainTabCtrl)

public:
	CMainTabCtrl();
	virtual ~CMainTabCtrl();

protected:
	DECLARE_MESSAGE_MAP()
public:
	void DrawItem(_In_ LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	void CreateAllDlg();
	afx_msg void OnTcnSelchange(NMHDR *pNMHDR, LRESULT *pResult);
};