
// WICO.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "WICO.h"
#include "MainFrm.h"

#include "WICODoc.h"
#include "WICOView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CWICOApp

BEGIN_MESSAGE_MAP(CWICOApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CWICOApp::OnAppAbout)
	// �����ļ��ı�׼�ĵ�����
	ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
END_MESSAGE_MAP()


// CWICOApp ����

CWICOApp::CWICOApp()
{

	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}

// Ψһ��һ�� CWICOApp ����

CWICOApp theApp;
HANDLE g_hMutex;
CTxDlg *g_pTxDlg;
CRxDlg *g_pRxDlg;


// CWICOApp ��ʼ��

BOOL CWICOApp::InitInstance()
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

	GdiplusStartupInput gidplusStartupInput;
	GdiplusStartup(&m_gdiplusToken,&gidplusStartupInput,NULL);
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
		RUNTIME_CLASS(CWICODoc),
		RUNTIME_CLASS(CMainFrame),       // �� SDI ��ܴ���
		RUNTIME_CLASS(CWICOView));
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
	m_pMainWnd->SetWindowText(APPNAME);
	m_pMainWnd->ShowWindow(SW_SHOWMAXIMIZED);
	m_pMainWnd->UpdateWindow();
	// �������к�׺ʱ�ŵ��� DragAcceptFiles
	//  �� SDI Ӧ�ó����У���Ӧ�� ProcessShellCommand ֮����
	return TRUE;
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
void CWICOApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CWICOApp ��Ϣ�������


int CWICOApp::ExitInstance()
{
	// TODO: �ڴ����ר�ô����/����û���
	GdiplusShutdown(m_gdiplusToken);

	return CWinApp::ExitInstance();
}
