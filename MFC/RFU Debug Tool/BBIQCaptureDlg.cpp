#include "stdafx.h"
#include "RFU_Debug_Tool.h"
#include "BBIQCaptureDlg.h"
#include "IQCaptureThread.h"
#include "WaveVisionDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CBBIQCaptureDlg, CStretchDlg)
IMPLEMENT_CSE_STRETCH_DLG(CBBIQCaptureDlg)

CBBIQCaptureDlg::CBBIQCaptureDlg(CWnd* pParent /*=NULL*/)
	: CStretchDlg(CBBIQCaptureDlg::IDD, pParent)
	, m_uiTriggerOffset(0)
	, m_uiCapOffset(0)
	, m_uiTriggerTimeout(6553600)
	, m_uiCapSamples(40960)
	, m_uiTriggerGap(1)
	, m_dThreshold(0.0)
	, m_bShowI(TRUE)
	, m_bShowQ(TRUE)
	, m_bShowingDFT(false)
	, m_strStateMachine(_T("idle"))
	, m_dPower(0.0)
	, m_dPeakPower(0.0)
{
	m_Chart = NULL;
}

CBBIQCaptureDlg::~CBBIQCaptureDlg()
{
	if (m_Chart) {
		delete m_Chart;
		m_Chart = NULL;
	}
}

void CBBIQCaptureDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_TriggerSrc);
	DDX_Control(pDX, IDC_COMBO2, m_TriggerEdge);
	DDX_Text(pDX, IDC_EDIT1, m_uiTriggerOffset);
	DDX_Text(pDX, IDC_EDIT2, m_uiCapOffset);
	DDX_Text(pDX, IDC_EDIT3, m_uiTriggerTimeout);
	DDX_Text(pDX, IDC_EDIT4, m_uiCapSamples);
	DDX_Text(pDX, IDC_EDIT6, m_uiTriggerGap);
	DDX_Text(pDX, IDC_EDIT7, m_dThreshold);
	DDX_Control(pDX, IDC_EDIT5, m_FilePath);
	DDX_Check(pDX, IDC_CHECK3, m_bShowI);
	DDX_Check(pDX, IDC_CHECK4, m_bShowQ);
	DDX_Text(pDX, IDC_EDIT9, m_strStateMachine);
	DDX_Text(pDX, IDC_EDIT8, m_dPower);
	DDX_Text(pDX, IDC_EDIT10, m_dPeakPower);
}


BEGIN_MESSAGE_MAP(CBBIQCaptureDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON3, &CBBIQCaptureDlg::OnBnClickedStartCapture)
	ON_BN_CLICKED(IDC_BUTTON5, &CBBIQCaptureDlg::OnBnClickedAbortCapture)
	ON_BN_CLICKED(IDC_BUTTON9, &CBBIQCaptureDlg::OnBnClickedShowDataTimeDomain)
	ON_BN_CLICKED(IDC_BUTTON4, &CBBIQCaptureDlg::OnBnClickedSetDataPath)
	ON_BN_CLICKED(IDC_BUTTON1, &CBBIQCaptureDlg::OnBnClickedOpenWaveVisionDlg)
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_BUTTON2, &CBBIQCaptureDlg::OnBnClickedShowDataDFT)
	ON_BN_CLICKED(IDC_CHECK3, &CBBIQCaptureDlg::OnBnClickedShowDataI)
	ON_BN_CLICKED(IDC_CHECK4, &CBBIQCaptureDlg::OnBnClickedShowDataQ)
	ON_BN_CLICKED(IDC_BUTTON23, &CBBIQCaptureDlg::OnBnClickedStartPowerMeasure)
	ON_MESSAGE(WM_CSE_UPDATE_DATA_TRUE, &CBBIQCaptureDlg::UpdateDataTrue)
	ON_MESSAGE(WM_CSE_ALL_IQ_CAP_SHOW, &CBBIQCaptureDlg::ShowDataContinuousIQCap)
	ON_BN_CLICKED(IDC_CHECK2, &CBBIQCaptureDlg::OnBnClickedSetMaxHold)
	ON_BN_CLICKED(IDC_CHECK6, &CBBIQCaptureDlg::OnBnClickedSetMinHold)
	ON_EN_CHANGE(IDC_EDIT4, &CBBIQCaptureDlg::OnEnChangeSamples)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CBBIQCaptureDlg::OnCbnSelchangeTriggerSource)
	ON_CBN_SELCHANGE(IDC_COMBO2, &CBBIQCaptureDlg::OnCbnSelchangeTriggerEdge)
	ON_EN_CHANGE(IDC_EDIT1, &CBBIQCaptureDlg::OnEnChangeTriggerOffset)
	ON_EN_CHANGE(IDC_EDIT2, &CBBIQCaptureDlg::OnEnChangeCaptureOffset)
	ON_EN_CHANGE(IDC_EDIT3, &CBBIQCaptureDlg::OnEnChangeTimeout)
	ON_EN_CHANGE(IDC_EDIT6, &CBBIQCaptureDlg::OnEnChangeTriggerGap)
	ON_EN_CHANGE(IDC_EDIT7, &CBBIQCaptureDlg::OnEnChangeThreshold)
