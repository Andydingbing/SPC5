#include "stdafx.h"
#include "RFU_Debug_Tool.h"
#include "CalR1CRxFilterDlg.h"
#include "CalR1CRxFilterThread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CCalR1CRxFilterDlg, CCalR1CBaseDlg)
IMPLEMENT_CSE_CAL_R1C_DLG(CCalR1CRxFilterDlg)

CCalR1CRxFilterDlg::CCalR1CRxFilterDlg(CWnd* pParent /*=NULL*/)
: CCalR1CBaseDlg(CCalR1CRxFilterDlg::IDD, _T("2G"), pParent)
, m_bFast(FALSE)
{

}

CCalR1CRxFilterDlg::~CCalR1CRxFilterDlg()
{
}

void CCalR1CRxFilterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_TabCtrl);
	DDX_Text(pDX, IDC_EDIT1, m_strFreq);
	DDX_Control(pDX, IDC_COMBO61, m_BW);
	DDX_Check(pDX, IDC_CHECK1, m_bFast);
}


BEGIN_MESSAGE_MAP(CCalR1CRxFilterDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON2, &CCalR1CRxFilterDlg::OnBnClickedStartCal)
	ON_BN_CLICKED(IDC_BUTTON3, &CCalR1CRxFilterDlg::OnBnClickedStop)
	ON_BN_CLICKED(IDC_BUTTON1, &CCalR1CRxFilterDlg::OnBnClickedPause)
	ON_BN_CLICKED(IDC_BUTTON5, &CCalR1CRxFilterDlg::OnBnClickedContinue)
	ON_BN_CLICKED(IDC_BUTTON4, &CCalR1CRxFilterDlg::OnBnClickedExportCalFile)
	ON_BN_CLICKED(IDC_BUTTON6, &CCalR1CRxFilterDlg::OnBnClickedGetAndSet)
	ON_MESSAGE(WM_CSE_SINGLE_THREAD_DONE, &CCalR1CRxFilterDlg::Done)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_RX_FILTER_R1C_80,  &CCalR1CRxFilterDlg::OnLvnGetdispinfoListRxR1cFilter80)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_RX_FILTER_R1C_160, &CCalR1CRxFilterDlg::OnLvnGetdispinfoListRxR1cFilter160)
	ON_WM_SIZE()
	ON_MESSAGE(WM_CSE_CAL_RX_FILTER_RFFR_SHOW, &CCalR1CRxFilterDlg::ShowData)
	ON_MESSAGE(WM_CSE_CAL_RX_FILTER_IFFR_SHOW, &CCalR1CRxFilterDlg::ShowData)
	ON_MESSAGE(WM_CSE_CAL_RX_FILTER_80_SHOW,  &CCalR1CRxFilterDlg::ShowData)
	ON_MESSAGE(WM_CSE_CAL_RX_FILTER_160_SHOW, &CCalR1CRxFilterDlg::ShowData)
	ON_MESSAGE(WM_CSE_SP3301_CHANGE, &CCalR1CRxFilterDlg::SP3301Change)
END_MESSAGE_MAP()


void CCalR1CRxFilterDlg::ResetShowCtrl()
{
	m_TabCtrl.ResetShowCtrl();
}

void CCalR1CRxFilterDlg::OnBnClickedStartCal()
{
	CAL_THREAD_EXISTED_CHKV();
	UpdateData(TRUE);
	CtrlPrepare();
	ResetShowCtrl();

	m_pThread = new CCalR1CRxFilterThread(this);
	m_pThread->CreateThread();
}

void CCalR1CRxFilterDlg::OnBnClickedExportCalFile()
{
	CAL_THREAD_EXISTED_CHKV();
	UpdateData(TRUE);
	ResetShowCtrl();

	m_pThread = new CExportR1CRxFilterThread(this);
	m_pThread->CreateThread();
}

