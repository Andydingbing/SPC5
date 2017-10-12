#include "stdafx.h"
#include "RFU_Debug_Tool.h"
#include "CalR1CTxPowerDlg.h"
#include "CalR1CTxPowerThread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CCalR1CTxPowerDlg, CCalR1CBaseDlg)
IMPLEMENT_CSE_CAL_R1C_DLG(CCalR1CTxPowerDlg)

CCalR1CTxPowerDlg::CCalR1CTxPowerDlg(CWnd* pParent /*=NULL*/)
	: CCalR1CBaseDlg(CCalR1CTxPowerDlg::IDD, _T("2G"), pParent)
	, m_strFreqStar(_T(""))
	, m_strFreqStop(_T(""))
	, m_strFreqStep(_T(""))
	, m_dDGain(0.0)
	, m_dAtt0(0.0)
	, m_dAtt1(0.0)
	, m_dAtt2(0.0)
	, m_dAtt3(0.0)
	, m_dPower(0.0)
{
	m_pChart = NULL;
	m_OPLineSerie.clear();
	m_IOLineSerie.clear();
	m_DataOP.clear();
	m_DataIO.clear();
}

CCalR1CTxPowerDlg::~CCalR1CTxPowerDlg()
{
	for (int i = 0;i < m_OPLineSerie.size();i ++)
		m_OPLineSerie.at(i)->ClearSerie();
	m_OPLineSerie.clear();
	for (int i = 0;i < m_IOLineSerie.size();i ++)
		m_IOLineSerie.at(i)->ClearSerie();
	m_IOLineSerie.clear();
	if (m_pChart) {
		delete m_pChart;
		m_pChart = NULL;
	}
}

void CCalR1CTxPowerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_TX_R1C_POWER_OP, m_ListOP);
	DDX_Control(pDX, IDC_LIST_TX_R1C_POWER_IO, m_ListIO);
	DDX_Text(pDX, IDC_EDIT1,  m_strFreqStar);
	DDX_Text(pDX, IDC_EDIT11, m_strFreqStop);
	DDX_Text(pDX, IDC_EDIT12, m_strFreqStep);
	DDX_Text(pDX, IDC_EDIT6, m_strFreq);
	DDX_Text(pDX, IDC_EDIT7, m_dDGain);
	DDX_Text(pDX, IDC_EDIT8, m_dAtt0);
	DDX_Text(pDX, IDC_EDIT9, m_dAtt1);
	DDX_Text(pDX, IDC_EDIT10, m_dAtt2);
	DDX_Text(pDX, IDC_EDIT21, m_dAtt3);
	DDX_Text(pDX, IDC_EDIT15, m_dPower);
	DDX_Control(pDX, IDC_COMBO2, m_IOMode);
	DDX_Control(pDX, IDC_COMBO61, m_CalIOMode);
}


BEGIN_MESSAGE_MAP(CCalR1CTxPowerDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON2, &CCalR1CTxPowerDlg::OnBnClickedStartCal)
	ON_BN_CLICKED(IDC_BUTTON3, &CCalR1CTxPowerDlg::OnBnClickedStop)
	ON_BN_CLICKED(IDC_BUTTON21, &CCalR1CTxPowerDlg::OnBnClickedPause)
	ON_BN_CLICKED(IDC_BUTTON15, &CCalR1CTxPowerDlg::OnBnClickedContinue)
	ON_MESSAGE(WM_CSE_SINGLE_THREAD_DONE, &CCalR1CTxPowerDlg::Done)
	ON_MESSAGE(WM_CSE_CAL_TX_POWER_SHOW, &CCalR1CTxPowerDlg::ShowData)
	ON_BN_CLICKED(IDC_BUTTON6, &CCalR1CTxPowerDlg::OnBnClickedGetAndSet)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON4, &CCalR1CTxPowerDlg::OnBnClickedExportCalFile)
	ON_MESSAGE(WM_CSE_SP3301_CHANGE, &CCalR1CTxPowerDlg::SP3301Change)
	ON_EN_CHANGE(IDC_EDIT6, &CCalR1CTxPowerDlg::OnEnChangeFreqEdit)
	ON_MESSAGE(WM_CSE_UPDATE_DATA_FALSE, &CCalR1CTxPowerDlg::UpdateDataFalse)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_TX_R1C_POWER_OP, &CCalR1CTxPowerDlg::OnLvnGetdispinfoListTxR1cPowerOp)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_TX_R1C_POWER_IO, &CCalR1CTxPowerDlg::OnLvnGetdispinfoListTxR1cPowerIo)
