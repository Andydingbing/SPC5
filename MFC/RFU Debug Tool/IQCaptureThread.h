#pragma once
#include "CSEWinThread.h"

class CIQCaptureThread : public CCSEWinThread
{
	DECLARE_DYNCREATE(CIQCaptureThread)
public:
	typedef struct Result {
		int32_t m_iPts;
		double *m_pXTD;		//time domain
		int16_t *m_pI;
		int16_t *m_pQ;
		double *m_pXDFT;	//dft
		double *m_pdB;
	}Result;
protected:
	CIQCaptureThread();           // 动态创建所使用的受保护的构造函数
	virtual ~CIQCaptureThread();
public:
	CIQCaptureThread(CWnd *pParent);
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	DECLARE_MESSAGE_MAP()

public:
	virtual int Run();
};

typedef struct PowMeasInfo {
	CSP1401R1A *RF;
	CSP1401R1A::PowerMeasSrcs m_PMSrc;
}PowMeasInfo;

class CPowMeasThread : public CWinThread
{
	DECLARE_DYNCREATE(CPowMeasThread)

protected:
	CPowMeasThread();           // 动态创建所使用的受保护的构造函数
	virtual ~CPowMeasThread();
public:
	CPowMeasThread(CWnd *pParent,PowMeasInfo *pInfo);
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	DECLARE_MESSAGE_MAP()
private:
	CWnd *ParentWnd;
	PowMeasInfo m_powmeasinfo;
public:
	virtual int Run();
};

