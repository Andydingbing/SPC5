#include "stdafx.h"
#include "RFU_Debug_Tool.h"
#include "CalRxSgpmmDlg.h"
#include "CalRxSgpmmThread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CCalRxSgpmmDlg, CCalR1ABaseDlg)
IMPLEMENT_CSE_CAL_R1A_DLG(CCalRxSgpmmDlg)

CCalRxSgpmmDlg::CCalRxSgpmmDlg(CWnd* pParent /*=NULL*/)
	: CCalR1ABaseDlg(CCalRxSgpmmDlg::IDD, pParent)
	, m_strFreqStar(_T(""))
	, m_strFreqStop(_T(""))
	, m_strFreqStep(_T(""))
	, m_strPowerStar(_T(""))
	, m_strPowerStop(_T(""))
	, m_strPowerStep(_T(""))
	, m_strPath(_T(""))
{
	m_FreqSeg.clear();
}

CCalRxSgpmmDlg::~CCalRxSgpmmDlg()
{
}

void CCalRxSgpmmDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strFreqStar);
	DDX_Text(pDX, IDC_EDIT2, m_strFreqStop);
	DDX_Text(pDX, IDC_EDIT3, m_strFreqStep);
	DDX_Text(pDX, IDC_EDIT12, m_strPowerStar);
	DDX_Text(pDX, IDC_EDIT67, m_strPowerStop);
	DDX_Text(pDX, IDC_EDIT68, m_strPowerStep);
	DDX_Control(pDX, IDC_LIST_SG_PM_MAP, m_List);
	DDX_Text(pDX, IDC_EDIT4, m_strPath);
}


BEGIN_MESSAGE_MAP(CCalRxSgpmmDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON2, &CCalRxSgpmmDlg::OnBnClickedStartCal)
	ON_BN_CLICKED(IDC_BUTTON3, &CCalRxSgpmmDlg::OnBnClickedStop)
	ON_BN_CLICKED(IDC_BUTTON175, &CCalRxSgpmmDlg::OnBnClickedPause)
	ON_BN_CLICKED(IDC_BUTTON58, &CCalRxSgpmmDlg::OnBnClickedContinue)
	ON_BN_CLICKED(IDC_BUTTON1, &CCalRxSgpmmDlg::OnBnClickedSelCalFile)
	ON_BN_CLICKED(IDC_BUTTON4, &CCalRxSgpmmDlg::OnBnClickedExportCalFile)
	ON_MESSAGE(WM_CSE_CAL_RX_SGPMM_SHOW, &CCalRxSgpmmDlg::ShowData)
	ON_MESSAGE(WM_CSE_SINGLE_THREAD_DONE, &CCalRxSgpmmDlg::Done)
	ON_WM_SIZE()
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_SG_PM_MAP, &CCalRxSgpmmDlg::OnLvnGetdispinfoListSgPmMap)
	ON_MESSAGE(WM_CSE_SP3301_CHANGE, &CCalRxSgpmmDlg::SP3301Change)
END_MESSAGE_MAP()


void CCalRxSgpmmDlg::ResetShowCtrl()
{
	m_FreqSeg.clear();
	CSGPMMFile::Point Pt;
	Pt.m_Key.m_uiFreq = 0;
	Pt.m_Key.m_fPow = 0.0;
	Pt.m_fPow = 0.0;
	CSGPMMFile::FreqSeg Seg;
	Seg.clear();

	float fPowerStar = 0.0,fPowerStop = 0.0,fPowerStep = 0.0;
	str2freq(m_strPowerStar,fPowerStar);
	str2freq(m_strPowerStop,fPowerStop);
	str2freq(m_strPowerStep,fPowerStep);

	int32_t iPowPts = (int32_t)(SERIE_SIZE(fPowerStar,fPowerStop,fPowerStep));
	for (int32_t i = 0;i < iPowPts;i ++) {
		Pt.m_Key.m_fPow = fPowerStar + i * fPowerStep;
		Seg.push_back(Pt);
	}

	CString strPow;
	uint64_t uiFreq[SERIE_SIZE(RF_RX_FREQ_STAR,RF_RX_FREQ_STOP,RF_RX_FREQ_STEP_CALLED_R1A)];
	int32_t iFreqPts = freq2array(m_strFreqStar,m_strFreqStop,m_strFreqStep,(int64_t *)uiFreq);

	m_List.DeleteAllItems();
	while (m_List.DeleteColumn(0));
	m_List.InsertColumn(0,_T("Freq(Hz)"),LVCFMT_CENTER,60);
	for (int32_t i = 0;i < iPowPts;i ++) {
		strPow.Format("%.2f",Seg.at(i).m_Key.m_fPow);
		m_List.InsertColumn(i + 1,strPow,LVCFMT_CENTER,50);
	}
	for (int32_t i = 0;i < iFreqPts;i ++)
		m_FreqSeg.push_back(Seg);

	m_List.SetItemCount(iFreqPts);

	DWORD dwStyleList = ::GetWindowLong(m_List.m_hWnd,GWL_STYLE);
	dwStyleList |= LVS_SINGLESEL;
	::SetWindowLong(m_List.m_hWnd,GWL_STYLE,dwStyleList);
}

