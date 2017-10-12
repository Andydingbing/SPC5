#include "stdafx.h"
#include "RFU_Debug_Tool.h"
#include "LTC266616Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CLTC266616Dlg, CDialog)

CLTC266616Dlg::CLTC266616Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLTC266616Dlg::IDD, pParent)
	, m_strDAC0(_T("001111111111"))
	, m_strDAC1(_T("001111111111"))
	, m_strDAC2(_T("001111111111"))
	, m_strDAC3(_T("001111111111"))
	, m_strDAC4(_T("011010110111"))
	, m_strDAC5(_T("010110011001"))
	, m_strDAC6(_T("011010110111"))
	, m_strDAC7(_T("001111111111"))
	, m_dV0(-2.5)
	, m_dV1(-2.5)
	, m_dV2(-2.5)
	, m_dV3(-2.5)
	, m_dV4(-0.8)
	, m_dV5(-1.25)
	, m_dV6(-0.8)
	, m_dV7(-2.5)
	, m_bUseV0(FALSE)
	, m_bUseV1(FALSE)
	, m_bUseV2(FALSE)
	, m_bUseV3(FALSE)
	, m_bUseV4(FALSE)
	, m_bUseV5(FALSE)
	, m_bUseV6(FALSE)
	, m_bUseV7(FALSE)
	, m_bUseV(FALSE)
{

}

CLTC266616Dlg::~CLTC266616Dlg()
{
}

void CLTC266616Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strDAC0);
	DDX_Text(pDX, IDC_EDIT2, m_strDAC1);
	DDX_Text(pDX, IDC_EDIT3, m_strDAC2);
	DDX_Text(pDX, IDC_EDIT4, m_strDAC3);
	DDX_Text(pDX, IDC_EDIT5, m_strDAC4);
	DDX_Text(pDX, IDC_EDIT6, m_strDAC5);
	DDX_Text(pDX, IDC_EDIT7, m_strDAC6);
	DDX_Text(pDX, IDC_EDIT8, m_strDAC7);
	DDX_Text(pDX, IDC_EDIT11, m_dV0);
	DDX_Text(pDX, IDC_EDIT66, m_dV1);
	DDX_Text(pDX, IDC_EDIT26, m_dV2);
	DDX_Text(pDX, IDC_EDIT14, m_dV3);
	DDX_Text(pDX, IDC_EDIT15, m_dV4);
	DDX_Text(pDX, IDC_EDIT9, m_dV5);
	DDX_Text(pDX, IDC_EDIT10, m_dV6);
	DDX_Text(pDX, IDC_EDIT17, m_dV7);
	DDX_Check(pDX, IDC_CHECK1, m_bUseV0);
	DDX_Check(pDX, IDC_CHECK2, m_bUseV1);
	DDX_Check(pDX, IDC_CHECK5, m_bUseV2);
	DDX_Check(pDX, IDC_CHECK6, m_bUseV3);
	DDX_Check(pDX, IDC_CHECK7, m_bUseV4);
	DDX_Check(pDX, IDC_CHECK21, m_bUseV5);
	DDX_Check(pDX, IDC_CHECK22, m_bUseV6);
	DDX_Check(pDX, IDC_CHECK23, m_bUseV7);
	DDX_Check(pDX, IDC_CHECK24, m_bUseV);
}


BEGIN_MESSAGE_MAP(CLTC266616Dlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON7, &CLTC266616Dlg::OnBnClickedSetDAC0)
	ON_BN_CLICKED(IDC_BUTTON19, &CLTC266616Dlg::OnBnClickedSetDAC1)
	ON_BN_CLICKED(IDC_BUTTON28, &CLTC266616Dlg::OnBnClickedSetDAC2)
	ON_BN_CLICKED(IDC_BUTTON29, &CLTC266616Dlg::OnBnClickedSetDAC3)
	ON_BN_CLICKED(IDC_BUTTON34, &CLTC266616Dlg::OnBnClickedSetDAC4)
	ON_BN_CLICKED(IDC_BUTTON53, &CLTC266616Dlg::OnBnClickedSetDAC5)
	ON_BN_CLICKED(IDC_BUTTON54, &CLTC266616Dlg::OnBnClickedSetDAC6)
	ON_BN_CLICKED(IDC_BUTTON55, &CLTC266616Dlg::OnBnClickedSetDAC7)
	ON_BN_CLICKED(IDC_CHECK24, &CLTC266616Dlg::OnBnClickedUseAllV)
	ON_BN_CLICKED(IDC_BUTTON57, &CLTC266616Dlg::OnBnClickedSetDAC)
