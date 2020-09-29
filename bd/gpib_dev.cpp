#include "gpib_dev.h"
#include "pci_dev_vi.h"
#include "visa.h"

using namespace std;
//using namespace sp_rd;

gpib_dev::gpib_dev() :
    _session(0)
{
    vi_pci_dev::open_default_rm();
}

bool gpib_dev::init(const std::string &dev)
{
    ViSession session = 0;

    if (viOpen(vi_pci_dev::get_default_rm(),const_cast<ViRsrc>(dev.c_str()),VI_NULL,50000,&session) < VI_SUCCESS) {
        _session = 0;
		return false;
    }
    if (viSetAttribute(session,VI_ATTR_TMO_VALUE,50000)) {
        viClose(session);
        _session = 0;
		return false;
	}
    _session = session;
	return true;
}

bool gpib_dev::w(const string &scpi) const
{
    ViUInt32 length = ViUInt32(scpi.length());
    ViUInt32 ret_cnt = 0;
    ViStatus status = VI_SUCCESS;

    status = viWrite(_session,ViBuf(scpi.c_str()),length,&ret_cnt);

    if (status < VI_SUCCESS || ret_cnt != length) {
        return false;
    }
    return true;
}

bool gpib_dev::r(string &buf,uint32_t length,uint32_t to) const
{
    ViUInt32 ret_cnt = 0;
    ViStatus status = VI_SUCCESS;
    uint32_t last_to = 0;

    ViPBuf read_buf = new ViByte[length + 1];

    if (read_buf == nullptr) {
        return false;
    }

    if (last_to != 3000) {
        if ((status = viGetAttribute(_session,VI_ATTR_TMO_VALUE,&last_to)) < VI_SUCCESS) {
            SAFE_DEL_ARRAY(read_buf);
			return false;
        }
        if ((status = viSetAttribute(_session,VI_ATTR_TMO_VALUE,to)) < VI_SUCCESS) {
            SAFE_DEL_ARRAY(read_buf);
			return false;
        }
	}

    if ((status = viRead(_session,read_buf,length,&ret_cnt)) < VI_SUCCESS) {
        SAFE_DEL_ARRAY(read_buf);
		return false;
    }
    read_buf[length] = '\0';

    buf.clear();
    for (uint32_t i = 0;i <= length;i ++) {
        buf.push_back(char(read_buf[i]));
    }

    if (to != 3000) {
        if ((status = viSetAttribute(_session,VI_ATTR_TMO_VALUE,last_to)) < VI_SUCCESS) {
            SAFE_DEL_ARRAY(read_buf);
			return false;
        }
	}

    SAFE_DEL_ARRAY(read_buf);
	return true;
}
