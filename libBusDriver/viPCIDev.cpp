#include "viPCIDev.h"
#include <string.h>

#ifndef NIVISA_PXI
#define NIVISA_PXI
#endif

#ifdef _MSC_VER
#pragma data_seg("viRsrcMgr")
	ViSession g_RsrcMgr = 0;
#pragma data_seg()
#pragma comment(linker,"/section:viRsrcMgr,s")
#pragma comment(lib,"visa64.lib")
#elif defined __GNUC__
    ViSession g_RsrcMgr __attribute__((section("viRsrcMgr"))) = 0;
#endif

viPCIDev::viPCIDev(void) : IPCIDev(),m_viSession(0)
{
	memset(m_szDes,0,sizeof(m_szDes));
	OpenDefaultRM();
}

viPCIDev::viPCIDev(const char* pDes) : IPCIDev(),m_viSession(0)
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

	OpenDefaultRM();
}

viPCIDev::~viPCIDev(void)
{
}

int32_t viPCIDev::GetDevices(vector<string> &Devs)
{
	ViStatus Status = VI_SUCCESS;
	ViFindList FindList = 0;
	ViUInt32 uiRetCnt = 0;
    ViChar szBuf[256] = {0};
    ViChar szExpr[32] = "PXI?*INSTR";

	Devs.clear();

    if ((Status = viFindRsrc(g_RsrcMgr,szExpr,&FindList,&uiRetCnt,szBuf)) < VI_SUCCESS)
		return Status;

	Devs.push_back(szBuf);
	while (uiRetCnt --) {
		if ((Status = viFindNext(FindList,szBuf)) == VI_ERROR_INV_OBJECT)
			return Status;
		Devs.push_back(szBuf);
	}
	Devs.pop_back();
	viClose(FindList);
	return Status;
}

int32_t viPCIDev::Init(const string strDev)
{
	char szDev[256] = {0};
	memset(szDev,0,sizeof(szDev));

#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4996)
	strDev.copy(szDev,strDev.length(),0);
#pragma warning (pop)
#elif defined __GNUC__
    strDev.copy(szDev,strDev.length(),0);
#endif

	return Init(szDev);
}

int32_t viPCIDev::Init(const ViRsrc strDev)
{
	ViStatus Status = VI_SUCCESS;

	if ((Status = viOpen(g_RsrcMgr,strDev,VI_NULL,VI_NULL,&m_viSession)) < VI_SUCCESS) {
		m_viSession = 0;
		return Status;
	}
	if ((Status = viSetAttribute(m_viSession,VI_ATTR_DEST_INCREMENT,0)) < VI_SUCCESS) {
		viClose(m_viSession);
		return Status;
	}
	return Status;
}

int32_t viPCIDev::Init(const string strDev,uint32_t uiVID,uint32_t uiDID)
{
	char szDev[256] = {0};
	memset(szDev,0,sizeof(szDev));

#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4996)
	strDev.copy(szDev,strDev.length(),0);
#pragma warning (pop)
#elif defined __GNUC__
    strDev.copy(szDev,strDev.length(),0);
#endif

	return Init(szDev,uiVID,uiDID);
}

int32_t viPCIDev::Init(const ViRsrc strDev,uint32_t uiVID,uint32_t uiDID)
{
	ViStatus Status = VI_SUCCESS;
	ViUInt32 uiManfID = 0;
	ViUInt32 uiModelCode = 0;

	if ((Status = viOpen(g_RsrcMgr,strDev,VI_NULL,VI_NULL,&m_viSession)) < VI_SUCCESS) {
		m_viSession = 0;
		return Status;
	}
	if ((Status = viGetAttribute(m_viSession,VI_ATTR_MANF_ID,&uiManfID)) < VI_SUCCESS || uiManfID != uiVID) {
		viClose(m_viSession);
		return Status;
	}
	if ((Status = viGetAttribute(m_viSession,VI_ATTR_MODEL_CODE,&uiModelCode)) < VI_SUCCESS || uiModelCode != uiDID) {
		viClose(m_viSession);
		return Status;
	}
	if ((Status = viSetAttribute(m_viSession,VI_ATTR_DEST_INCREMENT,0)) < VI_SUCCESS) {
		viClose(m_viSession);
		return Status;
	}
	return Status;
}

int32_t viPCIDev::Close()
{
	ViStatus Status = VI_SUCCESS;
	if (m_viSession) {
		if ((Status = viClose(m_viSession)) < VI_SUCCESS)
			return Status;
	}
	if (g_RsrcMgr) {
		if ((Status = viClose(g_RsrcMgr)) < VI_SUCCESS)
			return Status;
	}
	return Status;
}

char* viPCIDev::GetDes()
{
	return m_szDes;
}

int32_t viPCIDev::W8(AS Addr,uint32_t uiOffset,uint8_t uiData)
{
	return viOut8(m_viSession,(ViUInt16)Addr,uiOffset,uiData);
}

int32_t viPCIDev::W16(AS Addr,uint32_t uiOffset,uint16_t uiData)
{
	return viOut16(m_viSession,(ViUInt16)Addr,uiOffset,uiData);
}

int32_t viPCIDev::W32(AS Addr,uint32_t uiOffset,uint32_t uiData)
{
	return viOut32(m_viSession,(ViUInt16)Addr,uiOffset,uiData);
}

int32_t viPCIDev::W32(AS Addr,uint32_t uiOffset,uint32_t uiLength,uint32_t *pBuf)
{
    return viMoveOut32(m_viSession,(ViUInt16)Addr,uiOffset,uiLength,(ViAUInt32)pBuf);
}

int32_t viPCIDev::R8(AS Addr,uint32_t uiOffset,uint8_t *pData)
{
	return viIn8(m_viSession,(ViUInt16)Addr,uiOffset,pData);
}

int32_t viPCIDev::R16(AS Addr,uint32_t uiOffset,uint16_t *pData)
{
	return viIn16(m_viSession,(ViUInt16)Addr,uiOffset,pData);
}

int32_t viPCIDev::R32(AS Addr,uint32_t uiOffset,uint32_t *pData)
{
    return viIn32(m_viSession,(ViUInt16)Addr,uiOffset,(ViPUInt32)pData);
}

int32_t viPCIDev::LLAW32(uint32_t uiOffset,uint32_t uiData)
{
	viPoke32(m_viSession,(ViPByte)m_viMapAddr + uiOffset,uiData);
	return 0;
}

int32_t viPCIDev::LLAR32(uint32_t uiOffset,uint32_t *pData)
{
    viPeek32(m_viSession,(ViPByte)m_viMapAddr + uiOffset,(ViPUInt32)pData);
	return 0;
}

int32_t viPCIDev::OpenDefaultRM()
{
	if (!g_RsrcMgr)
		return viOpenDefaultRM(&g_RsrcMgr);
	return 0;
}

ViSession viPCIDev::GetDefaultRM()
{
	return g_RsrcMgr;
}
