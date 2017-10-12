#include "stdafx.h"
#include "RFU_Debug_Tool.h"
#include "X9119Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CX9119Dlg, CDialog)

CX9119Dlg::CX9119Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CX9119Dlg::IDD, pParent)
	, m_strX9119Value(_T("0"))
	, m_strX9119Voltage(_T("0"))
{

}

CX9119Dlg::~CX9119Dlg()
{
}

void CX9119Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT4, m_strX9119Value);
	DDX_Control(pDX, IDC_EDIT4, m_X9119Value);
	DDX_Text(pDX, IDC_EDIT3, m_strX9119Voltage);
}


BEGIN_MESSAGE_MAP(CX9119Dlg, CDialog)
	ON_EN_CHANGE(IDC_EDIT4, &CX9119Dlg::OnEnChangeEditX9119Value)
END_MESSAGE_MAP()


void CX9119Dlg::OnEnChangeEditX9119Value()
{
	UpdateData(TRUE);
	int iValue = _ttoi(m_strX9119Value);
	STACHKV(m_pSP3501->Vol9119((int16_t)iValue));
	m_strX9119Voltage.Format("%fV",x9119val2vol(iValue));
	UpdateData(FALSE);
	(CX9119ScrollEdit *)(GetDlgItem(IDC_EDIT4))->SetFocus();
}

BOOL CX9119Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_pSP3501 = &SP3501;

	return TRUE;
}
