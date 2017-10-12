#include "stdafx.h"
#include "RFU_Debug_Tool.h"
#include "BBArbDlg.h"
#include "ArbIQChartDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CBBArbDlg, CStretchDlg)
IMPLEMENT_CSE_STRETCH_DLG(CBBArbDlg)

CBBArbDlg::CBBArbDlg(CWnd* pParent /*=NULL*/)
	: CStretchDlg(CBBArbDlg::IDD, pParent)
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
{

}

CBBArbDlg::~CBBArbDlg()
{
}

void CBBArbDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT5, m_FilePath);
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
	DDX_Control(pDX, IDC_EDIT6, m_FileInfo);
	DDX_Text(pDX, IDC_EDIT7, m_uiCurSegment);
	DDX_Text(pDX, IDC_EDIT8, m_uiCurSample);
	DDX_Text(pDX, IDC_EDIT9, m_uiCurCycle);
	DDX_Text(pDX, IDC_EDIT10, m_dCurTimer);
	DDX_Text(pDX, IDC_EDIT25, m_uiMultiSegNext);
}


BEGIN_MESSAGE_MAP(CBBArbDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON3, &CBBArbDlg::OnBnClickedSelectArbFilePath)
	ON_BN_CLICKED(IDC_BUTTON4, &CBBArbDlg::OnBnClickedSetArb)
	ON_BN_CLICKED(IDC_BUTTON1, &CBBArbDlg::OnBnClickedArbStart)
	ON_BN_CLICKED(IDC_BUTTON2, &CBBArbDlg::OnBnClickedArbStop)
	ON_BN_CLICKED(IDC_BUTTON5, &CBBArbDlg::OnBnClickedArbLoad)
	ON_BN_CLICKED(IDC_BUTTON6, &CBBArbDlg::OnBnClickedGetArbStatus)
	ON_BN_CLICKED(IDC_BUTTON8, &CBBArbDlg::OnBnClickedSetNextSeg)
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_ARB_IQWV, OnPopupIQChart)
	ON_BN_CLICKED(IDC_BUTTON28, &CBBArbDlg::OnBnClickedMunualTrigger)
	ON_WM_SIZE()
END_MESSAGE_MAP()


void CBBArbDlg::OnBnClickedSelectArbFilePath()
{
	CFileDialog ArbFile(TRUE);
	if (ArbFile.DoModal() == IDOK)
		m_FilePath.SetWindowText(ArbFile.GetPathName());
	else
		return;

	char szFileInfo[2048] = {0};
	memset(szFileInfo,0,sizeof(szFileInfo));
	ArbReader File;
	File.Load((LPSTR)(LPCTSTR)ArbFile.GetPathName());
	File.GetFileInfo(szFileInfo);

	m_FileInfo.SetWindowText(szFileInfo);
	((CButton *)GetDlgItem(IDC_BUTTON5))->EnableWindow(TRUE);
	UpdateData(FALSE);
}

BOOL CBBArbDlg::OnInitDialog()
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

void CBBArbDlg::OnBnClickedSetArb()
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

void CBBArbDlg::OnBnClickedArbStart()
{
	m_pSP1401->ArbStart();
}

void CBBArbDlg::OnBnClickedArbStop()
{
	m_pSP1401->ArbStop();
}

void CBBArbDlg::OnBnClickedArbLoad()
{
	UpdateData(TRUE);
	CString strPath;
	m_FilePath.GetWindowText(strPath);
	m_pSP1401->ArbLoad((LPSTR)(LPCTSTR)strPath);
}

void CBBArbDlg::OnBnClickedGetArbStatus()
{
	m_uiCurSegment = m_pSP1401->GetArbCurrentSeg();
	m_uiCurSample  = m_pSP1401->GetArbCurrentSample();
	m_uiCurCycle   = m_pSP1401->GetArbCurrentCycle();
	m_dCurTimer    = m_pSP1401->GetArbCurrentTimer();
	UpdateData(FALSE);
}

void CBBArbDlg::OnBnClickedSetNextSeg()
{
	UpdateData(TRUE);
	m_pSP1401->SetArbMultiSegNext(m_uiMultiSegNext);
}

void CBBArbDlg::OnRButtonDown(UINT nFlags, CPoint point)
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

void CBBArbDlg::OnPopupIQChart()
{
	HWND hWnd = ::FindWindow(NULL,"IQ Wave Chart");
	::SendMessage(hWnd,WM_PASTE,0,0);
}

void CBBArbDlg::OnBnClickedMunualTrigger()
{
	m_pSP1401->SetArbManualTrigger();
}