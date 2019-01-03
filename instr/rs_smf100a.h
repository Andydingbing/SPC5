#ifndef RS_SMF100A_H
#define RS_SMF100A_H

#include "ks_n5182b.h"

class rs_smf100a : public ks_n5182b
{
public:
    rs_smf100a(void);
    virtual ~rs_smf100a(void);
    virtual char* get_des();
    virtual bool set_en_mod(bool en);
};

#endif // RS_SMF100A_H
