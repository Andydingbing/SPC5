#ifndef GPIB_DEV_H
#define GPIB_DEV_H

#include "rd.h"
#include "visa.h"

namespace sp_rd {

class RD_API gpib_dev
{
public:
    gpib_dev(void);
    virtual ~gpib_dev(void);
public:
    virtual int32_t get_default_pri_addr() = 0;
    virtual int32_t get_default_sec_addr() = 0;
    virtual char* get_des() = 0;
public:
    virtual bool init(ViRsrc dev);
    virtual bool w(const char *format, ...);
    virtual bool r(char *buf, uint32_t max_len, uint32_t to = 3000);
private:
    ViSession m_session;
};

} //namespace sp_rd

#endif // GPIB_DEV_H
