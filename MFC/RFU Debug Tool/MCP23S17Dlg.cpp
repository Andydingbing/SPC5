#include "stdafx.h"
#include "MCP23S17Dlg.h"
#include "algorithm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CMCP23S17Dlg, CDialog)

CMCP23S17Dlg::CMCP23S17Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMCP23S17Dlg::IDD, pParent)
	, m_bGPA0W(FALSE)
	, m_bGPA1W(FALSE)
	, m_bGPA2W(FALSE)
	, m_bGPA3W(FALSE)
	, m_bGPA4W(FALSE)
	, m_bGPA5W(FALSE)
	, m_bGPA6W(FALSE)
	, m_bGPA7W(FALSE)
	, m_bGPB0W(FALSE)
	, m_bGPB1W(FALSE)
	, m_bGPB2W(FALSE)
	, m_bGPB3W(FALSE)
	, m_bGPB4W(FALSE)
	, m_bGPB5W(FALSE)
	, m_bGPB6W(FALSE)
	, m_bGPB7W(FALSE)
	, m_bGPA0R(FALSE)
	, m_bGPA1R(FALSE)
	, m_bGPA2R(FALSE)
	, m_bGPA3R(FALSE)
	, m_bGPA4R(FALSE)
	, m_bGPA5R(FALSE)
	, m_bGPA6R(FALSE)
	, m_bGPA7R(FALSE)
	, m_bGPB0R(FALSE)
	, m_bGPB1R(FALSE)
	, m_bGPB2R(FALSE)
	, m_bGPB3R(FALSE)
	, m_bGPB4R(FALSE)
	, m_bGPB5R(FALSE)
	, m_bGPB6R(FALSE)
	, m_bGPB7R(FALSE)
	, m_bGPAW(FALSE)
	, m_bGPBW(FALSE)
	, m_strAddrW(_T(""))
	, m_strDataW(_T(""))
	, m_strAddrR(_T(""))
	, m_strDataR(_T(""))
{
	m_pSP1401 = NULL;
}

CMCP23S17Dlg::~CMCP23S17Dlg()
{
}

void CMCP23S17Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK1, m_bGPA0W);
	DDX_Check(pDX, IDC_CHECK2, m_bGPA1W);
	DDX_Check(pDX, IDC_CHECK5, m_bGPA2W);
	DDX_Check(pDX, IDC_CHECK6, m_bGPA3W);
	DDX_Check(pDX, IDC_CHECK7, m_bGPA4W);
	DDX_Check(pDX, IDC_CHECK8, m_bGPA5W);
	DDX_Check(pDX, IDC_CHECK9, m_bGPA6W);
	DDX_Check(pDX, IDC_CHECK10, m_bGPA7W);
	DDX_Check(pDX, IDC_CHECK3, m_bGPB0W);
	DDX_Check(pDX, IDC_CHECK4, m_bGPB1W);
	DDX_Check(pDX, IDC_CHECK25, m_bGPB2W);
	DDX_Check(pDX, IDC_CHECK27, m_bGPB3W);
	DDX_Check(pDX, IDC_CHECK28, m_bGPB4W);
	DDX_Check(pDX, IDC_CHECK29, m_bGPB5W);
	DDX_Check(pDX, IDC_CHECK30, m_bGPB6W);
	DDX_Check(pDX, IDC_CHECK31, m_bGPB7W);
	DDX_Check(pDX, IDC_CHECK34, m_bGPA0R);
	DDX_Check(pDX, IDC_CHECK13, m_bGPA1R);
	DDX_Check(pDX, IDC_CHECK33, m_bGPA2R);
	DDX_Check(pDX, IDC_CHECK21, m_bGPA3R);
	DDX_Check(pDX, IDC_CHECK14, m_bGPA4R);
	DDX_Check(pDX, IDC_CHECK23, m_bGPA5R);
	DDX_Check(pDX, IDC_CHECK24, m_bGPA6R);
	DDX_Check(pDX, IDC_CHECK42, m_bGPA7R);
	DDX_Check(pDX, IDC_CHECK32, m_bGPB0R);
	DDX_Check(pDX, IDC_CHECK77, m_bGPB1R);
	DDX_Check(pDX, IDC_CHECK78, m_bGPB2R);
	DDX_Check(pDX, IDC_CHECK79, m_bGPB3R);
	DDX_Check(pDX, IDC_CHECK65, m_bGPB4R);
	DDX_Check(pDX, IDC_CHECK80, m_bGPB5R);
	DDX_Check(pDX, IDC_CHECK81, m_bGPB6R);
	DDX_Check(pDX, IDC_CHECK66, m_bGPB7R);
	DDX_Check(pDX, IDC_CHECK11, m_bGPAW);
	DDX_Check(pDX, IDC_CHECK12, m_bGPBW);
	DDX_Text(pDX, IDC_EDIT1, m_strAddrW);
	DDX_Text(pDX, IDC_EDIT2, m_strDataW);
	DDX_Text(pDX, IDC_EDIT3, m_strAddrR);
	DDX_Text(pDX, IDC_EDIT4, m_strDataR);
}


