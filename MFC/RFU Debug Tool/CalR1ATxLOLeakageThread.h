#pragma once
#include "CalBaseThread.h"

class CCalR1ATxLOLeakageThread : public CCalBaseThread
{
	DECLARE_DYNCREATE(CCalR1ATxLOLeakageThread)
public:
	typedef struct Result : public TxLOLeakageTableR1A::DataF {
		int32_t m_iIdx;
	}Result;
protected:
	CCalR1ATxLOLeakageThread();           // 动态创建所使用的受保护的构造函数
	virtual ~CCalR1ATxLOLeakageThread();
public:
	CCalR1ATxLOLeakageThread(CWnd *pParent);
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual int Run();
	int CalTxLOLeakage();
	double DerivativeDCOffsetI_SA(CSP2401R1A *pSP2401,int16_t iDC_I_m,int16_t iDC_Q_m,int16_t iDelta,double &dPower);
	double DerivativeDCOffsetQ_SA(CSP2401R1A *pSP2401,int16_t iDC_I_m,int16_t iDC_Q_m,int16_t iDelta,double &dPower);
	void GetMinDCOffsetI_Rx(CSP1401R1A *pSP1401,CSP2401R1A *pSP2401,int16_t iStep,int16_t iLRCoef,int16_t *pDC_I_L,int16_t *pDC_I_R,int16_t *pDC_I_M,int16_t *pDC_Q_M,int64_t *pAD);
	void GetMinDCOffsetQ_Rx(CSP1401R1A *pSP1401,CSP2401R1A *pSP2401,int16_t iStep,int16_t iLRCoef,int16_t *pDC_Q_L,int16_t *pDC_Q_R,int16_t *pDC_I_M,int16_t *pDC_Q_M,int64_t *pAD);
};

class CExportR1ATxLoLeakageThread : public CCSEWinThread
{
	DECLARE_DYNCREATE(CExportR1ATxLoLeakageThread)

protected:
	CExportR1ATxLoLeakageThread();           // 动态创建所使用的受保护的构造函数
public:
	CExportR1ATxLoLeakageThread(CWnd *pParent);
	virtual ~CExportR1ATxLoLeakageThread();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual int Run();
};
