#ifndef __ALGORITHM_H__
#define __ALGORITHM_H__

#include "stdint_common.h"
#include <stdio.h>
#include <math.h>

#define PI 3.141592653589793

#define ARRAY_SIZE(array) (sizeof(array) / sizeof(array[0]))
#define SERIE_SIZE(star,stop,step) (int32_t)((stop - star) / step + 1)
#define SERIE_INDEX(num,star,step) (int32_t)((num - star) / step)

#define ANGLE2RADIAN(th) (th / 180.0 * PI)

//freq strings to exact number
//e.g. "50M"--->50000000	"1G"--->1000000000
int32_t str2freq(const char *str_freq, double   &freq);
int32_t str2freq(const char *str_freq, float    &freq);
int32_t str2freq(const char *str_freq, uint64_t &freq);
int32_t str2freq(const char *str_freq, int64_t  &freq);

//freq number to strings
//e.g. 50000000--->"50M"	1000000000--->"1G"
int32_t freq2str(double   freq, char *str_freq, int32_t digit = 0);
int32_t freq2str(float    freq, char *str_freq, int32_t digit = 0);
int32_t freq2str(uint64_t freq, char *str_freq, int32_t digit = 0);
int32_t freq2str(int32_t  freq, char *str_freq, int32_t digit = 0);
int32_t freq2str(int64_t  freq, char *str_freq, int32_t digit = 0);

//exact number to easy to see strings
//e.g. 1048576--->"1M"
int32_t byte2str(uint32_t byte, char *str_byte, int32_t digit = 0);

//coefficient to string
int32_t coef2str(int16_t *coef,int32_t order,char *buf);

//get how much sections does the strings have
//pin : e.g. "50M;1G,4G" return 3
int32_t strsections(const char *pin);

//freq range strings to freq array,return total freq points
//pin1 : e.g. "50M;1G,4G;"
//pin2 : e.g. "200M,2G;5G,"
//step : freq step
//pout : suppose pin1 pin2 are given like the example,pout will be "50M,100M,...200M,1G,1050M,...2G,4G,4050M,...5G",which are exact number
//pfreqstar : give every begin freq of every section
//pmax : the maximum freq
//pmin : the minimum freq
int32_t freq2array(const char *pin1,const char *pin2,int64_t step,int64_t *pout = NULL,int64_t *pfreqstar = NULL,int64_t *pfreqstop = NULL,int64_t *pmax = NULL,int64_t *pmin = NULL);

//separate freq string to freq array
int32_t freq2array(char *pin,int64_t *pout);

//get which section does the "freq" in
int32_t freqsection(char *pin1,char *pin2,int64_t freq);
int32_t freqsection(int64_t *pfreqstar,int64_t *pfreqstop,int64_t freq);
int32_t freqsection_s(int64_t *pfreqstar,int64_t *pfreqstop,int64_t freq,int32_t sec);

bool iseven(int32_t x);
bool iseven(uint32_t x);
bool isexactdiv(int32_t x,int32_t div);
double round(double x,int32_t digit);
uint32_t gcd(uint32_t x,uint32_t y);
uint64_t gcd(uint64_t x,uint64_t y);
float invsqrt(float x);						//1/sqrt()
double ad2dBc(int64_t x,int64_t x1);
double ad2dBc(double x,double x1);
int64_t dBc2ad(int64_t x,double dBc);
void hmc624(double *x);

template <class T_freq,class T_data>
inline void linear(T_freq x1,T_data y1,T_freq x2,T_data y2,T_freq x,T_data &y)
{
#ifdef _MSC_VER
#pragma warning ( push )
#pragma warning ( disable : 4244)
#endif
	y = (T_data)((((double)(x - x1)) / ((double)(x2 - x1))) * ((double)(y2 - y1)) + y1);
#ifdef _MSC_VER
#pragma warning ( pop )
#endif
}