void CCalRxSgpmmDlg::OnBnClickedStartCal()
{
	CAL_THREAD_EXISTED_CHKV();
	UpdateData(TRUE);
	CtrlPrepare();
	ResetShowCtrl();

	m_pThread = new CCalRxSgpmmThread(this);
	m_pThread->CreateThread();
}

LRESULT CCalRxSgpmmDlg::ShowData(WPARAM wParam,LPARAM lParam)
{
	CCalRxSgpmmThread::Result Data = *(CCalRxSgpmmThread::Result *)lParam;
	m_List.EnsureVisible(Data.m_iFreqIdx,FALSE);
	m_List.SetItemState(Data.m_iFreqIdx,LVIS_SELECTED,LVIS_SELECTED);
	return 0;
}

void CCalRxSgpmmDlg::OnBnClickedGetAndSet()
{
	return;
}

void CCalRxSgpmmDlg::OnBnClickedSelCalFile()
{
	CFileDialog CalFile(TRUE);
	if (CalFile.DoModal() == IDOK)
		((CEdit *)(GetDlgItem(IDC_EDIT4)))->SetWindowText(CalFile.GetPathName());
}

void CCalRxSgpmmDlg::OnBnClickedExportCalFile()
{
	UpdateData(TRUE);

	CString strPath,strPow;
	vector<uint64_t> Freqs;
	vector<float> Pows;
	int32_t i = 1;

	CSGPMMFile SgpmmFile((LPSTR)(LPCTSTR)(m_strPath));
	SgpmmFile.To2DArray(m_FreqSeg,&Freqs,&Pows);

	m_List.DeleteAllItems();
	while (m_List.DeleteColumn(0));
	m_List.InsertColumn(0,_T("Freq(Hz)"),LVCFMT_CENTER,60);
	for (vector<float>::iterator iter = Pows.begin();iter != Pows.end();iter ++) {
		strPow.Format("%.2f",(*iter));
		m_List.InsertColumn(i,strPow,LVCFMT_CENTER,50);
		i ++;
	}
	m_List.SetItemCount((int)(Freqs.size()));
}

LRESULT CCalRxSgpmmDlg::Done(WPARAM wParam,LPARAM lParam)
{
	m_List.RedrawItems(0,(int)m_FreqSeg.size());

	DWORD dwStyleList = ::GetWindowLong(m_List.m_hWnd,GWL_STYLE);
	dwStyleList &= ~(LVS_SINGLESEL);
	::SetWindowLong(m_List.m_hWnd,GWL_STYLE,dwStyleList);

	return CCalR1ABaseDlg::Done(wParam,lParam);
}

