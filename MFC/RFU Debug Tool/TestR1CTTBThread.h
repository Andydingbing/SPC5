#pragma once
#include "CSEWinThread.h"

class CTestR1CTTBThread : public CCSEWinThread
{
	DECLARE_DYNCREATE(CTestR1CTTBThread)
public:
	typedef struct Result {
		int32_t m_iFreqIdx;
		double m_dTemp;
		double m_dPower;
	}Result;
protected:
	CTestR1CTTBThread();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CTestR1CTTBThread();
public:
	CTestR1CTTBThread(CWnd *pParent);
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual int Run();
	void SetBlowerToDefault(CSP3501 *pSP3501);
};


