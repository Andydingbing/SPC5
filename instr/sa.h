#ifndef INSTR_SA_H
#define INSTR_SA_H

#include "gpib_dev.h"

class sa : public gpib_dev
{
public:
    enum det_type_t {
        DET_AUTO,
        DET_NORMAL,
        DET_AVERAGE,
        DET_PEAK,
        DET_SAMPLE,
        DET_NEG_PEAK
	};

    enum peak_search_type_t {
        PEAK_HIGHEST,
        PEAK_NEXT_HIGH,
        PEAK_NEXT_RIGHT,
        PEAK_NEXT_LEFT
	};

    enum marker_type_t {
        MARKER_NORMAL,
        MARKER_DELTA,
        MARKER_OFF
	};

    enum marker_move_type_t {
        MARKER_TO_CF,
        MARKER_TO_REFLVL
	};

    enum cal_type_t {
		AUTO_ON,
		AUTO_OFF,
		CAL_NOW
	};

public:
    virtual bool reset() = 0;
    virtual bool set_cf(double freq) = 0;
    virtual bool set_span(double freq) = 0;
    virtual bool set_rbw(double freq,bool is_auto) = 0;
    virtual bool set_vbw(double freq,bool is_auto) = 0;
    virtual bool set_det(det_type_t type) = 0;
    virtual bool set_peak_search(peak_search_type_t type) = 0;
    virtual bool set_marker(marker_type_t type) = 0;
    virtual bool set_cal(cal_type_t type) = 0;
    virtual bool set_avg_trace(bool en,uint32_t cnt) = 0;
    virtual bool set_ref(double ref) = 0;
    virtual bool get_ref(double &ref) = 0;
    virtual bool get_marker_pwr(double &pwr) = 0;
    virtual bool set_en_preamp(bool en) = 0;
    virtual bool get_marker_freq(double &freq) = 0;
    virtual bool wait_for_continue() = 0;
    virtual bool sweep_once() = 0;
    virtual bool marker_move(marker_move_type_t type) = 0;
    virtual bool marker_move(double freq) = 0;
    virtual bool set_avg_trace_get_data(uint32_t avg_cnt,uint32_t pt_cnt,double *data) = 0;
    virtual bool set_mech_att(bool is_auto,int32_t att) = 0;

public:
    bool set_ref_auto();
};

#endif // INSTR_SA_H
