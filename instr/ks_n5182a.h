#ifndef KS_N5182A_H
#define KS_N5182A_H

#include "ks_n5182b.h"

class ks_n5182a : public ks_n5182b
{
public:
    ks_n5182a(void);
    virtual ~ks_n5182a(void);
    virtual char* get_des();
};

#endif // KS_N5182A_H
