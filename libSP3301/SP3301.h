#ifndef __SP3301_H__
#define __SP3301_H__

#include "libSP1401.h"
#include "libSP2401.h"

#define SP3301_0 CSP3301::Instance(0)
#define SP3301_1 CSP3301::Instance(1)
#define SP3301_2 CSP3301::Instance(2)
#define SP3301_3 CSP3301::Instance(3)
#define SP3301_4 CSP3301::Instance(4)

using namespace std;
using namespace SP1401;

class CSP3301
{
public:
	CSP3301(uint32_t uiRfuIdx);
	CSP3301(CSP3301 &);
	virtual ~CSP3301();
	static CSP3301 &Instance(uint32_t uiRfuIdx);
private:
	CSP3301 & operator = (CSP3301 &);
public:
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
	enum BB_INPUTSOURCE
	{	
		BB_OFF,
		BB_ON
	};
	enum ARB_TRIGGERMODE
	{
		ARB_MANUAL, ARB_CXUTRIGGER, ARB_FREERUN	
	};
public:
	int32_t GetOCXO(int16_t &iValue);
	int32_t Boot();
	int32_t SetLoadBit(bool bLoadK7_0,bool bLoadK7_1,char *pBitPath_0 = NULL,char *pBitPath_1 = NULL);
	int32_t LoadBit();
	int32_t GetRfPort();
	int32_t GetSN(char *pSN);
	int32_t GetVer(char *pVer);
	const char *GetDriverVer();
	string AssK7Name(uint32_t uiK7Idx,uint32_t uiRfuIdx);
	string AssS6Name(uint32_t uiRfuIdx);

	struct Active;
	struct RFUInfo;
	Active  GetActive();
	RFUInfo GetInfo();
public:
	int32_t RF_SetTxState(uint32_t uiRfIdx,bool bState);
	int32_t RF_SetTxPower(uint32_t uiRfIdx,float fPower);
	int32_t RF_SetTxFreq(uint32_t uiRfIdx,uint64_t uiFreq);
	int32_t RF_SetTxBW(uint32_t uiRfIdx,BW Bw);
	int32_t RF_SetTxDelay(uint32_t uiRfIdx,double dDelayns);
	int32_t RF_SetTxSource(uint32_t uiRfIdx,CSP2401R1A::DASrc Source);
	int32_t RF_SetSrcFreq(uint32_t uiRfIdx,uint64_t uiFreq);
	
	int32_t ArbLoad(uint32_t uiRfIdx,char *pPath);
	int32_t ArbLoad(uint32_t uiRfIdx);
	int32_t RF_SetArbEn(uint32_t uiRfIdx,bool bState);
	int32_t RF_SetArbTrigSrc(uint32_t uiRfIdx,ARB_TRIGGERMODE Source);
	int32_t RF_SetArbTrig(uint32_t uiRfIdx);
	int32_t RF_SetArbCount(uint32_t uiRfIdx,int iCnt);

	int32_t RF_SetRxLevel(uint32_t uiRfIdx,double dLevel);
	int32_t RF_SetRxFreq(uint32_t uiRfIdx,uint64_t uiFreq);
	int32_t RF_SetRxBW(uint32_t uiRfIdx,BW Bw);
	int32_t RF_SetRxDelay(uint32_t uiRfIdx,double dDelayns);

	int32_t RF_SetIOMode(uint32_t uiRfIdx,IOMode Mode);
	int32_t RF_GetTemp(uint32_t uiRfIdx,double &dTxTemp,double &dRxTemp);

	int32_t RF_SetTrigSrc(uint32_t uiRfIdx,RFU_TRIGGERSOURCE TrigSrc = RFU_TRIGGER_INTERNAL);
	int32_t RF_SetTrigMode(uint32_t uiRfIdx,ISP1401::PowerMeasSrcs Mode = ISP1401::PowerMeasFreeRun);
	int32_t RF_SetTrigLevel(uint32_t uiRfIdx,float fLevel = 0);
	int32_t RF_SetTrigOffset(uint32_t uiRfIdx,uint32_t uiOffset);
	int32_t RF_InitPowerMeas(uint32_t uiRfIdx);
	int32_t RF_AbortPowerMeas(uint32_t uiRfIdx);
	int32_t RF_GetPowerProc(uint32_t uiRfIdx,ISP1401::PowerMeasState &Proc);
	int32_t RF_GetMeasResult(uint32_t uiRfIdx,float &fPower,float &fCrest);

	int32_t RF_SetIQCapLength(uint32_t uiRfIdx,uint32_t uiLength);
	int32_t RF_SetIQDataMap(uint32_t uiRfIdx,uint16_t *pData);
	int32_t RF_InitIQCap(uint32_t uiRfIdx);
	int32_t RF_AbortIQCap(uint32_t uiRfIdx);
//	int32_t RF_GetIQCapProc(uint32_t uiRfIdx,PROCESS &Process);

	int32_t BB_SetInputSource(uint32_t uiRFUIndex,BB_INPUTSOURCE InputSource);
	int32_t BB_SetTriggerMode(uint32_t uiRFUIndex,BB_TRIGGERMODE TriggerMode);
	int32_t BB_SetTriggerLevel(uint32_t uiRFUIndex,float fTriggerLevel);
	int32_t BB_SetTriggerOffset(uint32_t uiRFUIndex,int Offset);
	int32_t BB_SetCaptureLength(uint32_t uiRFUIndex,int iMLength);
	int32_t BB_InitBBTest(uint32_t uiRFUIndex);
	int32_t BB_AbortBBTest(uint32_t uiRFUIndex);
//	int32_t BB_GetBBTestProcess(uint32_t uiRFUIndex,PROCESS &Process);
public:
	vector<ISP1401 *>	 m_pSP1401;
	vector<CSP1401R1A *> m_pSP1401R1A;
	vector<CSP1401R1C *> m_pSP1401R1C;
	vector<CSP2401R1A *> m_pSP2401;
public:
	viPCIDev m_K7_0;
	viPCIDev m_K7_1;
	viPCIDev m_S6;
public:
    typedef struct RFUInfo {
		typedef struct RsrcName {
			char m_K7_0[32];
			char m_K7_1[32];
			char m_S6[32];
		}RsrcName;
		RsrcName m_RsrcName;
		char	 m_SN[128];
		uint32_t m_uiK7_0_Ver;
		uint32_t m_uiK7_1_Ver;
		uint32_t m_uiS6_Ver;
	public:
        RFUInfo(const char *pK7_0 = "",const char *pK7_1 = "",const char *pS6 = "",const char *pSN = "");
	}RFUInfo;
public:
	RFUInfo m_RFUInfo;
	uint32_t m_uiRFUIndex;
	typedef struct Active{
		bool m_bK7_0;
		bool m_bK7_1;
		bool m_bS6;
		bool m_bSP1401[4];
		Active(){
			m_bK7_0 = false;
			m_bK7_1 = false;
			m_bS6   = false;
            for (uint32_t i = 0;i < ARRAY_SIZE(m_bSP1401);i ++)
				m_bSP1401[i] = false;
		}
	}Active;
private:
	ISP1401::HwVer m_HwVer[MAX_RF];
	IOMode	 m_IOMode[MAX_RF];
	uint64_t m_uiTxFreq[MAX_RF];
	double	 m_dTxPower[MAX_RF];
	uint64_t m_uiRxFreq[MAX_RF];
	double	 m_dReff[MAX_RF];
	bool	 m_bLoadBitK7_0;
	bool	 m_bLoadBitK7_1;
	char	 m_szBitPath_0[128];
	char	 m_szBitPath_1[128];
	Active   m_Active;
};

#endif
