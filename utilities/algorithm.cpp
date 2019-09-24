#include "algorithm.h"
#include <limits>
#include <string>
#include <cstdlib>
#include "algo_math.h"
#include <boost/rational.hpp>

// unsigned __int64 get_cycle_count()
// {
// // #pragma warning ( push )
//  #pragma warning ( 2 : 4235 )
// 	__asm _emit 0x0f;
// 	__asm _emit 0x31;
// /*#pragma warning ( pop )*/
// }

double ad2dBc(int64_t x,int64_t x1)
{
    return 10.0 * log10(double(x1) / double(x));
}

double ad2dBc(double x,double x1)
{
    return 10.0 * log10(x1 / x);
}

float ad2dBc(float x,float x1)
{
    return 10.0f * log10(x1 / x);
}

int64_t dBc2ad(int64_t x,double dBc)
{
    return int64_t(pow(10.0,(-1.0 * dBc / 10.0)) * x);
}

void hmc624(double *x)
{
    *x = (int32_t(*x * 10) / 5) * 5 / 10.0;
}

uint32_t adf5355para2reg0(uint32_t _int,uint32_t prescaler,uint32_t autocal)
{
    return (0 | (_int << 4) | (prescaler << 20) | (autocal << 21));
}

uint32_t adf5355para2reg1(uint32_t _frac1)
{
    return (1 | (_frac1 << 4));
}

uint32_t adf5355para2reg2(uint32_t _mod2,uint32_t _frac2)
{
    return (2 | (_mod2 << 4) | (_frac2 << 18));
}

uint32_t adf5355para2reg3(uint32_t phase_value,uint32_t phase_adjust,uint32_t phase_resync,uint32_t sd_load_reset)
{
    return (3 | (phase_value << 4) | (phase_adjust << 28) | (phase_resync << 29) | (sd_load_reset << 30));
}

uint32_t adf5355para2reg4(uint32_t counter_reset,uint32_t cp_three_state,uint32_t power_down,uint32_t pd_polarity,uint32_t mux_logic,uint32_t ref_mode,uint32_t current_set,uint32_t double_buff,uint32_t r_counter,uint32_t rdiv2,uint32_t ref_doubler,uint32_t muxout)
{
    return (4 | (counter_reset << 4) | (cp_three_state << 5) | (power_down<< 6) | (pd_polarity << 7) | (mux_logic << 8) | (ref_mode << 9) | (current_set << 10) | (double_buff << 14) | (r_counter << 15) | (rdiv2 << 25) | (ref_doubler << 26) | (muxout << 27));
}

uint32_t adf5355para2reg5()
{
    return 0x00800025;
}

uint32_t adf5355para2reg6(uint32_t rf_output_power,uint32_t rf_out_a,uint32_t rf_out_b,uint32_t mtld,uint32_t charge_pump_bc,uint32_t rf_divider_sel,uint32_t feedback_sel,uint32_t negative_bleed,uint32_t gated_bleed)
{
    return (6 | (rf_output_power << 4) | (rf_out_a << 6) | (rf_out_b << 10) | (mtld << 11) | (charge_pump_bc << 13) | (rf_divider_sel << 21) | (feedback_sel << 24) | (0xa << 25) | (negative_bleed << 29) | (gated_bleed << 30));
}

uint32_t adf5355para2reg7(uint32_t ld_mode,uint32_t frac_n_ld_precison,uint32_t lol_mode,uint32_t ld_cycle_count,uint32_t lesync)
{
    return (7 | (ld_mode << 4) | (frac_n_ld_precison << 5) | (lol_mode << 7) | (ld_cycle_count << 8) | (lesync << 25) | (0x4 << 26));
}

uint32_t adf5355para2reg8()
{
    return 0x102d0428;
}

uint32_t adf5355para2reg9(uint32_t synth_lock_to,uint32_t auto_level_to,uint32_t timeout,uint32_t vco_band_div)
{
    return (9 | (synth_lock_to << 4) | (auto_level_to << 9) | (timeout << 14) | (vco_band_div << 24));
}

