#include "complex_sequence.h"
#include "pthread.h"
#include <string.h>
#include <complex>
#include <math.h>
#include "algo_math.hpp"

#include "trace.inline.hpp"

pthread_mutex_t g_complex_sequence_lock = PTHREAD_MUTEX_INITIALIZER;

complex_sequence::complex_sequence()
{
    _full_scale = 134217728.0; // i/q = 8192,i^2+q^2
    _sr = 245760000;

    _dft_in = nullptr;
    _dft_out = nullptr;
    update_normalized_freq();
}

complex_sequence::~complex_sequence()
{
    fftw_free(_dft_in);
    fftw_free(_dft_out);
}

void complex_sequence::_new(uint32_t samples)
{
    pthread_mutex_lock(&g_complex_sequence_lock);

    size_t samples_befor = this->samples();

    if (samples > this->samples()) {
        fftw_free(_dft_in);
        fftw_free(_dft_out);

        _dft_in  = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * samples);
        _dft_out = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * samples);

    }

    _i.allocate(samples);
    _q.allocate(samples);
    _normalized_freq.allocate(samples);
    _magnitude.allocate(samples);

    if (samples != samples_befor) {
        update_normalized_freq();
    }

    pthread_mutex_unlock(&g_complex_sequence_lock);
}

void complex_sequence::set_sr(uint64_t sr)
{
    if (_sr != sr) {
        _sr = sr;
        update_normalized_freq();
    }
}

double complex_sequence::pwr_total() const
{
    uint64_t sum = 0;

    for (size_t i = 0;i < _i.size();++i) {
        sum += uint64_t(_i.at(i) * _i.at(i) + _q.at(i) * _q.at(i));
    }
    return ad_to_dBc(_full_scale,double(sum) / _i.size());
}

void complex_sequence::dft()
{
    fftw_plan plan = fftw_plan_dft_1d(int(samples()),_dft_in,_dft_out,FFTW_FORWARD,FFTW_ESTIMATE);

    for (uint32_t i = 0;i < samples();i ++) {
        _dft_in[i][0] = double(_i[i]);
        _dft_in[i][1] = double(_q[i]);
    }

    fftw_execute(plan);

//    for (uint32_t i = 0;i < samples() / 2;i ++) {
//        // positive frequencys
//        _normalized_freq[i + samples() / 2] = _sr / 2.0 / double(samples() / 2.0) * i / 1e6;

//        //negative frequencys
//        _normalized_freq[samples() / 2 - 1 - i] = _sr / 2.0 / double(samples() / 2.0) * i / 1e6 * -1;
//    }

    for (uint32_t i = 0;i < samples() / 2;i ++) {
        _magnitude.set_with_hold_type(i + samples() / 2,::magnitude(_dft_out[i][0]/samples(),_dft_out[i][1]/samples(),_full_scale));
        _magnitude.set_with_hold_type(i,::magnitude(_dft_out[i + samples() / 2][0]/samples(),_dft_out[i + samples() / 2][1]/samples(),_full_scale));
    }

    fftw_destroy_plan(plan);
}

void complex_sequence::update_normalized_freq()
{
    for (uint32_t i = 0;i < samples() / 2;i ++) {
        // positive frequencys
        _normalized_freq[i + samples() / 2] = _sr / 2.0 / double(samples() / 2.0) * i / 1e6;

        //negative frequencys
        _normalized_freq[samples() / 2 - 1 - i] = _sr / 2.0 / double(samples() / 2.0) * i / 1e6 * -1;
    }
}
