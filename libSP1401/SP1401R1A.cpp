#include "SP1401R1A.h"
#include "RegDef.h"
#include "CalFileR1A.h"
#include <math.h>
#include <stdlib.h>
#ifdef _WIN64
#include <windows.h>
#elif defined(__unix__) || defined(__linux__)
#include <unistd.h>
#endif

CSP1401R1A::CSP1401R1A(uint32_t uiRfIdx,uint32_t uiRfuIdx)
{
    m_pCalFile = new CalFileR1A(uiRfIdx,uiRfuIdx);
}

CSP1401R1A::~CSP1401R1A()
{
}

int32_t CSP1401R1A::OpenBoard()
{
    INT_CHECK(m_pCalFile->Open());
    INT_CHECK(SetPowEn(true));
    INT_CHECK(SetIOMode(OutPut));
    INT_CHECK(InitLO());
    INT_CHECK(SetTxModulatorEn(true));
    INT_CHECK(SetTxBandSw(High));
    INT_CHECK(SetTxAtt(10,20,20));
    INT_CHECK(SetTxPAAttSw(SP1401::Tx_Att));
    INT_CHECK(SetRxLNAAttSw(SP1401::Rx_Att));
    INT_CHECK(SetRxAtt(20.0,20));
    INT_CHECK(SetRxIFFilterSw(_100MHz));
    INT_CHECK(SetRxLMH5401N(-2.5));				//LT2666 DAC0
    INT_CHECK(SetRxLMH5401P(-2.5));				//LT2666 DAC1
    INT_CHECK(SetTxLO2XB1007(-2.5));			//LT2666 DAC2
    INT_CHECK(SetLTC2666(3,-2.5));				//LT2666 DAC3
    INT_CHECK(SetRxIF1HMC694VGG1(-0.8));		//LT2666 DAC4
    INT_CHECK(SetRxIF1HMC694VGA(-1.25));		//LT2666 DAC5
    INT_CHECK(SetRxIF1HMC694VGG2(-0.8));		//LT2666 DAC6
    INT_CHECK(SetLTC2666(7,-2.5));				//LT2666 DAC7
    INT_CHECK(SetIQCapSamples(40960));
	return 0;
}

int32_t CSP1401R1A::InitLO()
{
	uint32_t uiReg[13];
	for (int i = 0;i < 13;i ++)
		uiReg[i] = 0;

	uint64_t uiFreqVCO = 0;
	uint64_t uiLO[4] = {0,0,0,0};
	TxBand Band = Low;
	uint32_t INT = 0;
	uint32_t FRAC1 = 0;
	uint32_t FRAC2 = 0;
	uint32_t MOD1 = 0;
	uint32_t MOD2 = 0;

	uiReg[3] = 0x00000013;
	uiReg[4] = 0x30008394;
	uiReg[5] = 0x00800025;
	uiReg[7] = 0x12000067;
	uiReg[8] = 0x102d0428;
	uiReg[9] = 0x28323de9;
	uiReg[10] = 0x00c025ba;
	uiReg[11] = 0x0061300b;
	uiReg[12] = 0x4e20041c;

	DECLARE_R1A_SETLOREG_FUNCTION_MAP()

	TxFreq2LO(2400000000,uiLO[0],uiLO[1],Band);
	RxFreq2LO(2400000000,uiLO[2],uiLO[3]);
	
	for (int32_t iLO = (int32_t)Tx_LO_1;iLO <= (int32_t)Rx_LO_2;iLO ++) {
		uiFreqVCO = uiLO[iLO];
		if (uiLO[iLO] > 6800000000) uiFreqVCO = uiLO[iLO] / 2;
		if (uiLO[iLO] < 3400000000) uiFreqVCO = uiLO[iLO] * 2;
		adf5355_freq_formula(uiFreqVCO,100000000,false,false,1,ADF5355_FREQ_SPACE,INT,FRAC1,FRAC2,MOD1,MOD2);
		uiReg[0] = adf5355para2reg0(INT,0,1);
		uiReg[1] = adf5355para2reg1(FRAC1);
		uiReg[2] = adf5355para2reg2(MOD2,FRAC2);
		uiReg[6] = uiLO[iLO] < 3400000000 ? 0x3520a076 : 0x3500a076;
		for (int i = 0;i < 13;i ++)
			(this->*Set_R1A_LO_Reg[iLO])(uiReg[12 - i]);
	}
	SetLO(Rx_LO_2,uiLO[(int32_t)Rx_LO_2]);
	return 0;
}

