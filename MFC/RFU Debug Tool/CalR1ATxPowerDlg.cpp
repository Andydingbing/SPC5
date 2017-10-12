#include "stdafx.h"
#include "RFU_Debug_Tool.h"
#include "CalR1ATxPowerDlg.h"
#include "CalR1ATxPowerThread.h"
#include "CheckDlg.h"
#include "LogDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CCalR1ATxPowerDlg, CCalR1ABaseDlg)
IMPLEMENT_CSE_CAL_R1A_DLG(CCalR1ATxPowerDlg)

CCalR1ATxPowerDlg::CCalR1ATxPowerDlg(CWnd* pParent /*=NULL*/)
	: CCalR1ABaseDlg(CCalR1ATxPowerDlg::IDD, _T("2G"), pParent)
	, m_strFreqStar(_T(""))
	, m_strFreqStop(_T(""))
	, m_strFreqStep(_T(""))
	, m_dDGain(0.0)
	, m_iAtt1(0)
	, m_iAtt2(0)
	, m_strPAAtt(_T(""))
	, m_iAtt3(0)
	, m_dPower(0)
{
	m_pChart = NULL;
	for (int i = 0;i < ARRAY_SIZE(m_OPLineSerie);i ++)
		m_OPLineSerie[i].clear();
	for (int i = 0;i < ARRAY_SIZE(m_IOLineSerie);i ++)
		m_IOLineSerie[i].clear();
}

CCalR1ATxPowerDlg::~CCalR1ATxPowerDlg()
{
	for (int i = 0;i < ARRAY_SIZE(m_OPLineSerie);i ++) {
		for (int j = 0;j < m_OPLineSerie[i].size();j ++)
			(m_OPLineSerie[i].at(j))->ClearSerie();
		m_OPLineSerie[i].clear();
	}
	for (int i = 0;i < ARRAY_SIZE(m_IOLineSerie);i ++) {
		for (int j = 0;j < m_IOLineSerie[i].size();j ++)
			(m_IOLineSerie[i].at(j))->ClearSerie();
		m_IOLineSerie[i].clear();
	}
	if (m_pChart) {
		delete m_pChart;
		m_pChart = NULL;
	}
}

void CCalR1ATxPowerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1,  m_strFreqStar);
	DDX_Text(pDX, IDC_EDIT11, m_strFreqStop);
	DDX_Text(pDX, IDC_EDIT12, m_strFreqStep);
	DDX_Text(pDX, IDC_EDIT6, m_strFreq);
	DDX_Text(pDX, IDC_EDIT7, m_dDGain);
	DDX_Text(pDX, IDC_EDIT8, m_iAtt1);
	DDX_Text(pDX, IDC_EDIT9, m_iAtt2);
	DDX_Text(pDX, IDC_EDIT10, m_strPAAtt);
	DDX_Text(pDX, IDC_EDIT21, m_iAtt3);
	DDX_Text(pDX, IDC_EDIT15, m_dPower);
	DDX_Control(pDX, IDC_COMBO2, m_IOMode);
	DDX_Control(pDX, IDC_COMBO61, m_CalIOMode);
}

BOOL CCalR1ATxPowerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_pChart = new CChartCtrl;
	m_pChart->Create(this,CAL_CHART_RECT,IDC_CAL_R1A_TX_POWER_CHART);
	m_pChart->GetLegend()->SetHorizontalMode(false);
	m_pChart->GetLegend()->UndockLegend(50,20);
	m_pChart->GetLegend()->SetVisible(true);
	m_pChart->SetZoomRectColor(BLACK);
	m_pChart->GetTitle()->AddString(_T("Base Power's Digital Gain Curve(X:Freq(MHz) Y:Digital Gain(dB))"));
	m_pChart->GetTitle()->SetColor(BLUE);

	CChartAxis *pAxis;
	pAxis = m_pChart->CreateStandardAxis(CChartCtrl::BottomAxis);
	pAxis->SetAutomatic(false);
	pAxis->SetMinMax(RF_TX_FREQ_STAR / 1000000,RF_TX_FREQ_STOP / 1000000);
	pAxis = m_pChart->CreateStandardAxis(CChartCtrl::LeftAxis);
	pAxis->SetAutomatic(false);
	pAxis->SetMinMax((double)(R1A_DGAIN_MIN - 2),(double)(R1A_DGAIN_MAX + 3));
	
	m_CalIOMode.SetCurSel(2);
	m_IOMode.SetCurSel(1);
	m_strFreqStar.Format("%dM",RF_TX_FREQ_STAR / 1000000);
	m_strFreqStop.Format("%dM",RF_TX_FREQ_STOP / 1000000);
	m_strFreqStep.Format("%dM",RF_TX_FREQ_STEP_CALLED / 1000000);

	CString strPath;
	strPath.Format("c:\\CSECal\\rfu%drf%d.cal",m_pSP1401->GetRfuIdx(),m_pSP1401->GetRfIdx());
	((CEdit *)(GetDlgItem(IDC_EDIT4)))->SetWindowText(strPath);
	UpdateData(FALSE);

	this->GetWindowRect(&m_crWnd);
	SetControlInfo(IDC_EDIT1,FixL_xStretch,1,0,1);
	SetControlInfo(IDC_EDIT11,FixL_xStretch,1,0,1);
	SetControlInfo(IDC_EDIT4,ChgL_Fixx,1,1,1);
	SetControlInfo(IDC_BUTTON4,ChgL_Fixx,1,1,1);
	SetControlInfo(IDC_CAL_R1A_TX_POWER_CHART,FixL_xyStretch,1,0,1,1,0,1);
	AddThreadIdleCtrl(IDC_EDIT1);
	AddThreadIdleCtrl(IDC_EDIT11);
	AddThreadIdleCtrl(IDC_EDIT12);
	AddThreadIdleCtrl(IDC_BUTTON2);
	AddThreadIdleCtrl(IDC_EDIT6);
	AddThreadIdleCtrl(IDC_EDIT15);
	AddThreadIdleCtrl(IDC_COMBO2);
	AddThreadIdleCtrl(IDC_EDIT7);
	AddThreadIdleCtrl(IDC_EDIT8);
	AddThreadIdleCtrl(IDC_EDIT9);
	AddThreadIdleCtrl(IDC_EDIT10);
	AddThreadIdleCtrl(IDC_EDIT21);
	AddThreadIdleCtrl(IDC_BUTTON6);
	AddThreadIdleCtrl(IDC_BUTTON4);
	AddThreadRunningCtrl(IDC_BUTTON3);
	AddThreadToPauseCtrl(IDC_BUTTON21);
	AddThreadToRunningCtrl(IDC_BUTTON15);
	CtrlResume();
	return TRUE;
}


