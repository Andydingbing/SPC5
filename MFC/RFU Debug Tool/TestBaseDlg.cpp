#include "stdafx.h"
#include "RFU_Debug_Tool.h"
#include "TestBaseDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//r1a/b
IMPLEMENT_DYNAMIC(CTestR1ABaseDlg, CAttachThreadDlg)

CTestR1ABaseDlg::CTestR1ABaseDlg(UINT nIDTemplate, CWnd* pParent /*=NULL*/)
	: CAttachThreadDlg(nIDTemplate, pParent)
{

}

CTestR1ABaseDlg::~CTestR1ABaseDlg()
{
}

void CTestR1ABaseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTestR1ABaseDlg, CDialog)
	ON_MESSAGE(WM_CSE_SINGLE_THREAD_DONE ,&CTestR1ABaseDlg::Done)
END_MESSAGE_MAP()


//r1c
IMPLEMENT_DYNAMIC(CTestR1CBaseDlg, CAttachThreadDlg)

CTestR1CBaseDlg::CTestR1CBaseDlg(UINT nIDTemplate, CWnd* pParent /*=NULL*/)
: CAttachThreadDlg(nIDTemplate, pParent)
{

}

CTestR1CBaseDlg::~CTestR1CBaseDlg()
{
}

void CTestR1CBaseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTestR1CBaseDlg, CDialog)
	ON_MESSAGE(WM_CSE_SINGLE_THREAD_DONE ,&CTestR1CBaseDlg::Done)
END_MESSAGE_MAP()