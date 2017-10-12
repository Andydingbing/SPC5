#include "stdafx.h"
#include "RFU_Debug_Tool.h"
#include "TestCMPBDlg.h"
#include "TestCMPBThread.h"
#include "RegDef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CTestCMPBDlg, CAttachThreadDlg)
IMPLEMENT_CSE_ATTACHTHREAD_DLG(CTestCMPBDlg)

CTestCMPBDlg::CTestCMPBDlg(CWnd* pParent /*=NULL*/)
	: CAttachThreadDlg(CTestCMPBDlg::IDD, pParent)
	, m_uiRepeatTimes(1)
	, m_uiOneBlockSize(500)
	, m_uiDelayms(0)
{

}

CTestCMPBDlg::~CTestCMPBDlg()
{
}

void CTestCMPBDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_uiRepeatTimes);
	DDX_Control(pDX, IDC_LIST_DEVICE, m_DevList);
	DDX_Text(pDX, IDC_EDIT2, m_uiOneBlockSize);
	DDX_Text(pDX, IDC_EDIT3, m_uiDelayms);
}


BEGIN_MESSAGE_MAP(CTestCMPBDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CTestCMPBDlg::OnBnClickedReScanDevice)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_DEVICE, &CTestCMPBDlg::OnNMDblclkListDevice)
	ON_BN_CLICKED(IDC_BUTTON2, &CTestCMPBDlg::OnBnClickedStart)
	ON_MESSAGE(WM_CSE_SINGLE_THREAD_DONE, &CTestCMPBDlg::Done)
	ON_MESSAGE(WM_CSE_TEST_CMPB_SHOW, &CTestCMPBDlg::ShowData)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON3, &CTestCMPBDlg::OnBnClickedStop)
END_MESSAGE_MAP()


BOOL CTestCMPBDlg::OnInitDialog()
{
	CAttachThreadDlg::OnInitDialog();

	DWORD dwStyleDevList;
	dwStyleDevList = m_DevList.GetStyle();
	dwStyleDevList |= LVS_EX_GRIDLINES;
	dwStyleDevList |= LVS_EX_FULLROWSELECT;
	dwStyleDevList |= LVS_EX_CHECKBOXES;
	dwStyleDevList &= ~LVS_EX_AUTOSIZECOLUMNS;
	m_DevList.SetExtendedStyle(dwStyleDevList);
	m_DevList.InsertColumn(0,_T("Device"),LVCFMT_LEFT,180);
	m_DevList.InsertColumn(1,_T("VID"),LVCFMT_LEFT,50);
	m_DevList.InsertColumn(2,_T("DID"),LVCFMT_LEFT,50);
	m_DevList.InsertColumn(3,_T("Version"),LVCFMT_LEFT,80);
	m_DevList.InsertColumn(4,_T("Total"),LVCFMT_LEFT,60);
	m_DevList.InsertColumn(5,_T("Error"),LVCFMT_LEFT,60);
	m_DevList.InsertColumn(6,_T(".bit"),LVCFMT_LEFT,400);

	this->GetWindowRect(&m_crWnd);
	SetControlInfo(IDC_EDIT1,FixL_xStretch,1,0,1);
	SetControlInfo(IDC_BUTTON3,ChgL_Fixx,1,1,1);
	SetControlInfo(IDC_LIST_DEVICE,FixL_xyStretch,1,0,1,1,0,1);
	SetControlInfo(IDC_BUTTON2,ChgL_Fixx,1,1,1);
	AddThreadIdleCtrl(IDC_EDIT1);
	AddThreadIdleCtrl(IDC_BUTTON1);
	AddThreadIdleCtrl(IDC_EDIT2);
	AddThreadIdleCtrl(IDC_EDIT3);
	AddThreadIdleCtrl(IDC_BUTTON2);
	AddThreadRunningCtrl(IDC_BUTTON3);
	CtrlResume();
	return TRUE;
}

