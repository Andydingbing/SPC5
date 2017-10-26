#include "stdafx.h"
#include "RFU_Debug_Tool.h"
#include "FPGAdlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CFPGADlg, CStretchDlg)
IMPLEMENT_CSE_STRETCH_DLG(CFPGADlg)

CFPGADlg::CFPGADlg(CWnd* pParent /*=NULL*/)
	: CStretchDlg(CFPGADlg::IDD, pParent)
	, m_strK7_0_Offset(_T("0"))
	, m_strK7_1_Offset(_T("0"))
	, m_strS6_BAR0_Offset(_T("0"))
	, m_strK7_0_Data(_T("00000000"))
	, m_strK7_1_Data(_T("00000000"))
	, m_strS6_BAR0_Data(_T("00000000"))
	, m_strK7_0_Status(_T("00000000"))
	, m_strK7_1_Status(_T("00000000"))
	, m_strS6Status(_T("00000000"))
	, m_strS6_BAR1_Offset(_T("0"))
	, m_strS6_BAR1_Data(_T("00000000"))
	, m_strFPGABitStatus(_T(""))
	, m_strRegFileData(_T(""))
{

}

CFPGADlg::~CFPGADlg()
{
}

void CFPGADlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strK7_0_Offset);
	DDX_Text(pDX, IDC_EDIT12, m_strK7_1_Offset);
	DDX_Text(pDX, IDC_EDIT68, m_strS6_BAR0_Offset);
	DDX_Text(pDX, IDC_EDIT2, m_strK7_0_Data);
	DDX_Text(pDX, IDC_EDIT4, m_strK7_1_Data);
	DDX_Text(pDX, IDC_EDIT69, m_strS6_BAR0_Data);
	DDX_Text(pDX, IDC_EDIT3, m_strK7_0_Status);
	DDX_Text(pDX, IDC_EDIT67, m_strK7_1_Status);
	DDX_Text(pDX, IDC_EDIT70, m_strS6Status);
	DDX_Text(pDX, IDC_EDIT71, m_strS6_BAR1_Offset);
	DDX_Text(pDX, IDC_EDIT72, m_strS6_BAR1_Data);
	DDX_Control(pDX, IDC_EDIT6, m_BitFileK7_0);
	DDX_Control(pDX, IDC_EDIT8, m_BitFileK7_1);
	DDX_Text(pDX, IDC_EDIT78, m_strFPGABitStatus);
	DDX_Control(pDX, IDC_EDIT17, m_RegFile);
	DDX_Text(pDX, IDC_EDIT18, m_strRegFileData);
	DDX_Control(pDX, IDC_COMBO18, m_K7Sel);
}

