#include "viMemIO.h"
#include "viPCIDev.h"

#ifdef _MSC_VER
#pragma comment(lib,"visa64.lib")
#elif defined __GNUC__
#pragma GCC diagnostic ignored  "-Wunused-parameter"
#endif

viMemIO::viMemIO() : m_viMem(0)
				   , m_PhyAddr(0)
				   , m_PhyAddrBfrAlign(0)
{
	viPCIDev::OpenDefaultRM();
}

viMemIO::~viMemIO()
{
}

viMemIO &viMemIO::operator = (viMemIO &MemIO)
{
	return MemIO;
}

int32_t viMemIO::Alloc(uint32_t uiSize)
{
	if (m_PhyAddr)
		return VI_SUCCESS;

	ViStatus Status = VI_SUCCESS;

	if ((Status = viOpen(viPCIDev::GetDefaultRM(),(ViRsrc)"PXI::MEMACC",VI_NULL,VI_NULL,&m_viMem)) < VI_SUCCESS) {
		m_viMem = 0;
		return Status;
	}

	uiSize += 4096;		// 4k align

	if ((Status = viMemAlloc(m_viMem,uiSize,&m_PhyAddr)) < VI_SUCCESS)
		return Status;

	m_PhyAddrBfrAlign = m_PhyAddr;
	m_PhyAddr = (ViBusAddress)(m_PhyAddr / 4096 + 1) * 4096;	//4k align

	if ((Status = viSetAttribute(m_viMem,VI_ATTR_DEST_INCREMENT,1)) < VI_SUCCESS)
		return Status;
// 	if ((Status = viInstallHandler(m_viMem,VI_EVENT_PXI_INTR,(ViHndlr)pxi_isr,0)) < VI_SUCCESS)
// 		return Status;
// 	if ((Status = viEnableEvent(m_viMem,VI_EVENT_PXI_INTR,VI_HNDLR,VI_NULL)) < VI_SUCCESS)
// 		return Status;

	return Status;
}

int32_t viMemIO::Release()
{
	ViStatus Status = VI_SUCCESS;

	if (m_viMem) {
		if ((Status = viMemFree(m_viMem,m_PhyAddrBfrAlign)) < VI_SUCCESS)
			return Status;
		if ((Status = viClose(m_viMem)) < VI_SUCCESS)
			return Status;
	}
	return Status;
}

int32_t viMemIO::Write8(uint8_t *pBuf,uint32_t uiSize)
{
	return viMoveOut8(m_viMem,VI_PXI_ALLOC_SPACE,m_PhyAddr,uiSize,pBuf);
}

int32_t viMemIO::Write16(uint16_t *pBuf,uint32_t uSize)
{
	return viMoveOut16(m_viMem,VI_PXI_ALLOC_SPACE,m_PhyAddr,uSize,pBuf);
}

int32_t viMemIO::Write32(uint32_t *pBuf,uint32_t uSize)
{
    return viMoveOut32(m_viMem, VI_PXI_ALLOC_SPACE, m_PhyAddr, uSize, (ViAUInt32)pBuf);
}

int32_t viMemIO::Read8(uint8_t *pBuf,uint32_t uSize)
{
	return viMoveIn8(m_viMem,VI_PXI_ALLOC_SPACE,m_PhyAddr,uSize,pBuf);
}

int32_t viMemIO::Read16(uint16_t *pBuf, uint32_t uSize)
{
	return viMoveIn16(m_viMem,VI_PXI_ALLOC_SPACE,m_PhyAddr,uSize,pBuf);
}

int32_t viMemIO::Read32(uint32_t *pBuf, uint32_t uSize)
{
    return viMoveIn32(m_viMem,VI_PXI_ALLOC_SPACE,m_PhyAddr,uSize,(ViAUInt32)pBuf);
}

int32_t _VI_FUNCH pxi_isr(ViSession vi,ViEventType etype,ViEvent eventContext,ViAddr userHandle)
{
	//	SetEvent(&g_Event);

	// 	viOut32(vi,VI_PXI_BAR0_SPACE, LEAGACYCLR, 0x00000100);
	// 	viOut32(vi,VI_PXI_BAR0_SPACE, LEAGACYCLR, 0x00000000);

	// 	viGetAttribute(context, VI_ATTR_PXI_RECV_INTR_SEQ, &g_isr_params.recv_seq);
	// 	viGetAttribute(context, VI_ATTR_PXI_RECV_INTR_DATA, &g_isr_params.recv_data);

	return VI_SUCCESS;
}
