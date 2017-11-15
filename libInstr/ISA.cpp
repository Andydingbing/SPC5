#include "ISA.h"
#ifdef _WIN64
#include <Windows.h>
#elif defined(__unix__) || defined(__linux__)
#include <unistd.h>
#endif

ISA::ISA(void)
{
}

ISA::~ISA(void)
{
}

bool ISA::SetRefAuto()
{
	double dRefLevel = 0.0;
	double dPeakPwr = 0.0;
	do {
		BOOL_CHECK(SweepOnce());
		BOOL_CHECK(SetPeakSearch(PeakHighest));
		BOOL_CHECK(GetMarkerPwr(dPeakPwr));
		BOOL_CHECK(GetRefLevel(dRefLevel));
		BOOL_CHECK(SetRefLevel((int)dPeakPwr + 5));
		BOOL_CHECK(SweepOnce());
		BOOL_CHECK(SetPeakSearch(PeakHighest));
		BOOL_CHECK(GetMarkerPwr(dPeakPwr));
	} while(dPeakPwr > dRefLevel);
#ifdef _WIN64
	Sleep(200);
#else
    usleep(200 * 1000);
#endif
	return true;
}
