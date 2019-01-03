#ifndef KS_N9010A_H
#define KS_N9010A_H

#include "ks_n9030a.h"

class ks_n9010a : public ks_n9030a
{
public:
    ks_n9010a(void);
    virtual ~ks_n9010a(void);
    virtual char* get_des();
};

#endif // KS_N9010A_H
