#pragma once
#include "CalBaseThread.h"

class CCalR1CTxSidebandThread : public CCalBaseThread
{
	DECLARE_DYNCREATE(CCalR1CTxSidebandThread)
public:
	typedef struct Result : public TxSidebandTableR1C::DataF {
		int32_t m_iIdx;
	}Result;
protected:
	CCalR1CTxSidebandThread();           // 动态创建所使用的受保护的构造函数
	virtual ~CCalR1CTxSidebandThread();
public:
	CCalR1CTxSidebandThread(CWnd *pParent);
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual int Run();
	int CalTxSideband();
	double DerivativeTh_SA(CSP2401R1A *pSP2401,double dTh,double dDelta,double &dPower);
	double DerivativeAmI_SA(CSP2401R1A *pSP2401,uint16_t uiAmI,uint16_t uiAmQ,uint16_t uiDelta,double &dPower);
	double DerivativeAmQ_SA(CSP2401R1A *pSP2401,uint16_t uiAmI,uint16_t uiAmQ,uint16_t uiDelta,double &dPower);
	double GetMinThI_Rx(CSP1401R1C *pSP1401,CSP2401R1A *pSP2401,double dStep,double dLRCoef,double *pThL,double *pThR,double *pThM,double *pPower);
	double GetMinAmI_Rx(CSP1401R1C *pSP1401,CSP2401R1A *pSP2401,uint16_t uiStep,uint16_t uiLRCoef,uint16_t *pAm_I_L,uint16_t *pAm_I_R,uint16_t *pAm_I_M,uint16_t *pAm_Q_M,double *pPower);
	double GetMinAmQ_Rx(CSP1401R1C *pSP1401,CSP2401R1A *pSP2401,uint16_t uiStep,uint16_t uiLRCoef,uint16_t *pAm_Q_L,uint16_t *pAm_Q_R,uint16_t *pAm_I_M,uint16_t *pAm_Q_M,double *pPower);
	void MeasOnce_Rx(CSP1401R1C *pSP1401,double *pPowerAvg);
};

class CExportR1CTxSidebandThread : public CCSEWinThread
{
	DECLARE_DYNCREATE(CExportR1CTxSidebandThread)
protected:
	CExportR1CTxSidebandThread();           // 动态创建所使用的受保护的构造函数
public:
	CExportR1CTxSidebandThread(CWnd *pParent);
	virtual ~CExportR1CTxSidebandThread();
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual int Run();
};