int32_t CSP1401R1A::CloseBoard()
{
	return 0;
}

int32_t CSP1401R1A::SetLED(bool bTx,bool bRx)
{
    RFU_K7_REG_DECLARE_2(0x1667,0x16e7);
    RFU_K7_REG_2(0x1667,0x16e7).tx_led = bTx;
    RFU_K7_REG_2(0x1667,0x16e7).rx_led = bRx;
    RFU_K7_W_2(0x1667,0x16e7);
	return 0;
}

bool CSP1401R1A::GetConnect()
{
	double dTxTemp = 0.0;
    INT_CHECKB(GetTxTemp(dTxTemp));
	if (dTxTemp <= 0) {
		double dRxTemp = 0.0;
        INT_CHECKB(GetRxTemp(dRxTemp));
        if (dRxTemp <= 0) {
            Log.SetLastError(-2,"missing rfu%u--->rf%u",m_pCalFile->GetRfuIdx(),m_pCalFile->GetRfIdx());
			return false;
		}
	}
	return true;
}

CalFileR1A *CSP1401R1A::GetCalFile()
{
    return (CalFileR1A *)m_pCalFile;
}

int32_t CSP1401R1A::TxFreq2LO(uint64_t uiFreq,uint64_t &uiLO1,uint64_t &uiLO2,TxBand &Band)
{
	if (uiFreq > 0 && uiFreq <= 3100000000) {
		Band = Low;
		uiLO1 = 4100000000;
		uiLO2 = uiFreq + 4100000000;
	}
	else {
		Band = High;
		uiLO1 = uiFreq;
		uiLO2 = 6800000000;
	}
	return 0;
}

int32_t CSP1401R1A::TxLO2Freq(uint64_t uiLO1,uint64_t uiLO2,TxBand Band,uint64_t &uiFreq)
{
	uiFreq = 0;
	if (Low == Band) {
		if (uiLO2 > uiLO1)
			uiFreq = uiLO2 - uiLO1;
		else
			return -1;
	}
	else
		uiFreq = uiLO1;
	return 0;
}

int32_t CSP1401R1A::RxFreq2LO(uint64_t uiFreq,uint64_t &uiLO1,uint64_t &uiLO2)
{
	uiLO1 = uiFreq + 7500000000;
	uiLO2 = 7224000000;
	return 0;
}

int32_t CSP1401R1A::RxLO2Freq(uint64_t uiLO1,uint64_t uiLO2,uint64_t &uiFreq)
{
	uiFreq = 0;
	if (uiLO1 > 7500000000)
		uiFreq = uiLO1 - 7500000000; 
	else
		return -1;
	return 0;
}

int32_t CSP1401R1A::IOMode2IOSw(IOMode Mode,TxIOSw &TxSw,RxIOSw &RxSw,RxIOSw1 &RxSw1,RxIOSw2 &RxSw2,bool &bTxLED,bool &bRxLED)
{
	switch (Mode) {
		case IO : {						//sw6---sw1 11 10 1 0
			TxSw = Tx_IO_On;
			RxSw = Rx_IO_On;
			RxSw1 = Rx_IO_On_1;
			RxSw2 = Rx_IO_On_2;
			bTxLED = false;
			bRxLED = true;
			break;
				  }
		case OutPut : {					//sw6---sw1 10 11 0 0
			TxSw = Tx_On;
			RxSw = Rx_On;
			RxSw1 = Rx_IO_On_1;
			RxSw2 = Rx_On_2;
			bTxLED = true;
			bRxLED = true;
			break;
					  }
		case Loop : {					//sw6---sw1 01 00 1 1
			TxSw = Tx_Loop_On;
			RxSw = Rx_Loop_On;
			RxSw1 = Rx_IO_Off_1;
			RxSw2 = Rx_IO_On_2;
			bTxLED = false;
			bRxLED = false;
			break;
					}
		default :
			break;
	}
	return 0;
}

int32_t CSP1401R1A::SetPowEn(bool bEnable)
{
    RFU_K7_REG_DECLARE_2(0x1660,0x16e0);
    RFU_K7_REG_2(0x1660,0x16e0).pow_en = (bEnable ? 1 : 0);
    RFU_K7_W_2(0x1660,0x16e0);
	return 0;
}

