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

#ifndef RD_UTILITIES_ALGO_FIT_H
#define RD_UTILITIES_ALGO_FIT_H

#include "rd.h"
#include "exception.hpp"
#include <boost/rational.hpp>

// Traits
template <typename T>
class fit_traits;

template<>
class fit_traits<double> {
public:
    typedef double data_t;
    typedef double float_t;
    static float_t value_float(data_t x) { return x; }
    static data_t _0() { return 0.0; }
    static data_t _1() { return 1.0; }
    static bool equal_0(data_t x) { return x == 0.0; }
    static bool equal_1(data_t x) { return x == 1.0; }
};

template<>
class fit_traits<float> {
public:
    typedef float data_t;
    typedef float float_t;
    static float_t value_float(data_t x) { return x; }
    static data_t _0() { return 0.0f; }
    static data_t _1() { return 1.0f; }
    static bool equal_0(data_t x) { return x == 0.0f; }
    static bool equal_1(data_t x) { return x == 1.0f; }
};

template <typename int_type>
class fit_traits<boost::rational<int_type>> {
public:
    typedef boost::rational<int_type> data_t;
    typedef double float_t;
    static float_t value_float(data_t x) { return float_t(x.numerator()) / float_t(x.denominator()); }
    static data_t _0() { return boost::rational<int_type>(); }
    static data_t _1() { return boost::rational<int_type>(1,1); }
    static bool equal_0(data_t x) { return x == _0(); }
    static bool equal_1(data_t x) { return x == _1(); }
};

template <typename int_type>
RD_INLINE boost::rational<int_type> pow(boost::rational<int_type> x,size_t y)
{ x.assign(int_type(std::pow(x.numerator(),y)),int_type(std::pow(x.denominator(),y))); return x; }

// x(0)^n + x(1)^n + x(2)^n + ... + x(x.size() - 1)^n
template <typename T,typename data_type = fit_traits<T>>
typename data_type::data_t sum_pow(const std::vector<typename data_type::data_t> &x,size_t n)
{
    typename data_type::data_t sum = data_type::_0();
    for (size_t i = 0;i < x.size();i ++) {
        sum += pow(x[i],n);
    }
    return sum;
}

// Value of polynomial
// coef(0) + coef(1)*x^1 + coef(2)*x^2 + ... + coef(coef.size() - 1)*x^n
template <typename T,typename data_type = fit_traits<T>>
typename data_type::float_t polynomial(const std::vector<typename data_type::data_t> &coef,typename data_type::data_t x)
{
    typename data_type::float_t y = 0.0;
    for (size_t i = 0;i < coef.size();i ++) {
        y += data_type::value_float(coef[i] * pow(x,i));
    }
    return y;
}

/*
 * Resolve polynomial
 *
 * 1.0*a + 2.0*b + 3.0*c = 1.0 // x : 1.0,2.0,3.0 // y : 1.0
 * 4.0*a + 5.0*b + 6.0*c = 2.0 // x : 4.0,5.0,6.0 // y : 2.0
 * 7.0*a + 8.0*b + 9.0*c = 3.0 // x : 7.0,8.0,9.0 // y : 3.0
 */
