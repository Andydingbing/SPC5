#ifndef PM_H
#define PM_H

#include "visa.h"

class pm
{
public:
    pm(void){}
    virtual ~pm(void){}
    virtual char* get_des() = 0;
    virtual bool init(ViRsrc dev) = 0;
public:
    virtual bool reset() = 0;
    virtual bool set_freq(double freq) = 0;
    virtual bool get_pwr(double &pwr) = 0;
    virtual bool get_pwr(double freq,double &pwr) = 0;
    virtual bool close() = 0;
protected:
    ViSession m_session;
};

#endif // PM_H
