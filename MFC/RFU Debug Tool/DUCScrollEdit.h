#pragma once

class CDUCScrollEdit : public CEdit
{
	DECLARE_DYNAMIC(CDUCScrollEdit)

public:
	CDUCScrollEdit();
	virtual ~CDUCScrollEdit();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
};


