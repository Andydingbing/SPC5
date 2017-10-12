#pragma once
#include "CalBaseThread.h"

class CCalR1CRxFilterThread : public CCalBaseThread
{
	DECLARE_DYNCREATE(CCalR1CRxFilterThread)
public:
	typedef struct Result_80 : public RxFilter_80M_TableR1C::DataF {
		int32_t m_iIdx;
	}Result_80;
	typedef struct Result_160 : public RxFilter_160M_TableR1C::DataF {
		int32_t m_iIdx;
	}Result_160;
protected:
	CCalR1CRxFilterThread();           // 动态创建所使用的受保护的构造函数
	virtual ~CCalR1CRxFilterThread();
public:
	CCalR1CRxFilterThread(CWnd *pParent);
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual int Run();
	int CalRxFilter();
	void AjustSG(double dFreq,int32_t iPower);
	double GetRxPower(CSP1401R1C *pSP1401);
};

class CExportR1CRxFilterThread : public CCSEWinThread
{
	DECLARE_DYNCREATE(CExportR1CRxFilterThread)

protected:
	CExportR1CRxFilterThread();           // 动态创建所使用的受保护的构造函数
public:
	CExportR1CRxFilterThread(CWnd *pParent);
	virtual ~CExportR1CRxFilterThread();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual int Run();
};