BOOL CFPGADlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_K7Sel.SetCurSel(0);

	this->GetWindowRect(&m_crWnd);
	SetControlInfo(IDC_EDIT1,FixL_xStretch,3,0,1);
	SetControlInfo(IDC_EDIT2,FixL_xStretch,3,0,1);
	SetControlInfo(IDC_EDIT3,FixL_xStretch,3,0,1);
	SetControlInfo(IDC_BUTTON1,ChgL_Fixx,3,1,1);
	SetControlInfo(IDC_BUTTON2,ChgL_Fixx,3,1,1);
	SetControlInfo(IDC_STATIC_K7_0,FixL_xStretch,3,0,1);
	SetControlInfo(IDC_STATIC_K7_0_ACTION,ChgL_Fixx,3,1,1);

	SetControlInfo(IDC_EDIT12,FixL_xStretch,3,0,1);
	SetControlInfo(IDC_EDIT4,FixL_xStretch,3,0,1);
	SetControlInfo(IDC_EDIT67,FixL_xStretch,3,0,1);
	SetControlInfo(IDC_BUTTON3,ChgL_Fixx,3,1,1);
	SetControlInfo(IDC_BUTTON4,ChgL_Fixx,3,1,1);
	SetControlInfo(IDC_STATIC_K7_1,FixL_xStretch,3,0,1);
	SetControlInfo(IDC_STATIC_K7_1_ACTION,ChgL_Fixx,3,1,1);

	SetControlInfo(IDC_STATIC_S6_OFFSET_BAR0,ChgL_Fixx,3,1,1);
	SetControlInfo(IDC_STATIC_S6_DATA_BAR0,ChgL_Fixx,3,1,1);
	SetControlInfo(IDC_EDIT68,ChgL_xStretch,3,1,2);
	SetControlInfo(IDC_EDIT69,ChgL_xStretch,3,1,2);
	SetControlInfo(IDC_STATIC_S6_STATUS,ChgL_Fixx,3,1,1);
	SetControlInfo(IDC_EDIT70,ChgL_xStretch,3,1,3);
	SetControlInfo(IDC_BUTTON58,ChgL_Fixx,3,2,2);
	SetControlInfo(IDC_BUTTON10,ChgL_Fixx,3,2,2);
	SetControlInfo(IDC_STATIC_S6,ChgL_xStretch,3,1,3);
	SetControlInfo(IDC_STATIC_S6_ACTION_BAR0,ChgL_Fixx,3,2,2);

	SetControlInfo(IDC_STATIC_S6_OFFSET_BAR1,ChgL_Fixx,3,2,2);
	SetControlInfo(IDC_STATIC_S6_DATA_BAR1,ChgL_Fixx,3,2,2);
	SetControlInfo(IDC_EDIT71,ChgL_xStretch,3,2,3);
	SetControlInfo(IDC_EDIT72,ChgL_xStretch,3,2,3);
	SetControlInfo(IDC_BUTTON60,ChgL_Fixx,3,3,3);
	SetControlInfo(IDC_BUTTON27,ChgL_Fixx,3,3,3);
	SetControlInfo(IDC_STATIC_S6_ACTION_BAR1,ChgL_Fixx,3,3,3);

	SetControlInfo(IDC_STATIC_WRITE_FROM_FILE,ChgL_xStretch,3,1,2);
	SetControlInfo(IDC_EDIT17,ChgL_xStretch,3,1,2);
	SetControlInfo(IDC_BUTTON8,ChgL_Fixx,3,2,2);
	SetControlInfo(IDC_EDIT18,ChgL_xStretch,3,1,2);
	SetControlInfo(IDC_COMBO18,ChgL_xStretch,3,1,2);
	SetControlInfo(IDC_BUTTON20,ChgL_Fixx,3,2,2);

	SetControlInfo(IDC_STATIC_DOWNLOAD_BIT,ChgL_xStretch,3,2,3);
	SetControlInfo(IDC_EDIT6,ChgL_xStretch,3,2,3);
	SetControlInfo(IDC_BUTTON5,ChgL_Fixx,3,3,3);
	SetControlInfo(IDC_BUTTON12,ChgL_Fixx,3,3,3);
	SetControlInfo(IDC_EDIT8,ChgL_xStretch,3,2,3);
	SetControlInfo(IDC_BUTTON13,ChgL_Fixx,3,3,3);
	SetControlInfo(IDC_BUTTON59,ChgL_Fixx,3,3,3);
	SetControlInfo(IDC_STATIC_DOWNLOAD_BIT_STATUS,ChgL_Fixx,3,2,2);
	SetControlInfo(IDC_EDIT78,ChgL_xStretch,3,2,3);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}


