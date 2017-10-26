#include "stdafx.h"
#include "RFU_Debug_Tool.h"
#include "TestCMPBThread.h"
#include "CheckDlg.h"
#include "RegDef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CTestCMPBThread, CWinThread)

CTestCMPBThread::Info::Rfu::Rfu()
{
	for (int32_t i = 0;i < 2;i ++) {
		m_strPath[i] = _T("");
		m_bProgram[i] = FALSE;
		m_bConnect[i] = FALSE;
	}
}

CTestCMPBThread::Info::Ceu::Ceu()
{
	for (int32_t i = 0;i < 4;i ++) {
		m_strPath[i] = _T("");
		m_bProgram[i] = FALSE;
		m_bConnect[i] = FALSE;
	}
}

CTestCMPBThread::Info::Cxu::Cxu() : m_strPath(_T("")),m_bProgram(FALSE),m_bConnect(FALSE)
{

}

CTestCMPBThread::Result::Rfu::Rfu()
{
	for (int32_t i = 0;i < 2;i ++) {
		m_uiTimes[i] = 0;
		m_uiFails[i] = 0;
		m_uiVer[i] = 0;
		m_bConnect[i] = FALSE;
	}
}

CTestCMPBThread::Result::Ceu::Ceu()
{
	for (int32_t i = 0;i < 4;i ++) {
		m_uiTimes[i] = 0;
		m_uiFails[i] = 0;
		m_uiVer[i] = 0;
		m_bConnect[i] = FALSE;
	}
}

CTestCMPBThread::Result::Cxu::Cxu()
{
	m_uiTimes = 0;
	m_uiFails = 0;
	m_uiVer = 0;
	m_bConnect = FALSE;
}

CTestCMPBThread::CTestCMPBThread()
{
}

CTestCMPBThread::CTestCMPBThread(CWnd *pParent,Info *pInfo)
{
	m_pParent = pParent;
	m_Info.m_Rfu = pInfo->m_Rfu;
	m_Info.m_Ceu = pInfo->m_Ceu;
	m_Info.m_Cxu = pInfo->m_Cxu;
	m_Info.m_uiTimes = pInfo->m_uiTimes;
	m_Info.m_uiDelayms = pInfo->m_uiDelayms;
	m_Info.m_uiOneBlockSize = pInfo->m_uiOneBlockSize;
}

CTestCMPBThread::~CTestCMPBThread()
{
}

BOOL CTestCMPBThread::InitInstance()
{
	return TRUE;
}

int CTestCMPBThread::ExitInstance()
{
	return CWinThread::ExitInstance();
}


BEGIN_MESSAGE_MAP(CTestCMPBThread, CWinThread)
END_MESSAGE_MAP()


