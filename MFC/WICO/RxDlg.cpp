#include "stdafx.h"
#include "RxDlg.h"

IMPLEMENT_DYNAMIC(CRxDlg, CStretchDlg)
IMPLEMENT_WICO_STRETCH_DLG(CRxDlg)

CRxDlg::CRxDlg(CWnd* pParent /*=NULL*/)
	: CStretchDlg(CRxDlg::IDD, pParent)
	, m_dCF(2400.0)
{

}

CRxDlg::~CRxDlg()
{
}

void CRxDlg::DoDataExchange(CDataExchange* pDX)
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
	DDX_Text(pDX, IDC_EDIT1, m_dRefOffset[0]);
	DDX_Text(pDX, IDC_EDIT2, m_dRefOffset[1]);
	DDX_Text(pDX, IDC_EDIT3, m_dRefOffset[2]);
	DDX_Text(pDX, IDC_EDIT4, m_dRefOffset[3]);
	DDX_Text(pDX, IDC_EDIT5, m_dRefOffset[4]);
	DDX_Text(pDX, IDC_EDIT6, m_dRefOffset[5]);
	DDX_Text(pDX, IDC_EDIT7, m_dRefOffset[6]);
	DDX_Text(pDX, IDC_EDIT8, m_dRefOffset[7]);
	DDX_Control(pDX, IDC_EDIT1, m_RefOff[0]);
	DDX_Control(pDX, IDC_EDIT2, m_RefOff[1]);
	DDX_Control(pDX, IDC_EDIT3, m_RefOff[2]);
	DDX_Control(pDX, IDC_EDIT4, m_RefOff[3]);
	DDX_Control(pDX, IDC_EDIT5, m_RefOff[4]);
	DDX_Control(pDX, IDC_EDIT6, m_RefOff[5]);
	DDX_Control(pDX, IDC_EDIT7, m_RefOff[6]);
	DDX_Control(pDX, IDC_EDIT8, m_RefOff[7]);
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


BEGIN_MESSAGE_MAP(CRxDlg, CDialog)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON1, &CRxDlg::OnBnClickedSelIQ0)
	ON_BN_CLICKED(IDC_BUTTON2, &CRxDlg::OnBnClickedSelIQ1)
	ON_BN_CLICKED(IDC_BUTTON3, &CRxDlg::OnBnClickedSelIQ2)
	ON_BN_CLICKED(IDC_BUTTON4, &CRxDlg::OnBnClickedSelIQ3)
	ON_BN_CLICKED(IDC_BUTTON5, &CRxDlg::OnBnClickedSelIQ4)
	ON_BN_CLICKED(IDC_BUTTON6, &CRxDlg::OnBnClickedSelIQ5)
	ON_BN_CLICKED(IDC_BUTTON7, &CRxDlg::OnBnClickedSelIQ6)
	ON_BN_CLICKED(IDC_BUTTON8, &CRxDlg::OnBnClickedSelIQ7)
END_MESSAGE_MAP()


BOOL CRxDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	COLORREF clrLed = RGB(255,0,0);
	for (int32_t i = 0;i < ARRAY_SIZE(m_Led);i ++)
		m_Led[i].SetClr(clrLed);

	for (int32_t i = 0;i < ARRAY_SIZE(m_dRefOffset);i ++)
		m_dRefOffset[i] = 0.0;
	for (int32_t i = 0;i < ARRAY_SIZE(m_dPhase);i ++)
		m_dPhase[i] = 0.0;
	UpdateData(FALSE);

	CString strRefOff = "";
	for (int32_t i = 0;i < ARRAY_SIZE(m_RefOff);i ++) {
		strRefOff.Format("%.2f",m_dRefOffset[i]);
		m_RefOff[i].SetWindowText(strRefOff);
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

void CRxDlg::OnBnClickedSelIQ0()
{
	CFileDialog IQFile(TRUE);
	if (IQFile.DoModal() == IDOK)
		m_Path[0].SetWindowText(IQFile.GetPathName());
}

void CRxDlg::OnBnClickedSelIQ1()
{
	CFileDialog IQFile(TRUE);
	if (IQFile.DoModal() == IDOK)
		m_Path[1].SetWindowText(IQFile.GetPathName());
}

void CRxDlg::OnBnClickedSelIQ2()
{
	CFileDialog IQFile(TRUE);
	if (IQFile.DoModal() == IDOK)
		m_Path[2].SetWindowText(IQFile.GetPathName());
}

void CRxDlg::OnBnClickedSelIQ3()
{
	CFileDialog IQFile(TRUE);
	if (IQFile.DoModal() == IDOK)
		m_Path[3].SetWindowText(IQFile.GetPathName());
}

void CRxDlg::OnBnClickedSelIQ4()
{
	CFileDialog IQFile(TRUE);
	if (IQFile.DoModal() == IDOK)
		m_Path[4].SetWindowText(IQFile.GetPathName());
}

void CRxDlg::OnBnClickedSelIQ5()
{
	CFileDialog IQFile(TRUE);
	if (IQFile.DoModal() == IDOK)
		m_Path[5].SetWindowText(IQFile.GetPathName());
}

void CRxDlg::OnBnClickedSelIQ6()
{
	CFileDialog IQFile(TRUE);
	if (IQFile.DoModal() == IDOK)
		m_Path[6].SetWindowText(IQFile.GetPathName());
}

void CRxDlg::OnBnClickedSelIQ7()
{
	CFileDialog IQFile(TRUE);
	if (IQFile.DoModal() == IDOK)
		m_Path[7].SetWindowText(IQFile.GetPathName());
}