END_MESSAGE_MAP()


BOOL CCalR1CTxPowerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

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
	m_pChart->Create(this,crChart,IDC_CAL_R1C_TX_POWER_CHART);
	m_pChart->GetLegend()->SetHorizontalMode(true);
	m_pChart->GetLegend()->UndockLegend(20,40);
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
	pAxis->SetMinMax((double)(R1C_DGAIN_MIN) + 2.0,(double)(R1C_DGAIN_MAX) - 5.0);

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
	SetControlInfo(IDC_LIST_TX_R1C_POWER_OP,ChgL_xyStretch,2,0,1,1,0,1);
	SetControlInfo(IDC_LIST_TX_R1C_POWER_IO,ChgL_xyStretch,2,1,2,1,0,1);
	SetControlInfo(IDC_CAL_R1C_TX_POWER_CHART,ChgLT_xyStretch,1,0,1,1,1,1);
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

void CCalR1CTxPowerDlg::ResetShowCtrl()
{
	int64_t iFreqStar = 0,iFreqStop = 0;
	int32_t iPts = freq2array(m_strFreqStar,m_strFreqStop,m_strFreqStep,NULL,NULL,NULL,&iFreqStop,&iFreqStar);
	m_pChart->GetBottomAxis()->SetMinMax(iFreqStar / 1000000.0,iFreqStop / 1000000.0);

	int32_t iSec = strsections((LPSTR)(LPCTSTR)m_strFreqStar);
	CalIOMode Mode = (CalIOMode)(m_CalIOMode.GetCurSel());

	for (int i = 0;i < m_OPLineSerie.size();i ++)
		m_OPLineSerie.at(i)->ClearSerie();
	m_OPLineSerie.clear();

	for (int i = 0;i < m_IOLineSerie.size();i ++)
		m_IOLineSerie.at(i)->ClearSerie();
	m_IOLineSerie.clear();

	m_pChart->RemoveAllSeries();

	if (CalOP(Mode)) {
		for (int i = 0;i < iSec;i ++) {
			CChartLineSerie *pLineSerie;
			m_OPLineSerie.push_back(pLineSerie);
			m_OPLineSerie.at(i) = m_pChart->CreateLineSerie();
			m_OPLineSerie.at(i)->SetSeriesOrdering(poNoOrdering);
			m_OPLineSerie.at(i)->SetColor(RED);
			m_OPLineSerie.at(i)->SetWidth(3);
			if (0 == i) {
				TChartString strName;
				TChartStringStream strStr;
				strStr << "OP@" << R1C_TX_BASE_POWER_OP << "dBm";
				strName = strStr.str();
				m_OPLineSerie.at(i)->SetName(strName);
			}
		}

		m_DataOP.clear();
		m_ListOP.DeleteAllItems();
		while (m_ListOP.DeleteColumn(0));
		m_ListOP.InsertColumn(0,_T("Freq(MHz)"),LVCFMT_CENTER,65);
		m_ListOP.InsertColumn(1,_T("DGain(dBm)"),LVCFMT_RIGHT,70);
		m_ListOP.InsertColumn(2,_T("Att0"),LVCFMT_RIGHT,40);
		m_ListOP.InsertColumn(3,_T("Att1"),LVCFMT_RIGHT,40);
		m_ListOP.InsertColumn(4,_T("Att2"),LVCFMT_RIGHT,40);
		m_ListOP.InsertColumn(5,_T("Att3"),LVCFMT_RIGHT,40);
		m_ListOP.InsertColumn(6,_T("Temp4_TxLO2"),LVCFMT_CENTER,85);
		m_ListOP.InsertColumn(7,_T("Temp5_TxLO1"),LVCFMT_CENTER,85);
		m_ListOP.InsertColumn(8,_T("Temp6_TxPA4"),LVCFMT_CENTER,85);
		m_ListOP.InsertColumn(9,_T("Temp7_TxPA3"),LVCFMT_CENTER,85);
		m_ListOP.InsertColumn(10,_T("End Time"),LVCFMT_RIGHT,120);

		TxPowerOPTableR1C::DataF Data;
		for (int32_t i = 0;i < iPts;i ++)
			m_DataOP.push_back(Data);

		m_ListOP.SetItemCount(iPts);
		m_ListOP.SetRedraw();
	}
	if (CalIO(Mode)) {
		for (int i = 0;i < iSec;i ++) {
			CChartLineSerie *pLineSerie;
			m_IOLineSerie.push_back(pLineSerie);
			m_IOLineSerie.at(i) = m_pChart->CreateLineSerie();
			m_IOLineSerie.at(i)->SetSeriesOrdering(poNoOrdering);
			m_IOLineSerie.at(i)->SetColor(GREEN);
			m_IOLineSerie.at(i)->SetWidth(3);
			if (0 == i) {
				TChartString strName;
				TChartStringStream strStr;
				strStr << "IO@" << R1C_TX_BASE_POWER_IO << "dBm";
				strName = strStr.str();
				m_IOLineSerie.at(i)->SetName(strName);
			}
		}

		m_DataIO.clear();
		m_ListIO.DeleteAllItems();
		while (m_ListIO.DeleteColumn(0));
		m_ListIO.InsertColumn(0,_T("Freq(MHz)"),LVCFMT_CENTER,65);
		m_ListIO.InsertColumn(1,_T("DGain(dBm)"),LVCFMT_RIGHT,70);
		m_ListIO.InsertColumn(2,_T("Att0"),LVCFMT_RIGHT,40);
		m_ListIO.InsertColumn(3,_T("Att1"),LVCFMT_RIGHT,40);
		m_ListIO.InsertColumn(4,_T("Att2"),LVCFMT_RIGHT,40);
		m_ListIO.InsertColumn(5,_T("Att3"),LVCFMT_RIGHT,40);
		m_ListIO.InsertColumn(6,_T("Temp4_TxLO2"),LVCFMT_CENTER,85);
		m_ListIO.InsertColumn(7,_T("Temp5_TxLO1"),LVCFMT_CENTER,85);
		m_ListIO.InsertColumn(8,_T("Temp6_TxPA4"),LVCFMT_CENTER,85);
		m_ListIO.InsertColumn(9,_T("Temp7_TxPA3"),LVCFMT_CENTER,85);
		m_ListIO.InsertColumn(10,_T("End Time"),LVCFMT_RIGHT,120);

		TxPowerIOTableR1C::DataF Data;
		for (int32_t i = 0;i < iPts;i ++)
			m_DataIO.push_back(Data);

		m_ListIO.SetItemCount(iPts);
		m_ListIO.SetRedraw();
	}

	DWORD dwStyleList = ::GetWindowLong(m_ListOP.m_hWnd,GWL_STYLE);
	dwStyleList |= LVS_SINGLESEL;
	::SetWindowLong(m_ListOP.m_hWnd,GWL_STYLE,dwStyleList);
	::SetWindowLong(m_ListIO.m_hWnd,GWL_STYLE,dwStyleList);
}

