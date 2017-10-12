#include "stdafx.h"
#include "RFU_Debug_Tool.h"
#include "CalR1CRxRefDlg.h"
#include "CalR1CRxRefThread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CCalR1CRxRefDlg, CCalR1CBaseDlg)
IMPLEMENT_CSE_CAL_R1C_DLG(CCalR1CRxRefDlg)

CCalR1CRxRefDlg::CCalR1CRxRefDlg(CWnd* pParent /*=NULL*/)
	: CCalR1CBaseDlg(CCalR1CRxRefDlg::IDD, _T("2G"), pParent)
	, m_strFreqStar(_T(""))
	, m_strFreqStop(_T(""))
	, m_strFreqStep(_T(""))
	, m_dRef(0.0)
	, m_dPower(0)
	, m_strLNAAtt(_T(""))
	, m_fAtt1(0.0)
	, m_fAtt2(0.0)
	, m_fAtt3(0.0)
{
	m_DataOP.clear();
	m_DataIO.clear();
}

CCalR1CRxRefDlg::~CCalR1CRxRefDlg()
{
	for (int32_t i = 0;i < ARRAY_SIZE(m_OPLineSerie);i ++) {
		for (int32_t j = 0;j < m_OPLineSerie[i].size();j ++)
			m_OPLineSerie[i].at(j)->ClearSerie();
		m_OPLineSerie[i].clear();
	}
	for (int32_t i = 0;i < ARRAY_SIZE(m_IOLineSerie);i ++) {
		for (int32_t j = 0;j < m_IOLineSerie[i].size();j ++)
			m_IOLineSerie[i].at(j)->ClearSerie();
		m_IOLineSerie[i].clear();
	}
	if (m_pChart) {
		delete m_pChart;
		m_pChart = NULL;
	}
}

void CCalR1CRxRefDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strFreqStar);
	DDX_Text(pDX, IDC_EDIT2, m_strFreqStop);
	DDX_Text(pDX, IDC_EDIT3, m_strFreqStep);
	DDX_Text(pDX, IDC_EDIT6, m_strFreq);
	DDX_Text(pDX, IDC_EDIT8, m_dRef);
	DDX_Text(pDX, IDC_EDIT9, m_dPower);
	DDX_Text(pDX, IDC_EDIT10, m_fAtt1);
	DDX_Text(pDX, IDC_EDIT21, m_fAtt2);
	DDX_Text(pDX, IDC_EDIT22, m_fAtt3);
	DDX_Text(pDX, IDC_EDIT23, m_strLNAAtt);
	DDX_Control(pDX, IDC_COMBO1, m_IOMode);
	DDX_Control(pDX, IDC_LIST1, m_ListOP);
	DDX_Control(pDX, IDC_LIST2, m_ListIO);
	DDX_Control(pDX, IDC_COMBO62, m_CalIOMode);
}


BEGIN_MESSAGE_MAP(CCalR1CRxRefDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON2, &CCalR1CRxRefDlg::OnBnClickedStartCal)
	ON_BN_CLICKED(IDC_BUTTON3, &CCalR1CRxRefDlg::OnBnClickedStop)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON6, &CCalR1CRxRefDlg::OnBnClickedGetAndSet)
	ON_MESSAGE(WM_CSE_SINGLE_THREAD_DONE, &CCalR1CRxRefDlg::Done)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST1, &CCalR1CRxRefDlg::OnLvnGetdispinfoList1RxCalData)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST2, &CCalR1CRxRefDlg::OnLvnGetdispinfoList2RxCalData)
	ON_MESSAGE(WM_CSE_CAL_RX_REF_SHOW, &CCalR1CRxRefDlg::ShowData)
	ON_BN_CLICKED(IDC_BUTTON4, &CCalR1CRxRefDlg::OnBnClickedExportCalFile)
	ON_MESSAGE(WM_CSE_SP3301_CHANGE, &CCalR1CRxRefDlg::SP3301Change)
	ON_MESSAGE(WM_CSE_UPDATE_DATA_FALSE, &CCalR1CRxRefDlg::UpdateDataFalse)
	ON_BN_CLICKED(IDC_BUTTON1, &CCalR1CRxRefDlg::OnBnClickedPause)
	ON_BN_CLICKED(IDC_BUTTON5, &CCalR1CRxRefDlg::OnBnClickedContinue)