int CTestCMPBThread::Run()
{
	TEST_THREAD_INSTR_CHK(ThreadCheckBox("Prepare Testing FPGA Program bit"));
	Result Data;
	Result::Rfu RfuData;
	Result::Ceu CeuData;
	Result::Cxu CxuData;
	for (int32_t i = 0;i < m_Info.m_Rfu.size();i ++) {
		for (int32_t j = 0;j < 2;j ++)
			RfuData.m_bConnect[j] = m_Info.m_Rfu[i].m_bConnect[j];
		Data.m_Rfu.push_back(RfuData);
	}
	for (int32_t i = 0;i < m_Info.m_Ceu.size();i ++) {
		for (int32_t j = 0;j < 4;j ++)
			CeuData.m_bConnect[j] = m_Info.m_Ceu[i].m_bConnect[j];
		Data.m_Ceu.push_back(CeuData);
	}
	for (int32_t i = 0;i < m_Info.m_Cxu.size();i ++) {
		CxuData.m_bConnect = m_Info.m_Cxu[i].m_bConnect;
		Data.m_Cxu.push_back(CxuData);
	}

	vector<Info::Rfu>::iterator Rfu_iter;
	vector<Info::Ceu>::iterator Ceu_iter;
	vector<Info::Cxu>::iterator Cxu_iter;

	CGeneralIniFile IniFile(CONFIG_FILE_PATH);
	char szProductForm[64];
	char szRsrcSection[64];
	char szDevKey[64];
	char szRsrcNameS6[64];
	char szRsrcNameK7[64];
	char szItemName[64];
	uint32_t uiVer = 0;

	memset(szProductForm,0,ARRAY_SIZE(szProductForm));
	memset(szRsrcSection,0,ARRAY_SIZE(szRsrcSection));
	memset(szDevKey,0,ARRAY_SIZE(szDevKey));
	memset(szRsrcNameS6,0,ARRAY_SIZE(szRsrcNameS6));
	memset(szRsrcNameK7,0,ARRAY_SIZE(szRsrcNameK7));
	memset(szItemName,0,ARRAY_SIZE(szItemName));
	IniFile.GetConfigStringValue("Product Form","Form",szProductForm);
	strcpy(szRsrcSection,"RESOURCE");
	strcat(szRsrcSection,szProductForm);

	InitSinglePos("Testing FPGA Program bit",m_Info.m_uiTimes,m_nThreadID);

	for (uint32_t i = 0;i < m_Info.m_uiTimes;i ++) {
		TEST_THREAD_TEST_CANCEL();
		for (Rfu_iter = m_Info.m_Rfu.begin();Rfu_iter != m_Info.m_Rfu.end();Rfu_iter ++) {
			TEST_THREAD_TEST_CANCEL();
			for (int32_t j = 0;j < 2;j ++) {
				if ((*Rfu_iter).m_bProgram[j]) {
					TEST_THREAD_TEST_CANCEL();
					if (!ProgramRfuK7(j,(*Rfu_iter).m_strS6Rsrc,(*Rfu_iter).m_strPath[j])) {	//download success
						sprintf(szDevKey,"RFU%d_K7_%d",(*Rfu_iter).m_uiIdx,j);
						IniFile.GetConfigStringValue(szRsrcSection,szDevKey,szRsrcNameK7);
						viPCIDev K7;
						K7.Init(szRsrcNameK7);
						if (K7.R32(viPCIDev::AS_BAR0,0,&uiVer))											//but abnormal
							Data.m_Rfu.at(Rfu_iter - m_Info.m_Rfu.begin()).m_uiFails[j] ++;
						else
							Data.m_Rfu.at(Rfu_iter - m_Info.m_Rfu.begin()).m_uiVer[j] = uiVer;
						K7.Close();
					}
					else
						Data.m_Rfu.at(Rfu_iter - m_Info.m_Rfu.begin()).m_uiFails[j] ++;
					Data.m_Rfu.at((Rfu_iter - m_Info.m_Rfu.begin())).m_uiTimes[j] ++;
				}
				::SendMessage(m_pParent->GetSafeHwnd(),WM_CSE_TEST_CMPB_SHOW,0,(LPARAM)(&Data));
			}
		}
		for (Ceu_iter = m_Info.m_Ceu.begin();Ceu_iter != m_Info.m_Ceu.end();Ceu_iter ++) {
			TEST_THREAD_TEST_CANCEL();
			for (int32_t j = 0;j < 4;j ++) {
				if ((*Ceu_iter).m_bProgram[j]) {
					TEST_THREAD_TEST_CANCEL();
					if (!ProgramCeuK7(j,(*Ceu_iter).m_strS6Rsrc,(*Ceu_iter).m_strPath[j])) {
						sprintf(szDevKey,"CEU%d_K7_%d",(*Ceu_iter).m_uiIdx,j);
						IniFile.GetConfigStringValue(szRsrcSection,szDevKey,szRsrcNameK7);
						viPCIDev K7;
						K7.Init(szRsrcNameK7);
						if (K7.R32(viPCIDev::AS_BAR0,4,&uiVer))
							Data.m_Ceu.at(Ceu_iter - m_Info.m_Ceu.begin()).m_uiFails[j] ++;
						else
							Data.m_Ceu.at(Ceu_iter - m_Info.m_Ceu.begin()).m_uiVer[j] = uiVer;
						K7.Close();
					}
					else
						Data.m_Ceu.at(Ceu_iter - m_Info.m_Ceu.begin()).m_uiFails[j] ++;
					Data.m_Ceu.at(Ceu_iter - m_Info.m_Ceu.begin()).m_uiTimes[j] ++;
				}
				::SendMessage(m_pParent->GetSafeHwnd(),WM_CSE_TEST_CMPB_SHOW,0,(LPARAM)(&Data));
			}
		}
		for (Cxu_iter = m_Info.m_Cxu.begin();Cxu_iter != m_Info.m_Cxu.end();Cxu_iter ++) {
			TEST_THREAD_TEST_CANCEL();
			if ((*Cxu_iter).m_bProgram) {
				TEST_THREAD_TEST_CANCEL();
				if (!ProgramCxuV7((*Cxu_iter).m_strS6Rsrc,(*Cxu_iter).m_strPath)) {
					IniFile.GetConfigStringValue(szRsrcSection,"CXU_V7",szRsrcNameK7);
					viPCIDev V7;
					V7.Init(szRsrcNameK7);
					if (V7.R32(viPCIDev::AS_BAR0,0,&uiVer))
						Data.m_Cxu.at(Cxu_iter - m_Info.m_Cxu.begin()).m_uiFails ++;
					else
						Data.m_Cxu.at(Cxu_iter - m_Info.m_Cxu.begin()).m_uiVer = uiVer;
					V7.Close();
				}
				else
					Data.m_Cxu.at(Cxu_iter - m_Info.m_Cxu.begin()).m_uiFails ++;
				Data.m_Cxu.at(Cxu_iter - m_Info.m_Cxu.begin()).m_uiTimes ++;
			}
			::SendMessage(m_pParent->GetSafeHwnd(),WM_CSE_TEST_CMPB_SHOW,0,(LPARAM)(&Data));
		}
		SetPos(i + 1,m_nThreadID);
	}
	TEST_THREAD_ABOART();
}

