#include "stdafx.h"
#include "RFU_Debug_Tool.h"
#include "CalR1ARxRefDlg.h"
#include "CalR1ARxRefThread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CCalR1ARxRefDlg, CCalR1ABaseDlg)
IMPLEMENT_CSE_CAL_R1A_DLG(CCalR1ARxRefDlg)

CCalR1ARxRefDlg::CCalR1ARxRefDlg(CWnd* pParent /*=NULL*/)
	: CCalR1ABaseDlg(CCalR1ARxRefDlg::IDD, _T("2G"), pParent)
	, m_strFreqStar(_T(""))
	, m_strFreqStop(_T(""))
	, m_strFreqStep(_T(""))
	, m_dRef(0.0)
	, m_dPower(0)
	, m_strLNAAtt(_T(""))
	, m_dAtt1(0.0)
	, m_iAtt2(0)
	, m_i0dBFS(0)
{
	m_Data.clear();
}

CCalR1ARxRefDlg::~CCalR1ARxRefDlg()
{
}

void CCalR1ARxRefDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strFreqStar);
	DDX_Text(pDX, IDC_EDIT2, m_strFreqStop);
	DDX_Text(pDX, IDC_EDIT3, m_strFreqStep);
	DDX_Text(pDX, IDC_EDIT6, m_strFreq);
	DDX_Text(pDX, IDC_EDIT8, m_dRef);
	DDX_Text(pDX, IDC_EDIT9, m_dPower);
	DDX_Text(pDX, IDC_EDIT10, m_strLNAAtt);
	DDX_Text(pDX, IDC_EDIT21, m_dAtt1);
	DDX_Text(pDX, IDC_EDIT22, m_iAtt2);
	DDX_Text(pDX, IDC_EDIT23, m_i0dBFS);
	DDX_Control(pDX, IDC_COMBO1, m_IOMode);
	DDX_Control(pDX, IDC_LIST1, m_ListOP);
	DDX_Control(pDX, IDC_LIST2, m_ListIO);
	DDX_Control(pDX, IDC_COMBO62, m_CalIOMode);
}


BEGIN_MESSAGE_MAP(CCalR1ARxRefDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON2, &CCalR1ARxRefDlg::OnBnClickedStartCal)
	ON_BN_CLICKED(IDC_BUTTON3, &CCalR1ARxRefDlg::OnBnClickedStop)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON6, &CCalR1ARxRefDlg::OnBnClickedGetAndSet)
	ON_MESSAGE(WM_CSE_SINGLE_THREAD_DONE, &CCalR1ARxRefDlg::Done)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST1, &CCalR1ARxRefDlg::OnLvnGetdispinfoList1RxCalData)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST2, &CCalR1ARxRefDlg::OnLvnGetdispinfoList2RxCalData)
	ON_MESSAGE(WM_CSE_CAL_RX_REF_SHOW, &CCalR1ARxRefDlg::ShowData)
	ON_BN_CLICKED(IDC_BUTTON4, &CCalR1ARxRefDlg::OnBnClickedExportCalFile)
	ON_MESSAGE(WM_CSE_SP3301_CHANGE, &CCalR1ARxRefDlg::SP3301Change)
	ON_MESSAGE(WM_CSE_UPDATE_DATA_FALSE, &CCalR1ARxRefDlg::UpdateDataFalse)
	ON_BN_CLICKED(IDC_BUTTON1, &CCalR1ARxRefDlg::OnBnClickedPause)
	ON_BN_CLICKED(IDC_BUTTON5, &CCalR1ARxRefDlg::OnBnClickedContinue)
END_MESSAGE_MAP()