void CTestCMPBDlg::OnBnClickedReScanDevice()
{
	m_DevList.DeleteAllItems();
	viPCIDev PCIDev;
	vector<string> Devs;
	uint32_t uiID = 0,uiVer = 0;
	CString strVID,strDID,strVer;
	PCIDev.GetDevices(Devs);

	CGeneralIniFile IniFile(CONFIG_FILE_PATH);
	char szProductForm[64];
	char szRsrcSection[64];
	char szDevKey[64];
	char szRsrcNameS6[64];
	char szRsrcNameK7[64];
	char szItemName[64];

	memset(szProductForm,0,ARRAY_SIZE(szProductForm));
	memset(szRsrcSection,0,ARRAY_SIZE(szRsrcSection));
	memset(szDevKey,0,ARRAY_SIZE(szDevKey));
	memset(szRsrcNameS6,0,ARRAY_SIZE(szRsrcNameS6));
	memset(szRsrcNameK7,0,ARRAY_SIZE(szRsrcNameK7));
	memset(szItemName,0,ARRAY_SIZE(szItemName));
	IniFile.GetConfigStringValue("Product Form","Form",szProductForm);
	strcpy(szRsrcSection,"RESOURCE");
	strcat(szRsrcSection,szProductForm);
	for (int32_t i = 0;i < MAX_RFU;i ++) {
		sprintf(szDevKey,"RFU%d_S6",i);
		if (!IniFile.GetConfigStringValue(szRsrcSection,szDevKey,szRsrcNameS6)) {						//rfu s6 exists in ini file
			for (int32_t j = 0;j < Devs.size();j ++) {
				if (0 == strcmp(szRsrcNameS6,Devs.at(j).c_str())) {						//rfu s6 already connected
					for (int32_t m = 0;m < 2;m ++) {
						sprintf(szDevKey,"RFU%d_K7_%d",i,m);
						IniFile.GetConfigStringValue(szRsrcSection,szDevKey,szRsrcNameK7);
						for (int32_t k = 0;k < Devs.size();k ++) {
							if (0 == strcmp(szRsrcNameK7,Devs.at(k).c_str())) {			//rfuk7 already connected
								sprintf(szItemName,"RFU%d_K7_%d(%s)",i,m,szRsrcNameK7);
								int iItem = m_DevList.GetItemCount();
								m_DevList.InsertItem(iItem,szItemName);
								m_DevList.SetItemColor(iItem,BLUE);
								STACHKV(PCIDev.Init(Devs.at(k)));
								STACHKV(PCIDev.R32(viPCIDev::AS_CFG,0,&uiID));
								STACHKV(PCIDev.R32(viPCIDev::AS_BAR0,0,&uiVer));
								STACHKV(PCIDev.Close());
								strVID.Format("%#6x",uiID & 0x0000ffff);
								strDID.Format("%#6x",(uiID & 0xffff0000) >> 16);
								strVer.Format("%#010x",uiVer);
								m_DevList.SetItemText(iItem,1,strVID);
								m_DevList.SetItemText(iItem,2,strDID);
								m_DevList.SetItemText(iItem,3,strVer);
								m_DevList.SetCheck(iItem,TRUE);
							}
						}
					}
				}
			}
		}
	}
	for (int32_t i = 0;i < MAX_CEU;i ++) {
		sprintf(szDevKey,"CEU%d_S6",i);
		if (!IniFile.GetConfigStringValue(szRsrcSection,szDevKey,szRsrcNameS6)) {						//ceu s6 exists in ini file
			for (int32_t j = 0;j < Devs.size();j ++) {
				if (0 == strcmp(szRsrcNameS6,Devs.at(j).c_str())) {						//ceu s6 already connected
					for (int32_t m = 0;m < 4;m ++) {
						sprintf(szDevKey,"CEU%d_K7_%d",i,m);
						IniFile.GetConfigStringValue(szRsrcSection,szDevKey,szRsrcNameK7);
						for (int32_t k = 0;k < Devs.size();k ++) {
							if (0 == strcmp(szRsrcNameK7,Devs.at(k).c_str())) {			//ceu k7 already connected
								sprintf(szItemName,"CEU%d_K7_%d(%s)",i,m,szRsrcNameK7);
								int iItem = m_DevList.GetItemCount();
								m_DevList.InsertItem(iItem,szItemName);
								m_DevList.SetItemColor(iItem,BROWN);
								STACHKV(PCIDev.Init(Devs.at(k)));
								STACHKV(PCIDev.R32(viPCIDev::AS_CFG,0,&uiID));
								STACHKV(PCIDev.R32(viPCIDev::AS_BAR0,0,&uiVer));
								STACHKV(PCIDev.Close());
								strVID.Format("%#6x",uiID & 0x0000ffff);
								strDID.Format("%#6x",(uiID & 0xffff0000) >> 16);
								strVer.Format("%#010x",uiVer);
								m_DevList.SetItemText(iItem,1,strVID);
								m_DevList.SetItemText(iItem,2,strDID);
								m_DevList.SetItemText(iItem,3,strVer);
								m_DevList.SetCheck(iItem,TRUE);
							}
						}
					}
				}
			}
		}
	}
	sprintf(szDevKey,"CXU_S6");
	if (!IniFile.GetConfigStringValue(szRsrcSection,szDevKey,szRsrcNameS6)) {					//cxu s6 exists in ini file
		for (int32_t j = 0;j < Devs.size();j ++) {
			if (0 == strcmp(szRsrcNameS6,Devs.at(j).c_str())) {					//cxu s6 already connected
				sprintf(szDevKey,"CXU_V7");
				IniFile.GetConfigStringValue(szRsrcSection,szDevKey,szRsrcNameK7);
				for (int32_t k = 0;k < Devs.size();k ++) {
					if (0 == strcmp(szRsrcNameK7,Devs.at(k).c_str())) {			//cxu v7 already connected
						sprintf(szItemName,"CXU_V7(%s)",szRsrcNameK7);
						int iItem = m_DevList.GetItemCount();
						m_DevList.InsertItem(iItem,szItemName);
						m_DevList.SetItemColor(iItem,BLACK);
						STACHKV(PCIDev.Init(Devs.at(k)));
						STACHKV(PCIDev.R32(viPCIDev::AS_CFG,0,&uiID));
						STACHKV(PCIDev.R32(viPCIDev::AS_BAR0,0,&uiVer));
						STACHKV(PCIDev.Close());
						strVID.Format("%#6x",uiID & 0x0000ffff);
						strDID.Format("%#6x",(uiID & 0xffff0000) >> 16);
						strVer.Format("%#010x",uiVer);
						m_DevList.SetItemText(iItem,1,strVID);
						m_DevList.SetItemText(iItem,2,strDID);
						m_DevList.SetItemText(iItem,3,strVer);
						m_DevList.SetCheck(iItem,TRUE);
					}
				}
			}
		}
	}
}