int32_t CSP1401R1A::SetTxModulatorEn(bool bEnable)
{
    RFU_K7_REG_DECLARE_2(0x1662,0x16e2);
    RFU_K7_REG_2(0x1662,0x16e2).mod_en = (bEnable ? 0 : 1);
    RFU_K7_W_2(0x1662,0x16e2);
	return 0;
}

int32_t CSP1401R1A::SetIOMode(IOMode Mode)
{
    RFU_K7_REG_DECLARE_2(0x1666,0x16e6);
	TxIOSw TxSw = Tx_On;
	RxIOSw RxSw = Rx_On;
	RxIOSw1 RxSw1 = Rx_IO_On_1;
	RxIOSw2 RxSw2 = Rx_On_2;
	bool bTxLED = true,bRxLED = true;

	IOMode2IOSw(Mode,TxSw,RxSw,RxSw1,RxSw2,bTxLED,bRxLED);
    RFU_K7_REG_2(0x1666,0x16e6).tx_io_sw = (unsigned)TxSw;
    RFU_K7_REG_2(0x1666,0x16e6).rx_io_sw = (unsigned)RxSw;
    RFU_K7_REG_2(0x1666,0x16e6).rx_io_sw1 = (unsigned)RxSw1;
    RFU_K7_REG_2(0x1666,0x16e6).rx_io_sw2 = (unsigned)RxSw2;
    RFU_K7_W_2(0x1666,0x16e6);

    INT_CHECK(SetLED(bTxLED,bRxLED));
	return 0;
}

int32_t CSP1401R1A::GetIOMode(IOMode &Mode)
{
    RFU_K7_REG_DECLARE_2(0x1666,0x16e6);
    RFU_K7_R_2(0x1666,0x16e6);
    if (0x0000002c == RFU_K7_REG_U32_2(0x1666,0x16e6))
		Mode = OutPut;
    if (0x0000003a == RFU_K7_REG_U32_2(0x1666,0x16e6))
		Mode = IO;
    if (0x00000013 == RFU_K7_REG_U32_2(0x1666,0x16e6))
		Mode = Loop;
	return 0;
}

int32_t CSP1401R1A::SetTxBandSw(TxBand Band)
{
    RFU_K7_REG_DECLARE_2(0x1663,0x16e3);
    RFU_K7_REG_2(0x1663,0x16e3).band_sw = (Low == Band ? 0 : 1);
    RFU_K7_REG_2(0x1663,0x16e3).q = (Low == Band ? 0 : 1);
    RFU_K7_W_2(0x1663,0x16e3);
	return 0;
}

int32_t CSP1401R1A::SetTxPAAttSw(SP1401::TxPAAtt PAAtt)
{
    RFU_K7_REG_DECLARE_2(0x1665,0x16e5);
    RFU_K7_REG_2(0x1665,0x16e5).att_pa_sw = (SP1401::Tx_PA == PAAtt ? 0 : 1);
    RFU_K7_W_2(0x1665,0x16e5);
	return 0;
}

int32_t CSP1401R1A::SetTxIOSw(TxIOSw Mode)
{
    RFU_K7_REG_DECLARE_2(0x1666,0x16e6);
    RFU_K7_R_2(0x1666,0x16e6);
    RFU_K7_REG_2(0x1666,0x16e6).tx_io_sw = (unsigned)Mode;
    RFU_K7_W_2(0x1666,0x16e6);
	return 0;
}

