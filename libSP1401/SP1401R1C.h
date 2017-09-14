#ifndef __SP1401R1C_H__
#define __SP1401R1C_H__

#include "ISP1401.h"
#include "CalFileR1C.h"

class CSP1401R1C;
typedef int32_t (CSP1401R1C::*SetR1CLOReg)(uint32_t);
typedef int32_t (CSP1401R1C::*DetR1CLOLock)(bool &);
typedef int32_t (CSP1401R1C::*pGetTemp)(double *);

#define DECLARE_R1C_SETLOREG_FUNCTION_MAP()						\
	SetR1CLOReg Set_R1C_LO_Reg[4] = {&CSP1401R1C::SetTxLO1Reg,	\
									 &CSP1401R1C::SetTxLO2Reg,	\
									 &CSP1401R1C::SetTxLO3Reg,	\
									 &CSP1401R1C::SetRxLO1Reg};

#define DECLARE_R1C_GETTEMP_FUNCTION_MAP()					\
	pGetTemp tGetR1CTemp[8] = {&CSP1401R1C::GetTemp0RxLO2,	\
							   &CSP1401R1C::GetTemp1RxLNA,	\
							   &CSP1401R1C::GetTemp2RxLO1,	\
							   &CSP1401R1C::GetTemp3RxPA1,	\
							   &CSP1401R1C::GetTemp4TxLO2,	\
							   &CSP1401R1C::GetTemp5TxLO1,	\
							   &CSP1401R1C::GetTemp6TxPA4,	\
							   &CSP1401R1C::GetTemp7TxPA3};

using namespace SP1401;

//r1c just fail
//actually this class has been modified for R1D,so some manual operation must config if work with r1c
//e.g. all lo configuration
class CSP1401R1C : public ISP1401
{
public:
    virtual ~CSP1401R1C();
	DECLARE_SP1401(CSP1401R1C)
public:
	enum LO {
		Tx_LO_1 = 0,
		Tx_LO_2,
		Tx_LO_3,
		Rx_LO_1
	};
	enum TxFilter {
		LPF = 0,
		BPF
	};
	enum TxIOSw1 {
		Tx_Off_1 = 0,
		Tx_Loop_On,
		Tx_On_1,
		Tx_IO_On
	};
	enum TxIOSw2 {
		Tx_Off_2 = 0,
		Tx_On_2
	};
	enum LoopSw {
		Loop_On = 0,
		Loop_Off
	};
	enum RxIOSw1 {
		Rx_IO_On_1 = 0,
		Rx_IO_Off_1
	};
	enum RxIOSw2 {
		Rx_On_2 = 0,
		Rx_IO_On_2
	};
	enum RxIOSw3 {
		Rx_Loop_On = 0,
		Rx_Off_3,
		Rx_IO_On_3,
		Rx_On_3
	};
	typedef struct GPIOA {
		unsigned pd_rxif12_txif2 : 1;
		unsigned pd_rxcomb_other : 1;
		unsigned op_green_led	 : 1;
		unsigned op_red_led		 : 1;
		unsigned tx_io_sw2		 : 1;
		unsigned tx_io_sw1		 : 2;
		unsigned io_red_led		 : 1;
		unsigned rsv			 : 24;
	public:
		GPIOA();
	}GPIOA;
	typedef struct GPIOB {
		unsigned io_green_led : 1;
		unsigned rx_io_sw1	  : 1;
		unsigned rx_io_sw2	  : 1;
		unsigned rx_io_sw3	  : 2;
		unsigned loop_sw	  : 1;
		unsigned rx_att019_sw : 1;
		unsigned rx_lnaatt_sw : 1;
		unsigned rsv		  : 24;
	public:
		GPIOB();
	}GPIOB;
	typedef struct TxChainStateAll {
		bool	 m_bPower;
		bool	 m_bMod;
		uint64_t m_uiLO1;
		uint64_t m_uiLO2;
		TxFilter m_Filter;
		uint64_t m_uiLO3;
		double	 m_dAtt0;
		double	 m_dAtt1;
		double	 m_dAtt2;
		double	 m_dAtt3;
		uint32_t m_uiDetIF2;
		double	 m_dTemp4;
		double	 m_dTemp5;
		double	 m_dTemp6;
		double	 m_dTemp7;
	}TxChainStateAll;
	typedef struct RxChainStateAll {
		double	 m_dAtt1;
		double	 m_dAtt2;
		uint64_t m_uiLO1;
		uint64_t m_uiLO2;
		double	 m_dAtt3;
		uint32_t m_uiDetComb;
		uint32_t m_uiDetIF1;
		uint32_t m_uiDetIF2;
		double	 m_dTemp0;
		double	 m_dTemp1;
		double	 m_dTemp2;
		double	 m_dTemp3;
		IOMode   m_Mode;
        SP1401::RxLNAAtt m_LNAAtt;
        SP1401::RxAtt019 m_Att019;
		ISP1401::RxIFFilter  m_IFFilter;
	}RxChainStateAll;
public:
    CalFileR1C* GetCalFile();