template <typename T,typename data_type = fit_traits<T>>
void polynomial(std::vector<std::vector<typename data_type::data_t>> x,
                std::vector<typename data_type::data_t> y,
                std::vector<typename data_type::data_t> &coef)
{
    /*
     * dimen_total === 3;
     *
     * x0 * a + x1 * b + x2 * c = y0; --- S(1) // dimen_cur = 3; dimen_cur_total = 3;
     * x3 * a + x4 * b + x5 * c = y1; --- S(2) // dimen_cur = 2; dimen_cur_total = 3;
     * x6 * a + x7 * b + x8 * c = y2; --- S(3) // dimen_cur = 1; dimen_cur_total = 3;
     *              |
     *              v
     * a + x1/x0 * b + x2/x0 * c = y0/x0; --- S(1) // dimen_cur = 3; dimen_cur_total = 3;
     * a + x4/x3 * b + x5/x3 * c = y1/x3; --- S(2) // dimen_cur = 2; dimen_cur_total = 3;
     * a + x7/x6 * b + x8/x6 * c = y2/x6; --- S(3) // dimen_cur = 1; dimen_cur_total = 3;
     *              |
     *              v
     * S(1)
     * S(2) - S(1) // dimen_cur = 2; dimen_cur_total = 2;
     * S(3) - S(1) // dimen_cur = 1; dimen_cur_total = 2;
     *              |
     *              v
     *            ......
     */

    typedef typename data_type::data_t data_t;

    RD_ASSERT_THROW(x.size() == y.size());

    const size_t dimen_total = x.size();
    size_t dimen_cur_total = dimen_total;
    size_t dimen_cur = dimen_total;
    data_t div = data_type::_0();

    coef.clear();
    for (size_t i = 0;i < dimen_total;i ++) {
        coef.push_back(data_type::_0());
    }

    for (dimen_cur = dimen_total;dimen_cur > 0;dimen_cur --) {
        dimen_cur_total = dimen_total - dimen_cur;
        for (size_t i = dimen_cur_total;i < dimen_total;i ++) {
            div = x[i][dimen_cur_total];
            if (!data_type::equal_0(div) && !data_type::equal_1(div)) {
                for (size_t j = dimen_cur_total;j < dimen_total;j ++) {
                    x[i][j] /= div;
                }
                y[i] /= div;
            }
        }
        for (size_t i = dimen_cur_total + 1;i < dimen_total;i ++) {
            if (!data_type::equal_0(x[i][dimen_cur_total])) {
                for (size_t j = dimen_cur_total;j < dimen_total;j ++) {
                    x[i][j] -= x[dimen_cur_total][j];
                }
                y[i] -= y[dimen_cur_total];
            }
        }
    }

    coef[dimen_total - 1] = y[dimen_total - 1];
    for (size_t i = dimen_total - 2;i > 0;i --) {
        for (size_t j = i + 1;j < dimen_total;j ++) {
            y[i] -= x[i][j] * coef[j];
        }
        coef[i] = y[i];
    }
    for (size_t j = 1;j < dimen_total;j ++) {
        y[0] -= x[0][j] * coef[j];
    }
    coef[0] = y[0];
}

/*
 * polyfit(Least Squares)
 *
 * e.g.
 * x : 1.0,2.0,3.0
 * y : 4.0,5.0,7.0
 *
 * So the points are (1.0,4.0),(2.0,5.0),(3.0,7.0).
 * If n = 2,the function will make an equation like "a + b*x + c*x^2 = 0",
 * and coef will store a,b,c.
 */
template <typename T,typename data_type = fit_traits<T>>
void polyfit(const std::vector<typename data_type::data_t> &x,
             const std::vector<typename data_type::data_t> &y,
             size_t n,
             std::vector<typename data_type::data_t> &coef)
{
    typedef typename data_type::data_t data_t;

    RD_ASSERT_THROW(x.size() == y.size());

    std::vector<data_t> xx;
    std::vector<data_t> YY;
    std::vector<std::vector<data_t>> XX;

    coef.clear();
    for (size_t i = 0;i <= n;i ++) {
        coef.push_back(data_type::_0());

        xx.clear();
        for (size_t j = 0;j <= n;j ++) {
            xx.push_back(sum_pow<data_t>(x,i + j));
        }
        XX.push_back(xx);
    }
    for (size_t i = 0;i <= n;i ++) {
        YY.push_back(data_type::_0());
        for (size_t j = 0;j < x.size();j ++) {
            YY[i] += pow(x[j],i) * y[j];
        }
    }
    polynomial<data_t>(XX,YY,coef);
}

#endif // RD_UTILITIES_ALGO_FIT_H