int32_t CSP1401R1A::SetLO(LO Lo,uint64_t uiFreq)
{
	uint32_t reg[13];
	for (int i = 0;i < 13;i ++)
		reg[i] = 0;

	uint64_t uiFreqVCO = uiFreq;
	if (uiFreq > 6800000000) uiFreqVCO = uiFreq / 2;
	if (uiFreq < 3400000000) uiFreqVCO = uiFreq * 2;

	uint32_t INT = 0;
	uint32_t FRAC1 = 0;
	uint32_t FRAC2 = 0;
	uint32_t MOD1 = 0;
	uint32_t MOD2 = 0;

	adf5355_freq_formula(uiFreqVCO,100000000,false,false,1,ADF5355_FREQ_SPACE,INT,FRAC1,FRAC2,MOD1,MOD2);
	reg[0] = adf5355para2reg0(INT,0,1);
	reg[1] = adf5355para2reg1(FRAC1);
	reg[2] = adf5355para2reg2(MOD2,FRAC2);
	reg[4] = 0x30008394;
	reg[6] = uiFreq < 3400000000 ? 0x3520a076 : 0x3500a076;

	DECLARE_R1A_SETLOREG_FUNCTION_MAP()

	(this->*Set_R1A_LO_Reg[Lo])(reg[6]);
	reg[4] = reg[4] | (1 << 4);
	(this->*Set_R1A_LO_Reg[Lo])(reg[4]);
	(this->*Set_R1A_LO_Reg[Lo])(reg[2]);
	(this->*Set_R1A_LO_Reg[Lo])(reg[1]);
	reg[0] = reg[0] & (~(1 << 21));
	(this->*Set_R1A_LO_Reg[Lo])(reg[0]);
	reg[4] = reg[4] & (~(1 << 4));
	(this->*Set_R1A_LO_Reg[Lo])(reg[4]);
#ifdef _WIN64
	Sleep(100);
#else
    usleep(100 * 1000);
#endif
	reg[0] = reg[0] | (1 << 21);
	(this->*Set_R1A_LO_Reg[Lo])(reg[0]);
	return 0;
}

int32_t CSP1401R1A::DetLO(LO Lo,bool &bLock)
{
#ifdef _WIN64
	Sleep(100);
#else
    usleep(100 * 1000);
#endif
	DetR1ALOLock Det_LO_Lock[4] = {&CSP1401R1A::DetTxLO1Lock,
								   &CSP1401R1A::DetTxLO2Lock,
								   &CSP1401R1A::DetRxLO1Lock,
								   &CSP1401R1A::DetRxLO2Lock};
	return (this->*Det_LO_Lock[Lo])(bLock);
}

int32_t CSP1401R1A::SetTxLO1Reg(uint32_t uiRegData)
{	
    RFU_K7_REG_DECLARE_2(0x160d,0x168d);
    RFU_K7_REG_DECLARE_2(0x160e,0x168e);
    RFU_K7_REG_2(0x160e,0x168e).data = uiRegData;
    RFU_K7_WAIT_IDLE_2(0x160d,0x168d,0,1000);
    RFU_K7_W_2(0x160e,0x168e);
    RFU_K7_WAIT_IDLE_2(0x160d,0x168d,0,1000);
    RFU_K7_OP_2(0x160d,0x168d);
    RFU_K7_WAIT_IDLE_2(0x160d,0x168d,0,1000);
	return 0;
}

int32_t CSP1401R1A::SetTxLO2Reg(uint32_t uiRegData)
{
    RFU_K7_REG_DECLARE_2(0x1609,0x1689);
    RFU_K7_REG_DECLARE_2(0x160a,0x168a);
    RFU_K7_REG_2(0x160a,0x168a).data = uiRegData;
    RFU_K7_WAIT_IDLE_2(0x1609,0x1689,0,1000);
    RFU_K7_W_2(0x160a,0x168a);
    RFU_K7_WAIT_IDLE_2(0x1609,0x1689,0,1000);
    RFU_K7_OP_2(0x1609,0x1689);
    RFU_K7_WAIT_IDLE_2(0x1609,0x1689,0,1000);
	return 0;
}

int32_t CSP1401R1A::DetTxLO1Lock(bool &bLock)
{
    RFU_K7_REG_DECLARE_2(0x160f,0x168f);
    RFU_K7_R_2(0x160f,0x168f);
    bLock = (RFU_K7_REG_2(0x160f,0x168f).muxout == 0 ? false : true);
	return 0;
}

int32_t CSP1401R1A::DetTxLO2Lock(bool &bLock)
{
    RFU_K7_REG_DECLARE_2(0x160b,0x168b);
    RFU_K7_R_2(0x160b,0x168b);
    bLock = (RFU_K7_REG_2(0x160b,0x168b).muxout == 0 ? false : true);
	return 0;
}

int32_t CSP1401R1A::SetTxFreq(uint64_t uiFreq)
{
	uint64_t uiLO1 = 0,uiLO2 = 0;
	TxBand Band = Low;
	TxFreq2LO(uiFreq,uiLO1,uiLO2,Band);
    INT_CHECK(SetTxBandSw(Band));
    INT_CHECK(SetLO(Tx_LO_1,uiLO1));
    INT_CHECK(SetLO(Tx_LO_2,uiLO2));
	return 0;
}

