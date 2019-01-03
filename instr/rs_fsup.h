#ifndef RS_FSUP_H
#define RS_FSUP_H

#include "sa.h"

class rs_fsup : public sa
{
public:
    rs_fsup();
    virtual ~rs_fsup();
    virtual int32_t	get_default_pri_addr();
    virtual int32_t	get_default_sec_addr();
    virtual char* get_des();
public:
    virtual bool reset();
    virtual bool set_cf(double freq);
    virtual bool set_span(double freq);
    virtual bool set_rbw(double freq,bool is_auto);
    virtual bool set_vbw(double freq,bool is_auto);
    virtual bool set_det(det_type_t type);
    virtual bool set_peak_search(peak_search_type_t type);
    virtual bool set_marker(marker_type_t type);
    virtual bool set_cal(cal_type_t type);
    virtual bool set_avg_trace(bool en,int32_t cnt);
    virtual bool set_ref(double ref);
    virtual bool get_ref(double &ref);
    virtual bool get_marker_pwr(double &pwr);
    virtual bool set_en_preamp(bool en);
    virtual bool get_marker_freq(double &freq);
    virtual bool wait_for_continue();
    virtual bool sweep_once();
    virtual bool marker_move(marker_move_type_t type);
    virtual bool marker_move(double freq);
    virtual bool set_avg_trace_get_data(int32_t avg_cnt,int32_t pt_cnt,double *data);
    virtual bool set_mech_att(bool is_auto,int32_t att);
};

#endif // RS_FSUP_H
