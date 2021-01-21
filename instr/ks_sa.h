#ifndef INSTR_KS_SA_H
#define INSTR_KS_SA_H

#include "sa.h"

class ks_n9030a : public sa
{
public:
    virtual int32_t	get_default_pri_addr() { return -1; }
    virtual int32_t	get_default_sec_addr() { return -1; }
    std::string get_descriptor() { return std::string("N9030A"); }

public:
    virtual bool reset() OVERRIDE;
    virtual bool set_cf(double freq) OVERRIDE;
    virtual bool set_span(double freq) OVERRIDE;
    virtual bool set_rbw(double freq,bool is_auto) OVERRIDE;
    virtual bool set_vbw(double freq,bool is_auto) OVERRIDE;
    virtual bool set_det(det_type_t type) OVERRIDE;
    virtual bool set_peak_search(peak_search_type_t type) OVERRIDE;
    virtual bool set_marker(marker_type_t type) OVERRIDE;
    virtual bool set_cal(cal_type_t type) OVERRIDE;
    virtual bool set_avg_trace(bool en,uint32_t cnt) OVERRIDE;
    virtual bool get_trace_avg_cnt(uint32_t &cnt) OVERRIDE;
    virtual bool set_ref(double ref) OVERRIDE;
    virtual bool get_ref(double &ref) OVERRIDE;
    virtual bool get_marker_pwr(double &pwr) OVERRIDE;
    virtual bool set_en_preamp(bool en) OVERRIDE;
    virtual bool is_en_preamp(bool &en) OVERRIDE;
    virtual bool is_en_avg_trace(bool &en) OVERRIDE;
    virtual bool get_marker_freq(double &freq) OVERRIDE;
    virtual bool wait_for_continue() OVERRIDE;
    virtual bool sweep_once() OVERRIDE;
    virtual bool marker_move(marker_move_type_t type) OVERRIDE;
    virtual bool marker_move(double freq) OVERRIDE;
    virtual bool set_avg_trace_get_data(uint32_t avg_cnt,uint32_t pt_cnt,double *data) OVERRIDE;
    virtual bool set_mech_att(bool is_auto,int32_t att) OVERRIDE;
};

class ks_e4440a : public ks_n9030a
{
public:
    std::string get_descriptor() { return std::string("E4440A"); }
};

class ks_e4443a : public ks_n9030a
{
public:
    std::string get_descriptor() { return std::string("E4443A"); }
};

class ks_e4445a : public ks_n9030a
{
public:
    std::string get_descriptor() { return std::string("E4445A"); }
    bool set_en_preamp(bool en);
};

class ks_n9010a : public ks_n9030a
{
public:
    std::string get_descriptor() { return std::string("N9010A"); }
};

class ks_n9020a : public ks_n9030a
{
public:
    std::string get_descriptor() { return std::string("N9020A"); }
};

class ks_n9020b : public ks_n9030a
{
public:
    std::string get_descriptor() { return std::string("N9020B"); }
};

#endif // INSTR_KS_SA_H