END_MESSAGE_MAP()


BOOL CLTC266616Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	SetWindowTextX();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CLTC266616Dlg::SetWindowTextX()
{
	CString strWndText;
	uint32_t uiRfIdx = m_pSP1401->GetRfIdx();
	uint32_t uiRfuIdx = m_pSP1401->GetRfuIdx();
	strWndText.Format("R1A_LTC266616_RFU%d_RF%d",uiRfuIdx,uiRfIdx);
	SetWindowText(strWndText);
}

BOOL CLTC266616Dlg::Create(UINT nIDTemplate, CWnd* pParentWnd /* = NULL */, CSP1401R1A *pSP1401)
{
	m_pSP1401 = pSP1401;
	return CDialog::Create(nIDTemplate,pParentWnd);
}

BOOL CLTC266616Dlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
		return TRUE;
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
		return TRUE;
	else
		return CDialog::PreTranslateMessage(pMsg);
}

void CLTC266616Dlg::OnBnClickedSetDAC0()
{
	UpdateData(TRUE);
	if (m_bUseV0) {
		char szDAC[12];
		radixchg(ltc2666vol2dac(m_dV0),szDAC,2);
		m_strDAC0.Format("%012s",szDAC);
		m_pSP1401->SetLTC2666(0,m_dV0);
	}
	else {
		int32_t iDAC = strtol((LPSTR)(LPCTSTR)m_strDAC0,NULL,2);
		m_dV0 = ltc2666dac2vol(iDAC);
		m_pSP1401->SetLTC2666(0,iDAC);
	}
	SetFocus();
	UpdateData(FALSE);
}

void CLTC266616Dlg::OnBnClickedSetDAC1()
{
	UpdateData(TRUE);
	if (m_bUseV1) {
		char szDAC[12];
		radixchg(ltc2666vol2dac(m_dV1),szDAC,2);
		m_strDAC1.Format("%012s",szDAC);
		m_pSP1401->SetLTC2666(1,m_dV1);
	}
	else {
		int32_t iDAC = strtol((LPSTR)(LPCTSTR)m_strDAC1,NULL,2);
		m_dV1 = ltc2666dac2vol(iDAC);
		m_pSP1401->SetLTC2666(1,iDAC);
	}
	SetFocus();
	UpdateData(FALSE);
}

void CLTC266616Dlg::OnBnClickedSetDAC2()
{
	UpdateData(TRUE);
	if (m_bUseV2) {
		char szDAC[12];
		radixchg(ltc2666vol2dac(m_dV2),szDAC,2);
		m_strDAC2.Format("%012s",szDAC);
		m_pSP1401->SetLTC2666(2,m_dV2);
	}
	else {
		int32_t iDAC = strtol((LPSTR)(LPCTSTR)m_strDAC2,NULL,2);
		m_dV2 = ltc2666dac2vol(iDAC);
		m_pSP1401->SetLTC2666(2,iDAC);
	}
	SetFocus();
	UpdateData(FALSE);
}

void CLTC266616Dlg::OnBnClickedSetDAC3()
{
	UpdateData(TRUE);
	if (m_bUseV3) {
		char szDAC[12];
		radixchg(ltc2666vol2dac(m_dV3),szDAC,2);
		m_strDAC3.Format("%012s",szDAC);
		m_pSP1401->SetLTC2666(3,m_dV3);
	}
	else {
		int32_t iDAC = strtol((LPSTR)(LPCTSTR)m_strDAC3,NULL,2);
		m_dV3 = ltc2666dac2vol(iDAC);
		m_pSP1401->SetLTC2666(3,iDAC);
	}
	SetFocus();
	UpdateData(FALSE);
}

