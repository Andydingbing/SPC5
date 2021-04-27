#include "ks_sg.h"
#include <boost/format.hpp>

using namespace std;

ks_n5182b::ks_n5182b() :
    sg()
{
    m_pwr = 0.0;
}

bool ks_n5182b::reset()
{
    return w("*RST");
}

bool ks_n5182b::set_cw(double freq)
{
    BOOL_CHECK(w((boost::format("FREQ:CW %.0f Hz") % freq).str()));
    BOOL_CHECK(set_pl(m_pwr));
	return true;
}

bool ks_n5182b::get_cw(double &freq)
{
    string buf;

    BOOL_CHECK(w("FREQ:CW?"));
    BOOL_CHECK(r(buf,256));
    freq = atof(buf.c_str());
	return true;
}

bool ks_n5182b::set_pl(double pwr)
{
    BOOL_CHECK(w((boost::format("POW %f dbm") % pwr).str()));
    m_pwr = pwr;
	return true;
}

bool ks_n5182b::get_pl(double &pwr)
{
    string buf;

    BOOL_CHECK(w("POW?"));
    BOOL_CHECK(r(buf,256));
    pwr = atof(buf.c_str());
	return true;
}

bool ks_n5182b::set_en_output(bool en)
{
    return en ? w("OUTP ON") : w("OUTP OFF");
}

bool ks_n5182b::set_en_mod(bool en)
{
    return en ? w("OUTP:MOD ON") : w("OUTP:MOD OFF");
}
