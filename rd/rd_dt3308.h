#ifndef RD_DT3308F_H
#define RD_DT3308F_H

#include "stdint_common.h"

#ifndef __cplusplus
#define __cplusplus
#endif

#if defined(__cplusplus) || defined(__cplusplus__)
extern "C" {
#endif

RD_API int32_t DT3308_RF_Boot();
RD_API int32_t DT3308_RF_SetTxFrequency(const uint64_t Freq);
RD_API int32_t DT3308_RF_SetRxFrequency(const uint64_t Freq);

#if defined(__cplusplus) || defined(__cplusplus__)
}
#endif

#endif // RD_DT3308F_H
