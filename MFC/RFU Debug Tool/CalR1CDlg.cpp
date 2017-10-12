#include "stdafx.h"
#include "RFU_Debug_Tool.h"
#include "CalR1CDlg.h"
#include "CalR1CThread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CCalR1CDlg, CAttachThreadDlg)
IMPLEMENT_CSE_ATTACHTHREAD_DLG(CCalR1CDlg)

CCalR1CDlg::CCalR1CDlg(CWnd* pParent /*=NULL*/)
	: CAttachThreadDlg(CCalR1CDlg::IDD, pParent)
{
}

CCalR1CDlg::~CCalR1CDlg()
{
}

void CCalR1CDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CUSTOM1, m_Led[0]);
	DDX_Control(pDX, IDC_CUSTOM2, m_Led[1]);
	DDX_Control(pDX, IDC_CUSTOM3, m_Led[2]);
	DDX_Control(pDX, IDC_CUSTOM4, m_Led[3]);
	DDX_Control(pDX, IDC_CUSTOM5, m_Led[4]);
	DDX_Control(pDX, IDC_CUSTOM6, m_Led[5]);
	DDX_Control(pDX, IDC_CUSTOM7, m_Led[6]);
	DDX_Control(pDX, IDC_CUSTOM8, m_Led[7]);
	DDX_Control(pDX, IDC_CUSTOM9, m_Led[8]);
	DDX_Control(pDX, IDC_CUSTOM10, m_Led[9]);
}


BEGIN_MESSAGE_MAP(CCalR1CDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON2, &CCalR1CDlg::OnBnClickedStartCal)
	ON_BN_CLICKED(IDC_BUTTON3, &CCalR1CDlg::OnBnClickedStop)
	ON_MESSAGE(WM_CSE_SINGLE_THREAD_DONE, &CCalR1CDlg::Done)
	ON_MESSAGE(WM_CSE_SINGLE_THREAD_CANCEL, &CCalR1CDlg::CalChildThreadCancel)
	ON_WM_SIZE()
END_MESSAGE_MAP()


void CCalR1CDlg::ResetShowCtrl()
{
	for (int32_t i = 0;i < ARRAY_SIZE(m_Led);i ++)
		m_Led[i].SetClr(RED);
}

void CCalR1CDlg::OnBnClickedStartCal()
{
	CAL_THREAD_EXISTED_CHKV();
	STACHKV(ThreadCheckBox("Prepare Calibrating Single Board"));
	UpdateData(TRUE);
	CtrlPrepare();
	ResetShowCtrl();

	m_pThread = new CCalR1CThread(this);
	m_pThread->CreateThread();
}

BOOL CCalR1CDlg::OnInitDialog()
{
	CAttachThreadDlg::OnInitDialog();

	for (int32_t i = 0;i < ARRAY_SIZE(m_Led);i ++)
		m_Led[i].SetClr(RED);

	this->GetWindowRect(&m_crWnd);
	AddThreadIdleCtrl(IDC_BUTTON2);
	AddThreadRunningCtrl(IDC_BUTTON3);
	CtrlResume();
	return TRUE;
}

LRESULT CCalR1CDlg::CalChildThreadCancel(WPARAM wParam,LPARAM lParam)
{
	DWORD nThreadID = *(DWORD *)lParam;
	if (nThreadID == m_pThread->m_nThreadID)
		return 0;

	vector<CCalBaseThread *>::iterator iter;
	CCalR1CThread *pThread = (CCalR1CThread *)m_pThread;
	int32_t i = 0;

	for (iter = pThread->m_Thread.begin();iter != pThread->m_Thread.end();iter ++) {
		if ((*iter)->m_nThreadID == nThreadID) {
			m_Led[i].SetClr(YELLOW);
			return 0;
		}
		i ++;
	}
	return 0;
}

LRESULT CCalR1CDlg::Done(WPARAM wParam,LPARAM lParam)
{
	if (NULL == lParam)
		return CAttachThreadDlg::Done(wParam,lParam);

	int32_t iThread = *(int32_t *)lParam;
	vector<CCalBaseThread *>::iterator iter;
	CCalR1CThread *pThread = (CCalR1CThread *)m_pThread;
	int32_t i = 0;

	for (iter = pThread->m_Thread.begin();iter != pThread->m_Thread.end();iter ++) {
		if (iThread == i) {
			if (YELLOW != m_Led[i].GetClr()) {
				m_Led[i].SetClr(GREEN);
				return 0;
			}
		}
		i ++;
	}
	return 0;
}