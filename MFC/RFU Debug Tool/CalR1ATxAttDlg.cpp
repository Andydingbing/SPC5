#include "stdafx.h"
#include "RFU_Debug_Tool.h"
#include "CalR1ATxAttDlg.h"
#include "CalR1ATxAttThread.h"

#define CLR_ATT_10 BLACK
#define CLR_ATT_20 RED
#define CLR_ATT_30 GREEN
#define CLR_ATT_40 BLUE
#define CLR_ATT_50 BROWN
#define CLR_ATT_60 NAVY
#define CLR_ATT_70 PURPLE
#define CLR_ATT_80 GREY
#define CLR_ATT_90 ORANGE

#define UPDATE_ONELINE_SHOW(line_serie_group,show)								\
	for (int i = 0;i < line_serie_group.size();i ++) {							\
		pChartSerie = m_pChart->GetSerie(line_serie_group.at(i)->GetSerieId());	\
		pChartSerie->SetVisible(show);											\
	}

#define UPDATE_LINE_SHOW(line_serie_group_op,line_serie_group_io,show)			\
	UpdateData(TRUE);															\
	CChartSerie *pChartSerie;													\
	if (CalOP((CalIOMode)(m_CalIOMode.GetCurSel()))) {							\
		UPDATE_ONELINE_SHOW(line_serie_group_op,show);							\
	}																			\
	if (CalIO((CalIOMode)(m_CalIOMode.GetCurSel()))) {							\
		UPDATE_ONELINE_SHOW(line_serie_group_io,show);							\
	}

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CCalR1ATxAttDlg, CCalR1ABaseDlg)
IMPLEMENT_CSE_CAL_R1A_DLG(CCalR1ATxAttDlg)

CCalR1ATxAttDlg::CCalR1ATxAttDlg(CWnd* pParent /*=NULL*/)
	: CCalR1ABaseDlg(CCalR1ATxAttDlg::IDD, _T("2G"), pParent)
	, m_strFreqStar(_T(""))
	, m_strFreqStop(_T(""))
	, m_strFreqStep(_T(""))
	, m_bShowAtt10(TRUE)
	, m_bShowAtt20(TRUE)
	, m_bShowAtt30(TRUE)
	, m_bShowAtt40(TRUE)
	, m_bShowAtt50(TRUE)
	, m_bShowAtt60(TRUE)
	, m_bShowAtt70(TRUE)
	, m_bShowAtt80(TRUE)
	, m_bShowAtt90(TRUE)
	, m_dAttOffset(0)
{
	m_pChart = NULL;
	for (int i = 0;i < ARRAY_SIZE(m_OPLineSerie);i ++)
		m_OPLineSerie[i].clear();
	for (int i = 0;i < ARRAY_SIZE(m_IOLineSerie);i ++)
		m_IOLineSerie[i].clear();
}

CCalR1ATxAttDlg::~CCalR1ATxAttDlg()
{
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
	if (m_pChart) {
		delete m_pChart;
		m_pChart = NULL;
	}
}

void CCalR1ATxAttDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strFreqStar);
	DDX_Text(pDX, IDC_EDIT2, m_strFreqStop);
	DDX_Text(pDX, IDC_EDIT3, m_strFreqStep);
	DDX_Check(pDX, IDC_CHECK3,  m_bShowAtt10);
	DDX_Check(pDX, IDC_CHECK4,  m_bShowAtt20);
	DDX_Check(pDX, IDC_CHECK25, m_bShowAtt30);
	DDX_Check(pDX, IDC_CHECK27, m_bShowAtt40);
	DDX_Check(pDX, IDC_CHECK28, m_bShowAtt50);
	DDX_Check(pDX, IDC_CHECK29, m_bShowAtt60);
	DDX_Check(pDX, IDC_CHECK30, m_bShowAtt70);
	DDX_Check(pDX, IDC_CHECK31, m_bShowAtt80);
	DDX_Check(pDX, IDC_CHECK32, m_bShowAtt90);
	DDX_Control(pDX, IDC_COMBO1, m_CalIOMode);
	DDX_Text(pDX, IDC_EDIT5, m_strFreq);
	DDX_Control(pDX, IDC_COMBO2, m_IOMode);
	DDX_Control(pDX, IDC_COMBO3, m_Att);
	DDX_Text(pDX, IDC_EDIT6, m_dAttOffset);
}


