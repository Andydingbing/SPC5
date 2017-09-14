#include "E4440A.h"

E4440A::E4440A(void) : N9030A()
{
}

E4440A::~E4440A(void)
{
}

char* E4440A::GetDes()
{
    return const_cast<char *>("E4440A");
}
