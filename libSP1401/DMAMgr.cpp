#include "DMAMgr.h"
#include "RegDef.h"
#include "algorithm.h"
#include "libLog.h"

DMAMgr::DMAMgr()
{
	m_DMAR = new viMemIO;
    for (uint32_t i = 0;i < R_BLOCKS;i ++)
		m_DMAW[i] = new viMemIO;
	m_pUsrSpc = new uint32_t[T_BLOCK_SAMPLES >= R_BLOCK_SAMPLES ? T_BLOCK_SAMPLES : R_BLOCK_SAMPLES];
}

DMAMgr::~DMAMgr()
{
	if (m_DMAR) {
		m_DMAR->Release();
		delete m_DMAR;
		m_DMAR = NULL;
	}
    for (uint32_t i = 0;i < R_BLOCKS;i ++) {
		if (m_DMAW[i]) {
			m_DMAW[i]->Release();
			delete m_DMAW[i];
			m_DMAW[i] = NULL;
		}
	}
	SAFE_DELETE(m_pUsrSpc);
}

DMAMgr &DMAMgr::operator = (DMAMgr &Mgr)
{
	return Mgr;
}

DMAMgr &DMAMgr::Instance()
{
    static DMAMgr g_DMAMgr = DMAMgr();
    return g_DMAMgr;
}

int32_t DMAMgr::FpgaRead(viPCIDev *pK7,uint32_t *pBuf,uint32_t uiLength,float *pTime)
{
    RFU_K7_REG_DECLARE(0x0007);
    RFU_K7_REG_DECLARE(0x0008);
    RFU_K7_REG_DECLARE(0x0009);
    RFU_K7_REG_DECLARE(0x0020);
    RFU_K7_REG_DECLARE(0x0021);

    viPCIDev *m_pK7 = pK7;
	bool bComp = false;
    double dTimeStar = Log.GetTimeStamp();
	double dTimeStop = 0.0;

    INT_CHECK(m_DMAR->Write32(pBuf,uiLength));

//	Log.stdprintf("fpga prepare read samples:%d\n",uiLength);

    RFU_K7_REG(0x0008).tlp_size = 16;
    RFU_K7_W(0x0008);

    RFU_K7_REG(0x0009).tlp_count = uiLength / 16;
    RFU_K7_W(0x0009);

    INT_CHECK(Reset(pK7));
    INT_CHECK(RStart(pK7));

	/* wait interrupt
	for (int i = 0; i < 3; i++) {
		ret = WaitForSingleObject(g_Event, 1000);
		Log.stdprintf("WaitForSingleObject:%d\n",ret);

		CSE_K7_R(0x0021);
		Log.stdprintf("REG(0x0021) = %d\n",REG(0x0021).dma_rd_counter);
		if (ret != WAIT_TIMEOUT)
		break;
	}*/

	unsigned uiBytesCnt[5] = {0};
	while(1) {
		for (int i = 0;i < 5;i ++) {
            RFU_K7_R(0x0021);
            uiBytesCnt[i] = RFU_K7_REG(0x0021).dma_rd_counter;
			if (uiBytesCnt[i] == uiLength) {
				bComp = true;
				break;
			}
		}
		if (bComp)
			break;
		else if (uiBytesCnt[4] == uiBytesCnt[3]) {
            Log.SetLastError("stop@sample:%d",uiBytesCnt[4]);
            Log.stdprintf("\n%s\n",Log.GetLastError());
			return -1;
		}
	}

    dTimeStop = Log.GetTimeStamp();
	if (pTime)
		*pTime = (float)(dTimeStop - dTimeStar);

//	CSE_K7_R(0x0020);									//fpga¼ÆËãµÄDMAÊ±¼ä
//	Log.stdprintf("timer = %lu\n", REG(0x0020).dma_rd_timer);
	return 0;
}

int32_t DMAMgr::FpgaReadAll(viPCIDev *pK7,uint32_t *pBuf,uint32_t uiLength)
{
    RFU_K7_REG_DECLARE(0x002f);

    viPCIDev *m_pK7 = pK7;
	uint32_t uiSplsTotal = uiLength;
	uint32_t uiSplsTransed = 0;
	uint32_t uiSplsLeft = uiSplsTotal;
	uint32_t uiSplsTransing = 0;
	uint32_t uiCnt = 0;
	
    RFU_K7_REG(0x002f).samples = uiSplsTotal;
    RFU_K7_W(0x002f);

	while(uiSplsLeft > 0)
	{
		uiCnt ++;
		uiSplsTransing = uiSplsLeft > T_BLOCK_SAMPLES ? T_BLOCK_SAMPLES : uiSplsLeft;

        Log.stdprintf("dma%d ready:samples:%d\n",uiCnt,uiSplsTransing);
        INT_CHECK(FpgaRead(pK7,&pBuf[uiSplsTransed],uiSplsTransing,NULL));
        Log.stdprintf("dma%d done\n",uiCnt);

		uiSplsTransed += uiSplsTransing;
		uiSplsLeft = uiSplsTotal - uiSplsTransed;
	}
    Log.stdprintf("all done\n");
	return 0;
}

