#ifndef __PCI_DEV_H__
#define __PCI_DEV_H__

#define INT_CHECK(func) if (int ret = func) {return ret;}
#define INT_CHECKB(func) if (int ret = func) {return false;}
#define INT_CHECKV(func) if (int ret = func) {return;}

#include <vector>
#include <string>
#include "stdint.h"

using namespace std;

class IPCIDev
{
public:
    enum AS {
		AS_LOCAL = 0,
		AS_A16 = 1,
		AS_A24 = 2,
		AS_A32 = 3,
		AS_A64 = 4,
		AS_ALLOC = 9,
		AS_CFG = 10,
		AS_BAR0 = 11,
		AS_BAR1 = 12,
		AS_BAR2 = 13,
		AS_BAR3 = 14,
		AS_BAR4 = 15,
		AS_BAR5 = 16,
		AS_OPAQUE = 0xFFFF
	};
public:
	IPCIDev(void){}
	virtual ~IPCIDev(void){}
public:
	virtual int32_t GetDevices(vector<string> &Devs) = 0;
	virtual int32_t Init(const string strDev) = 0;
	virtual int32_t Init(const string strDev,uint32_t uiVID,uint32_t uiDID) = 0;
	virtual int32_t Close() = 0;
	virtual char* GetDevName(){return m_szDes;}
public:
	virtual int32_t W8(AS Addr,uint32_t uiOffset,uint8_t uiData) = 0;
	virtual int32_t W16(AS Addr,uint32_t uiOffset,uint16_t uiData) = 0;
	virtual int32_t W32(AS Addr,uint32_t uiOffset,uint32_t uiData) = 0;
	virtual int32_t W32(AS Addr,uint32_t uiOffset,uint32_t uiLength,uint32_t *pBuf) = 0;
	virtual int32_t R8(AS Addr,uint32_t uiOffset,uint8_t *pData) = 0;
	virtual int32_t R16(AS Addr,uint32_t uiOffset,uint16_t *pData) = 0;
	virtual int32_t R32(AS Addr,uint32_t uiOffset,uint32_t *pData) = 0;
protected:
	char m_szDes[16];
};

#endif
