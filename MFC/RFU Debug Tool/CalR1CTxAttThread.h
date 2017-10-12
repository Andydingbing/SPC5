#pragma once
#include "CalBaseThread.h"

class CCalR1CTxAttThread : public CCalBaseThread
{
	DECLARE_DYNCREATE(CCalR1CTxAttThread)
public:
	typedef struct ResultOP : public TxAttOPTableR1C::DataF {
		int32_t m_iIdx;
		int32_t m_iSec;
		int32_t m_iIdxOffsetStar;
		int32_t m_iIdxOffsetStop;
	}ResultOP;
	typedef struct ResultIO : public TxAttIOTableR1C::DataF {
		int32_t m_iIdx;
		int32_t m_iSec;
		int32_t m_iIdxOffsetStar;
		int32_t m_iIdxOffsetStop;
	}ResultIO;
protected:
	CCalR1CTxAttThread();           // 动态创建所使用的受保护的构造函数
public:	
	CCalR1CTxAttThread(CWnd *pParent);
	virtual ~CCalR1CTxAttThread();
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual int Run();
	int CalTxAtt();
	int CalOneSegOP(CalIOMode CallingMode,CSP1401R1C *pSP1401,CSP2401R1A *pSP2401,int32_t iPts,uint64_t *pFreq,uint64_t *pFreqStar,uint64_t *pFreqStop,int32_t *pIdxOffsetStar,double dPowerStar,double dPowerStop,double dPowerStep);
	int CalOneSegIO(CalIOMode CallingMode,CSP1401R1C *pSP1401,CSP2401R1A *pSP2401,int32_t iPts,uint64_t *pFreq,uint64_t *pFreqStar,uint64_t *pFreqStop,int32_t *pIdxOffsetStar,double dPowerStar,double dPowerStop,double dPowerStep);
};

class CExportR1CTxAttThread : public CCSEWinThread
{
	DECLARE_DYNCREATE(CExportR1CTxAttThread)

protected:
	CExportR1CTxAttThread();           // 动态创建所使用的受保护的构造函数
public:
	CExportR1CTxAttThread(CWnd *pParent);
	virtual ~CExportR1CTxAttThread();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual int Run();
};