BEGIN_MESSAGE_MAP(CFPGADlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CFPGADlg::OnBnClickedWriteK7_0)
	ON_BN_CLICKED(IDC_BUTTON2, &CFPGADlg::OnBnClickedReadK7_0)
	ON_BN_CLICKED(IDC_BUTTON3, &CFPGADlg::OnBnClickedWriteK7_1)
	ON_BN_CLICKED(IDC_BUTTON4, &CFPGADlg::OnBnClickedReadK7_1)
	ON_BN_CLICKED(IDC_BUTTON58, &CFPGADlg::OnBnClickedWriteS6_BAR0)
	ON_BN_CLICKED(IDC_BUTTON10, &CFPGADlg::OnBnClickedReadS6_BAR0)
	ON_BN_CLICKED(IDC_BUTTON60, &CFPGADlg::OnBnClickedWriteS6_BAR1)
	ON_BN_CLICKED(IDC_BUTTON27, &CFPGADlg::OnBnClickedReadS6_BAR1)
	ON_BN_CLICKED(IDC_BUTTON5, &CFPGADlg::OnBnClickedSelBitK7_0)
	ON_BN_CLICKED(IDC_BUTTON13, &CFPGADlg::OnBnClickedSelBitK7_1)
	ON_BN_CLICKED(IDC_BUTTON12, &CFPGADlg::OnBnClickedLoadBitK7_0)
	ON_BN_CLICKED(IDC_BUTTON59, &CFPGADlg::OnBnClickedLoadBitK7_1)
	ON_BN_CLICKED(IDC_BUTTON8, &CFPGADlg::OnBnClickedSelRegFile)
	ON_BN_CLICKED(IDC_BUTTON20, &CFPGADlg::OnBnClickedExecuteWriteFromFile)
	ON_WM_SIZE()
END_MESSAGE_MAP()


void CFPGADlg::OnBnClickedWriteK7_0()
{
	UpdateData(TRUE);
	int iStatus = 0;
	uint32_t uiOffset = strtol(m_strK7_0_Offset,NULL,16);
	uint32_t uiData = strtoul(m_strK7_0_Data,NULL,16);
	iStatus = m_K7_0->W32(viPCIDev::AS_BAR0,uiOffset << 2,uiData);
	Log.AddRegList(iStatus,m_K7_0->GetDevName(),uiOffset,uiData);
	if (iStatus)
		m_strK7_0_Status.Format("%#010X",iStatus);
	else
		m_strK7_0_Status.Format("%#08X",iStatus);
	UpdateData(FALSE);
}

void CFPGADlg::OnBnClickedReadK7_0()
{
	UpdateData(TRUE);
	int iStatus = 0;
	uint32_t uiOffset = strtol(m_strK7_0_Offset,NULL,16);
	uint32_t uiData = 0;
	iStatus = m_K7_0->R32(viPCIDev::AS_BAR0,uiOffset << 2,&uiData);
	m_strK7_0_Data.Format("%08X",uiData);
	Log.AddRegList(iStatus,m_K7_0->GetDevName(),uiOffset,-1,uiData);
	if (iStatus)
		m_strK7_0_Status.Format("%#010X",iStatus);
	else
		m_strK7_0_Status.Format("%#08X",iStatus);
	UpdateData(FALSE);
}

void CFPGADlg::OnBnClickedWriteK7_1()
{
	UpdateData(TRUE);
	int iStatus = 0;
	uint32_t uiOffset = strtol(m_strK7_1_Offset,NULL,16);
	uint32_t uiData = strtoul(m_strK7_1_Data,NULL,16);
	iStatus = m_K7_1->W32(viPCIDev::AS_BAR0,uiOffset << 2,uiData);
	Log.AddRegList(iStatus,m_K7_1->GetDevName(),uiOffset,uiData);
	if (iStatus)
		m_strK7_1_Status.Format("%#010X",iStatus);
	else
		m_strK7_1_Status.Format("%#08X",iStatus);
	UpdateData(FALSE);
}

void CFPGADlg::OnBnClickedReadK7_1()
{
	UpdateData(TRUE);
	int iStatus = 0;
	uint32_t uiOffset = strtol(m_strK7_1_Offset,NULL,16);
	uint32_t uiData = 0;
	iStatus = m_K7_1->R32(viPCIDev::AS_BAR0,uiOffset << 2,&uiData);
	m_strK7_1_Data.Format("%08X",uiData);
	Log.AddRegList(iStatus,m_K7_1->GetDevName(),uiOffset,-1,uiData);
	if (iStatus)
		m_strK7_1_Status.Format("%#010X",iStatus);
	else
		m_strK7_1_Status.Format("%#08X",iStatus);
	UpdateData(FALSE);
}

