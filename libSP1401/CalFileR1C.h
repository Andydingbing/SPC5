#ifndef __CAL_FILE_R1C_H__
#define __CAL_FILE_R1C_H__

#include "ICalFile.h"

class CalFileR1C : public ICalFile
{
    DECLARE_CAL_FILE()
private:
	CalFileR1C();	//for preventing stack overflow
    CalFileR1C(uint32_t uiRfIdx,uint32_t uiRfuIdx);
	virtual ~CalFileR1C();
public:
	friend class CfvCtrlR1C;
	friend class CSP1401R1C;
public:
	virtual int32_t Open();
	virtual int32_t SetBW(BW bw);
public:
	TxLOLeakageTableR1C *m_pTxLOLeak;
	TxSidebandTableR1C	*m_pTxSideband;
	TxPowerOPTableR1C	*m_pTxPowerOP;
	TxPowerIOTableR1C	*m_pTxPowerIO;
	TxAttOPTableR1C		*m_pTxAttOP;
	TxAttIOTableR1C		*m_pTxAttIO;
	RxRefOPTableR1C		*m_pRxRefOP;
	RxRefIOTableR1C		*m_pRxRefIO;
	RxAttOPTableR1C		*m_pRxAttOP;
	RxAttIOTableR1C		*m_pRxAttIO;
	TxFilter_80M_TableR1C	*m_pTxFilter_80M;
	TxFilter_160M_TableR1C	*m_pTxFilter_160M;
	RxFilter_80M_TableR1C	*m_pRxFilter_80M;
	RxFilter_160M_TableR1C	*m_pRxFilter_160M;
	TxFilterOffTableR1C		*m_pTxFilOffOP_80M;
	TxFilterOffTableR1C		*m_pTxFilOffIO_80M;
	RxFilterOffTableR1C		*m_pRxFilOffOP_80M;
	RxFilterOffTableR1C		*m_pRxFilOffIO_80M;
public:
	TxRFFr_0_TableR1C *m_pTxRFFr0;	//<=3100
	TxRFFr_1_TableR1C *m_pTxRFFr1;	//>3100
	TxIFFrTableR1C *m_pTxIFFr;
	RxRFFrTableR1C *m_pRxRFFr;
	RxIFFrTableR1C *m_pRxIFFr;
};
#endif
