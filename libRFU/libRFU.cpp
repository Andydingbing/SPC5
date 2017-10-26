#include "libLog.h"
#include "libBusDriver.h"
#include "libSP3301.h"
#include "libSP3501.h"
#include "libRFU.h"
#include "RegDef.h"
#include <vector>

typedef struct AvailableModule {
	CSP3301 *m_pSP3301;	//which rfu does this rf module belong
	uint32_t m_uiRfIdx;	//the rf module index of particular rfu above
}AvailableModule;

vector <AvailableModule> g_Module;

#define DECLARE_DYNAMIC_SP3301						\
	if (RFIndex > g_Module.size())					\
		return -1;									\
	CSP3301 *pSP3301 = g_Module[RFIndex].m_pSP3301;	\
	uint32_t uiRfIdx = g_Module[RFIndex].m_uiRfIdx;

int32_t RF_SetBitPath(char *Path_0,char *Path_1)
{
	INT_CHECK(SP3301_2.SetLoadBit(true,true,Path_0,Path_1));
	INT_CHECK(SP3301_3.SetLoadBit(true,true,Path_0,Path_1));
	return 0;
}

int32_t RF_Boot()
{
	Log.SetEnable(L_ALL_OFF,true);
	Log.SetEnable(L_PROMPT,true);
	g_Module.clear();
	AvailableModule AvaiModule;
	CSP3301::Active RfuActiveInfo[MAX_RFU];

	SP3301_2.Boot();
	SP3301_3.Boot();
	SP3301_2.LoadBit();
	SP3301_3.LoadBit();
	SP3301_2.Boot();
	SP3301_3.Boot();

	int16_t iOCXO = 0;
	SP3301_2.GetOCXO(iOCXO);
	Log.stdprintf("ocxo : %d\n",iOCXO);
	SP3501.OpenBoard();
	SP3501.Vol9119(iOCXO);

	RfuActiveInfo[2] = SP3301_2.GetActive();
	RfuActiveInfo[3] = SP3301_3.GetActive();

	for (int i = 0;i < MAX_RF;i ++) {
		if (RfuActiveInfo[2].m_bSP1401[i]) {
			AvaiModule.m_pSP3301 = &SP3301_2;
			AvaiModule.m_uiRfIdx = i;
			g_Module.push_back(AvaiModule);
		}
	}
	for (int i = 0;i < MAX_RF;i ++) {
		if (RfuActiveInfo[3].m_bSP1401[i]) {
			AvaiModule.m_pSP3301 = &SP3301_3;
			AvaiModule.m_uiRfIdx = i;
			g_Module.push_back(AvaiModule);
		}
	}
	return 0;
}

int32_t RF_GetRFUNumber(uint32_t &uiRFUNumber)
{
	uiRFUNumber = 0;
	CSP3301::RFUInfo RfuInfo[MAX_RF];
	vector<string> strRsrcRfu;
	vector<string>::iterator iterRsrcRfu;
	viPCIDev viDev;

	char szProductForm[64];
	char szRsrcSection[64];
	char szDevKey[64];
	memset(szProductForm,0,ARRAY_SIZE(szProductForm));
	memset(szRsrcSection,0,ARRAY_SIZE(szRsrcSection));
	memset(szDevKey,0,ARRAY_SIZE(szDevKey));

	CGeneralIniFile IniFile(CONFIG_FILE_PATH);

	bool bK7_0_Active[MAX_RFU],bK7_1_Active[MAX_RFU],bS6_Active[MAX_RFU];
	for (int32_t i = 0;i < MAX_RFU;i ++) {
		bK7_0_Active[i] = false;
		bK7_1_Active[i] = false;
		bS6_Active[i] = false;
	}

	IniFile.GetConfigStringValue("Product Form","Form",szProductForm);
	strcpy(szRsrcSection,"RESOURCE");
	strcat(szRsrcSection,szProductForm);
	for (int32_t i = 0;i < MAX_RFU;i ++) {
		sprintf(szDevKey,"RFU%d_K7_0",i);
		IniFile.GetConfigStringValue(szRsrcSection,szDevKey,RfuInfo[i].m_RsrcName.m_K7_0);
		sprintf(szDevKey,"RFU%d_K7_1",i);
		IniFile.GetConfigStringValue(szRsrcSection,szDevKey,RfuInfo[i].m_RsrcName.m_K7_1);
		sprintf(szDevKey,"RFU%d_S6",i);
		IniFile.GetConfigStringValue(szRsrcSection,szDevKey,RfuInfo[i].m_RsrcName.m_S6);
	}

	viDev.GetDevices(strRsrcRfu);
	for (int32_t i = 0;i < MAX_RFU;i ++) {
		for (iterRsrcRfu = strRsrcRfu.begin();iterRsrcRfu != strRsrcRfu.end();iterRsrcRfu ++ ) {
			if (0 == strcmp(RfuInfo[i].m_RsrcName.m_K7_0,iterRsrcRfu->c_str()))
				bK7_0_Active[i] = true;
			if (0 == strcmp(RfuInfo[i].m_RsrcName.m_K7_1,iterRsrcRfu->c_str()))
				bK7_1_Active[i] = true;
			if (0 == strcmp(RfuInfo[i].m_RsrcName.m_S6,iterRsrcRfu->c_str()))
				bS6_Active[i] = true;
		}
		if (bK7_0_Active[i] == true || bK7_1_Active[i] == true || bS6_Active[i] == true) {
			uiRFUNumber ++;
		}
	}
	return 0;
}

