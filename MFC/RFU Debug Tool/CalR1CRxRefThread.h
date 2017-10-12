#pragma once
#include "CalBaseThread.h"

class CCalR1CRxRefThread : public CCalBaseThread
{
	DECLARE_DYNCREATE(CCalR1CRxRefThread)
public:
	typedef struct ResultOP : public RxRefOPTableR1C::DataF {
		int32_t m_iIdx;
		int32_t m_iRefIdx;
		int32_t m_iSec;
	}ResultOP;
	typedef struct ResultIO : public RxRefIOTableR1C::DataF {
		int32_t m_iIdx;
		int32_t m_iRefIdx;
		int32_t m_iSec;
	}ResultIO;
protected:
	CCalR1CRxRefThread();           // 动态创建所使用的受保护的构造函数
	virtual ~CCalR1CRxRefThread();
public:
	CCalR1CRxRefThread(CWnd *pParent);
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual int Run();
	int CalRxRef();
	void AjustSG(double dFreq,int32_t iPower);
private:
	int32_t m_iSGPow;
	double m_dPMIL;
};

class CExportR1CRxRefThread : public CCSEWinThread
{
	DECLARE_DYNCREATE(CExportR1CRxRefThread)

protected:
	CExportR1CRxRefThread();           // 动态创建所使用的受保护的构造函数
public:
	CExportR1CRxRefThread(CWnd *pParent);
	virtual ~CExportR1CRxRefThread();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual int Run();
};