END_MESSAGE_MAP()


void CCalR1CRxRefDlg::ResetShowCtrl()
{
	int64_t iFreqStar = 0,iFreqStop = 0;
	int32_t iPts = freq2array(m_strFreqStar,m_strFreqStop,m_strFreqStep,NULL,NULL,NULL,&iFreqStop,&iFreqStar);
	m_pChart->GetBottomAxis()->SetMinMax(iFreqStar / 1000000.0,iFreqStop / 1000000.0);

	int32_t iSec = strsections((LPSTR)(LPCTSTR)m_strFreqStar);
	CalIOMode Mode = (CalIOMode)(m_CalIOMode.GetCurSel());
	
	for (int32_t i = 0;i < ARRAY_SIZE(m_OPLineSerie);i ++) {
		for (int32_t j = 0;j < m_OPLineSerie[i].size();j ++)
			m_OPLineSerie[i].at(j)->ClearSerie();
		m_OPLineSerie[i].clear();
	}
	for (int32_t i = 0;i < ARRAY_SIZE(m_IOLineSerie);i ++) {
		for (int32_t j = 0;j < m_IOLineSerie[i].size();j ++)
			m_IOLineSerie[i].at(j)->ClearSerie();
		m_IOLineSerie[i].clear();
	}

	m_pChart->RemoveAllSeries();

	if (CalOP(Mode)) {
		TChartString strRefName[3] = {"10dBm(Att19)","10dBm(Att0)","-20dBm(LNA)"};
		COLORREF clrRef[3] = {RED,GREEN,BLUE};
		for (int32_t i = 0;i < ARRAY_SIZE(m_OPLineSerie);i ++) {
			for (int32_t j = 0;j < iSec;j ++) {
				CChartLineSerie *pLineSerie = m_pChart->CreateLineSerie();
				pLineSerie->SetSeriesOrdering(poNoOrdering);
				pLineSerie->SetColor(clrRef[i]);
				pLineSerie->SetWidth(3);
				if (0 == j) {
					TChartString strName;
					TChartStringStream strStr;
					strStr << "OP@" << strRefName[i];
					strName = strStr.str();
					pLineSerie->SetName(strName);
				}
				m_OPLineSerie[i].push_back(pLineSerie);
			}
		}

		RxRefOPTableR1C::DataF Data;
		m_DataOP.clear();
		for (int32_t i = 0;i < iPts;i ++)
			m_DataOP.push_back(Data);

		m_ListOP.DeleteAllItems();
		while (m_ListOP.DeleteColumn(0));
		m_ListOP.InsertColumn(0,_T("Freq(MHz)"),LVCFMT_CENTER,70);
		m_ListOP.InsertColumn(1,_T("Reference"),LVCFMT_RIGHT,70);
		m_ListOP.InsertColumn(2,_T("LNA/Att"),LVCFMT_CENTER,60);
		m_ListOP.InsertColumn(3,_T("Att0/19"),LVCFMT_CENTER,60);
		m_ListOP.InsertColumn(4,_T("Att 1"),LVCFMT_RIGHT,40);
		m_ListOP.InsertColumn(5,_T("Att 2"),LVCFMT_RIGHT,40);
		m_ListOP.InsertColumn(6,_T("Att 3"),LVCFMT_RIGHT,40);
		m_ListOP.InsertColumn(7,_T("Offset"),LVCFMT_RIGHT,100);
		m_ListOP.InsertColumn(8,_T("Temp0_RxLO2"),LVCFMT_CENTER,90);
		m_ListOP.InsertColumn(9,_T("Temp1_RxLNA"),LVCFMT_CENTER,90);
		m_ListOP.InsertColumn(10,_T("Temp2_RxLO1"),LVCFMT_CENTER,90);
		m_ListOP.InsertColumn(11,_T("Temp3_RxPA1"),LVCFMT_CENTER,90);
		m_ListOP.InsertColumn(12,_T("End Time"),LVCFMT_RIGHT,120);

		DWORD dwStyleList = ::GetWindowLong(m_ListOP.m_hWnd,GWL_STYLE);
		dwStyleList |= LVS_SINGLESEL;
		::SetWindowLong(m_ListOP.m_hWnd,GWL_STYLE,dwStyleList);

		m_ListOP.SetItemCount(iPts * ARRAY_SIZE(Data.m_State));
		m_ListOP.SetRedraw();
	}

	if (CalIO(Mode)) {
		TChartString strRefName[3] = {"10dBm(Att19)","10dBm(Att0)","-20dBm(LNA)"};
		COLORREF clrRef[3] = {BROWN,NAVY,PURPLE};
		for (int32_t i = 0;i < ARRAY_SIZE(m_IOLineSerie);i ++) {
			for (int32_t j = 0;j < iSec;j ++) {
				CChartLineSerie *pLineSerie = m_pChart->CreateLineSerie();
				pLineSerie->SetSeriesOrdering(poNoOrdering);
				pLineSerie->SetColor(clrRef[i]);
				pLineSerie->SetWidth(3);
				if (0 == j) {
					TChartString strName;
					TChartStringStream strStr;
					strStr << "IO@" << strRefName[i];
					strName = strStr.str();
					pLineSerie->SetName(strName);
				}
				m_IOLineSerie[i].push_back(pLineSerie);
			}
		}

		RxRefIOTableR1C::DataF Data;
		m_DataIO.clear();
		for (int32_t i = 0;i < iPts;i ++)
			m_DataIO.push_back(Data);

		m_ListIO.DeleteAllItems();
		while (m_ListIO.DeleteColumn(0));
		m_ListIO.InsertColumn(0,_T("Freq(MHz)"),LVCFMT_CENTER,70);
		m_ListIO.InsertColumn(1,_T("Reference"),LVCFMT_RIGHT,70);
		m_ListIO.InsertColumn(2,_T("LNA/Att"),LVCFMT_CENTER,60);
		m_ListIO.InsertColumn(3,_T("Att0/19"),LVCFMT_RIGHT,60);
		m_ListIO.InsertColumn(4,_T("Att 1"),LVCFMT_RIGHT,40);
		m_ListIO.InsertColumn(5,_T("Att 2"),LVCFMT_RIGHT,40);
		m_ListIO.InsertColumn(6,_T("Att 3"),LVCFMT_RIGHT,40);
		m_ListIO.InsertColumn(7,_T("Offset"),LVCFMT_RIGHT,100);
		m_ListIO.InsertColumn(8,_T("Temp0_RxLO2"),LVCFMT_CENTER,90);
		m_ListIO.InsertColumn(9,_T("Temp1_RxLNA"),LVCFMT_CENTER,90);
		m_ListIO.InsertColumn(10,_T("Temp2_RxLO1"),LVCFMT_CENTER,90);
		m_ListIO.InsertColumn(11,_T("Temp3_RxPA1"),LVCFMT_CENTER,90);
		m_ListIO.InsertColumn(12,_T("End Time"),LVCFMT_RIGHT,120);

		DWORD dwStyleList = ::GetWindowLong(m_ListIO.m_hWnd,GWL_STYLE);
		dwStyleList |= LVS_SINGLESEL;
		::SetWindowLong(m_ListIO.m_hWnd,GWL_STYLE,dwStyleList);

		m_ListIO.SetItemCount(iPts * ARRAY_SIZE(Data.m_State));
		m_ListIO.SetRedraw();
	}
}

