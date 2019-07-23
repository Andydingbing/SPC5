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

#ifndef RD_UTILITIES_FREQ_STRING_HPP
#define RD_UTILITIES_FREQ_STRING_HPP

#include "rd.h"
#include "exception.hpp"
#include <vector>
#include <list>
#include <sstream>
#include <regex>
#include <cmath>
#include <algorithm>
#include <boost/format.hpp>
#include <boost/algorithm/string/trim.hpp>

/*!
 *
 * freq_string is a std::string type string with this format :
 *
 *     1. "freq_abbrev_string"(;|,)"freq_abbrev_string"(;|,)......([;,]?)
 *
 *     2. And freq_abbrev_string showing above must be one of these formats :
 *
 *        a. Normal : "([-+]?)(\\d*)(\\.?)(\\d+)([KMG]?)";
 *
 *           "123"  "+12.3"  "-.4"  "1K"  "12.3M"  "-.4G"
 *
 *        b. Scientific : "([-+]?)(\\d*)(\\.?)(\\d+)(e|E)(\\d*)(\\.?)(\\d+)[KMG]?";
 *
 *           "12e3"  "-12E4" "12.3e4" "12.3e.4"  "12e3K"  "-12E4M"  "12.3e1G"
 *
 *     3. These are valid freq strings :
 *
 *        "1M;2M;3M"  "1G,2G,3G,"  "1.2M;1e1M,.1G;"
 *
 *
 * range_freq_string is a group of
 * star_freq_string,stop_freq_string and step_freq_string :
 *
 *     1. Must : Section of star_freq_string = stop_freq_string.
 *
 *     2. No limit of step_freq_string as long as its section > 0.
 *
 *     3. Repeated results are ignored!
 *
 *     4. This is a valid range_freq_string :
 *
 *        range_freq_string
 *        .star           .stop             .step
 *          |               |                 |
 *        "10M;100M;1G"   "20M;150M;1.1G"   "10M"
 *
 *        It can be parsed to range_freqs<uint64_t>:
 *
 *        .freqs = {10M,20M,   100M,110M,120M,130M,140M,150M,   1G,1.01G,...1G}
 *
 *        .star = {10M,100M,1G}
 *
 *        .stop = {20M,150M,1.1G}
 *
 *        .max = 1.1G
 *
 *        .min = 10M
 *
 */

struct range_freq_string {
    std::string star;
    std::string stop;
    std::string step;
};

template <typename T>
struct range_freq {
    std::vector<T> freqs;
    std::vector<T> star;
    std::vector<T> stop;
    T max;
    T min;
};

/*!
 * Calculate which sectoin does "freq" in "range".
 * "range" must be valid and "freq" must be in range.
 *
 * e.g.
 * range_freq parsed from range_freq_string :
 * .star = "10M,1G"
 * .stop = "100M,2G"
 * .step = "10M"
 *
 * freq_section(10000000,range) = 0;
 * freq_section(1000000000,range) = 1;
 */
template <typename T>
uint32_t freq_section(T freq, range_freq<T> &range)
{
    RD_ASSERT_THROW(range.star.size() == range.stop.size());
    RD_ASSERT_THROW(freq >= range.min);
    RD_ASSERT_THROW(freq <= range.max);

    for (size_t i = 0;i < range.star.size();i ++) {
        if (freq >= range.star.at(i) && freq <= range.stop.at(i)) {
            return uint32_t(i);
        }
    }
    return 0;
}


/*!
 * Normal notation string to built in types.
 *
 * string ---> int/float/double/int8_t/......
 * "123" ----> 123(int)(u?int8_t)(u?int16_t)(u?int32_t)(u?int64_t)
 * "12.3" ---> 12.3(float)(double)
 * ......
 *
 * Type specialized functions :
 * normal_notation_string_to_int(xxx);
 * normal_notation_string_to_float(xxx);
 * normal_notation_string_to_double(xxx);
 * ......
 */
template <typename T>
T normal_notation_string_to_built_in(const std::string &str)
{
    T value = T(0);
    std::stringstream ss(str);
    ss >> value;
    return value;
}

#define IMPL_NORMAL_NOTATION_STRING_TO_(type) \
RD_INLINE type normal_notation_string_to_##type(const std::string &str) \
{ return normal_notation_string_to_built_in<type>(str); }