int32_t CSP1401R1A::SetTxAtt1(int32_t iAtt)
{
	iAtt = abs(iAtt);
	iAtt = uint32_t(iAtt / 10.0) * 10;
	unsigned c1 = (iAtt == 30 || iAtt == 10) ? 0 : 1;
	unsigned c2 = (iAtt == 30 || iAtt == 20) ? 0 : 1;

    RFU_K7_REG_DECLARE_2(0x1664,0x16e4);
    RFU_K7_R_2(0x1664,0x16e4);
    RFU_K7_REG_2(0x1664,0x16e4).tx_att1_c1 = c1;
    RFU_K7_REG_2(0x1664,0x16e4).tx_att1_c2 = c2;
    RFU_K7_W_2(0x1664,0x16e4);
	return 0;
}

int32_t CSP1401R1A::SetTxAtt2(int32_t iAtt)
{
	iAtt = abs(iAtt);
	iAtt = uint32_t(iAtt / 10.0) * 10;
	unsigned c1 = (iAtt == 30 || iAtt == 10) ? 0 : 1;
	unsigned c2 = (iAtt == 30 || iAtt == 20) ? 0 : 1;

    RFU_K7_REG_DECLARE_2(0x1664,0x16e4);
    RFU_K7_R_2(0x1664,0x16e4);
    RFU_K7_REG_2(0x1664,0x16e4).tx_att2_c1 = c1;
    RFU_K7_REG_2(0x1664,0x16e4).tx_att2_c2 = c2;
    RFU_K7_W_2(0x1664,0x16e4);
	return 0;
}

int32_t CSP1401R1A::SetTxAtt3(int32_t iAtt)
{
	iAtt = abs(iAtt);
	iAtt = uint32_t(iAtt / 10.0) * 10;
	unsigned c1 = (iAtt == 30 || iAtt == 10) ? 0 : 1;
	unsigned c2 = (iAtt == 30 || iAtt == 20) ? 0 : 1;

    RFU_K7_REG_DECLARE_2(0x1664,0x16e4);
    RFU_K7_R_2(0x1664,0x16e4);
    RFU_K7_REG_2(0x1664,0x16e4).tx_att3_c1 = c1;
    RFU_K7_REG_2(0x1664,0x16e4).tx_att3_c2 = c2;
    RFU_K7_W_2(0x1664,0x16e4);
	return 0;
}

int32_t CSP1401R1A::SetTxAtt(int32_t iAtt1,int32_t iAtt2,int32_t iAtt3)
{
    INT_CHECK(SetTxAtt1(iAtt1));
    INT_CHECK(SetTxAtt2(iAtt2));
    INT_CHECK(SetTxAtt3(iAtt3));
	return 0;
}

int32_t CSP1401R1A::SetTxAtt(double dAtt)
{
	int32_t att1 = dAtt >= 30.0 ? 30 : ((int32_t)(dAtt / 10.0)) * 10;
	int32_t att2 = (dAtt - att1) >= 30 ? 30 : ((int32_t)((dAtt - att1) / 10.0)) * 10;
	int32_t att3 = (dAtt - att1 - att2) >= 30 ? 30 : ((int32_t)((dAtt - att1 - att2) / 10.0)) * 10;
    INT_CHECK(SetTxAtt(att1,att2,att3));
	return 0;
}

int32_t CSP1401R1A::SyncSetTxGain(int32_t iAtt1,int32_t iAtt2,int32_t iAtt3,double dDGain)
{
    RFU_K7_REG_DECLARE_2(0x0094,0x2094);
    RFU_K7_REG_DECLARE_2(0x00e6,0x20e6);

    RFU_K7_REG_2(0x00e6,0x20e6).op_0 = 0;
    RFU_K7_REG_2(0x00e6,0x20e6).op_1 = 0;
    RFU_K7_W_2(0x00e6,0x20e6);
    RFU_K7_REG_2(0x00e6,0x20e6).op_1 = 1;
    RFU_K7_W_2(0x00e6,0x20e6);

    INT_CHECK(SetTxAtt(iAtt1,iAtt2,iAtt3));
    RFU_K7_REG_2(0x0094,0x2094).i = (uint16_t)(dDGain * 512.0);
    RFU_K7_REG_2(0x0094,0x2094).q = (uint16_t)(dDGain * 512.0);
    RFU_K7_W_2(0x0094,0x2094);
	
    RFU_K7_REG_2(0x00e6,0x20e6).op_0 = 1;
    RFU_K7_W_2(0x00e6,0x20e6);

    RFU_K7_REG_2(0x00e6,0x20e6).op_0 = 0;
    RFU_K7_REG_2(0x00e6,0x20e6).op_1 = 0;
    RFU_K7_W_2(0x00e6,0x20e6);
	return 0;
}

