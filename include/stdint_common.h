//
// Copyright 2018 StarPoint Inc.,Ltd
//

#ifndef STDINT_COMMON
#define STDINT_COMMON

#include "config.h"

/*!
  * \brief Bring in std type
  * \details Use platform related int8_t/int16_t/int32_t/int64_t,uint8_t/uint16_t/uint32_t/uint64_t....
  * There is no standard stdint define while MSC_VER <= 1500 in Windows,
  * and the stdint_msc.h is from the Internet.
  */
#ifdef RD_C_MSC

#if _MSC_VER <= 1500
#include "stdint_msc.h"
#else
#include <cstdint>
#endif

#elif defined RD_C_GNUC
#include "stdint.h"
#endif

#endif