int32_t DMAMgr::WDumpToBuf(int32_t *pData,uint32_t uiSamples)
{
	int	iDataIdx = 0;
	int iBlocksTotal = uiSamples / R_BLOCK_SAMPLES;
	int	iSplsLeft = uiSamples;
	int iSplsReading = 0;
	int iSplsLastBlock = uiSamples % R_BLOCK_SAMPLES;

	if (iSplsLastBlock != 0)
		iBlocksTotal ++;

	for (int i = 0;i < iBlocksTotal;i ++) {
        iSplsReading = (iSplsLeft > int(R_BLOCK_SAMPLES) ? int(R_BLOCK_SAMPLES) : iSplsLeft);
        INT_CHECK(m_DMAW[i]->Read32(m_pUsrSpc,iSplsReading));

		for (int j = 0;j < iSplsReading;j ++) {
			iDataIdx = i * R_BLOCK_SAMPLES + j;
			pData[iDataIdx] = m_pUsrSpc[j];
		}
		iSplsLeft -= iSplsReading;
	}
	return 0;
}

int32_t DMAMgr::WDumpToBufCh(uint32_t uiRfIdx,int32_t *pData,uint32_t uiSamples,int32_t *pDataInterCh)
{
	uiSamples *= 2;
	int	iDataIdx = 0;
	int iWDataIdx = uiRfIdx % 2 + 1;
	int iWDataIdxInterCh = (0 == iWDataIdx ? 1 : 0);
	int iBlocksTotal = uiSamples / R_BLOCK_SAMPLES;
	int iSplsLeft = uiSamples;
	int iSplsReading = 0;
	int iSplsLastBlock = uiSamples % R_BLOCK_SAMPLES;

	if (iSplsLastBlock != 0)
		iBlocksTotal ++;

	WData Data;
	WData DataInterCh;

	for (int i = 0;i < iBlocksTotal;i ++) {
        iSplsReading = (iSplsLeft > int(R_BLOCK_SAMPLES) ? int(R_BLOCK_SAMPLES) : iSplsLeft);
        INT_CHECK(m_DMAW[i]->Read32(m_pUsrSpc,iSplsReading));

		for (int j = 0;j < iSplsReading;j += 2) {
			iDataIdx = (i * R_BLOCK_SAMPLES + j) / 2;

			Data = ((WPackage *)(&m_pUsrSpc[j]))->m_Pack[iWDataIdx];
			pData[iDataIdx] = *(int32_t *)&Data;

			if (pDataInterCh) {
				DataInterCh = ((WPackage *)(&m_pUsrSpc[j]))->m_Pack[iWDataIdxInterCh];
				pDataInterCh[iDataIdx] = *(int32_t *)&DataInterCh;
			}
		}
		iSplsLeft -= iSplsReading;
	}
	return 0;
}