BOOL CCalR1CRxFilterDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_BW.SetCurSel(0);
	m_TabCtrl.InsertItem(0,_T("Freq Res"));
	m_TabCtrl.InsertItem(1,_T("Coef_80M"));
	m_TabCtrl.InsertItem(2,_T("Coef_160M"));
	m_TabCtrl.AttachChild();

	CString strPath;
	strPath.Format("c:\\CSECal\\rfu%drf%d.cal",m_pSP1401->GetRfuIdx(),m_pSP1401->GetRfIdx());
	((CEdit *)(GetDlgItem(IDC_EDIT4)))->SetWindowText(strPath);
	UpdateData(FALSE);

	this->GetWindowRect(&m_crWnd);
	SetControlInfo(IDC_EDIT4,ChgL_Fixx,2,2,2);
	SetControlInfo(IDC_BUTTON4,ChgL_Fixx,2,2,2);
	SetControlInfo(IDC_TAB1,FixL_xyStretch,1,0,1,1,0,1);
	AddThreadIdleCtrl(IDC_EDIT4);
	AddThreadIdleCtrl(IDC_BUTTON4);
	AddThreadIdleCtrl(IDC_BUTTON2);
	AddThreadRunningCtrl(IDC_BUTTON3);
	AddThreadToPauseCtrl(IDC_BUTTON1);
	AddThreadToRunningCtrl(IDC_BUTTON5);
	CtrlResume();
	return TRUE;
}

LRESULT CCalR1CRxFilterDlg::ShowData(WPARAM wParam,LPARAM lParam)
{
	UINT uiMsg = GetCurrentMessage()->message;
	if (WM_CSE_CAL_RX_FILTER_RFFR_SHOW == uiMsg) {
		RxRFFrTableR1C::DataF Data = *(RxRFFrTableR1C::DataF *)lParam;
		m_TabCtrl.m_pLineSerieRFFr->AddPoint(Data.m_uiFreq / 1000000.0,Data.m_dPower);
		return 0;
	}
	else if (WM_CSE_CAL_RX_FILTER_IFFR_SHOW == uiMsg) {
		RxIFFrTableR1C::DataF Data = *(RxIFFrTableR1C::DataF *)lParam;
		m_TabCtrl.m_pLineSerieIFFr->AddPoint(Data.m_dFreq / 1000000.0,Data.m_dPower);
		return 0;
	}
	else if (WM_CSE_CAL_RX_FILTER_80_SHOW == uiMsg) {
		CCalR1CRxFilterThread::Result_80 Data = *(CCalR1CRxFilterThread::Result_80 *)lParam;
		m_TabCtrl.m_pList_80->EnsureVisible(Data.m_iIdx,FALSE);
		m_TabCtrl.m_pList_80->SetItemState(Data.m_iIdx,LVIS_SELECTED,LVIS_SELECTED);
		return 0;
	}
	else if (WM_CSE_CAL_RX_FILTER_160_SHOW == uiMsg) {
		CCalR1CRxFilterThread::Result_160 Data = *(CCalR1CRxFilterThread::Result_160 *)lParam;
		m_TabCtrl.m_pList_160->EnsureVisible(Data.m_iIdx,FALSE);
		m_TabCtrl.m_pList_160->SetItemState(Data.m_iIdx,LVIS_SELECTED,LVIS_SELECTED);
		return 0;
	}
	return 0;
}

void CCalR1CRxFilterDlg::OnBnClickedGetAndSet()
{
	UpdateData(TRUE);
	double dReal[RX_FILTER_ORDER] = {0.0};
	double dImag[RX_FILTER_ORDER] = {0.0};
	memset(dReal,0,sizeof(dReal));
	memset(dImag,0,sizeof(dImag));
	uint64_t uiFreq = 0;
	str2freq(m_strFreq,uiFreq);
	m_pSP1401->SetRxFreq(uiFreq);

	BW bw = (BW)(m_BW.GetCurSel());
	m_pSP1401->GetCalFile()->SetBW(bw);

	if (_80M == bw) {
		RxFilter_80M_TableR1C::DataM Data;
		((CalFileR1C *)(m_pSP1401->GetCalFile()))->m_pRxFilter_80M->Get(uiFreq,&Data);

		Data._2Double(dReal,dImag);
		m_pSP2401->SetRxFilterSw(CSP2401R1A::_3D);
		m_pSP2401->SetRxFilter(dReal,dImag);
	}
	if (_160M == bw) {
		RxFilter_160M_TableR1C::DataM Data;
		((CalFileR1C *)(m_pSP1401->GetCalFile()))->m_pRxFilter_160M->Get(uiFreq,&Data);

		Data._2Double(dReal,dImag);
		m_pSP2401->SetRxFilterSw(CSP2401R1A::_2I3D);
		m_pSP2401->SetRxFilter(dReal,dImag);
	}
}

