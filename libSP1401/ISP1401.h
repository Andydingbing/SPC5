#ifndef __SP1401_H__
#define __SP1401_H__

#define SN_NULL  0	//missing eeprom
#define SN_WRONG 1	//sn read ok but wrong
#define SN_RIGHT 2	//sn right

#include "sgpmm.h"
#include "ArbReader.h"
#include "DMAMgr.h"
#include "IQBufMgr.h"
#include "ICalFile.h"

#ifdef _MSC_VER
#pragma comment(lib,"libLog.lib")
#endif

class ISP1401
{
public:
	ISP1401();
    ~ISP1401();
public:
	enum HwVer {
		HwError = -1,
		R1A = 0,
		R1B = 1,
		R1C = 2,
		R1D = 3
	};
	enum RFCh {
		Tx = 0,
		Rx = 1
	};
	enum TriSrc	{
		TriSrc_Manual,
		TriSrc_External_Trigger_A,
		TriSrc_External_Trigger_B,
		TriSrc_External_Trigger_C,
		TriSrc_External_Trigger_D
	};
	enum RepMode {
		RepMode_Single_Slot,
		RepMode_Continous
	};
	enum MultiSegTrigSrc {
		MultiSegTrigSrc_Manual
	};
	enum MultiSegRepMode {
		MultiSegRepMode_Auto,
		MultiSegRepMode_Continous,
		MultiSegRepMode_Continous_Seamless
	};
	enum RxIFFilter {									
		_100MHz,
		_160MHz
	};
	enum IQCaptureSrcs {	
		IQCapsrcFreeRun = 0x00,
		IQCapsrcRstMkr  = 0x09,
		IQCapsrcMkr1	= 0x0a,
		IQCapsrcMkr2	= 0x0b,
		IQCapsrcMkr3	= 0x0c,
		IQCapsrcMkr4	= 0x0d,
		IQCapsrcUsrMkr	= 0x0e,
		IQCapsrcETrig	= 0x11,
		IQCapsrcETrig1	= 0x12,
		IQCapsrcETrig2	= 0x13,
		IQCapsrcETrig3	= 0x14,
		IQCapsrcETrig4	= 0x15,
		IQCapsrcIFPwr	= 0x21
	};
	enum PowerMeasSrcs {
		PowerMeasFreeRun = 0x00,
		PowerMeasIFPower = 0x21,
	};
	enum PowerMeasState {
		IDLE	= 0x0,
		WFT		= 0x1,			//waiting for trigger
		TT		= 0x2,			//trigger timeout,manual set to IDLE
		RUNNING = 0x3,			//calculating
		DONE	= 0x4			//manual set to IDLE
	};

#define DECLARE_SP1401(class_name)									\
public:																\
    class_name(uint32_t uiRfIdx,uint32_t uiRfuIdx);					\
	virtual int32_t OpenBoard();									\
	virtual int32_t CloseBoard();									\
	virtual int32_t SetLED(bool bTx,bool bRx);						\
	virtual bool	GetConnect();									\
	virtual int32_t SetTxFreq(uint64_t uiFreq);						\
	virtual int32_t SetTxAtt(double dAtt);							\
	virtual int32_t SetTxModulatorEn(bool bEnable);					\
	virtual int32_t SetPowEn(bool bEnable);							\
	virtual int32_t SetRxFreq(uint64_t uiFreq);						\
    virtual int32_t SetRxLNAAttSw(SP1401::RxLNAAtt LNAAtt);         \
	virtual int32_t SetIOMode(IOMode Mode);							\
	virtual int32_t GetIOMode(IOMode &Mode);

public:
	virtual int32_t  OpenBoard() = 0;
	virtual int32_t  CloseBoard() = 0;
	virtual int32_t  SetLED(bool bTx,bool bRx) = 0;
	virtual bool	 GetConnect() = 0;
	virtual int32_t  SetTxFreq(uint64_t uiFreq) = 0;
	virtual int32_t  SetTxAtt(double dAtt) = 0;
	virtual int32_t  SetTxModulatorEn(bool bEnable) = 0;
	virtual int32_t  SetPowEn(bool bEnable) = 0;
	virtual int32_t  SetRxFreq(uint64_t uiFreq) = 0;
    virtual int32_t  SetRxLNAAttSw(SP1401::RxLNAAtt LNAAtt) = 0;
	virtual int32_t  SetIOMode(IOMode Mode) = 0;
	virtual int32_t  GetIOMode(IOMode &Mode) = 0;
public:
	virtual BW	GetBw();
	virtual void	 SetEnableExpt(bool bEnable);
	virtual uint32_t GetRfIdx();
    virtual uint32_t GetRfuIdx();
    virtual ICalFile* GetCalFile();
    virtual viPCIDev* GetK7();
    virtual int32_t	 Connect(viPCIDev *pK7);
	virtual int32_t  GetK7Ver(uint32_t &uiK7Ver);
	virtual int32_t  GetSN(char *pSN);
	virtual ArbReader *GetArbReader();