void CFPGADlg::OnBnClickedWriteS6_BAR0()
{
	UpdateData(TRUE);
	int iStatus = 0;
	uint32_t uiOffset = strtol(m_strS6_BAR0_Offset,NULL,16);
	uint32_t uiData = strtoul(m_strS6_BAR0_Data,NULL,16);
	iStatus = m_S6->W32(viPCIDev::AS_BAR0,uiOffset << 2,uiData);
	Log.AddRegList(iStatus,m_S6->GetDevName(),uiOffset,uiData);
	if (iStatus)
		m_strS6Status.Format("%#010X",iStatus);
	else
		m_strS6Status.Format("%#08X",iStatus);
	UpdateData(FALSE);
}

void CFPGADlg::OnBnClickedReadS6_BAR0()
{
	UpdateData(TRUE);
	int iStatus = 0;
	uint32_t uiOffset = strtol(m_strS6_BAR0_Offset,NULL,16);
	uint32_t uiData = 0;
	iStatus = m_S6->R32(viPCIDev::AS_BAR0,uiOffset << 2,&uiData);
	m_strS6_BAR0_Data.Format("%08X",uiData);
	Log.AddRegList(iStatus,m_S6->GetDevName(),uiOffset,-1,uiData);
	if (iStatus)
		m_strS6Status.Format("%#010X",iStatus);
	else
		m_strS6Status.Format("%#08X",iStatus);
	UpdateData(FALSE);
}

void CFPGADlg::OnBnClickedWriteS6_BAR1()
{
	UpdateData(TRUE);
	int iStatus = 0;
	uint32_t uiOffset = strtol(m_strS6_BAR1_Offset,NULL,16);
	uint32_t uiData = strtoul(m_strS6_BAR1_Data,NULL,16);
	iStatus = m_S6->W32(viPCIDev::AS_BAR0,uiOffset << 2,uiData);
	Log.AddRegList(iStatus,m_S6->GetDevName(),uiOffset,uiData);
	if (iStatus)
		m_strS6Status.Format("%#010X",iStatus);
	else
		m_strS6Status.Format("%#08X",iStatus);
	UpdateData(FALSE);
}

void CFPGADlg::OnBnClickedReadS6_BAR1()
{
	UpdateData(TRUE);
	int iStatus = 0;
	uint32_t uiOffset = strtol(m_strS6_BAR1_Offset,NULL,16);
	uint32_t uiData = 0;
	iStatus = m_S6->R32(viPCIDev::AS_BAR0,uiOffset << 2,&uiData);
	m_strS6_BAR1_Data.Format("%08X",uiData);
	Log.AddRegList(iStatus,m_S6->GetDevName(),uiOffset,-1,uiData);
	if (iStatus)
		m_strS6Status.Format("%#010X",iStatus);
	else
		m_strS6Status.Format("%#08X",iStatus);
	UpdateData(FALSE);
}

void CFPGADlg::OnBnClickedSelBitK7_0()
{
	UpdateData(TRUE);
	CFileDialog BitDlg(TRUE,_T("bit"),_T("*.bit"),OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("bit(*.bit)|*.bit|All Files (*.*)|*.*||"));
	if (BitDlg.DoModal() == IDOK)
		m_BitFileK7_0.SetWindowText(BitDlg.GetPathName());
}

void CFPGADlg::OnBnClickedSelBitK7_1()
{
	UpdateData(TRUE);
	CFileDialog BitDlg(TRUE,_T("bit"),_T("*.bit"),OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("bit(*.bit)|*.bit|All Files (*.*)|*.*||"));
	if (BitDlg.DoModal() == IDOK)
		m_BitFileK7_1.SetWindowText(BitDlg.GetPathName());
}