template <class T_x>
void discretept(T_x x,int64_t step,T_x &x1,T_x &x2)
{
	T_x x_min = (T_x)((int64_t)(((int64_t)(x / (step * 1.0))) * step));
	if (x_min > x) {
		if (step < 0)
			x1 = x_min;
		else
			x1 = x_min - step;
	}
	else if (x_min < x) {
		if (step < 0)
			x1 = x_min - step;
		else
			x1 = x_min;
	}
	else {
		x1 = x_min;
	}
	x2 = x1 + step;
}

//unsigned __int64 GetCycleCount();

int32_t radixchg(int32_t dec,char *result,int32_t radix);

//ADF5355
int32_t adf5355_freq_formula(uint64_t freq,uint32_t ref_freq,bool ref_doubler,bool ref_divider,uint32_t r_counter,uint32_t f_chsp,uint32_t &_int,uint32_t &_frac1,uint32_t &_frac2,uint32_t &_mod1,uint32_t &_mod2);
int32_t adf5355_freq_formula(uint32_t _int,uint32_t _frac1,uint32_t _frac2,uint32_t _mod1,uint32_t _mod2,uint32_t ref_freq,double &freq);

uint32_t adf5355para2reg0(uint32_t _int,uint32_t prescaler,uint32_t autocal);
uint32_t adf5355para2reg1(uint32_t _frac1);
uint32_t adf5355para2reg2(uint32_t _mod2,uint32_t _frac2);
uint32_t adf5355para2reg3(uint32_t phase_value,uint32_t phase_adjust,uint32_t phase_resync,uint32_t sd_load_reset);
uint32_t adf5355para2reg4(uint32_t counter_reset,uint32_t cp_three_state,uint32_t power_down,uint32_t pd_polarity,uint32_t mux_logic,uint32_t ref_mode,uint32_t current_set,uint32_t double_buff,uint32_t r_counter,uint32_t rdiv2,uint32_t ref_doubler,uint32_t muxout);
uint32_t adf5355para2reg5();
uint32_t adf5355para2reg6(uint32_t rf_output_power,uint32_t rf_out_a,uint32_t rf_out_b,uint32_t mtld,uint32_t charge_pump_bc,uint32_t rf_divider_sel,uint32_t feedback_sel,uint32_t negative_bleed,uint32_t gated_bleed);
uint32_t adf5355para2reg7(uint32_t ld_mode,uint32_t frac_n_ld_precison,uint32_t lol_mode,uint32_t ld_cycle_count,uint32_t lesync);
uint32_t adf5355para2reg8();
uint32_t adf5355para2reg9(uint32_t synth_lock_to,uint32_t auto_level_to,uint32_t timeout,uint32_t vco_band_div);
uint32_t adf5355para2reg10(uint32_t adc_enable,uint32_t adc_conversion,uint32_t adc_clock_div);
uint32_t adf5355para2reg11();
uint32_t adf5355para2reg12(uint32_t resync_clock);

int32_t ltc2666vol2dac(double vol);
double  ltc2666dac2vol(int32_t dac);

int16_t x9119vol2val(double vol);
double  x9119val2vol(int16_t val);

double mcp3208ad2vol(uint32_t ad);
double tc1407vol2temp(double vol_mv);

