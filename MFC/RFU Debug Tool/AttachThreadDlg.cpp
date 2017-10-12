#include "stdafx.h"
#include "RFU_Debug_Tool.h"
#include "AttachThreadDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CAttachThreadDlg, CStretchDlg)

CCSEWinThread *CAttachThreadDlg::m_pThread = NULL;

CAttachThreadDlg::CAttachThreadDlg(UINT nIDTemplate, CWnd* pParent /*=NULL*/)
	: CStretchDlg(nIDTemplate, pParent)
{
	m_iIdleID.clear();
	m_iRunningID.clear();
}

CAttachThreadDlg::~CAttachThreadDlg()
{
}

void CAttachThreadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

void CAttachThreadDlg::AddThreadRunningCtrl(int iID)
{
	m_iRunningID.push_back(iID);
}

void CAttachThreadDlg::AddThreadIdleCtrl(int iID)
{
	m_iIdleID.push_back(iID);
}

void CAttachThreadDlg::AddThreadToPauseCtrl(int iID)
{
	m_iResumedID.push_back(iID);
}

void CAttachThreadDlg::AddThreadToRunningCtrl(int iID)
{
	m_iPausingID.push_back(iID);
}

LRESULT CAttachThreadDlg::CtrlPrepare()
{
	for (size_t i = 0;i < m_iRunningID.size();i ++)
		(CWnd *)(GetDlgItem(m_iRunningID[i]))->EnableWindow(TRUE);
	for (size_t i = 0;i < m_iIdleID.size();i ++)
		(CWnd *)(GetDlgItem(m_iIdleID[i]))->EnableWindow(FALSE);
	return CtrlRunning();
}

LRESULT CAttachThreadDlg::CtrlResume()
{
	for (size_t i = 0;i < m_iRunningID.size();i ++)
		(CWnd *)(GetDlgItem(m_iRunningID[i]))->EnableWindow(FALSE);
	for (size_t i = 0;i < m_iIdleID.size();i ++)
		(CWnd *)(GetDlgItem(m_iIdleID[i]))->EnableWindow(TRUE);
	for (size_t i = 0;i < m_iPausingID.size();i ++)
		(CWnd *)(GetDlgItem(m_iPausingID[i]))->EnableWindow(FALSE);
	for (size_t i = 0;i < m_iResumedID.size();i ++)
		(CWnd *)(GetDlgItem(m_iResumedID[i]))->EnableWindow(FALSE);
	return 0;
}

LRESULT CAttachThreadDlg::CtrlPausing()
{
	for (size_t i = 0;i < m_iPausingID.size();i ++)
		(CWnd *)(GetDlgItem(m_iPausingID[i]))->EnableWindow(TRUE);
	for (size_t i = 0;i < m_iResumedID.size();i ++)
		(CWnd *)(GetDlgItem(m_iResumedID[i]))->EnableWindow(FALSE);
	return 0;
}

LRESULT CAttachThreadDlg::CtrlRunning()
{
	for (size_t i = 0;i < m_iPausingID.size();i ++)
		(CWnd *)(GetDlgItem(m_iPausingID[i]))->EnableWindow(FALSE);
	for (size_t i = 0;i < m_iResumedID.size();i ++)
		(CWnd *)(GetDlgItem(m_iResumedID[i]))->EnableWindow(TRUE);
	return 0;
}

LRESULT CAttachThreadDlg::CtrlDisable()
{
	for (size_t i = 0;i < m_iRunningID.size();i ++)
		(CWnd *)(GetDlgItem(m_iRunningID[i]))->EnableWindow(FALSE);
	for (size_t i = 0;i < m_iIdleID.size();i ++)
		(CWnd *)(GetDlgItem(m_iIdleID[i]))->EnableWindow(FALSE);
	return 0;
}


BEGIN_MESSAGE_MAP(CAttachThreadDlg, CDialog)
END_MESSAGE_MAP()


void CAttachThreadDlg::OnBnClickedPause()
{
	m_pThread->SuspendThread();
	CtrlPausing();
}

void CAttachThreadDlg::OnBnClickedContinue()
{
	m_pThread->ResumeThread();
	CtrlRunning();
}

void CAttachThreadDlg::OnBnClickedStop()
{
	if (ThreadCancelBox("Cancel?"))
		m_pThread->ResumeThread();
	SetActiveWindow();
}

LRESULT CAttachThreadDlg::Done(WPARAM wParam,LPARAM lParam)
{
	CCSEWinThread::m_bRunning = FALSE;
	SetActiveWindow();
	return CtrlResume();
}