uint32_t adf5355para2reg10(uint32_t adc_enable,uint32_t adc_conversion,uint32_t adc_clock_div)
{
    uint32_t reserved_10 = strtol("000000001100000000",nullptr,2) << 14;
    return (10 | adc_enable << 4 | adc_conversion << 5 | adc_clock_div << 6 | reserved_10);
}

uint32_t adf5355para2reg11()
{
    return 0x0061300b;
}

uint32_t adf5355para2reg12(uint32_t resync_clock)
{
    uint32_t reserved_12 = strtol("000001000001",nullptr,2) << 4;
    return (12 | reserved_12 | (resync_clock << 16));
}

int32_t radix_chg(int32_t dec,char *result,int32_t radix)
{
    int i = 0;
    while (1) {
        if (dec / radix) {
            sprintf(&result[i],"%d",dec % radix);
            dec /= radix;
            i ++;
        }
        else {
            sprintf(&result[i],"%d",dec);
            break;
        }
    }
    char *revert = new char[i + 1];
    for (int n = 0;n <= i;n ++)
        revert[n] = result[i - n];
    for (int n = 0;n <= i;n ++)
        result[n] = revert[n];
    delete []revert;
    return 0;
}

int32_t ltc2666vol2dac(double vol)
{
    double v_min = -5.0;
    double v_max = 5.0;
    return int32_t(0 + 4095.0 * ((vol - v_min) / (v_max - v_min)));
}

double  ltc2666dac2vol(int32_t dac)
{
    double v_min = -5.0;
    double v_max = 5.0;
    return double(v_min + (v_max - v_min) * dac / (4095.0 - 0.0));
}

int32_t hmc1197_freq_formula_intmode(uint64_t freq,uint32_t ref_freq,uint32_t r_counter,uint32_t &_int)
{
    _int = uint32_t(round((double(freq) / (double(ref_freq) / double(r_counter))),1));
    return 0;
}

int32_t hmc1197_freq_formula_intmode(uint64_t freq,uint32_t ref_freq,uint32_t r_counter,uint32_t &_int,double &freq_error)
{
    double f_pd = double(ref_freq) / double(r_counter);
    _int = uint32_t(round((double(freq) / f_pd),1));
    freq_error = double(freq) - double(_int) * f_pd;
    return 0;
}

int32_t hmc1197_freq_formula_fracmode(uint64_t freq,uint32_t ref_freq,uint32_t r_counter,uint32_t &_int,uint32_t &_frac)
{
    double f_pd = double(ref_freq) / double(r_counter);
    double _int_all_part = double(freq) / f_pd;
    _int = uint32_t(round(_int_all_part,1));
    _frac = uint32_t(round((_int_all_part - double(_int)) * 16777216.0,0));
    return 0;
}

int32_t hmc1197_freq_formula_fracmode(uint64_t freq,uint32_t ref_freq,uint32_t r_counter,uint32_t &_int,uint32_t &_frac,double &freq_error)
{
    double f_pd = double(ref_freq) / double(r_counter);
    double _int_all_part = double(freq) / f_pd;
    _int = uint32_t(round(_int_all_part,1));
    _frac = uint32_t(round((_int_all_part - double(_int)) * 16777216.0,0));
    freq_error = double(freq) - (double(_int) + double(_frac) / 16777216.0) * f_pd;
    return 0;
}

int32_t hmc1197_freq_formula_exactmode(uint64_t freq,uint32_t ref_freq,uint32_t r_counter,uint32_t &_int,uint32_t &_frac,uint32_t &_chs)
{
    _frac = _chs = 0;
    double f_pd = double(ref_freq) / double(r_counter);
    double _int_all_part = double(freq) / f_pd;
    _int = uint32_t(round(_int_all_part,1));
    uint64_t f_gcd = boost::gcd(freq,uint64_t(ref_freq / r_counter));	//ref_freq = r_counter * integer default
    if (double(f_gcd) > (f_pd / 16777216.0)) {
        _chs = uint32_t(ref_freq / r_counter / f_gcd);
        _frac = uint32_t(round((_int_all_part - double(_int)) * 16777216.0,0));
        return 0;
    }
    else
        return -1;
}