	int32_t TxFreq2LO(uint64_t uiFreq,uint64_t &uiLO1,uint64_t &uiLO2,uint64_t &uiLO3,TxFilter &Filter);
	int32_t TxLO2Freq(uint64_t uiLO1,uint64_t uiLO2,uint64_t uiLO3,uint64_t &uiFreq);
	int32_t RxFreq2LO(uint64_t uiFreq,uint64_t &uiLO1,uint64_t &uiLO2);
	int32_t RxLO2Freq(uint64_t uiLO1,uint64_t uiLO2,uint64_t &uiFreq);
	int32_t IOMode2IOSw(IOMode Mode,TxIOSw1 &TxSw1,TxIOSw2 &TxSw2,RxIOSw1 &RxSw1,RxIOSw2 &RxSw2,RxIOSw3 &RxSw3,LoopSw &LpSw,bool &bTxLED,bool &bRxLED);
	int32_t IOMode2LED(IOMode Mode,bool &bOPGreen,bool &bOPRed,bool &bIOGreen,bool &bIORed);

	int32_t InitADF5355();
	int32_t SetADF5355(LO Lo,uint64_t uiFreq);
	int32_t InitHMC1197();
	int32_t SetHMC1197(uint64_t uiFreq);
	int32_t DetLO(LO Lo,bool &bLock);

	int32_t SetTxLO1Reg(uint32_t uiRegData);
	int32_t GetTxLO1Reg(uint32_t uiReg,uint32_t &uiRegData);
	int32_t SetTxLO1RegOpenMode(uint32_t uiRegData);
	int32_t GetTxLO1RegOpenMode(uint32_t uiReg,uint32_t &uiRegData);
	int32_t SetTxLO1RegHMCMode(uint32_t uiRegData);
	int32_t GetTxLO1RegHMCMode(uint32_t uiReg,uint32_t &uiRegData);

	int32_t SetTxLO2Reg(uint32_t uiRegData);
	int32_t SetTxLO3Reg(uint32_t uiRegData);
	int32_t DetTxLO1Lock(bool &bLock);
	int32_t DetTxLO2Lock(bool &bLock);
	int32_t DetTxLO3Lock(bool &bLock);

	int32_t SetTxFilterSw(TxFilter Filter1,TxFilter Filter2);
	int32_t SetTxIOSw1(TxIOSw1 Mode);
	int32_t SetTxIOSw2(TxIOSw2 Mode);
	int32_t SetLoopSw(LoopSw Mode);
	int32_t SetTxAtt0(double dAtt);
	int32_t SetTxAtt1(double dAtt);
	int32_t SetTxAtt2(double dAtt);
	int32_t SetTxAtt3(double dAtt);
	int32_t SetTxAtt(double dAtt0,double dAtt1,double dAtt2,double dAtt3);
	int32_t SyncSetTxGain(double dAtt0,double dAtt1,double dAtt2,double dAtt3,double dDGain);

	int32_t SetRxIOSw1(RxIOSw1 Mode);
	int32_t SetRxIOSw2(RxIOSw2 Mode);
	int32_t SetRxIOSw3(RxIOSw3 Mode);
	int32_t SetRxLO1Reg(uint32_t uiRegData);
	int32_t DetRxLO1Lock(bool &bLock);
    int32_t SetRxAtt019Sw(SP1401::RxAtt019 Att);
	int32_t SetRxAtt1(double dAtt);
	int32_t SetRxAtt2(double dAtt);
	int32_t SetRxAtt3(double dAtt);
	int32_t SetRxAtt(double dAtt1,double dAtt2,double dAtt3);

	int32_t GetTemp0RxLO2(double *pTemp);
	int32_t GetTemp1RxLNA(double *pTemp);
	int32_t GetTemp2RxLO1(double *pTemp);
	int32_t GetTemp3RxPA1(double *pTemp);
	int32_t GetTemp4TxLO2(double *pTemp);
	int32_t GetTemp5TxLO1(double *pTemp);
	int32_t GetTemp6TxPA4(double *pTemp);
	int32_t GetTemp7TxPA3(double *pTemp);

	int32_t GetAD7680TxIF2(uint32_t &uiDet);
	int32_t GetAD7680RxComb(uint32_t &uiDet);
	int32_t GetAD7680RxIF1(uint32_t &uiDet);
	int32_t GetAD7680RxIF2(uint32_t &uiDet);

	int32_t SetMCP23S17Reg(uint32_t uiAddr,uint32_t uiData);
	int32_t GetMCP23S17Reg(uint32_t uiAddr,uint32_t &uiData);
	int32_t InitMCP23S17();

	int32_t GetMCP3208(uint32_t uiAddr,uint32_t &uiData);
};

#endif
