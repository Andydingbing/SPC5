#ifndef KS_E4443A_H
#define KS_E4443A_H

#include "ks_n9030a.h"

class ks_e4443a : public ks_n9030a
{
public:
    ks_e4443a(void);
    virtual ~ks_e4443a(void);
    virtual char* get_des();
};

#endif // KS_E4443A_H
