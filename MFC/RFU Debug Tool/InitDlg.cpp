#include "stdafx.h"
#include "RFU_Debug_Tool.h"
#include "InitDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CInitDlg, CDialog)

CInitDlg::CInitDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CInitDlg::IDD, pParent)
{
}

CInitDlg::~CInitDlg()
{
}

void CInitDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CInitDlg, CDialog)
END_MESSAGE_MAP()