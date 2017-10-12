#include "stdafx.h"
#include "RFU_Debug_Tool.h"
#include "LogConfigDlg.h"
#include "CheckDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CLogConfigDlg, CDialog)

CLogConfigDlg::CLogConfigDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLogConfigDlg::IDD, pParent)
	, m_bPrompt(FALSE)
	, m_bMsgList(FALSE)
	, m_bMsgListToFile(FALSE)
	, m_bRegList(FALSE)
	, m_bRegListToFile(FALSE)
	, m_bAll(FALSE)
	, m_bTrace(FALSE)
{

}

CLogConfigDlg::~CLogConfigDlg()
{
}

void CLogConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK1, m_bPrompt);
	DDX_Check(pDX, IDC_CHECK5, m_bMsgList);
	DDX_Check(pDX, IDC_CHECK6, m_bMsgListToFile);
	DDX_Check(pDX, IDC_CHECK7, m_bRegList);
	DDX_Check(pDX, IDC_CHECK8, m_bRegListToFile);
	DDX_Check(pDX, IDC_CHECK9, m_bAll);
	DDX_Check(pDX, IDC_CHECK2, m_bTrace);
}


BEGIN_MESSAGE_MAP(CLogConfigDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CLogConfigDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CHECK9, &CLogConfigDlg::OnBnClickedSetAll)
	ON_BN_CLICKED(IDC_CHECK1, &CLogConfigDlg::OnBnClickedUpdate)
	ON_BN_CLICKED(IDC_CHECK2, &CLogConfigDlg::OnBnClickedUpdate)
	ON_BN_CLICKED(IDC_CHECK5, &CLogConfigDlg::OnBnClickedUpdate)
	ON_BN_CLICKED(IDC_CHECK6, &CLogConfigDlg::OnBnClickedUpdate)
	ON_BN_CLICKED(IDC_CHECK7, &CLogConfigDlg::OnBnClickedUpdate)
	ON_BN_CLICKED(IDC_CHECK8, &CLogConfigDlg::OnBnClickedUpdate)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


void CLogConfigDlg::OnBnClickedOk()
{
	UpdateData(TRUE);
	Log->SetEnable(L_PROMPT,m_bPrompt);
	Log->SetEnable(L_TRACE,m_bTrace);
	Log->SetEnable(L_MSG,m_bMsgList);
	Log->SetEnable(L_MSG_F,m_bMsgListToFile);
	Log->SetEnable(L_REG,m_bRegList);
	Log->SetEnable(L_REG_F,m_bRegListToFile);
	OnOK();
}

void CLogConfigDlg::OnCancel()
{
	ShowWindow(FALSE);
	CDialog::OnCancel();
}

BOOL CLogConfigDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_bAll = Log->IsEnabled(L_ALL_ON);
	m_bPrompt = Log->IsEnabled(L_PROMPT);
	m_bTrace = Log->IsEnabled(L_TRACE);
	m_bMsgList = Log->IsEnabled(L_MSG);
	m_bMsgListToFile = Log->IsEnabled(L_MSG_F);
	m_bRegList = Log->IsEnabled(L_REG);
	m_bRegListToFile = Log->IsEnabled(L_REG_F);
	UpdateData(FALSE);
	return TRUE;
}

void CLogConfigDlg::OnBnClickedSetAll()
{
	UpdateData(TRUE);
	m_bPrompt		 = m_bAll;
	m_bTrace		 = m_bAll;
	m_bMsgList		 = m_bAll;
	m_bMsgListToFile = m_bAll;
	m_bRegList		 = m_bAll;
	m_bRegListToFile = m_bAll;
	UpdateData(FALSE);
}

void CLogConfigDlg::OnBnClickedUpdate()
{
	UpdateData(TRUE);
	if (m_bPrompt && m_bTrace && m_bMsgList && m_bMsgListToFile && m_bRegList && m_bRegListToFile)
		m_bAll = TRUE;
	else
		m_bAll = FALSE;
	UpdateData(FALSE);
}

void CLogConfigDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	if (bShow == TRUE || bShow == SW_NORMAL || bShow == SW_SHOW || bShow == SW_SHOWNORMAL) {
		m_bAll = Log->IsEnabled(L_ALL_ON);
		m_bPrompt = Log->IsEnabled(L_PROMPT);
		m_bTrace = Log->IsEnabled(L_TRACE);
		m_bMsgList = Log->IsEnabled(L_MSG);
		m_bMsgListToFile = Log->IsEnabled(L_MSG_F);
		m_bRegList = Log->IsEnabled(L_REG);
		m_bRegListToFile = Log->IsEnabled(L_REG_F);
	}
	UpdateData(FALSE);
}