BEGIN_MESSAGE_MAP(CCalR1ATxAttDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON2, &CCalR1ATxAttDlg::OnBnClickedStartCal)
	ON_BN_CLICKED(IDC_BUTTON3, &CCalR1ATxAttDlg::OnBnClickedStop)
	ON_BN_CLICKED(IDC_BUTTON9, &CCalR1ATxAttDlg::OnBnClickedPause)
	ON_BN_CLICKED(IDC_BUTTON5, &CCalR1ATxAttDlg::OnBnClickedContinue)
	ON_MESSAGE(WM_CSE_CAL_TX_ATT_SHOW, &CCalR1ATxAttDlg::ShowData)
	ON_MESSAGE(WM_CSE_SINGLE_THREAD_DONE, &CCalR1ATxAttDlg::Done)
	ON_BN_CLICKED(IDC_CHECK3, &CCalR1ATxAttDlg::OnBnClickedShowAtt10)
	ON_BN_CLICKED(IDC_CHECK4, &CCalR1ATxAttDlg::OnBnClickedShowAtt20)
	ON_BN_CLICKED(IDC_CHECK25, &CCalR1ATxAttDlg::OnBnClickedShowAtt30)
	ON_BN_CLICKED(IDC_CHECK27, &CCalR1ATxAttDlg::OnBnClickedShowAtt40)
	ON_BN_CLICKED(IDC_CHECK28, &CCalR1ATxAttDlg::OnBnClickedShowAtt50)
	ON_BN_CLICKED(IDC_CHECK29, &CCalR1ATxAttDlg::OnBnClickedShowAtt60)
	ON_BN_CLICKED(IDC_CHECK30, &CCalR1ATxAttDlg::OnBnClickedShowAtt70)
	ON_BN_CLICKED(IDC_CHECK31, &CCalR1ATxAttDlg::OnBnClickedShowAtt80)
	ON_BN_CLICKED(IDC_CHECK32, &CCalR1ATxAttDlg::OnBnClickedShowAtt90)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON4, &CCalR1ATxAttDlg::OnBnClickedExportCalFile)
	ON_WM_SIZE()
	ON_CBN_SELCHANGE(IDC_COMBO1, &CCalR1ATxAttDlg::OnCbnSelchangeIOMode)
	ON_MESSAGE(WM_CSE_SP3301_CHANGE, &CCalR1ATxAttDlg::SP3301Change)
	ON_BN_CLICKED(IDC_BUTTON1, &CCalR1ATxAttDlg::OnBnClickedGetAndSet)
	ON_MESSAGE(WM_CSE_UPDATE_DATA_FALSE, &CCalR1ATxAttDlg::UpdateDataFalse)
END_MESSAGE_MAP()


void CCalR1ATxAttDlg::ResetShowCtrl()
{
	int64_t iFreqStar = 0,iFreqStop = 0;
	freq2array(m_strFreqStar,m_strFreqStop,m_strFreqStep,NULL,NULL,NULL,&iFreqStop,&iFreqStar);
	m_pChart->GetBottomAxis()->SetMinMax(iFreqStar / 1000000.0,iFreqStop / 1000000.0);

	int32_t iSec = strsections((LPSTR)(LPCTSTR)m_strFreqStar);
	COLORREF clrAtt[9] = {CLR_ATT_10,CLR_ATT_20,CLR_ATT_30,CLR_ATT_40,CLR_ATT_50,CLR_ATT_60,CLR_ATT_70,CLR_ATT_80,CLR_ATT_90};
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
				m_OPLineSerie[i].at(j)->SetColor(clrAtt[i]);
				if (0 == j) {
					TChartString strName;
					TChartStringStream strStr;
					strStr << "Att" << i + 1 << "0";
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
				m_IOLineSerie[i].at(j)->SetColor(clrAtt[i]);
				if (0 == j) {
					if (FALSE == bCalOP) {
						TChartString strName;
						TChartStringStream strStr;
						strStr << "Att" << i + 1 << "0";
						strName = strStr.str();
						m_IOLineSerie[i].at(j)->SetName(strName);
					}
				}
			}
		}
	}
}

