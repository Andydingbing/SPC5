#include "ks_e4445a.h"

ks_e4445a::ks_e4445a(void) : ks_n9030a()
{
}

ks_e4445a::~ks_e4445a(void)
{
}

char* ks_e4445a::get_des()
{
    return const_cast<char *>("E4445A");
}

bool ks_e4445a::set_en_preamp(bool en)
{
    if (en) {
        BOOL_CHECK(w("POW:GAIN:STAT ON"));
	} else {
        BOOL_CHECK(w("POW:GAIN:STAT OFF"));
	}
	return true;
}
