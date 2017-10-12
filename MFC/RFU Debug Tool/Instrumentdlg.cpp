#include "stdafx.h"
#include "RFU_Debug_Tool.h"
#include "InstrumentDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CInstrumentDlg, CDialog)

CInstrumentDlg::CInstrumentDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CInstrumentDlg::IDD, pParent)
	, m_SA_CF(_T("2G"))
	, m_SA_SPAN(_T("10M"))
	, m_SA_REF(5)
	, m_SA_RBW(_T("Auto"))
	, m_SA_VBW(_T("Auto"))
	, m_SA_PEAK_FREQ(_T(""))
	, m_SA_PEAK_PWR(_T(""))
	, m_SA_MARKER_FREQ(_T(""))
	, m_SA_MARKER_PWR(_T(""))
	, m_SA_AVERAGE_COUNT(0)
	, m_SA_MECHATT(_T("Auto"))
	, m_SG_RFOP_ENABLE(FALSE)
	, m_SG_MOD_ENABLE(FALSE)
	, m_SG_CW(_T(""))
	, m_SG_PL(0)
	, m_PM_FREQ(_T(""))
	, m_PM_PWR(_T(""))
{

}

CInstrumentDlg::~CInstrumentDlg()
{
}

void CInstrumentDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_SA_CF);
	DDX_Text(pDX, IDC_EDIT2, m_SA_SPAN);
	DDX_Text(pDX, IDC_EDIT4, m_SA_REF);
	DDX_Text(pDX, IDC_EDIT11, m_SA_RBW);
	DDX_Text(pDX, IDC_EDIT12, m_SA_VBW);
	DDX_Text(pDX, IDC_EDIT13, m_SA_PEAK_FREQ);
	DDX_Text(pDX, IDC_EDIT14, m_SA_PEAK_PWR);
	DDX_Text(pDX, IDC_EDIT15, m_SA_MARKER_FREQ);
	DDX_Text(pDX, IDC_EDIT16, m_SA_MARKER_PWR);
	DDX_Control(pDX, IDC_COMBO5, m_SA_PEAKSEARCH_TYPE);
	DDX_Control(pDX, IDC_COMBO6, m_SA_PREAMP);
	DDX_Text(pDX, IDC_EDIT9, m_SA_AVERAGE_COUNT);
	DDX_Text(pDX, IDC_EDIT10, m_SA_MECHATT);
	DDX_Check(pDX, IDC_SG_RFOP_BUTTON, m_SG_RFOP_ENABLE);
	DDX_Check(pDX, IDC_SG_MOD_BUTTON, m_SG_MOD_ENABLE);
	DDX_Text(pDX, IDC_EDIT17, m_SG_CW);
	DDX_Text(pDX, IDC_EDIT18, m_SG_PL);
	DDX_Text(pDX, IDC_EDIT19, m_PM_FREQ);
	DDX_Text(pDX, IDC_EDIT20, m_PM_PWR);
}