IMPL_NORMAL_NOTATION_STRING_TO_(int)
IMPL_NORMAL_NOTATION_STRING_TO_(float)
IMPL_NORMAL_NOTATION_STRING_TO_(double)
IMPL_NORMAL_NOTATION_STRING_TO_(int8_t)
IMPL_NORMAL_NOTATION_STRING_TO_(int16_t)
IMPL_NORMAL_NOTATION_STRING_TO_(int32_t)
IMPL_NORMAL_NOTATION_STRING_TO_(int64_t)
IMPL_NORMAL_NOTATION_STRING_TO_(uint8_t)
IMPL_NORMAL_NOTATION_STRING_TO_(uint16_t)
IMPL_NORMAL_NOTATION_STRING_TO_(uint32_t)
IMPL_NORMAL_NOTATION_STRING_TO_(uint64_t)


/*!
 * Scientific notation string to built in types.
 *
 * string ---> int/float/double/int8_t/......
 * "1e2" ----> 100(int)(u?int8_t)(u?int16_t)(u?int32_t)(u?int64_t)
 * "1E2" ----> 100.0(float)(double)
 * ......
 *
 * Type specialized functions :
 * scientific_notation_string_to_int(xxx);
 * scientific_notation_string_to_float(xxx);
 * scientific_notation_string_to_double(xxx);
 * ......
 */
template <typename T>
T scientific_notation_string_to_built_in(const std::string &str)
{
    std::string::size_type pos_e = str.find('e');
    std::string::size_type pos_E = std::string::npos;
    std::string::size_type pos = pos_e;
    double base = 0.0;
    double exponent = 0.0;
    double value = 0.0;
    
    if (pos_e == std::string::npos) {
        pos_E = str.find('E');
        pos = pos_E;
    }

    base = normal_notation_string_to_built_in<double>(str.substr(0,pos));
    exponent = normal_notation_string_to_built_in<double>(str.substr(pos + 1,str.length()));
    value = base * pow(10.0,exponent);
    return T(value);
}

#define IMPL_SCIENTIFIC_NOTATION_STRING_TO_(type) \
RD_INLINE type scientific_notation_string_to_##type(const std::string &str) \
{ return scientific_notation_string_to_built_in<type>(str); }

IMPL_SCIENTIFIC_NOTATION_STRING_TO_(int)
IMPL_SCIENTIFIC_NOTATION_STRING_TO_(float)
IMPL_SCIENTIFIC_NOTATION_STRING_TO_(double)
IMPL_SCIENTIFIC_NOTATION_STRING_TO_(int8_t)
IMPL_SCIENTIFIC_NOTATION_STRING_TO_(int16_t)
IMPL_SCIENTIFIC_NOTATION_STRING_TO_(int32_t)
IMPL_SCIENTIFIC_NOTATION_STRING_TO_(int64_t)
IMPL_SCIENTIFIC_NOTATION_STRING_TO_(uint8_t)
IMPL_SCIENTIFIC_NOTATION_STRING_TO_(uint16_t)
IMPL_SCIENTIFIC_NOTATION_STRING_TO_(uint32_t)
IMPL_SCIENTIFIC_NOTATION_STRING_TO_(uint64_t)


/*!
 * Frequency abbreviation string to type T.
 *
 * string -----> int/float/double/int8_t......
 * "123" ------> 123(int)(u?int8_t)(u?int16_t)(u?int32_t)(u?int64_t)
 * "1G" -------> 1000000000(int)(u?int32_t)(u?int64_t)
 * "2G" -------> 2000000000.0(float)(double)
 * "1.2M" -----> 1200000(int)(u?int32_t)(u?int64_t)
 * "1.2M" -----> 1200000.0(float)(double)
 * "2e3" ------> 2000(int)(u?int16_t)(u?int32_t)(u?int64_t)
 * "2.5E3" ----> 2500(int)(u?int16_t)(u?int32_t)(u?int64_t)
 * "2.5E1M" ---> 25000000.0(float)(double)
 *
 * Type specialized functions :
 * freq_string_to_int(xxx);
 * freq_string_to_float(xxx);
 * freq_string_to_double(xxx);
 * ......
 */
