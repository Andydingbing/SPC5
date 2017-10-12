#include "stdafx.h"
#include "RFU_Debug_Tool.h"
#include "CheckDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CCheckDlg, CDialog)

CCheckDlg::CCheckDlg(CString strMsg,CWnd* pParent /*=NULL*/)
	: CDialog(CCheckDlg::IDD, pParent)
{
	m_strMsg = strMsg;
}

CCheckDlg::~CCheckDlg()
{
}

void CCheckDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CCheckDlg, CDialog)
END_MESSAGE_MAP()


BOOL CCheckDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	SetWindowText(m_strMsg);
	((CStatic *)(GetDlgItem(IDC_STATIC_MSG)))->SetWindowText(m_strMsg);
	Invalidate(TRUE);
	UpdateWindow();
	return TRUE;
}

void CseMessageBox(CString strMsg)
{
	CCheckDlg CheckDlg(strMsg);
	if (CheckDlg.DoModal() == IDCANCEL)
		return;
}

void CseMessageBox(char *format,...)
{
	char szMsg[512];
	memset(szMsg,0,sizeof(szMsg));

	va_list ap;
	va_start(ap,format);
	vsprintf(szMsg,format,ap);
	va_end(ap);

	CCheckDlg CheckDlg(szMsg);
	if (CheckDlg.DoModal() == IDCANCEL)
		return;
}

INT ThreadCheckBox(char *format,...)
{
	char szMsg[512];
	memset(szMsg,0,sizeof(szMsg));

	va_list ap;
	va_start(ap,format);
	vsprintf(szMsg,format,ap);
	va_end(ap);

	CCheckDlg CheckDlg(szMsg);
	if (CheckDlg.DoModal() == IDCANCEL) {
		CCSEWinThread::m_bRunning = FALSE;
		return -1;
	}
	return 0;
}

INT ThreadCancelBox(char *format,...)
{
	char szMsg[512];
	memset(szMsg,0,sizeof(szMsg));

	va_list ap;
	va_start(ap,format);
	vsprintf(szMsg,format,ap);
	va_end(ap);

	CCheckDlg CheckDlg(szMsg);
	if (CheckDlg.DoModal() == IDOK) {
		CCSEWinThread::m_bRunning = FALSE;
		return -1;
	}
	return 0;
}