int32_t CTestCMPBThread::ProgramRfuK7(int32_t iK7Index,CString strS6Rsrc,CString strPath)
{
	int iBlockSize = (int)m_Info.m_uiOneBlockSize;
	viPCIDev S6;
	STACHK(S6.Init(strS6Rsrc.GetBuffer(0)));
	STACHK(SetFpgaBit(iK7Index,(LPSTR)(LPCTSTR)strPath,&S6,iBlockSize));
	S6.Close();
	return 0;
}

#define PROGRAM_B_REG 0x12
#define LOAD_STATE_REG 0x13
#define LOAD_MODE_REG 0x14
#define LOAD_DATA_REG 0x15

int32_t CTestCMPBThread::ProgramCeuK7(int32_t iK7Index,CString strS6Rsrc,CString strPath)
{
	uint32_t state = 0;
	int iFileSize = 0;
	int iBlockSize = (int)m_Info.m_uiOneBlockSize;
	int iInitFlag = 0;

	FILE *fp = fopen((LPSTR)(LPCTSTR)strPath,"rb");
	if (fp == NULL) {
		Log.SetLastError("file : %s missing",strPath);
		return -1;
	}
	fseek(fp,0,SEEK_END);
	iFileSize = ftell(fp);
	iFileSize = (iFileSize / 4 + ((iFileSize % 4) ? 1 : 0)) * 4;
	fseek(fp,0,SEEK_SET);
	uint32_t *pBuf = new uint32_t[iFileSize / 4];
	memset(pBuf,0,iFileSize / 4);
	fread(pBuf,1,iFileSize,fp);
	int iDownCnt = (iFileSize / 4 / iBlockSize) + 1;

	viPCIDev S6;
	STACHK(S6.Init(strS6Rsrc.GetBuffer(0)));

	S6.W32(viPCIDev::AS_BAR0,(LOAD_MODE_REG+(iK7Index/2)*16+(iK7Index%2)*4) << 2,1);
	S6.W32(viPCIDev::AS_BAR0,(PROGRAM_B_REG+(iK7Index/2)*16+(iK7Index%2)*4) << 2,0);
	Sleep(10);
	S6.W32(viPCIDev::AS_BAR0,(PROGRAM_B_REG+(iK7Index/2)*16+(iK7Index%2)*4) << 2,1);
	Sleep(10);
	//check cfg init
	S6.R32(viPCIDev::AS_BAR0,(LOAD_STATE_REG+(iK7Index/2)*16+(iK7Index%2)*4) << 2,&state);
	while((state&1) == 0) {
		S6.R32(viPCIDev::AS_BAR0,(LOAD_STATE_REG+(iK7Index/2)*16+(iK7Index%2)*4) << 2,&state);
		if ((++iInitFlag) > 1000)
			break;
	}
	//wait fifo empty
	Log.stdprintf("preparing programming ceu k7_%d,waiting fifo empty...\n",iK7Index);
	iInitFlag = 0;
	S6.R32(viPCIDev::AS_BAR0,(LOAD_STATE_REG+(iK7Index/2)*16+(iK7Index%2)*4) << 2,&state);
	while(((state>>2) & 1) == 0) {
		S6.R32(viPCIDev::AS_BAR0,(LOAD_STATE_REG+(iK7Index/2)*16+(iK7Index%2)*4) << 2,&state);
		if ((++iInitFlag) > 1000)
			break;
	}
	//downloading
	Log.stdprintf("downloading...\n");
	for (int i=0;i<iDownCnt;i++)
	{
		if (i == (iDownCnt-1))
			S6.W32(viPCIDev::AS_BAR0,(LOAD_DATA_REG+(iK7Index/2)*16+(iK7Index%2)*4) << 2,(iFileSize - i*4*iBlockSize) / 4,(pBuf+i*iBlockSize));
		else
			S6.W32(viPCIDev::AS_BAR0,(LOAD_DATA_REG+(iK7Index/2)*16+(iK7Index%2)*4) << 2,iBlockSize,(pBuf+i*iBlockSize));
		//wait fifo empty
		iInitFlag = 0;
		S6.R32(viPCIDev::AS_BAR0,(LOAD_STATE_REG+(iK7Index/2)*16+(iK7Index%2)*4) << 2,&state);
		while(((state>>2) & 1) == 0) {
			S6.R32(viPCIDev::AS_BAR0,(LOAD_STATE_REG+(iK7Index/2)*16+(iK7Index%2)*4) << 2,&state);
			if ((++iInitFlag) > 1000)
				break;
		}
		if (m_Info.m_uiDelayms)
			Sleep(m_Info.m_uiDelayms);
		for (int n = 0;n < 300000;n ++) {
			int j = 0;
			j ++;
		}
		Log.stdprintf("%-5d done\r",i);
	}
	S6.R32(viPCIDev::AS_BAR0,(LOAD_STATE_REG+(iK7Index/2)*16+(iK7Index%2)*4) << 2,&state);
	Log.stdprintf("\n");
	//check cfg done
	if (((state>>1) & 1) == 0)
		goto FAIL;
	else
		goto SUCCESS;

SUCCESS:
	delete []pBuf;
	fclose(fp);
	Log.stdprintf("restarting...\n");
	system("devcon_x64.exe restart \"PCI\\VEN_10EE&DEV_7022\"");
	Sleep(1000);
	Log.stdprintf("restarted\n");
	S6.Close();
	return 0;
FAIL:
	Log.SetLastError("download complete,but device restart fail!!!");
	delete []pBuf;
	fclose(fp);
	S6.Close();
	return -1;
}

