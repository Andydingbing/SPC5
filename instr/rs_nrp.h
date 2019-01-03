#ifndef RS_NRP_H
#define RS_NRP_H

#include "pm.h"

class rs_nrp : public pm
{
public:
    rs_nrp();
    virtual ~rs_nrp();
    virtual char* get_des();
    virtual bool init(ViRsrc dev);
public:
    virtual bool reset();
    virtual bool set_freq(double freq);
    virtual bool get_pwr(double &pwr);
    virtual bool get_pwr(double freq,double &pwr);
    virtual bool close();
};

#endif // RS_NRP_H
