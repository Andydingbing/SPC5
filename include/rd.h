//
// Copyright 2018 StarPoint Inc.,Ltd
//

#ifndef RD_H
#define RD_H

#ifndef _AMD64_
#define _AMD64_
#endif

#include "stdint_common.h"

//! struct align defines
#ifdef RD_C_MSC
#define STRUCT_ALIGN_S(name,x)  \
        typedef __declspec(align(x)) struct name {

#define STRUCT_ALIGN_E(name,x)  \
        }name;

#define STRUCT_ALIGN_API_S(name,x)	\
		typedef __declspec(align(x)) struct RD_API name {

#define STRUCT_ALIGN_API_E(name,x)	\
		}name;

#define CLASS_ALIGN_S(name,x)   \
        __declspec(align(x)) class class_name {

#define CLASS_ALIGN_E(name,x)   \
                };

#define STRUCT_ALIGN_INHERIT_S(name,parent,x)   \
        typedef __declspec(align(x)) struct name : public parent {

#define STRUCT_ALIGN_INHERIT_E(name,x)          \
        }name;

#define CLASS_ALIGN_INHERIT_S(name,parent,x)    \
        __declspec(align(x)) class name : public parent {

#define CLASS_ALIGN_INHERIT_E(name,x)           \
                };
#elif defined RD_C_GNUC
#define STRUCT_ALIGN_S(name,x)  \
        typedef struct name {

#define STRUCT_ALIGN_E(name,x)  \
        }__attribute__((aligned(x))) name;

#define STRUCT_ALIGN_API_S(name,x)  \
        typedef struct RD_API name {

#define STRUCT_ALIGN_API_E(name,x)  \
        STRUCT_ALIGN_E(name,x)

#define CLASS_ALIGN_S(name,x)   \
        class name {

#define CLASS_ALIGN_E(name,x)   \
        }__attribute__((aligned(x)));

#define STRUCT_ALIGN_INHERIT_S(name,parent,x)   \
        typedef struct name : public parent {

#define STRUCT_ALIGN_INHERIT_E(name,x)          \
        }__attribute__((aligned(x))) name;

#define STRUCT_ALIGN_INHERIT_API_S(name,parent,x)   \
        typedef struct RD_API name : public parent {

#define STRUCT_ALIGN_INHERIT_API_E(name,x)          \
        }__attribute__((aligned(x))) name;

#define CLASS_ALIGN_INHERIT_S(name,parent,x)    \
        class name : public parent {

#define CLASS_ALIGN_INHERIT_E(name,x)           \
        }__attribute((aligned(x)));
#endif

#define SAFE_NEW(ptr,t)         { if (!ptr) ptr = new t; }
#define SAFE_DELETE(ptr)        { if (ptr) {delete ptr;ptr = NULL;} }
#define SAFE_DELETE_ARRAY(a)    { if (a) {delete []a;a = NULL;} }
#define SAFE_DELETE_VECTOR(v)   { if (v) {v->clear();delete v;v = NULL;} }
#define SAFE_CLOSE_FP(fp)       { if (fp) {fclose(fp);fp = NULL;} }

#define BOOST_SPTR_SAFE_MAKE(type,sptr) { if (!sptr) sptr = boost::make_shared<type>(); }

#define BOOL_CHECK(func)        { if(!(func)) {return false;} }
#define BOOL_CHECK_S(func,post) { if(!(func)) {post;return false;} }
#define INT_CHECK(func)         { if (int ret = func) {return ret;} }
#define INT_CHECKB(func)        { if (int ret = func) {return false;} }
#define INT_CHECKV(func)        { if (int ret = func) {return;} }
#define PTR_CHECKV(ptr)         { if (!ptr) return; }

#define ARRAY_SIZE(array)           (int32_t)(sizeof(array) / sizeof(array[0]))
#define SERIE_SIZE(star,stop,step)  (int32_t)((stop - star) / step + 1)
#define SERIE_INDEX(num,star,step)  (int32_t)((num - star) / step)

#define PI 3.141592653589793
#define ANGLE2RADIAN(th) (th / 180.0 * PI)

#ifdef RD_C_MSC
#pragma warning ( disable : 4996 )
#pragma warning ( disable : 4100 )
#pragma warning ( disable : 4251 )
#elif defined RD_C_GNUC
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wmisleading-indentation"
#endif

namespace sp_rd {

typedef struct iq_data_t {
    short i : 16;
    short q : 16;
}iq_data_t;

} //namespace sp_rd

#endif