void CFPGADlg::OnBnClickedLoadBitK7_0()
{
	UpdateData(TRUE);
	CString strPath;
	m_BitFileK7_0.GetWindowText(strPath);
	if (m_pSP2401_2->SetFpgaBit((LPSTR)(LPCTSTR)strPath))
		m_strFPGABitStatus.Format("%s",Log.GetLastError());
	UpdateData(FALSE);
}

void CFPGADlg::OnBnClickedLoadBitK7_1()
{
	UpdateData(TRUE);
	CString strPath;
	m_BitFileK7_1.GetWindowText(strPath);
	if (m_pSP2401_0->SetFpgaBit((LPSTR)(LPCTSTR)strPath) < 0)
		m_strFPGABitStatus.Format("%s",Log.GetLastError());
	UpdateData(FALSE);
}

void CFPGADlg::OnBnClickedSelRegFile()
{
	m_strRegFileData = "";
	UpdateData(FALSE);
	char szOffsetBuf[16],szDataBuf[16];
	CString strBuf = "",strFileInfo = "";
	int iFileSize = 0,iRow = 0;

	CFileDialog RegFile(TRUE);
	if (RegFile.DoModal() == IDOK) {
		m_RegFile.SetWindowText(RegFile.GetPathName());
		UpdateData(FALSE);
	}
	else
		return;
	FILE *fp = fopen(RegFile.GetPathName(),"r");
	
	fseek(fp,0,SEEK_END);
	iFileSize = ftell(fp);
	iRow = (int)round((iFileSize / ( 7 + 11)),0);
	fseek(fp,0,SEEK_SET);

	for (int i = 0;i < iRow;i ++) {
		memset(szOffsetBuf,0,sizeof(szOffsetBuf));
		memset(szDataBuf,0,sizeof(szDataBuf));
		fread(szOffsetBuf,1,7,fp);
		fread(szDataBuf,1,11,fp);

		m_strRegFileData += szOffsetBuf;
		m_strRegFileData += "   ";
		m_strRegFileData += szDataBuf;
		m_strRegFileData += "\r\n";
	}
	fclose(fp);
	UpdateData(FALSE);
}

void CFPGADlg::OnBnClickedExecuteWriteFromFile()
{
	int32_t iStatus = 0;

	int iFileSize = 0,iRow = 0;

	viPCIDev *pFPGA;
	if (0 == m_K7Sel.GetCurSel())
		pFPGA = m_K7_0;
	else if (1 == m_K7Sel.GetCurSel())
		pFPGA = m_K7_1;
	else
		pFPGA = m_S6;

	char szOffsetBuf[16],szDataBuf[16];
	uint32_t uiOffset = 0;
	uint32_t uiData = 0;

	CString strPath;
	m_RegFile.GetWindowText(strPath);

	FILE *fp = fopen(strPath,"r");;

	fseek(fp,0,SEEK_END);
	iFileSize = ftell(fp);
	iRow = (int)round((iFileSize / ( 7 + 11)),0);
	fseek(fp,0,SEEK_SET);

	for (int i = 0;i < iRow;i ++) {
		memset(szOffsetBuf,0,sizeof(szOffsetBuf));
		memset(szDataBuf,0,sizeof(szDataBuf));
		fread(szOffsetBuf,1,7,fp);
		fread(szDataBuf,1,11,fp);

		uiOffset = strtol(szOffsetBuf,NULL,16);
		uiData = strtoul(szDataBuf,NULL,16);
		if ((iStatus = pFPGA->W32(viPCIDev::AS_BAR0,uiOffset << 2,uiData)) < 0)
			return;
		Log.AddRegList(iStatus,pFPGA->GetDevName(),uiOffset,uiData);
	}
	fclose(fp);
}
