#include "stdafx.h"
#include "TxDlg.h"

IMPLEMENT_DYNAMIC(CTxDlg, CStretchDlg)
IMPLEMENT_WICO_STRETCH_DLG(CTxDlg)

CTxDlg::CTxDlg(CWnd* pParent /*=NULL*/)
	: CStretchDlg(CTxDlg::IDD, pParent)
	, m_dCF(2400.0)
{

}

CTxDlg::~CTxDlg()
{
}

void CTxDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CUSTOM1, m_Led[0]);
	DDX_Control(pDX, IDC_CUSTOM2, m_Led[1]);
	DDX_Control(pDX, IDC_CUSTOM3, m_Led[2]);
	DDX_Control(pDX, IDC_CUSTOM4, m_Led[3]);
	DDX_Control(pDX, IDC_CUSTOM5, m_Led[4]);
	DDX_Control(pDX, IDC_CUSTOM6, m_Led[5]);
	DDX_Control(pDX, IDC_CUSTOM7, m_Led[6]);
	DDX_Control(pDX, IDC_CUSTOM8, m_Led[7]);
	DDX_Control(pDX, IDC_EDIT17, m_Path[0]);
	DDX_Control(pDX, IDC_EDIT18, m_Path[1]);
	DDX_Control(pDX, IDC_EDIT19, m_Path[2]);
	DDX_Control(pDX, IDC_EDIT20, m_Path[3]);
	DDX_Control(pDX, IDC_EDIT21, m_Path[4]);
	DDX_Control(pDX, IDC_EDIT22, m_Path[5]);
	DDX_Control(pDX, IDC_EDIT23, m_Path[6]);
	DDX_Control(pDX, IDC_EDIT24, m_Path[7]);
	DDX_Text(pDX, IDC_EDIT1, m_dAmpOffset[0]);
	DDX_Text(pDX, IDC_EDIT2, m_dAmpOffset[1]);
	DDX_Text(pDX, IDC_EDIT3, m_dAmpOffset[2]);
	DDX_Text(pDX, IDC_EDIT4, m_dAmpOffset[3]);
	DDX_Text(pDX, IDC_EDIT5, m_dAmpOffset[4]);
	DDX_Text(pDX, IDC_EDIT6, m_dAmpOffset[5]);
	DDX_Text(pDX, IDC_EDIT7, m_dAmpOffset[6]);
	DDX_Text(pDX, IDC_EDIT8, m_dAmpOffset[7]);
	DDX_Control(pDX, IDC_EDIT1, m_AmpOff[0]);
	DDX_Control(pDX, IDC_EDIT2, m_AmpOff[1]);
	DDX_Control(pDX, IDC_EDIT3, m_AmpOff[2]);
	DDX_Control(pDX, IDC_EDIT4, m_AmpOff[3]);
	DDX_Control(pDX, IDC_EDIT5, m_AmpOff[4]);
	DDX_Control(pDX, IDC_EDIT6, m_AmpOff[5]);
	DDX_Control(pDX, IDC_EDIT7, m_AmpOff[6]);
	DDX_Control(pDX, IDC_EDIT8, m_AmpOff[7]);
	DDX_Text(pDX, IDC_EDIT9,  m_dPhase[0]);
	DDX_Text(pDX, IDC_EDIT10, m_dPhase[1]);
	DDX_Text(pDX, IDC_EDIT11, m_dPhase[2]);
	DDX_Text(pDX, IDC_EDIT12, m_dPhase[3]);
	DDX_Text(pDX, IDC_EDIT13, m_dPhase[4]);
	DDX_Text(pDX, IDC_EDIT14, m_dPhase[5]);
	DDX_Text(pDX, IDC_EDIT15, m_dPhase[6]);
	DDX_Text(pDX, IDC_EDIT16, m_dPhase[7]);
	DDX_Text(pDX, IDC_EDIT26, m_dCF);
}


BEGIN_MESSAGE_MAP(CTxDlg, CDialog)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON1, &CTxDlg::OnBnClickedSelArb0)
	ON_BN_CLICKED(IDC_BUTTON2, &CTxDlg::OnBnClickedSelArb1)
	ON_BN_CLICKED(IDC_BUTTON3, &CTxDlg::OnBnClickedSelArb2)
	ON_BN_CLICKED(IDC_BUTTON4, &CTxDlg::OnBnClickedSelArb3)
	ON_BN_CLICKED(IDC_BUTTON5, &CTxDlg::OnBnClickedSelArb4)
	ON_BN_CLICKED(IDC_BUTTON6, &CTxDlg::OnBnClickedSelArb5)
	ON_BN_CLICKED(IDC_BUTTON7, &CTxDlg::OnBnClickedSelArb6)
	ON_BN_CLICKED(IDC_BUTTON8, &CTxDlg::OnBnClickedSelArb7)
	ON_BN_CLICKED(IDC_CHECK1, &CTxDlg::OnBnClickedUseRF0)
	ON_BN_CLICKED(IDC_CHECK2, &CTxDlg::OnBnClickedUseRF2)
	ON_BN_CLICKED(IDC_CHECK3, &CTxDlg::OnBnClickedUseRF4)
	ON_BN_CLICKED(IDC_CHECK4, &CTxDlg::OnBnClickedUseRF6)
	ON_EN_CHANGE(IDC_EDIT17, &CTxDlg::OnEnChangeArb0)
	ON_EN_CHANGE(IDC_EDIT19, &CTxDlg::OnEnChangeArb2)
	ON_EN_CHANGE(IDC_EDIT21, &CTxDlg::OnEnChangeArb4)
	ON_EN_CHANGE(IDC_EDIT23, &CTxDlg::OnEnChangeArb6)
