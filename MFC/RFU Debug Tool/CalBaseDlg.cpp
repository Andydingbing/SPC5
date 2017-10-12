#include "stdafx.h"
#include "RFU_Debug_Tool.h"
#include "CalBaseDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//r1a/b
IMPLEMENT_DYNAMIC(CCalR1ABaseDlg, CAttachThreadDlg)

CCalR1ABaseDlg::CCalR1ABaseDlg(UINT nIDTemplate, CWnd* pParent /*=NULL*/)
	: CAttachThreadDlg(nIDTemplate, pParent)
{

}

CCalR1ABaseDlg::CCalR1ABaseDlg(UINT nIDTemplate, CString strFreq,CWnd* pParent /*=NULL*/)
	: CAttachThreadDlg(nIDTemplate, pParent)
{
	m_strFreq = strFreq;
}

CCalR1ABaseDlg::~CCalR1ABaseDlg()
{
}

void CCalR1ABaseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CCalR1ABaseDlg, CDialog)
	ON_MESSAGE(WM_CSE_CAL_SHOW_DATA, &CCalR1ABaseDlg::ShowData)
	ON_MESSAGE(WM_CSE_SINGLE_THREAD_DONE ,&CCalR1ABaseDlg::Done)
END_MESSAGE_MAP()


void CCalR1ABaseDlg::OnBnClickedGetAndSet()
{
	return;
}

void CCalR1ABaseDlg::OnBnClickedExportCalFile()
{
	return;
}

LRESULT CCalR1ABaseDlg::SP3301Change(WPARAM wParam,LPARAM lParam)
{
	return 0;
}

void CCalR1ABaseDlg::ResetShowCtrl()
{
	return;
}

//r1c
IMPLEMENT_DYNAMIC(CCalR1CBaseDlg, CAttachThreadDlg)

CCalR1CBaseDlg::CCalR1CBaseDlg(UINT nIDTemplate, CWnd* pParent /*=NULL*/)
: CAttachThreadDlg(nIDTemplate, pParent)
{

}

CCalR1CBaseDlg::CCalR1CBaseDlg(UINT nIDTemplate, CString strFreq,CWnd* pParent /*=NULL*/)
: CAttachThreadDlg(nIDTemplate, pParent)
{
	m_strFreq = strFreq;
}

CCalR1CBaseDlg::~CCalR1CBaseDlg()
{
}

void CCalR1CBaseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CCalR1CBaseDlg, CDialog)
	ON_MESSAGE(WM_CSE_CAL_SHOW_DATA, &CCalR1CBaseDlg::ShowData)
	ON_MESSAGE(WM_CSE_SINGLE_THREAD_DONE ,&CCalR1CBaseDlg::Done)
END_MESSAGE_MAP()


void CCalR1CBaseDlg::OnBnClickedGetAndSet()
{
	return;
}

void CCalR1CBaseDlg::OnBnClickedExportCalFile()
{
	return;
}

LRESULT CCalR1CBaseDlg::SP3301Change(WPARAM wParam,LPARAM lParam)
{
	return 0;
}

void CCalR1CBaseDlg::ResetShowCtrl()
{
	return;
}

BOOL CalOP(CalIOMode Mode)
{
	return (Mode == CAL_OP || Mode == CAL_IO_OP);
}

BOOL CalIO(CalIOMode Mode)
{
	return (Mode == CAL_IO || Mode == CAL_IO_OP);
}