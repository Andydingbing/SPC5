#include "E4443A.h"

E4443A::E4443A(void) : N9030A()
{
}

E4443A::~E4443A(void)
{
}

char* E4443A::GetDes()
{
    return const_cast<char *>("E4443A");
}