void CCalR1CTxPowerDlg::OnBnClickedStartCal()
{
	CAL_THREAD_EXISTED_CHKV();
	UpdateData(TRUE);
	CtrlPrepare();
	ResetShowCtrl();

	m_pThread = new CCalR1CTxPowerThread(this);
	m_pThread->CreateThread();
}

LRESULT CCalR1CTxPowerDlg::ShowData(WPARAM wParam,LPARAM lParam)
{
	CalIOMode Mode = *(CalIOMode *)wParam;
	switch (Mode) {
		case CAL_IO : {
			CCalR1CTxPowerThread::ResultIO Data = *(CCalR1CTxPowerThread::ResultIO *)lParam;
			m_ListIO.EnsureVisible(Data.m_iIdx,FALSE);
			m_ListIO.SetItemState(Data.m_iIdx,LVIS_SELECTED,LVIS_SELECTED);
			m_IOLineSerie.at(Data.m_iSec)->AddPoint(Data.m_uiFreq / 1000000.0,Data.m_dDGain);
			return 0;
						   }
		case CAL_OP : {
			CCalR1CTxPowerThread::ResultOP Data = *(CCalR1CTxPowerThread::ResultOP *)lParam;
			m_ListOP.EnsureVisible(Data.m_iIdx,FALSE);
			m_ListOP.SetItemState(Data.m_iIdx,LVIS_SELECTED,LVIS_SELECTED);
			m_OPLineSerie.at(Data.m_iSec)->AddPoint(Data.m_uiFreq / 1000000.0,Data.m_dDGain);
			return 0;
							   }
		default:return 0;
	}
 	return 0;
}

