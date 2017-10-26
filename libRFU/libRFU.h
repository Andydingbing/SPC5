#ifndef __LIBRFU_H__
#define __LIBRFU_H__

#include "stdint_common.h"

#define __cplusplus
#define libRFU

#ifdef libRFU
#else
#define libRFU __declspec(dllexport)
#endif

#if defined(__cplusplus) || defined(__cplusplus__)
extern "C" {
#endif

enum SAMPLERATE
{
	BW40,BW80,BW100,BW160
};
enum SOURCE
{
	ARB,FPGA,CW
};
enum CONNECTOR
{
	IO,OUTPUT,LOOP
};
enum RFU_CLOCKSOURCE
{
	RFU_CLOCK_INTERNAL,
	RFU_CLOCK_EXTERNAL
};
enum RFU_TRIGGERSOURCE
{
	RFU_TRIGGER_INTERNAL,
	RFU_TRIGGER_EXTERNAL
};
enum TRIGGERMODE
{
	IF,FREERUN,MARKER
};
enum BB_TRIGGERMODE
{
	BB_IF,BB_FREERUN,BB_MARKER
};
enum PROCESS
{
	IDLE_Driver,
	WFTrigger_Driver,
	Timeout_Driver,
	RUNNING_Driver,
	DONE_Driver
};
enum ARB_TRIGGERMODE
{
	ARB_MANUAL, ARB_CXUTRIGGER, ARB_FREERUN	
};

//启动函数列表
libRFU int32_t RF_SetBitPath(char *Path_0,char *Path_1);
libRFU int32_t RF_Boot();
libRFU int32_t RF_GetRFUNumber(uint32_t &uiRFUNumber);
libRFU int32_t RF_GetRFPortNumber(uint32_t &uiRFPortNumber);
libRFU int32_t RF_GetRFUSerialNumber(uint32_t RFUIndex,char *SerialNumber);
libRFU int32_t RF_GetRFUVersion(char *version);
//RF控制函数列表
libRFU int32_t RF_SetTxState(uint32_t RFIndex,bool State);
libRFU int32_t RF_SetTxPower(uint32_t RFIndex,float Power);
libRFU int32_t RF_SetTxFrequency(uint32_t RFIndex,uint64_t Freq);
libRFU int32_t RF_SetTxSampleRate(uint32_t RFIndex,SAMPLERATE SampleRate);
libRFU int32_t RF_SetTxDelay(uint32_t RFIndex,double Delay_ns);
libRFU int32_t RF_SetTxSource(uint32_t RFIndex,SOURCE Source);
libRFU int32_t RF_SetSourceFrequency(uint32_t RFIndex,uint64_t Freq);
libRFU int32_t RF_LoadARBSource(uint32_t RFIndex,char *filename);
//BBT ARB
libRFU int32_t RF_SetARBEnable(uint32_t RFIndex,bool bState);
libRFU int32_t RF_SetARBTriggerSource(uint32_t RFIndex,ARB_TRIGGERMODE Source);
libRFU int32_t RF_ExcuteARBTrigger(uint32_t RFIndex);
libRFU int32_t RF_SetARBCount(uint32_t RFIndex,int iCnt);
//BBT
libRFU int32_t BB_SetBBTestState(uint32_t RFIndex,bool InputSource);
libRFU int32_t BB_SetTriggerMode(uint32_t RFIndex,BB_TRIGGERMODE TriggerMode);
libRFU int32_t BB_SetTriggerLevel(uint32_t RFIndex,float TriggerLevel);
libRFU int32_t BB_SetTriggerOffset(uint32_t RFIndex,int Offset);
libRFU int32_t BB_SetCaptureLength(uint32_t RFIndex,int iMLength);
libRFU int32_t BB_InitBBTest(uint32_t RFIndex);
libRFU int32_t BB_AbortBBTest(uint32_t RFIndex);
libRFU int32_t BB_GetBBTestProcess(uint32_t RFIndex,PROCESS &Process);

libRFU int32_t RF_SetRxLevel(uint32_t RFIndex,double Level);
libRFU int32_t RF_SetRxFrequency(uint32_t RFIndex,uint64_t Freq);
libRFU int32_t RF_SetRxSampleRate(uint32_t RFIndex,SAMPLERATE SampleRate);
libRFU int32_t RF_SetRxDelay(uint32_t RFIndex,double Delay_ns);
libRFU int32_t RF_SetConnector(uint32_t RFIndex,CONNECTOR Connector);
//时钟与触发源函数列表
libRFU int32_t RF_SetClockSource(uint32_t RFIndex,RFU_CLOCKSOURCE ClockSource = RFU_CLOCK_INTERNAL);
libRFU int32_t RF_SetTriggerSource(uint32_t RFIndex,RFU_TRIGGERSOURCE TriggerSource = RFU_TRIGGER_INTERNAL);
//测量设置函数列表
libRFU int32_t RF_SetTriggerMode(uint32_t RFIndex,TRIGGERMODE TriggerMode = IF);
libRFU int32_t RF_SetTriggerLevel(uint32_t RFIndex,float TriggerLevel = 0);
libRFU int32_t RF_SetTriggerOffset(uint32_t RFIndex,uint32_t Offset);
libRFU int32_t RF_SetCaptureLength(uint32_t RFIndex,uint32_t MLength);
libRFU int32_t RF_SetIQDataMap(uint32_t RFIndex,uint16_t *pData);
//功率测量函数列表
libRFU int32_t RF_InitPowerMeasure(uint32_t RFIndex);
libRFU int32_t RF_AbortPowerMeasure(uint32_t RFIndex);
libRFU int32_t RF_GetMeasProcess(uint32_t RFIndex,PROCESS &Process);
libRFU int32_t RF_GetMeasResult(uint32_t RFIndex,float &Power,float &Crest);
//自检测量函数列表
libRFU int32_t RF_InitIQCapture(uint32_t RFIndex);
libRFU int32_t RF_AbortIQCapture(uint32_t RFIndex);
libRFU int32_t RF_GetIQCaptureProcess(uint32_t RFIndex,PROCESS &Process);
//系统控制函数列表
libRFU int32_t RF_WarningInfo(uint32_t RFIndex,uint32_t &State);
libRFU int32_t RF_GetTemperature(uint32_t RFIndex,double &TxTemperature,double &RxTemperature);

#if defined(__cplusplus) || defined(__cplusplus__)
}
#endif

#endif