END_MESSAGE_MAP()


BOOL CBBIQCaptureDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_TriggerSrc.SetCurSel(0);
	m_TriggerEdge.SetCurSel(0);
	GetDlgItem(IDC_CHECK2)->EnableWindow(FALSE);
	GetDlgItem(IDC_CHECK6)->EnableWindow(FALSE);

	CChartAxis *pAxis = NULL;
	m_Chart = new CChartCtrl;
	m_Chart->Create(this,IQ_CAPTURE_CHART_RECT,IDC_BB_CAP_CHART);
	pAxis = m_Chart->CreateStandardAxis(CChartCtrl::BottomAxis);
	pAxis->SetAutomatic(false);
	pAxis->SetMinMax(0,40960);
	pAxis->SetTextColor(WHITE);
	pAxis = m_Chart->CreateStandardAxis(CChartCtrl::LeftAxis);
	pAxis->SetAutomatic(false);
	pAxis->SetTextColor(WHITE);
	pAxis->SetMinMax(IQ_MIN,IQ_MAX);

	m_Chart->SetBackColor(SA_BCKGRD);
	m_pILineSerie = m_Chart->CreateLineSerie();
	m_pILineSerie->SetSeriesOrdering(poNoOrdering);
	m_pQLineSerie = m_Chart->CreateLineSerie();
	m_pQLineSerie->SetSeriesOrdering(poNoOrdering);
	m_pDFTLineSerie = m_Chart->CreateLineSerie();
	m_pDFTLineSerie->SetSeriesOrdering(poNoOrdering);
	m_pDFTLineSerie->SetColor(YELLOW);
	m_pDFTLineSerie->SetSeriesHolding(phNoHolding);

	m_FilePath.SetWindowText("c:\\iq.txt");

	this->GetWindowRect(&m_crWnd);
	SetControlInfo(IDC_BB_CAP_CHART,FixL_xyStretch,1,0,1);
	return TRUE;
}

void CBBIQCaptureDlg::OnBnClickedStartCapture()
{
	STACHKV(m_pSP1401->SetIQCapSamples((uint32_t)m_uiCapSamples));
	STACHKV(m_pSP1401->IQCap());

	CString strFilePath;
	m_FilePath.GetWindowText(strFilePath);
	if (strFilePath != "") {
		STACHKV(DDR->IQToFile(m_pSP1401->GetRfIdx(),(LPSTR)(LPCTSTR)strFilePath,uint32_t(m_uiCapSamples),"c:\\iq_interch.txt"));
	}
}

void CBBIQCaptureDlg::OnBnClickedAbortCapture()
{
	m_pSP1401->IQCapAbort();
}

void CBBIQCaptureDlg::OnBnClickedShowDataTimeDomain()
{
	if (m_bShowingDFT) {
		m_pDFTLineSerie->SetVisible(false);
		GetDlgItem(IDC_CHECK2)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK6)->EnableWindow(FALSE);
		m_pILineSerie->SetVisible(m_bShowI);
		m_pQLineSerie->SetVisible(m_bShowQ);
		GetDlgItem(IDC_CHECK3)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK4)->EnableWindow(TRUE);
		m_Chart->GetLeftAxis()->SetMinMax(IQ_MIN,IQ_MAX);
		m_bShowingDFT = false;
		m_Chart->GetBottomAxis()->SetMinMax(0,m_uiCapSamples);
	}
	UpdateData(TRUE);
	TimeDomain(m_pSP1401,(uint32_t)m_uiCapSamples,m_pILineSerie,m_pQLineSerie);
}