template <typename T>
T freq_string_to(const std::string &str)
{
    if (str.length() < 1) {
        RD_THROW sp_rd::syntax_error("Invalid string : \""
                                     + str
                                     + "\",it's empty.");
    }

    std::string str_copy = boost::trim_copy(str);
    std::string str_freq_numerical = str_copy;
    std::string pattern_normal = "([-+]?)(\\d*)(\\.?)(\\d+)";
    std::string pattern_scientific = "([-+]?)(\\d*)(\\.?)(\\d+)(e|E)(\\d*)(\\.?)(\\d+)";
    std::string::const_reference unit = str_copy.back();
    std::regex reg_normal(pattern_normal);
    std::regex reg_scientific(pattern_scientific);
    double freq_numerical = 0.0;
    bool with_unit = false;

    if (unit == 'K' || unit == 'M' || unit == 'G') {
        str_freq_numerical = std::string(str_copy,0,str_copy.length() - 1);
        with_unit = true;
    }

    if (str_copy.find(' ') != std::string::npos) {
        RD_THROW sp_rd::syntax_error("Invalid string : \""
                                     + str_copy
                                     + "\",with space character.");
    }
    if (std::regex_match(str_freq_numerical,reg_normal)) {
        freq_numerical = normal_notation_string_to_built_in<double>(str_freq_numerical);
    } else if (std::regex_match(str_freq_numerical,reg_scientific)) {
        freq_numerical = scientific_notation_string_to_built_in<double>(str_freq_numerical);
    } else {
        RD_THROW sp_rd::syntax_error("Invalid string : \""  + str_copy + "\".");
    }

    if (with_unit) {
        freq_numerical *= (unit == 'K' ? 1e3  :
                          (unit == 'M' ? 1e6  :
                          (unit == 'G' ? 1e9  : 1)));
    }

    return T(freq_numerical);
}

#define IMPL_FREQ_STRING_TO_(type) \
RD_INLINE type freq_string_to_##type(const std::string &str) \
{ return freq_string_to<type>(str); }

IMPL_FREQ_STRING_TO_(int)
IMPL_FREQ_STRING_TO_(float)
IMPL_FREQ_STRING_TO_(double)
IMPL_FREQ_STRING_TO_(int8_t)
IMPL_FREQ_STRING_TO_(int16_t)
IMPL_FREQ_STRING_TO_(int32_t)
IMPL_FREQ_STRING_TO_(int64_t)
IMPL_FREQ_STRING_TO_(uint8_t)
IMPL_FREQ_STRING_TO_(uint16_t)
IMPL_FREQ_STRING_TO_(uint32_t)
IMPL_FREQ_STRING_TO_(uint64_t)


/*!
 * Frequency abbreviation string from type T.
 * Only support integer types.
 *
 * int/int8_t/int16_t...... ---> String
 * 123 ------------------------> "123"
 * 1200 -----------------------> "1.2K"
 * 2000000 --------------------> "2M"
 * 3400000000 -----------------> "3.4G"
 *
 * Type specialized functions :
 * freq_string_from_int(xxx);
 * freq_string_from_int8_t(xxx);
 * freq_string_from_int16_t(xxx);
 * ......
 */
template <typename T>
std::string freq_string_from(const T &freq)
{
    char unit = '\0';
    double freq_copy = double(freq);
    std::string str;

    if (freq_copy < 0) {
        freq_copy *= -1.0;
        str += "-";
    }

    if (freq_copy >= 1e9) {
        freq_copy /= 1e9;
        unit = 'G';
    } else if (freq_copy >= 1e6) {
        freq_copy /= 1e6;
        unit = 'M';
    } else if (freq_copy >= 1e3) {
        freq_copy /= 1e3;
        unit = 'K';
    }

    boost::format fmt("%g");
    fmt % freq_copy;
    str += fmt.str();

    if (unit != '\0')
        str += unit;

    return str;
}

#define IMPL_FREQ_STRING_FROM_(type) \
RD_INLINE std::string freq_string_from_##type(const type &freq) \
{ return freq_string_from<type>(freq); }

