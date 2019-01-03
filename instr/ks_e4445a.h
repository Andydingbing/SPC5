#ifndef KS_E4445A_H
#define KS_E4445A_H

#include "ks_n9030a.h"

class ks_e4445a : public ks_n9030a
{
public:
    ks_e4445a(void);
    virtual ~ks_e4445a(void);
    virtual char* get_des();
public:
    virtual bool set_en_preamp(bool en);
};

#endif // KS_E4445A_H
