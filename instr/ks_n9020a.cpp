#include "ks_n9020a.h"

ks_n9020a::ks_n9020a(void) : ks_n9030a()
{
}

ks_n9020a::~ks_n9020a(void)
{
}

char* ks_n9020a::get_des()
{
    return const_cast<char *>("N9020A");
}
