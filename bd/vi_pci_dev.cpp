/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

#include "vi_pci_dev.h"
#include "visa.h"

#ifndef NIVISA_PXI
#define NIVISA_PXI
#endif

//! Define shared data seg "viRsrcMgr(vi Resource Manager)"
//#ifdef RD_C_MSC
//#pragma data_seg("viRsrcMgr")
//    static ViSession g_rsrc_mgr = 0;
//#pragma data_seg()
//#pragma comment(linker,"/section:viRsrcMgr,s")
//#pragma comment(lib,"visa64.lib")
//#elif defined RD_C_GNUC
//    ViSession g_rsrc_mgr __attribute__((section("viRsrcMgr"))) = 0;
//#endif

static ViSession g_rsrc_mgr = 0;

using namespace std;
using namespace sp_rd;

vi_pci_dev::vi_pci_dev() :
    pci_dev(),
    _session(0)
{
    _descriptor = "";
    open_default_rm();
}

vi_pci_dev::vi_pci_dev(const std::string &descriptor) :
    pci_dev(),
    _session(0)
{
    _descriptor = descriptor;
    open_default_rm();
}

vi_pci_dev::~vi_pci_dev()
{
    ViStatus status = VI_SUCCESS;
    if (_session) {
        if ((status = viClose(_session)) < VI_SUCCESS)
            return;
    }
//    if (g_rsrc_mgr) {
//        if ((status = viClose(g_rsrc_mgr)) < VI_SUCCESS)
//            return;
//    }
}

int32_t vi_pci_dev::get_devs(vector<string> &devs)
{
    ViStatus status = VI_SUCCESS;
    ViFindList find_list = 0;
    ViUInt32 ret_cnt = 0;
    ViChar buf[256] = {0};
    ViChar expr[32] = "PXI?*INSTR";

    devs.clear();

    if ((status = viFindRsrc(g_rsrc_mgr,expr,&find_list,&ret_cnt,buf)) < VI_SUCCESS) {
        return status;
    }

    devs.push_back(buf);

    while (ret_cnt --) {
        if ((status = viFindNext(find_list,buf)) == VI_ERROR_INV_OBJECT) {
            return status;
        }
        devs.push_back(buf);
	}

    devs.pop_back();
    viClose(find_list);

    return status;
}

int32_t vi_pci_dev::init(const string &dev)
{
    return init(const_cast<ViRsrc>(dev.c_str()));
}

int32_t vi_pci_dev::init(const char *dev)
{
    ViStatus status = VI_SUCCESS;

    if ((status = viOpen(g_rsrc_mgr,const_cast<ViRsrc>(dev),VI_NULL,VI_NULL,&_session)) < VI_SUCCESS) {
        _session = 0;
        return status;
	}
    if ((status = viSetAttribute(_session,VI_ATTR_DEST_INCREMENT,0)) < VI_SUCCESS) {
        viClose(_session);
        _session = 0;
        return status;
	}
    return status;
}

int32_t vi_pci_dev::init(const string &dev, uint32_t vid, uint32_t did)
{
    return init(const_cast<ViRsrc>(dev.c_str()),vid,did);
}

int32_t vi_pci_dev::init(const char *dev, uint32_t vid, uint32_t did)
{
    ViStatus status = VI_SUCCESS;
    ViUInt32 manf_id = 0;
    ViUInt32 model_code = 0;

    if ((status = viOpen(g_rsrc_mgr,const_cast<ViRsrc>(dev),VI_NULL,VI_NULL,&_session)) < VI_SUCCESS) {
        _session = 0;
        return status;
	}
    if ((status = viGetAttribute(_session,VI_ATTR_MANF_ID,&manf_id)) < VI_SUCCESS || manf_id != vid) {
        viClose(_session);
        _session = 0;
        return status;
	}
    if ((status = viGetAttribute(_session,VI_ATTR_MODEL_CODE,&model_code)) < VI_SUCCESS || model_code != did) {
        viClose(_session);
        _session = 0;
        return status;
	}
    if ((status = viSetAttribute(_session,VI_ATTR_DEST_INCREMENT,0)) < VI_SUCCESS) {
        viClose(_session);
        _session = 0;
        return status;
	}
    return status;
}

int32_t vi_pci_dev::w8(addr_space_t addr, uint32_t offset, uint8_t data)
{
    return viOut8(_session,ViUInt16(addr),offset,data);
}

int32_t vi_pci_dev::w16(addr_space_t addr, uint32_t offset, uint16_t data)
{
    return viOut16(_session,ViUInt16(addr),offset,data);
}

int32_t vi_pci_dev::w32(addr_space_t addr, uint32_t offset, uint32_t data)
{
    return viOut32(_session,ViUInt16(addr),offset,data);
}

int32_t vi_pci_dev::w32(addr_space_t addr, uint32_t offset, uint32_t len, uint32_t *buf)
{
    return viMoveOut32(_session,ViUInt16(addr),offset,len,ViAUInt32(buf));
}

int32_t vi_pci_dev::r8(addr_space_t addr, uint32_t offset, uint8_t *data)
{
    return viIn8(_session,ViUInt16(addr),offset,data);
}

int32_t vi_pci_dev::r16(addr_space_t addr, uint32_t offset, uint16_t *data)
{
    return viIn16(_session,ViUInt16(addr),offset,data);
}

int32_t vi_pci_dev::r32(addr_space_t addr, uint32_t offset, uint32_t *data)
{
    return viIn32(_session,ViUInt16(addr),offset,ViPUInt32(data));
}

int32_t vi_pci_dev::lla_w32(uint32_t offset,uint32_t data)
{
    viPoke32(_session,ViPByte(_map_addr) + offset,data);
	return 0;
}

int32_t vi_pci_dev::lla_r32(uint32_t offset,uint32_t *data)
{
    viPeek32(_session,ViPByte(_map_addr) + offset,ViPUInt32(data));
	return 0;
}

int32_t vi_pci_dev::open_default_rm()
{
    if (!g_rsrc_mgr)
        return viOpenDefaultRM(&g_rsrc_mgr);
    else
        return 0;
}

unsigned long vi_pci_dev::get_default_rm()
{
    return g_rsrc_mgr;
}
