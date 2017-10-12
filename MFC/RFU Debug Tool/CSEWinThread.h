#pragma once

//for testing
#define TEST_THREAD_TEST_CANCEL()		\
	if (!m_bRunning) {					\
		TEST_THREAD_ABOART();			\
	}

#define TEST_THREAD_TEST_CANCEL_S(func)	\
	if (!m_bRunning) {					\
		TEST_THREAD_ABOART_S(func);		\
	}

#define TEST_THREAD_ABOART()														\
	::PostMessage(m_pParent->GetSafeHwnd(),WM_CSE_SINGLE_THREAD_DONE,NULL,NULL);	\
	return 0;

#define TEST_THREAD_ABOART_S(func)													\
	func;																			\
	::PostMessage(m_pParent->GetSafeHwnd(),WM_CSE_SINGLE_THREAD_DONE,NULL,NULL);	\
	return 0;

#define TEST_THREAD_INSTR_CHK(func)		\
	do {								\
		if (int ret = func) {			\
			TEST_THREAD_ABOART();		\
			return ret;					\
		}								\
	}while (0)

//for calibration
#define CAL_THREAD_TEST_CANCEL()																			\
	if (!m_bRunning) {																						\
		uint32_t uiRfIdx = ((CCalR1CBaseDlg *)m_pParent)->m_pSP1401->GetRfIdx();							\
		::SendMessage(ALL_DLG->m_pCalR1CDlg[uiRfIdx],WM_CSE_SINGLE_THREAD_CANCEL,NULL,(LPARAM)&m_nThreadID);\
		CAL_THREAD_ABOART();																				\
	}

#define CAL_THREAD_TEST_CANCEL_S(func)																		\
	if (!m_bRunning) {																						\
		uint32_t uiRfIdx = ((CCalR1CBaseDlg *)m_pParent)->m_pSP1401->GetRfIdx();							\
		::SendMessage(ALL_DLG->m_pCalR1CDlg[uiRfIdx],WM_CSE_SINGLE_THREAD_CANCEL,NULL,(LPARAM)&m_nThreadID);\
		CAL_THREAD_ABOART_S(func);																			\
	}

#define CAL_THREAD_ABOART()															\
	Instrument.Close();																\
	::SendMessage(m_pParent->GetSafeHwnd(),WM_CSE_SINGLE_THREAD_DONE,NULL,NULL);	\
	return 0;

#define CAL_THREAD_ABOART_S(func)													\
	func;																			\
	Instrument.Close();																\
	::SendMessage(m_pParent->GetSafeHwnd(),WM_CSE_SINGLE_THREAD_DONE,NULL,NULL);	\
	return 0;

#define CAL_THREAD_INSTR_CHK(func)		\
	do {								\
		if (int ret = func) {			\
			uint32_t uiRfIdx = ((CCalR1CBaseDlg *)m_pParent)->m_pSP1401->GetRfIdx();							\
			::SendMessage(ALL_DLG->m_pCalR1CDlg[uiRfIdx],WM_CSE_SINGLE_THREAD_CANCEL,NULL,(LPARAM)&m_nThreadID);\
			CAL_THREAD_ABOART();		\
		}								\
	}while (0)

class CCSEWinThread : public CWinThread
{
	DECLARE_DYNCREATE(CCSEWinThread)

public:
	CCSEWinThread();           // 动态创建所使用的受保护的构造函数
	virtual ~CCSEWinThread();

public:
	int32_t m_iFuncIdx;
	static BOOL m_bRunning;
	static CString m_strProc;
	CWnd *m_pParent;
	virtual BOOL InitInstance();
	virtual int ExitInstance();
protected:
	DECLARE_MESSAGE_MAP()
};


