#ifndef INSTR_RS_SMF100A_H
#define INSTR_RS_SMF100A_H

#include "ks_sg.h"

class rs_smf100a : public ks_n5182b
{
public:
    std::string get_descriptor() { return std::string("SMF100A"); }

    bool set_en_mod(bool en)
    { return en ? w("MOD:STAT ON") : w("MOD:STAT OFF"); }
};

#endif // INSTR_RS_SMF100A_H
