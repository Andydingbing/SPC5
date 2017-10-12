#pragma once
#include "CSEWinThread.h"

class CTestR1CBTRThread : public CCSEWinThread
{
	DECLARE_DYNCREATE(CTestR1CBTRThread)
public:
	typedef struct Result {
		int32_t m_iCurSpeed;
		double m_dTemp[MAX_RF][8];
		COleDateTime m_Time;
		bool m_bAddLiner;
	public:
		Result();
	}Result;
protected:
	CTestR1CBTRThread();           // 动态创建所使用的受保护的构造函数
	virtual ~CTestR1CBTRThread();
public:
	CTestR1CBTRThread(CWnd *pParent);
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual int Run();
	void SetBlowerToDefault(CSP3501 *pSP3501);
};


