#pragma once

class CTestR1ASBFSThread : public CCSEWinThread
{
	DECLARE_DYNCREATE(CTestR1ASBFSThread)
public:
	typedef struct IsLOLock {
		bool m_bTxLO1;
		bool m_bTxLO2;
		bool m_bRxLO1;
		bool m_bRxLO2;
	}IsLOLock;
protected:
	CTestR1ASBFSThread();           // 动态创建所使用的受保护的构造函数
	virtual ~CTestR1ASBFSThread();
public:
	CTestR1ASBFSThread(CWnd *pParent,int32_t iFuncIdx);
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual int Run();
public:
	int32_t TestDynDown();
	int32_t TestSN();
	int32_t TestLO();
};


