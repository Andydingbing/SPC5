#include "stdafx.h"
#include "RFU_Debug_Tool.h"
#include "MainFrm.h"
#include "RFU_Debug_ToolDoc.h"
#include "RFU_Debug_ToolView.h"
#include <complex>
#include "fftw3.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CCSEApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CCSEApp::OnAppAbout)
	// �����ļ��ı�׼�ĵ�����
	ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
END_MESSAGE_MAP()


CCSEApp::CCSEApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}

CCSEApp theApp;		// Ψһ��һ�� CCSEApp ����
HANDLE g_hMutex;	//only allow one process running
CMainDlg *g_pMainDlg;
CADF5355Dlg *g_pADF5355Dlg;
CHMC1197Dlg *g_pHMC1197Dlg;

CLogDlg		  *g_pLogDlg;
CLogViewerDlg *g_pLogViewerDlg;

CTestCMPBDlg	 *g_pTestCMPBDlg;
CTestAlgo5355Dlg *g_pTestAlgo5355Dlg;
CTestFittingInterpDlg *g_pTestFittingInterpDlg;

// CCSEApp ��ʼ��

BOOL CCSEApp::InitInstance()
{
	g_hMutex = ::CreateMutex(NULL,TRUE,APPNAME);
	if (GetLastError() == ERROR_ALREADY_EXISTS) {
		HWND oldHWnd = NULL;
		oldHWnd = ::FindWindow(NULL,_T(APPNAME));
		if (oldHWnd) {
			::ShowWindow(oldHWnd,SW_SHOWNORMAL);
			::SetForegroundWindow(oldHWnd);
		}
		CloseHandle(g_hMutex);
		g_hMutex = NULL;
		return FALSE;
	}
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CLed::RegisterWndClass();

	CWinApp::InitInstance();

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));
	LoadStdProfileSettings(4);  // ���ر�׼ INI �ļ�ѡ��(���� MRU)
	// ע��Ӧ�ó�����ĵ�ģ�塣�ĵ�ģ��
	// �������ĵ�����ܴ��ں���ͼ֮�������
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CCSEDoc),
		RUNTIME_CLASS(CMainFrame),       // �� SDI ��ܴ���
		RUNTIME_CLASS(CCSEView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);



	// ������׼������DDE�����ļ�������������
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);


	// ��������������ָ����������
	// �� /RegServer��/Register��/Unregserver �� /Unregister ����Ӧ�ó����򷵻� FALSE��
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// Ψһ��һ�������ѳ�ʼ���������ʾ����������и���
	m_pMainWnd->SetWindowText(APPNAME);						//����title
	m_pMainWnd->ShowWindow(SW_SHOWMAXIMIZED);
	m_pMainWnd->UpdateWindow();
	// �������к�׺ʱ�ŵ��� DragAcceptFiles
	//  �� SDI Ӧ�ó����У���Ӧ�� ProcessShellCommand ֮����
	return TRUE;
}

int32_t TimeDomain(ISP1401 *pSP1401,uint32_t uiSamples,double *pX,int16_t *pI,int16_t *pQ)
{
	for (uint32_t i = 0;i < uiSamples;i ++)
		pX[i] = double(i);
	STACHK(DDR->IQToBuf(pSP1401->GetRfIdx(),pI,pQ,uiSamples));
	return 0;
}

int32_t TimeDomain(ISP1401 *pSP1401,uint32_t uiSamples,CChartXYSerie *pILineSerie,CChartXYSerie *pQLineSerie)
{
	int16_t *pI = new int16_t[uiSamples];
	int16_t *pQ = new int16_t[uiSamples];
	double *pX  = new double[uiSamples];
	memset(pI,0,sizeof(int16_t) * uiSamples);
	memset(pQ,0,sizeof(int16_t) * uiSamples);
	for (uint32_t i = 0;i < uiSamples;i ++)
		pX[i] = double(i);

	STACHK(DDR->IQToBuf(pSP1401->GetRfIdx(),pI,pQ,uiSamples));
	
	pILineSerie->ClearSerie();
	pQLineSerie->ClearSerie();
	pILineSerie->AddPoints(pX,pI,uiSamples);
	pQLineSerie->AddPoints(pX,pQ,uiSamples);
	delete []pX;
	delete []pQ;
	delete []pI;
	return 0;
}