int32_t CSP1401R1A::SetRxLNAAttSw(SP1401::RxLNAAtt LNAAtt)
{
    RFU_K7_REG_DECLARE_2(0x166a,0x16ea);
    RFU_K7_REG_2(0x166a,0x16ea).lna_att_sw = (LNAAtt == SP1401::Rx_Att ? 0 : 1);
    RFU_K7_W_2(0x166a,0x16ea);
	return 0;
}

int32_t CSP1401R1A::SetRxFreq(uint64_t uiFreq)
{
	uint64_t uiLO1 = 0,uiLO2 = 0;
	RxFreq2LO(uiFreq,uiLO1,uiLO2);
    INT_CHECK(SetLO(Rx_LO_1,uiLO1));
	return 0;
}

int32_t CSP1401R1A::SetRxIOSw1(RxIOSw1 Mode)
{
    RFU_K7_REG_DECLARE_2(0x1666,0x16e6);
    RFU_K7_R_2(0x1666,0x16e6);
    RFU_K7_REG_2(0x1666,0x16e6).rx_io_sw1 = (unsigned)Mode;
    RFU_K7_W_2(0x1666,0x16e6);
	return 0;
}

int32_t CSP1401R1A::SetRxIOSw2(RxIOSw2 Mode)
{
    RFU_K7_REG_DECLARE_2(0x1666,0x16e6);
    RFU_K7_R_2(0x1666,0x16e6);
    RFU_K7_REG_2(0x1666,0x16e6).rx_io_sw2 = (unsigned)Mode;
    RFU_K7_W_2(0x1666,0x16e6);
	return 0;
}

int32_t CSP1401R1A::SetRxIOSw(RxIOSw Mode)
{
    RFU_K7_REG_DECLARE_2(0x1666,0x16e6);
    RFU_K7_R_2(0x1666,0x16e6);
    RFU_K7_REG_2(0x1666,0x16e6).rx_io_sw = (unsigned)Mode;
    RFU_K7_W_2(0x1666,0x16e6);
	return 0;
}

int32_t CSP1401R1A::SetRxLO1Reg(uint32_t uiRegData)
{
    RFU_K7_REG_DECLARE_2(0x1601,0x1681);
    RFU_K7_REG_DECLARE_2(0x1602,0x1682);
    RFU_K7_REG_2(0x1602,0x1682).data = uiRegData;
    RFU_K7_WAIT_IDLE_2(0x1601,0x1681,0,1000);
    RFU_K7_W_2(0x1602,0x1682);
    RFU_K7_WAIT_IDLE_2(0x1601,0x1681,0,1000);
    RFU_K7_OP_2(0x1601,0x1681);
    RFU_K7_WAIT_IDLE_2(0x1601,0x1681,0,1000);
	return 0;
}

int32_t CSP1401R1A::DetRxLO1Lock(bool &bLock)
{
    RFU_K7_REG_DECLARE_2(0x1603,0x1683);
    RFU_K7_R_2(0x1603,0x1683);
    bLock = (RFU_K7_REG_2(0x1603,0x1683).muxout == 0 ? false : true);
	return 0;
}

int32_t CSP1401R1A::SetRxLO2Reg(uint32_t uiRegData)
{
    RFU_K7_REG_DECLARE_2(0x1605,0x1685);
    RFU_K7_REG_DECLARE_2(0x1606,0x1686);
    RFU_K7_REG_2(0x1606,0x1686).data = uiRegData;
    RFU_K7_WAIT_IDLE_2(0x1605,0x1685,0,1000);
    RFU_K7_W_2(0x1606,0x1686);
    RFU_K7_WAIT_IDLE_2(0x1605,0x1685,0,1000);
    RFU_K7_OP_2(0x1605,0x1685);
    RFU_K7_WAIT_IDLE_2(0x1605,0x1685,0,1000);
	return 0;
}

