#include "stdafx.h"
#include "RFU_Debug_Tool.h"
#include "RFR1AGuardDlg.h"
#include "ChartDateTimeAxis.h"
#include "ChartAxisLabel.h"
#include "RFR1AGuardThread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CRFR1AGuardDlg, CAttachThreadDlg)
IMPLEMENT_CSE_ATTACHTHREAD_DLG(CRFR1AGuardDlg)

CRFR1AGuardDlg::CRFR1AGuardDlg(CWnd* pParent /*=NULL*/)
	: CAttachThreadDlg(CRFR1AGuardDlg::IDD, pParent)
{
	m_pSP1401.clear();
	m_pTempLineSerie.clear();
	m_pChart = NULL;
}

CRFR1AGuardDlg::~CRFR1AGuardDlg()
{
	for (int32_t i = 0;i < m_pTempLineSerie.size();i ++)
		m_pTempLineSerie[i]->ClearSerie();
	m_pTempLineSerie.clear();

	if (m_pChart) {
		delete m_pChart;
		m_pChart = NULL;
	}
}

void CRFR1AGuardDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_Mode);
}


BEGIN_MESSAGE_MAP(CRFR1AGuardDlg, CDialog)
	ON_WM_SIZE()
	ON_WM_VSCROLL()
	ON_BN_CLICKED(IDC_CHECK1, &CRFR1AGuardDlg::OnBnClickedStartRecord)
	ON_MESSAGE(WM_CSE_RF_R1A_TEMP_GUARD_SHOW, &CRFR1AGuardDlg::ShowData)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CRFR1AGuardDlg::OnCbnSelchangeBlowerAjustMode)
	ON_MESSAGE(WM_CSE_SINGLE_THREAD_DONE, &CRFR1AGuardDlg::SetToIdle)
END_MESSAGE_MAP()


BOOL CRFR1AGuardDlg::OnInitDialog()
{
	CAttachThreadDlg::OnInitDialog();

	m_Mode.SetCurSel(0);

	m_pChart = new CChartCtrl;
	m_pChart->Create(this,CRect(360,15,800,260),IDC_RF_R1A_TEMP_CHART);
	m_pChart->GetLegend()->UndockLegend(50,20);
	m_pChart->GetLegend()->SetVisible(true);
	m_pChart->GetTitle()->AddString(_T("Temperature(Not Recording)"));
	m_pChart->SetZoomRectColor(BLACK);

	CChartDateTimeAxis *pDataTimeAxis = m_pChart->CreateDateTimeAxis(CChartCtrl::BottomAxis);
	pDataTimeAxis->SetDiscrete(false);
	pDataTimeAxis->SetTickIncrement(true,CChartDateTimeAxis::tiSecond,5);
	pDataTimeAxis->SetAutomatic(true);
	pDataTimeAxis->SetTickLabelFormat(false,_T("%H:%M:%S"));
	CChartStandardAxis *pTempAxis = m_pChart->CreateStandardAxis(CChartCtrl::LeftAxis);
	pTempAxis->SetAutomatic(false);
	pTempAxis->SetMinMax(40.0,80.0);
	pTempAxis->GetGrid()->SetVisible(false);

	this->GetWindowRect(&m_crWnd);

	int nID[11] = { IDC_SLIDER_BLOWER_1,
					IDC_SLIDER_BLOWER_2,
					IDC_SLIDER_BLOWER_3,
					IDC_SLIDER_BLOWER_4,
					IDC_SLIDER_BLOWER_5,
					IDC_SLIDER_BLOWER_6,
					IDC_SLIDER_BLOWER_7,
					IDC_SLIDER_BLOWER_8,
					IDC_SLIDER_BLOWER_9,
					IDC_SLIDER_BLOWER_10,
					IDC_SLIDER_BLOWER_11
	};
	for (int i = 0;i < ARRAY_SIZE(nID);i ++) {
		((CSliderCtrl *)(GetDlgItem(nID[i])))->SetRange(0,255);
		((CSliderCtrl *)(GetDlgItem(nID[i])))->SetLineSize(1);
		((CSliderCtrl *)(GetDlgItem(nID[i])))->SetTicFreq(1);
		((CSliderCtrl *)(GetDlgItem(nID[i])))->SetPos(255);
		AddThreadIdleCtrl(nID[i]);
	}
	
	SetControlInfo(IDC_SLIDER_BLOWER_1,FixL_yStretch,1,1,1,1,0,1);
	SetControlInfo(IDC_SLIDER_BLOWER_2,FixL_yStretch,1,1,1,1,0,1);
	SetControlInfo(IDC_SLIDER_BLOWER_3,FixL_yStretch,1,1,1,1,0,1);
	SetControlInfo(IDC_SLIDER_BLOWER_4,FixL_yStretch,1,1,1,1,0,1);
	SetControlInfo(IDC_SLIDER_BLOWER_5,FixL_yStretch,1,1,1,1,0,1);
	SetControlInfo(IDC_SLIDER_BLOWER_6,FixL_yStretch,1,1,1,1,0,1);
	SetControlInfo(IDC_SLIDER_BLOWER_7,FixL_yStretch,1,1,1,1,0,1);
	SetControlInfo(IDC_SLIDER_BLOWER_8,FixL_yStretch,1,1,1,1,0,1);
	SetControlInfo(IDC_SLIDER_BLOWER_9,FixL_yStretch,1,1,1,1,0,1);
	SetControlInfo(IDC_SLIDER_BLOWER_10,FixL_yStretch,1,1,1,1,0,1);
	SetControlInfo(IDC_SLIDER_BLOWER_11,FixL_yStretch,1,1,1,1,0,1);
	SetControlInfo(IDC_RF_R1A_TEMP_CHART,FixL_xyStretch,1,0,1,1,0,1);
	SetControlInfo(IDC_STATIC_RF_R1A_GUARD_GROUP_BOX,FixL_xyStretch,1,0,1,1,0,1);

	AddThreadIdleCtrl(IDC_CHECK1);
	CtrlResume();
	return TRUE;
}