BOOL CCalRxSgpmmDlg::OnInitDialog()
{
	CCalR1ABaseDlg::OnInitDialog();

	m_strFreqStar.Format("%dM",RF_RX_FREQ_STAR / 1000000);
	m_strFreqStop.Format("%dM",RF_RX_FREQ_STOP / 1000000);
	m_strFreqStep.Format("%dM",RF_RX_FREQ_STEP_CALLED_R1A / 1000000);
	m_strPowerStar.Format("%.2f",10.00);
	m_strPowerStop.Format("%.2f",float(R1A_RX_REF_OP_STOP));
	m_strPowerStep.Format("%.2f",float(R1A_RX_REF_STEP));
	((CEdit *)(GetDlgItem(IDC_EDIT4)))->SetWindowText("c:\\CSECal\\n5182bz98.sgpmm");
	UpdateData(FALSE);

	DWORD dwStyleList = m_List.GetExtendedStyle();
	dwStyleList |= LVS_EX_GRIDLINES;
	dwStyleList |= LVS_EX_FULLROWSELECT;
	dwStyleList &= ~LVS_EX_AUTOSIZECOLUMNS;
	m_List.SetExtendedStyle(dwStyleList);
	m_List.SetBkColor(PROTECT_EYE);
	m_List.SetTextBkColor(PROTECT_EYE);

	this->GetWindowRect(&m_crWnd);
	SetControlInfo(IDC_EDIT1,FixL_xStretch,2,0,1);
	SetControlInfo(IDC_EDIT2,FixL_xStretch,2,0,1);
	SetControlInfo(IDC_EDIT3,FixL_xStretch,2,0,1);
	SetControlInfo(IDC_STATIC_PW_STAR,ChgL_Fixx,2,1,1);
	SetControlInfo(IDC_STATIC_PW_STOP,ChgL_Fixx,2,1,1);
	SetControlInfo(IDC_STATIC_PW_STEP,ChgL_Fixx,2,1,1);
	SetControlInfo(IDC_EDIT12,ChgL_xStretch,2,1,2);
	SetControlInfo(IDC_EDIT67,ChgL_xStretch,2,1,2);
	SetControlInfo(IDC_EDIT68,ChgL_xStretch,2,1,2);
	SetControlInfo(IDC_EDIT4,ChgL_Fixx,2,2,2);
	SetControlInfo(IDC_BUTTON1,ChgL_Fixx,2,2,2);
	SetControlInfo(IDC_BUTTON4,ChgL_Fixx,2,2,2);
	SetControlInfo(IDC_BUTTON2,ChgL_Fixx,2,2,2);
	SetControlInfo(IDC_BUTTON3,ChgL_Fixx,2,2,2);
	SetControlInfo(IDC_BUTTON175, ChgL_Fixx,2,2,2);
	SetControlInfo(IDC_BUTTON58, ChgL_Fixx,2,2,2);
	SetControlInfo(IDC_LIST_SG_PM_MAP,FixL_xyStretch,2,0,2,1,0,1);
	AddThreadIdleCtrl(IDC_EDIT1);
	AddThreadIdleCtrl(IDC_EDIT2);
	AddThreadIdleCtrl(IDC_EDIT3);
	AddThreadIdleCtrl(IDC_EDIT12);
	AddThreadIdleCtrl(IDC_EDIT67);
	AddThreadIdleCtrl(IDC_EDIT68);
	AddThreadIdleCtrl(IDC_BUTTON1);
	AddThreadIdleCtrl(IDC_BUTTON4);
	AddThreadIdleCtrl(IDC_BUTTON2);
	AddThreadRunningCtrl(IDC_BUTTON3);
	AddThreadToPauseCtrl(IDC_BUTTON175);
	AddThreadToRunningCtrl(IDC_BUTTON58);
	CtrlResume();
	return TRUE;
}

void CCalRxSgpmmDlg::OnLvnGetdispinfoListSgPmMap(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	LVITEMA pItem= pDispInfo->item;
	int iItemIdx = pItem.iItem;

	if (pItem.mask & LVIF_TEXT)
	{
		CString strText;

		if (0 == pItem.iSubItem)
			strText.Format("%dM",m_FreqSeg[iItemIdx].at(0).m_Key.m_uiFreq / 1000000);
		else
			strText.Format("%.2f",m_FreqSeg[iItemIdx].at(pItem.iSubItem - 1).m_fPow);
		lstrcpyn(pItem.pszText, strText, pItem.cchTextMax);
	}
	*pResult = 0;
}

LRESULT CCalRxSgpmmDlg::SP3301Change(WPARAM wParam,LPARAM lParam)
{
	return 0;
}