void CLTC266616Dlg::OnBnClickedSetDAC4()
{
	UpdateData(TRUE);
	if (m_bUseV4) {
		char szDAC[12];
		radixchg(ltc2666vol2dac(m_dV4),szDAC,2);
		m_strDAC4.Format("%012s",szDAC);
		m_pSP1401->SetLTC2666(4,m_dV4);
	}
	else {
		int32_t iDAC = strtol((LPSTR)(LPCTSTR)m_strDAC4,NULL,2);
		m_dV4 = ltc2666dac2vol(iDAC);
		m_pSP1401->SetLTC2666(4,iDAC);
	}
	SetFocus();
	UpdateData(FALSE);
}

void CLTC266616Dlg::OnBnClickedSetDAC5()
{
	UpdateData(TRUE);
	if (m_bUseV5) {
		char szDAC[12];
		radixchg(ltc2666vol2dac(m_dV5),szDAC,2);
		m_strDAC5.Format("%012s",szDAC);
		m_pSP1401->SetLTC2666(5,m_dV5);
	}
	else {
		int32_t iDAC = strtol((LPSTR)(LPCTSTR)m_strDAC5,NULL,2);
		m_dV5 = ltc2666dac2vol(iDAC);
		m_pSP1401->SetLTC2666(5,iDAC);
	}
	SetFocus();
	UpdateData(FALSE);
}

void CLTC266616Dlg::OnBnClickedSetDAC6()
{
	UpdateData(TRUE);
	if (m_bUseV6) {
		char szDAC[12];
		radixchg(ltc2666vol2dac(m_dV6),szDAC,2);
		m_strDAC6.Format("%012s",szDAC);
		m_pSP1401->SetLTC2666(6,m_dV6);
	}
	else {
		int32_t iDAC = strtol((LPSTR)(LPCTSTR)m_strDAC6,NULL,2);
		m_dV6 = ltc2666dac2vol(iDAC);
		m_pSP1401->SetLTC2666(6,iDAC);
	}
	SetFocus();
	UpdateData(FALSE);
}

void CLTC266616Dlg::OnBnClickedSetDAC7()
{
	UpdateData(TRUE);
	if (m_bUseV7) {
		char szDAC[12];
		radixchg(ltc2666vol2dac(m_dV7),szDAC,2);
		m_strDAC7.Format("%012s",szDAC);
		m_pSP1401->SetLTC2666(7,m_dV7);
	}
	else {
		int32_t iDAC = strtol((LPSTR)(LPCTSTR)m_strDAC7,NULL,2);
		m_dV7 = ltc2666dac2vol(iDAC);
		m_pSP1401->SetLTC2666(7,iDAC);
	}
	SetFocus();
	UpdateData(FALSE);
}

void CLTC266616Dlg::OnBnClickedUseAllV()
{
	UpdateData(TRUE);
	m_bUseV0 = m_bUseV;
	m_bUseV1 = m_bUseV;
	m_bUseV2 = m_bUseV;
	m_bUseV3 = m_bUseV;
	m_bUseV4 = m_bUseV;
	m_bUseV5 = m_bUseV;
	m_bUseV6 = m_bUseV;
	m_bUseV7 = m_bUseV;
	UpdateData(FALSE);
}

void CLTC266616Dlg::OnBnClickedSetDAC()
{
	OnBnClickedSetDAC0();
	OnBnClickedSetDAC1();
	OnBnClickedSetDAC2();
	OnBnClickedSetDAC3();
	OnBnClickedSetDAC4();
	OnBnClickedSetDAC5();
	OnBnClickedSetDAC6();
	OnBnClickedSetDAC7();
}

void CLTC266616Dlg::OnCancel()
{
	ShowWindow(FALSE);
	/*CDialog::OnCancel();*/
}

BOOL CLTC266616Dlg::ShowWindow(int nCmdShow)
{
	SetWindowTextX();
	return CDialog::ShowWindow(nCmdShow);
}