void CRFR1AGuardDlg::ResetChart()
{
	for (int32_t i = 0;i < m_pTempLineSerie.size();i ++)
		m_pTempLineSerie[i]->ClearSerie();
	m_pTempLineSerie.clear();
	m_pChart->RemoveAllSeries();

	COLORREF clrTemp[8] = {BLACK,RED,GREEN,BLUE,BROWN,NAVY,PURPLE,GREY};
	
	for (int32_t i = 0;i < MAX_RF * 2;i ++) {
		TChartStringStream strStr;
		CChartLineSerie *pLineSerie = m_pChart->CreateLineSerie();
		m_pTempLineSerie.push_back(pLineSerie);
		m_pTempLineSerie[i]->SetSeriesOrdering(poNoOrdering);
		m_pTempLineSerie[i]->SetColor(clrTemp[i]);
		strStr << "RF" << (i / 2) << (iseven(i) ? "_Tx" : "_Rx");
		m_pTempLineSerie[i]->SetName(strStr.str());
		m_pTempLineSerie[i]->SetVisible(true);
	}
}

void CRFR1AGuardDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	int nCurCtrlID = pScrollBar->GetDlgCtrlID();
	if (nCurCtrlID < IDC_SLIDER_BLOWER_1 || nCurCtrlID > IDC_SLIDER_BLOWER_11)
		return;

	int nCurCtrlPos = ((CSliderCtrl *)pScrollBar)->GetPos();
	int nBlowerID[11] = { IDC_SLIDER_BLOWER_1,
		IDC_SLIDER_BLOWER_2,
		IDC_SLIDER_BLOWER_3,
		IDC_SLIDER_BLOWER_4,
		IDC_SLIDER_BLOWER_5,
		IDC_SLIDER_BLOWER_6,
		IDC_SLIDER_BLOWER_7,
		IDC_SLIDER_BLOWER_8,
		IDC_SLIDER_BLOWER_9,
		IDC_SLIDER_BLOWER_10,
		IDC_SLIDER_BLOWER_11
	};
	int nSpeedID[11] = {IDC_STATIC_BLOWER_1_SPEED,
		IDC_STATIC_BLOWER_2_SPEED,
		IDC_STATIC_BLOWER_3_SPEED,
		IDC_STATIC_BLOWER_4_SPEED,
		IDC_STATIC_BLOWER_5_SPEED,
		IDC_STATIC_BLOWER_6_SPEED,
		IDC_STATIC_BLOWER_7_SPEED,
		IDC_STATIC_BLOWER_8_SPEED,
		IDC_STATIC_BLOWER_9_SPEED,
		IDC_STATIC_BLOWER_10_SPEED,
		IDC_STATIC_BLOWER_11_SPEED
	};
	if (0 == m_Mode.GetCurSel()) {
		for (int i = 0;i < ARRAY_SIZE(nBlowerID);i ++) {
			m_pSP3501->SetBlower(i,255 - nCurCtrlPos);
			if (nBlowerID[i] != pScrollBar->GetDlgCtrlID())
				((CSliderCtrl *)(GetDlgItem(nBlowerID[i])))->SetPos(nCurCtrlPos);
		}
	}
	if (1 == m_Mode.GetCurSel()) {
		for (int i = 0;i < ARRAY_SIZE(nBlowerID);i ++) {
			if (nBlowerID[i] == pScrollBar->GetDlgCtrlID()) {
				m_pSP3501->SetBlower(i,255 - nCurCtrlPos);
				break;
			}
		}
	}

	for (int i = 0;i < ARRAY_SIZE(nBlowerID);i ++) {
		int iPos = ((CSliderCtrl *)(GetDlgItem(nBlowerID[i])))->GetPos();
		CString strSpeed = "";
		strSpeed.Format("%d",255 - iPos);
		for (int32_t j = 0;j < MAX_RF;j ++) {
			if (::IsWindow(ALL_DLG->m_pRFR1CDlg[j])) {
				((CSliderCtrl *)(ALL_DLG->m_pRFR1CDlg[j].GetDlgItem(nBlowerID[i])))->SetPos(iPos);
				((CStatic *)(ALL_DLG->m_pRFR1CDlg[j].GetDlgItem(nSpeedID[i])))->SetWindowText(strSpeed);
			}
		}
	}
	CAttachThreadDlg::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CRFR1AGuardDlg::OnBnClickedStartRecord()
{
	if (BST_CHECKED == ((CButton *)(GetDlgItem(IDC_CHECK1)))->GetCheck()) {
		TEST_THREAD_EXISTED_CHKV();
		ResetChart();
		m_pThread = new CRFR1AGuardThread(this);
		m_pThread->CreateThread();
		((CButton *)(GetDlgItem(IDC_CHECK1)))->SetWindowText(_T("Stop Record-->"));
		m_pChart->GetTitle()->RemoveAll();
		m_pChart->GetTitle()->AddString(_T("Temperature(Recording)(X:Time Y:Temp)"));
	}
	else {
		((CButton *)(GetDlgItem(IDC_CHECK1)))->SetWindowText(_T("Start Record-->"));
		m_pChart->GetTitle()->RemoveAll();
		m_pChart->GetTitle()->AddString(_T("Temperature(Not Recording)"));
		m_pChart->GetTitle()->SetColor(BLACK);
		CCSEWinThread::m_bRunning = FALSE;
	}
}

