#pragma once

class CPhaseRotateScrollEdit : public CEdit
{
	DECLARE_DYNAMIC(CPhaseRotateScrollEdit)

public:
	CPhaseRotateScrollEdit();
	virtual ~CPhaseRotateScrollEdit();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
};


