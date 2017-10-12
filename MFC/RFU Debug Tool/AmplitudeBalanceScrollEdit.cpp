#include "stdafx.h"
#include "RFU_Debug_Tool.h"
#include "AmplitudeBalanceScrollEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CAmplitudeBalanceScrollEdit, CEdit)

CAmplitudeBalanceScrollEdit::CAmplitudeBalanceScrollEdit()
{

}

CAmplitudeBalanceScrollEdit::~CAmplitudeBalanceScrollEdit()
{
}


BEGIN_MESSAGE_MAP(CAmplitudeBalanceScrollEdit, CEdit)
ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()


BOOL CAmplitudeBalanceScrollEdit::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	UpdateData(TRUE);
	CString str_text;
	this->GetWindowText(str_text);
	int32_t i_text = _ttoi(str_text);
	if (zDelta == WHEEL_DELTA)
			i_text ++;
	else
			i_text --;
	str_text.Format("%d",i_text);
	this->SetWindowText(str_text);
	UpdateData(FALSE);
	SetFocus();
	return CEdit::OnMouseWheel(nFlags, zDelta, pt);
}