END_MESSAGE_MAP()


BOOL CTxDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	COLORREF clrLed = RGB(255,0,0);
	for (int32_t i = 0;i < ARRAY_SIZE(m_Led);i ++)
		m_Led[i].SetClr(clrLed);

	for (int32_t i = 0;i < ARRAY_SIZE(m_dAmpOffset);i ++)
		m_dAmpOffset[i] = 0.0;
	for (int32_t i = 0;i < ARRAY_SIZE(m_dPhase);i ++)
		m_dPhase[i] = 0.0;
	UpdateData(FALSE);

	CString strAmpOff = "";
	for (int32_t i = 0;i < ARRAY_SIZE(m_AmpOff);i ++) {
		strAmpOff.Format("%.2f",m_dAmpOffset[i]);
		m_AmpOff[i].SetWindowText(strAmpOff);
	}

	this->GetWindowRect(&m_crWnd);
	SetControlInfo(IDC_EDIT17,FixL_xStretch,1,0,1);
	SetControlInfo(IDC_EDIT18,FixL_xStretch,1,0,1);
	SetControlInfo(IDC_EDIT19,FixL_xStretch,1,0,1);
	SetControlInfo(IDC_EDIT20,FixL_xStretch,1,0,1);
	SetControlInfo(IDC_EDIT21,FixL_xStretch,1,0,1);
	SetControlInfo(IDC_EDIT22,FixL_xStretch,1,0,1);
	SetControlInfo(IDC_EDIT23,FixL_xStretch,1,0,1);
	SetControlInfo(IDC_EDIT24,FixL_xStretch,1,0,1);
	SetControlInfo(IDC_CHECK1,ChgL_Fixx,1,1,1);
	SetControlInfo(IDC_CHECK2,ChgL_Fixx,1,1,1);
	SetControlInfo(IDC_CHECK3,ChgL_Fixx,1,1,1);
	SetControlInfo(IDC_CHECK4,ChgL_Fixx,1,1,1);
	SetControlInfo(IDC_BUTTON1,ChgL_Fixx,1,1,1);
	SetControlInfo(IDC_BUTTON2,ChgL_Fixx,1,1,1);
	SetControlInfo(IDC_BUTTON3,ChgL_Fixx,1,1,1);
	SetControlInfo(IDC_BUTTON4,ChgL_Fixx,1,1,1);
	SetControlInfo(IDC_BUTTON5,ChgL_Fixx,1,1,1);
	SetControlInfo(IDC_BUTTON6,ChgL_Fixx,1,1,1);
	SetControlInfo(IDC_BUTTON7,ChgL_Fixx,1,1,1);
	SetControlInfo(IDC_BUTTON8,ChgL_Fixx,1,1,1);
	SetControlInfo(IDC_CHECK5,ChgL_Fixx,1,1,1);
	SetControlInfo(IDC_CHECK6,ChgL_Fixx,1,1,1);
	SetControlInfo(IDC_CHECK7,ChgL_Fixx,1,1,1);
	SetControlInfo(IDC_CHECK8,ChgL_Fixx,1,1,1);
	SetControlInfo(IDC_CHECK9,ChgL_Fixx,1,1,1);
	SetControlInfo(IDC_CHECK10,ChgL_Fixx,1,1,1);
	SetControlInfo(IDC_CHECK11,ChgL_Fixx,1,1,1);
	SetControlInfo(IDC_CHECK12,ChgL_Fixx,1,1,1);
	SetControlInfo(IDC_BUTTON9,ChgL_Fixx,1,1,1);
	SetControlInfo(IDC_BUTTON10,ChgL_Fixx,1,1,1);
	SetControlInfo(IDC_STATIC_EN,ChgL_Fixx,1,1,1);
	return TRUE;
}

void CTxDlg::OnBnClickedSelArb0()
{
	CFileDialog ArbFile(TRUE);
	if (ArbFile.DoModal() == IDOK)
		m_Path[0].SetWindowText(ArbFile.GetPathName());
}

void CTxDlg::OnBnClickedSelArb1()
{
	CFileDialog ArbFile(TRUE);
	if (ArbFile.DoModal() == IDOK)
		m_Path[1].SetWindowText(ArbFile.GetPathName());
}

void CTxDlg::OnBnClickedSelArb2()
{
	CFileDialog ArbFile(TRUE);
	if (ArbFile.DoModal() == IDOK)
		m_Path[2].SetWindowText(ArbFile.GetPathName());
}