void CCalR1CRxRefDlg::OnBnClickedStartCal()
{
	CAL_THREAD_EXISTED_CHKV();
	UpdateData(TRUE);
	CtrlPrepare();
	ResetShowCtrl();

	m_pThread = new CCalR1CRxRefThread(this);
	m_pThread->CreateThread();
}

BOOL CCalR1CRxRefDlg::OnInitDialog()
{
	CStretchDlg::OnInitDialog();

	m_CalIOMode.SetCurSel(2);
	m_IOMode.SetCurSel(1);

	DWORD dwStyleList = m_ListOP.GetExtendedStyle();
	dwStyleList |= LVS_EX_GRIDLINES;
	dwStyleList |= LVS_EX_FULLROWSELECT;
	dwStyleList &= ~LVS_EX_AUTOSIZECOLUMNS;
	m_ListOP.SetExtendedStyle(dwStyleList);
	m_ListOP.SetBkColor(PROTECT_EYE);
	m_ListOP.SetTextBkColor(PROTECT_EYE);
	m_ListIO.SetExtendedStyle(dwStyleList);
	m_ListIO.SetBkColor(PROTECT_EYE);
	m_ListIO.SetTextBkColor(PROTECT_EYE);

	m_pChart = new CChartCtrl;
	CRect crChart = CAL_CHART_RECT;
	CRect crListOP;
	m_ListOP.GetWindowRect(&crListOP);
	ScreenToClient(&crListOP);
	crChart.top = crListOP.bottom;
	m_pChart->Create(this,crChart,IDC_CAL_R1C_RX_REF_CHART);
	m_pChart->GetLegend()->SetHorizontalMode(true);
	m_pChart->GetLegend()->UndockLegend(20,40);
	m_pChart->GetLegend()->SetVisible(true);
	m_pChart->SetZoomRectColor(BLACK);
	m_pChart->GetTitle()->AddString(_T("Reference's Digital Gain Curve(X:Freq(MHz) Y:Digital Gain(dB))"));
	m_pChart->GetTitle()->SetColor(BLUE);

	CChartAxis *pAxis;
	pAxis = m_pChart->CreateStandardAxis(CChartCtrl::BottomAxis);
	pAxis->SetAutomatic(false);
	pAxis->SetMinMax(RF_RX_FREQ_STAR / 1000000,RF_RX_FREQ_STOP / 1000000);
	pAxis = m_pChart->CreateStandardAxis(CChartCtrl::LeftAxis);
	pAxis->SetAutomatic(false);
	pAxis->SetMinMax(-1.0,2.2);

	m_strFreqStar.Format("%dM",RF_RX_FREQ_STAR / 1000000);
	m_strFreqStop.Format("%dM",RF_RX_FREQ_STOP / 1000000);
	m_strFreqStep.Format("%dM",RF_RX_FREQ_STEP_CALLED_R1C / 1000000);

	CString strPath;
	strPath.Format("c:\\CSECal\\rfu%drf%d.cal",m_pSP1401->GetRfuIdx(),m_pSP1401->GetRfIdx());
	((CEdit *)(GetDlgItem(IDC_EDIT4)))->SetWindowText(strPath);
	UpdateData(FALSE);

	this->GetWindowRect(&m_crWnd);
	SetControlInfo(IDC_EDIT1,FixL_xStretch,1,0,1);
	SetControlInfo(IDC_EDIT2,FixL_xStretch,1,0,1);
	SetControlInfo(IDC_EDIT4,ChgL_Fixx,1,1,1);
	SetControlInfo(IDC_BUTTON4,ChgL_Fixx,1,1,1);
	SetControlInfo(IDC_LIST1,FixL_xyStretch,2,0,1);
	SetControlInfo(IDC_LIST2,ChgL_xyStretch,2,1,2);
	SetControlInfo(IDC_CAL_R1C_RX_REF_CHART,ChgLT_xyStretch,1,0,1,1,1,1);
	AddThreadIdleCtrl(IDC_EDIT1);
	AddThreadIdleCtrl(IDC_EDIT2);
	AddThreadIdleCtrl(IDC_EDIT3);
	AddThreadIdleCtrl(IDC_BUTTON2);
	AddThreadIdleCtrl(IDC_EDIT6);
	AddThreadIdleCtrl(IDC_EDIT8);
	AddThreadIdleCtrl(IDC_COMBO1);
	AddThreadIdleCtrl(IDC_BUTTON6);
	AddThreadIdleCtrl(IDC_BUTTON4);
	AddThreadIdleCtrl(IDC_COMBO62);
	AddThreadRunningCtrl(IDC_BUTTON3);
	AddThreadToPauseCtrl(IDC_BUTTON1);
	AddThreadToRunningCtrl(IDC_BUTTON5);
	CtrlResume();
	return TRUE;
}

