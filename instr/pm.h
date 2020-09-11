#ifndef INSTR_PM_H
#define INSTR_PM_H

#include "rd.h"
#include "visa.h"
#include <string>
#include <boost/noncopyable.hpp>

class pm : boost::noncopyable
{
public:
    virtual ~pm(void) {}
    virtual std::string get_descriptor() = 0;
    virtual bool init(const std::string &dev) = 0;

public:
    virtual bool reset() = 0;
    virtual bool set_freq(double freq) = 0;
    virtual bool get_pwr(double &pwr) = 0;
    virtual bool get_pwr(double freq,double &pwr) = 0;
    virtual bool close() = 0;

protected:
    unsigned long m_session;
};

#endif // INSTR_PM_H