//HMC1197
int32_t hmc1197_freq_formula_intmode(uint64_t freq,uint32_t ref_freq,uint32_t r_counter,uint32_t &_int);
int32_t hmc1197_freq_formula_intmode(uint64_t freq,uint32_t ref_freq,uint32_t r_counter,uint32_t &_int,double &freq_error);
int32_t hmc1197_freq_formula_fracmode(uint64_t freq,uint32_t ref_freq,uint32_t r_counter,uint32_t &_int,uint32_t &_frac);
int32_t hmc1197_freq_formula_fracmode(uint64_t freq,uint32_t ref_freq,uint32_t r_counter,uint32_t &_int,uint32_t &_frac,double &freq_error);
int32_t hmc1197_freq_formula_exactmode(uint64_t freq,uint32_t ref_freq,uint32_t r_counter,uint32_t &_int,uint32_t &_frac,uint32_t &_chs);
uint32_t hmc1197para2reg00(uint32_t read_addr,uint32_t soft_rst);
uint32_t hmc1197para2reg01(uint32_t chip_en_pin_sel,uint32_t spi_chip_en,uint32_t keep_bias_on,uint32_t keep_pfd_on,uint32_t keep_cp_on,uint32_t keep_ref_buffer_on,uint32_t keep_vco_on,uint32_t keep_gpo_driver_on);
uint32_t hmc1197para2reg02(uint32_t rdiv);
uint32_t hmc1197para2reg03(uint32_t _int);
uint32_t hmc1197para2reg04(uint32_t _frac);
uint32_t hmc1197para2reg05();
uint32_t hmc1197para2reg06(uint32_t dsm_order,uint32_t sync_spi_mode,uint32_t exact_freq_mode,uint32_t frac_bypass,uint32_t autoseed,uint32_t dsm_en);
uint32_t hmc1197para2reg07(uint32_t lkd_wincnt_max,uint32_t ld_en,uint32_t ld_training,uint32_t csp_en);
uint32_t hmc1197para2reg08(uint32_t gpo_en,uint32_t vco_buffer_prescaler_bias_en,uint32_t high_ref_freq,uint32_t sdo_output_level);
uint32_t hmc1197para2reg09(uint32_t cp_down_gain,uint32_t cp_up_gain,uint32_t cp_offset,uint32_t offset_up_en,uint32_t offset_down_en,uint32_t hik_cp_mode);
uint32_t hmc1197para2reg0a(uint32_t vtune_resolution,uint32_t autocal_dis,uint32_t fsm_vspi_clk_sel,uint32_t auto_relock);
uint32_t hmc1197para2reg0b(uint32_t pd_phase_sel,uint32_t pd_up_output_en,uint32_t pd_down_output_en,uint32_t force_cp_up,uint32_t force_cp_down,uint32_t force_cp_mid);
uint32_t hmc1197para2reg0c(uint32_t chs_per_fpd);
uint32_t hmc1197para2reg0f(uint32_t gpo,uint32_t gpo_test_data,uint32_t prevent_automux_sdo,uint32_t pfet_dis,uint32_t nfet_dis);
uint32_t hmc1197para2reg14(uint32_t aux_spi_mode,uint32_t aux_gpo_value,uint32_t aux_gpo_output_level,uint32_t phase_sync,uint32_t aux_spi_gpo_output,uint32_t aux_spi_outputs);
uint32_t hmc1197para2reg15(uint32_t manual_cal_mode,uint32_t capacitor_switch_setting,uint32_t manual_vco_sel,uint32_t manual_vco_tune_en,uint32_t autoscale_cp_current_en);
uint32_t hmc1197para2reg16(uint32_t rf_div_ratio,uint32_t lo1_output_gain,uint32_t lo2_output_gain,uint32_t div_output_gain);
uint32_t hmc1197para2reg17(uint32_t vco_master_en,uint32_t vco_en,uint32_t ext_vco_buffer_en,uint32_t pll_en,uint32_t lo1_output_en,uint32_t lo2_output_en,uint32_t ext_input_en,uint32_t pre_lock_mute_en,uint32_t lo1_ouput_mode,uint32_t lo2_output_mode,uint32_t cp_output_sel);
uint32_t hmc1197para2reg18(uint32_t ext_input_bias);
uint32_t hmc1197para2reg19();
uint32_t hmc1197para2reg1a(uint32_t dsm_seed);
uint32_t hmc1197para2reg21(uint32_t harmonic_lpf_sel);

//bubble sort,1 dimension,ascend mode
void sort_bubble(double *x,int32_t n);
//bubble sort,2 dimension,both x,y ascend mode
//if x has "3.0,3.0.3.0",and mapped y "6.0,5.0,4.0",the output y will be "4.0,5.0,6.0"
void sort_bubble(double *x,double *y,int32_t n);