int32_t DMAMgr::WDumpToFileCh(uint32_t uiRfIdx,char *pPath,uint32_t uiSamples,char *pPathInterCh)
{
	uiSamples *= 2;
	int iWDataIdx = uiRfIdx % 2 + 1;
	int iWDataIdxInterCh = (0 == iWDataIdx ? 1 : 0);
	int iBlocksTotal = uiSamples / R_BLOCK_SAMPLES;
	int iSplsLeft = uiSamples;
	int iSplsReading = 0;
	int iSplsLastBlock = uiSamples % R_BLOCK_SAMPLES;

	if (iSplsLastBlock != 0)
		iBlocksTotal ++;

	FILE *fp = fopen(pPath,"w");
	if (NULL == fp) {
        Log.SetLastError("can't open %s",pPath);
		return -1;
	}
	FILE *fp_interch = NULL;
	if (pPathInterCh) {
		fp_interch = fopen(pPathInterCh,"w");
		if (NULL == fp_interch) {
            Log.SetLastError("can't open %s",pPathInterCh);
			return -1;
		}
	}

	WData Data;
	WData DataInterCh;
	int32_t iData;
	int32_t iDataInterCh;

	for (int i = 0;i < iBlocksTotal;i ++) {
        iSplsReading = (iSplsLeft > int(R_BLOCK_SAMPLES) ? int(R_BLOCK_SAMPLES) : iSplsLeft);
        INT_CHECK(m_DMAW[i]->Read32(m_pUsrSpc,iSplsReading));

		for (int j = 0;j < iSplsReading;j += 2) {
			Data = ((WPackage *)(&m_pUsrSpc[j]))->m_Pack[iWDataIdx];
			iData = *(int32_t *)&Data;
			fprintf(fp,"%d\n",iData);

			if (pPathInterCh) {
				DataInterCh = ((WPackage *)(&m_pUsrSpc[j]))->m_Pack[iWDataIdxInterCh];
				iDataInterCh = *(int32_t *)&DataInterCh;
				fprintf(fp_interch,"%d\n",iDataInterCh);
			}
		}
		iSplsLeft -= iSplsReading;
	}
	fclose(fp);
	if (fp_interch)
		fclose(fp_interch);

	return 0;
}

int32_t DMAMgr::IQToBuf(uint32_t uiRfIdx,int16_t *pI,int16_t *pQ,uint32_t uiSamples,int16_t *pIInterCh,int16_t *pQInterCh)
{
	uiSamples *= 2;
	int	iDataIdx = 0;
	int iWDataIdx = uiRfIdx % 2 + 1;
	int iWDataIdxInterCh = (0 == iWDataIdx ? 1 : 0);
	int iBlocksTotal = uiSamples / R_BLOCK_SAMPLES;
	int iSplsLeft = uiSamples;
	int iSplsReading = 0;
	int iSplsLastBlock = uiSamples % R_BLOCK_SAMPLES;

	if (iSplsLastBlock != 0)
		iBlocksTotal ++;

	WData Data;
	WData DataInterCh;

	for (int i = 0;i < iBlocksTotal;i ++) {
        iSplsReading = (iSplsLeft > int(R_BLOCK_SAMPLES) ? int(R_BLOCK_SAMPLES) : iSplsLeft);
        INT_CHECK(m_DMAW[i]->Read32(m_pUsrSpc,iSplsReading));

		for (int j = 0;j < iSplsReading;j += 2) {
			iDataIdx = (i * R_BLOCK_SAMPLES + j) / 2;

			Data = ((WPackage *)(&m_pUsrSpc[j]))->m_Pack[iWDataIdx];
			pI[iDataIdx] = (int16_t)(Data.m_I);
			pQ[iDataIdx] = (int16_t)(Data.m_Q);

			DataInterCh = ((WPackage *)(&m_pUsrSpc[j]))->m_Pack[iWDataIdxInterCh];
			if (pIInterCh)
				pIInterCh[iDataIdx] = DataInterCh.m_I;
			if (pQInterCh)
				pQInterCh[iDataIdx] = DataInterCh.m_Q;
		}
		iSplsLeft -= iSplsReading;
	}
	return 0;
}

int32_t	DMAMgr::IQToFile(uint32_t uiRfIdx,char *pPath,uint32_t uiSamples,char *pPathInterCh)
{
    uiSamples *= 2;
	int iWDataIdx = uiRfIdx % 2 + 1;
	int iWDataIdxInterCh = (0 == iWDataIdx ? 1 : 0);
	int iBlocksTotal = uiSamples / R_BLOCK_SAMPLES;
	int iSplsLeft = uiSamples;
	int iSplsReading = 0;
	int iSplsLastBlock = uiSamples % R_BLOCK_SAMPLES;

	if (iSplsLastBlock != 0)
		iBlocksTotal ++;

	FILE *fp = fopen(pPath,"w");
	if (NULL == fp) {
        Log.SetLastError("can't open %s",pPath);
		return -1;
	}
	FILE *fp_interch = NULL;
	if (pPathInterCh) {
		fp_interch = fopen(pPathInterCh,"w");
		if (NULL == fp_interch) {
            Log.SetLastError("can't open %s",pPathInterCh);
			return -1;
		}
	}

	WData Data;
	WData DataInterCh;

	for (int i = 0;i < iBlocksTotal;i ++) {
        iSplsReading = (iSplsLeft > int(R_BLOCK_SAMPLES) ? int(R_BLOCK_SAMPLES) : iSplsLeft);
        INT_CHECK(m_DMAW[i]->Read32(m_pUsrSpc,iSplsReading));

        for (int j = 0;j < iSplsReading;j += 2) {
			Data = ((WPackage *)(&m_pUsrSpc[j]))->m_Pack[iWDataIdx];
			fprintf(fp,"%6d,%6d\n",Data.m_I,Data.m_Q);

			if (pPathInterCh) {
				DataInterCh = ((WPackage *)(&m_pUsrSpc[j]))->m_Pack[iWDataIdxInterCh];
				fprintf(fp_interch,"%6d,%6d\n",DataInterCh.m_I,DataInterCh.m_Q);
			}
		}
		iSplsLeft -= iSplsReading;
	}
	fclose(fp);
	if (fp_interch)
		fclose(fp_interch);

	return 0;
}

