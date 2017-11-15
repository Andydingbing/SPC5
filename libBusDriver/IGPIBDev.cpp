#include "IGPIBDev.h"
#include <stdio.h>
#include <string.h>
#include "viPCIDev.h"

IGPIBDev::IGPIBDev(void) : m_viSession(0)
{
	viPCIDev::OpenDefaultRM();
}

IGPIBDev::~IGPIBDev(void)
{
}

bool IGPIBDev::Init(ViRsrc strDev)
{
	if (viOpen(viPCIDev::GetDefaultRM(),strDev,VI_NULL,50000,&m_viSession) < VI_SUCCESS)
		return false;
	if (viSetAttribute(m_viSession,VI_ATTR_TMO_VALUE,50000)) {
		viClose(m_viSession);
		return false;
	}
	return true;
}

bool IGPIBDev::Write(const char *format,...)
{
	char szBuf[256];
	ViUInt32 retCnt = 0;
	ViStatus Status = VI_SUCCESS;

	memset(szBuf,0,sizeof(szBuf));
	va_list ap;
	va_start(ap, format);
    vsprintf(szBuf,format,ap);
	va_end(ap);

	Status = viWrite(m_viSession,(ViBuf)szBuf,(ViUInt32)(strlen(szBuf)),&retCnt);
	if (Status < VI_SUCCESS || retCnt != strlen(szBuf) )
		return false;
	return true;
}

bool IGPIBDev::Read(char *pBuf,uint32_t uiMaxLength,uint32_t uiTo)
{
	ViUInt32 retCnt = 0;
	ViStatus Status = VI_SUCCESS;
	uint32_t uiLastTo = 0;

	if (uiTo != 3000) {
		if ((Status = viGetAttribute(m_viSession,VI_ATTR_TMO_VALUE,&uiLastTo)) < VI_SUCCESS)
			return false;
		if ((Status = viSetAttribute(m_viSession,VI_ATTR_TMO_VALUE,uiTo)) < VI_SUCCESS)
			return false;
	}
	if ((Status = viRead(m_viSession,(ViPBuf)pBuf,uiMaxLength,&retCnt)) < VI_SUCCESS)
		return false;
	if (uiTo != 3000) {
		if ((Status = viSetAttribute(m_viSession,VI_ATTR_TMO_VALUE,uiLastTo)) < VI_SUCCESS)
			return false;
	}
	return true;
}
