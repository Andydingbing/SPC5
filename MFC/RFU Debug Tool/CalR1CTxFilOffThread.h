#pragma once
#include "CalBaseThread.h"

class CCalR1CTxFilOffThread : public CCalBaseThread
{
	DECLARE_DYNCREATE(CCalR1CTxFilOffThread)
public:
	typedef struct Result : public TxFilterOffTableR1C::DataF {
		int32_t m_iIdx;
		int32_t m_iSec;
	}Result;
protected:
	CCalR1CTxFilOffThread();           // 动态创建所使用的受保护的构造函数
public:	
	CCalR1CTxFilOffThread(CWnd *pParent);
	virtual ~CCalR1CTxFilOffThread();
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual int Run();
	int CalTxFilOff();
};

class CExportR1CTxFilOffThread : public CCSEWinThread
{
	DECLARE_DYNCREATE(CExportR1CTxFilOffThread)

protected:
	CExportR1CTxFilOffThread();           // 动态创建所使用的受保护的构造函数
public:
	CExportR1CTxFilOffThread(CWnd *pParent);
	virtual ~CExportR1CTxFilOffThread();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual int Run();
};