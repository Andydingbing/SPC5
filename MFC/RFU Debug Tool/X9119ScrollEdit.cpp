#include "stdafx.h"
#include "RFU_Debug_Tool.h"
#include "X9119ScrollEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CX9119ScrollEdit, CEdit)

CX9119ScrollEdit::CX9119ScrollEdit()
{

}

CX9119ScrollEdit::~CX9119ScrollEdit()
{
}


BEGIN_MESSAGE_MAP(CX9119ScrollEdit, CEdit)
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()


BOOL CX9119ScrollEdit::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	UpdateData(TRUE);
	CString str_text;
	this->GetWindowText(str_text);
	int32_t i_text = _ttoi(str_text);
	int32_t i_step = 1;
	if (nFlags == MK_CONTROL)
		i_step = 5;
	if (nFlags == MK_SHIFT)
		i_step = 10;
	if (zDelta == WHEEL_DELTA)
		i_text += i_step;
	else
		i_text -= i_step;
	i_text = i_text >= 1023 ? 1023 : i_text;
	i_text = i_text <= 0 ? 0 : i_text;
	str_text.Format("%d",i_text);
	this->SetWindowText(str_text);
	UpdateData(FALSE);
	SetFocus();
	return CEdit::OnMouseWheel(nFlags, zDelta, pt);
}
