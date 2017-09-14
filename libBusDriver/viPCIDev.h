#ifndef __VI_PCIDEV_H__
#define __VI_PCIDEV_H__

#include "IPCIDev.h"
#include "visa.h"

class viPCIDev : public IPCIDev
{
public:
	viPCIDev(void);
	viPCIDev(const char* pDes);
	virtual ~viPCIDev(void);
public:
	virtual int32_t GetDevices(vector<string> &Devs);
	virtual int32_t Init(const string strDev);
	virtual int32_t Init(const ViRsrc strDev);
	virtual int32_t Init(const string strDev,uint32_t uiVID,uint32_t uiDID);
	virtual int32_t Init(const ViRsrc strDev,uint32_t uiVID,uint32_t uiDID);
	virtual int32_t Close();
	virtual char* GetDes();

	virtual int32_t W8(AS Addr,uint32_t uiOffset,uint8_t uiData);
	virtual int32_t W16(AS Addr,uint32_t uiOffset,uint16_t uiData);
	virtual int32_t W32(AS Addr,uint32_t uiOffset,uint32_t uiData);
	virtual int32_t W32(AS Addr,uint32_t uiOffset,uint32_t uiLength,uint32_t *pBuf);
	virtual int32_t R8(AS Addr,uint32_t uiOffset,uint8_t *pData);
	virtual int32_t R16(AS Addr,uint32_t uiOffset,uint16_t *pData);
	virtual int32_t R32(AS Addr,uint32_t uiOffset,uint32_t *pData);

	virtual int32_t LLAW32(uint32_t uiOffset,uint32_t uiData);
	virtual int32_t LLAR32(uint32_t uiOffset,uint32_t *pData);
public:
	static int32_t OpenDefaultRM();
	static ViSession GetDefaultRM();
private:
	ViSession m_viSession;
	ViAddr	  m_viMapAddr;
};

#endif
