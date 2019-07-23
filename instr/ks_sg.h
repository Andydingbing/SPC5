#ifndef KS_N5182B_H
#define KS_N5182B_H

#include "sg.h"

class ks_n5182b : public sg
{
public:
    ks_n5182b();
    virtual int32_t get_default_pri_addr() { return -1; }
    virtual int32_t get_default_sec_addr() { return -1; }
    std::string get_descriptor() { return std::string("N5182B"); }

public:
    virtual bool reset();
    virtual bool set_cw(double freq);
    virtual bool get_cw(double &freq);
    virtual bool set_pl(double pwr);
    virtual bool get_pl(double &pwr);
    virtual bool set_en_output(bool en);
    virtual bool set_en_mod(bool en);

protected:
    double m_pwr;
};

class ks_n5182a : public ks_n5182b
{
public:
    std::string get_descriptor() { return std::string("N5182A"); }
};

class ks_e4438c : public ks_n5182b
{
public:
    std::string get_descriptor() { return std::string("E4438C"); }
};

class ks_e8267d : public ks_n5182b
{
public:
    std::string get_descriptor() { return std::string("E8267D"); }
};

#endif // KS_N5182B_H
