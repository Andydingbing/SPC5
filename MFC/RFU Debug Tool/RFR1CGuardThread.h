#pragma once

class CRFR1CGuardThread : public CCSEWinThread
{
	DECLARE_DYNCREATE(CRFR1CGuardThread)
public:
	typedef struct Result {
		int32_t m_iIdx;	//decide what color to show in the title
		COleDateTime m_Time;
		double m_dTemp[8];
	public:
		Result();
	}Result;
protected:
	CRFR1CGuardThread();           // 动态创建所使用的受保护的构造函数
	virtual ~CRFR1CGuardThread();
public:
	CRFR1CGuardThread(CWnd *pParent);
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual int Run();
};