LRESULT CRFR1AGuardDlg::ShowData(WPARAM wParam,LPARAM lParam)
{
	CRFR1AGuardThread::Result Data = *(CRFR1AGuardThread::Result *)lParam;
	
	m_pChart->GetTitle()->SetColor(iseven(Data.m_iIdx) ? BLACK : GREEN);
	for (int32_t i = 0;i < MAX_RF * 2;i ++) {
		TChartString strName = m_pTempLineSerie.at(i)->GetName();
		strName.resize(6);
		TChartStringStream strStr;
		if (Data.m_dTemp[i] > 0.0) {
			strStr << strName << "   " << Data.m_dTemp[i];
			m_pTempLineSerie.at(i)->SetName(strStr.str());
			m_pTempLineSerie.at(i)->AddPoint(Data.m_Time,Data.m_dTemp[i]);
		}
		else {
			m_pTempLineSerie.at(i)->AddPoint(Data.m_Time,Data.m_dTemp[i]);
		}
	}
	return 0;
}

void CRFR1AGuardDlg::OnCbnSelchangeBlowerAjustMode()
{
	int iMode = m_Mode.GetCurSel();
	for (int32_t i = 0;i < MAX_RF;i ++) {
		if (::IsWindow(ALL_DLG->m_pRFR1CDlg[i]))
			ALL_DLG->m_pRFR1CDlg[i].m_BlowerAjustMode.SetCurSel(iMode);
	}
}

LRESULT CRFR1AGuardDlg::SetToIdle(WPARAM wParam,LPARAM lParam)
{
	CCSEWinThread::m_bRunning = FALSE;
	return 0;
}