void CTestCMPBDlg::OnNMDblclkListDevice(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	if (pNMItemActivate->iItem != -1) {
		CFileDialog BitDlg(TRUE,_T("bit"),_T("*.bit"),OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("bit(*.bit)|*.bit|All Files (*.*)|*.*||"));
		if (BitDlg.DoModal() == IDOK)
			m_DevList.SetItemText(pNMItemActivate->iItem,6,BitDlg.GetPathName());
	}
	*pResult = 0;
}

void CTestCMPBDlg::OnBnClickedStart()
{
	TEST_THREAD_EXISTED_CHKV();
	CtrlPrepare();
	UpdateData(TRUE);

	for (int i = 0;i < m_DevList.GetItemCount();i ++) {
		if (m_DevList.GetCheck(i)) {
			m_DevList.SetItemText(i,3,_T("0x00000000"));
			m_DevList.SetItemText(i,4,_T("0"));
			m_DevList.SetItemText(i,5,_T("0"));
		}
	}

	CTestCMPBThread::Info Info;

	int iItem = 0;
	viPCIDev PCIDev;
	CStringArray Devs;
	uint32_t uiID = 0,uiVer = 0;
	CString strVID,strDID,strVer;

	vector<string> strDevs;
	PCIDev.GetDevices(strDevs);
	for (int32_t i = 0;i < strDevs.size();i ++) {
		CString strDevice = "";
		strDevice.Format("%s",strDevs.at(i));
		Devs.Add(strDevice);
	}

	CGeneralIniFile IniFile(CONFIG_FILE_PATH);
	char szProductForm[64];
	char szRsrcSection[64];
	char szDevKey[64];
	char szRsrcNameS6[64];
	char szRsrcNameK7[64];
	char szItemName[64];

	memset(szProductForm,0,ARRAY_SIZE(szProductForm));
	memset(szRsrcSection,0,ARRAY_SIZE(szRsrcSection));
	memset(szDevKey,0,ARRAY_SIZE(szDevKey));
	memset(szRsrcNameS6,0,ARRAY_SIZE(szRsrcNameS6));
	memset(szRsrcNameK7,0,ARRAY_SIZE(szRsrcNameK7));
	memset(szItemName,0,ARRAY_SIZE(szItemName));
	IniFile.GetConfigStringValue("Product Form","Form",szProductForm);
	strcpy(szRsrcSection,"RESOURCE");
	strcat(szRsrcSection,szProductForm);

	Info.m_uiTimes = m_uiRepeatTimes;
	Info.m_uiDelayms = this->m_uiDelayms;
	Info.m_uiOneBlockSize = this->m_uiOneBlockSize;
	for (int32_t i = 0;i < MAX_RFU;i ++) {
		sprintf(szDevKey,"RFU%d_S6",i);
		if (!IniFile.GetConfigStringValue(szRsrcSection,szDevKey,szRsrcNameS6)) {					//rfu s6 exists in ini file
			for (int32_t j = 0;j < Devs.GetSize();j ++) {
				if (0 == strcmp(szRsrcNameS6,(LPSTR)(LPCTSTR)Devs.ElementAt(j))) {					//rfu s6 already connected
					CTestCMPBThread::Info::Rfu RfuInfo;
					RfuInfo.m_strS6Rsrc.Format("%s",szRsrcNameS6);
					RfuInfo.m_uiIdx = (uint32_t)i;
					for (int32_t m = 0;m < 2;m ++) {
						sprintf(szDevKey,"RFU%d_K7_%d",i,m);
						IniFile.GetConfigStringValue(szRsrcSection,szDevKey,szRsrcNameK7);
						for (int32_t k = 0;k < Devs.GetSize();k ++) {
							if (0 == strcmp(szRsrcNameK7,(LPSTR)(LPCTSTR)Devs.ElementAt(k))) {		//rfu k7 already connected
								RfuInfo.m_bConnect[m] = TRUE;
								RfuInfo.m_bProgram[m] = m_DevList.GetCheck(iItem);
								RfuInfo.m_strPath[m] = m_DevList.GetItemText(iItem,6);
								iItem ++;
							}
						}
					}
					Info.m_Rfu.push_back(RfuInfo);
				}
			}
		}
	}
	for (int32_t i = 0;i < MAX_CEU;i ++) {
		sprintf(szDevKey,"CEU%d_S6",i);
		if (!IniFile.GetConfigStringValue(szRsrcSection,szDevKey,szRsrcNameS6)) {					//ceu s6 exists in ini file
			for (int32_t j = 0;j < Devs.GetSize();j ++) {
				if (0 == strcmp(szRsrcNameS6,(LPSTR)(LPCTSTR)Devs.ElementAt(j))) {					//ceu s6 already connected
					CTestCMPBThread::Info::Ceu CeuInfo;
					CeuInfo.m_strS6Rsrc.Format("%s",szRsrcNameS6);
					CeuInfo.m_uiIdx = (uint32_t)i;
					for (int32_t m = 0;m < 4;m ++) {
						sprintf(szDevKey,"CEU%d_K7_%d",i,m);
						IniFile.GetConfigStringValue(szRsrcSection,szDevKey,szRsrcNameK7);
						for (int32_t k = 0;k < Devs.GetSize();k ++) {
							if (0 == strcmp(szRsrcNameK7,(LPSTR)(LPCTSTR)Devs.ElementAt(k))) {		//ceu k7 already connected
								CeuInfo.m_bConnect[m] = TRUE;
								CeuInfo.m_bProgram[m] = m_DevList.GetCheck(iItem);
								CeuInfo.m_strPath[m] = m_DevList.GetItemText(iItem,6);
								iItem ++;
							}
						}
					}
					Info.m_Ceu.push_back(CeuInfo);
				}
			}
		}
	}
	sprintf(szDevKey,"CXU_S6");
	if (!IniFile.GetConfigStringValue(szRsrcSection,szDevKey,szRsrcNameS6)) {					//cxu s6 exists in ini file
		for (int32_t j = 0;j < Devs.GetSize();j ++) {
			if (0 == strcmp(szRsrcNameS6,(LPSTR)(LPCTSTR)Devs.ElementAt(j))) {					//cxu s6 already connected
				CTestCMPBThread::Info::Cxu CxuInfo;
				CxuInfo.m_strS6Rsrc.Format("%s",szRsrcNameS6);
				sprintf(szDevKey,"CXU_K7");
				IniFile.GetConfigStringValue(szRsrcSection,szDevKey,szRsrcNameK7);
				for (int32_t k = 0;k < Devs.GetSize();k ++) {
					if (0 == strcmp(szRsrcNameK7,(LPSTR)(LPCTSTR)Devs.ElementAt(k))) {			//cxu v7 already connected
						CxuInfo.m_bConnect = TRUE;
						CxuInfo.m_bProgram = m_DevList.GetCheck(iItem);
						CxuInfo.m_strPath = m_DevList.GetItemText(iItem,6);
						iItem ++;
					}
				}
				Info.m_Cxu.push_back(CxuInfo);
			}
		}
	}

	m_pThread = new CTestCMPBThread(this,&Info);
	m_pThread->CreateThread();
}