void CCalR1CRxFilterDlg::OnLvnGetdispinfoListRxR1cFilter80(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	LVITEMA pItem= pDispInfo->item;
	int iItemIdx = pItem.iItem;

	if (pItem.mask & LVIF_TEXT)
	{
		CString strText;
		RxFilter_80M_TableR1C::DataF Data;

		if (0 == pItem.iSubItem)
			strText.Format("%.0f",m_TabCtrl.m_Data_80[iItemIdx].m_uiFreq / 1000000.0);
		else if (1 == pItem.iSubItem)
			coef2str(m_TabCtrl.m_Data_80[iItemIdx].m_iCoefReal,RX_FILTER_ORDER,strText);
		else if (2 == pItem.iSubItem)
			coef2str(m_TabCtrl.m_Data_80[iItemIdx].m_iCoefImag,RX_FILTER_ORDER,strText);
		else if (3 == pItem.iSubItem)
			strText.Format("%.3f",m_TabCtrl.m_Data_80[iItemIdx].m_dTemp[0]);
		else if (4 == pItem.iSubItem)
			strText.Format("%.3f",m_TabCtrl.m_Data_80[iItemIdx].m_dTemp[1]);
		else if (5 == pItem.iSubItem)
			strText.Format("%.3f",m_TabCtrl.m_Data_80[iItemIdx].m_dTemp[2]);
		else if (6 == pItem.iSubItem)
			strText.Format("%.3f",m_TabCtrl.m_Data_80[iItemIdx].m_dTemp[3]);
		else if (7 == pItem.iSubItem)
			tm2str(m_TabCtrl.m_Data_80[iItemIdx].m_EndTime,strText);

		lstrcpyn(pItem.pszText, strText, pItem.cchTextMax);
	}
	*pResult = 0;
}

void CCalR1CRxFilterDlg::OnLvnGetdispinfoListRxR1cFilter160(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	LVITEMA pItem= pDispInfo->item;
	int iItemIdx = pItem.iItem;

	if (pItem.mask & LVIF_TEXT)
	{
		CString strText;
		RxFilter_160M_TableR1C::DataF Data;

		if (0 == pItem.iSubItem)
			strText.Format("%.0f",m_TabCtrl.m_Data_160[iItemIdx].m_uiFreq / 1000000.0);
		else if (1 == pItem.iSubItem)
			coef2str(m_TabCtrl.m_Data_160[iItemIdx].m_iCoefReal,RX_FILTER_ORDER,strText);
		else if (2 == pItem.iSubItem)
			coef2str(m_TabCtrl.m_Data_160[iItemIdx].m_iCoefImag,RX_FILTER_ORDER,strText);
		else if (3 == pItem.iSubItem)
			strText.Format("%.3f",m_TabCtrl.m_Data_160[iItemIdx].m_dTemp[0]);
		else if (4 == pItem.iSubItem)
			strText.Format("%.3f",m_TabCtrl.m_Data_160[iItemIdx].m_dTemp[1]);
		else if (5 == pItem.iSubItem)
			strText.Format("%.3f",m_TabCtrl.m_Data_160[iItemIdx].m_dTemp[2]);
		else if (6 == pItem.iSubItem)
			strText.Format("%.3f",m_TabCtrl.m_Data_160[iItemIdx].m_dTemp[3]);
		else if (7 == pItem.iSubItem) {
			tm2str(m_TabCtrl.m_Data_160[iItemIdx].m_EndTime,strText);
		}

		lstrcpyn(pItem.pszText, strText, pItem.cchTextMax);
	}
	*pResult = 0;
}

LRESULT CCalR1CRxFilterDlg::Done(WPARAM wParam,LPARAM lParam)
{
	m_TabCtrl.m_pList_80->RedrawItems(0,m_TabCtrl.m_pList_80->GetItemCount());
	m_TabCtrl.m_pList_160->RedrawItems(0,m_TabCtrl.m_pList_160->GetItemCount());

	DWORD dwStyleList = ::GetWindowLong(m_TabCtrl.m_pList_80->m_hWnd,GWL_STYLE);
	dwStyleList &= ~(LVS_SINGLESEL);
	::SetWindowLong(m_TabCtrl.m_pList_80->m_hWnd,GWL_STYLE,dwStyleList);
	::SetWindowLong(m_TabCtrl.m_pList_160->m_hWnd,GWL_STYLE,dwStyleList);

	return CCalR1CBaseDlg::Done(wParam,lParam);
}

LRESULT CCalR1CRxFilterDlg::SP3301Change(WPARAM wParam,LPARAM lParam)
{
	CString strPath;
	strPath.Format("c:\\CSECal\\rfu%drf%d.cal",m_pSP1401->GetRfuIdx(),m_pSP1401->GetRfIdx());
	((CEdit *)(GetDlgItem(IDC_EDIT4)))->SetWindowText(strPath);
	return 0;
}