uint32_t hmc1197para2reg00(uint32_t read_addr,uint32_t soft_rst)
{
    return (0x00 << 3 | (read_addr | soft_rst << 5) << 8);
}

uint32_t hmc1197para2reg01(uint32_t chip_en_pin_sel,uint32_t spi_chip_en,uint32_t keep_bias_on,uint32_t keep_pfd_on,uint32_t keep_cp_on,uint32_t keep_ref_buffer_on,uint32_t keep_vco_on,uint32_t keep_gpo_driver_on)
{
    return (0x01 << 3 | (chip_en_pin_sel | spi_chip_en << 1 | keep_bias_on << 2 | keep_pfd_on << 3 | keep_cp_on << 4 | keep_ref_buffer_on << 5 | keep_vco_on << 6 | keep_gpo_driver_on << 7) << 8);
}

uint32_t hmc1197para2reg02(uint32_t rdiv)
{
    return (0x02 << 3 | rdiv << 8);
}

uint32_t hmc1197para2reg03(uint32_t _int)
{
    return (0x03 << 3 | _int << 8);
}

uint32_t hmc1197para2reg04(uint32_t _frac)
{
    return (0x04 << 3 | _frac << 8);
}

uint32_t hmc1197para2reg05()
{
    return (0x05 << 3 | 0 << 8);
}

uint32_t hmc1197para2reg06(uint32_t dsm_order,uint32_t sync_spi_mode,uint32_t exact_freq_mode,uint32_t frac_bypass,uint32_t autoseed,uint32_t dsm_en)
{
    return (0x06 << 3 | (dsm_order << 2 | sync_spi_mode << 4 | exact_freq_mode << 5 | frac_bypass << 7 | autoseed << 8 | 3 << 9 | dsm_en << 11 | 48 << 12) << 8);
}

uint32_t hmc1197para2reg07(uint32_t lkd_wincnt_max,uint32_t ld_en,uint32_t ld_training,uint32_t csp_en)
{
    return (0x07 << 3 | (lkd_wincnt_max | 40 << 3 | ld_en << 11 | ld_training << 20 | csp_en << 21) << 8);
}

uint32_t hmc1197para2reg08(uint32_t gpo_en,uint32_t vco_buffer_prescaler_bias_en,uint32_t high_ref_freq,uint32_t sdo_output_level)
{
    return (0x08 << 3 | (31 | gpo_en << 5 | 15 << 6 | vco_buffer_prescaler_bias_en << 10 | 55 << 11 | high_ref_freq << 21 | sdo_output_level << 22) << 8);
}

uint32_t hmc1197para2reg09(uint32_t cp_down_gain,uint32_t cp_up_gain,uint32_t cp_offset,uint32_t offset_up_en,uint32_t offset_down_en,uint32_t hik_cp_mode)
{
    return (0x09 << 3 | (cp_down_gain | cp_up_gain << 7 | cp_offset << 14 | offset_up_en << 21 | offset_down_en << 22 | hik_cp_mode << 23) << 8);
}

uint32_t hmc1197para2reg0a(uint32_t vtune_resolution,uint32_t autocal_dis,uint32_t fsm_vspi_clk_sel,uint32_t auto_relock)
{
    return (0x0a << 3 | (vtune_resolution | 8 << 3 | autocal_dis << 11 | fsm_vspi_clk_sel << 13 | auto_relock << 17) << 8);
}

uint32_t hmc1197para2reg0b(uint32_t pd_phase_sel,uint32_t pd_up_output_en,uint32_t pd_down_output_en,uint32_t force_cp_up,uint32_t force_cp_down,uint32_t force_cp_mid)
{
    return (0x0b << 3 | (1 | pd_phase_sel << 4 | pd_up_output_en << 5 | pd_down_output_en << 6 | force_cp_up << 9 | force_cp_down << 10 | force_cp_mid << 11 | 120 << 12) << 8);
}

uint32_t hmc1197para2reg0c(uint32_t chs_per_fpd)
{
    return (0x0c << 3 | chs_per_fpd << 8);
}

