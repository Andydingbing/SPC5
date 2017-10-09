#ifndef __DMA_MGR_H__
#define __DMA_MGR_H__

#include "viPCIDev.h"
#include "viMemIO.h"

#define T_BLOCK_SAMPLES (uint32_t)1048576	//fpga dma read, physical memory one block size(1M samples = 4M bytes)
#define R_BLOCK_SAMPLES (uint32_t)1048576	//fpga dma write,physi......
#define R_BLOCKS		(uint32_t)16		//fpga dma write physical memory total blocks
#define DDR DMAMgr::Instance()

class DMAMgr
{
private:
	DMAMgr();
	~DMAMgr();
	DMAMgr & operator = (DMAMgr &);
public:
    static DMAMgr* Instance();
public:
	typedef struct WData {
		short m_I : 16;
		short m_Q : 16;
	}WData;
	typedef struct WPackage {
		WData m_Pack[2];
	}WPackage;
public:

	int32_t FpgaRead(viPCIDev *pK7,uint32_t *pBuf,uint32_t uiLength,float *pTime);
	int32_t FpgaReadAll(viPCIDev *pK7,uint32_t *pBuf,uint32_t uiLength);	//just for testing for particular fpga version
public:
	int32_t WDumpToBuf(int32_t *pData,uint32_t uiSamples);
	int32_t WDumpToBufCh(uint32_t uiRfIdx,int32_t *pData,uint32_t uiSamples,int32_t *pDataInterCh = NULL);
	int32_t WDumpToFileCh(uint32_t uiRfIdx,char *pPath,uint32_t uiSamples,char *pPathInterCh = NULL);
	int32_t IQToBuf(uint32_t uiRfIdx,int16_t *pI,int16_t *pQ,uint32_t uiSamples,int16_t *pIInterCh = NULL,int16_t *pQInterCh = NULL);
	int32_t	IQToFile(uint32_t uiRfIdx,char *pPath,uint32_t uiSamples,char *pPathInterCh = NULL);
public:
	int32_t Reset(viPCIDev *pK7);
	int32_t RAlloc();
	int32_t RSendPhyAddr(viPCIDev *pK7);
	int32_t RStart(viPCIDev *pK7);
	int32_t WAlloc();
	int32_t WSendPhyAddr(viPCIDev *pK7);
	int32_t WStart(viPCIDev *pK7);
public:
	uint32_t *GetUsrSpc() const;
private:
	viMemIO *m_DMAR;	
	viMemIO *m_DMAW[R_BLOCKS];
	uint32_t *m_pUsrSpc;
};
#endif
