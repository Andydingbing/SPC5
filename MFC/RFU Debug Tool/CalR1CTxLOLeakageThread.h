#pragma once
#include "CalBaseThread.h"

class CCalR1CTxLOLeakageThread : public CCalBaseThread
{
	DECLARE_DYNCREATE(CCalR1CTxLOLeakageThread)
public:
	typedef struct Result : public TxLOLeakageTableR1C::DataF {
		int32_t m_iIdx;
	}Result;
protected:
	CCalR1CTxLOLeakageThread();           // 动态创建所使用的受保护的构造函数
	virtual ~CCalR1CTxLOLeakageThread();
public:
	CCalR1CTxLOLeakageThread(CWnd *pParent);
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
	int64_t GetMinDCOffsetI_Rx(CSP1401R1C *pSP1401,CSP2401R1A *pSP2401,int16_t iStep,int16_t iLRCoef,int16_t *pDC_I_L,int16_t *pDC_I_R,int16_t *pDC_I_M,int16_t *pDC_Q_M,int64_t *pAD);
	int64_t GetMinDCOffsetQ_Rx(CSP1401R1C *pSP1401,CSP2401R1A *pSP2401,int16_t iStep,int16_t iLRCoef,int16_t *pDC_Q_L,int16_t *pDC_Q_R,int16_t *pDC_I_M,int16_t *pDC_Q_M,int64_t *pAD);
};

class CExportR1CTxLoLeakageThread : public CCSEWinThread
{
	DECLARE_DYNCREATE(CExportR1CTxLoLeakageThread)

protected:
	CExportR1CTxLoLeakageThread();           // 动态创建所使用的受保护的构造函数
public:
	CExportR1CTxLoLeakageThread(CWnd *pParent);
	virtual ~CExportR1CTxLoLeakageThread();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual int Run();
};

