#pragma once

class CTestR1ATxFRThread : public CCSEWinThread
{
	DECLARE_DYNCREATE(CTestR1ATxFRThread)
public:
	typedef struct TestRFFRInfo {
		CSP1401R1A *m_pSP1401;
		CSP2401R1A *m_pSP2401;
		CString  m_strFreqStar;
		CString  m_strFreqStop;
		CString  m_strFreqStep;
	}TestRFFRInfo;

	typedef struct TestBBFRInfo {
		CSP1401R1A *m_pSP1401;
		CSP2401R1A *m_pSP2401;
		uint64_t m_uiRFFreq;
		uint64_t m_uiFreqStep;
	}TestBBFRInfo;

	typedef struct Result {
		int64_t m_iFreq;
		int32_t	m_iSec;
		double	m_dRes;
	}Result;
public:
	CTestR1ATxFRThread();           // 动态创建所使用的受保护的构造函数
	virtual ~CTestR1ATxFRThread();
public:
	CTestR1ATxFRThread(CWnd *pParent,TestRFFRInfo *pInfo);
	CTestR1ATxFRThread(CWnd *pParent,TestBBFRInfo *pInfo);
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual int Run();
	int32_t TestRFFR();
	int32_t TestBBFR();
private:
	TestRFFRInfo m_TestRFFRInfo;
	TestBBFRInfo m_TestBBFRInfo;
};


