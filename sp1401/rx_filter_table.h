#ifndef RX_FILTER_TABLE_H
#define RX_FILTER_TABLE_H
#include "cal_table.h"

namespace sp_rd {

// rf frequency response
class RD_API rx_rf_fr_table
{
public:
    typedef struct RD_API data_f_t {
        uint64_t freq;
        double   pwr;
	public:
        data_f_t();
    }data_f_t;
public:
    rx_rf_fr_table(){}
    virtual ~rx_rf_fr_table(){}
public:
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
    typedef struct RD_API data_f_t {
        double freq;
        double pwr;
	public:
        data_f_t();
    }data_f_t;
public:
    rx_if_fr_table(){}
    virtual ~rx_if_fr_table(){}
public:
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
    typedef struct RD_API data_f_t {
        uint64_t freq;
        int16_t  coef_real[RX_FILTER_ORDER];
        int16_t  coef_imag[RX_FILTER_ORDER];
        double   temp[4];
        tm       time;
        int32_t  rsv_0;
	public:
        data_f_t();
    }data_f_t;

    STRUCT_ALIGN_API_S(data_m_t,4)
        sp_rd::sp1401::coef_complex_t coef[RX_FILTER_ORDER];
    public:
        void _2double(double *real,double *imag);
    STRUCT_ALIGN_API_E(data_m_t,4)

    DECLARE_CAL_ITEM_TABLE(rx_filter_80m_table,RF_RX_FREQ_PTS_CALLED_R1C)
public:
    void get(uint64_t freq,data_m_t *data);
    void get(uint64_t freq,data_f_t *data);
};

class RD_API rx_filter_160m_table
{
public:
    typedef struct RD_API data_f_t {
        uint64_t freq;
        int16_t  coef_real[RX_FILTER_ORDER_2I3D];	//order 144 in file,maybe reserved for later change,but only 96 in memory
        int16_t  coef_imag[RX_FILTER_ORDER_2I3D];
        double   temp[4];
        tm       time;
        int32_t  rsv_0;
	public:
        data_f_t();
    }data_f_t;

    STRUCT_ALIGN_API_S(data_m_t,4)
        sp_rd::sp1401::coef_complex_t coef[RX_FILTER_ORDER];		//order 96 in memory
    public:
        void _2double(double *real,double *imag);
    STRUCT_ALIGN_API_E(data_m_t,4)

    DECLARE_CAL_ITEM_TABLE(rx_filter_160m_table,RF_RX_FREQ_PTS_CALLED_R1C)
public:
    void get(uint64_t freq,data_m_t *data);
    void get(uint64_t freq,data_f_t *data);
};

} //namespace sp_rd

#endif // RX_FILTER_TABLE_H
