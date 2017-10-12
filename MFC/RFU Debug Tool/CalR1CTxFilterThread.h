#pragma once
#include "CalBaseThread.h"

class CCalR1CTxFilterThread : public CCalBaseThread
{
	DECLARE_DYNCREATE(CCalR1CTxFilterThread)
public:
	typedef struct Result_80 : public TxFilter_80M_TableR1C::DataF {
		int32_t m_iIdx;
	}Result_80;
	typedef struct Result_160 : public TxFilter_160M_TableR1C::DataF {
		int32_t m_iIdx;
	}Result_160;
protected:
	CCalR1CTxFilterThread();           // 动态创建所使用的受保护的构造函数
public:
	CCalR1CTxFilterThread(CWnd *pParent);
	virtual ~CCalR1CTxFilterThread();
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual int Run();
	int CalTxFilter();
};

class CExportR1CTxFilterThread : public CCSEWinThread
{
	DECLARE_DYNCREATE(CExportR1CTxFilterThread)

protected:
	CExportR1CTxFilterThread();           // 动态创建所使用的受保护的构造函数
public:
	CExportR1CTxFilterThread(CWnd *pParent);
	virtual ~CExportR1CTxFilterThread();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual int Run();
};