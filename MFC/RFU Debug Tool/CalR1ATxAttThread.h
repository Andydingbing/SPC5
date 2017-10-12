#pragma once
#include "CalBaseThread.h"

class CCalR1ATxAttThread : public CCalBaseThread
{
	DECLARE_DYNCREATE(CCalR1ATxAttThread)
public:
	typedef struct Result : public TxAttTableR1A::DataF {
		int32_t m_iSec;
		int32_t m_iLine;	//every bit represent one line from low to high
	};
protected:
	CCalR1ATxAttThread();           // 动态创建所使用的受保护的构造函数
public:
	CCalR1ATxAttThread(CWnd *pParent);
	virtual ~CCalR1ATxAttThread();
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual int Run();
	int CalTxAtt();
	static double CalSARef(double dPeak);
	static BOOL ShowLine(int32_t iLine,int32_t iMask);
};

class CExportR1ATxAttThread : public CCSEWinThread
{
	DECLARE_DYNCREATE(CExportR1ATxAttThread)
protected:
	CExportR1ATxAttThread();           // 动态创建所使用的受保护的构造函数
public:
	CExportR1ATxAttThread(CWnd *pParent);
	virtual ~CExportR1ATxAttThread();
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual int Run();
};
