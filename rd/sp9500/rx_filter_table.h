#ifndef RX_FILTER_TABLE_H
#define RX_FILTER_TABLE_H

#include "cal_table.h"

namespace rd {

// rf frequency response
class RD_API rx_rf_fr_table
{
public:
    struct RD_API data_f_t {
        uint64_t freq;
        double   pwr;

        data_f_t();
    };

    void virtual add(data_f_t *pData);
    void map2buf(data_f_t *buf) { m_data_f = buf; }
    void get(uint64_t freq,data_f_t *data);
    void save_as(const char *path);

private:					
    data_f_t *m_data_f;
};

// if frequency response
class RD_API rx_if_fr_table
{
public:
    struct RD_API data_f_t {
        double freq;
        double pwr;

        data_f_t();
    };

    void virtual add(data_f_t *data);
    void map2buf(data_f_t *buf) { m_data_f = buf; }
    void get(double freq,data_f_t *data);
    void save_as(const char *path);

private:
    data_f_t *m_data_f;
};

// filter bandwidth 80m/160m
class RD_API rx_filter_80m_table
{
public:
    struct RD_API data_f_t {
        uint64_t freq;
        int16_t  coef_real[ns_sp2401::ul_filter_tap];
        int16_t  coef_imag[ns_sp2401::ul_filter_tap];
        double   temp[4];
        tm       time;
        int32_t  rsv_0;

        data_f_t();
    };

    STRUCT_ALIGN_API_S(data_m_t,4)
        ns_sp1401::coef_complex_t coef[ns_sp2401::ul_filter_tap];
    public:
        void _2double(double *real,double *imag);
    STRUCT_ALIGN_API_E(data_m_t,4)

    DECLARE_CAL_ITEM_TABLE(rx_filter_80m_table,ns_sp1401::r1c::rx_freqs_called)
public:
    void get(uint64_t freq,data_m_t *data);
    void get(uint64_t freq,data_f_t *data);
};

class RD_API rx_filter_160m_table
{
public:
    struct RD_API data_f_t {
        uint64_t freq;
        int16_t  coef_real[ns_sp2401::ul_filter_tap_2i3d];	//order 144 in file,maybe reserved for later change,but only 96 in memory
        int16_t  coef_imag[ns_sp2401::ul_filter_tap_2i3d];
        double   temp[4];
        tm       time;
        int32_t  rsv_0;

        data_f_t();
    };

    STRUCT_ALIGN_API_S(data_m_t,4)
        ns_sp1401::coef_complex_t coef[ns_sp2401::ul_filter_tap]; // order 96 in memory
    public:
        void _2double(double *real,double *imag);
    STRUCT_ALIGN_API_E(data_m_t,4)

    DECLARE_CAL_ITEM_TABLE(rx_filter_160m_table,ns_sp1401::r1c::rx_freqs_called)
public:
    void get(uint64_t freq,data_m_t *data);
    void get(uint64_t freq,data_f_t *data);
};

} // namespace rd

#endif // RX_FILTER_TABLE_H