uint32_t hmc1197para2reg0f(uint32_t gpo,uint32_t gpo_test_data,uint32_t prevent_automux_sdo,uint32_t pfet_dis,uint32_t nfet_dis)
{
    return (0x0f << 3 | (gpo | gpo_test_data << 5 | prevent_automux_sdo << 6 | pfet_dis << 8 | nfet_dis << 9) << 8);
}

uint32_t hmc1197para2reg14(uint32_t aux_spi_mode,uint32_t aux_gpo_value,uint32_t aux_gpo_output_level,uint32_t phase_sync,uint32_t aux_spi_gpo_output,uint32_t aux_spi_outputs)
{
    return (0x14 << 3 | (aux_spi_mode | aux_gpo_value << 1 | aux_gpo_output_level << 4 | 1 << 5 | phase_sync << 9 | aux_spi_gpo_output << 10 | aux_spi_outputs << 12) << 8);
}

uint32_t hmc1197para2reg15(uint32_t manual_cal_mode,uint32_t capacitor_switch_setting,uint32_t manual_vco_sel,uint32_t manual_vco_tune_en,uint32_t autoscale_cp_current_en)
{
    return (0x15 << 3 | (manual_cal_mode | capacitor_switch_setting << 1 | manual_vco_sel << 6 | manual_vco_tune_en << 9 | 18 << 10 | autoscale_cp_current_en << 16 | 7 << 17) << 8);
}

uint32_t hmc1197para2reg16(uint32_t rf_div_ratio,uint32_t lo1_output_gain,uint32_t lo2_output_gain,uint32_t div_output_gain)
{
    return (0x16 << 3 | (rf_div_ratio | lo1_output_gain << 6 | lo2_output_gain << 8 | div_output_gain << 10) << 8);
}

uint32_t hmc1197para2reg17(uint32_t vco_master_en,uint32_t vco_en,uint32_t ext_vco_buffer_en,uint32_t pll_en,uint32_t lo1_output_en,uint32_t lo2_output_en,uint32_t ext_input_en,uint32_t pre_lock_mute_en,uint32_t lo1_ouput_mode,uint32_t lo2_output_mode,uint32_t cp_output_sel)
{
    return (0x17 << 3 | (vco_master_en | vco_en << 1 | ext_vco_buffer_en << 2 | pll_en << 3 | lo1_output_en << 4 | lo2_output_en << 5 | ext_input_en << 6 | pre_lock_mute_en << 7 | lo1_ouput_mode << 8 | lo2_output_mode << 9 | cp_output_sel << 11) << 8);
}

uint32_t hmc1197para2reg18(uint32_t ext_input_bias)
{
    return (0x18 << 3 | (0x54c3 | ((ext_input_bias & 0x1) << 19) | (((ext_input_bias & 0x2) >> 1) << 20)) << 8);
}

uint32_t hmc1197para2reg19()
{
    return (0x19 << 3 | 0xab2 << 8);
}

uint32_t hmc1197para2reg1a(uint32_t dsm_seed)
{
    return (0x1a << 3 | dsm_seed << 8);
}

uint32_t hmc1197para2reg21(uint32_t harmonic_lpf_sel)
{
    return (0x21 << 3 | (harmonic_lpf_sel | 0xc1 << 16) << 8);
}

void sort_bubble(double *x,int32_t n)
{
    double xmin = x[0];
    for (int32_t i = 0;i < n - 1;i ++) {
        xmin = x[i];
        for (int32_t j = i;j < n;j ++) {
            if (x[j] < xmin) {
                xmin = x[j];
                x[j] = x[i];
                x[i] = xmin;
            }
        }
    }
}

void sort_bubble(double *x,double *y,int32_t n)
{
    double xmin = x[0];
    double ytemp = 0.0;
    for (int32_t i = 0;i < n - 1;i ++) {
        xmin = x[i];
        for (int32_t j = i + 1;j < n;j ++) {
            if (x[j] < xmin) {
                xmin = x[j];
                x[j] = x[i];
                x[i] = xmin;
                ytemp = y[j];
                y[j] = y[i];
                y[i] = ytemp;
            }
        }
    }

    int32_t num_y_dup = 0;
    for (int32_t i = 0;i < n - 1;i ++) {
        if (x[i + 1] != x[i])
            continue;
        else {
            num_y_dup = 1;
            while (x[i + num_y_dup + 1] == x[i + num_y_dup])
                num_y_dup ++;
            num_y_dup ++;
            sort_bubble(y + i,num_y_dup);
            i += num_y_dup - 1;
        }
    }
}

