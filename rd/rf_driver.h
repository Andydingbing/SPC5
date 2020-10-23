#ifndef RD_RF_DRIVER_H
#define RD_RF_DRIVER_H

#include "rd_spc5.h"
#include "rd_sp9500.h"
#include "rd_dt3308.h"
#include "rd_sp9500x.h"

RD_API int32_t RF_SetLogSwitch(const uint32_t Switch);
RD_API int32_t RF_SetLogSwitchEnable(const RD_Log_Switch &Switch);

#endif // RD_RF_DRIVER_H
