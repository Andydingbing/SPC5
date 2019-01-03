#include "ks_n9010a.h"

ks_n9010a::ks_n9010a(void) : ks_n9030a()
{
}

ks_n9010a::~ks_n9010a(void)
{
}

char* ks_n9010a::get_des()
{
    return const_cast<char *>("N9010A");
}