//resolve polynomial
//x : e.g. 1.0,2.0,3.0,4.0,5.0,6.0,7.0,8.0,9.0
//y : e.g. 1.0,2.0,3.0
//n : e.g. 3
//this will resolve the polynomial shown below : 
//1.0*a + 2.0*b + 3.0*c = 1.0
//4.0*a + 5.0*b + 6.0*c = 2.0
//7.0*a + 8.0*b + 9.0*c = 3.0
//coef : will store a,b,c
int32_t polynomial(double *x,double *y,int32_t n,double *coef);

//value of polynomial
//a[0] + a[1]*x^1 + a[2]*x^2 + ... + a[n]*x^n
double polynomial(double *a,double x,int32_t n);

//sum of cross multiply array a and b
//a[0]*b[n] + a[1]*b[n-1] + a[2]*b[n-2] + ... + a[n]*b[0]
double sumxmul(double *a,double *b,int32_t n);

//convolution and polynomial multiplication
//a : e.g. 1.0,2.0,3.0
//m : e.g. 2
//b : e.g. 5.0,6.0,7.0,8.0,9.0
//n : e.g. 4
//(1.0 + 2.0*x * 3.0*x^2) * (5.0 + 6.0*x + 7.0*x^2 + 8.0*x^3 + 9.0*x^4),coef will store coef
int32_t conv(double *a,int32_t m,double *b,int32_t n,double *coef);

//lagrange interpolation
//x : e.g. 4.0, 5.0, 6.0
//y : e.g. 10.0,5.25,1.0 ---> f(4.0) = 10.0; f(5.0) = 5.25; f(6.0) = 1
//n : e.g. 3
//lagrange will make an function such as f(x) = a + b*x + c*x^2,and coef will store a,b,c
int32_t lagrange(double *x,double *y,int32_t n,double *coef);

//method of least squares
//x : e.g. 1.0,2.0,3.0
//y : e.g. 4.0,5.0,6.0
//m : e.g. 3
//n : e.g. 4
//coef : polyfit will make an equation like "a + b*x + c*x^2 + d*x^3 + e*x^4 = 0",and coef will store a,b,c,d,e
int32_t polyfit(double *x,double *y,int32_t m,int32_t n,double *coef);

//as shown before,pout will be something like "f(x)= a + b*x + c*x^2 + d*x^3 + e*x^4";
int32_t coef2polynomial(double *coef,int32_t n,char *pout);

//x(1)^n + x(2)^n + ... + x(m)^n
double sumpow(double *x,int32_t m,int32_t n);

//1 dimension data interpolation
////m_linear : linear interpolation,default
////m_nearest : nearest neighbor interpolation
////m_zeor : zero interpolation
////m_spline : cubic spline interpolation
//x,y : x,y data
//n   : x,y array size
//x1,y1 : output x,y data
//method : linear method,shown in the enum
//I : interpolation factor
enum interp_method {
	m_linear = 0,
	m_nearest = 1,
	m_zero = 2,
	m_spline = 3
};
int32_t interp1(double *x,double *y,int32_t n,double *x1,double *y1,interp_method method = m_linear,int32_t I = 2);

//cubic spline data interpolation
//x,y : x,y data
//n   : x,y array size
//a,b,c,d : each(total n - 1) coefficient of equation S(x) = a + b*(x-x[i]) + c*(x-x[i])^2 + d*(x-x[i])^3
//end : end condition
//A,B : e_clamped's end condition
enum spline_end {
	e_natural = 0,		//S"(0) = S"(n-1) = 0
	e_clamped = 1,		//S'(0) = A,S'(n-1) = B
	e_not_a_knot = 2	//S'''(0) = S'''(1),S'''(n-2) = S'''(n-1)
};
int32_t spline(double *x,double *y,int32_t n,double *a,double *b,double *c,double *d,spline_end end = e_not_a_knot,double A = 0.0,double B = 0.0);

//make a discrete array of sine or cosine wave
//A : peak amplitude
//f : frequency
//phy : original phase
//B : dc offset
//sr : sample rate
//x,y : output array
//samples : output array size,sample points
int32_t sine(double A,double f,double phy,double B,double sr,double *x,double *y,int32_t samples);
int32_t cosine(double A,double f,double phy,double B,double sr,double *x,double *y,int32_t samples);

#endif
