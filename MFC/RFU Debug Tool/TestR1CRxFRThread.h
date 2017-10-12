#pragma once

class CTestR1CRxFRThread : public CCSEWinThread
{
	DECLARE_DYNCREATE(CTestR1CRxFRThread)
public:
	typedef struct TestRFFRInfo {
		CSP1401R1C *m_pSP1401;
		CSP2401R1A *m_pSP2401;
		CString  m_strFreqStar;
		CString  m_strFreqStop;
		CString  m_strFreqStep;
	}TestRFFRInfo;

	typedef struct TestIFFRInfo {
		CSP1401R1C *m_pSP1401;
		CSP2401R1A *m_pSP2401;
		uint64_t m_uiRFFreq;
		uint64_t m_uiFreqStep;
	}TestIFFRInfo;

	typedef struct Result {
		int64_t m_iFreq;
		int32_t	m_iSec;
		double	m_dRes;
	}Result;
protected:
	CTestR1CRxFRThread();           // 动态创建所使用的受保护的构造函数
	virtual ~CTestR1CRxFRThread();
public:
	CTestR1CRxFRThread(CWnd *pParent,TestRFFRInfo *pInfo);
	CTestR1CRxFRThread(CWnd *pParent,TestIFFRInfo *pInfo);
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual int Run();
	int32_t TestRFFR();
	int32_t TestIFFR();
private:
	TestRFFRInfo	m_TestRFFRInfo;
	TestIFFRInfo	m_TestIFFRInfo;
};