int32_t RF_GetRFPortNumber(uint32_t &uiRFPortNumber)
{
	uiRFPortNumber = SP3301_2.GetRfPort() + SP3301_3.GetRfPort();
	return 0;
}

int32_t RF_GetRFUSerialNumber(uint32_t RFUIndex,char *SerialNumber)
{
	switch (RFUIndex) {
// 		case 0 : {SP3301_0.GetSN(SerialNumber);break;}
// 		case 1 : {SP3301_1.GetSN(SerialNumber);break;}
		case 2 : {SP3301_2.GetSN(SerialNumber);break;}
		case 3 : {SP3301_3.GetSN(SerialNumber);break;}
//		case 4 : {SP3301_4.GetSN(SerialNumber);break;}
		default:break;
	}
	return 0;
}

int32_t RF_GetRFUVersion(char *version)
{
	SP3301_2.GetVer(version);
	return 0;
}

//RF控制函数列表

int32_t RF_SetTxState(uint32_t RFIndex,bool State)
{
	return 0;
}

int32_t RF_SetTxPower(uint32_t RFIndex,float Power)
{
	DECLARE_DYNAMIC_SP3301;
	INT_CHECK(pSP3301->RF_SetTxPower(uiRfIdx,Power));
	return 0;
}

int32_t RF_SetTxFrequency(uint32_t RFIndex,uint64_t Freq)
{
	DECLARE_DYNAMIC_SP3301;
	INT_CHECK(pSP3301->RF_SetTxFreq(uiRfIdx,Freq));
	return 0;
}

int32_t RF_SetTxSampleRate(uint32_t RFIndex,SAMPLERATE SampleRate)
{
	return 0;
}

int32_t RF_SetTxDelay(uint32_t RFIndex,double Delay_ns)
{
	return 0;
}

int32_t RF_SetTxSource(uint32_t RFIndex,SOURCE Source)
{
	DECLARE_DYNAMIC_SP3301;
	CSP2401R1A::DASrc TxSrc = CSP2401R1A::Inter_Filter;

	switch (Source) {
		case ARB  : {break;}
		case FPGA : {TxSrc = CSP2401R1A::Inter_Filter;break;}
		case CW   : {TxSrc = CSP2401R1A::Sin_Tone;break;}
		default:break;
	}
	INT_CHECK(pSP3301->RF_SetTxSource(uiRfIdx,TxSrc));
	return 0;
}

int32_t RF_SetSourceFrequency(uint32_t RFIndex,uint64_t Freq)
{
	DECLARE_DYNAMIC_SP3301;
	INT_CHECK(pSP3301->RF_SetSrcFreq(uiRfIdx,Freq));
	return 0;
}

int32_t RF_LoadARBSource(uint32_t RFIndex,char *filename)
{
	DECLARE_DYNAMIC_SP3301;
//	INT_CHECK(pSP3301->RF_LoadArb(uiRfIdx,filename));
	return 0;
}

//BBT ARB
int32_t RF_SetARBEnable(uint32_t RFIndex,bool bState)
{
	return 0;
}

int32_t RF_SetARBTriggerSource(uint32_t RFIndex,ARB_TRIGGERMODE Source)
{
	return 0;
}

int32_t RF_ExcuteARBTrigger(uint32_t RFIndex)
{
	return 0;
}

int32_t RF_SetARBCount(uint32_t RFIndex,int iCnt)
{
	return 0;
}

//BBT
int32_t BB_SetBBTestState(uint32_t RFIndex,bool InputSource)
{
	return 0;
}

int32_t BB_SetTriggerMode(uint32_t RFIndex,BB_TRIGGERMODE TriggerMode)
{
	return 0;
}

int32_t BB_SetTriggerLevel(uint32_t RFIndex,float TriggerLevel)
{
	return 0;
}

int32_t BB_SetTriggerOffset(uint32_t RFIndex,int Offset)
{
	return 0;
}

int32_t BB_SetCaptureLength(uint32_t RFIndex,int iMLength)
{
	return 0;
}

int32_t BB_InitBBTest(uint32_t RFIndex)
{
	return 0;
}

int32_t BB_AbortBBTest(uint32_t RFIndex)
{
	return 0;
}

int32_t BB_GetBBTestProcess(uint32_t RFIndex,PROCESS &Process)
{
	return 0;
}


int32_t RF_SetRxLevel(uint32_t RFIndex,double Level)
{
	DECLARE_DYNAMIC_SP3301;
	INT_CHECK(pSP3301->RF_SetRxLevel(uiRfIdx,Level));
	return 0;
}

int32_t RF_SetRxFrequency(uint32_t RFIndex,uint64_t Freq)
{
	DECLARE_DYNAMIC_SP3301;
	INT_CHECK(pSP3301->RF_SetRxFreq(uiRfIdx,Freq));
	return 0;
}

