#ifndef __SP1401R1A_H__
#define __SP1401R1A_H__

#include "ISP1401.h"
#include "CalFileR1A.h"

class CSP1401R1A;
typedef int32_t (CSP1401R1A::*SetR1ALOReg)(uint32_t);
typedef int32_t (CSP1401R1A::*DetR1ALOLock)(bool &);

#define DECLARE_R1A_SETLOREG_FUNCTION_MAP()						\
	SetR1ALOReg Set_R1A_LO_Reg[4] = {&CSP1401R1A::SetTxLO1Reg,	\
									 &CSP1401R1A::SetTxLO2Reg,	\
									 &CSP1401R1A::SetRxLO1Reg,	\
									 &CSP1401R1A::SetRxLO2Reg};

using namespace SP1401;

class CSP1401R1A : public ISP1401
{
public:
    virtual ~CSP1401R1A();
	DECLARE_SP1401(CSP1401R1A)
public:
	enum LO {
		Tx_LO_1 = 0,
		Tx_LO_2,
		Rx_LO_1,
		Rx_LO_2
	};
	enum TxBand {
		Low = 0,
		High
	};
	enum TxIOSw {
		Tx_Off = 0,
		Tx_Loop_On,
		Tx_On,
		Tx_IO_On
	};
	enum RxIOSw1 {
		Rx_IO_On_1 = 0,
		Rx_IO_Off_1
	};
	enum RxIOSw2 {
		Rx_On_2 = 0,
		Rx_IO_On_2
	};
	enum RxIOSw {
		Rx_Loop_On = 0,
		Rx_Off,
		Rx_IO_On,
		Rx_On
	};
	typedef struct TxChainStateAll {
		bool	 m_bPower;
		bool	 m_bADL5375;
		TxBand	 m_Band;
		uint64_t m_uiLO1;
		uint64_t m_uiLO2;
		int32_t	 m_iAtt1;
		int32_t	 m_iAtt2;
		int32_t	 m_iAtt3;
		uint32_t m_uiDet;
		double	 m_dTemp;
        SP1401::TxPAAtt m_PAAtt;
	}TxChainStateAll;
	typedef struct RxChainStateAll {
		double	 m_dAtt1;
		int32_t	 m_iAtt2;
		uint64_t m_uiLO1;
		uint64_t m_uiLO2;
		uint32_t m_uiDet;
		double	 m_dTemp;
		IOMode	 m_Mode;
        SP1401::RxLNAAtt m_LNAAtt;
		ISP1401::RxIFFilter  m_IFFilter;
	}RxChainStateAll;
public:
    CalFileR1A* GetCalFile();

	int32_t TxFreq2LO(uint64_t uiFreq,uint64_t &uiLO1,uint64_t &uiLO2,TxBand &Band);
	int32_t TxLO2Freq(uint64_t uiLO1,uint64_t uiLO2,TxBand Band,uint64_t &uiFreq);
	int32_t RxFreq2LO(uint64_t uiFreq,uint64_t &uiLO1,uint64_t &uiLO2);
	int32_t RxLO2Freq(uint64_t uiLO1,uint64_t uiLO2,uint64_t &uiFreq);
	int32_t IOMode2IOSw(IOMode Mode,TxIOSw &TxSw,RxIOSw &RxSw,RxIOSw1 &RxSw1,RxIOSw2 &RxSw2,bool &bTxLED,bool &bRxLED);

	int32_t InitLO();
	int32_t SetLO(LO Lo,uint64_t uiFreq);
	int32_t DetLO(LO Lo,bool &bLock);

	int32_t SetTxBandSw(TxBand Band);
    int32_t SetTxPAAttSw(SP1401::TxPAAtt PAAtt);
	int32_t SetTxIOSw(TxIOSw Mode);
	int32_t SetTxLO1Reg(uint32_t uiRegData);
	int32_t SetTxLO2Reg(uint32_t uiRegData);
	int32_t DetTxLO1Lock(bool &bLock);
	int32_t DetTxLO2Lock(bool &bLock);

	int32_t SetTxAtt1(int32_t iAtt);
	int32_t SetTxAtt2(int32_t iAtt);
	int32_t SetTxAtt3(int32_t iAtt);
	int32_t SetTxAtt(int32_t iAtt1,int32_t iAtt2,int32_t iAtt3);
	int32_t SyncSetTxGain(int32_t iAtt1,int32_t iAtt2,int32_t iAtt3,double dDGain);
	
	int32_t SetRxIOSw1(RxIOSw1 Mode);
	int32_t SetRxIOSw2(RxIOSw2 Mode);
	int32_t SetRxIOSw(RxIOSw Mode);
	int32_t SetRxLO1Reg(uint32_t uiRegData);
	int32_t DetRxLO1Lock(bool &bLock);
	int32_t SetRxLO2Reg(uint32_t uiRegData);
	int32_t DetRxLO2Lock(bool &bLock);
	int32_t SetRxAtt1(double dAtt,double *pActualAtt = NULL);
	int32_t SetRxAtt2(int32_t iAtt,int32_t *pActualAtt = NULL);
	int32_t SetRxAtt(double dAtt1,int32_t iAtt2);
	int32_t SetRxAtt(double dAtt,double *pActualAtt1 = NULL,int32_t *pActualAtt2 = NULL);

	int32_t GetTxTemp(double &dTemp);
	int32_t GetRxTemp(double &dTemp);

	int32_t SetLTC2666(uint16_t uiDACCh,int32_t iDACValue);
	int32_t SetLTC2666(uint16_t uiDACCh,double dVoltage);
	int32_t GetLTC2666(uint16_t uiDACCh,int32_t &iDACValue);

	int32_t SetDetSw(RFCh Ch);

	int32_t SetRxLMH5401N(double dVol);
	int32_t SetRxLMH5401P(double dVol);
	int32_t SetTxLO2XB1007(double dVol);
	int32_t SetRxIF1HMC694VGG1(double dVol);
	int32_t SetRxIF1HMC694VGA(double dVol);
	int32_t SetRxIF1HMC694VGG2(double dVol);
};

#endif