BEGIN_MESSAGE_MAP(CCalR1ATxPowerDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON2, &CCalR1ATxPowerDlg::OnBnClickedStartCal)
	ON_BN_CLICKED(IDC_BUTTON3, &CCalR1ATxPowerDlg::OnBnClickedStop)
	ON_MESSAGE(WM_CSE_SINGLE_THREAD_DONE, &CCalR1ATxPowerDlg::Done)
	ON_MESSAGE(WM_CSE_CAL_TX_POWER_SHOW, &CCalR1ATxPowerDlg::ShowData)
	ON_BN_CLICKED(IDC_BUTTON6, &CCalR1ATxPowerDlg::OnBnClickedGetAndSet)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON4, &CCalR1ATxPowerDlg::OnBnClickedExportCalFile)
	ON_MESSAGE(WM_CSE_SP3301_CHANGE, &CCalR1ATxPowerDlg::SP3301Change)
	ON_EN_CHANGE(IDC_EDIT6, &CCalR1ATxPowerDlg::OnEnChangeFreqEdit)
	ON_MESSAGE(WM_CSE_UPDATE_DATA_FALSE, &CCalR1ATxPowerDlg::UpdateDataFalse)
	ON_BN_CLICKED(IDC_BUTTON21, &CCalR1ATxPowerDlg::OnBnClickedPause)
	ON_BN_CLICKED(IDC_BUTTON15, &CCalR1ATxPowerDlg::OnBnClickedContinue)
END_MESSAGE_MAP()


void CCalR1ATxPowerDlg::ResetShowCtrl()
{
	int64_t iFreqStar = 0,iFreqStop = 0;
	freq2array(m_strFreqStar,m_strFreqStop,m_strFreqStep,NULL,NULL,NULL,&iFreqStop,&iFreqStar);
	m_pChart->GetBottomAxis()->SetMinMax(iFreqStar / 1000000.0,iFreqStop / 1000000.0);

	int32_t iSec = strsections((LPSTR)(LPCTSTR)m_strFreqStar);
	COLORREF clrOP[4] = {BLACK,RED,GREEN,BLUE};
	COLORREF clrIO[4] = {VIOLET,NAVY,PURPLE,BROWN};
	CalIOMode Mode = (CalIOMode)(m_CalIOMode.GetCurSel());
	BOOL bCalOP = CalOP(Mode);
	BOOL bCalIO = CalIO(Mode);

	for (int i = 0;i < ARRAY_SIZE(m_OPLineSerie);i ++) {
		for (int j = 0;j < m_OPLineSerie[i].size();j ++)
			((m_OPLineSerie[i]).at(j))->ClearSerie();
		m_OPLineSerie[i].clear();
	}
	for (int i = 0;i < ARRAY_SIZE(m_IOLineSerie);i ++) {
		for (int j = 0;j < m_IOLineSerie[i].size();j ++)
			((m_IOLineSerie[i]).at(j))->ClearSerie();
		m_IOLineSerie[i].clear();
	}
	m_pChart->RemoveAllSeries();

	if (bCalOP) {
		for (int i = 0;i < ARRAY_SIZE(m_OPLineSerie);i ++) {
			for (int j = 0;j < iSec;j ++) {
				CChartLineSerie *pLineSerie;
				m_OPLineSerie[i].push_back(pLineSerie);
				m_OPLineSerie[i].at(j) = m_pChart->CreateLineSerie();
				m_OPLineSerie[i].at(j)->SetSeriesOrdering(poNoOrdering);
				m_OPLineSerie[i].at(j)->SetColor(clrOP[i]);
				m_OPLineSerie[i].at(j)->SetWidth(2);
				if (0 == j) {
					TChartString strName;
					TChartStringStream strStr;
					strStr << "OP@" << 10 - i * 10 << "dBm";
					strName = strStr.str();
					m_OPLineSerie[i].at(j)->SetName(strName);
				}
			}
		}
	}
	if (bCalIO) {
		for (int i = 0;i < ARRAY_SIZE(m_IOLineSerie);i ++) {
			for (int j = 0;j < iSec;j ++) {
				CChartLineSerie *pLineSerie;
				m_IOLineSerie[i].push_back(pLineSerie);
				m_IOLineSerie[i].at(j) = m_pChart->CreateLineSerie();
				m_IOLineSerie[i].at(j)->SetSeriesOrdering(poNoOrdering);
				m_IOLineSerie[i].at(j)->SetColor(clrIO[i]);
				m_IOLineSerie[i].at(j)->SetWidth(2);
				if (0 == j) {
					TChartString strName;
					TChartStringStream strStr;
					strStr << "IO@" << 0 - i * 10 << "dBm";
					strName = strStr.str();
					m_IOLineSerie[i].at(j)->SetName(strName);
				}
			}
		}
	}
}

