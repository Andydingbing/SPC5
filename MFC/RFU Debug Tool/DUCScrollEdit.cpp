#include "stdafx.h"
#include "RFU_Debug_Tool.h"
#include "DUCScrollEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CDUCScrollEdit, CEdit)

CDUCScrollEdit::CDUCScrollEdit()
{

}

CDUCScrollEdit::~CDUCScrollEdit()
{
}


BEGIN_MESSAGE_MAP(CDUCScrollEdit, CEdit)
ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()


BOOL CDUCScrollEdit::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	UpdateData(TRUE);
	CString strText;
	this->GetWindowText(strText);
	int64_t iFreq = 0,iFreqStep = 1000000;
	str2freq(strText,iFreq);
	if (nFlags == MK_CONTROL)
		iFreqStep = 2000000;
	if (nFlags == MK_SHIFT)
		iFreqStep = 5000000;
	if (zDelta == WHEEL_DELTA)
		iFreq += iFreqStep;
	else
		iFreq -= iFreqStep;
	freq2str(iFreq,strText);
	this->SetWindowText(strText);
	UpdateData(FALSE);
	SetFocus();
	return CEdit::OnMouseWheel(nFlags, zDelta, pt);
}