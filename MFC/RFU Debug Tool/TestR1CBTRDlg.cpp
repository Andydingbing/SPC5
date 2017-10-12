#include "stdafx.h"
#include "RFU_Debug_Tool.h"
#include "TestR1CBTRDlg.h"
#include "TestR1CBTRThread.h"
#include "ChartLineSerie.h"
#include "ChartAxisLabel.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CTestR1CBTRDlg, CTestR1CBaseDlg)
IMPLEMENT_CSE_TEST_R1C_DLG(CTestR1CBTRDlg)

CTestR1CBTRDlg::CTestR1CBTRDlg(CWnd* pParent /*=NULL*/)
	: CTestR1CBaseDlg(CTestR1CBTRDlg::IDD, pParent)
	, m_dEnvTemp(25)
	, m_iSpeedStep(10)
	, m_iKeepTime(60)
{
	m_pTempChart = NULL;
	m_pLinerChart = NULL;
}

CTestR1CBTRDlg::~CTestR1CBTRDlg()
{
	int32_t iSerieCnt = 0;
	if (m_pTempChart) {
		if (iSerieCnt = (int32_t)(m_pTempChart->GetSeriesCount())) {
			m_pTempChart->GoToFirstSerie();
			for (int32_t i = 0;i < (int32_t)(m_pTempChart->GetSeriesCount());i ++)
				((CChartLineSerie *)(m_pTempChart->GetNextSerie()))->ClearSerie();
		}
		delete m_pTempChart;
		m_pTempChart = NULL;
	}

	if (m_pLinerChart) {
		if (iSerieCnt = (int32_t)(m_pLinerChart->GetSeriesCount())) {
			m_pLinerChart->GoToFirstSerie();
			for (int32_t i = 0;i < (int32_t)(m_pLinerChart->GetSeriesCount());i ++) {
				((CChartLineSerie *)(m_pLinerChart->GetNextSerie()))->ClearSerie();
			}
		}
		delete m_pLinerChart;
		m_pLinerChart = NULL;
	}
}

void CTestR1CBTRDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_dEnvTemp);
	DDX_Text(pDX, IDC_EDIT2, m_iSpeedStep);
	DDX_Text(pDX, IDC_EDIT3, m_iKeepTime);
}


BEGIN_MESSAGE_MAP(CTestR1CBTRDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CTestR1CBTRDlg::OnBnClickedSetPath)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON3, &CTestR1CBTRDlg::OnBnClickedStop)
	ON_BN_CLICKED(IDC_BUTTON2, &CTestR1CBTRDlg::OnBnClickedStart)
	ON_MESSAGE(WM_CSE_TEST_BTR_SHOW, &CTestR1CBTRDlg::ShowData)
	ON_MESSAGE(WM_CSE_UPDATE_DATA_FALSE, &CTestR1CBTRDlg::UpdateDataFalse)
	ON_MESSAGE(WM_CSE_SINGLE_THREAD_DONE, &CTestR1CBTRDlg::Done)
	ON_BN_CLICKED(IDC_CHECK1, &CTestR1CBTRDlg::OnBnClickedShowRF0)
	ON_BN_CLICKED(IDC_CHECK2, &CTestR1CBTRDlg::OnBnClickedShowRF1)
	ON_BN_CLICKED(IDC_CHECK5, &CTestR1CBTRDlg::OnBnClickedShowRF2)
	ON_BN_CLICKED(IDC_CHECK6, &CTestR1CBTRDlg::OnBnClickedShowRF3)
	ON_BN_CLICKED(IDC_CHECK3, &CTestR1CBTRDlg::OnBnClickedShowFitData)
	ON_BN_CLICKED(IDC_CHECK4, &CTestR1CBTRDlg::OnBnClickedShowOriginLinerData)
	ON_EN_CHANGE(IDC_EDIT1, &CTestR1CBTRDlg::OnEnChangeEnvTemp)
	ON_EN_CHANGE(IDC_EDIT2, &CTestR1CBTRDlg::OnEnChangeSpeedStep)
	ON_EN_CHANGE(IDC_EDIT3, &CTestR1CBTRDlg::OnEnChangeKeepTime)
