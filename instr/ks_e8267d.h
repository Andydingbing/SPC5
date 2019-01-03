#ifndef KS_E8267D_H
#define KS_E8267D_H

#include "ks_n5182b.h"

class ks_e8257d : public ks_n5182b
{
public:
    ks_e8257d(void);
    virtual ~ks_e8257d(void);
    virtual char* get_des();
};

#endif // KS_E8267D_H
