#ifndef KS_N9020A_H
#define KS_N9020A_H

#include "ks_n9030a.h"

class ks_n9020a : public ks_n9030a
{
public:
    ks_n9020a(void);
    virtual ~ks_n9020a(void);
    virtual char* get_des();
};

#endif // KS_N9020A_H
