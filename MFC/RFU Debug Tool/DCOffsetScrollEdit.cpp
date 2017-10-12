#include "stdafx.h"
#include "RFU_Debug_Tool.h"
#include "DCOffsetScrollEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CDCOffsetScrollEdit, CEdit)

CDCOffsetScrollEdit::CDCOffsetScrollEdit()
{

}

CDCOffsetScrollEdit::~CDCOffsetScrollEdit()
{
}


BEGIN_MESSAGE_MAP(CDCOffsetScrollEdit, CEdit)
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()


BOOL CDCOffsetScrollEdit::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	CString strText;
	this->GetWindowText(strText);
	int iText = _ttoi(strText);
	int iStep = 1;
	if (nFlags == MK_CONTROL)
		iStep = 5;
	if (nFlags == MK_SHIFT)
		iStep = 10;
	if (zDelta == WHEEL_DELTA)
		iText += iStep;
	else
		iText -= iStep;
	strText.Format("%d",iText);
	this->SetWindowText(strText);
	SetFocus();
	return CEdit::OnMouseWheel(nFlags, zDelta, pt);
}