void CCalR1ATxAttDlg::OnBnClickedStartCal()
{
	CAL_THREAD_EXISTED_CHKV();
	UpdateData(TRUE);
	CtrlPrepare();
	ResetShowCtrl();

	m_pThread = new CCalR1ATxAttThread(this);
	m_pThread->CreateThread();
}

BOOL CCalR1ATxAttDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_pChart = new CChartCtrl;
	m_pChart->Create(this,CAL_CHART_RECT,IDC_CAL_R1A_TX_ATT_CHART);
	m_pChart->GetLegend()->SetHorizontalMode(true);
	m_pChart->GetLegend()->UndockLegend(50,50);
	m_pChart->GetLegend()->SetVisible(true);
	m_pChart->SetZoomRectColor(BLACK);
	m_pChart->GetTitle()->AddString(_T("Att Offset Curve(X:Freq(MHz) Y:Offset(dB))"));
	m_pChart->GetTitle()->SetColor(BLUE);

	CChartAxis *pAxis;
	pAxis = m_pChart->CreateStandardAxis(CChartCtrl::BottomAxis);
	pAxis->SetAutomatic(false);
	pAxis->SetMinMax(RF_TX_FREQ_STAR / 1000000.0,RF_TX_FREQ_STOP / 1000000.0);
	pAxis = m_pChart->CreateStandardAxis(CChartCtrl::LeftAxis);
	pAxis->SetAutomatic(false);
	pAxis->SetMinMax(-3.0,2.0);

	m_CalIOMode.SetCurSel(2);
	m_IOMode.SetCurSel(0);
	m_Att.SetCurSel(0);
	m_strFreqStar.Format("%dM",RF_TX_FREQ_STAR / 1000000);
	m_strFreqStop.Format("%dM",RF_TX_FREQ_STOP / 1000000);
	m_strFreqStep.Format("%dM",RF_TX_FREQ_STEP_CALLED / 1000000);

	CString strPath;
	strPath.Format("c:\\CSECal\\rfu%drf%d.cal",m_pSP1401->GetRfuIdx(),m_pSP1401->GetRfIdx());
	((CEdit *)(GetDlgItem(IDC_EDIT4)))->SetWindowText(strPath);
	UpdateData(FALSE);

	this->GetWindowRect(&m_crWnd);
	SetControlInfo(IDC_EDIT1,FixL_xStretch,1,0,1);
	SetControlInfo(IDC_EDIT2,FixL_xStretch,1,0,1);
	SetControlInfo(IDC_CHECK3,ChgL_Fixx,1,1,1);
	SetControlInfo(IDC_CHECK4,ChgL_Fixx,1,1,1);
	SetControlInfo(IDC_CHECK25,ChgL_Fixx,1,1,1);
	SetControlInfo(IDC_CHECK27,ChgL_Fixx,1,1,1);
	SetControlInfo(IDC_CHECK28,ChgL_Fixx,1,1,1);
	SetControlInfo(IDC_CHECK29,ChgL_Fixx,1,1,1);
	SetControlInfo(IDC_CHECK30,ChgL_Fixx,1,1,1);
	SetControlInfo(IDC_CHECK31,ChgL_Fixx,1,1,1);
	SetControlInfo(IDC_CHECK32,ChgL_Fixx,1,1,1);
	SetControlInfo(IDC_EDIT4,ChgL_Fixx,1,1,1);
	SetControlInfo(IDC_BUTTON4,ChgL_Fixx,1,1,1);
	SetControlInfo(IDC_STATIC_30,ChgL_Fixx,1,1,1);
	SetControlInfo(IDC_STATIC_31,ChgL_Fixx,1,1,1);
	SetControlInfo(IDC_STATIC_32,ChgL_Fixx,1,1,1);
	SetControlInfo(IDC_STATIC_33,ChgL_Fixx,1,1,1);
	SetControlInfo(IDC_STATIC_34,ChgL_Fixx,1,1,1);
	SetControlInfo(IDC_STATIC_35,ChgL_Fixx,1,1,1);
	SetControlInfo(IDC_STATIC_36,ChgL_Fixx,1,1,1);
	SetControlInfo(IDC_STATIC_37,ChgL_Fixx,1,1,1);
	SetControlInfo(IDC_STATIC_38,ChgL_Fixx,1,1,1);
	SetControlInfo(IDC_CAL_R1A_TX_ATT_CHART,FixL_xyStretch,1,0,1,1,0,1);
	AddThreadIdleCtrl(IDC_EDIT1);
	AddThreadIdleCtrl(IDC_EDIT2);
	AddThreadIdleCtrl(IDC_EDIT3);
	AddThreadIdleCtrl(IDC_COMBO1);
	AddThreadIdleCtrl(IDC_BUTTON2);
	AddThreadIdleCtrl(IDC_CHECK3);
	AddThreadIdleCtrl(IDC_CHECK4);
	AddThreadIdleCtrl(IDC_CHECK25);
	AddThreadIdleCtrl(IDC_CHECK27);
	AddThreadIdleCtrl(IDC_CHECK28);
	AddThreadIdleCtrl(IDC_CHECK29);
	AddThreadIdleCtrl(IDC_CHECK30);
	AddThreadIdleCtrl(IDC_CHECK31);
	AddThreadIdleCtrl(IDC_CHECK32);
	AddThreadIdleCtrl(IDC_BUTTON4);
	AddThreadIdleCtrl(IDC_EDIT5);
	AddThreadIdleCtrl(IDC_COMBO2);
	AddThreadIdleCtrl(IDC_COMBO3);
	AddThreadIdleCtrl(IDC_EDIT6);
	AddThreadIdleCtrl(IDC_BUTTON1);
	AddThreadRunningCtrl(IDC_BUTTON3);
	AddThreadToPauseCtrl(IDC_BUTTON9);
	AddThreadToRunningCtrl(IDC_BUTTON5);
	CtrlResume();
	return TRUE;
}

