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

#ifndef RD_UTILITIES_UTILITIES_HPP
#define RD_UTILITIES_UTILITIES_HPP

#include "rd.h"
#include <string>
#include <boost/format.hpp>

/*
 * Size abbreviation string from type T.
 * Only support integer types.
 *
 * int/int8_t/int16_t...... ---> String
 * 1048576 --------------------> "1M"
 *
 * Type specialized functions :
 * size_abbrev_from_int(xxx);
 * size_abbrev_from_int8_t(xxx);
 * size_abbrev_from_int16_t(xxx);
 * ......
 */
template <typename T>
std::string size_abbrev_from(const T &freq)
{
    char unit = '\0';
    double freq_copy = double(freq);
    std::string str;

    if (freq_copy < 0) {
        freq_copy *= -1.0;
        str += "-";
    }

    if (freq_copy >= (1024 * 1024 * 1024)) {
        freq_copy /= (1024 * 1024 * 1024);
        unit = 'G';
    } else if (freq_copy >= (1024 * 1024)) {
        freq_copy /= (1024 * 1024);
        unit = 'M';
    } else if (freq_copy >= 1024) {
        freq_copy /= 1024;
        unit = 'K';
    }

    boost::format fmt("%g");
    fmt % freq_copy;
    str += fmt.str();

    if (unit != '\0')
        str += unit;

    return str;
}

#define MAKE_FUNC_SIZE_ABBREV_FROM_(type) \
RD_INLINE std::string size_abbrev_from_##type(const type &freq) \
{ return size_abbrev_from<type>(freq); }

MAKE_FUNC_SIZE_ABBREV_FROM_(int)
MAKE_FUNC_SIZE_ABBREV_FROM_(int8_t)
MAKE_FUNC_SIZE_ABBREV_FROM_(int16_t)
MAKE_FUNC_SIZE_ABBREV_FROM_(int32_t)
MAKE_FUNC_SIZE_ABBREV_FROM_(int64_t)
MAKE_FUNC_SIZE_ABBREV_FROM_(uint8_t)
MAKE_FUNC_SIZE_ABBREV_FROM_(uint16_t)
MAKE_FUNC_SIZE_ABBREV_FROM_(uint32_t)
MAKE_FUNC_SIZE_ABBREV_FROM_(uint64_t)


template <typename T>
bool is_even(T value)
{
    return value & 1 == 0;
}

#endif // RD_UTILITIES_UTILITIES_HPP