IMPL_FREQ_STRING_FROM_(int)
IMPL_FREQ_STRING_FROM_(int8_t)
IMPL_FREQ_STRING_FROM_(int16_t)
IMPL_FREQ_STRING_FROM_(int32_t)
IMPL_FREQ_STRING_FROM_(int64_t)
IMPL_FREQ_STRING_FROM_(uint8_t)
IMPL_FREQ_STRING_FROM_(uint16_t)
IMPL_FREQ_STRING_FROM_(uint32_t)
IMPL_FREQ_STRING_FROM_(uint64_t)


/*!
 * Calculate how much sections does the string have.
 * The overload c-style function use a different algorithm.
 *
 * e.g.
 * str = "50M;1G,4G";
 * "str" has 3 sections(50M and 1G and 4G).
 */
uint32_t freq_string_sections(const std::string &str);

/*!
 * Make sure the end of "str" is '\0'!
 */
uint32_t freq_string_sections(const char *str);

/*!
 * Split freq_string.Usually called follow freq_string_sections.
 * So there is no syntax check.
 * The overload c-style function use a different algorithm.
 *
 * e.g.
 * str_in -------> str_out
 * "1G;2G;3G" ---> {"1G","2G","3G"}
 */
template <typename T>
void split_freq_string(const std::string &str_in, uint32_t sections, T &str_out)
{
    std::string::size_type length = str_in.length();

    RD_ASSERT_THROW(length > 0);

    std::string::size_type pos[2] = {0,0};
    std::string::size_type pos_start = 0;

    str_out.clear();

    for (uint32_t i = 0;i < sections;i ++) {
        pos[0] = str_in.find(';',pos_start);
        pos[1] = str_in.find(',',pos_start);
        pos[0] = (pos[0] != std::string::npos ?
                 (pos[1] != std::string::npos ? std::min(pos[0],pos[1]) : pos[0]) :
                 (pos[1] != std::string::npos ? pos[1] : length));

        str_out.push_back(str_in.substr(pos_start,pos[0] - pos_start));
        pos_start = pos[0] + 1;
    }
}

/*!
 * Make sure the end of "str" is '\0'!
 */
void split_freq_string(const char *str_in, uint32_t sections, char (*str_out)[32]);

template <typename T>
uint32_t parse_range_freq_string(const range_freq_string &freq_string, range_freq<T> &freq_out)
{
    uint32_t section_star = freq_string_sections(freq_string.star);
    uint32_t section_stop = freq_string_sections(freq_string.stop);
    uint32_t section_step = freq_string_sections(freq_string.step);

    RD_ASSERT_THROW(section_star == section_stop);

    freq_out.freqs.clear();
    freq_out.star.clear();
    freq_out.stop.clear();

    T T_freq_star = 0;
    T T_freq_stop = 0;
    T T_freq_step = 0;
    std::vector<std::string> splitted_freq_star;
    std::vector<std::string> splitted_freq_stop;
    std::vector<std::string> splitted_freq_step;
    typename std::vector<T>::iterator iter_freqs_repeated_begin;

    split_freq_string(freq_string.star,section_star,splitted_freq_star);
    split_freq_string(freq_string.stop,section_stop,splitted_freq_stop);
    split_freq_string(freq_string.step,section_step,splitted_freq_step);

    for (uint32_t i = 0;i < section_star;i ++) {
        T_freq_star = freq_string_to<T>(splitted_freq_star.at(i));
        T_freq_stop = freq_string_to<T>(splitted_freq_stop.at(i));
        T_freq_step = freq_string_to<T>(splitted_freq_step.at(std::min(i,section_step - 1)));

        freq_out.star.push_back(T_freq_star);
        freq_out.stop.push_back(T_freq_stop);

        for (uint32_t j = 0;j < SERIE_SIZE(T_freq_star,T_freq_stop,T_freq_step);j ++) {
            freq_out.freqs.push_back(T_freq_star + T(j) * T_freq_step);
        }
    }

    sort(freq_out.freqs.begin(),freq_out.freqs.end());

    iter_freqs_repeated_begin = unique(freq_out.freqs.begin(),freq_out.freqs.end());
    freq_out.freqs.erase(iter_freqs_repeated_begin,freq_out.freqs.end());

    freq_out.max = freq_out.freqs.at(freq_out.freqs.size() - 1);
    freq_out.min = freq_out.freqs.at(0);

    return uint32_t(freq_out.freqs.size());
}

#endif // RD_UTILITIES_FREQ_STRING_HPP