int32_t RF_SetRxSampleRate(uint32_t RFIndex,SAMPLERATE SampleRate)
{
	return 0;
}

int32_t RF_SetRxDelay(uint32_t RFIndex,double Delay_ns)
{
	return 0;
}

int32_t RF_SetConnector(uint32_t RFIndex,CONNECTOR Connector)
{
	DECLARE_DYNAMIC_SP3301;
	IOMode Mode = OutPut;
	switch (Connector) {
		case ::IO     : {Mode = SP1401::IO;break;}
		case ::OUTPUT : {Mode = SP1401::OutPut;break;}
		case ::LOOP   : {Mode = SP1401::Loop;break;}
		default:break;
	}
	INT_CHECK(pSP3301->RF_SetIOMode(uiRfIdx,Mode));
	return 0;
}

//时钟与触发源函数列表

int32_t RF_SetClockSource(uint32_t RFIndex,RFU_CLOCKSOURCE ClockSource)
{
	return 0;
}

int32_t RF_SetTriggerSource(uint32_t RFIndex,RFU_TRIGGERSOURCE TriggerSource)
{
	return 0;
}

//测量设置函数列表

int32_t RF_SetTriggerMode(uint32_t RFIndex,TRIGGERMODE TriggerMode)
{
	DECLARE_DYNAMIC_SP3301;
	ISP1401::PowerMeasSrcs MeasSrc = ISP1401::PowerMeasFreeRun;
	switch (TriggerMode) {
		case IF		 : {MeasSrc = ISP1401::PowerMeasIFPower;break;}
		case FREERUN : {MeasSrc = ISP1401::PowerMeasFreeRun;break;}
		case MARKER  : {break;}
		default:break;
	}
	INT_CHECK(pSP3301->RF_SetTrigMode(uiRfIdx,MeasSrc));
	return 0;
}

int32_t RF_SetTriggerLevel(uint32_t RFIndex,float TriggerLevel)
{
	DECLARE_DYNAMIC_SP3301;
	INT_CHECK(pSP3301->RF_SetTrigLevel(uiRfIdx,TriggerLevel));
	return 0;
}

int32_t RF_SetTriggerOffset(uint32_t RFIndex,uint32_t Offset)
{
	return 0;
}

int32_t RF_SetCaptureLength(uint32_t RFIndex,uint32_t MLength)
{
	DECLARE_DYNAMIC_SP3301;
	INT_CHECK(pSP3301->RF_SetIQCapLength(uiRfIdx,MLength));
	return 0;
}

int32_t RF_SetIQDataMap(uint32_t RFIndex,uint16_t *pData)
{
	return 0;
}

//功率测量函数列表

int32_t RF_InitPowerMeasure(uint32_t RFIndex)
{
	DECLARE_DYNAMIC_SP3301;
	INT_CHECK(pSP3301->RF_InitPowerMeas(uiRfIdx));
	return 0;
}

int32_t RF_AbortPowerMeasure(uint32_t RFIndex)
{
	DECLARE_DYNAMIC_SP3301;
	INT_CHECK(pSP3301->RF_AbortPowerMeas(uiRfIdx));
	return 0;
}

int32_t RF_GetMeasProcess(uint32_t RFIndex,PROCESS &Process)
{
	DECLARE_DYNAMIC_SP3301;
	Process = IDLE_Driver;
	ISP1401::PowerMeasState MeasState = ISP1401::IDLE;
	INT_CHECK(pSP3301->RF_GetPowerProc(uiRfIdx,MeasState));
	switch (MeasState) {
		case ISP1401::IDLE	  : {Process = IDLE_Driver;break;}
		case ISP1401::WFT	  : {Process = WFTrigger_Driver;break;}
		case ISP1401::TT	  : {Process = Timeout_Driver;break;}
		case ISP1401::RUNNING : {Process = RUNNING_Driver;break;}
		case ISP1401::DONE	  : {Process = DONE_Driver;break;}
		default:break;
	}
	return 0;
}

int32_t RF_GetMeasResult(uint32_t RFIndex,float &Power,float &Crest)
{
	DECLARE_DYNAMIC_SP3301;
	INT_CHECK(pSP3301->RF_GetMeasResult(uiRfIdx,Power,Crest));
	return 0;
}

//自检测量函数列表

int32_t RF_InitIQCapture(uint32_t RFIndex)
{
	return 0;
}

int32_t RF_AbortIQCapture(uint32_t RFIndex)
{
	return 0;
}

int32_t RF_GetIQCaptureProcess(uint32_t RFIndex,PROCESS &Process)
{
	return 0;
}

//系统控制函数列表

int32_t RF_WarningInfo(uint32_t RFIndex,uint32_t &State)
{
	return 0;
}

int32_t RF_GetTemperature(uint32_t RFIndex,double &TxTemperature,double &RxTemperature)
{
	DECLARE_DYNAMIC_SP3301;
	INT_CHECK(pSP3301->RF_GetTemp(uiRfIdx,TxTemperature,RxTemperature));
	return 0;
}