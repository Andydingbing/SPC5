#include "stdafx.h"
#include "WICO.h"
#include "AmpOffScrollEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CAmpOffScrollEdit, CEdit)

CAmpOffScrollEdit::CAmpOffScrollEdit()
{

}

CAmpOffScrollEdit::~CAmpOffScrollEdit()
{
}


BEGIN_MESSAGE_MAP(CAmpOffScrollEdit, CEdit)
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()


BOOL CAmpOffScrollEdit::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	CString strText;
	this->GetWindowText(strText);
	double dAmp = strtod(strText,NULL);
	double dStep = 0.01;
	if (nFlags == MK_CONTROL)
		dStep = 0.5;
	if (nFlags == MK_SHIFT)
		dStep = 1.0;
	if (zDelta == WHEEL_DELTA)
		dAmp += dStep;
	else
		dAmp -= dStep;
	strText.Format("%.2f",dAmp);
	this->SetWindowText(strText);
	SetFocus();
	return CEdit::OnMouseWheel(nFlags, zDelta, pt);
}