int32_t CSP1401R1A::DetRxLO2Lock(bool &bLock)
{
    RFU_K7_REG_DECLARE_2(0x1607,0x1687);
    RFU_K7_R_2(0x1607,0x1687);
    bLock = (RFU_K7_REG_2(0x1607,0x1687).muxout == 0 ? false : true);
	return 0;
}

int32_t CSP1401R1A::SetRxAtt1(double dAtt,double *pActualAtt)
{
	dAtt = abs(dAtt);
	dAtt = ((int32_t)(dAtt * 10) / 5) * 5 / 10.0;	//×ª»»Îª0.5µÄÕûÊý±¶
	if (pActualAtt)
		*pActualAtt = dAtt;
	int32_t D0_5 = 63 - (int32_t)(63 * dAtt / 31.5);

    RFU_K7_REG_DECLARE_2(0x1641,0x16c1);
    RFU_K7_REG_DECLARE_2(0x1642,0x16c2);
    RFU_K7_REG_2(0x1642,0x16c2).data_word = (unsigned)D0_5;
    RFU_K7_WAIT_IDLE_2(0x1641,0x16c1,0,1000);
    RFU_K7_W_2(0x1642,0x16c2);
    RFU_K7_WAIT_IDLE_2(0x1641,0x16c1,0,1000);
    RFU_K7_OP_2(0x1641,0x16c1);
    RFU_K7_WAIT_IDLE_2(0x1641,0x16c1,0,1000);
	return 0;
}

int32_t CSP1401R1A::SetRxAtt2(int32_t iAtt,int32_t *pActualAtt)
{
	iAtt = abs(iAtt);
	iAtt = uint32_t(iAtt / 10.0) * 10;
	if (pActualAtt)
		*pActualAtt = iAtt;
	unsigned c1 = (iAtt == 30 || iAtt == 10) ? 0 : 1;
	unsigned c2 = (iAtt == 30 || iAtt == 20) ? 0 : 1;

    RFU_K7_REG_DECLARE_2(0x1669,0x16e9);
    RFU_K7_REG_2(0x1669,0x16e9).rx_att2_c1 = c1;
    RFU_K7_REG_2(0x1669,0x16e9).rx_att2_c2 = c2;
    RFU_K7_W_2(0x1669,0x16e9);
	return 0;
}

int32_t CSP1401R1A::SetRxAtt(double dAtt1,int32_t iAtt2)
{
    INT_CHECK(SetRxAtt1(dAtt1));
    INT_CHECK(SetRxAtt2(iAtt2));
	return 0;
}

int32_t CSP1401R1A::SetRxAtt(double dAtt,double *pActualAtt1,int32_t *pActualAtt2)
{
	dAtt = abs(dAtt);
	dAtt = dAtt > 60.0 ? 60.0 : dAtt;

	int32_t att_2 = dAtt > 30.0 ? ((int32_t)((dAtt - 30) / 10) + 1) * 10 : 0;
	att_2 = att_2 > 30 ? 30 : att_2;
    INT_CHECK(SetRxAtt2(att_2,pActualAtt2));

	double att_1 = dAtt - *pActualAtt2;
	att_1 = att_1 > 30 ? 30 : att_1;
    INT_CHECK(SetRxAtt1(att_1,pActualAtt1));

	return 0;
}

int32_t CSP1401R1A::GetTxTemp(double &dTemp)
{
	unsigned uiTempHex = 0;

    RFU_K7_REG_DECLARE_2(0x1618,0x1698);
    RFU_K7_R_2(0x1618,0x1698);
    uiTempHex = RFU_K7_REG_2(0x1618,0x1698).tmp;

	if ((uiTempHex & 0x300) == 0x200)
		dTemp = (signed)((uiTempHex & 0xff) - 0x00) / 4.0 + (-128.0);
	if ((uiTempHex & 0x300) == 0x300)
		dTemp = (signed)((uiTempHex & 0xff) - 0xfc) / 4.0 + (-1.0);
	if ((uiTempHex & 0x300) == 0x000)
		dTemp = (signed)((uiTempHex & 0xff) - 0x00) / 4.0 + (0.0);
	if ((uiTempHex & 0x300) == 0x100)
		dTemp = (signed)((uiTempHex & 0xff) - 0x90) / 4.0 + (100.0);
	return 0;
}