END_MESSAGE_MAP()


LRESULT CTestR1CBTRDlg::ShowData(WPARAM wParam,LPARAM lParam)
{
	CTestR1CBTRThread::Result Data = *(CTestR1CBTRThread::Result *)lParam;
	if (Data.m_bAddLiner) {
		int32_t iAvgPts = (int32_t)m_iKeepTime * 60 / 3 / 4;	//test once about every 3 sec,then take last 1/4 of all data
		double dAvgTemp = 0.0;
		
		m_pTempChart->GoToFirstSerie();
		m_pLinerChart->GoToFirstSerie();
		for (int32_t i = 0;i < MAX_RF;i ++) {
			for (int32_t j = 0;j < 8;j ++) {
				CChartLineSerie *pCurTempLineSerie = (CChartLineSerie *)(m_pTempChart->GetNextSerie());
				CChartLineSerie *pCurLinerLineSerie = (CChartLineSerie *)(m_pLinerChart->GetNextSerie());
				unsigned uiTotalPts = pCurTempLineSerie->GetPointsCount();
				dAvgTemp = 0.0;
				for (int32_t k = 0;k < iAvgPts;k ++)
					dAvgTemp += pCurTempLineSerie->GetYPointValue(uiTotalPts - k - 1);
				dAvgTemp /= iAvgPts;
				if (dAvgTemp > 0 && dAvgTemp < 100)
					pCurLinerLineSerie->AddPoint((double)Data.m_iCurSpeed,dAvgTemp);
			}
		}
	}
	else {
		bool bShow[MAX_RF] = {
			BST_CHECKED == ((CButton *)(GetDlgItem(IDC_CHECK1)))->GetCheck(),
			BST_CHECKED == ((CButton *)(GetDlgItem(IDC_CHECK2)))->GetCheck(),
			BST_CHECKED == ((CButton *)(GetDlgItem(IDC_CHECK5)))->GetCheck(),
			BST_CHECKED == ((CButton *)(GetDlgItem(IDC_CHECK6)))->GetCheck()
		};

		m_pTempChart->GoToFirstSerie();
		for (int32_t i = 0;i < MAX_RF;i ++) {
			for (int32_t j = 0;j < 8;j ++)
				((CChartLineSerie *)(m_pTempChart->GetNextSerie()))->AddPoint(Data.m_Time,Data.m_dTemp[i][j]);
		}
		m_pTempChart->GoToFirstSerie();
		for (int32_t i = 0;i < MAX_RF;i ++) {
			if (bShow[i]) {
				for (int32_t j = 0;j < 8;j ++) {
					CChartLineSerie *pCurrentLineSerie = (CChartLineSerie *)(m_pTempChart->GetNextSerie());
					TChartString strName = pCurrentLineSerie->GetName();
					strName.resize(6);
					TChartStringStream strStr;
					char szTemp[16] = {0};
					strStr << strName << " ";
					for (int32_t k = 0;k < MAX_RF;k ++) {
						sprintf(szTemp,"%.3f",(Data.m_dTemp[k][j] > 0.0 && Data.m_dTemp[k][j] < 100.0) ? Data.m_dTemp[k][j] : 0);
						if (!strcmp(szTemp,"0.000"))
							strcpy(szTemp,"--------");
						strStr << szTemp << "    ";
					}
					pCurrentLineSerie->SetName(strStr.str());
				}
				break;
			}
			else {
				for (int32_t j = 0;j < 8;j ++)
					m_pTempChart->GetNextSerie();
			}
		}
	}
	return 0;
}

void CTestR1CBTRDlg::OnBnClickedSetPath()
{
	CFileDialog File(TRUE);
	if (File.DoModal() == IDOK) {
		for (int32_t i = 0;i < MAX_RFU;i ++) {
			if (::IsWindow(ALL_DLG->m_pTestR1CBTRDlg[i].GetSafeHwnd()))
				((CEdit *)(ALL_DLG->m_pTestR1CBTRDlg[i].GetDlgItem(IDC_EDIT4)))->SetWindowText(File.GetPathName());
		}
	}
}