LRESULT CCalR1ATxAttDlg::ShowData(WPARAM wParam,LPARAM lParam)
{
	CCalR1ATxAttThread::Result Data = *(CCalR1ATxAttThread::Result *)lParam;
	CalIOMode Mode = *(CalIOMode *)wParam;
	double dAttData[9] = {(double)(Data.m_fAtt10),(double)(Data.m_fAtt20),(double)(Data.m_fAtt30),
						  (double)(Data.m_fAtt40),(double)(Data.m_fAtt50),(double)(Data.m_fAtt60),
						  (double)(Data.m_fAtt70),(double)(Data.m_fAtt80),(double)(Data.m_fAtt90)};
	switch (Mode) {
		case CAL_IO : {
			for (int32_t i = 0;i < ARRAY_SIZE(m_IOLineSerie);i ++) {
				if (CCalR1ATxAttThread::ShowLine(Data.m_iLine,i))
					m_IOLineSerie[i].at(Data.m_iSec)->AddPoint(Data.m_uiFreq / 1000000.0,(double)(dAttData[i]));
			}
			return 0;
				}
		case CAL_OP : {
			for (int32_t i = 0;i < ARRAY_SIZE(m_OPLineSerie);i ++) {
				if (CCalR1ATxAttThread::ShowLine(Data.m_iLine,i))
					m_OPLineSerie[i].at(Data.m_iSec)->AddPoint(Data.m_uiFreq / 1000000.0,(double)(dAttData[i]));
			}
			return 0;
				 }
		default:return 0;
	}
	return 0;
}

void CCalR1ATxAttDlg::OnBnClickedShowAtt10()
{
	UPDATE_LINE_SHOW(m_OPLineSerie[0],m_IOLineSerie[0],m_bShowAtt10);
}

void CCalR1ATxAttDlg::OnBnClickedShowAtt20()
{
	UPDATE_LINE_SHOW(m_OPLineSerie[1],m_IOLineSerie[1],m_bShowAtt20);
}