BEGIN_MESSAGE_MAP(CInstrumentDlg, CDialog)
	ON_BN_CLICKED(IDC_INIT_INSTR_BUTTON, &CInstrumentDlg::OnBnClickedInitInstrButton)
	ON_BN_CLICKED(IDC_CLOSE_INSTR_BUTTON, &CInstrumentDlg::OnBnClickedCloseInstrButton)
	ON_BN_CLICKED(IDC_SA_RESET_BUTTON, &CInstrumentDlg::OnBnClickedSaResetButton)
	ON_BN_CLICKED(IDC_SA_SETCF_BUTTON, &CInstrumentDlg::OnBnClickedSaSetcfButton)
	ON_BN_CLICKED(IDC_SA_SETSPAN_BUTTON, &CInstrumentDlg::OnBnClickedSaSetspanButton)
	ON_BN_CLICKED(IDC_SA_SETREF_BUTTON, &CInstrumentDlg::OnBnClickedSaSetrefButton)
	ON_BN_CLICKED(IDC_SA_SETRBW_BUTTON, &CInstrumentDlg::OnBnClickedSaSetrbwButton)
	ON_BN_CLICKED(IDC_SA_SETVBW_BUTTON, &CInstrumentDlg::OnBnClickedSaSetvbwButton)
	ON_BN_CLICKED(IDC_SA_PEAKHIGHEST_BUTTON, &CInstrumentDlg::OnBnClickedSaPeakhighestButton)
	ON_BN_CLICKED(IDC_SA_GETMARKER_BUTTON, &CInstrumentDlg::OnBnClickedSaGetmarkerButton)
	ON_BN_CLICKED(IDC_SA_PEAKSEARCH_BUTTON, &CInstrumentDlg::OnBnClickedSaPeaksearchButton)
	ON_BN_CLICKED(IDC_SA_SETPREAMP_BUTTON, &CInstrumentDlg::OnBnClickedSaSetpreampButton)
	ON_BN_CLICKED(IDC_SA_SWEEPONCE_BUTTON, &CInstrumentDlg::OnBnClickedSaSweeponceButton)
	ON_BN_CLICKED(IDC_SA_SETAVERAGE_BUTTON, &CInstrumentDlg::OnBnClickedSaSetaverageButton)
	ON_BN_CLICKED(IDC_SA_SETMECHATT_BUTTON, &CInstrumentDlg::OnBnClickedSaSetmechattButton)
	ON_BN_CLICKED(IDC_SG_RESET_BUTTON, &CInstrumentDlg::OnBnClickedSgResetButton)
	ON_BN_CLICKED(IDC_SG_RFOP_BUTTON, &CInstrumentDlg::OnBnClickedSgRfopButton)
	ON_BN_CLICKED(IDC_SG_MOD_BUTTON, &CInstrumentDlg::OnBnClickedSgModButton)
	ON_BN_CLICKED(IDC_SG_SETCW_BUTTON, &CInstrumentDlg::OnBnClickedSgSetcwButton)
	ON_BN_CLICKED(IDC_SG_SETPLEVEL_BUTTON, &CInstrumentDlg::OnBnClickedSgSetplevelButton)
	ON_BN_CLICKED(IDC_PM_RESET_BUTTON, &CInstrumentDlg::OnBnClickedPmResetButton)
	ON_BN_CLICKED(IDC_PM_SETFREQ_BUTTON, &CInstrumentDlg::OnBnClickedPmSetfreqButton)
	ON_BN_CLICKED(IDC_PM_GETPWR_BUTTON, &CInstrumentDlg::OnBnClickedPmGetpwrButton)
	ON_BN_CLICKED(IDCANCEL, &CInstrumentDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CInstrumentDlg 消息处理程序

// BOOL CInstrumentDlg::Create()
// {
// 	// TODO: 在此添加专用代码和/或调用基类
// 	return CDialog::Create(CInstrumentDlg::IDD);
// }

void CInstrumentDlg::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类
	ShowWindow(FALSE);
	CDialog::OnCancel();
}

BOOL CInstrumentDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_SA_PEAKSEARCH_TYPE.SetCurSel(ISA::PeakSearchTypes(0));
	m_SA_PREAMP.SetCurSel(1);

	((CButton *)GetDlgItem(IDC_SG_RFOP_BUTTON))->SetWindowText("Disabled");
	((CButton *)GetDlgItem(IDC_SG_MOD_BUTTON)) ->SetWindowText("Disabled");

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CInstrumentDlg::OnBnClickedInitInstrButton()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	try {
		Instrument.Init();
		if (Instrument.HasSA())
			Instrument.SA_Reset();
		if (Instrument.HasSG())
			Instrument.SG_Reset();
		if (Instrument.HasPM())
			Instrument.PM_Reset();
	} catch (CInstrExpt &excep) {
		MessageBox(excep.GetExpt());
	}
	UpdateData(FALSE);
}

void CInstrumentDlg::OnBnClickedCloseInstrButton()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	try {
		Instrument.Close();
	} catch (CInstrExpt &excep) {
		MessageBox(excep.GetExpt());
	}
	UpdateData(FALSE);
}

void CInstrumentDlg::OnBnClickedSaResetButton()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	try {
		Instrument.SA_Reset();
	} catch (CInstrExpt &excep) {
		MessageBox(excep.GetExpt());
	}
	UpdateData(FALSE);
}

void CInstrumentDlg::OnBnClickedSaSetcfButton()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	double sa_cf = 0.0;
	str2freq(m_SA_CF,sa_cf);
	try {
		Instrument.SA_SetCF(sa_cf);
	} catch (CInstrExpt &excep) {
		MessageBox(excep.GetExpt());
	}
	UpdateData(FALSE);
}

