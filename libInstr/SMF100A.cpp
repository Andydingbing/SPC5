#include "SMF100A.h"

SMF100A::SMF100A(void)
{
}

SMF100A::~SMF100A(void)
{
}

char* SMF100A::GetDes()
{
    return const_cast<char *>("SMF100A");
}

bool SMF100A::SetModEnable(bool bEn)
{
	if (bEn)
		return Write("MOD:STAT ON");
	else
		return Write("MOD:STAT OFF");

	return true;
}