void CBBIQCaptureDlg::OnBnClickedSetDataPath()
{
	CFileDialog DataFile(TRUE);
	if (DataFile.DoModal() == IDOK)
		m_FilePath.SetWindowText(DataFile.GetPathName());
}

void CBBIQCaptureDlg::OnBnClickedOpenWaveVisionDlg()
{
	UpdateData(TRUE);
	CString strFilePath;
	m_FilePath.GetWindowText(strFilePath);

	CWaveVisionDlg *pWVDlg = new CWaveVisionDlg;
	pWVDlg->Create(CWaveVisionDlg::IDD,this,strFilePath,m_uiCapSamples);
	pWVDlg->ShowWindow(SW_NORMAL);
}

void CBBIQCaptureDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CStretchDlg::OnShowWindow(bShow, nStatus);
	if (bShow)
		m_pSP2401->SetADSw(CSP2401R1A::FromBb);
}

void CBBIQCaptureDlg::OnBnClickedShowDataDFT()
{
	if (!m_bShowingDFT) {
		m_pDFTLineSerie->SetVisible(true);
		GetDlgItem(IDC_CHECK2)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK6)->EnableWindow(TRUE);
		m_pILineSerie->SetVisible(false);
		m_pQLineSerie->SetVisible(false);
		GetDlgItem(IDC_CHECK3)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK4)->EnableWindow(FALSE);
		m_Chart->GetLeftAxis()->SetMinMax(POWER_MIN,POWER_MAX);
		m_bShowingDFT = true;
		m_Chart->GetBottomAxis()->SetMinMax(-122.88,122.88);
	}
	UpdateData(TRUE);
	DFT(m_pSP1401,245760000,(uint32_t)m_uiCapSamples,m_pDFTLineSerie);
}

LRESULT CBBIQCaptureDlg::ShowDataContinuousIQCap(WPARAM wParam,LPARAM lParam)
{
	CIQCaptureThread::Result Data = *(CIQCaptureThread::Result *)lParam;
	if (m_bShowingDFT)
		m_pDFTLineSerie->SetPoints(Data.m_pXDFT,Data.m_pdB,Data.m_iPts);
	else {
		m_pILineSerie->SetPoints(Data.m_pXTD,Data.m_pI,Data.m_iPts);
		m_pQLineSerie->SetPoints(Data.m_pXTD,Data.m_pQ,Data.m_iPts);
	}
	return 0;
}

void CBBIQCaptureDlg::OnBnClickedShowDataI()
{
	UpdateData(TRUE);
	m_pILineSerie->SetVisible(m_bShowI);
}

void CBBIQCaptureDlg::OnBnClickedShowDataQ()
{
	UpdateData(TRUE);
	m_pQLineSerie->SetVisible(m_bShowQ);
}

