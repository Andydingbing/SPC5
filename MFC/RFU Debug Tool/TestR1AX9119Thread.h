#pragma once

class CTestR1AX9119Thread : public CCSEWinThread
{
	DECLARE_DYNCREATE(CTestR1AX9119Thread)

public:
	typedef struct Result {
		int16_t  m_iValue;
		double m_dFreq;
	}Result;
protected:
	CTestR1AX9119Thread();           // 动态创建所使用的受保护的构造函数
	virtual ~CTestR1AX9119Thread();
public:
	CTestR1AX9119Thread(CWnd *pParent);
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual int Run();
};