void CTxDlg::OnBnClickedSelArb3()
{
	CFileDialog ArbFile(TRUE);
	if (ArbFile.DoModal() == IDOK)
		m_Path[3].SetWindowText(ArbFile.GetPathName());
}

void CTxDlg::OnBnClickedSelArb4()
{
	CFileDialog ArbFile(TRUE);
	if (ArbFile.DoModal() == IDOK)
		m_Path[4].SetWindowText(ArbFile.GetPathName());
}

void CTxDlg::OnBnClickedSelArb5()
{
	CFileDialog ArbFile(TRUE);
	if (ArbFile.DoModal() == IDOK)
		m_Path[5].SetWindowText(ArbFile.GetPathName());
}

void CTxDlg::OnBnClickedSelArb6()
{
	CFileDialog ArbFile(TRUE);
	if (ArbFile.DoModal() == IDOK)
		m_Path[6].SetWindowText(ArbFile.GetPathName());
}

void CTxDlg::OnBnClickedSelArb7()
{
	CFileDialog ArbFile(TRUE);
	if (ArbFile.DoModal() == IDOK)
		m_Path[7].SetWindowText(ArbFile.GetPathName());
}

void CTxDlg::OnBnClickedUseRF0()
{
	CString strPath = "";
	m_Path[0].GetWindowText(strPath);
	m_Path[1].SetWindowText(strPath);

	int iEn = ((CButton *)(GetDlgItem(IDC_CHECK1)))->GetCheck();
	BOOL bUseRf0 = (BST_CHECKED == iEn ? TRUE : FALSE);
	((CButton *)(GetDlgItem(IDC_BUTTON2)))->EnableWindow(!bUseRf0);
}

void CTxDlg::OnBnClickedUseRF2()
{
	CString strPath = "";
	m_Path[2].GetWindowText(strPath);
	m_Path[3].SetWindowText(strPath);

	int iEn = ((CButton *)(GetDlgItem(IDC_CHECK2)))->GetCheck();
	BOOL bUseRf2 = (BST_CHECKED == iEn ? TRUE : FALSE);
	((CButton *)(GetDlgItem(IDC_BUTTON4)))->EnableWindow(!bUseRf2);
}

void CTxDlg::OnBnClickedUseRF4()
{
	CString strPath = "";
	m_Path[4].GetWindowText(strPath);
	m_Path[5].SetWindowText(strPath);

	int iEn = ((CButton *)(GetDlgItem(IDC_CHECK3)))->GetCheck();
	BOOL bUseRf4 = (BST_CHECKED == iEn ? TRUE : FALSE);
	((CButton *)(GetDlgItem(IDC_BUTTON6)))->EnableWindow(!bUseRf4);
}

void CTxDlg::OnBnClickedUseRF6()
{
	CString strPath = "";
	m_Path[6].GetWindowText(strPath);
	m_Path[7].SetWindowText(strPath);

	int iEn = ((CButton *)(GetDlgItem(IDC_CHECK4)))->GetCheck();
	BOOL bUseRf6 = (BST_CHECKED == iEn ? TRUE : FALSE);
	((CButton *)(GetDlgItem(IDC_BUTTON8)))->EnableWindow(!bUseRf6);
}

void CTxDlg::OnEnChangeArb0()
{
	int iEn = ((CButton *)(GetDlgItem(IDC_CHECK1)))->GetCheck();
	BOOL bUseRf0 = (BST_CHECKED == iEn ? TRUE : FALSE);
	
	if (bUseRf0) {
		CString strPath = "";
		m_Path[0].GetWindowText(strPath);
		m_Path[1].SetWindowText(strPath);
	}
}

void CTxDlg::OnEnChangeArb2()
{
	int iEn = ((CButton *)(GetDlgItem(IDC_CHECK2)))->GetCheck();
	BOOL bUseRf2 = (BST_CHECKED == iEn ? TRUE : FALSE);

	if (bUseRf2) {
		CString strPath = "";
		m_Path[2].GetWindowText(strPath);
		m_Path[3].SetWindowText(strPath);
	}
}

void CTxDlg::OnEnChangeArb4()
{
	int iEn = ((CButton *)(GetDlgItem(IDC_CHECK3)))->GetCheck();
	BOOL bUseRf4 = (BST_CHECKED == iEn ? TRUE : FALSE);

	if (bUseRf4) {
		CString strPath = "";
		m_Path[4].GetWindowText(strPath);
		m_Path[5].SetWindowText(strPath);
	}
}

void CTxDlg::OnEnChangeArb6()
{
	int iEn = ((CButton *)(GetDlgItem(IDC_CHECK4)))->GetCheck();
	BOOL bUseRf6 = (BST_CHECKED == iEn ? TRUE : FALSE);

	if (bUseRf6) {
		CString strPath = "";
		m_Path[6].GetWindowText(strPath);
		m_Path[7].SetWindowText(strPath);
	}
}