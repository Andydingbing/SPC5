#include "ks_n5182b.h"
#include <stdlib.h>
#include <string.h>

ks_n5182b::ks_n5182b() : sg()
{
    m_pwr = 0.0;
}

ks_n5182b::~ks_n5182b()
{
}

int32_t ks_n5182b::get_default_pri_addr()
{
	return -1;
}

int32_t ks_n5182b::get_default_sec_addr()
{
	return -1;
}

char* ks_n5182b::get_des()
{
    return const_cast<char *>("N5182B");
}

bool ks_n5182b::reset()
{
    return w("*RST");
}

bool ks_n5182b::set_cw(double freq)
{
    BOOL_CHECK(w("FREQ:CW %.0f Hz",freq));
    BOOL_CHECK(set_pl(m_pwr));
	return true;
}

bool ks_n5182b::get_cw(double &freq)
{
    char buf[256] = {0};
    memset(buf,0,sizeof(buf));

    BOOL_CHECK(w("FREQ:CW?"));
    BOOL_CHECK(r(buf,256));
    freq = atof(buf);
	return true;
}

bool ks_n5182b::set_pl(double pwr)
{
    BOOL_CHECK(w("POW %f dbm",pwr));
    m_pwr = pwr;
	return true;
}

bool ks_n5182b::get_pl(double &pwr)
{
    char buf[256] = {0};
    memset(buf,0,sizeof(buf));

    BOOL_CHECK(w("POW?"));
    BOOL_CHECK(r(buf,256));
    pwr = atof(buf);
	return true;
}

bool ks_n5182b::set_en_output(bool en)
{
    if (en)
        return w("OUTP ON");
	else
        return w("OUTP OFF");
	return true;
}

bool ks_n5182b::set_en_mod(bool en)
{
    if (en)
        return w("OUTP:MOD ON");
	else
        return w("OUTP:MOD OFF");
	return true;
}
