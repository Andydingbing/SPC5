#pragma once
#include "Stretchdlg.h"
#include "CSEWinThread.h"
#include <vector>

#define DECLARE_CSE_ATTACHTHREAD_DLG()				\
	afx_msg void OnSize(UINT nType, int cx, int cy);\
	virtual BOOL PreTranslateMessage(MSG* pMsg);

#define IMPLEMENT_CSE_ATTACHTHREAD_DLG(class_name)						\
	void class_name::OnSize(UINT nType, int cx, int cy)					\
	{																	\
		CStretchDlg::OnSize(nType, cx, cy);								\
	}																	\
	BOOL class_name::PreTranslateMessage(MSG* pMsg)						\
	{																	\
		if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)	\
			return TRUE;												\
		if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)	\
			return TRUE;												\
		else															\
			return CDialog::PreTranslateMessage(pMsg);					\
	}						

#define TEST_THREAD_EXISTED_CHK()	THREAD_EXISTED_CHK()
#define TEST_THREAD_EXISTED_CHKV()	THREAD_EXISTED_CHKV()

#define CAL_THREAD_EXISTED_CHK()	THREAD_EXISTED_CHK()
#define CAL_THREAD_EXISTED_CHKV()	THREAD_EXISTED_CHKV()

#define THREAD_EXISTED_CHK()							\
	if (CCSEWinThread::m_bRunning) {					\
		CseMessageBox("%s",CCSEWinThread::m_strProc);	\
		return 0;										\
	}													\
	CCSEWinThread::m_bRunning = TRUE;

#define THREAD_EXISTED_CHKV()							\
	if (CCSEWinThread::m_bRunning) {					\
		CseMessageBox("%s",CCSEWinThread::m_strProc);	\
		return;											\
	}													\
	CCSEWinThread::m_bRunning = TRUE;

using namespace std;

class CAttachThreadDlg : public CStretchDlg
{
	DECLARE_DYNAMIC(CAttachThreadDlg)
public:
	static CCSEWinThread *m_pThread;	//for those ui threads which allow only one thread running at a time,e.g. calibration and test ui threads
	void AddThreadRunningCtrl(int iID);
	void AddThreadIdleCtrl(int iID);
	void AddThreadToPauseCtrl(int iID);
	void AddThreadToRunningCtrl(int iID);
	LRESULT CtrlPrepare();
	LRESULT CtrlResume();
	LRESULT CtrlPausing();
	LRESULT CtrlRunning();
	LRESULT CtrlDisable();
private:
	vector<int> m_iRunningID;
	vector<int> m_iIdleID;
	vector<int> m_iPausingID;
	vector<int> m_iResumedID;
public:
	CAttachThreadDlg(UINT nIDTemplate,CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAttachThreadDlg();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedPause();
	afx_msg void OnBnClickedContinue();
	afx_msg void OnBnClickedStop();
	afx_msg virtual LRESULT Done(WPARAM wParam,LPARAM lParam);
};
