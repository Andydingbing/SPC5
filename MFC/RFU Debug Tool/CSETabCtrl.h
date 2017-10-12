#pragma once

class CCSETabCtrl : public CTabCtrl
{
	DECLARE_DYNAMIC(CCSETabCtrl)

public:
	CCSETabCtrl();
	virtual ~CCSETabCtrl();

protected:
	DECLARE_MESSAGE_MAP()
public:
	void DrawItem(_In_ LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};