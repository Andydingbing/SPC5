#ifndef __VI_MEMIO_H__
#define __VI_MEMIO_H__

#include "visa.h"
#include "stdint_common.h"

class viMemIO
{
public:
	viMemIO(void);
	virtual ~viMemIO(void);
	viMemIO & operator = (viMemIO &);
public:
	int32_t Alloc(uint32_t uiSize);
	int32_t Release();
public:
	virtual int32_t Write8(uint8_t *pBuf,uint32_t uiSize);
	virtual int32_t Write16(uint16_t *pBuf,uint32_t uiSize);
	virtual int32_t Write32(uint32_t *pBuf,uint32_t uiSize);
	virtual int32_t Read8(uint8_t *pBuf,uint32_t uiSize);
	virtual int32_t Read16(uint16_t *pBuf,uint32_t uiSize);
	virtual int32_t Read32(uint32_t *pBuf,uint32_t uiSize);
public:
	ViSession    m_viMem;
	ViBusAddress m_PhyAddr;
	ViBusAddress m_PhyAddrBfrAlign;
};


int32_t _VI_FUNCH pxi_isr(ViSession vi,ViEventType etype,ViEvent eventContext,ViAddr userHandle);

#endif