void CCalR1CRxRefDlg::OnBnClickedGetAndSet()
{
	UpdateData(TRUE);
	uint64_t uiFreq = 0;
	int32_t iOffset = 0;
	IOMode Mode = ((m_IOMode.GetCurSel()) == 0 ? SP1401::IO : SP1401::OutPut);
	BW bw = m_pSP1401->GetBw();
	RxRefOPTableR1C::RxStateM RxState;

	str2freq(m_strFreq,uiFreq);

	if (OutPut == Mode) {
		((CalFileR1C *)(m_pSP1401->GetCalFile()))->m_pRxRefOP->Get(uiFreq,m_dRef,&RxState);
		iOffset = ((CalFileR1C *)(m_pSP1401->GetCalFile()))->m_pRxAttOP->Get(uiFreq,m_dRef);
		iOffset += RxState.m_iADOffset;

		if (_80M == bw) {
			RxFilterOffTableR1C::DataM DataFilOff;
			((CalFileR1C *)(m_pSP1401->GetCalFile()))->m_pRxFilOffOP_80M->Get(uiFreq,&DataFilOff);
			iOffset += DataFilOff.m_iOffset[RxRefOPTableR1C::GetRefSegIdx(m_dRef)];
		}
	}
	if (IO == Mode) {
		((CalFileR1C *)(m_pSP1401->GetCalFile()))->m_pRxRefIO->Get(uiFreq,m_dRef,&RxState);
		iOffset = ((CalFileR1C *)(m_pSP1401->GetCalFile()))->m_pRxAttIO->Get(uiFreq,m_dRef);
		iOffset += RxState.m_iADOffset;

		if (_80M == bw) {
			RxFilterOffTableR1C::DataM DataFilOff;
			((CalFileR1C *)(m_pSP1401->GetCalFile()))->m_pRxFilOffIO_80M->Get(uiFreq,&DataFilOff);
			iOffset += DataFilOff.m_iOffset[RxRefIOTableR1C::GetRefSegIdx(m_dRef)];
		}
	}

	m_strLNAAtt.Format("%s",SP1401::Rx_LNA == RxState.m_sLNAAtt ? "LNA" : (SP1401::Rx_Att_0 == RxState.m_sAtt019 ? "Att 0" : "Att 19"));
	m_fAtt1 = RxState.m_fAtt1;
	m_fAtt2 = RxState.m_fAtt2;
	m_fAtt3 = RxState.m_fAtt3;

	m_pSP1401->SetIOMode(Mode);
	m_pSP1401->SetRxFreq(uiFreq);
	m_pSP1401->SetRxLNAAttSw((SP1401::RxLNAAtt)(RxState.m_sLNAAtt));
	m_pSP1401->SetRxAtt019Sw((SP1401::RxAtt019)(RxState.m_sAtt019));
	m_pSP1401->SetRxAtt((double)m_fAtt1,(double)m_fAtt2,(double)m_fAtt3);
	m_pSP2401->SetRxPowerComp(iOffset);
	Sleep(2);

	CSP1401R1A::PowerMeasState State = CSP1401R1A::IDLE;
	STACHKV(m_pSP1401->SetPowerMeasSrc(CSP1401R1A::PowerMeasFreeRun,false));
	m_pSP1401->SetPowerMeasTimeout(6553600);
	STACHKV(m_pSP1401->SetPowerMeasSamples(102400));
	STACHKV(m_pSP1401->PowerMeasAbort());
	STACHKV(m_pSP1401->PowerMeasStart());

	while (State != CSP1401R1A::DONE) {
		STACHKV(m_pSP1401->GetPowerMeasState(State));
		if (State == CSP1401R1A::TT)
			break;
	}
	STACHKV(m_pSP1401->GetPowerMeasPower(m_dPower));
	m_dPower += m_dRef;
	UpdateData(FALSE);
}

