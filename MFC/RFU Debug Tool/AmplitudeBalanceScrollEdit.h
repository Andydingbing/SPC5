#pragma once

class CAmplitudeBalanceScrollEdit : public CEdit
{
	DECLARE_DYNAMIC(CAmplitudeBalanceScrollEdit)

public:
	CAmplitudeBalanceScrollEdit();
	virtual ~CAmplitudeBalanceScrollEdit();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
};


