#pragma once
#include "CalBaseThread.h"

class CCalR1ARxRefThread : public CCalBaseThread
{
	DECLARE_DYNCREATE(CCalR1ARxRefThread)
public:
	typedef struct CalRxRefResult : public RxRefTableR1A::DataF {
		int32_t m_iIdx;
		int32_t m_iRefIdxOP;
		int32_t m_iRefIdxIO;
	}CalRxRefResult;
protected:
	CCalR1ARxRefThread();           // 动态创建所使用的受保护的构造函数
	virtual ~CCalR1ARxRefThread();
public:
	CCalR1ARxRefThread(CWnd *pParent);
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

class CExportR1ARxRefThread : public CCSEWinThread
{
	DECLARE_DYNCREATE(CExportR1ARxRefThread)

protected:
	CExportR1ARxRefThread();           // 动态创建所使用的受保护的构造函数
public:
	CExportR1ARxRefThread(CWnd *pParent);
	virtual ~CExportR1ARxRefThread();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual int Run();
};
