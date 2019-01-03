#include "ks_e4443a.h"

ks_e4443a::ks_e4443a(void) : ks_n9030a()
{
}

ks_e4443a::~ks_e4443a(void)
{
}

char* ks_e4443a::get_des()
{
    return const_cast<char *>("E4443A");
}
