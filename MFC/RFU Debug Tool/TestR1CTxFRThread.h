#pragma once

class CTestR1CTxFRThread : public CCSEWinThread
{
	DECLARE_DYNCREATE(CTestR1CTxFRThread)
public:
	typedef struct TestRFFRInfo {
		CSP1401R1C *m_pSP1401;
		CSP2401R1A *m_pSP2401;
		CString  m_strFreqStar;
		CString  m_strFreqStop;
		CString  m_strFreqStep;
	}TestRFFRInfo;

	typedef struct TestBBFRInfo {
		CSP1401R1C *m_pSP1401;
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
	CTestR1CTxFRThread();           // 动态创建所使用的受保护的构造函数
	virtual ~CTestR1CTxFRThread();
public:
	CTestR1CTxFRThread(CWnd *pParent,TestRFFRInfo *pInfo);
	CTestR1CTxFRThread(CWnd *pParent,TestBBFRInfo *pInfo);
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual int Run();
public:
	int32_t TestRFFR();
	int32_t TestBBFR();
private:
	TestRFFRInfo m_TestRFFRInfo;
	TestBBFRInfo m_TestBBFRInfo;
};