double sumxmul(double *a,double *b,int32_t n)
{
    double y = 0.0;
    for (int32_t i = 0;i <= n;i ++)
        y += a[i] * b[n - i];
    return y;
}

int32_t conv(double *a,int32_t m,double *b,int32_t n,double *coef)
{
    double *A = nullptr;
    double *B = nullptr;
    int32_t M = 0,N = 0;
    if (m >= n) {
        A = a;
        M = m;
        B = b;
        N = n;
    }
    else {
        A = b;
        M = n;
        B = a;
        N = m;
    }
    for (int32_t i = 0;i <= N;i ++)
        coef[i] = sumxmul(A,B,i);
    for (int32_t i = N + 1;i <= M;i ++)
        coef[i] = sumxmul(A + i - N,B,N);
    for (int32_t i = M + 1;i <= M + N;i ++)
        coef[i] = sumxmul(A + i - N,B + i - M,N + M - i);
    return 0;
}

int32_t lagrange(double *x,double *y,int32_t n,double *coef)
{
    double b[2] = {0.0,1.0};
    double divider = 1.0;
    int32_t degree = 0;

    double **l = new double *[n];
    for (int32_t i = 0;i < n;i ++) {
        coef[i] = 1.0;
        l[i] = new double[n];
        for (int32_t j = 1;j < n;j ++)
            l[i][j] = 0.0;
    }

    for (int32_t i = 0;i < n;i ++) {
        divider = 1.0;
        degree = 0;
        for (int32_t j = 0;j < n;j ++)
            coef[j] = 1.0;
        for (int32_t j = 0;j < n;j ++) {
            if (i == j) {
                b[0] = 1.0;
                b[1] = 0.0;
                conv(coef,degree,b,0,l[i]);
            }
            else {
                b[0] = x[j] * -1.0;
                b[1] = 1.0;
                divider *= (x[i] - x[j]);
                conv(coef,degree,b,1,l[i]);
                degree ++;
            }
            memcpy(coef,l[i],n * sizeof(double));
        }
        for (int32_t j = 0;j < n;j ++) {
            l[i][j] /= divider;
            l[i][j] *= y[i];
        }
    }
    for (int32_t i = 0;i < n;i ++) {
        coef[i] = 0;
        for (int32_t j = 0;j < n;j ++)
            coef[i] += l[j][i];
    }

    for (int32_t i = 0;i < n;i ++)
        delete [](l[i]);
    delete []l;
    return 0;
}

int32_t coef2polynomial(double *coef,int32_t n,char *pout)
{
    char rank[32] = {0};
    memset(rank,0,sizeof(rank));
    strcpy(pout,"f(x)=");
    for (int32_t i = n;i > 0;i --) {
        sprintf(rank,"%+.6fx^%d",coef[i],i);
        strcat(pout,rank);
    }
    sprintf(rank,"%+.6f",coef[0]);
    strcat(pout,rank);
    return 0;
}