void CCalR1CRxRefDlg::OnLvnGetdispinfoList1RxCalData(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	LVITEMA pItem = pDispInfo->item;
	int iItemIdx = pItem.iItem;

	if (pItem.mask & LVIF_TEXT)
	{
		CString strText;
		int iFreqIdx = (int)(iItemIdx / 3);
		int iDataIdx = (int)(iItemIdx % 3);
		if (0 == pItem.iSubItem)
			strText.Format("%.0f",m_DataOP[iFreqIdx].m_uiFreq / 1000000.0);
		else if (1 == pItem.iSubItem) {
			if (0 == iDataIdx)
				strText = _T("10");
			else if (1 == iDataIdx)
				strText = _T("10");
			else
				strText = _T("-20");
		}
		else if (2 == pItem.iSubItem)
			strText = m_DataOP[iFreqIdx].m_State[iDataIdx].m_iLNAAtt ? _T("LNA") : _T("Att");
		else if (3 == pItem.iSubItem)
			strText = m_DataOP[iFreqIdx].m_State[iDataIdx].m_iAtt019 ? _T("Att 0") : _T("Att 19");
		else if (4 == pItem.iSubItem)
			strText.Format("%.2f",m_DataOP[iFreqIdx].m_State[iDataIdx].m_fAtt1);
		else if (5 == pItem.iSubItem)
			strText.Format("%.2f",m_DataOP[iFreqIdx].m_State[iDataIdx].m_fAtt2);
		else if (6 == pItem.iSubItem)
			strText.Format("%.2f",m_DataOP[iFreqIdx].m_State[iDataIdx].m_fAtt3);
		else if (7 == pItem.iSubItem)
			strText.Format("%lld",m_DataOP[iFreqIdx].m_State[iDataIdx].m_iADOffset);
		else if (8 == pItem.iSubItem)
			strText.Format("%.3f",m_DataOP[iFreqIdx].m_State[iDataIdx].m_dTemp[0]);
		else if (9 == pItem.iSubItem)
			strText.Format("%.3f",m_DataOP[iFreqIdx].m_State[iDataIdx].m_dTemp[1]);
		else if (10 == pItem.iSubItem)
			strText.Format("%.3f",m_DataOP[iFreqIdx].m_State[iDataIdx].m_dTemp[2]);
		else if (11 == pItem.iSubItem)
			strText.Format("%.3f",m_DataOP[iFreqIdx].m_State[iDataIdx].m_dTemp[3]);
		else if (12 == pItem.iSubItem)
			tm2str(m_DataOP[iFreqIdx].m_EndTime,strText);

		lstrcpyn(pItem.pszText, strText, pItem.cchTextMax);
	}
	*pResult = 0;
}

