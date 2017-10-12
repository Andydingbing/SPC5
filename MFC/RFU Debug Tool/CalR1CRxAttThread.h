#pragma once
#include "CalBaseThread.h"

class CCalR1CRxAttThread : public CCalBaseThread
{
	DECLARE_DYNCREATE(CCalR1CRxAttThread)
public:
	typedef struct ResultOP : public RxAttOPTableR1C::DataF {
		int32_t m_iIdx;
		int32_t m_iSec;
	}ResultOP;
	typedef struct ResultIO : public RxAttIOTableR1C::DataF {
		int32_t m_iIdx;
		int32_t m_iSec;
	}ResultIO;
protected:
	CCalR1CRxAttThread();           // 动态创建所使用的受保护的构造函数
public:
	CCalR1CRxAttThread(CWnd *pParent);
	virtual ~CCalR1CRxAttThread();
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual int Run();
	int CalRxAtt();
	void SetRxState(CSP1401R1C *pSP1401,CSP2401R1A *pSP2401,RxRefOPTableR1C::RxStateM State);
	void AjustSG(double dFreq,int32_t iPower);
private:
	int32_t m_iSGPow;
	double m_dPMIL;
};

class CExportR1CRxAttThread : public CCSEWinThread
{
	DECLARE_DYNCREATE(CExportR1CRxAttThread)
protected:
	CExportR1CRxAttThread();           // 动态创建所使用的受保护的构造函数
public:
	CExportR1CRxAttThread(CWnd *pParent);
	virtual ~CExportR1CRxAttThread();
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual int Run();
};
