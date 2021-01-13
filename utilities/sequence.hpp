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

#ifndef RD_UTILITIES_SEQUENCE_HPP
#define RD_UTILITIES_SEQUENCE_HPP

#include "rd.h"
#include "set_helper.hpp"
#include <string>
#include <set>
#include <boost/any.hpp>

namespace sequence {

using namespace std;

template<typename T>
struct arithmetic_sequence_t
{
    T star;
    T step;
    T stop;

    size_t pts() const;

    void to_string(string &str) const;

    RD_INLINE string to_string() const
    { string str; to_string(str); return str; }
};


template<typename T,
template<typename E = arithmetic_sequence_t<T>,typename allocator_t = allocator<E>> class container_t>
int32_t parse(const string &str,container_t<arithmetic_sequence_t<T>> &out);

template<typename T,
template<typename E = arithmetic_sequence_t<T>,typename allocator_t = allocator<E>> class container_t> RD_INLINE
int32_t parse(const string &str,container_t<arithmetic_sequence_t<T>> *out)
{ return parse<T,container_t>(str,*out); }

template<typename T,
template<typename E = arithmetic_sequence_t<T>> class container_t>
int32_t parse(const string &str,container_t<arithmetic_sequence_t<T>> &out)
{ return parse<T,container_t>(str,out); }

template<typename T,
template<typename E = arithmetic_sequence_t<T>> class container_t> RD_INLINE
int32_t parse(const string &str,container_t<arithmetic_sequence_t<T>> *out)
{ return parse<T,container_t>(str,*out); }

template<typename T,
template<typename E = arithmetic_sequence_t<T>,typename allocator_t = allocator<E>> class container_t>
RD_INLINE container_t<arithmetic_sequence_t<T>> parse(const string &str)
{ container_t<arithmetic_sequence_t<T>> out; parse<T,container_t>(str,out); return out; }

template<typename T,
template<typename E = arithmetic_sequence_t<T>> class container_t>
RD_INLINE container_t<arithmetic_sequence_t<T>> parse(const string &str)
{ container_t<arithmetic_sequence_t<T>> out; parse<T,container_t>(str,out); return out; }


template<typename T,typename container_t = vector<arithmetic_sequence_t<T>>>
void parse(const container_t &in,string &str);

template<typename T,
template<typename E = arithmetic_sequence_t<T>,typename allocator_t = allocator<E>> class container_t>
RD_INLINE void parse(const container_t<arithmetic_sequence_t<T>> &in,string &str)
{ parse<T,container_t<>>(in,str); }

template<typename T,
template<typename E = arithmetic_sequence_t<T>> class container_t>
RD_INLINE void parse(const container_t<arithmetic_sequence_t<T>> &in,string &str)
{ parse<T,container_t<>>(in,str); }

template<typename T,typename container_t = vector<arithmetic_sequence_t<T>>>
RD_INLINE string parse(const container_t &in)
{ string str; parse<T,container_t>(in,str); return str; }

template<typename T,
template<typename E = arithmetic_sequence_t<T>,typename allocator_t = allocator<E>> class container_t>
RD_INLINE string parse(const container_t<arithmetic_sequence_t<T>> &in)
{ return parse<T,container_t<arithmetic_sequence_t<T>,allocator<arithmetic_sequence_t<T>>>>(in); }

template<typename T,
template<typename E = arithmetic_sequence_t<T>> class container_t>
RD_INLINE string parse(const container_t<arithmetic_sequence_t<T>> &in)
{ return parse<T,container_t<arithmetic_sequence_t<T>>>(in); }

} // namespace sequence

#include "sequence.inline.hpp"

#endif // RD_UTILITIES_SEQUENCE_HPP