void CCalR1CRxRefDlg::OnLvnGetdispinfoList2RxCalData(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	LVITEMA pItem = pDispInfo->item;
	int iItemIdx = pItem.iItem;

	if (pItem.mask & LVIF_TEXT)
	{
		CString strText;
		int iFreqIdx = (int)(iItemIdx / 3);
		int iDataIdx = (int)(iItemIdx % 3);
		if (0 == pItem.iSubItem)
			strText.Format("%.0f",m_DataIO[iFreqIdx].m_uiFreq / 1000000.0);
		else if (1 == pItem.iSubItem) {
			if (0 == iDataIdx)
				strText = _T("10");
			else if (1 == iDataIdx)
				strText = _T("10");
			else
				strText = _T("-20");
		}
		else if (2 == pItem.iSubItem)
			strText = m_DataIO[iFreqIdx].m_State[iDataIdx].m_iLNAAtt ? _T("LNA") : _T("Att");
		else if (3 == pItem.iSubItem)
			strText = m_DataIO[iFreqIdx].m_State[iDataIdx].m_iAtt019 ? _T("Att 0") : _T("Att 19");
		else if (4 == pItem.iSubItem)
			strText.Format("%.2f",m_DataIO[iFreqIdx].m_State[iDataIdx].m_fAtt1);
		else if (5 == pItem.iSubItem)
			strText.Format("%.2f",m_DataIO[iFreqIdx].m_State[iDataIdx].m_fAtt2);
		else if (6 == pItem.iSubItem)
			strText.Format("%.2f",m_DataIO[iFreqIdx].m_State[iDataIdx].m_fAtt3);
		else if (7 == pItem.iSubItem)
			strText.Format("%lld",m_DataIO[iFreqIdx].m_State[iDataIdx].m_iADOffset);
		else if (8 == pItem.iSubItem)
			strText.Format("%.3f",m_DataIO[iFreqIdx].m_State[iDataIdx].m_dTemp[0]);
		else if (9 == pItem.iSubItem)
			strText.Format("%.3f",m_DataIO[iFreqIdx].m_State[iDataIdx].m_dTemp[1]);
		else if (10 == pItem.iSubItem)
			strText.Format("%.3f",m_DataIO[iFreqIdx].m_State[iDataIdx].m_dTemp[2]);
		else if (11 == pItem.iSubItem)
			strText.Format("%.3f",m_DataIO[iFreqIdx].m_State[iDataIdx].m_dTemp[3]);
		else if (12 == pItem.iSubItem)
			tm2str(m_DataIO[iFreqIdx].m_EndTime,strText);

		lstrcpyn(pItem.pszText, strText, pItem.cchTextMax);
	}
	*pResult = 0;
}

