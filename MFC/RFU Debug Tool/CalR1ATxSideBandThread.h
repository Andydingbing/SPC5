#pragma once
#include "CalBaseThread.h"

class CCalR1ATxSideBandThread : public CCalBaseThread
{
	DECLARE_DYNCREATE(CCalR1ATxSideBandThread)
public:
	typedef struct Result : public TxSidebandTableR1A::DataF {
		int32_t m_iIdx;
	}Result;
protected:
	CCalR1ATxSideBandThread();           // 动态创建所使用的受保护的构造函数
	virtual ~CCalR1ATxSideBandThread();
public:
	CCalR1ATxSideBandThread(CWnd *pParent);
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual int Run();
	int CalTxSideBand();
	double DerivativeTh_SA(CSP2401R1A *pSP2401,double dTh,double dDelta,double &dPower);
	double DerivativeAmI_SA(CSP2401R1A *pSP2401,uint16_t uiAmI,uint16_t uiAmQ,uint16_t uiDelta,double &dPower);
	double DerivativeAmQ_SA(CSP2401R1A *pSP2401,uint16_t uiAmI,uint16_t uiAmQ,uint16_t uiDelta,double &dPower);
	void GetMinThI_Rx(CSP1401R1A *pSP1401,CSP2401R1A *pSP2401,double dStep,double dLRCoef,double *pThL,double *pThR,double *pThM,double *pPower);
	void GetMinAmI_Rx(CSP1401R1A *pSP1401,CSP2401R1A *pSP2401,uint16_t uiStep,uint16_t uiLRCoef,uint16_t *pAm_I_L,uint16_t *pAm_I_R,uint16_t *pAm_I_M,uint16_t *pAm_Q_M,double *pPower);
	void GetMinAmQ_Rx(CSP1401R1A *pSP1401,CSP2401R1A *pSP2401,uint16_t uiStep,uint16_t uiLRCoef,uint16_t *pAm_Q_L,uint16_t *pAm_Q_R,uint16_t *pAm_I_M,uint16_t *pAm_Q_M,double *pPower);
	void MeasOnce_Rx(CSP1401R1A *pSP1401,double *pPowerAvg);
};

class CExportR1ATxSideBandThread : public CCSEWinThread
{
	DECLARE_DYNCREATE(CExportR1ATxSideBandThread)
protected:
	CExportR1ATxSideBandThread();           // 动态创建所使用的受保护的构造函数
public:
	CExportR1ATxSideBandThread(CWnd *pParent);
	virtual ~CExportR1ATxSideBandThread();
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual int Run();
};
