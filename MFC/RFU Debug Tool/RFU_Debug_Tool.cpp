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
	// 基于文件的标准文档命令
	ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
END_MESSAGE_MAP()


CCSEApp::CCSEApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}

CCSEApp theApp;		// 唯一的一个 CCSEApp 对象
HANDLE g_hMutex;	//only allow one process running
CMainDlg *g_pMainDlg;
CADF5355Dlg *g_pADF5355Dlg;
CHMC1197Dlg *g_pHMC1197Dlg;

CLogDlg		  *g_pLogDlg;
CLogViewerDlg *g_pLogViewerDlg;

CTestCMPBDlg	 *g_pTestCMPBDlg;
CTestAlgo5355Dlg *g_pTestAlgo5355Dlg;
CTestFittingInterpDlg *g_pTestFittingInterpDlg;

// CCSEApp 初始化

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
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CLed::RegisterWndClass();

	CWinApp::InitInstance();

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));
	LoadStdProfileSettings(4);  // 加载标准 INI 文件选项(包括 MRU)
	// 注册应用程序的文档模板。文档模板
	// 将用作文档、框架窗口和视图之间的连接
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CCSEDoc),
		RUNTIME_CLASS(CMainFrame),       // 主 SDI 框架窗口
		RUNTIME_CLASS(CCSEView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);



	// 分析标准外壳命令、DDE、打开文件操作的命令行
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);


	// 调度在命令行中指定的命令。如果
	// 用 /RegServer、/Register、/Unregserver 或 /Unregister 启动应用程序，则返回 FALSE。
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// 唯一的一个窗口已初始化，因此显示它并对其进行更新
	m_pMainWnd->SetWindowText(APPNAME);						//窗口title
	m_pMainWnd->ShowWindow(SW_SHOWMAXIMIZED);
	m_pMainWnd->UpdateWindow();
	// 仅当具有后缀时才调用 DragAcceptFiles
	//  在 SDI 应用程序中，这应在 ProcessShellCommand 之后发生
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

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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

// 用于运行对话框的应用程序命令
void CCSEApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


// CCSEApp 消息处理程序