int32_t CSP1401R1A::GetRxTemp(double &dTemp)
{
	unsigned uiTempHex = 0;

    RFU_K7_REG_DECLARE_2(0x1615,0x1695);
    RFU_K7_R_2(0x1615,0x1695);
    uiTempHex = RFU_K7_REG_2(0x1615,0x1695).tmp;

	if ((uiTempHex & 0x300) == 0x200)
		dTemp = (signed)((uiTempHex & 0xff) - 0x00) / 4.0 + (-128.0);
	if ((uiTempHex & 0x300) == 0x300)
		dTemp = (signed)((uiTempHex & 0xff) - 0xfc) / 4.0 + (-1.0);
	if ((uiTempHex & 0x300) == 0x000)
		dTemp = (signed)((uiTempHex & 0xff) - 0x00) / 4.0 + (0.0);
	if ((uiTempHex & 0x300) == 0x100)
		dTemp = (signed)((uiTempHex & 0xff) - 0x90) / 4.0 + (100.0);
	return 0;
}

int32_t CSP1401R1A::SetLTC2666(uint16_t uiDACCh,int32_t iDACValue)
{
    RFU_K7_REG_DECLARE_2(0x163d,0x16bd);
    RFU_K7_REG_DECLARE_2(0x163f,0x16bf);
    RFU_K7_REG_2(0x163f,0x16bf).dont_care		= 0;
    RFU_K7_REG_2(0x163f,0x16bf).command_word	= 3;	//b0011 write to n,update n
    RFU_K7_REG_2(0x163f,0x16bf).address_word	= uiDACCh;
    RFU_K7_REG_2(0x163f,0x16bf).data_word		= (iDACValue << 4);
    RFU_K7_WAIT_IDLE_2(0x163d,0x16bd,0,1000);
    RFU_K7_W_2(0x163f,0x16bf);
    RFU_K7_WAIT_IDLE_2(0x163d,0x16bd,0,1000);
    RFU_K7_OP_2(0x163d,0x16bd);
    RFU_K7_WAIT_IDLE_2(0x163d,0x16bd,0,1000);
	return 0;
}

int32_t CSP1401R1A::SetLTC2666(uint16_t uiDACCh,double dVoltage)
{
	double dVMin = -5.0;
	double dVMax = 5.0;
	int32_t iDACValue = (int32_t)(4095.0 * ((dVoltage - dVMin) / (dVMax - dVMin)));
    INT_CHECK(SetLTC2666(uiDACCh,iDACValue));
	return 0;
}

int32_t CSP1401R1A::GetLTC2666(uint16_t uiDACCh,int32_t &iDACValue)
{
    RFU_K7_REG_DECLARE_2(0x163d,0x16bd);
    RFU_K7_REG_DECLARE_2(0x1640,0x16c0);
    RFU_K7_WAIT_IDLE_2(0x163d,0x16bd,0,1000);
    RFU_K7_R_2(0x1640,0x16c0);
    RFU_K7_WAIT_IDLE_2(0x163d,0x16bd,0,1000);
    iDACValue = (int32_t)RFU_K7_REG_2(0x1640,0x16c0).data_word;
	return 0;
}

int32_t CSP1401R1A::SetDetSw(RFCh Ch)
{
    RFU_K7_REG_DECLARE_2(0x1661,0x16e1);
    RFU_K7_REG_2(0x1661,0x16e1).det_sw = (Ch == Tx ? 0 : 1);
    RFU_K7_W_2(0x1661,0x16e1);
	return 0;
}

int32_t CSP1401R1A::SetRxLMH5401N(double dVol)
{
    INT_CHECK(SetLTC2666(0,dVol));
	return 0;
}

int32_t CSP1401R1A::SetRxLMH5401P(double dVol)
{
    INT_CHECK(SetLTC2666(1,dVol));
	return 0;
}

int32_t CSP1401R1A::SetTxLO2XB1007(double dVol)
{
    INT_CHECK(SetLTC2666(2,dVol));
	return 0;
}

int32_t CSP1401R1A::SetRxIF1HMC694VGG1(double dVol)
{
    INT_CHECK(SetLTC2666(4,dVol));
	return 0;
}

int32_t CSP1401R1A::SetRxIF1HMC694VGA(double dVol)
{
    INT_CHECK(SetLTC2666(5,dVol));
	return 0;
}

int32_t CSP1401R1A::SetRxIF1HMC694VGG2(double dVol)
{
    INT_CHECK(SetLTC2666(6,dVol));
	return 0;
}