void CCalR1CTxPowerDlg::OnBnClickedGetAndSet()
{
	UpdateData(TRUE);
	IOMode Mode = (IOMode)(m_IOMode.GetCurSel());
	BW bw = m_pSP1401->GetBw();
	uint64_t uiFreq = 0;

	str2freq(m_strFreq,uiFreq);

	if (OutPut == Mode) {
		TxPowerOPTableR1C::DataM DataPower;
		((CalFileR1C *)(m_pSP1401->GetCalFile()))->m_pTxPowerOP->Get(uiFreq,m_dPower,&DataPower);
		m_dAtt0 = DataPower.m_sAtt0 / 2.0;
		m_dAtt1 = DataPower.m_sAtt1 / 2.0;
		m_dAtt2 = DataPower.m_sAtt2 / 2.0;
		m_dAtt3 = DataPower.m_sAtt3 / 2.0;
		m_dDGain = (double)DataPower.m_fDGain + ((CalFileR1C *)(m_pSP1401->GetCalFile()))->m_pTxAttOP->Get(uiFreq,m_dPower);

		if (_80M == bw) {
			TxFilterOffTableR1C::DataM DataFilOff;
			((CalFileR1C *)(m_pSP1401->GetCalFile()))->m_pTxFilOffOP_80M->Get(uiFreq,&DataFilOff);
			m_dDGain += (double)(DataFilOff.m_fOffset);
		}
	}
	if (IO == Mode) {
		TxPowerIOTableR1C::DataM DataPower;
		((CalFileR1C *)(m_pSP1401->GetCalFile()))->m_pTxPowerIO->Get(uiFreq,m_dPower,&DataPower);
		m_dAtt0 = DataPower.m_sAtt0 / 2.0;
		m_dAtt1 = DataPower.m_sAtt1 / 2.0;
		m_dAtt2 = DataPower.m_sAtt2 / 2.0;
		m_dAtt3 = DataPower.m_sAtt3 / 2.0;
		m_dDGain = (double)DataPower.m_fDGain + ((CalFileR1C *)(m_pSP1401->GetCalFile()))->m_pTxAttIO->Get(uiFreq,m_dPower);

		if (_80M == bw) {
			TxFilterOffTableR1C::DataM DataFilOff;
			((CalFileR1C *)(m_pSP1401->GetCalFile()))->m_pTxFilOffIO_80M->Get(uiFreq,&DataFilOff);
			m_dDGain += (double)(DataFilOff.m_fOffset);
		}
	}

	m_pSP1401->SetTxFreq(uiFreq);
	m_pSP1401->SetIOMode(Mode);
	m_pSP1401->SetTxAtt(m_dAtt0,m_dAtt1,m_dAtt2,m_dAtt3);
	m_pSP2401->SetTxPowerComp(m_dDGain);
	UpdateData(FALSE);
}

void CCalR1CTxPowerDlg::OnBnClickedExportCalFile()
{
	CAL_THREAD_EXISTED_CHKV();
	UpdateData(TRUE);
	m_CalIOMode.SetCurSel(2);
	ResetShowCtrl();

	m_pThread = new CExportR1CTxPowerThread(this);
	m_pThread->CreateThread();
}

LRESULT CCalR1CTxPowerDlg::Done(WPARAM wParam,LPARAM lParam)
{
	m_ListOP.RedrawItems(0,(int)m_DataOP.size());
	m_ListIO.RedrawItems(0,(int)m_DataIO.size());

	DWORD dwStyleList = ::GetWindowLong(m_ListOP.m_hWnd,GWL_STYLE);
	dwStyleList &= ~(LVS_SINGLESEL);
	::SetWindowLong(m_ListOP.m_hWnd,GWL_STYLE,dwStyleList);
	::SetWindowLong(m_ListIO.m_hWnd,GWL_STYLE,dwStyleList);

	return CCalR1CBaseDlg::Done(wParam,lParam);
}

