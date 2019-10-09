/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef RD_UTILITIES_ALGO_MATH_H
#define RD_UTILITIES_ALGO_MATH_H

#include "rd.h"
#include "exception.hpp"
#include "traits.hpp"

double RD_API round(double x,uint32_t digit);

template <typename int_type>
RD_INLINE boost::rational<int_type> pow(boost::rational<int_type> x,size_t y)
{ x.assign(int_type(std::pow(x.numerator(),y)),int_type(std::pow(x.denominator(),y))); return x; }

template <typename T>
RD_INLINE T linear_quantify(const T star,const T step,const T value)
{ return uint64_t((value - star) / step) * step + star; }

// 1 / sqrt(x)
float RD_API inv_sqrt(float x);

template <typename dBc_t = double,typename ad_base_t,typename ad_t>
RD_INLINE dBc_t ad_to_dBc(ad_base_t x,ad_t x1)
{ return dBc_t(10.0 * log10(double(x1) / double(x))); }

template <typename ad_t,typename dBc_t = double>
RD_INLINE ad_t dBc_to_ad(ad_t x,dBc_t dBc)
{ return ad_t(pow(10.0,(-1.0 * double(dBc) / 10.0)) * x); }


// sumxmul = a[0] * b[n] + a[1] * b[n - 1] + a[2] * b[n - 2] + ... + a[n] * b[0]

// Unsafed overload
template <typename T,typename data_type = float_traits<T>>
T sumxmul(typename std::vector<T>::const_iterator a,
          typename std::vector<T>::const_iterator b,size_t n)
{
    T y = T(0);

    for (size_t i = 0;i <= n;i ++) {
        y += a[i] * b[n - i];
    }
    return y;
}

template <typename T,typename data_type = float_traits<T>>
T sumxmul(const std::vector<T> &a,typename std::vector<T>::const_iterator iter_a,
          const std::vector<T> &b,typename std::vector<T>::const_iterator iter_b,
          size_t n)
{
    if (iter_a - a.begin() + n >= a.size() || iter_b - b.begin() + n >= b.size()) {
        RD_THROW sp_rd::index_error(RD_THROW_SITE_INFO("Out of array range"));
    }

    return sumxmul<T>(iter_a,iter_b,n);
}

template <typename T,typename data_type = float_traits<T>>
T sumxmul(const std::vector<T> &a,const std::vector<T> &b,size_t n)
{ return sumxmul(a,a.begin(),b,b.begin(),n); }


// convolution and polynomial multiplication
template <typename T,typename data_type = float_traits<T>>
void conv(typename std::vector<T>::const_iterator iter_a_begin,size_t a_length,
          typename std::vector<T>::const_iterator iter_b_begin,size_t b_length,
          std::vector<T> &coef,typename std::vector<T>::iterator iter_coef)
{
    typedef typename std::vector<T>::const_iterator iter_type;

    iter_type A;
    iter_type B;

    a_length = (a_length == 0 || a_length == 1) ? (a_length + 1) : a_length;
    b_length = (b_length == 0 || b_length == 1) ? (b_length + 1) : b_length;
    size_t m = a_length - 1;
    size_t M = 0;
    size_t n = b_length - 1;
    size_t N = 0;

    if (coef.size() - (iter_coef - coef.begin()) < m + n + 1) {
        size_t coef_begin = iter_coef - coef.begin();
        coef.clear();
        coef.assign(m + n + 1,T(0));
        iter_coef = coef.begin() + coef_begin;
    }

    if (m >= n) {
        A = iter_a_begin;
        M = m;
        B = iter_b_begin;
        N = n;
    } else {
        A = iter_b_begin;
        M = n;
        B = iter_a_begin;
        N = m;
    }

    for (size_t i = 0;i <= N;i ++) {
        iter_coef[i] = (sumxmul<data_type::data_t>(A,B,i));
    }
    for (size_t i = N + 1;i <= M;i ++) {
        iter_coef[i] = (sumxmul<data_type::data_t>(A + (i - N),B,N));
    }
    for (size_t i = M + 1;i <= M + N;i ++) {
        iter_coef[i] = (sumxmul<data_type::data_t>(A + (i - N),B + (i - M),N + M - i));
    }
}

template <typename T,typename data_type = float_traits<T>>
void conv(const std::vector<T> &a,size_t a_length,
          const std::vector<T> &b,size_t b_length,
          std::vector<T> &coef,typename std::vector<T>::iterator iter_coef)
{ return conv(a.begin(),a_length,b.begin(),b_length,coef,iter_coef); }

template <typename T,typename data_type = float_traits<T>>
void conv(const std::vector<T> &a,size_t a_length,
          const std::vector<T> &b,size_t b_length,
          std::vector<T> &coef)
{ return conv(a.begin(),a_length,b.begin(),b_length,coef,coef.begin()); }

template <typename T,typename data_type = float_traits<T>>
void conv(const std::vector<T> &a,const std::vector<T> &b,std::vector<T> &coef)
{ return conv(a.begin(),a.size(),b.begin(),b.size(),coef,coef.begin()); }

#endif // RD_UTILITIES_ALGO_MATH_H