LRESULT CTestCMPBDlg::ShowData(WPARAM wParam,LPARAM lParam)
{
	int iItem = 0;
	CTestCMPBThread::Result Data = *(CTestCMPBThread::Result *)lParam;
	CString strVer,strTimes,strFails;
	for (int32_t i = 0;i < Data.m_Rfu.size();i ++) {
		for (int32_t j = 0;j < 2;j ++) {
			if (!Data.m_Rfu.at(i).m_bConnect[j])
				continue;
			if (m_DevList.GetCheck(iItem)) {
				strVer.Format("%#010x",Data.m_Rfu.at(i).m_uiVer[j]);
				strTimes.Format("%u",Data.m_Rfu.at(i).m_uiTimes[j]);
				strFails.Format("%u",Data.m_Rfu.at(i).m_uiFails[j]);
				m_DevList.SetItemText(iItem,3,strVer);
				m_DevList.SetItemText(iItem,4,strTimes);
				m_DevList.SetItemText(iItem,5,strFails);
				if (Data.m_Rfu.at(i).m_uiFails[j])
					m_DevList.SetItemColor(iItem,RED);
			}
			iItem ++;
		}
	}
	for (int32_t i = 0;i < Data.m_Ceu.size();i ++) {
		for (int32_t j = 0;j < 4;j ++) {
			if (!Data.m_Ceu.at(i).m_bConnect[j])
				continue;
			if (m_DevList.GetCheck(iItem)) {
				strVer.Format("%#010x",Data.m_Ceu.at(i).m_uiVer[j]);
				strTimes.Format("%u",Data.m_Ceu.at(i).m_uiTimes[j]);
				strFails.Format("%u",Data.m_Ceu.at(i).m_uiFails[j]);
				m_DevList.SetItemText(iItem,3,strVer);
				m_DevList.SetItemText(iItem,4,strTimes);
				m_DevList.SetItemText(iItem,5,strFails);
				if (Data.m_Ceu.at(i).m_uiFails[j])
					m_DevList.SetItemColor(iItem,RED);
			}
			iItem ++;
		}
	}
	for (int32_t i = 0;i <Data.m_Cxu.size();i ++) {
		if (!Data.m_Cxu.at(i).m_bConnect)
			continue;
		if (m_DevList.GetCheck(iItem)) {
			strTimes.Format("%u",Data.m_Cxu.at(i).m_uiTimes);
			strFails.Format("%u",Data.m_Cxu.at(i).m_uiFails);
			m_DevList.SetItemText(iItem,4,strTimes);
			m_DevList.SetItemText(iItem,5,strFails);
			if (Data.m_Cxu.at(i).m_uiFails)
				m_DevList.SetItemColor(iItem,RED);
		}
		iItem ++;
	}
	return 0;
}