LRESULT CCalR1CRxRefDlg::ShowData(WPARAM wParam,LPARAM lParam)
{
	CalIOMode Mode = *(CalIOMode *)wParam;
	if (CAL_OP == Mode) {
		CCalR1CRxRefThread::ResultOP Data = *(CCalR1CRxRefThread::ResultOP *)lParam;
		int iLastItemOP = (Data.m_iIdx) * ARRAY_SIZE(Data.m_State) + Data.m_iRefIdx;
		m_ListOP.EnsureVisible(iLastItemOP,FALSE);
		m_ListOP.SetItemState(iLastItemOP,LVIS_SELECTED,LVIS_SELECTED);

		double dOffset = 0.0;
		for (int32_t i = 0;i < ARRAY_SIZE(Data.m_State);i ++) {
			dOffset = ad2dBc(_0dBFS,_0dBFS + Data.m_State[i].m_iADOffset);
			m_OPLineSerie[i].at(Data.m_iSec)->AddPoint(Data.m_uiFreq / 1000000.0,dOffset);
		}
	}
	if (CAL_IO == Mode) {
		CCalR1CRxRefThread::ResultIO Data = *(CCalR1CRxRefThread::ResultIO *)lParam;
		int iLastItemIO = (Data.m_iIdx) * ARRAY_SIZE(Data.m_State) + Data.m_iRefIdx;
		m_ListIO.EnsureVisible(iLastItemIO,FALSE);
		m_ListIO.SetItemState(iLastItemIO,LVIS_SELECTED,LVIS_SELECTED);

		double dOffset = 0.0;
		for (int32_t i = 0;i < ARRAY_SIZE(Data.m_State);i ++) {
			dOffset = ad2dBc(_0dBFS,_0dBFS + Data.m_State[i].m_iADOffset);
			m_IOLineSerie[i].at(Data.m_iSec)->AddPoint(Data.m_uiFreq / 1000000.0,dOffset);
		}
	}
	return 0;
}

void CCalR1CRxRefDlg::OnBnClickedExportCalFile()
{
	CAL_THREAD_EXISTED_CHKV();
	UpdateData(TRUE);
	m_CalIOMode.SetCurSel(2);
	ResetShowCtrl();

	m_pThread = new CExportR1CRxRefThread(this);
	m_pThread->CreateThread();
}

LRESULT CCalR1CRxRefDlg::SP3301Change(WPARAM wParam,LPARAM lParam)
{
	CString strPath;
	strPath.Format("c:\\CSECal\\rfu%drf%d.cal",m_pSP1401->GetRfuIdx(),m_pSP1401->GetRfIdx());
	((CEdit *)(GetDlgItem(IDC_EDIT4)))->SetWindowText(strPath);
	return 0;
}

LRESULT CCalR1CRxRefDlg::Done(WPARAM wParam,LPARAM lParam)
{
	m_ListOP.RedrawItems(0,(int)m_DataOP.size() * SERIE_SIZE(R1C_RX_REF_OP_STAR,R1C_RX_REF_OP_STOP,R1C_RX_REF_STEP));
	m_ListIO.RedrawItems(0,(int)m_DataIO.size() * SERIE_SIZE(R1C_RX_REF_IO_STAR,R1C_RX_REF_IO_STOP,R1C_RX_REF_STEP));

	DWORD dwStyleList = ::GetWindowLong(m_ListOP.m_hWnd,GWL_STYLE);
	dwStyleList &= ~(LVS_SINGLESEL);
	::SetWindowLong(m_ListOP.m_hWnd,GWL_STYLE,dwStyleList);
	::SetWindowLong(m_ListIO.m_hWnd,GWL_STYLE,dwStyleList);

	return CCalR1CBaseDlg::Done(wParam,lParam);
}
