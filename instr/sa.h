#ifndef INSTR_SA_H
#define INSTR_SA_H

#include "gpib_dev.h"

class RD_API sa : public gpib_dev
{
public:
    enum io_mixer_t {
        IO_MIXER_INTERNAL,
        IO_MIXER_EXTERNAL
    };

    enum io_impedance_t {
        IO_IMPEDANCE_50,
        IO_IMPEDANCE_75
    };

    enum io_coupling_t {
        IO_COUPLING_AC,
        IO_COUPLING_DC
    };

    enum io_ref_in_t {
        IO_REF_IN_SENSE,
        IO_REF_IN_INTERNAL,
        IO_REF_IN_EXTERNAL,
        IO_REF_IN_PULSE
    };

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
    virtual bool set_window_zoom(const bool is_zoomed) const;
    virtual bool set_window_next() const;
    virtual bool set_window_number(const uint8_t n) const;
    virtual bool get_window_number(uint8_t &n) const;
    virtual bool set_window_fullscreen(const bool is_full) const;
    virtual bool get_window_fullscreen(bool &is_full) const;
    virtual bool set_window_display(const bool is_on) const;
    virtual bool get_window_display(bool &is_on) const;

    virtual bool set_io_reset() const;
    virtual bool set_io_mixer(const io_mixer_t mixer) const;
    virtual bool get_io_mixer(io_mixer_t &mixer) const;
    virtual bool set_io_impedance(const io_impedance_t ohm) const;
    virtual bool get_io_impedance(io_impedance_t &ohm) const;
    virtual bool set_io_coupling(const io_coupling_t coupling) const;
    virtual bool get_io_coupling(io_coupling_t &coupling) const;
    virtual bool set_io_ref_in(const io_ref_in_t ref) const;
    virtual bool get_io_ref_in(io_ref_in_t &ref) const;
    virtual bool set_io_ref_ext_freq(const double freq) const;
    virtual bool get_io_ref_ext_freq(double &freq) const;

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
    virtual bool get_trace_avg_cnt(uint32_t &cnt) = 0;
    virtual bool set_ref(double ref) = 0;
    virtual bool get_ref(double &ref) = 0;
    virtual bool get_marker_pwr(double &pwr) = 0;
    virtual bool set_en_preamp(bool en) = 0;
    virtual bool is_en_preamp(bool &en) = 0;
    virtual bool is_en_avg_trace(bool &en) = 0;
    virtual bool get_marker_freq(double &freq) = 0;
    virtual bool wait_for_continue() = 0;
    virtual bool sweep_once() = 0;
    virtual bool marker_move(marker_move_type_t type) = 0;
    virtual bool marker_move(double freq) = 0;
    virtual bool set_avg_trace_get_data(uint32_t avg_cnt,uint32_t pt_cnt,double *data) = 0;
    virtual bool set_mech_att(bool is_auto,int32_t att) = 0;

public:
    bool set_ref_auto();
    bool set_ref_auto(double pwr,double &ref);
    bool set_ref_auto(double pwr,double &ref,double &offset);
};

#endif // INSTR_SA_H
