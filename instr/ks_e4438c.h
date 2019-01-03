#ifndef KS_E4438C_H
#define KS_E4438C_H

#include "ks_n5182b.h"

class ks_e4438c : public ks_n5182b
{
public:
    ks_e4438c(void);
    virtual ~ks_e4438c(void);
    virtual char* get_des();
};

#endif // KS_E4438C_H
