#include "N9010A.h"

N9010A::N9010A(void) : N9030A()
{
}

N9010A::~N9010A(void)
{
}

char* N9010A::GetDes()
{
    return const_cast<char *>("N9010A");
}
