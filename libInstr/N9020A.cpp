#include "N9020A.h"

N9020A::N9020A(void) : N9030A()
{
}

N9020A::~N9020A(void)
{
}

char* N9020A::GetDes()
{
    return const_cast<char *>("N9020A");
}
