#include "E4445A.h"

E4445A::E4445A(void) : N9030A()
{
}

E4445A::~E4445A(void)
{
}

char* E4445A::GetDes()
{
    return const_cast<char *>("E4445A");
}

bool E4445A::SetEnablePreamp(bool bEn)
{
	if (bEn) {
		BOOL_CHECK(Write("POW:GAIN:STAT ON"));
	} else {
		BOOL_CHECK(Write("POW:GAIN:STAT OFF"));
	}
	return true;
}
