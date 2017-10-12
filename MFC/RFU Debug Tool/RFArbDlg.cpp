#include "stdafx.h"
#include "RFU_Debug_Tool.h"
#include "RFARBDlg.h"
#include "ARBIQChartDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CRFArbDlg, CStretchDlg)
IMPLEMENT_CSE_STRETCH_DLG(CRFArbDlg)

CRFArbDlg::CRFArbDlg(CWnd* pParent /*=NULL*/)
	: CStretchDlg(CRFArbDlg::IDD, pParent)
	, m_iFreqOffset(0)
	, m_bReTrigger(FALSE)
	, m_iCycles(1)
	, m_iTriggerDelay(0)
	, m_bAutoStart(TRUE)
	, m_iAdditionalSamples(0)
	, m_uiCurSegment(0)
	, m_uiCurSample(0)
	, m_uiCurCycle(0)
	, m_dCurTimer(0)
	, m_uiMultiSegNext(0)
	, m_strPath(_T(""))
	, m_strInfo(_T(""))
{

}

CRFArbDlg::~CRFArbDlg()
{
}

void CRFArbDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT5, m_strPath);
	DDX_Control(pDX, IDC_COMBO1, m_TriggerSrc);
	DDX_Text(pDX, IDC_EDIT1, m_iFreqOffset);
	DDX_Check(pDX, IDC_CHECK1, m_bReTrigger);
	DDX_Text(pDX, IDC_EDIT2, m_iCycles);
	DDX_Text(pDX, IDC_EDIT3, m_iTriggerDelay);
	DDX_Check(pDX, IDC_CHECK2, m_bAutoStart);
	DDX_Text(pDX, IDC_EDIT4, m_iAdditionalSamples);
	DDX_Control(pDX, IDC_COMBO2, m_RepMode);
	DDX_Control(pDX, IDC_COMBO3, m_MultiSegTrigSrc);
	DDX_Control(pDX, IDC_COMBO4, m_MultiSegRepMode);
	DDX_Text(pDX, IDC_EDIT6, m_strInfo);
	DDX_Text(pDX, IDC_EDIT7, m_uiCurSegment);
	DDX_Text(pDX, IDC_EDIT8, m_uiCurSample);
	DDX_Text(pDX, IDC_EDIT9, m_uiCurCycle);
	DDX_Text(pDX, IDC_EDIT10, m_dCurTimer);
	DDX_Text(pDX, IDC_EDIT25, m_uiMultiSegNext);
}


BEGIN_MESSAGE_MAP(CRFArbDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON3, &CRFArbDlg::OnBnClickedSelectArbFilePath)
	ON_BN_CLICKED(IDC_BUTTON4, &CRFArbDlg::OnBnClickedSetArb)
	ON_BN_CLICKED(IDC_BUTTON1, &CRFArbDlg::OnBnClickedArbStart)
	ON_BN_CLICKED(IDC_BUTTON2, &CRFArbDlg::OnBnClickedArbStop)
	ON_BN_CLICKED(IDC_BUTTON5, &CRFArbDlg::OnBnClickedArbLoad)
	ON_BN_CLICKED(IDC_BUTTON6, &CRFArbDlg::OnBnClickedGetArbStatus)
	ON_BN_CLICKED(IDC_BUTTON8, &CRFArbDlg::OnBnClickedSetNextSeg)
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_ARB_IQWV, OnPopupIQChart)
	ON_BN_CLICKED(IDC_BUTTON28, &CRFArbDlg::OnBnClickedMunualTrigger)
	ON_WM_SIZE()
END_MESSAGE_MAP()


void CRFArbDlg::OnBnClickedSelectArbFilePath()
{
	CFileDialog ArbFile(TRUE);
	if (ArbFile.DoModal() == IDOK) {
		m_strPath = ArbFile.GetPathName();

		char szFileInfo[2048] = {0};
		memset(szFileInfo,0,sizeof(szFileInfo));

		m_pSP1401->GetArbReader()->Load((LPSTR)(LPCTSTR)m_strPath);
		m_pSP1401->GetArbReader()->GetFileInfo(szFileInfo);

		m_strInfo.Format("%s",szFileInfo);
		UpdateData(FALSE);
		((CButton *)GetDlgItem(IDC_BUTTON5))->EnableWindow(TRUE);
	}
}

