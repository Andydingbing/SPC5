#ifndef INSTR_SG_H
#define INSTR_SG_H

#include "gpib_dev.h"

class sg : public sp_rd::gpib_dev
{
public:
    virtual bool reset() = 0;
    virtual bool set_cw(double freq) = 0;
    virtual bool get_cw(double &freq) = 0;
    virtual bool set_pl(double pwr) = 0;
    virtual bool get_pl(double &pwr) = 0;
    virtual bool set_en_output(bool en) = 0;
    virtual bool set_en_mod(bool en) = 0;
};

#endif // INSTR_SG_H