int32_t CTestCMPBThread::ProgramCxuV7(CString strS6Rsrc,CString strPath)
{
	int iFileSize = 0;
	int iBlockSize = 300;
	int iInitFlag = 0;
	uint32_t state = 0;

	FILE *fp = fopen(strPath,"rb");
	if (NULL == fp) {
		Log.SetLastError("file : %s missing",strPath);
		return -1;
	}
	fseek(fp,0,SEEK_END);
	iFileSize = ftell(fp);
	iFileSize = (iFileSize / 4 + ((iFileSize % 4) ? 1 : 0)) * 4;
	fseek(fp,0,SEEK_SET);
	uint32_t *pBuf = new uint32_t[iFileSize / 4];
	memset(pBuf,0,iFileSize / 4);
	fread(pBuf,1,iFileSize,fp);
	int iDownCnt = (iFileSize / 4 / iBlockSize) + 1;

	viPCIDev S6;
	STACHK(S6.Init(strS6Rsrc.GetBuffer(0)));

	S6.W32(viPCIDev::AS_BAR0,(LOAD_MODE_REG+16) << 2,1);
	S6.W32(viPCIDev::AS_BAR0,(PROGRAM_B_REG+16) << 2,0);
	Sleep(10);
	S6.W32(viPCIDev::AS_BAR0,(PROGRAM_B_REG+16) << 2,1);
	Sleep(10);
	//check cfg init
	S6.R32(viPCIDev::AS_BAR0,(LOAD_STATE_REG+16) << 2,&state);
	while((state&1) == 0) {
		S6.R32(viPCIDev::AS_BAR0,(LOAD_STATE_REG+16) << 2,&state);
		if ((++iInitFlag) > 1000)
			break;
	}
	//wait fifo empty
	Log.stdprintf("preparing programming cxu v7,waiting fifo empty...\n");
	iInitFlag = 0;
	S6.R32(viPCIDev::AS_BAR0,(LOAD_STATE_REG+16) << 2,&state);
	while(((state>>2) & 1) == 0) {
		S6.R32(viPCIDev::AS_BAR0,(LOAD_STATE_REG+16) << 2,&state);
		if ((++iInitFlag) > 1000)
			break;
	}
	//downloading
	Log.stdprintf("downloading...\n");
	for (int i=0;i<iDownCnt;i++)
	{
		if (i == (iDownCnt-1))
			S6.W32(viPCIDev::AS_BAR0,(LOAD_DATA_REG+16) << 2,(iFileSize - i*4*iBlockSize) / 4,(pBuf+i*iBlockSize));
		else
			S6.W32(viPCIDev::AS_BAR0,(LOAD_DATA_REG+16) << 2,iBlockSize,(pBuf+i*iBlockSize));
		//wait fifo empty
		iInitFlag = 0;
		S6.R32(viPCIDev::AS_BAR0,(LOAD_STATE_REG+16) << 2,&state);
		while(((state>>2) & 1) == 0) {
			S6.R32(viPCIDev::AS_BAR0,(LOAD_STATE_REG+16) << 2,&state);
			if ((++iInitFlag) > 1000)
				break;
		}
		if (m_Info.m_uiDelayms)
			Sleep(m_Info.m_uiDelayms);
		for (int n = 0;n < 100000;n ++) {
			int j = 0;
			j ++;
		}
		Log.stdprintf("%-5d done\r",i);
	}
	S6.R32(viPCIDev::AS_BAR0,(LOAD_STATE_REG+16) << 2,&state);
	Log.stdprintf("\n");
	//check cfg done
	if (((state>>1) & 1) == 0)
		goto FAIL;
	else
		goto SUCCESS;

SUCCESS:
	delete []pBuf;
	fclose(fp);
	Log.stdprintf("restarting...\n");
	system("devcon_x64.exe restart \"PCI\\VEN_10EE&DEV_7024\"");
	Sleep(1000);
	Log.stdprintf("restarted\n");
	S6.Close();
	return 0;
FAIL:
	Log.SetLastError("download complete,but device restart fail");
	delete []pBuf;
	fclose(fp);
	S6.Close();
	return -1;
}