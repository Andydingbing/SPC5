#include "rs_smf100a.h"

rs_smf100a::rs_smf100a(void)
{
}

rs_smf100a::~rs_smf100a(void)
{
}

char* rs_smf100a::get_des()
{
    return const_cast<char *>("SMF100A");
}

bool rs_smf100a::set_en_mod(bool en)
{
    if (en)
        return w("MOD:STAT ON");
	else
        return w("MOD:STAT OFF");

	return true;
}
