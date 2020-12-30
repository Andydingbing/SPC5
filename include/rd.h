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

#ifndef INCLUDE_RD_H
#define INCLUDE_RD_H

#ifndef _AMD64_
#define _AMD64_
#endif

//#ifndef _X86_
//#define _X86_
//#endif

#include "stdint_common.h"
#include <boost/noncopyable.hpp>
#include <boost/thread.hpp>

/*
 * nullptr_t & nullptr for compiler not support C++11 and later.
 * Re-precompile here to reduce the include files.
 */
#if (defined(__cplusplus) && (__cplusplus < 201103L))
#include "nullptr.h"
#endif // __cplusplus

#ifdef BOOST_THREAD_VERSION
#undef BOOST_THREAD_VERSION
#endif

#define BOOST_THREAD_VERSION 5

// struct align defines
#ifdef RD_C_MSC
    #define STRUCT_ALIGN_S(name,x) \
            __declspec(align(x)) struct name {

    #define STRUCT_ALIGN_E(name,x) \
            };

    #define STRUCT_ALIGN_API_S(name,x) \
            __declspec(align(x)) struct RD_API name {

    #define STRUCT_ALIGN_API_E(name,x) \
            };

    #define CLASS_ALIGN_S(name,x) \
            __declspec(align(x)) class class_name {

    #define CLASS_ALIGN_E(name,x) \
            };

    #define STRUCT_ALIGN_INHERIT_S(name,parent,x) \
            __declspec(align(x)) struct name : public parent {

    #define STRUCT_ALIGN_INHERIT_E(name,x) \
            };

    #define CLASS_ALIGN_INHERIT_S(name,parent,x) \
            __declspec(align(x)) class name : public parent {

    #define CLASS_ALIGN_INHERIT_E(name,x) \
            };
#elif defined RD_C_GNUC
    #define STRUCT_ALIGN_S(name,x) \
            struct name {

    #define STRUCT_ALIGN_E(name,x) \
            }__attribute__((aligned(x)));

    #define STRUCT_ALIGN_API_S(name,x) \
            struct RD_API name {

    #define STRUCT_ALIGN_API_E(name,x) \
            STRUCT_ALIGN_E(name,x)

    #define CLASS_ALIGN_S(name,x) \
            class name {

    #define CLASS_ALIGN_E(name,x) \
            }__attribute__((aligned(x)));

    #define STRUCT_ALIGN_INHERIT_S(name,parent,x) \
            struct name : public parent {

    #define STRUCT_ALIGN_INHERIT_E(name,x) \
            }__attribute__((aligned(x)));

    #define STRUCT_ALIGN_INHERIT_API_S(name,parent,x) \
            struct RD_API name : public parent {

    #define STRUCT_ALIGN_INHERIT_API_E(name,x) \
            }__attribute__((aligned(x)));

    #define CLASS_ALIGN_INHERIT_S(name,parent,x) \
            class name : public parent {

    #define CLASS_ALIGN_INHERIT_E(name,x) \
            }__attribute((aligned(x)));
#endif

#define FAKE_TEMPLATE_CLASS_PREFIX(name) \
template<int T = 0> class name##_t { public :

#define FAKE_TEMPLATE_CLASS_INHERIT_PREFIX(name,parent) \
template<int T = 0> class name##_t : public parent { public :

#define FAKE_TEMPLATE_CLASS_SUFFIX(name) \
};typedef name##_t<> name;

#define FAKE_TEMPLATE_CLASS_FUNC(name) \
template<int T> int32_t name##_t<T>

#define AD908X_FUNC  FAKE_TEMPLATE_CLASS_FUNC(ad908x)
#define ADF5355_FUNC FAKE_TEMPLATE_CLASS_FUNC(adf5355)
#define LMX2594_FUNC FAKE_TEMPLATE_CLASS_FUNC(lmx2594)

#define SAFE_NEW(ptr,t)         { if (ptr == nullptr) ptr = new t; }
#define SAFE_DEL(ptr)           { if (ptr != nullptr) {delete ptr;ptr = nullptr;} }
#define SAFE_NEW_ARRAY(a,t,n)   { if (a == nullptr) a = new t[n]; }
#define SAFE_DEL_ARRAY(a)       { if (a != nullptr) {delete []a;a = nullptr;} }
#define SAFE_CLOSE_FP(fp)       { if (fp != nullptr) {fclose(fp);fp = nullptr;} }

#define BOOL_CHECK(func)        { if(!(func)) {return false;} }
#define BOOL_CHECK_S(func,post) { if(!(func)) {post;return false;} }
#define INT_CHECK(func)         { if (int ret = func) {return ret;} }
#define INT_CHECKB(func)        { if (int ret = func) {return false;} }
#define INT_CHECKV(func)        { if (int ret = func) {return;} }
#define PTR_CHECKV(ptr)         { if (ptr == nullptr) {return;} }
#define VI_CHECK(func)          { if ((status = func) < VI_SUCCESS) { return status; }}

#define ARRAY_SIZE(array)          (uint32_t(sizeof(array) / sizeof(array[0])))
#define SERIE_SIZE(star,stop,step) (uint32_t((stop - star) / step + 1))
#define SERIE_INDEX(num,star,step) (uint32_t((num - star) / step))
#define SERIE_VALUE(star,step,i)   (star + step * i)

#define ZERO_ARRAY(array) { memset(array,0,sizeof(array)); }

#define BOOST_SPTR_SAFE_MAKE(type,sptr) { if (!sptr) sptr = boost::make_shared<type>(); }

#define FREQ_K(freq) freq##000ll
#define FREQ_M(freq) freq##000000ll
#define FREQ_G(freq) freq##000000000ll

#ifdef RD_C_MSC
    #pragma warning(disable : 4996)
    #pragma warning(disable : 4100)
    #pragma warning(disable : 4251)
    #pragma warning(disable : 4275)
#elif defined RD_C_GNUC
    #pragma GCC diagnostic ignored "-Wunused-parameter"
    #pragma GCC diagnostic ignored "-Wunused-variable"
    #pragma GCC diagnostic ignored "-Wunused-function"
    #pragma GCC diagnostic ignored "-Wmisleading-indentation"
    #pragma GCC diagnostic ignored "-Wunused-but-set-parameter"
#endif

namespace sp_rd {

class noncopyable
{
protected:
    constexpr noncopyable() = default;
    ~noncopyable() = default;
    noncopyable( const noncopyable& ) = delete;
    noncopyable& operator=( const noncopyable& ) = delete;
};

}

namespace rd {

enum link_t { UL,DL };

struct iq_data_t {
    int16_t i : 16;
    int16_t q : 16;
};

template<typename x_t,typename y_t>
struct point_2d { x_t x; y_t y; };

} // namespace rd

#endif // INCLUDE_RD_H
