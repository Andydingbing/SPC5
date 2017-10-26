#ifndef __IQ_BUF_MGR_H__
#define __IQ_BUF_MGR_H__
#include "stdint_common.h"

#define IQBUF CIQBufMgr::Instance()

class CIQBufMgr
{
private:
	CIQBufMgr();
	~CIQBufMgr();
	CIQBufMgr & operator = (CIQBufMgr &);
public:
    static CIQBufMgr& Instance();
	void New(uint32_t uiSamples);
    void SetSr(uint32_t uiSr);
    uint32_t GetSr();
	int16_t *GetI() const;
	int16_t *GetQ() const;
	int16_t GetI(uint32_t uiIdx) const;
	int16_t GetQ(uint32_t uiIdx) const;
    double *GetNormFreq() const;
    double *GetPower() const;
    double GetNormFreq(uint32_t uiIdx) const;
    double GetPower(uint32_t uiIdx) const;
	void _2Ptr(int32_t* pI,int32_t* pQ);
    void DFT();
    uint32_t TDGetSamples();
    uint32_t DFTGetPts();
    uint32_t DFTGetPts(uint32_t uiSr,uint32_t uiSamples);
private:
	int16_t *m_pI;
	int16_t *m_pQ;
    double *m_pNormFreq;
    double *m_pPower;
    uint32_t m_uiSr;
	uint32_t m_uiSamples;
};

#endif