void CTestR1CBTRDlg::ResetChart()
{
	COLORREF clrTemp[8] = {BLACK,RED,GREEN,BLUE,BROWN,NAVY,PURPLE,GREY};
	TChartString strName[8] = {"Rx_LO2","Rx_LNA","Rx_LO1","Rx_PA1","Tx_LO2","Tx_LO1","Tx_PA4","Tx_PA3"};

	m_pTempChart->SetZoomEnabled(false);
	m_pTempChart->RemoveAllSeries();
	for (int32_t i = 0;i < MAX_RF;i ++) {
		for (int32_t j = 0;j < 8;j ++) {
			CChartLineSerie *pLineSerie = m_pTempChart->CreateLineSerie();
			pLineSerie->SetSeriesOrdering(poNoOrdering);
			pLineSerie->SetColor(clrTemp[j]);
			if (0 == i)
				pLineSerie->SetName(strName[j]);
			else
				pLineSerie->SetName("");
		}
	}

	m_pLinerChart->RemoveAllSeries();
	for (int32_t i = 0;i < MAX_RF;i ++) {
		for (int32_t j = 0;j < 8;j ++) {
			CChartLineSerie *pLineSerie = m_pLinerChart->CreateLineSerie();
			pLineSerie->SetSeriesOrdering(poNoOrdering);
			pLineSerie->SetColor(clrTemp[j]);
		}
	}
	CChartLineSerie *pLineSerie = m_pLinerChart->CreateLineSerie();
	pLineSerie->SetSeriesOrdering(poNoOrdering);
	pLineSerie->SetColor(ROYALBLUE4);
}

BOOL CTestR1CBTRDlg::OnInitDialog()
{
	CTestR1CBaseDlg::OnInitDialog();

	((CButton *)(GetDlgItem(IDC_CHECK1)))->SetCheck(BST_CHECKED);
	((CButton *)(GetDlgItem(IDC_CHECK2)))->SetCheck(BST_CHECKED);
	((CButton *)(GetDlgItem(IDC_CHECK5)))->SetCheck(BST_CHECKED);
	((CButton *)(GetDlgItem(IDC_CHECK6)))->SetCheck(BST_CHECKED);
	((CButton *)(GetDlgItem(IDC_CHECK3)))->SetCheck(BST_CHECKED);
	((CButton *)(GetDlgItem(IDC_CHECK4)))->SetCheck(BST_CHECKED);
	
	m_pTempChart = new CChartCtrl;
	m_pTempChart->Create(this,CRect(140,40,810,192),IDC_TEST_R1C_BTR_TEMP_CHART);
	m_pTempChart->GetLegend()->UndockLegend(50,20);
	m_pTempChart->GetLegend()->SetVisible(true);
	m_pTempChart->SetZoomRectColor(BLACK);
	m_pTempChart->GetTitle()->SetColor(BLUE);
	m_pTempChart->GetTitle()->AddString(_T("Temperature Curve [Chart1:(X:Time Y:Temp)]   [Chart2:(X:Blower Speed Y:Temp)]"));
	m_pTempChart->ShowWindow(SW_NORMAL);

	CChartDateTimeAxis *pDataTimeAxis = m_pTempChart->CreateDateTimeAxis(CChartCtrl::BottomAxis);
	pDataTimeAxis->SetDiscrete(false);
	pDataTimeAxis->SetTickIncrement(true,CChartDateTimeAxis::tiSecond,5);
	pDataTimeAxis->SetAutomatic(true);
	pDataTimeAxis->SetTickLabelFormat(false,_T("%H:%M:%S"));
	CChartStandardAxis *pTempAxis = m_pTempChart->CreateStandardAxis(CChartCtrl::LeftAxis);
	pTempAxis->SetAutomatic(false);
	pTempAxis->SetMinMax(30.0,80.0);
	pTempAxis->GetGrid()->SetVisible(false);

	m_pLinerChart = new CChartCtrl;
	m_pLinerChart->Create(this,CRect(140,192,810,384),IDC_TEST_R1C_BTR_LINER_CHART);
	m_pLinerChart->SetZoomRectColor(BLACK);

	CChartStandardAxis *pSpeedAxis = m_pLinerChart->CreateStandardAxis(CChartCtrl::BottomAxis);
	pSpeedAxis->SetAutomatic(false);
	pSpeedAxis->SetMinMax(50.0,255.0);
	pTempAxis = m_pLinerChart->CreateStandardAxis(CChartCtrl::LeftAxis);
	pTempAxis->SetAutomatic(false);
	pTempAxis->SetMinMax(30.0,80.0);

	this->GetWindowRect(&m_crWnd);
	SetControlInfo(IDC_STATIC_TEMP_FUNC,FixL_xStretch,2,0,1);
	SetControlInfo(IDC_EDIT4,ChgL_xStretch,2,1,2);
	SetControlInfo(IDC_BUTTON1,ChgL_Fixx,1,1,1);
	SetControlInfo(IDC_TEST_R1C_BTR_TEMP_CHART,ChgLT_xyStretch,1,0,1,3,0,2);
	SetControlInfo(IDC_TEST_R1C_BTR_LINER_CHART,ChgLT_xyStretch,1,0,1,3,2,3);

	AddThreadIdleCtrl(IDC_BUTTON2);
	AddThreadRunningCtrl(IDC_BUTTON3);
	CtrlResume();
	return TRUE;
}

