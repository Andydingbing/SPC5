#ifndef __CAL_FILE_R1A_H__
#define __CAL_FILE_R1A_H__

#include "ICalFile.h"

class CalFileR1A : public ICalFile
{
    DECLARE_CAL_FILE()
private:
	CalFileR1A();	//for preventing stack overflow
    CalFileR1A(uint32_t uiRfIdx,uint32_t uiRfuIdx);
	virtual ~CalFileR1A();
public:
	friend class CSP1401R1A;
	friend class CSP3301;
public:
	virtual int32_t Open();
	int32_t GetItemSizeV0000(ICalFile::CalItem Item,uint32_t &dwPos,uint32_t &uiSize);
	int32_t GetItemSizeV0001(ICalFile::CalItem Item,uint32_t &dwPos,uint32_t &uiSize);
	int32_t GetItemSizeV0002(ICalFile::CalItem Item,uint32_t &dwPos,uint32_t &uiSize);
	int32_t GetItemSizeV0004(ICalFile::CalItem Item,uint32_t &dwPos,uint32_t &uiSize);
public:
	void GetTxAtt(uint64_t uiFreq,IOMode Mode,float *pAtt);
	void GetTxAtt(uint64_t uiFreq,IOMode Mode,int32_t iAtt,double &dOffset);
    void GetTxPower(uint64_t uiFreq,double dPower,IOMode Mode,double &dBBGain,int32_t &iAtt1,int32_t &iAtt2,SP1401::TxPAAtt &PAAtt,int32_t &iAtt3);
public:
    TxSidebandTableR1A  *m_pTxSideband;
	TxLOLeakageTableR1A *m_pTxLOLeak;
	TxAttTableR1A		*m_pTxAttOP;
	TxPowerTableR1A     *m_pTxPowerOP;
	TxPowerTableR1A		*m_pTxPowerIO;
	RxRefTableR1A		*m_pRxRef;
	X9119TableR1A		*m_pX9119;
	TxAttTableR1A		*m_pTxAttIO;
};

#endif