int32_t DMAMgr::Reset(viPCIDev *pK7)
{
    viPCIDev *m_pK7 = pK7;
    RFU_K7_REG_DECLARE(0x0000);
    RFU_K7_OP(0x0000);
	return 0;
}

int32_t DMAMgr::RAlloc()
{
    for (uint32_t i = 0; i < T_BLOCK_SAMPLES; i++)
		m_pUsrSpc[i] = 0xffffffff;

	if (m_DMAR->Alloc(T_BLOCK_SAMPLES * 4)) {
        Log.SetLastError("dma read memory alloc 4M error");
	}
    Log.AddMsgList("dmar physical address:%#x",m_DMAR->m_PhyAddr);

	if (m_DMAR->Write32(m_pUsrSpc,T_BLOCK_SAMPLES)) {
        Log.SetLastError("dma read memory write default value error");
		return -1;
	}
	return 0;
}

int32_t DMAMgr::RSendPhyAddr(viPCIDev *pK7)
{
    viPCIDev *m_pK7 = pK7;
    RFU_K7_REG_DECLARE(0x0007);

    RFU_K7_REG(0x0007).adr = (((uint64_t)m_DMAR->m_PhyAddr & 0x000000ffffffffff) >> 8);
    RFU_K7_W(0x0007);
	return 0;
}

int32_t DMAMgr::RStart(viPCIDev *pK7)
{
    viPCIDev *m_pK7 = pK7;
    RFU_K7_REG_DECLARE(0x0001);
    RFU_K7OP(0x0001, fpga_rd_op);
	return 0;
}

int32_t DMAMgr::WAlloc()
{
    for (uint32_t i = 0;i < R_BLOCK_SAMPLES; i++)
		m_pUsrSpc[i] = 0xffffffff;

    for (uint32_t i = 0;i < R_BLOCKS;i ++) {
		if(m_DMAW[i]->Alloc(R_BLOCK_SAMPLES * 4) < 0){
            Log.SetLastError("dma write memory alloc @block%d error",i);
			return -1;
		}
		if(m_DMAW[i]->Write32(m_pUsrSpc, R_BLOCK_SAMPLES)) {
            Log.SetLastError("dma write memory write default valued @block%d error",i);
			return -1;
		}
	}
	return 0;
}

int32_t DMAMgr::WSendPhyAddr(viPCIDev *pK7)
{
    viPCIDev *m_pK7 = pK7;
    RFU_K7_REG_DECLARE(0x0016);
    RFU_K7_REG_DECLARE(0x0017);
	uint64_t uiAddrFpga[8] = {0}; //8 divide

    for (uint32_t i = 0;i < R_BLOCKS;i ++) {
		for (int j = 0;j < 8;j ++) {
			uiAddrFpga[j] = (uint64_t)m_DMAW[i]->m_PhyAddr + 128 * 1024 * 4 * j;
			uiAddrFpga[j] = ((uiAddrFpga[j] & 0x000000ffffffffff) >> 8);
            RFU_K7_REG(0x0017).adr = (uint32_t)uiAddrFpga[j];
            RFU_K7_W(0x0017);
            RFU_K7_REG(0x0016).data = 0;
            RFU_K7_W(0x0016);
            RFU_K7_REG(0x0016).data = 0x80000000 | (i*8+j);
            RFU_K7_W(0x0016);
		}
	}
	return 0;
}

int32_t DMAMgr::WStart(viPCIDev *pK7)
{
    viPCIDev *m_pK7 = pK7;
    RFU_K7_REG_DECLARE(0x0001);
    RFU_K7OP(0x0001, fpga_wr_op);
	return 0;
}

uint32_t *DMAMgr::GetUsrSpc() const
{
	return m_pUsrSpc;
}