void CTestR1CBTRDlg::OnBnClickedStart()
{
	STACHKV(ThreadCheckBox("准备测试风扇转速和硬件温度之间的关系"));
	TEST_THREAD_EXISTED_CHKV();
	UpdateData(TRUE);
	for (int32_t i = 0;i < MAX_RFU;i ++) {
		ALL_DLG->m_pTestR1CBTRDlg[i].CtrlPrepare();
		ALL_DLG->m_pTestR1CBTRDlg[i].ResetChart();
	}

	m_pThread = new CTestR1CBTRThread(this);
	m_pThread->CreateThread();
}

LRESULT CTestR1CBTRDlg::Done(WPARAM wParam,LPARAM lParam)
{
	for (int32_t i = 0;i < MAX_RFU;i ++) {
		if (::IsWindow(ALL_DLG->m_pTestR1CBTRDlg[i].GetSafeHwnd())) {
			ALL_DLG->m_pTestR1CBTRDlg[i].CtrlResume();
			ALL_DLG->m_pTestR1CBTRDlg[i].PolyFit();
			ALL_DLG->m_pTestR1CBTRDlg[i].m_pTempChart->SetZoomEnabled(true);
		}
	}
	return CTestR1CBaseDlg::Done(wParam,lParam);
}

void CTestR1CBTRDlg::ResetTempLineSerie()
{
	bool bShow[MAX_RF] = {
		BST_CHECKED == ((CButton *)(GetDlgItem(IDC_CHECK1)))->GetCheck(),
		BST_CHECKED == ((CButton *)(GetDlgItem(IDC_CHECK2)))->GetCheck(),
		BST_CHECKED == ((CButton *)(GetDlgItem(IDC_CHECK5)))->GetCheck(),
		BST_CHECKED == ((CButton *)(GetDlgItem(IDC_CHECK6)))->GetCheck()
	};
	TChartString strName[8] = {"Rx_LO2","Rx_LNA","Rx_LO1","Rx_PA1","Tx_LO2","Tx_LO1","Tx_PA4","Tx_PA3"};
	double dTemp[MAX_RF][8] = {0};
	unsigned uiPts = 0;

	if (m_pTempChart->GetSeriesCount()) {
		m_pTempChart->GoToFirstSerie();
		for (int32_t i = 0;i < MAX_RF;i ++) {
			for (int32_t j = 0;j < 8;j ++) {
				CChartLineSerie *pCurrentLineSerie = (CChartLineSerie *)(m_pTempChart->GetNextSerie());
				pCurrentLineSerie->SetVisible(bShow[i]);
				if ((uiPts = pCurrentLineSerie->GetPointsCount()) > 0)
					dTemp[i][j] = pCurrentLineSerie->GetYPointValue(uiPts - 1);
				if (pCurrentLineSerie->GetName() != "")
					pCurrentLineSerie->SetName(_T(""));
			}
		}
		m_pTempChart->GoToFirstSerie();
		for (int32_t i = 0;i < MAX_RF;i ++) {
			if (bShow[i]) {
				for (int32_t j = 0;j < 8;j ++) {
					TChartStringStream strStr;
					char szTemp[16] = {0};
					strStr << strName[j] << " ";
					for (int32_t k = 0;k < MAX_RF;k ++) {
						sprintf(szTemp,"%.3f",(dTemp[k][j] > 0.0 && dTemp[k][j] < 100.0) ? dTemp[k][j] : 0);
						if (!strcmp(szTemp,"0.000"))
							strcpy(szTemp,"--------");
						strStr << szTemp << "    ";
					}
					((CChartLineSerie *)(m_pTempChart->GetNextSerie()))->SetName(strStr.str());
				}
				break;
			}
			else {
				for (int32_t j = 0;j < 8;j ++)
					m_pTempChart->GetNextSerie();
			}
		}
	}
}