void CCalR1ATxAttDlg::OnBnClickedShowAtt30()
{
	UPDATE_LINE_SHOW(m_OPLineSerie[2],m_IOLineSerie[2],m_bShowAtt30);
}

void CCalR1ATxAttDlg::OnBnClickedShowAtt40()
{
	UPDATE_LINE_SHOW(m_OPLineSerie[3],m_IOLineSerie[3],m_bShowAtt40);
}

void CCalR1ATxAttDlg::OnBnClickedShowAtt50()
{
	UPDATE_LINE_SHOW(m_OPLineSerie[4],m_IOLineSerie[4],m_bShowAtt50);
}

void CCalR1ATxAttDlg::OnBnClickedShowAtt60()
{
	UPDATE_LINE_SHOW(m_OPLineSerie[5],m_IOLineSerie[5],m_bShowAtt60);
}

void CCalR1ATxAttDlg::OnBnClickedShowAtt70()
{
	UPDATE_LINE_SHOW(m_OPLineSerie[6],m_IOLineSerie[6],m_bShowAtt70);
}

void CCalR1ATxAttDlg::OnBnClickedShowAtt80()
{
	UPDATE_LINE_SHOW(m_OPLineSerie[7],m_IOLineSerie[7],m_bShowAtt80);
}

void CCalR1ATxAttDlg::OnBnClickedShowAtt90()
{
	UPDATE_LINE_SHOW(m_OPLineSerie[8],m_IOLineSerie[8],m_bShowAtt90);
}

HBRUSH CCalR1ATxAttDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	COLORREF clrText = CLR_ATT_10;

	switch (pWnd->GetDlgCtrlID()) {
		case IDC_STATIC_30 : {clrText = CLR_ATT_10;break;}
		case IDC_STATIC_31 : {clrText = CLR_ATT_20;break;}
		case IDC_STATIC_32 : {clrText = CLR_ATT_30;break;}
		case IDC_STATIC_33 : {clrText = CLR_ATT_40;break;}
		case IDC_STATIC_34 : {clrText = CLR_ATT_50;break;}
		case IDC_STATIC_35 : {clrText = CLR_ATT_60;break;}
		case IDC_STATIC_36 : {clrText = CLR_ATT_70;break;}
		case IDC_STATIC_37 : {clrText = CLR_ATT_80;break;}
		case IDC_STATIC_38 : {clrText = CLR_ATT_90;break;}
		default:
			return hbr;
	}
	pDC->SetTextColor(clrText);
	return (HBRUSH)GetSysColorBrush(COLOR_BTNFACE);
}

void CCalR1ATxAttDlg::OnBnClickedExportCalFile()
{
	CAL_THREAD_EXISTED_CHKV();
	UpdateData(TRUE);
	ResetShowCtrl();

	m_pThread = new CExportR1ATxAttThread(this);
	m_pThread->CreateThread();
}

void CCalR1ATxAttDlg::OnBnClickedGetAndSet()
{
	UpdateData(TRUE);
	uint64_t uiFreq = 0;
	str2freq(m_strFreq,uiFreq);
	IOMode Mode = (m_IOMode.GetCurSel() == 0 ? IO : OutPut);
	int32_t iAtt = m_Att.GetCurSel() * 10;

	((CalFileR1A *)(m_pSP1401->GetCalFile()))->GetTxAtt(uiFreq,Mode,iAtt,m_dAttOffset);

	UpdateData(FALSE);
	STACHKV(m_pSP1401->SetTxFreq(uiFreq));
	STACHKV(m_pSP1401->SetTxAtt((double)iAtt));
	STACHKV(m_pSP2401->SetTxPowerComp(m_dAttOffset));
	return;
}