void CBBIQCaptureDlg::OnBnClickedStartPowerMeasure()
{
	UpdateData(TRUE);
	CString strStateMachine[5] = {"idle","waiting","timeout","running","done"};
	uint32_t samples = (uint32_t)m_uiCapSamples;
	CSP1401R1A::PowerMeasSrcs source = CSP1401R1A::PowerMeasFreeRun;
	switch (m_TriggerSrc.GetCurSel()) {
		case 0  : {source = CSP1401R1A::PowerMeasFreeRun;break;}
		case 12 : {source = CSP1401R1A::PowerMeasIFPower;break;}
		default:break;
	}
	CSP1401R1A::PowerMeasState state = CSP1401R1A::IDLE;
	bool edge = (m_TriggerEdge.GetCurSel() == 0 ? false : true);

// 	PowMeasInfo info;
// 	info.RF = m_pSP1401;
// 	info.m_PMSrc = source;
// 	CPowMeasThread *pThread = new CPowMeasThread(this,&info);
// 	pThread->CreateThread();

	STACHKV(m_pSP2401->SetADSw(CSP2401R1A::FromBb));
	STACHKV(m_pSP1401->SetPowerMeasSrc(source,edge));
	STACHKV(m_pSP1401->SetPowerMeasTimeout((uint32_t)m_uiTriggerTimeout));
	STACHKV(m_pSP1401->SetPowerMeasSamples((uint32_t)m_uiCapSamples));
	STACHKV(m_pSP1401->SetPowerMeasTrigThreshold(m_dThreshold));
	STACHKV(m_pSP1401->PowerMeasAbort());
	STACHKV(m_pSP1401->PowerMeasStart());

	while (state != CSP1401R1A::DONE) {
		STACHKV(m_pSP1401->GetPowerMeasState(state));
		m_strStateMachine = strStateMachine[(int)state];
		UpdateData(FALSE);
		if (state == CSP1401R1A::TT)
			break;
	}
	STACHKV(m_pSP1401->GetPowerMeasPower(m_dPower));
	STACHKV(m_pSP1401->GetPowerMeasPeak(m_dPeakPower));
	UpdateData(FALSE);
}

void CBBIQCaptureDlg::OnBnClickedSetMaxHold()
{
	if (BST_CHECKED == ((CButton *)(GetDlgItem(IDC_CHECK2)))->GetCheck()) {
		((CButton *)(GetDlgItem(IDC_CHECK6)))->SetCheck(BST_UNCHECKED);
		m_pDFTLineSerie->SetSeriesHolding(phMaxHolding);
	}
	else
		m_pDFTLineSerie->SetSeriesHolding(phNoHolding);
}

void CBBIQCaptureDlg::OnBnClickedSetMinHold()
{
	if (BST_CHECKED == ((CButton *)(GetDlgItem(IDC_CHECK6)))->GetCheck()) {
		((CButton *)(GetDlgItem(IDC_CHECK2)))->SetCheck(BST_UNCHECKED);
		m_pDFTLineSerie->SetSeriesHolding(phMinHolding);
	}
	else
		m_pDFTLineSerie->SetSeriesHolding(phNoHolding);
}

void CBBIQCaptureDlg::OnEnChangeSamples()
{
	UpdateData(TRUE);
	STACHKV(m_pSP1401->SetIQCapSamples((uint32_t)m_uiCapSamples));
	m_pDFTLineSerie->ClearSerie();
	m_pILineSerie->ClearSerie();
	m_pQLineSerie->ClearSerie();
	if (m_bShowingDFT)
		m_Chart->GetBottomAxis()->SetMinMax(-122.88,122.88);
	else
		m_Chart->GetBottomAxis()->SetMinMax(0,(double)(m_uiCapSamples));
}

void CBBIQCaptureDlg::OnCbnSelchangeTriggerSource()
{
	UpdateData(TRUE);
	//	CSP1401R1A::IQCaptureSrcs Src = (CSP1401R1A::IQCaptureSrcs)(m_TriggerSrc.GetCurSel());
	ISP1401::PowerMeasSrcs Src = (ISP1401::PowerMeasSrcs)(m_TriggerSrc.GetCurSel());
	bool bEdge = (0 == m_TriggerEdge.GetCurSel() ? false : true);
	STACHKV(m_pSP1401->SetPowerMeasSrc(Src,bEdge));
}

void CBBIQCaptureDlg::OnCbnSelchangeTriggerEdge()
{
	OnCbnSelchangeTriggerSource();
}

void CBBIQCaptureDlg::OnEnChangeTriggerOffset()
{
}

void CBBIQCaptureDlg::OnEnChangeCaptureOffset()
{
}

void CBBIQCaptureDlg::OnEnChangeTimeout()
{
	UpdateData(TRUE);
	STACHKV(m_pSP1401->SetPowerMeasTimeout(uint32_t(m_uiTriggerTimeout)));
}

void CBBIQCaptureDlg::OnEnChangeTriggerGap()
{
}

void CBBIQCaptureDlg::OnEnChangeThreshold()
{
	UpdateData(TRUE);
	STACHKV(m_pSP1401->SetPowerMeasTrigThreshold(m_dThreshold));
}