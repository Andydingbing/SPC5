#ifndef __IQ_BUF_MGR_H__
#define __IQ_BUF_MGR_H__
#include "stdint.h"

#define IQBUF CIQBufMgr::Instance()

class CIQBufMgr
{
private:
	CIQBufMgr();
	~CIQBufMgr();
	CIQBufMgr & operator = (CIQBufMgr &);
public:
	static CIQBufMgr & Instance();
	void New(uint32_t uiSamples);
	int16_t *GetI() const;
	int16_t *GetQ() const;
	int16_t GetI(uint32_t uiIdx) const;
	int16_t GetQ(uint32_t uiIdx) const;
	void _2Ptr(int32_t* pI,int32_t* pQ);
private:
	int16_t *m_pI;
	int16_t *m_pQ;
	uint32_t m_uiSamples;
};

#endif