void CCalR1ATxPowerDlg::OnBnClickedStartCal()
{
	CAL_THREAD_EXISTED_CHKV();
	UpdateData(TRUE);
	CtrlPrepare();
	ResetShowCtrl();

	m_pThread = new CCalR1ATxPowerThread(this);
	m_pThread->CreateThread();
}

LRESULT CCalR1ATxPowerDlg::ShowData(WPARAM wParam,LPARAM lParam)
{
	CCalR1ATxPowerThread::CalTxPowerResult Data = *(CCalR1ATxPowerThread::CalTxPowerResult *)lParam;
	CalIOMode Mode = *(CalIOMode *)wParam;
	switch (Mode) {
		case CAL_IO : {
			for (int i = 0;i < ARRAY_SIZE(m_IOLineSerie);i ++)
				(m_IOLineSerie[i].at(Data.m_iSec))->AddPoint(Data.m_State[i].m_uiFreq / 1000000.0,Data.m_State[i].m_dDGain);
			return 0;
						   }
		case CAL_OP : {
			for (int i = 0;i < ARRAY_SIZE(m_OPLineSerie);i ++)
				(m_OPLineSerie[i].at(Data.m_iSec))->AddPoint(Data.m_State[i].m_uiFreq / 1000000.0,Data.m_State[i].m_dDGain);
			return 0;
							   }
		default:return 0;
	}
	return 0;
}

void CCalR1ATxPowerDlg::OnBnClickedGetAndSet()
{
	UpdateData(TRUE);
	double dDGain = 0.0,dDGainOffset = 0.0;
	int32_t iAtt1 = 0,iAtt2 = 0,iAtt3 = 0;
	SP1401::TxPAAtt PAAtt = SP1401::Tx_Att;
	IOMode Mode = ((m_IOMode.GetCurSel()) == 0 ? IO : OutPut);
	uint64_t uiFreq = 0;
	double dPower = m_dPower;

	str2freq(m_strFreq,uiFreq);

	((CalFileR1A *)(m_pSP1401->GetCalFile()))->GetTxPower(uiFreq,dPower,Mode,dDGain,iAtt1,iAtt2,PAAtt,iAtt3);
	((CalFileR1A *)(m_pSP1401->GetCalFile()))->GetTxAtt(uiFreq,Mode,iAtt1 + iAtt2 + iAtt3,dDGainOffset);

	m_strPAAtt.Format("%s",SP1401::Tx_PA == PAAtt ? "PA" : "Att");
	m_iAtt1 = iAtt1;
	m_iAtt2 = iAtt2;
	m_iAtt3 = iAtt3;
	m_dDGain = dDGain + dDGainOffset;

	m_pSP1401->SetIOMode(Mode);
	m_pSP1401->SetTxPAAttSw(PAAtt);
	m_pSP1401->SetTxAtt(iAtt1,iAtt2,iAtt3);
	m_pSP2401->SetTxPowerComp(m_dDGain);
	UpdateData(FALSE);
}

void CCalR1ATxPowerDlg::OnBnClickedExportCalFile()
{
	CAL_THREAD_EXISTED_CHKV();
	UpdateData(TRUE);
	ResetShowCtrl();

	m_pThread = new CExportR1ATxPowerThread(this);
	m_pThread->CreateThread();
}

LRESULT CCalR1ATxPowerDlg::SP3301Change(WPARAM wParam,LPARAM lParam)
{
	CString strPath;
	strPath.Format("c:\\CSECal\\rfu%drf%d.cal",m_pSP1401->GetRfuIdx(),m_pSP1401->GetRfIdx());
	((CEdit *)(GetDlgItem(IDC_EDIT4)))->SetWindowText(strPath);
	return 0;
}