#pragma once

class CX9119ScrollEdit : public CEdit
{
	DECLARE_DYNAMIC(CX9119ScrollEdit)

public:
	CX9119ScrollEdit();
	virtual ~CX9119ScrollEdit();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
};