BOOL CRFArbDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	((CButton *)GetDlgItem(IDC_BUTTON5))->EnableWindow(FALSE);
	m_TriggerSrc.SetCurSel(0);
	m_RepMode.SetCurSel(1);
	m_MultiSegTrigSrc.SetCurSel(0);
	m_MultiSegRepMode.SetCurSel(0);

	this->GetWindowRect(&m_crWnd);
	SetControlInfo(IDC_EDIT5,FixL_xStretch,1,0,1);
	SetControlInfo(IDC_BUTTON3,ChgL_Fixx,1,1,1);
	SetControlInfo(IDC_BUTTON5,ChgL_Fixx,1,1,1);
	SetControlInfo(IDC_EDIT6,FixL_xyStretch,1,0,1);
	return TRUE;
}

void CRFArbDlg::OnBnClickedSetArb()
{
	UpdateData(TRUE);

	bool bReTrigger = bool(m_bReTrigger);
	bool bAutoStart = bool(m_bAutoStart);
	uint32_t uiSrc = uint32_t(m_TriggerSrc.GetCurSel());
	uint32_t uiTrigDelay = uint32_t(m_iTriggerDelay);
	m_pSP1401->SetArbTrigger(bReTrigger,bAutoStart,uiSrc,uiTrigDelay);

	uint32_t uiAddSamp = uint32_t(m_iAdditionalSamples);
	uint32_t uiCycl = uint32_t(m_iCycles);
	uint32_t uiRepMo = uint32_t(m_RepMode.GetCurSel());
	m_pSP1401->SetArbParam(uiAddSamp,uiCycl,uiRepMo);

	uint32_t uiFreqMHz = uint32_t(m_iFreqOffset);
	m_pSP1401->SetArbFreqOffset(uiFreqMHz);

	uint32_t uiMSTS = uint32_t(m_MultiSegTrigSrc.GetCurSel());
	uint32_t uiMSRM = uint32_t(m_MultiSegRepMode.GetCurSel());
	m_pSP1401->SetArbMultiSegMode(uiMSTS,uiMSRM);
}

void CRFArbDlg::OnBnClickedArbStart()
{
	m_pSP1401->ArbStart();
}

void CRFArbDlg::OnBnClickedArbStop()
{
	m_pSP1401->ArbStop();
}

void CRFArbDlg::OnBnClickedArbLoad()
{
	m_pSP3301->ArbLoad(m_pSP1401->GetRfIdx(),(LPSTR)(LPCTSTR)m_strPath);
}

void CRFArbDlg::OnBnClickedGetArbStatus()
{
	m_uiCurSegment = m_pSP1401->GetArbCurrentSeg();
	m_uiCurSample  = m_pSP1401->GetArbCurrentSample();
	m_uiCurCycle   = m_pSP1401->GetArbCurrentCycle();
	m_dCurTimer    = m_pSP1401->GetArbCurrentTimer();
	UpdateData(FALSE);
}

void CRFArbDlg::OnBnClickedSetNextSeg()
{
	UpdateData(TRUE);
	m_pSP1401->SetArbMultiSegNext(m_uiMultiSegNext);
}

void CRFArbDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
	if ((CButton *)GetDlgItem(IDC_BUTTON5)->IsWindowEnabled()) {
		ClientToScreen(&point);
		CMenu menuArb;
		menuArb.LoadMenu(IDR_MENU_ARB);
		CMenu *pMenu;
		pMenu = menuArb.GetSubMenu(0);
		pMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON,point.x,point.y,this);
		pMenu->DestroyMenu();

		CDialog::OnRButtonDown(nFlags, point);
	}
	else
		return;
}

void CRFArbDlg::OnPopupIQChart()
{
	HWND hWnd = ::FindWindow(NULL,"IQ Wave Chart");
	::SendMessage(hWnd,WM_PASTE,0,0);	
}

void CRFArbDlg::OnBnClickedMunualTrigger()
{
	m_pSP1401->SetArbManualTrigger();
}