int32_t TimeDomain(uint32_t uiSamples,const ArbReader::Data_t *pData,CChartXYSerie *pILineSerie,CChartXYSerie *pQLineSerie)
{
	int16_t *pI = new int16_t[uiSamples];
	int16_t *pQ = new int16_t[uiSamples];
	double *pX  = new double[uiSamples];
	memset(pI,0,sizeof(int16_t) * uiSamples);
	memset(pQ,0,sizeof(int16_t) * uiSamples);
	for (uint32_t i = 0;i < uiSamples;i ++)
		pX[i] = double(i);

	for (uint32_t n = 0;n < uiSamples;n ++) {
		pI[n] = pData[n].i;
		pQ[n] = pData[n].q;
	}

	pILineSerie->ClearSerie();
	pQLineSerie->ClearSerie();
	pILineSerie->AddPoints(pX,pI,uiSamples);
	pQLineSerie->AddPoints(pX,pQ,uiSamples);
	delete []pX;
	delete []pQ;
	delete []pI;
	return 0;
}

int32_t DFT(ISP1401 *pSP1401,int32_t iSampleRate,uint32_t uiSamples,int32_t *pPts,double *pX,double *pOut)
{
	int32_t iPts = (iSampleRate / 2) / (iSampleRate / uiSamples) * 2;
	int16_t *pI = new int16_t[uiSamples];
	int16_t *pQ = new int16_t[uiSamples];
	memset(pI,0,sizeof(int16_t) * uiSamples);
	memset(pQ,0,sizeof(int16_t) * uiSamples);

	STACHK(DDR->IQToBuf(pSP1401->GetRfIdx(),pI,pQ,uiSamples));

	fftw_complex *in, *out;
	fftw_plan plan;
	in  = (fftw_complex *) fftw_malloc(sizeof(fftw_complex) * uiSamples);
	out = (fftw_complex *) fftw_malloc(sizeof(fftw_complex) * uiSamples);
	plan = fftw_plan_dft_1d(uiSamples, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
	for (uint32_t i = 0;i < uiSamples;i ++) {
		in[i][0] = (double)pI[i];
		in[i][1] = (double)pQ[i];
	}
	fftw_execute(plan);
	for (int i = 0;i < iPts / 2;i ++) {						
		pX[i + iPts / 2] = iSampleRate / 2.0 / (double)(iPts / 2.0) * i / 1e6;			//positive frequencys
		pOut[i + iPts / 2] = 10 * log10((out[i][0] * out[i][0] + out[i][1] * out[i][1]) / 134217728.0 / uiSamples / uiSamples);
	}
	for (int i = 0;i < iPts / 2;i ++) {						
		pX[iPts / 2 - 1 - i] = iSampleRate / 2.0 / (double)(iPts / 2.0) * i / 1e6 * -1;	//negative frequencys
		pOut[i] = 10 * log10((out[i + iPts / 2][0] * out[i + iPts / 2][0] + out[i + iPts / 2][1] * out[i + iPts / 2][1]) / 134217728.0 / uiSamples / uiSamples);
	}
	*pPts = iPts;
	fftw_destroy_plan(plan);
	fftw_free(in);
	fftw_free(out);

	delete []pQ;
	delete []pI;
	return 0;
}

int32_t DFT(ISP1401 *pSP1401,int32_t iSampleRate,uint32_t uiSamples,CChartXYSerie *pLineSerie)
{
	int32_t iPts = (iSampleRate / 2) / (iSampleRate / uiSamples) * 2;
	int16_t *pI = new int16_t[uiSamples];
	int16_t *pQ = new int16_t[uiSamples];
	double  *pX = new double[iSampleRate];
	double  *pOut = new double[iPts];
	memset(pI,0,sizeof(int16_t) * uiSamples);
	memset(pQ,0,sizeof(int16_t) * uiSamples);
	memset(pX,0,sizeof(double) * iSampleRate);
	memset(pOut,0,sizeof(double) * iPts);

	STACHK(DDR->IQToBuf(pSP1401->GetRfIdx(),pI,pQ,uiSamples));

	fftw_complex *in, *out;
	fftw_plan plan;
	in  = (fftw_complex *) fftw_malloc(sizeof(fftw_complex) * uiSamples);
	out = (fftw_complex *) fftw_malloc(sizeof(fftw_complex) * uiSamples);
	plan = fftw_plan_dft_1d(uiSamples, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
	for (uint32_t i = 0;i < uiSamples;i ++) {
		in[i][0] = (double)pI[i];
		in[i][1] = (double)pQ[i];
	}
	fftw_execute(plan);
	for (int i = 0;i < iPts / 2;i ++) {						
		pX[i + iPts / 2] = iSampleRate / 2.0 / (double)(iPts / 2.0) * i / 1e6;			//positive frequencys
		pOut[i + iPts / 2] = 10 * log10((out[i][0] * out[i][0] + out[i][1] * out[i][1]) / 134217728.0 / uiSamples / uiSamples);
	}
	for (int i = 0;i < iPts / 2;i ++) {						
		pX[iPts / 2 - 1 - i] = iSampleRate / 2.0 / (double)(iPts / 2.0) * i / 1e6 * -1;	//negative frequencys
		pOut[i] = 10 * log10((out[i + iPts / 2][0] * out[i + iPts / 2][0] + out[i + iPts / 2][1] * out[i + iPts / 2][1]) / 134217728.0 / uiSamples / uiSamples);
	}
	fftw_destroy_plan(plan);
	fftw_free(in);
	fftw_free(out);

// 	pLineSerie->ClearSerie();
// 	pLineSerie->AddPoints(pX,pOut,iPts);
	pLineSerie->SetPoints(pX,pOut,iPts);

	delete []pOut;
	delete []pX;
	delete []pQ;
	delete []pI;
	return 0;
}

int32_t DFT(int32_t iSampleRate,uint32_t uiSamples,const ArbReader::Data_t *pData,CChartXYSerie *pLineSerie)
{
	int32_t iPts = (iSampleRate / 2) / (iSampleRate / uiSamples) * 2;
	double  *pX = new double[iPts];
	double  *pOut = new double[iPts];
	memset(pX,0,sizeof(double) * iPts);
	memset(pOut,0,sizeof(double) * iPts);

	fftw_complex *in, *out;
	fftw_plan plan;
	in  = (fftw_complex *) fftw_malloc(sizeof(fftw_complex) * uiSamples);
	out = (fftw_complex *) fftw_malloc(sizeof(fftw_complex) * iPts);
	plan = fftw_plan_dft_1d(uiSamples, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
	for (uint32_t n = 0;n < uiSamples;n ++) {
		in[n][0] = (double)pData[n].i;
		in[n][1] = (double)pData[n].q;
	}
	fftw_execute(plan);

	double dAmp = 0.0;
	for (int i = 0;i < iPts / 2;i ++) {						
		pX[i + iPts / 2] = iSampleRate / 2.0 / (double)(iPts / 2.0) * i / 1e6;			//positive frequencys
		dAmp = (out[i][0] * out[i][0] + out[i][1] * out[i][1]) / 134217728.0 / uiSamples / uiSamples;
		if (dAmp > 0)
			pOut[i + iPts / 2] = 10 * log10(dAmp);
		else
			pOut[i + iPts / 2] = -180.0;
	}
	for (int i = 0;i < iPts / 2;i ++) {						
		pX[iPts / 2 - 1 - i] = iSampleRate / 2.0 / (double)(iPts / 2.0) * i / 1e6 * -1;	//negative frequencys
		dAmp = (out[i + iPts / 2][0] * out[i + iPts / 2][0] + out[i + iPts / 2][1] * out[i + iPts / 2][1]) / 134217728.0 / uiSamples / uiSamples;
		if (dAmp > 0)
			pOut[i] = 10 * log10(dAmp);
		else
			pOut[i] = -180.0;
	}
	fftw_destroy_plan(plan);
	fftw_free(in);
	fftw_free(out);

	pLineSerie->ClearSerie();
	pLineSerie->AddPoints(pX,pOut,iPts);

	delete []pOut;
	delete []pX;
	return 0;
}

int32_t ExeFirProcess(char *pPath)
{
	PROCESS_INFORMATION pinfo;
	STARTUPINFO sinfo;

	ZeroMemory(&sinfo,sizeof(sinfo) );
	sinfo.cb = sizeof(sinfo);
	ZeroMemory(&pinfo,sizeof(pinfo) );

	if(!CreateProcess( NULL,(LPSTR)pPath,NULL,NULL,FALSE,NORMAL_PRIORITY_CLASS,NULL,NULL,&sinfo,&pinfo)) {
		Log->SetLastError(-1,"cannot execute \"%s\"(%d)",pPath,GetLastError());
		return -1;
	}

	DWORD dwRet = WaitForSingleObject(pinfo.hProcess,5 * 60 * 1000);
	if (WAIT_TIMEOUT == dwRet) {
		Log->SetLastError(-1,"process %s timeout");
		return -1;
	}

	CloseHandle(pinfo.hProcess);
	CloseHandle(pinfo.hThread);
	return 0;
}

int32_t freq2str(double	freq,CString &strFreq,int32_t digit)
{
	char szFreq[64] = {0};
	freq2str(freq,szFreq,digit);
	strFreq.Format("%s",szFreq);
	return 0;
}

int32_t freq2str(float freq,CString &strFreq,int32_t digit)
{
	char szFreq[64] = {0};
	freq2str(freq,szFreq,digit);
	strFreq.Format("%s",szFreq);
	return 0;
}

int32_t freq2str(uint64_t freq,CString &strFreq,int32_t digit)
{
	char szFreq[64] = {0};
	freq2str(freq,szFreq,digit);
	strFreq.Format("%s",szFreq);
	return 0;
}

int32_t freq2str(int32_t freq,CString &strFreq,int32_t digit)
{
	char szFreq[64] = {0};
	freq2str(freq,szFreq,digit);
	strFreq.Format("%s",szFreq);
	return 0;
}


int32_t freq2str(int64_t freq,CString &strFreq,int32_t digit)
{
	char szFreq[64] = {0};
	freq2str(freq,szFreq,digit);
	strFreq.Format("%s",szFreq);
	return 0;
}

int32_t coef2str(int16_t *pCoef,int32_t iOrder,CString &strBuf)
{
	char szBuf[1024 * 4] = {0};
	coef2str(pCoef,iOrder,szBuf);
	strBuf.Format("%s",szBuf);
	return 0;
}

int32_t freq2array(CString pin1,CString pin2,CString step,int64_t *pout,int64_t *pfreqstar,int64_t *pfreqstop,int64_t *pmax,int64_t *pmin)
{
	uint64_t uistep = 0;
	str2freq((LPSTR)(LPCTSTR)step,uistep);
	return freq2array((LPSTR)(LPCTSTR)pin1,(LPSTR)(LPCTSTR)pin2,uistep,pout,pfreqstar,pfreqstop,pmax,pmin);
}

int32_t tm2str(tm t,CString &strTime)
{
	strTime.Format("%4d/%02d/%02d %02d:%02d:%02d",t.tm_year + 1900,t.tm_mon,t.tm_mday,t.tm_hour,t.tm_min,t.tm_sec);
	return 0;
}

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// �������жԻ����Ӧ�ó�������
void CCSEApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


// CCSEApp ��Ϣ�������

