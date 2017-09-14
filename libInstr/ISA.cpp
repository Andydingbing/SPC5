#include "ISA.h"
#include <Windows.h>

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
	Sleep(200);
	return true;
}