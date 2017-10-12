#include "stdafx.h"
#include "RFU_Debug_Tool.h"
#include "AT24C256BDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CAT24C256BDlg, CDialog)

CAT24C256BDlg::CAT24C256BDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAT24C256BDlg::IDD, pParent)
	, m_strAddrW(_T("0000"))
	, m_strAddrR(_T("0000"))
	, m_uiLengthW(20)
	, m_uiLengthR(20)
	, m_strDataW(_T("You are my sunshine"))
	, m_strDataR(_T(""))
{
	m_pSP1401 = NULL;
}

CAT24C256BDlg::~CAT24C256BDlg()
{
}

BOOL CAT24C256BDlg::Create(UINT nIDTemplate, CWnd* pParentWnd, ISP1401 *pSP1401)
{
	m_pSP1401 = pSP1401;
	return CDialog::Create(nIDTemplate,pParentWnd);
}

void CAT24C256BDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strAddrW);
	DDX_Text(pDX, IDC_EDIT12, m_strAddrR);
	DDX_Text(pDX, IDC_EDIT2, m_uiLengthW);
	DDX_Text(pDX, IDC_EDIT4, m_uiLengthR);
	DDX_Text(pDX, IDC_EDIT3, m_strDataW);
	DDX_Text(pDX, IDC_EDIT26, m_strDataR);
}


BEGIN_MESSAGE_MAP(CAT24C256BDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CAT24C256BDlg::OnBnClickedWrite)
	ON_BN_CLICKED(IDC_BUTTON2, &CAT24C256BDlg::OnBnClickedRead)
END_MESSAGE_MAP()


void CAT24C256BDlg::OnBnClickedWrite()
{
	UpdateData(TRUE);
	uint16_t uiAddr = (uint16_t)strtol((LPSTR)(LPCTSTR)m_strAddrW,NULL,16);
	uint32_t uiLength = uint32_t(m_uiLengthW);
	STACHKV(m_pSP1401->WriteEEPROM(uiAddr,uiLength,(LPSTR)(LPCTSTR)m_strDataW));
	SetFocus();
	m_uiLengthR = m_uiLengthW;
	UpdateData(FALSE);
}

void CAT24C256BDlg::OnBnClickedRead()
{
	UpdateData(TRUE);
	uint16_t uiAddr = (uint16_t)strtol((LPSTR)(LPCTSTR)m_strAddrR,NULL,16);
	uint32_t uiLength = (uint32_t)m_uiLengthR;
	char *pBuf = new char[uiLength + 1];
	memset(pBuf,0,uiLength + 1);
	if (m_pSP1401->ReadEEPROM(uiAddr,uiLength,pBuf) < 0) {
		delete []pBuf;
		return;
	}
	m_strDataR.Format("%s",pBuf);
	SetFocus();
	delete []pBuf;
	UpdateData(FALSE);
}

BOOL CAT24C256BDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
		return TRUE;
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
		return TRUE;
	else
		return CDialog::PreTranslateMessage(pMsg);
}

void CAT24C256BDlg::OnCancel()
{
	ShowWindow(SW_HIDE);
	/*CDialog::OnCancel();*/
}

BOOL CAT24C256BDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	SetWindowTextX();
	return TRUE;
}

void CAT24C256BDlg::SetWindowTextX()
{
	CString strWndText;
	uint32_t uiRfuIdx = 0,uiRfIdx = 0;

	if (m_pSP1401) {
		uiRfIdx = m_pSP1401->GetRfIdx();
		uiRfuIdx = m_pSP1401->GetRfuIdx();
	}

	strWndText.Format("R1A/B/C _AT24C256B_RFU%d_RF%d",uiRfuIdx,uiRfIdx);
	SetWindowText(strWndText);
}

BOOL CAT24C256BDlg::ShowWindow(int nCmdShow)
{
	SetWindowTextX();
	return CDialog::ShowWindow(nCmdShow);
}