void CCalR1ATxAttDlg::OnCbnSelchangeIOMode()
{
	UpdateData(TRUE);
	CChartSerie *pChartSerie;
	CalIOMode Mode = (CalIOMode)(m_CalIOMode.GetCurSel());
	BOOL bCalOP = CalOP(Mode);
	BOOL bCalIO = CalIO(Mode);

	UPDATE_ONELINE_SHOW(m_OPLineSerie[0],bCalOP ? m_bShowAtt10 : bCalOP);
	UPDATE_ONELINE_SHOW(m_OPLineSerie[1],bCalOP ? m_bShowAtt20 : bCalOP);
	UPDATE_ONELINE_SHOW(m_OPLineSerie[2],bCalOP ? m_bShowAtt30 : bCalOP);
	UPDATE_ONELINE_SHOW(m_OPLineSerie[3],bCalOP ? m_bShowAtt40 : bCalOP);
	UPDATE_ONELINE_SHOW(m_OPLineSerie[4],bCalOP ? m_bShowAtt50 : bCalOP);
	UPDATE_ONELINE_SHOW(m_OPLineSerie[5],bCalOP ? m_bShowAtt60 : bCalOP);
	UPDATE_ONELINE_SHOW(m_OPLineSerie[6],bCalOP ? m_bShowAtt70 : bCalOP);
	UPDATE_ONELINE_SHOW(m_OPLineSerie[7],bCalOP ? m_bShowAtt80 : bCalOP);
	UPDATE_ONELINE_SHOW(m_OPLineSerie[8],bCalOP ? m_bShowAtt90 : bCalOP);
	
	UPDATE_ONELINE_SHOW(m_IOLineSerie[0],bCalIO ? m_bShowAtt10 : bCalIO);
	UPDATE_ONELINE_SHOW(m_IOLineSerie[1],bCalIO ? m_bShowAtt20 : bCalIO);
	UPDATE_ONELINE_SHOW(m_IOLineSerie[2],bCalIO ? m_bShowAtt30 : bCalIO);
	UPDATE_ONELINE_SHOW(m_IOLineSerie[3],bCalIO ? m_bShowAtt40 : bCalIO);
	UPDATE_ONELINE_SHOW(m_IOLineSerie[4],bCalIO ? m_bShowAtt50 : bCalIO);
	UPDATE_ONELINE_SHOW(m_IOLineSerie[5],bCalIO ? m_bShowAtt60 : bCalIO);
	UPDATE_ONELINE_SHOW(m_IOLineSerie[6],bCalIO ? m_bShowAtt70 : bCalIO);
	UPDATE_ONELINE_SHOW(m_IOLineSerie[7],bCalIO ? m_bShowAtt80 : bCalIO);
	UPDATE_ONELINE_SHOW(m_IOLineSerie[8],bCalIO ? m_bShowAtt90 : bCalIO);

	if (!m_OPLineSerie[0].empty() && !m_IOLineSerie[0].empty()) {
		if (CAL_IO == Mode) {
			for (int i = 0;i < ARRAY_SIZE(m_OPLineSerie);i ++) {
				m_OPLineSerie[i].at(0)->SetName("");
			}
			for (int i = 0;i < ARRAY_SIZE(m_IOLineSerie);i ++) {
				TChartString strName;
				TChartStringStream strStr;
				strStr << "Att" << i + 1 << "0";
				strName = strStr.str();
				m_IOLineSerie[i].at(0)->SetName(strName);

			}
		}
		if (CAL_OP == Mode) {
			for (int i = 0;i < ARRAY_SIZE(m_OPLineSerie);i ++) {
				TChartString strName;
				TChartStringStream strStr;
				strStr << "Att" << i + 1 << "0";
				strName = strStr.str();
				m_OPLineSerie[i].at(0)->SetName(strName);
			}
			for (int i = 0;i < ARRAY_SIZE(m_IOLineSerie);i ++) {
				m_IOLineSerie[i].at(0)->SetName("");
			}
		}
	} 
}

LRESULT CCalR1ATxAttDlg::SP3301Change(WPARAM wParam,LPARAM lParam)
{
	CString strPath;
	strPath.Format("c:\\CSECal\\rfu%drf%d.cal",m_pSP1401->GetRfuIdx(),m_pSP1401->GetRfIdx());
	((CEdit *)(GetDlgItem(IDC_EDIT4)))->SetWindowText(strPath);
	return 0;
}