void CInstrumentDlg::OnBnClickedSaSetspanButton()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	double sa_span = 0.0;
	str2freq(m_SA_SPAN,sa_span);
	try {
		Instrument.SA_SetSpan(sa_span);
	} catch (CInstrExpt &excep) {
		MessageBox(excep.GetExpt());
	}
	UpdateData(FALSE);
}

void CInstrumentDlg::OnBnClickedSaSetrefButton()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	try {
		Instrument.SA_SetRefLevel(m_SA_REF);
	} catch (CInstrExpt &excep) {
		MessageBox(excep.GetExpt());
	}
	UpdateData(FALSE);
}

void CInstrumentDlg::OnBnClickedSaSetrbwButton()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	double sa_rbw = 0.0;
	bool   sa_rbw_auto = true;
	if (m_SA_RBW == "Auto" || m_SA_RBW == "0")
		sa_rbw_auto = true;
	else {
		str2freq(m_SA_RBW,sa_rbw);
		sa_rbw_auto = false;
	}
	try {
		Instrument.SA_SetRBW(sa_rbw,sa_rbw_auto);
	} catch (CInstrExpt &excep) {
		MessageBox(excep.GetExpt());
	}
	UpdateData(FALSE);
}

void CInstrumentDlg::OnBnClickedSaSetvbwButton()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	double sa_vbw = 0.0;
	bool   sa_vbw_auto = true;
	if (m_SA_VBW == "Auto" || m_SA_VBW == "0")
		sa_vbw_auto = true;
	else {
		str2freq(m_SA_VBW,sa_vbw);
		sa_vbw_auto = false;
	}
	try {
		Instrument.SA_SetVBW(sa_vbw,sa_vbw_auto);
	} catch (CInstrExpt &excep) {
		MessageBox(excep.GetExpt());
	}
	UpdateData(FALSE);
}

void CInstrumentDlg::OnBnClickedSaPeakhighestButton()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	double sa_peak_freq = 0.0;
	double sa_peak_pwr  = 0.0;
	try {
		Instrument.SA_SetPeakSearch(ISA::PeakHighest);
		Instrument.SA_GetMarkerFrequency(sa_peak_freq);
		Instrument.SA_GetMarkerPwr(sa_peak_pwr);
		freq2str(sa_peak_freq,m_SA_PEAK_FREQ);
		m_SA_PEAK_FREQ += "Hz";
		m_SA_PEAK_PWR.Format("%.3fdBm",sa_peak_pwr);
	} catch (CInstrExpt &excep) {
		MessageBox(excep.GetExpt());
	}
	UpdateData(FALSE);
}

void CInstrumentDlg::OnBnClickedSaGetmarkerButton()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	double sa_marker_freq = 0.0;
	double sa_marker_pwr  = 0.0;
	try {
		Instrument.SA_GetMarkerFrequency(sa_marker_freq);
		Instrument.SA_GetMarkerPwr(sa_marker_pwr);
		freq2str(sa_marker_freq,m_SA_MARKER_FREQ);
		m_SA_MARKER_FREQ += "Hz";
		m_SA_MARKER_PWR.Format("%.3fdBm",sa_marker_pwr);
	} catch (CInstrExpt &excep) {
		MessageBox(excep.GetExpt());
	}
	UpdateData(FALSE);
}

void CInstrumentDlg::OnBnClickedSaPeaksearchButton()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	try {
		Instrument.SA_SetPeakSearch((ISA::PeakSearchTypes)m_SA_PEAKSEARCH_TYPE.GetCurSel());
	} catch (CInstrExpt &excep) {
		MessageBox(excep.GetExpt());
	}
	UpdateData(FALSE);
}

void CInstrumentDlg::OnBnClickedSaSetpreampButton()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	bool sa_enable_preamp = ((m_SA_PREAMP.GetCurSel() == 0) ? true : false);
	try {
		Instrument.SA_SetEnablePreamp(sa_enable_preamp);
	} catch (CInstrExpt &excep) {
		MessageBox(excep.GetExpt());
	}
	UpdateData(FALSE);
}