	static int32_t IsValidSN(const char *pSN);
	static int32_t GetHwVer(const char *pSN,HwVer &Ver);

	virtual int32_t  SetArbSegment(const ArbSeg_t &Seg);
	virtual int32_t  SetArbSegments(uint16_t uiSeg);
	virtual int32_t  SetArbParam(uint32_t uiAddSamp,uint32_t uiCycl,uint32_t uiRepMo);
	virtual int32_t  SetArbTrigger(bool bReTrigger, bool bAutoStart, uint32_t uiSrc, uint32_t uiTrigDelay);
	virtual int32_t  SetArbManualTrigger();
	virtual int32_t  SetArbMultiSegMode(uint32_t uiMSTS, uint32_t uiMSRM);
	virtual int32_t  SetArbMultiSegNext(uint32_t uiNextSeg);
	virtual int32_t  SetArbFreqOffset(uint32_t uiFreqMHz);
	virtual int32_t  SetArbMkrDelay(uint16_t *pDelays);
	virtual int32_t  SetArbUnperiodMarkerStart();
	virtual int32_t  SetArbUnperiodAddMarker(int iNo,uint32_t uiStart,uint32_t uiKeep);
	virtual int32_t  SetArbUnperiodMarkerEnd(int iNo,int iSeg);
	virtual uint32_t GetArbCurrentSeg();
	virtual uint32_t GetArbCurrentSample();
	virtual uint32_t GetArbCurrentCycle();
	virtual double   GetArbCurrentTimer();
	virtual int32_t  ArbLoad(char *pPath);
	virtual int32_t  ArbStart();
	virtual int32_t  ArbStop();

	virtual int32_t SetNListIQCapTrigOffset(uint32_t uiSamples);
	virtual int32_t SetNListIQCapCapOffset(uint32_t uiSamples);
//	virtual int32_t SetIQCapSrc(IQCaptureSrcs Src,bool bPosedge);
//	virtual int32_t SetIQCapTimeout(uint32_t us);
//	virtual int32_t GetIQCapTimeout(bool &bTimeout);
	virtual int32_t SetIQCapSamples(uint32_t uiSamples);
	virtual int32_t GetIQCapSamples(uint32_t &uiSamples);
	virtual int32_t SetIQCapTrigGap(uint32_t us);
//	virtual int32_t SetIQCapTrigThreshold(double IFPower);
//	virtual int32_t GetIQCapState(IQCaptureState &State);
//	virtual int32_t GetIQCapPeak(double &Power);
//	virtual int32_t GetIQCapPower(double &Power);
	virtual int32_t IQCap();
	virtual int32_t IQCapStart();
	virtual int32_t IQCapAbort();
	virtual int32_t IQDump2Buf(int16_t *pI,int16_t *pQ,uint32_t uiSamples);
	virtual int32_t IQDump2File(char *pPath,uint32_t uiSamples);

	virtual int32_t SetPowerMeasSrc(PowerMeasSrcs Src,bool bPosedge);
	virtual int32_t SetPowerMeasTimeout(uint32_t us);
	virtual int32_t GetPowerMeasTimeout(bool &bTimeout);
	virtual int32_t SetPowerMeasTrigThreshold(double dIFPower);
	virtual int32_t SetPowerMesaTrigGap(uint32_t us);
	virtual int32_t PowerMeasStart();
	virtual int32_t PowerMeasAbort();
	virtual int32_t SetPowerMeasSamples(uint32_t uiSamples);
	virtual int32_t GetPowerMeasSamples(uint32_t &uiSamples);
	virtual int32_t GetPowerMeasState(PowerMeasState &State);
	virtual int32_t GetPowerMeasPeak(double &Power);
	virtual int32_t GetPowerMeasPower(double &Power);
public:
    virtual int32_t SetSN(const char *pRFVer,int32_t iOrdinal);
	virtual int32_t SetSN(char *pSN);
	virtual int32_t WriteEEPROM(uint16_t uiAddr,uint32_t uiLength,char *pBuf);
	virtual int32_t ReadEEPROM(uint16_t uiAddr,uint32_t uiLength,char *pBuf);

	virtual int32_t GetADS5474(int64_t &iAD);
	virtual int32_t GetADS5474Manual(int64_t &iAD);
	virtual int32_t SetRxIFFilterSw(RxIFFilter Filter);
	virtual int32_t GetAD7680(uint32_t &uiDet);
protected:
    ICalFile  *m_pCalFile;
    viPCIDev  *m_pK7;
	ArbReader *m_pArbReader;
protected:
	int32_t  m_UnperiodMarkerCounter;
	float    m_ArbLevelOffset;	
	bool     m_bEnExpt;
};

#endif