void CTestR1CBTRDlg::OnBnClickedShowRF0()
{
	ResetTempLineSerie();
}

void CTestR1CBTRDlg::OnBnClickedShowRF1()
{
	ResetTempLineSerie();
}

void CTestR1CBTRDlg::OnBnClickedShowRF2()
{
	ResetTempLineSerie();
}

void CTestR1CBTRDlg::OnBnClickedShowRF3()
{
	ResetTempLineSerie();
}

void CTestR1CBTRDlg::OnBnClickedShowFitData()
{
	if (m_pLinerChart->GetSeriesCount()) {
		bool bShowFit = (BST_CHECKED == ((CButton *)(GetDlgItem(IDC_CHECK3)))->GetCheck());

		m_pLinerChart->GoToFirstSerie();
		for (int32_t i = 0;i < MAX_RF;i ++) {
			for (int32_t j = 0;j < 8;j ++) {
				m_pLinerChart->GetNextSerie();
			}
		}
		((CChartLineSerie *)(m_pLinerChart->GetNextSerie()))->SetVisible(bShowFit);
	}
}

void CTestR1CBTRDlg::OnBnClickedShowOriginLinerData()
{
	if (m_pLinerChart->GetSeriesCount()) {
		bool bShowLiner = (BST_CHECKED == ((CButton *)(GetDlgItem(IDC_CHECK4)))->GetCheck());

		m_pLinerChart->GoToFirstSerie();
		for (int32_t i = 0;i < MAX_RF;i ++) {
			for (int32_t j = 0;j < 8;j ++) {
				((CChartLineSerie *)(m_pLinerChart->GetNextSerie()))->SetVisible(bShowLiner);
			}
		}
	}
}

