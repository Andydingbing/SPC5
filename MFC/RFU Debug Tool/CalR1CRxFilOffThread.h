#pragma once
#include "CalBaseThread.h"

class CCalR1CRxFilOffThread : public CCalBaseThread
{
	DECLARE_DYNCREATE(CCalR1CRxFilOffThread)
public:
	typedef struct Result : public RxFilterOffTableR1C::DataF {
		double  m_ddBc[3];
		int32_t m_iIdx;
		int32_t m_iSec;
		Result();
	}Result;
protected:
	CCalR1CRxFilOffThread();           // 动态创建所使用的受保护的构造函数
public:	
	CCalR1CRxFilOffThread(CWnd *pParent);
	virtual ~CCalR1CRxFilOffThread();
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual int Run();
	int CalRxFilOff();
};

class CExportR1CRxFilOffThread : public CCSEWinThread
{
	DECLARE_DYNCREATE(CExportR1CRxFilOffThread)

protected:
	CExportR1CRxFilOffThread();           // 动态创建所使用的受保护的构造函数
public:
	CExportR1CRxFilOffThread(CWnd *pParent);
	virtual ~CExportR1CRxFilOffThread();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual int Run();
};