void CInstrumentDlg::OnBnClickedSaSweeponceButton()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	try {
		Instrument.SA_SweepOnce();
	} catch (CInstrExpt &excep) {
		MessageBox(excep.GetExpt());
	}
	UpdateData(FALSE);
}

void CInstrumentDlg::OnBnClickedSaSetaverageButton()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	bool sa_enable_averagetrace = ((m_SA_AVERAGE_COUNT == 0) ? false : true);
	try {
		Instrument.SA_SetAverTrace(sa_enable_averagetrace,m_SA_AVERAGE_COUNT);
	} catch (CInstrExpt &excep) {
		MessageBox(excep.GetExpt());
	}
	UpdateData(FALSE);
}

void CInstrumentDlg::OnBnClickedSaSetmechattButton()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	bool sa_mechatt_auto = true;
	int  sa_mechatt = 0;
	if (m_SA_MECHATT == "Auto") {
		sa_mechatt_auto = true;
		sa_mechatt = 0;
	}
	else {
		sa_mechatt_auto = false;
		sa_mechatt = atoi((LPSTR)(LPCTSTR)m_SA_MECHATT);
	}
	try {
		Instrument.SA_SetMechAtten(sa_mechatt_auto,sa_mechatt);
	}catch (CInstrExpt &excep) {
		MessageBox(excep.GetExpt());
	}
	UpdateData(FALSE);
}

void CInstrumentDlg::OnBnClickedSgResetButton()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	try {
		Instrument.SG_Reset();
	} catch (CInstrExpt &excep) {
		MessageBox(excep.GetExpt());
	}
	UpdateData(FALSE);
}

void CInstrumentDlg::OnBnClickedSgRfopButton()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	((CButton *)GetDlgItem(IDC_SG_RFOP_BUTTON))->SetWindowText((m_SG_RFOP_ENABLE ? "Enabled" : "Disabled"));
 	try {
 		Instrument.SG_SetOutputEnable(m_SG_RFOP_ENABLE);
 	} catch (CInstrExpt &excep) {
 		MessageBox(excep.GetExpt());
 	}
	UpdateData(FALSE);
}

void CInstrumentDlg::OnBnClickedSgModButton()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	((CButton *)GetDlgItem(IDC_SG_MOD_BUTTON))->SetWindowText((m_SG_MOD_ENABLE ? "Enabled" : "Disabled"));
	try {
		Instrument.SG_SetModEnable(m_SG_MOD_ENABLE);
	} catch (CInstrExpt &excep) {
		MessageBox(excep.GetExpt());
	}
	UpdateData(FALSE);
}

void CInstrumentDlg::OnBnClickedSgSetcwButton()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	double sg_cw = 0.0;
	str2freq(m_SG_CW,sg_cw);
	try {
		Instrument.SG_SetCW(sg_cw);
	} catch (CInstrExpt &excep) {
		MessageBox(excep.GetExpt());
	}
	UpdateData(FALSE);
}

void CInstrumentDlg::OnBnClickedSgSetplevelButton()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	try {
		Instrument.SG_SetPL(m_SG_PL);
	} catch (CInstrExpt &excep) {
		MessageBox(excep.GetExpt());
	}
	UpdateData(FALSE);
}

void CInstrumentDlg::OnBnClickedPmResetButton()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	try {
		Instrument.PM_Reset();
	} catch (CInstrExpt &excep) {
		MessageBox(excep.GetExpt());
	}
	UpdateData(FALSE);
}

void CInstrumentDlg::OnBnClickedPmSetfreqButton()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	double pm_freq = 0.0;
	str2freq(m_PM_FREQ,pm_freq);
	try {
		Instrument.PM_SetFrequency(pm_freq);
	} catch (CInstrExpt &excep) {
		MessageBox(excep.GetExpt());
	}
	UpdateData(FALSE);
}

void CInstrumentDlg::OnBnClickedPmGetpwrButton()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	double pm_pwr = 0.0;
	try {
		Instrument.PM_GetPwr(pm_pwr);
		m_PM_PWR.Format("%.3fdBm",pm_pwr);
	} catch (CInstrExpt &excep) {
		MessageBox(excep.GetExpt());
	}
	UpdateData(FALSE);
}

void CInstrumentDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
}

void CInstrumentDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类

//	CDialog::OnOK();
}