void CCalR1ARxRefDlg::ResetShowCtrl()
{
	m_Data.clear();
	RxRefTableR1A::DataF Data;
	int32_t iPts = freq2array(m_strFreqStar,m_strFreqStop,m_strFreqStep);
	for (int32_t i = 0;i < iPts;i ++)
		m_Data.push_back(Data);

	m_ListOP.DeleteAllItems();
	while (m_ListOP.DeleteColumn(0));
	m_ListOP.InsertColumn(0,_T("Freq(MHz)"),LVCFMT_CENTER,70);
	m_ListOP.InsertColumn(1,_T("Reference"),LVCFMT_RIGHT,70);
	m_ListOP.InsertColumn(2,_T("LNA/Att"),LVCFMT_CENTER,60);
	m_ListOP.InsertColumn(3,_T("Att 1"),LVCFMT_RIGHT,40);
	m_ListOP.InsertColumn(4,_T("Att 2"),LVCFMT_RIGHT,40);
	m_ListOP.InsertColumn(5,_T("Offset"),LVCFMT_RIGHT,100);

	m_ListIO.DeleteAllItems();
	while (m_ListIO.DeleteColumn(0));
	m_ListIO.InsertColumn(0,_T("Freq(MHz)"),LVCFMT_CENTER,70);
	m_ListIO.InsertColumn(1,_T("Reference"),LVCFMT_RIGHT,70);
	m_ListIO.InsertColumn(2,_T("LNA/Att"),LVCFMT_CENTER,60);
	m_ListIO.InsertColumn(3,_T("Att 1"),LVCFMT_RIGHT,40);
	m_ListIO.InsertColumn(4,_T("Att 2"),LVCFMT_RIGHT,40);
	m_ListIO.InsertColumn(5,_T("Offset"),LVCFMT_RIGHT,100);

	DWORD dwStyleList = ::GetWindowLong(m_ListOP.m_hWnd,GWL_STYLE);
	dwStyleList |= LVS_SINGLESEL;
	::SetWindowLong(m_ListOP.m_hWnd,GWL_STYLE,dwStyleList);
	::SetWindowLong(m_ListIO.m_hWnd,GWL_STYLE,dwStyleList);

	m_ListOP.SetItemCount(iPts * ARRAY_SIZE(Data.m_StateOP));
	m_ListOP.SetRedraw();

	m_ListIO.SetItemCount(iPts * ARRAY_SIZE(Data.m_StateIO));
	m_ListIO.SetRedraw();
}

void CCalR1ARxRefDlg::OnBnClickedStartCal()
{
	CAL_THREAD_EXISTED_CHKV();
	UpdateData(TRUE);
	CtrlPrepare();
	ResetShowCtrl();

	m_pThread = new CCalR1ARxRefThread(this);
	m_pThread->CreateThread();
}

BOOL CCalR1ARxRefDlg::OnInitDialog()
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

	m_strFreqStar.Format("%dM",RF_RX_FREQ_STAR / 1000000);
	m_strFreqStop.Format("%dM",RF_RX_FREQ_STOP / 1000000);
	m_strFreqStep.Format("%dM",RF_RX_FREQ_STEP_CALLED_R1A / 1000000);

	uint32_t uiRfIdx = m_pSP1401->GetRfuIdx();
	uint32_t uiRfuIdx = m_pSP1401->GetRfuIdx();
	CString strPath;
	strPath.Format("c:\\CSECal\\rfu%drf%d.cal",uiRfuIdx,uiRfIdx);
	((CEdit *)(GetDlgItem(IDC_EDIT4)))->SetWindowText(strPath);
	UpdateData(FALSE);

	this->GetWindowRect(&m_crWnd);
	SetControlInfo(IDC_EDIT1,FixL_xStretch,1,0,1);
	SetControlInfo(IDC_EDIT2,FixL_xStretch,1,0,1);
	SetControlInfo(IDC_EDIT4,ChgL_Fixx,1,1,1);
	SetControlInfo(IDC_BUTTON4,ChgL_Fixx,1,1,1);
	SetControlInfo(IDC_LIST1,FixL_xyStretch,2,0,1);
	SetControlInfo(IDC_LIST2,ChgL_xyStretch,2,1,2);
	SetControlInfo(IDC_STATIC_CAL_RX_REF_IO,ChgL_Fixx,2,1,2);
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

