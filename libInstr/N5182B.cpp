#include "N5182B.h"
#include <stdlib.h>
#include <string.h>

N5182B::N5182B() : ISG()
{
	m_dPwr = 0.0;
}

N5182B::~N5182B()
{
}

int32_t N5182B::GetDefaultPriAddr()
{
	return -1;
}

int32_t N5182B::GetDefaultSecAddr()
{
	return -1;
}

char* N5182B::GetDes()
{
    return const_cast<char *>("N5182B");
}

bool N5182B::Reset()
{
	return Write("*RST");
}

bool N5182B::SetCW(double dFreq)
{
	BOOL_CHECK(Write("FREQ:CW %.0f Hz",dFreq));
	BOOL_CHECK(SetPL(m_dPwr));
	return true;
}

bool N5182B::GetCW(double &dFreq)
{
	char szBuf[256] = {0};

	memset(szBuf,0,sizeof(szBuf));
	BOOL_CHECK(Write("FREQ:CW?"));
	BOOL_CHECK(Read(szBuf,256));
	dFreq = atof(szBuf);
	return true;
}

bool N5182B::SetPL(double dPwr)
{
	double dFreq = 0.0;
	
	BOOL_CHECK(Write("POW %f dbm",dPwr));
	m_dPwr = dPwr;
	return true;
}

bool N5182B::GetPL(double &dPwr)
{
	char szBuf[256] = {0};

	memset(szBuf,0,sizeof(szBuf));
	BOOL_CHECK(Write("POW?"));
	BOOL_CHECK(Read(szBuf,256));
	dPwr = atof(szBuf);
	return true;
}

bool N5182B::SetOutputEnable(bool bEn)
{
	if (bEn)
		return Write("OUTP ON");
	else
		return Write("OUTP OFF");
	return true;
}

bool N5182B::SetModEnable(bool bEn)
{
	if (bEn)
		return Write("OUTP:MOD ON");
	else
		return Write("OUTP:MOD OFF");
	return true;
}