void CTestR1CBTRDlg::PolyFit()
{
	if (!m_pLinerChart->GetSeriesCount())
		return;

	unsigned uiPts = 0;
	m_pLinerChart->GoToFirstSerie();
	for (int32_t i = 0;i < MAX_RF * 8;i ++){
		if ((uiPts = ((CChartLineSerie *)(m_pLinerChart->GetNextSerie()))->GetPointsCount()) > 0)
			break;
	}
	if (uiPts < 2)
		return;

	double dTemp = 0.0,dMaxTemp = 0.0;
	double dX[256] = {0.0},dY[256] = {0.0},dPhyY[256] = {0.0},dCoef[32] = {0.0},dResidual[32] = {0.0};
	int32_t iMaxSerieIdx = 0;

	m_pLinerChart->GoToFirstSerie();
	for (int32_t i = 0;i < MAX_RF;i ++) {
		for (int32_t j = 0;j < 8;j ++) {
			CChartLineSerie *pCurLineSerie = (CChartLineSerie *)(m_pLinerChart->GetNextSerie());
			if (pCurLineSerie->GetPointsCount()) {
				if ((dTemp = pCurLineSerie->GetYPointValue(uiPts - 1)) > dMaxTemp) {
					dMaxTemp = dTemp;
					iMaxSerieIdx = i * 8 + j;
				}
			}
		}
	}
	CChartLineSerie *pFitLineSerie = (CChartLineSerie *)(m_pLinerChart->GetNextSerie());
	m_pLinerChart->GoToFirstSerie();
	for (int32_t i = 0;i < iMaxSerieIdx;i ++)
		m_pLinerChart->GetNextSerie();
	CChartLineSerie *pMaxLineSerie = (CChartLineSerie *)(m_pLinerChart->GetNextSerie());

	memset(dX,0,sizeof(dX));
	memset(dY,0,sizeof(dY));
	memset(dPhyY,0,sizeof(dPhyY));
	memset(dCoef,0,sizeof(dCoef));
	memset(dResidual,0,sizeof(dResidual));
	for (unsigned i = 0;i < uiPts;i ++) {
		dX[i] = pMaxLineSerie->GetXPointValue(i);
		dY[i] = pMaxLineSerie->GetYPointValue(i);
	}
	for (int32_t n = 2;n <= 4;n ++) {
		dResidual[n] = 0;
		memset(dPhyY,0,sizeof(dPhyY));
		memset(dCoef,0,sizeof(dCoef));
		polyfit(dX,dY,(int32_t)uiPts,n,dCoef);
		for (unsigned i = 0;i < uiPts;i ++) {
			for (int32_t j = 0;j <= n;j ++)
				dPhyY[i] += dCoef[j] * pow(dX[i],j);
			dResidual[n] += (dPhyY[i] - dY[i]) * (dPhyY[i] - dY[i]);
		}
	}
	double dMinResidual = 10000000.0;
	int32_t iMinN = 2;
	for (int32_t n = 2;n <= 4;n ++) {
		if (dResidual[n] < dMinResidual) {
			dMinResidual = dResidual[n];
			iMinN = n;
		}
	}
	memset(dCoef,0,sizeof(dCoef));
	polyfit(dX,dY,(int32_t)uiPts,iMinN,dCoef);
	for (double dSpeed = 50.0;dSpeed <= 255;dSpeed ++) {
		dTemp = 0.0;
		for (int32_t i = 0;i <= iMinN;i ++)
			dTemp += dCoef[i] * pow(dSpeed,i);
		pFitLineSerie->AddPoint(dSpeed,dTemp);
	}
	char szFunc[256] = {0};
	memset(szFunc,0,sizeof(szFunc));
	coef2polynomial(dCoef,iMinN,szFunc);
	for (int32_t i = 0;i < MAX_RFU;i ++) {
		if (::IsWindow(ALL_DLG->m_pTestR1CBTRDlg[i].GetSafeHwnd()))
			((CStatic *)(ALL_DLG->m_pTestR1CBTRDlg[i].GetDlgItem(IDC_STATIC_TEMP_FUNC)))->SetWindowText(szFunc);
	}
}

void CTestR1CBTRDlg::OnEnChangeEnvTemp()
{
	UpdateData(TRUE);
	for (int32_t i = 0;i < MAX_RFU;i ++) {
		ALL_DLG->m_pTestR1CBTRDlg[i].m_dEnvTemp = this->m_dEnvTemp;
		::PostMessage(ALL_DLG->m_pTestR1CBTRDlg[i].GetSafeHwnd(),WM_CSE_UPDATE_DATA_FALSE,0,0);
	}
}

void CTestR1CBTRDlg::OnEnChangeSpeedStep()
{
	UpdateData(TRUE);
	for (int32_t i = 0;i < MAX_RFU;i ++) {
		ALL_DLG->m_pTestR1CBTRDlg[i].m_iSpeedStep = this->m_iSpeedStep;
		::PostMessage(ALL_DLG->m_pTestR1CBTRDlg[i].GetSafeHwnd(),WM_CSE_UPDATE_DATA_FALSE,0,0);
	}
}

void CTestR1CBTRDlg::OnEnChangeKeepTime()
{
	UpdateData(TRUE);
	for (int32_t i = 0;i < MAX_RFU;i ++) {
		ALL_DLG->m_pTestR1CBTRDlg[i].m_iKeepTime = this->m_iKeepTime;
		::PostMessage(ALL_DLG->m_pTestR1CBTRDlg[i].GetSafeHwnd(),WM_CSE_UPDATE_DATA_FALSE,0,0);
	}
}