BEGIN_MESSAGE_MAP(CMCP23S17Dlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CMCP23S17Dlg::OnBnClickedSetGPIO)
	ON_BN_CLICKED(IDC_BUTTON62, &CMCP23S17Dlg::OnBnClickedGetGPIO)
	ON_BN_CLICKED(IDC_CHECK11, &CMCP23S17Dlg::OnBnClickedEnAllGPA)
	ON_BN_CLICKED(IDC_CHECK12, &CMCP23S17Dlg::OnBnClickedEnAllGPB)
	ON_BN_CLICKED(IDC_BUTTON2, &CMCP23S17Dlg::OnBnClickedSPIWrite)
	ON_BN_CLICKED(IDC_BUTTON3, &CMCP23S17Dlg::OnBnClickedSPIRead)
END_MESSAGE_MAP()


void CMCP23S17Dlg::OnBnClickedSetGPIO()
{
	UpdateData(TRUE);
	uint32_t uiGPAData = (m_bGPA0W ? 1 : 0)		 |
						 (m_bGPA1W ? 1 : 0) << 1 |
						 (m_bGPA2W ? 1 : 0) << 2 |
						 (m_bGPA3W ? 1 : 0) << 3 |
						 (m_bGPA4W ? 1 : 0) << 4 |
						 (m_bGPA5W ? 1 : 0) << 5 |
						 (m_bGPA6W ? 1 : 0) << 6 |
						 (m_bGPA7W ? 1 : 0) << 7;
	uint32_t uiGPBData = (m_bGPB0W ? 1 : 0)		 |
						 (m_bGPB1W ? 1 : 0) << 1 |
						 (m_bGPB2W ? 1 : 0) << 2 |
						 (m_bGPB3W ? 1 : 0) << 3 |
						 (m_bGPB4W ? 1 : 0) << 4 |
						 (m_bGPB5W ? 1 : 0) << 5 |
						 (m_bGPB6W ? 1 : 0) << 6 |
						 (m_bGPB7W ? 1 : 0) << 7;
	STACHKV(m_pSP1401->SetMCP23S17Reg(0x12,uiGPAData));
	STACHKV(m_pSP1401->SetMCP23S17Reg(0x13,uiGPBData));
}

