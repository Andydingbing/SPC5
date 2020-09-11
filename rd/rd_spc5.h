#ifndef RD_SPC5_H
#define RD_SPC5_H

#include "stdint_common.h"
#include "rd_types.h"

#ifndef __cplusplus
#define __cplusplus
#endif

#if defined(__cplusplus) || defined(__cplusplus__)
extern "C" {
#endif

enum RD_SPC5_Trigger_Mode {
    SPC5_IF,
    SPC5_FreeRun,
    SPC5_Marker
};

enum RD_SPC5_Power_Meas_Process {
    SPC5_PM_Idle,
    SPC5_PM_Wait,
    SPC5_PM_Timeout,
    SPC5_PM_Running,
    SPC5_PM_Done
};

RD_API int32_t SPC5_RF_SetBitPath(char *Path);
RD_API int32_t SPC5_RF_Boot();
RD_API int32_t SPC5_RF_GetRFUNumber(uint32_t &uiRFUNumber);
RD_API int32_t SPC5_RF_GetRFPortNumber(uint32_t &uiRFPortNumber);
RD_API int32_t SPC5_RF_GetRFUSerialNumber(uint32_t RFUIndex,char *SerialNumber);
RD_API int32_t SPC5_RF_GetRFUVersion(char *version);

RD_API int32_t SPC5_RF_SetTxPower(uint32_t RFIndex,float Power);
RD_API int32_t SPC5_RF_SetTxFrequency(uint32_t RFIndex,uint64_t Freq);
RD_API int32_t SPC5_RF_SetTxSource(uint32_t RFIndex,RD_TX_Source Source);
RD_API int32_t SPC5_RF_SetSourceFrequency(uint32_t RFIndex,uint64_t Freq);

RD_API int32_t SPC5_RF_SetRxLevel(uint32_t RFIndex,double Level);
RD_API int32_t SPC5_RF_SetRxFrequency(uint32_t RFIndex,uint64_t Freq);
RD_API int32_t SPC5_RF_SetConnector(uint32_t RFIndex,RD_IO_Mode Connector);

RD_API int32_t SPC5_RF_SetTriggerMode(uint32_t RFIndex,RD_SPC5_Trigger_Mode TriggerMode);
RD_API int32_t SPC5_RF_SetTriggerLevel(uint32_t RFIndex,float TriggerLevel = 0);
RD_API int32_t SPC5_RF_SetCaptureLength(uint32_t RFIndex,uint32_t MLength);

RD_API int32_t SPC5_RF_InitPowerMeasure(uint32_t RFIndex);
RD_API int32_t SPC5_RF_AbortPowerMeasure(uint32_t RFIndex);
RD_API int32_t SPC5_RF_GetMeasProcess(uint32_t RFIndex,RD_SPC5_Power_Meas_Process &Process);
RD_API int32_t SPC5_RF_GetMeasResult(uint32_t RFIndex,float &Power,float &Crest);

RD_API int32_t SPC5_RF_GetTemperature(uint32_t RFIndex,double &TxTemperature,double &RxTemperature);

#if defined(__cplusplus) || defined(__cplusplus__)
}
#endif

#endif // RD_SPC5_H
