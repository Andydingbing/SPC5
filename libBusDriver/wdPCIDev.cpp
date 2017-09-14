#include "wdPCIDev.h"
#include "windrvr.h"
#include "wdc_lib.h"
#include "status_strings.h"
#include "wdc_defs.h"

#define LICENSE "6C3CC2CFE89E7AD042444C655646F21A0BF3FDEB.EMBRACE"

#ifdef _MSC_VER
#pragma comment(lib,"wdapi1020.lib")
#elif defined __GNUC__
#pragma GCC diagnostic ignored  "-Wunused-parameter"
#endif

wdPCIDev::wdPCIDev(void)
{
	WDC_DriverOpen(WDC_DRV_OPEN_DEFAULT,LICENSE);
}

wdPCIDev::wdPCIDev(const char* pDes)
{
	memset(m_szDes,0,sizeof(m_szDes));

#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4996)
	strcpy(m_szDes,pDes);
#pragma warning (pop)
#elif defined __GNUC__
    strcpy(m_szDes,pDes);
#endif

	WDC_DriverOpen(WDC_DRV_OPEN_DEFAULT,LICENSE);
}

wdPCIDev::~wdPCIDev(void)
{
}

int32_t wdPCIDev::GetDevices(vector<string> &Devs)
{
	WDC_PCI_SCAN_RESULT scanResult;
	uint32_t uiStatus = WD_STATUS_SUCCESS;
	char szDev[256] = {0};
	string strDev = "";

	if ((uiStatus = WDC_PciScanDevices(0,0,&scanResult)) != WD_STATUS_SUCCESS)
		return uiStatus;
	for (uint32_t i = 0;i < scanResult.dwNumDevices;i ++) {
		memset(szDev,0,sizeof(szDev));
		strDev.clear();
		sprintf_s(szDev,"device:%#x,VID:%6#x,DID:%6#x",i,scanResult.deviceId[i].dwVendorId,scanResult.deviceId[i].dwDeviceId);
		strDev = szDev;
		Devs.push_back(strDev);
	}
	return uiStatus;
}

int32_t wdPCIDev::Init(const string strDev)
{
	return WD_WINDRIVER_STATUS_ERROR;
}

int32_t wdPCIDev::Init(const string strDev,uint32_t uiVID,uint32_t uiDID)
{
	return 0;
}

int32_t wdPCIDev::Close()
{
	return 0;
}

char* wdPCIDev::GetDes()
{
	return 0;
}

int32_t wdPCIDev::W8(AS Addr,uint32_t uiOffset,uint8_t uiData)
{
	return 0;
}

int32_t wdPCIDev::W16(AS Addr,uint32_t uiOffset,uint16_t uiData)
{
	return 0;
}

int32_t wdPCIDev::W32(AS Addr,uint32_t uiOffset,uint32_t uiData)
{
	return 0;
}

int32_t wdPCIDev::W32(AS Addr,uint32_t uiOffset,uint32_t uiLength,uint32_t *pBuf)
{
	return 0;
}

int32_t wdPCIDev::R8(AS Addr,uint32_t uiOffset,uint8_t *pData)
{
	return 0;
}

int32_t wdPCIDev::R16(AS Addr,uint32_t uiOffset,uint16_t *pData)
{
	return 0;
}

int32_t wdPCIDev::R32(AS Addr,uint32_t uiOffset,uint32_t *pData)
{
	return 0;
}

int32_t wdPCIDev::LLAW32(uint32_t uiOffset,uint32_t uiData)
{
	return 0;
}

int32_t wdPCIDev::LLAR32(uint32_t uiOffset,uint32_t *pData)
{
	return 0;
}
