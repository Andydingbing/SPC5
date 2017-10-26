#pragma once

class CAmpOffScrollEdit : public CEdit
{
	DECLARE_DYNAMIC(CAmpOffScrollEdit)

public:
	CAmpOffScrollEdit();
	virtual ~CAmpOffScrollEdit();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
};