void CMCP23S17Dlg::OnBnClickedGetGPIO()
{
	uint32_t uiGPAData = 0,uiGPBData = 0;
	STACHKV(m_pSP1401->GetMCP23S17Reg(0x12,uiGPAData));
	STACHKV(m_pSP1401->GetMCP23S17Reg(0x13,uiGPBData));

	m_bGPA0R = uiGPAData & 0x01 ? TRUE : FALSE;
	m_bGPA1R = (uiGPAData & (0x01 << 1)) ? TRUE : FALSE;
	m_bGPA2R = (uiGPAData & (0x01 << 2)) ? TRUE : FALSE;
	m_bGPA3R = (uiGPAData & (0x01 << 3)) ? TRUE : FALSE;
	m_bGPA4R = (uiGPAData & (0x01 << 4)) ? TRUE : FALSE;
	m_bGPA5R = (uiGPAData & (0x01 << 5)) ? TRUE : FALSE;
	m_bGPA6R = (uiGPAData & (0x01 << 6)) ? TRUE : FALSE;
	m_bGPA7R = (uiGPAData & (0x01 << 7)) ? TRUE : FALSE;

	m_bGPB0R = uiGPBData & 0x01 ? TRUE : FALSE;
	m_bGPB1R = (uiGPBData & (0x01 << 1)) ? TRUE : FALSE;
	m_bGPB2R = (uiGPBData & (0x01 << 2)) ? TRUE : FALSE;
	m_bGPB3R = (uiGPBData & (0x01 << 3)) ? TRUE : FALSE;
	m_bGPB4R = (uiGPBData & (0x01 << 4)) ? TRUE : FALSE;
	m_bGPB5R = (uiGPBData & (0x01 << 5)) ? TRUE : FALSE;
	m_bGPB6R = (uiGPBData & (0x01 << 6)) ? TRUE : FALSE;
	m_bGPB7R = (uiGPBData & (0x01 << 7)) ? TRUE : FALSE;

	UpdateData(FALSE);
}

void CMCP23S17Dlg::OnBnClickedEnAllGPA()
{
	UpdateData(TRUE);
	m_bGPA0W = m_bGPAW;
	m_bGPA1W = m_bGPAW;
	m_bGPA2W = m_bGPAW;
	m_bGPA3W = m_bGPAW;
	m_bGPA4W = m_bGPAW;
	m_bGPA5W = m_bGPAW;
	m_bGPA6W = m_bGPAW;
	m_bGPA7W = m_bGPAW;
	UpdateData(FALSE);
}

void CMCP23S17Dlg::OnBnClickedEnAllGPB()
{
	UpdateData(TRUE);
	m_bGPB0W = m_bGPBW;
	m_bGPB1W = m_bGPBW;
	m_bGPB2W = m_bGPBW;
	m_bGPB3W = m_bGPBW;
	m_bGPB4W = m_bGPBW;
	m_bGPB5W = m_bGPBW;
	m_bGPB6W = m_bGPBW;
	m_bGPB7W = m_bGPBW;
	UpdateData(FALSE);
}

void CMCP23S17Dlg::OnBnClickedSPIWrite()
{
	UpdateData(TRUE);
	uint32_t uiAddr = strtol(m_strAddrW,NULL,16);
	uint32_t uiData = strtol(m_strDataW,NULL,2);
	STACHKV(m_pSP1401->SetMCP23S17Reg(uiAddr,uiData));
}

void CMCP23S17Dlg::OnBnClickedSPIRead()
{
	UpdateData(TRUE);
	uint32_t uiAddr = strtol(m_strAddrR,NULL,16);
	uint32_t uiData = 0;
	char szData[16] = {0};
	STACHKV(m_pSP1401->GetMCP23S17Reg(uiAddr,uiData));
	radixchg(uiData,szData,2);
	m_strDataR.Format("%8s",szData);
	UpdateData(FALSE);
}

BOOL CMCP23S17Dlg::Create(UINT nIDTemplate, CWnd* pParentWnd, CSP1401R1C *pSP1401)
{
	m_pSP1401 = pSP1401;
	return CDialog::Create(nIDTemplate,pParentWnd);
}

BOOL CMCP23S17Dlg::ShowWindow(int nCmdShow)
{
	SetWindowTextX();
	return CDialog::ShowWindow(nCmdShow);
}

void CMCP23S17Dlg::SetWindowTextX()
{
	CString strWndText;
	uint32_t uiRfuIdx = 0,uiRfIdx = 0;

	if (m_pSP1401) {
		uiRfIdx = m_pSP1401->GetRfIdx();
		uiRfuIdx = m_pSP1401->GetRfuIdx();
	}

	strWndText.Format("R1C_MCP23S17_RFU%d_RF%d",uiRfuIdx,uiRfIdx);
	SetWindowText(strWndText);
}

BOOL CMCP23S17Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	SetWindowTextX();
	return TRUE;
}

BOOL CMCP23S17Dlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
		return TRUE;
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
		return TRUE;
	else
		return CDialog::PreTranslateMessage(pMsg);
}