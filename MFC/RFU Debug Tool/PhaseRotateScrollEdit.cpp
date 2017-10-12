#include "stdafx.h"
#include "RFU_Debug_Tool.h"
#include "PhaseRotateScrollEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CPhaseRotateScrollEdit, CEdit)

CPhaseRotateScrollEdit::CPhaseRotateScrollEdit()
{

}

CPhaseRotateScrollEdit::~CPhaseRotateScrollEdit()
{
}


BEGIN_MESSAGE_MAP(CPhaseRotateScrollEdit, CEdit)
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()


BOOL CPhaseRotateScrollEdit::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	UpdateData(TRUE);
	CString str_text;
	this->GetWindowText(str_text);
	double d_text = strtod(str_text,NULL);
	if (zDelta == WHEEL_DELTA) {
		if (d_text < 20.0)
			d_text += 0.1;
	}
	else {
		if (d_text > -20.0)
			d_text -= 0.1;
	}
	str_text.Format("%g",d_text);
	this->SetWindowText(str_text);
	UpdateData(FALSE);
	SetFocus();
	return CEdit::OnMouseWheel(nFlags, zDelta, pt);
}
