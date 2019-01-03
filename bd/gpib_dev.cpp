#include "gpib_dev.h"
#include <stdio.h>
#include <string.h>
#include "vi_pci_dev.h"

using namespace sp_rd;

gpib_dev::gpib_dev(void) : m_session(0)
{
    vi_pci_dev::open_default_rm();
}

gpib_dev::~gpib_dev(void)
{
}

bool gpib_dev::init(ViRsrc dev)
{
    if (viOpen(vi_pci_dev::get_default_rm(),dev,VI_NULL,50000,&m_session) < VI_SUCCESS)
		return false;
    if (viSetAttribute(m_session,VI_ATTR_TMO_VALUE,50000)) {
        viClose(m_session);
		return false;
	}
	return true;
}

bool gpib_dev::w(const char *format, ...)
{
    char buf[256] = {0};
    ViUInt32 ret_cnt = 0;
    ViStatus status = VI_SUCCESS;

    memset(buf,0,sizeof(buf));
	va_list ap;
	va_start(ap, format);
    vsprintf(buf, format, ap);
	va_end(ap);

    status = viWrite(m_session,(ViBuf)buf,(ViUInt32)(strlen(buf)),&ret_cnt);
    if (status < VI_SUCCESS || ret_cnt != strlen(buf) )
		return false;
	return true;
}

bool gpib_dev::r(char *buf, uint32_t max_len, uint32_t to)
{
    ViUInt32 ret_cnt = 0;
    ViStatus status = VI_SUCCESS;
    uint32_t last_to = 0;

    if (last_to != 3000) {
        if ((status = viGetAttribute(m_session,VI_ATTR_TMO_VALUE,&last_to)) < VI_SUCCESS)
			return false;
        if ((status = viSetAttribute(m_session,VI_ATTR_TMO_VALUE,to)) < VI_SUCCESS)
			return false;
	}
    if ((status = viRead(m_session,(ViPBuf)buf,max_len,&ret_cnt)) < VI_SUCCESS)
		return false;
    if (to != 3000) {
        if ((status = viSetAttribute(m_session,VI_ATTR_TMO_VALUE,last_to)) < VI_SUCCESS)
			return false;
	}
	return true;
}