void CCalR1ARxRefDlg::OnBnClickedGetAndSet()
{
	UpdateData(TRUE);
	uint64_t uiFreq = 0;
	int64_t iAD_0dBFS = 0;
	SP1401::RxLNAAtt LNAAtt = SP1401::Rx_Att;
	IOMode Mode = ((m_IOMode.GetCurSel()) == 0 ? SP1401::IO : SP1401::OutPut);
	double dAtt1 = 0;
	int32_t iAtt2 = 0;

	str2freq(m_strFreq,uiFreq);
	((CalFileR1A *)(m_pSP1401->GetCalFile()))->m_pRxRef->Get(uiFreq,m_dRef,Mode,iAD_0dBFS,LNAAtt,dAtt1,iAtt2);
	m_i0dBFS = (int)iAD_0dBFS;
	m_strLNAAtt.Format("%s",LNAAtt == SP1401::Rx_Att ? "Att" : "LNA");
	m_dAtt1 = dAtt1;
	m_iAtt2 = (int)iAtt2;

	m_pSP1401->SetIOMode(Mode);
	m_pSP1401->SetRxFreq(uiFreq);
	m_pSP1401->SetRxLNAAttSw(LNAAtt);
	m_pSP1401->SetRxAtt(dAtt1,iAtt2);
	m_pSP2401->SetRxPowerComp((int32_t)(_0dBFS - iAD_0dBFS));
	Sleep(20);

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

void CCalR1ARxRefDlg::OnLvnGetdispinfoList1RxCalData(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	LVITEMA pItem = pDispInfo->item;
	int iItemIdx = pItem.iItem;

	if (pItem.mask & LVIF_TEXT)
	{
		CString strText;
		int iFreqIdx = (int)(iItemIdx / SERIE_SIZE(R1A_RX_REF_OP_STAR,R1A_RX_REF_OP_STOP,R1A_RX_REF_STEP));
		int iDataIdx = (int)(iItemIdx % SERIE_SIZE(R1A_RX_REF_OP_STAR,R1A_RX_REF_OP_STOP,R1A_RX_REF_STEP));
		if (0 == pItem.iSubItem)
			strText.Format("%.0f",m_Data[iFreqIdx].m_uiFreq / 1000000.0);
		else if (1 == pItem.iSubItem)
			strText.Format("%d",30 - iDataIdx);
		else if (2 == pItem.iSubItem)
			strText = m_Data[iFreqIdx].m_StateOP[iDataIdx].m_iLNAAtt ? _T("LNA") : _T("Att");
		else if (3 == pItem.iSubItem)
			strText.Format("%.2f",m_Data[iFreqIdx].m_StateOP[iDataIdx].m_dAtt1);
		else if (4 == pItem.iSubItem)
			strText.Format("%d",m_Data[iFreqIdx].m_StateOP[iDataIdx].m_iAtt2);
		else if (5 == pItem.iSubItem)
			strText.Format("%lld",m_Data[iFreqIdx].m_StateOP[iDataIdx].m_iADOffset);

		lstrcpyn(pItem.pszText, strText, pItem.cchTextMax);
	}
	*pResult = 0;
}

void CCalR1ARxRefDlg::OnLvnGetdispinfoList2RxCalData(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	LVITEMA pItem = pDispInfo->item;
	int iItemIdx = pItem.iItem;

	if (pItem.mask & LVIF_TEXT)
	{
		CString strText;
		int iFreqIdx = (int)(iItemIdx / SERIE_SIZE(R1A_RX_REF_IO_STAR,R1A_RX_REF_IO_STOP,R1A_RX_REF_STEP));
		int iDataIdx = (int)(iItemIdx % SERIE_SIZE(R1A_RX_REF_IO_STAR,R1A_RX_REF_IO_STOP,R1A_RX_REF_STEP));
		if (0 == pItem.iSubItem)
			strText.Format("%.0f",m_Data[iFreqIdx].m_uiFreq / 1000000.0);
		else if (1 == pItem.iSubItem)
			strText.Format("%d",30 - iDataIdx);
		else if (2 == pItem.iSubItem)
			strText = m_Data[iFreqIdx].m_StateIO[iDataIdx].m_iLNAAtt ? _T("LNA") : _T("Att");
		else if (3 == pItem.iSubItem)
			strText.Format("%.2f",m_Data[iFreqIdx].m_StateIO[iDataIdx].m_dAtt1);
		else if (4 == pItem.iSubItem)
			strText.Format("%d",m_Data[iFreqIdx].m_StateIO[iDataIdx].m_iAtt2);
		else if (5 == pItem.iSubItem)
			strText.Format("%lld",m_Data[iFreqIdx].m_StateIO[iDataIdx].m_iADOffset);

		lstrcpyn(pItem.pszText, strText, pItem.cchTextMax);
	}
	*pResult = 0;
}

LRESULT CCalR1ARxRefDlg::ShowData(WPARAM wParam,LPARAM lParam)
{
	CCalR1ARxRefThread::CalRxRefResult Data = *(CCalR1ARxRefThread::CalRxRefResult *)lParam;
	CalIOMode Mode = *(CalIOMode *)wParam;
	if (CAL_OP == Mode) {
		int iLastItemOP = (Data.m_iIdx) * ARRAY_SIZE(Data.m_StateOP) + Data.m_iRefIdxOP;
		m_ListOP.EnsureVisible(iLastItemOP,FALSE);
		m_ListOP.SetItemState(iLastItemOP,LVIS_SELECTED,LVIS_SELECTED);
	}
	if (CAL_IO == Mode) {
		int iLastItemIO = (Data.m_iIdx) * ARRAY_SIZE(Data.m_StateIO) + Data.m_iRefIdxIO;
		m_ListIO.EnsureVisible(iLastItemIO,FALSE);
		m_ListIO.SetItemState(iLastItemIO,LVIS_SELECTED,LVIS_SELECTED);
	}
	return 0;
}

void CCalR1ARxRefDlg::OnBnClickedExportCalFile()
{
	CAL_THREAD_EXISTED_CHKV();
	UpdateData(TRUE);
	ResetShowCtrl();

	m_pThread = new CExportR1ARxRefThread(this);
	m_pThread->CreateThread();
}

LRESULT CCalR1ARxRefDlg::SP3301Change(WPARAM wParam,LPARAM lParam)
{
	uint32_t uiRfIdx = m_pSP1401->GetRfIdx();
	uint32_t uiRfuIdx = m_pSP1401->GetRfuIdx();
	CString strPath;
	strPath.Format("c:\\CSECal\\rfu%drf%d.cal",uiRfuIdx,uiRfIdx);
	((CEdit *)(GetDlgItem(IDC_EDIT4)))->SetWindowText(strPath);
	return 0;
}

LRESULT CCalR1ARxRefDlg::Done(WPARAM wParam,LPARAM lParam)
{
	m_ListOP.RedrawItems(0,(int)m_Data.size() * ARRAY_SIZE(m_Data[0].m_StateOP));
	m_ListIO.RedrawItems(0,(int)m_Data.size() * ARRAY_SIZE(m_Data[0].m_StateIO));

	DWORD dwStyleList = ::GetWindowLong(m_ListOP.m_hWnd,GWL_STYLE);
	dwStyleList &= ~(LVS_SINGLESEL);
	::SetWindowLong(m_ListOP.m_hWnd,GWL_STYLE,dwStyleList);
	::SetWindowLong(m_ListIO.m_hWnd,GWL_STYLE,dwStyleList);

	return CCalR1ABaseDlg::Done(wParam,lParam);
}