int32_t interp1(double *x,double *y,int32_t n,double *x1,double *y1,interp_method method,int32_t I)
{
    int32_t i = 0;

    for (i = 0;i < n - 1;i ++) {
        x1[i * I] = x[i];
        y1[i * I] = y[i];
        for (int32_t j = 1;j < I;j ++)
            x1[i * I + j] = (x[i + 1] - x[i]) / I * j + x[i];
    }
    x1[i * I] = x[i];
    y1[i * I] = y[i];

    if (m_linear == method) {
        for (i = 0;i < n - 1;i ++) {
            for (int32_t j = 1;j < I;j ++)
                y1[i * I + j] = (y[i + 1] - y[i]) / I * j + y[i];
        }
    }
    else if (m_nearest == method) {
        for (i = 0;i < n - 1;i ++) {
            for (int32_t j = 1;j < I;j ++)
                y1[i * I + j] = y[i + 1];
        }
    }
    else if (m_zero == method) {
        for (i = 0;i < n - 1;i ++) {
            for (int32_t j = 1;j < I;j ++)
                y1[i * I + j] = 0;
        }
    }
    else if (m_spline == method) {
        double *a = new double[n - 1];
        double *b = new double[n - 1];
        double *c = new double[n - 1];
        double *d = new double[n - 1];
        spline(x,y,n,a,b,c,d,e_not_a_knot);

        double h = 0.0;
        for (i = 0;i < n - 1;i ++) {
            for (int32_t j = 1;j < I;j ++) {
                h = x1[i * I + j] - x[i];
                y1[i * I + j] = a[i] + b[i] * h + c[i] * h * h + d[i] * h * h * h;
            }
        }
        delete []d;
        delete []c;
        delete []b;
        delete []a;
    }
    return 0;
}

int32_t spline(double *x,double *y,int32_t n,double *a,double *b,double *c,double *d,spline_end end,double A,double B)
{
    double *X = new double[n * n];
    double *Y = new double[n];
    double *h = new double[n - 1];
    double *m = new double[n];
    memset(X,0,sizeof(double) * n * n);
    memset(Y,0,sizeof(double) * n);
    memset(h,0,sizeof(double) * (n - 1));
    memset(m,0,sizeof(double) * n);

    for (int32_t i = 0;i < n - 1;i ++)
        h[i] = x[i + 1] - x[i];

    if (e_natural == end) {
        X[0] = X[n * n - 1] = 1;
        Y[0] = Y[n - 1] = 0;
    }
    else if (e_clamped == end) {
        X[0] = h[0] * 2;
        X[1] = h[0];
        X[n * n - 1] = h[n - 2] * 2;
        X[n * n - 2] = h[n - 3];
        Y[0] = (y[1] - y[0]) / h[0] - A;
        Y[0] *= 6.0;
        Y[n - 1] = B - (y[n - 1] - y[n - 2]) / h[n - 2];
        Y[n - 1] *= 6.0;
    }
    else if (e_not_a_knot == end) {
        X[0] = h[1] * -1.0;
        X[1] = h[0] + h[1];
        X[2] = h[0] * -1.0;
        X[n * n - 1] = h[n - 3] * -1.0;
        X[n * n - 2] = h[n - 3] + h[n - 2];
        X[n * n - 3] = h[n - 2] * -1.0;
        Y[0] = Y[n - 1] = 0;
    }

    for (int32_t i = 1;i < n - 1;i ++) {
        X[i * n + i - 1] = h[i - 1];
        X[i * n + i]	 = (h[i - 1] + h[i]) * 2.0;
        X[i * n + i + 1] = h[i];
    }

    for (int32_t i = 1;i < n - 1;i ++) {
        Y[i] = (y[i + 1] - y[i]) / h[i] - (y[i] - y[i - 1]) / h[i - 1];
        Y[i] *= 6.0;
    }

 //   polynomial(X,Y,n,m);

    for (int32_t i = 0;i < n - 1;i ++) {
        a[i] = y[i];
        b[i] = (y[i + 1] - y[i]) / h[i] - h[i] * (2.0 * m[i] + m[i + 1]) / 6.0;
        c[i] = m[i] / 2.0;
        d[i] = (m[i + 1] - m[i]) / h[i] / 6.0;
    }

    delete []m;
    delete []h;
    delete []Y;
    delete []X;
    return 0;
}

int32_t sine(double A,double f,double phy,double B,double sr,double *x,double *y,int32_t samples)
{
    for (int32_t i = 0;i < samples;i ++) {
        x[i] = 1.0 / sr * double(i);
        y[i] = A * sin(2 * PI * f * x[i] + phy) + B;
    }
    return 0;
}

int32_t cosine(double A,double f,double phy,double B,double sr,double *x,double *y,int32_t samples)
{
    for (int32_t i = 0;i < samples;i ++) {
        x[i] = 1.0 / sr * double(i);
        y[i] = A * cos(2 * PI * f * x[i] + phy) + B;
    }
    return 0;
}
