#pragma once
#include "CalBaseThread.h"

class CCalR1CTxPowerThread : public CCalBaseThread
{
	DECLARE_DYNCREATE(CCalR1CTxPowerThread)
public:
	typedef struct ResultOP : public TxPowerOPTableR1C::DataF {
		int32_t m_iIdx;
		int32_t m_iSec;
	}ResultOP;
	typedef struct ResultIO : public TxPowerIOTableR1C::DataF {
		int32_t m_iIdx;
		int32_t m_iSec;
	}ResultIO;
protected:
	CCalR1CTxPowerThread();           // 动态创建所使用的受保护的构造函数
public:	
	CCalR1CTxPowerThread(CWnd *pParent);
	virtual ~CCalR1CTxPowerThread();
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual int Run();
	int CalTxPower();
};

class CExportR1CTxPowerThread : public CCSEWinThread
{
	DECLARE_DYNCREATE(CExportR1CTxPowerThread)

protected:
	CExportR1CTxPowerThread();           // 动态创建所使用的受保护的构造函数
public:
	CExportR1CTxPowerThread(CWnd *pParent);
	virtual ~CExportR1CTxPowerThread();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual int Run();
};