LRESULT CCalR1CTxPowerDlg::SP3301Change(WPARAM wParam,LPARAM lParam)
{
	CString strPath;
	strPath.Format("c:\\CSECal\\rfu%drf%d.cal",m_pSP1401->GetRfuIdx(),m_pSP1401->GetRfIdx());
	((CEdit *)(GetDlgItem(IDC_EDIT4)))->SetWindowText(strPath);
	return 0;
}

void CCalR1CTxPowerDlg::OnLvnGetdispinfoListTxR1cPowerOp(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	LVITEMA pItem= pDispInfo->item;
	int iItemIdx = pItem.iItem;

	if (pItem.mask & LVIF_TEXT)
	{
		CString strText;

		if (0 == pItem.iSubItem)
			strText.Format("%.0f",m_DataOP[iItemIdx].m_uiFreq / 1000000.0);
		else if (1 == pItem.iSubItem)
			strText.Format("%.3f",m_DataOP[iItemIdx].m_dDGain);
		else if (2 == pItem.iSubItem)
			strText.Format("%.1f",m_DataOP[iItemIdx].m_dAtt0);
		else if (3 == pItem.iSubItem)
			strText.Format("%.1f",m_DataOP[iItemIdx].m_dAtt1);
		else if (4 == pItem.iSubItem)
			strText.Format("%.1f",m_DataOP[iItemIdx].m_dAtt2);
		else if (5 == pItem.iSubItem)
			strText.Format("%.1f",m_DataOP[iItemIdx].m_dAtt3);
		else if (6 == pItem.iSubItem)
			strText.Format("%.3f",m_DataOP[iItemIdx].m_dTemp[0]);
		else if (7 == pItem.iSubItem)
			strText.Format("%.3f",m_DataOP[iItemIdx].m_dTemp[1]);
		else if (8 == pItem.iSubItem)
			strText.Format("%.3f",m_DataOP[iItemIdx].m_dTemp[2]);
		else if (9 == pItem.iSubItem)
			strText.Format("%.3f",m_DataOP[iItemIdx].m_dTemp[3]);
		else if (10 == pItem.iSubItem)
			tm2str(m_DataOP[iItemIdx].m_EndTime,strText);

		lstrcpyn(pItem.pszText, strText, pItem.cchTextMax);
	}
	*pResult = 0;
}

void CCalR1CTxPowerDlg::OnLvnGetdispinfoListTxR1cPowerIo(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	LVITEMA pItem= pDispInfo->item;
	int iItemIdx = pItem.iItem;

	if (pItem.mask & LVIF_TEXT)
	{
		CString strText;

		if (0 == pItem.iSubItem)
			strText.Format("%.0f",m_DataIO[iItemIdx].m_uiFreq / 1000000.0);
		else if (1 == pItem.iSubItem)
			strText.Format("%.3f",m_DataIO[iItemIdx].m_dDGain);
		else if (2 == pItem.iSubItem)
			strText.Format("%.1f",m_DataIO[iItemIdx].m_dAtt0);
		else if (3 == pItem.iSubItem)
			strText.Format("%.1f",m_DataIO[iItemIdx].m_dAtt1);
		else if (4 == pItem.iSubItem)
			strText.Format("%.1f",m_DataIO[iItemIdx].m_dAtt2);
		else if (5 == pItem.iSubItem)
			strText.Format("%.1f",m_DataIO[iItemIdx].m_dAtt3);
		else if (6 == pItem.iSubItem)
			strText.Format("%.3f",m_DataIO[iItemIdx].m_dTemp[0]);
		else if (7 == pItem.iSubItem)
			strText.Format("%.3f",m_DataIO[iItemIdx].m_dTemp[1]);
		else if (8 == pItem.iSubItem)
			strText.Format("%.3f",m_DataIO[iItemIdx].m_dTemp[2]);
		else if (9 == pItem.iSubItem)
			strText.Format("%.3f",m_DataIO[iItemIdx].m_dTemp[3]);
		else if (10 == pItem.iSubItem)
			tm2str(m_DataIO[iItemIdx].m_EndTime,strText);

		lstrcpyn(pItem.pszText, strText, pItem.cchTextMax);
	}
	*pResult = 0;
}
