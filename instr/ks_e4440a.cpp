#include "ks_e4440a.h"

ks_e4440a::ks_e4440a(void) : ks_n9030a()
{
}

ks_e4440a::~ks_e4440a(void)
{
}

char* ks_e4440a::get_des()
{
    return const_cast<char *>("E4440A");
}
