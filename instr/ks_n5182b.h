#ifndef KS_N5182B_H
#define KS_N5182B_H

#include "sg.h"

class ks_n5182b : public sg
{
public:
    ks_n5182b();
    virtual ~ks_n5182b();
    virtual int32_t get_default_pri_addr();
    virtual int32_t get_default_sec_addr();
    virtual char* get_des();
public:
    virtual bool reset();
    virtual bool set_cw(double freq);
    virtual bool get_cw(double &freq);
    virtual bool set_pl(double pwr);
    virtual bool get_pl(double &pwr);
    virtual bool set_en_output(bool en);
    virtual bool set_en_mod(bool en);
protected:
    double m_pwr;
};

#endif // KS_N5182B_H
