#pragma once

class CDCOffsetScrollEdit : public CEdit
{
	DECLARE_DYNAMIC(CDCOffsetScrollEdit)

public:
	CDCOffsetScrollEdit();
	virtual ~CDCOffsetScrollEdit();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
};


