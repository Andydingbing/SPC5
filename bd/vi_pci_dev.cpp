#include "vi_pci_dev.h"
#include <string>

#ifndef NIVISA_PXI
#define NIVISA_PXI
#endif

//! Define shared data seg "viRsrcMgr(vi Resource Manager)"
#ifdef RD_C_MSC
#pragma data_seg("viRsrcMgr")
    ViSession g_rsrc_mgr = 0;
#pragma data_seg()
#pragma comment(linker,"/section:viRsrcMgr,s")
#pragma comment(lib,"visa64.lib")
#elif defined RD_C_GNUC
    ViSession g_rsrc_mgr __attribute__((section("viRsrcMgr"))) = 0;
#endif

using namespace sp_rd;

vi_pci_dev::vi_pci_dev(void) : pci_dev(), m_session(0)
{
    memset(m_des,0,sizeof(m_des));
    open_default_rm();
}

vi_pci_dev::vi_pci_dev(const char *des) : pci_dev(), m_session(0)
{
    memset(m_des,0,sizeof(m_des));
    strcpy(m_des,des);
    open_default_rm();
}

vi_pci_dev::~vi_pci_dev(void)
{
}

int32_t vi_pci_dev::get_devs(vector<string> &devs)
{
    ViStatus status = VI_SUCCESS;
    ViFindList find_list = 0;
    ViUInt32 ret_cnt = 0;
    ViChar buf[256] = {0};
    ViChar expr[32] = "PXI?*INSTR";

    devs.clear();

    if ((status = viFindRsrc(g_rsrc_mgr,expr,&find_list,&ret_cnt,buf)) < VI_SUCCESS)
        return status;

    devs.push_back(buf);
    while (ret_cnt --) {
        if ((status = viFindNext(find_list,buf)) == VI_ERROR_INV_OBJECT)
            return status;
        devs.push_back(buf);
	}
    devs.pop_back();
    viClose(find_list);
    return status;
}

int32_t vi_pci_dev::init(const string dev)
{
    char buf[256] = {0};
    memset(buf,0,sizeof(buf));
    dev.copy(buf,dev.length(),0);
    return init(buf);
}

int32_t vi_pci_dev::init(const ViRsrc dev)
{
    ViStatus status = VI_SUCCESS;

    if ((status = viOpen(g_rsrc_mgr,dev,VI_NULL,VI_NULL,&m_session)) < VI_SUCCESS) {
        m_session = 0;
        return status;
	}
    if ((status = viSetAttribute(m_session,VI_ATTR_DEST_INCREMENT,0)) < VI_SUCCESS) {
        viClose(m_session);
        return status;
	}
    return status;
}

int32_t vi_pci_dev::init(const string dev, uint32_t vid, uint32_t did)
{
    char buf[256] = {0};
    memset(buf,0,sizeof(buf));
    dev.copy(buf,dev.length(),0);
    return init(buf,vid,did);
}

int32_t vi_pci_dev::init(const ViRsrc dev, uint32_t vid, uint32_t did)
{
    ViStatus status = VI_SUCCESS;
    ViUInt32 manf_id = 0;
    ViUInt32 model_code = 0;

    if ((status = viOpen(g_rsrc_mgr,dev,VI_NULL,VI_NULL,&m_session)) < VI_SUCCESS) {
        m_session = 0;
        return status;
	}
    if ((status = viGetAttribute(m_session,VI_ATTR_MANF_ID,&manf_id)) < VI_SUCCESS || manf_id != vid) {
        viClose(m_session);
        return status;
	}
    if ((status = viGetAttribute(m_session,VI_ATTR_MODEL_CODE,&model_code)) < VI_SUCCESS || model_code != did) {
        viClose(m_session);
        return status;
	}
    if ((status = viSetAttribute(m_session,VI_ATTR_DEST_INCREMENT,0)) < VI_SUCCESS) {
        viClose(m_session);
        return status;
	}
    return status;
}

int32_t vi_pci_dev::close()
{
    ViStatus status = VI_SUCCESS;
    if (m_session) {
        if ((status = viClose(m_session)) < VI_SUCCESS)
            return status;
	}
    if (g_rsrc_mgr) {
        if ((status = viClose(g_rsrc_mgr)) < VI_SUCCESS)
            return status;
	}
    return status;
}

int32_t vi_pci_dev::w8(addr_space_t addr, uint32_t offset, uint8_t data)
{
    return viOut8(m_session,(ViUInt16)addr,offset,data);
}

int32_t vi_pci_dev::w16(addr_space_t addr, uint32_t offset, uint16_t data)
{
    return viOut16(m_session,(ViUInt16)addr,offset,data);
}

int32_t vi_pci_dev::w32(addr_space_t addr, uint32_t offset, uint32_t data)
{
    return viOut32(m_session,(ViUInt16)addr,offset,data);
}

int32_t vi_pci_dev::w32(addr_space_t addr, uint32_t offset, uint32_t len, uint32_t *buf)
{
    return viMoveOut32(m_session,(ViUInt16)addr,offset,len,(ViAUInt32)buf);
}

int32_t vi_pci_dev::r8(addr_space_t addr, uint32_t offset, uint8_t *data)
{
    return viIn8(m_session,(ViUInt16)addr,offset,data);
}

int32_t vi_pci_dev::r16(addr_space_t addr, uint32_t offset, uint16_t *data)
{
    return viIn16(m_session,(ViUInt16)addr,offset,data);
}

int32_t vi_pci_dev::r32(addr_space_t addr, uint32_t offset, uint32_t *data)
{
    return viIn32(m_session,(ViUInt16)addr,offset,(ViPUInt32)data);
}

int32_t vi_pci_dev::lla_w32(uint32_t offset,uint32_t data)
{
    viPoke32(m_session,(ViPByte)m_map_addr + offset,data);
	return 0;
}

int32_t vi_pci_dev::lla_r32(uint32_t offset,uint32_t *data)
{
    viPeek32(m_session,(ViPByte)m_map_addr + offset,(ViPUInt32)data);
	return 0;
}

int32_t vi_pci_dev::open_default_rm()
{
    if (!g_rsrc_mgr)
        return viOpenDefaultRM(&g_rsrc_mgr);
    else
        return 0;
}

ViSession vi_pci_dev::get_default_rm()
{
    return g_rsrc_mgr;
}
