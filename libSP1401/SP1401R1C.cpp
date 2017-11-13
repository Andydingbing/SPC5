#include "SP1401R1C.h"
#include "RegDef.h"
#include <math.h>
#include <windows.h>

typedef int32_t (CSP1401R1C::*SetLOReg)(uint32_t);
typedef int32_t (CSP1401R1C::*DetLOLock)(bool &);

CSP1401R1C::GPIOA::GPIOA()
{
	pd_rxif12_txif2 = 0;
	pd_rxcomb_other = 0;
	op_green_led	= 0;
	op_red_led		= 0;
	tx_io_sw1		= 0;
	io_red_led		= 0;
	tx_io_sw2		= 0;
	rsv			    = 0;
}

CSP1401R1C::GPIOB::GPIOB()
{
	io_green_led = 0;
	rx_io_sw1	 = 0;
	rx_io_sw2	 = 0;
	rx_io_sw3	 = 0;
	loop_sw	     = 0;
	rx_att019_sw = 0;
	rx_lnaatt_sw = 0;
	rsv		     = 0;
}

CSP1401R1C::CSP1401R1C(uint32_t uiRfIdx,uint32_t uiRfuIdx)
{
	m_pCalFile = new CalFileR1C(uiRfIdx,uiRfuIdx);
}

CSP1401R1C::~CSP1401R1C()
{
}

int32_t CSP1401R1C::OpenBoard()
{
    INT_CHECK(m_pCalFile->Open());
    INT_CHECK(SetPowEn(true));
    INT_CHECK(InitADF5355());
    INT_CHECK(InitHMC1197());
    INT_CHECK(InitMCP23S17());
    INT_CHECK(SetIOMode(OutPut));
    INT_CHECK(SetTxModulatorEn(true));
    INT_CHECK(SetIQCapSamples(40960));
	return 0;
}

int32_t CSP1401R1C::InitADF5355()
{
	uint32_t uiReg[13];
	for (int i = 0;i < 13;i ++)
		uiReg[i] = 0;

	uint64_t uiFreqVCO = 4000000000;
	uint64_t uiLO[4] = {0,0,0,0};
	TxFilter Filter = LPF;
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

	DECLARE_R1C_SETLOREG_FUNCTION_MAP()

	TxFreq2LO(2400000000,uiLO[0],uiLO[1],uiLO[2],Filter);
	RxFreq2LO(2400000000,uiLO[3],uiLO[1]);

	for (int32_t iLO = (int32_t)Tx_LO_2;iLO <= (int32_t)Rx_LO_1;iLO ++) {
		uiFreqVCO = uiLO[iLO];
		if (uiLO[iLO] > 6800000000) uiFreqVCO = uiLO[iLO] / 2;
		if (uiLO[iLO] < 3400000000) uiFreqVCO = uiLO[iLO] * 2;
		adf5355_freq_formula(uiFreqVCO,100000000,false,false,1,ADF5355_FREQ_SPACE,INT,FRAC1,FRAC2,MOD1,MOD2);
		uiReg[0] = adf5355para2reg0(INT,0,1);
		uiReg[1] = adf5355para2reg1(FRAC1);
		uiReg[2] = adf5355para2reg2(MOD2,FRAC2);
		uiReg[6] = uiLO[iLO] < 3400000000 ? 0x3520a076 : 0x3500a076;
		for (int i = 0;i < 13;i ++)
			(this->*Set_R1C_LO_Reg[iLO])(uiReg[12 - i]);
	}
	SetADF5355(Tx_LO_2,uiLO[(int32_t)Tx_LO_2]);
	return 0;
}

int32_t CSP1401R1C::InitHMC1197()
{
	uint32_t uiReg[32];
	for (int32_t i = 0;i < 32;i ++)
		uiReg[i] = 0;

	DECLARE_R1C_SETLOREG_FUNCTION_MAP()

	uiReg[0x00] = 0x00000000;	(this->*Set_R1C_LO_Reg[Tx_LO_1])(uiReg[0x00]);
	uiReg[0x01] = 0x00000208;	(this->*Set_R1C_LO_Reg[Tx_LO_1])(uiReg[0x01]);
	uiReg[0x02] = 0x00000110;	(this->*Set_R1C_LO_Reg[Tx_LO_1])(uiReg[0x02]);
//	uiReg[0x03] = 0x00002718;	(this->*Set_R1C_LO_Reg[Tx_LO_1])(uiReg[0x03]);	//r1c	975MHz * 4
	uiReg[0x03] = 0x00002518;	(this->*Set_R1C_LO_Reg[Tx_LO_1])(uiReg[0x03]);	//r1d	925MHz * 4
	uiReg[0x04] = 0x00000020;	(this->*Set_R1C_LO_Reg[Tx_LO_1])(uiReg[0x04]);
	uiReg[0x05] = 0x00000028;	(this->*Set_R1C_LO_Reg[Tx_LO_1])(uiReg[0x05]);
	uiReg[0x06] = 0x03078830;	(this->*Set_R1C_LO_Reg[Tx_LO_1])(uiReg[0x06]);
	uiReg[0x07] = 0x20094438;	(this->*Set_R1C_LO_Reg[Tx_LO_1])(uiReg[0x07]);
	uiReg[0x08] = 0x41bfff40;	(this->*Set_R1C_LO_Reg[Tx_LO_1])(uiReg[0x08]);
	uiReg[0x09] = 0x00326448;	(this->*Set_R1C_LO_Reg[Tx_LO_1])(uiReg[0x09]);
	uiReg[0x0a] = 0x00204650;	(this->*Set_R1C_LO_Reg[Tx_LO_1])(uiReg[0x0a]);
	uiReg[0x0b] = 0x07806158;	(this->*Set_R1C_LO_Reg[Tx_LO_1])(uiReg[0x0b]);
	uiReg[0x0c] = 0x00000060;	(this->*Set_R1C_LO_Reg[Tx_LO_1])(uiReg[0x0c]);
	uiReg[0x0f] = 0x00008178;	(this->*Set_R1C_LO_Reg[Tx_LO_1])(uiReg[0x0f]);
	uiReg[0x14] = 0x000220a0;	(this->*Set_R1C_LO_Reg[Tx_LO_1])(uiReg[0x14]);
	uiReg[0x15] = 0x0f48a0a8;	(this->*Set_R1C_LO_Reg[Tx_LO_1])(uiReg[0x15]);
	uiReg[0x16] = 0x0006c4b0;	(this->*Set_R1C_LO_Reg[Tx_LO_1])(uiReg[0x16]);
	uiReg[0x17] = 0x00012bb8;	(this->*Set_R1C_LO_Reg[Tx_LO_1])(uiReg[0x17]);
	uiReg[0x18] = 0x0054c3c0;	(this->*Set_R1C_LO_Reg[Tx_LO_1])(uiReg[0x18]);
	uiReg[0x19] = 0x000ab2c8;	(this->*Set_R1C_LO_Reg[Tx_LO_1])(uiReg[0x19]);
	uiReg[0x1a] = 0xb29d0bd0;	(this->*Set_R1C_LO_Reg[Tx_LO_1])(uiReg[0x1a]);

	return 0;
}

int32_t CSP1401R1C::CloseBoard()
{
	return 0;
}

int32_t CSP1401R1C::SetLED(bool bTx,bool bRx)
{
	uint32_t uiGPA = 0,uiGPB = 0;
    INT_CHECK(GetMCP23S17Reg(0x12,uiGPA));
    INT_CHECK(GetMCP23S17Reg(0x13,uiGPB));
	GPIOA GPA = *(GPIOA *)&uiGPA;
	GPIOB GPB = *(GPIOB *)&uiGPB;
	GPA.op_green_led = bTx ? 0 : 1;
	GPA.op_red_led   = bTx ? 1 : 0;
	GPB.io_green_led = bRx ? 0 : 1;
	GPA.io_red_led   = bRx ? 1 : 0;
	uiGPA = *(uint32_t *)&GPA;
	uiGPB = *(uint32_t *)&GPB;
    INT_CHECK(SetMCP23S17Reg(0x12,uiGPA));
    INT_CHECK(SetMCP23S17Reg(0x13,uiGPB));
	return 0;
}

bool CSP1401R1C::GetConnect()
{
	DECLARE_R1C_GETTEMP_FUNCTION_MAP()

	double dTemp = 0.0;
	for (int32_t i = 0;i < 8;i ++) {
        INT_CHECKB((this->*tGetR1CTemp[i])(&dTemp));
		if (dTemp > 0.0 && dTemp < 100.0)
			return true;
    }
    Log.SetLastError(-2,"missing rfu%u--->rf%u",m_pCalFile->GetRfuIdx(),m_pCalFile->GetRfIdx());
	return false;
}

CalFileR1C* CSP1401R1C::GetCalFile()
{
    return (CalFileR1C *)m_pCalFile;
}

int32_t CSP1401R1C::TxFreq2LO(uint64_t uiFreq,uint64_t &uiLO1,uint64_t &uiLO2,uint64_t &uiLO3,TxFilter &Filter)
{
// 	uiLO1 = 975000000;	//r1c
// 	uiLO2 = 6524000000;	//r1c
	uiLO1 = 925000000;	//r1d
	uiLO2 = 6574000000;	//r1d
	uiLO3 = uiFreq + uiLO1 + uiLO2;
	Filter = (uiFreq >= 3100000000 ? BPF : LPF);
	return 0;
}

int32_t CSP1401R1C::TxLO2Freq(uint64_t uiLO1,uint64_t uiLO2,uint64_t uiLO3,uint64_t &uiFreq)
{
	uiFreq = 0;
//	if (975000000 == uiLO1 && 6524000000 == uiLO2) {	//r1c
	if (925000000 == uiLO1 && 6574000000 == uiLO2) {	//r1d
		if (uiLO3 > 7499000000 && uiLO3 <= 13499000000)
			uiFreq = uiLO3 - 7499000000;
		else
			return -1;
	}
	else
		return -1;

	

	return 0;
}

int32_t CSP1401R1C::RxFreq2LO(uint64_t uiFreq,uint64_t &uiLO1,uint64_t &uiLO2)
{
//	uiLO2 = 6524000000;	//r1c
	uiLO2 = 6574000000;	//r1d
	uiLO1 = uiFreq + uiLO2 + 276000000;
	return 0;
}

int32_t CSP1401R1C::RxLO2Freq(uint64_t uiLO1,uint64_t uiLO2,uint64_t &uiFreq)
{
	uiFreq = 0;
//	if (uiLO1 > 6800000000)	//r1c
	if (uiLO1 > 6850000000)	//r1d
		uiFreq = uiLO1 - uiLO2 - 276000000; 
	else
		return -1;
	return 0;
}

int32_t CSP1401R1C::IOMode2IOSw(IOMode Mode,TxIOSw1 &TxSw1,TxIOSw2 &TxSw2,RxIOSw1 &RxSw1,RxIOSw2 &RxSw2,RxIOSw3 &RxSw3,LoopSw &LpSw,bool &bTxLED,bool &bRxLED)
{
	switch (Mode) {
		case IO : {
			TxSw1 = Tx_IO_On;
			TxSw2 = Tx_Off_2;
			RxSw1 = Rx_IO_On_1;
			RxSw2 = Rx_IO_On_2;
			RxSw3 = Rx_IO_On_3;
			LpSw  = Loop_Off;
			bTxLED = false;
			bRxLED = true;
			break;
				  }
		case OutPut : {
			TxSw1 = Tx_On_1;
			TxSw2 = Tx_On_2;
			RxSw1 = Rx_IO_On_1;
			RxSw2 = Rx_On_2;
			RxSw3 = Rx_On_3;
			LpSw  = Loop_Off;
			bTxLED = true;
			bRxLED = true;
			break;
					  }
		case Loop : {
			TxSw1 = Tx_Loop_On;
			TxSw2 = Tx_Off_2;
			RxSw1 = Rx_IO_Off_1;
			RxSw2 = Rx_IO_On_2;
			RxSw3 = Rx_Loop_On;
			LpSw  = Loop_On;
			bTxLED = false;
			bRxLED = false;
			break;
					}
		default :
			break;
	}
	return 0;
}

int32_t CSP1401R1C::IOMode2LED(IOMode Mode,bool &bOPGreen,bool &bOPRed,bool &bIOGreen,bool &bIORed)
{
	switch (Mode) {
		case IO : {
			bOPGreen = true;
			bOPRed   = false;
			bIOGreen = false;
			bIORed   = true;
			break;
				  }
		case OutPut : {
			bOPGreen = false;
			bOPRed   = true;
			bIOGreen = false;
			bIORed   = true;
			break;
					  }
		case Loop : {
			bOPGreen = true;
			bOPRed   = false;
			bIOGreen = true;
			bIORed   = false;
			break;
					}
		default :
			break;
	}
	return 0;
}

int32_t CSP1401R1C::SetPowEn(bool bEnable)
{
    RFU_K7_REG_DECLARE_2(0x1829,0x2829);
    RFU_K7_REG_2(0x1829,0x2829).pow_en = (bEnable ? 1 : 0);
    RFU_K7_W_2(0x1829,0x2829);
	return 0;
}

int32_t CSP1401R1C::SetTxModulatorEn(bool bEnable)
{
    RFU_K7_REG_DECLARE_2(0x1804,0x2804);

    RFU_K7_WAIT_IDLE_2(0x1804,0x2804,0,1000);
    RFU_K7_R_2(0x1804,0x2804);
    RFU_K7_WAIT_IDLE_2(0x1804,0x2804,0,1000);
    RFU_K7_REG_2(0x1804,0x2804).mod = bEnable ? 1 : 0;
    RFU_K7_W_2(0x1804,0x2804);
    RFU_K7_WAIT_IDLE_2(0x1804,0x2804,0,1000);
	return 0;
}

int32_t CSP1401R1C::SetIOMode(IOMode Mode)
{
	TxIOSw1 TxSw1 = Tx_IO_On;
	TxIOSw2 TxSw2 = Tx_Off_2;
	RxIOSw1 RxSw1 = Rx_IO_On_1;
	RxIOSw2 RxSw2 = Rx_IO_On_2;
	RxIOSw3 RxSw3 = Rx_IO_On_3;
	LoopSw  LpSw  = Loop_Off;
	bool bTxLED = false,bRxLED = true;
	bool bOPGreen = false,bOPRed = true,bIOGreen = false,bIORed = true;
	uint32_t uiDataGPA = 0,uiDataGPB = 0;

	IOMode2IOSw(Mode,TxSw1,TxSw2,RxSw1,RxSw2,RxSw3,LpSw,bTxLED,bRxLED);
	IOMode2LED(Mode,bOPGreen,bOPRed,bIOGreen,bIORed);
    INT_CHECK(GetMCP23S17Reg(0x12,uiDataGPA));
    INT_CHECK(GetMCP23S17Reg(0x13,uiDataGPB));
	GPIOA GPA = *(GPIOA *)&uiDataGPA;
	GPIOB GPB = *(GPIOB *)&uiDataGPB;
	GPA.tx_io_sw1 = (unsigned)TxSw1;
	GPA.tx_io_sw2 = (unsigned)TxSw2;
	GPB.rx_io_sw1 = (unsigned)RxSw1;
	GPB.rx_io_sw2 = (unsigned)RxSw2;
	GPB.rx_io_sw3 = (unsigned)RxSw3;
	GPB.loop_sw   = (unsigned)LpSw;
	GPA.op_green_led = bOPGreen ? 1 : 0;
	GPA.op_red_led	 = bOPRed   ? 1 : 0;
	GPB.io_green_led = bIOGreen ? 1 : 0;
	GPA.io_red_led   = bIORed   ? 1 : 0;
	uiDataGPA = *(uint32_t *)&GPA;
	uiDataGPB = *(uint32_t *)&GPB;
    INT_CHECK(SetMCP23S17Reg(0x12,uiDataGPA));
    INT_CHECK(SetMCP23S17Reg(0x13,uiDataGPB));
	return 0;
}

int32_t CSP1401R1C::GetIOMode(IOMode &Mode)
{
	TxIOSw1 TxSw1 = Tx_IO_On;
	TxIOSw2 TxSw2 = Tx_Off_2;
	RxIOSw1 RxSw1 = Rx_IO_On_1;
	RxIOSw2 RxSw2 = Rx_IO_On_2;
	RxIOSw3 RxSw3 = Rx_IO_On_3;
	LoopSw  LpSw  = Loop_Off;
	bool bTxLED = false,bRxLED = true;
	uint32_t uiDataGPA = 0,uiDataGPB = 0;

    INT_CHECK(GetMCP23S17Reg(0x12,uiDataGPA));
    INT_CHECK(GetMCP23S17Reg(0x13,uiDataGPB));

	if (0xd8 == uiDataGPA && 0x38 == uiDataGPB)
		Mode = OutPut;
	if (0xe4 == uiDataGPA && 0x34 == uiDataGPB)
		Mode = IO;
	if (0x24 == uiDataGPA && 0x07 == uiDataGPB)
		Mode = Loop;
	return 0;
}

int32_t CSP1401R1C::SetADF5355(LO Lo,uint64_t uiFreq)
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

	DECLARE_R1C_SETLOREG_FUNCTION_MAP()

	(this->*Set_R1C_LO_Reg[Lo])(reg[6]);
	reg[4] = reg[4] | (1 << 4);
	(this->*Set_R1C_LO_Reg[Lo])(reg[4]);
	(this->*Set_R1C_LO_Reg[Lo])(reg[2]);
	(this->*Set_R1C_LO_Reg[Lo])(reg[1]);
	reg[0] = reg[0] & (~(1 << 21));
	(this->*Set_R1C_LO_Reg[Lo])(reg[0]);
	reg[4] = reg[4] & (~(1 << 4));
	(this->*Set_R1C_LO_Reg[Lo])(reg[4]);
	Sleep(100);
	reg[0] = reg[0] | (1 << 21);
	(this->*Set_R1C_LO_Reg[Lo])(reg[0]);
	return 0;
}

int32_t CSP1401R1C::SetHMC1197(uint64_t uiFreq)
{
	return 0;
}

int32_t CSP1401R1C::DetLO(LO Lo,bool &bLock)
{
	Sleep(100);
	DetLOLock Det_LO_Lock[4] = {&CSP1401R1C::DetTxLO1Lock,
								&CSP1401R1C::DetTxLO2Lock,
								&CSP1401R1C::DetTxLO3Lock,
								&CSP1401R1C::DetRxLO1Lock};
	return (this->*Det_LO_Lock[Lo])(bLock);
}

int32_t CSP1401R1C::SetTxLO1Reg(uint32_t uiRegData)
{
	return SetTxLO1RegOpenMode(uiRegData);
}

int32_t CSP1401R1C::GetTxLO1Reg(uint32_t uiReg,uint32_t &uiRegData)
{
	return GetTxLO1RegOpenMode(uiReg,uiRegData);
}

int32_t CSP1401R1C::SetTxLO1RegOpenMode(uint32_t uiRegData)
{	
    RFU_K7_REG_DECLARE_2(0x1804,0x2804);
    RFU_K7_REG_DECLARE_2(0x1805,0x2805);

    RFU_K7_WAIT_IDLE_2(0x1804,0x2804,0,1000);
    RFU_K7_R_2(0x1804,0x2804);
    RFU_K7_WAIT_IDLE_2(0x1804,0x2804,0,1000);
    RFU_K7_REG_2(0x1804,0x2804).wr = 0;
    RFU_K7_W_2(0x1804,0x2804);
    RFU_K7_WAIT_IDLE_2(0x1804,0x2804,0,1000);
    RFU_K7_REG_2(0x1805,0x2805).data = uiRegData;
    RFU_K7_W_2(0x1805,0x2805);
    RFU_K7_WAIT_IDLE_2(0x1804,0x2804,0,1000);
    RFU_K7_OP_2(0x1804,0x2804);
    RFU_K7_WAIT_IDLE_2(0x1804,0x2804,0,1000);
	return 0;
}

int32_t CSP1401R1C::GetTxLO1RegOpenMode(uint32_t uiReg,uint32_t &uiRegData)
{
    RFU_K7_REG_DECLARE_2(0x1804,0x2804);
    RFU_K7_REG_DECLARE_2(0x1805,0x2805);
    RFU_K7_REG_DECLARE_2(0x1807,0x2807);

    RFU_K7_WAIT_IDLE_2(0x1804,0x2804,0,1000);
    RFU_K7_R_2(0x1804,0x2804);
    RFU_K7_WAIT_IDLE_2(0x1804,0x2804,0,1000);
    RFU_K7_REG_2(0x1804,0x2804).wr = 1;
    RFU_K7_W_2(0x1804,0x2804);
    RFU_K7_WAIT_IDLE_2(0x1804,0x2804,0,1000);
    RFU_K7_REG_2(0x1805,0x2805).data = (0 | (uiReg << 8));
    RFU_K7_W_2(0x1805,0x2805);
    RFU_K7_WAIT_IDLE_2(0x1804,0x2804,0,1000);
    RFU_K7_OP_2(0x1804,0x2804);
    RFU_K7_WAIT_IDLE_2(0x1804,0x2804,0,1000);
    RFU_K7_R_2(0x1807,0x2807);
    RFU_K7_WAIT_IDLE_2(0x1804,0x2804,0,1000);
    uiRegData = (uint32_t)(RFU_K7_REG_2(0x1807,0x2807).data) >> 8;
	return 0;
}

int32_t CSP1401R1C::SetTxLO1RegHMCMode(uint32_t uiRegData)
{
    RFU_K7_REG_DECLARE_2(0x1836,0x2836);
    RFU_K7_REG_DECLARE_2(0x1837,0x2837);

    RFU_K7_REG_2(0x1836,0x2836).wr = 0;
    RFU_K7_REG_2(0x1836,0x2836).addr = (uiRegData & 0x000000f8) >> 3;
    RFU_K7_W_2(0x1836,0x2836);
    RFU_K7_REG_2(0x1837,0x2837).data = (uiRegData & 0xffffff00) >> 8;
    RFU_K7_W_2(0x1837,0x2837);

    RFU_K7_WAIT_IDLE_2(0x1836,0x2836,0,1000);
    RFU_K7_OP_2(0x1836,0x2836);
    RFU_K7_WAIT_IDLE_2(0x1836,0x2836,0,1000);
	return 0;
}

int32_t CSP1401R1C::GetTxLO1RegHMCMode(uint32_t uiReg,uint32_t &uiRegData)
{
    RFU_K7_REG_DECLARE_2(0x1836,0x2836);
    RFU_K7_REG_DECLARE_2(0x1838,0x2838);

    RFU_K7_REG_2(0x1836,0x2836).wr = 1;
    RFU_K7_REG_2(0x1836,0x2836).addr = uiReg;
    RFU_K7_W_2(0x1836,0x2836);

    RFU_K7_WAIT_IDLE_2(0x1836,0x2836,0,1000);
    RFU_K7_OP_2(0x1836,0x2836);
    RFU_K7_WAIT_IDLE_2(0x1836,0x2836,0,1000);

    RFU_K7_R_2(0x1838,0x2838);
    uiRegData = (uint32_t)(RFU_K7_REG_2(0x1838,0x2838).data);
	return 0;
}

int32_t CSP1401R1C::DetTxLO1Lock(bool &bLock)
{
    RFU_K7_REG_DECLARE_2(0x160f,0x168f);
    RFU_K7_R_2(0x160f,0x168f);
    bLock = (RFU_K7_REG_2(0x160f,0x168f).muxout == 0 ? false : true);
	return 0;
}

int32_t CSP1401R1C::SetTxLO2Reg(uint32_t uiRegData)
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

int32_t CSP1401R1C::DetTxLO2Lock(bool &bLock)
{
    RFU_K7_REG_DECLARE_2(0x160b,0x168b);
    RFU_K7_R_2(0x160b,0x168b);
    bLock = (RFU_K7_REG_2(0x160b,0x168b).muxout == 0 ? false : true);
	return 0;
}

int32_t CSP1401R1C::SetTxLO3Reg(uint32_t uiRegData)
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

int32_t CSP1401R1C::DetTxLO3Lock(bool &bLock)
{
    RFU_K7_REG_DECLARE_2(0x160f,0x168f);
    RFU_K7_R_2(0x160f,0x168f);
    bLock = (RFU_K7_REG_2(0x160f,0x168f).muxout == 0 ? false : true);
	return 0;
}

int32_t CSP1401R1C::SetTxFreq(uint64_t uiFreq)
{
	uint64_t uiLO1 = 0,uiLO2 = 0,uiLO3 = 0;
	TxFilter Filter = LPF;
	TxFreq2LO(uiFreq,uiLO1,uiLO2,uiLO3,Filter);
    INT_CHECK(SetTxFilterSw(Filter,Filter));
    INT_CHECK(SetADF5355(Tx_LO_3,uiLO3));
	return 0;
}

int32_t CSP1401R1C::SetTxFilterSw(TxFilter Filter1,TxFilter Filter2)
{
    RFU_K7_REG_DECLARE_2(0x1828,0x2828);
    RFU_K7_REG_DECLARE_2(0x1835,0x2835);
    RFU_K7_REG_2(0x1828,0x2828).filter = (LPF == Filter1 ? 0 : 1);
    RFU_K7_REG_2(0x1835,0x2835).filter = (LPF == Filter2 ? 0 : 1);
    RFU_K7_W_2(0x1828,0x2828);
    RFU_K7_W_2(0x1835,0x2835);
	return 0;
}

int32_t CSP1401R1C::SetTxIOSw1(TxIOSw1 Mode)
{
	uint32_t uiGPA = 0;
    INT_CHECK(GetMCP23S17Reg(0x12,uiGPA));
	GPIOA GPIO = *(GPIOA *)&uiGPA;
	GPIO.tx_io_sw1 = (unsigned)Mode;
	uiGPA = *(uint32_t *)&GPIO;
    INT_CHECK(SetMCP23S17Reg(0x12,uiGPA));
	return 0;
}

int32_t CSP1401R1C::SetTxIOSw2(TxIOSw2 Mode)
{
	uint32_t uiGPA = 0;
    INT_CHECK(GetMCP23S17Reg(0x12,uiGPA));
	GPIOA GPIO = *(GPIOA *)&uiGPA;
	GPIO.tx_io_sw2 = (unsigned)Mode;
	uiGPA = *(uint32_t *)&GPIO;
    INT_CHECK(SetMCP23S17Reg(0x12,uiGPA));
	return 0;
}

int32_t CSP1401R1C::SetLoopSw(LoopSw Mode)
{
	uint32_t uiGPB = 0;
    INT_CHECK(GetMCP23S17Reg(0x13,uiGPB));
	GPIOB GPIO = *(GPIOB *)&uiGPB;
	GPIO.loop_sw = (unsigned)Mode;
	uiGPB = *(uint32_t *)&GPIO;
    INT_CHECK(SetMCP23S17Reg(0x13,uiGPB));
	return 0;
}

int32_t CSP1401R1C::SetTxAtt0(double dAtt)
{
	int32_t D0_5 = 63 - (int32_t)(63 * dAtt / 31.5);

    RFU_K7_REG_DECLARE_2(0x1810,0x2810);
    RFU_K7_REG_DECLARE_2(0x1811,0x2811);
    RFU_K7_REG_2(0x1811,0x2811).data_word = (unsigned)D0_5;
    RFU_K7_WAIT_IDLE_2(0x1810,0x2810,0,1000);
    RFU_K7_W_2(0x1811,0x2811);
    RFU_K7_WAIT_IDLE_2(0x1810,0x2810,0,1000);
    RFU_K7_OP_2(0x1810,0x2810);
    RFU_K7_WAIT_IDLE_2(0x1810,0x2810,0,1000);
	return 0;
}

int32_t CSP1401R1C::SetTxAtt1(double dAtt)
{
	int32_t D0_5 = 63 - (int32_t)(63 * dAtt / 31.5);

    RFU_K7_REG_DECLARE_2(0x1814,0x2814);
    RFU_K7_REG_DECLARE_2(0x1815,0x2815);
    RFU_K7_REG_2(0x1815,0x2815).data_word = (unsigned)D0_5;
    RFU_K7_WAIT_IDLE_2(0x1814,0x2814,0,1000);
    RFU_K7_W_2(0x1815,0x2815);
    RFU_K7_WAIT_IDLE_2(0x1814,0x2814,0,1000);
    RFU_K7_OP_2(0x1814,0x2814);
    RFU_K7_WAIT_IDLE_2(0x1814,0x2814,0,1000);
	return 0;
}

int32_t CSP1401R1C::SetTxAtt2(double dAtt)
{
	int32_t D0_5 = 63 - (int32_t)(63 * dAtt / 31.5);

    RFU_K7_REG_DECLARE_2(0x1818,0x2818);
    RFU_K7_REG_DECLARE_2(0x1819,0x2819);
    RFU_K7_REG_2(0x1819,0x2819).data_word = (unsigned)D0_5;
    RFU_K7_WAIT_IDLE_2(0x1818,0x2818,0,1000);
    RFU_K7_W_2(0x1819,0x2819);
    RFU_K7_WAIT_IDLE_2(0x1818,0x2818,0,1000);
    RFU_K7_OP_2(0x1818,0x2818);
    RFU_K7_WAIT_IDLE_2(0x1818,0x2818,0,1000);
	return 0;
}

int32_t CSP1401R1C::SetTxAtt3(double dAtt)
{
	int32_t D0_5 = 63 - (int32_t)(63 * dAtt / 31.5);

    RFU_K7_REG_DECLARE_2(0x181c,0x281c);
    RFU_K7_REG_DECLARE_2(0x181d,0x281d);
    RFU_K7_REG_2(0x181d,0x281d).data_word = (unsigned)D0_5;
    RFU_K7_WAIT_IDLE_2(0x181c,0x281c,0,1000);
    RFU_K7_W_2(0x181d,0x281d);
    RFU_K7_WAIT_IDLE_2(0x181c,0x281c,0,1000);
    RFU_K7_OP_2(0x181c,0x281c);
    RFU_K7_WAIT_IDLE_2(0x181c,0x281c,0,1000);
	return 0;
}

int32_t CSP1401R1C::SetTxAtt(double dAtt0,double dAtt1,double dAtt2,double dAtt3)
{
    INT_CHECK(SetTxAtt0(dAtt0));
    INT_CHECK(SetTxAtt1(dAtt1));
    INT_CHECK(SetTxAtt2(dAtt2));
    INT_CHECK(SetTxAtt3(dAtt3));
	return 0;
}

int32_t CSP1401R1C::SetTxAtt(double dAtt)
{
// 	int32_t att1 = dAtt >= 30.0 ? 30 : ((int32_t)(dAtt / 10.0)) * 10;
// 	int32_t att2 = (dAtt - att1) >= 30 ? 30 : ((int32_t)((dAtt - att1) / 10.0)) * 10;
// 	int32_t att3 = (dAtt - att1 - att2) >= 30 ? 30 : ((int32_t)((dAtt - att1 - att2) / 10.0)) * 10;
// 	STACHK(SetTxAtt(att1,att2,att3));
	return 0;
}

int32_t CSP1401R1C::SyncSetTxGain(double dAtt0,double dAtt1,double dAtt2,double dAtt3,double dDGain)
{
    RFU_K7_REG_DECLARE_2(0x0094,0x2094);
    RFU_K7_REG_DECLARE_2(0x00e6,0x20e6);

    RFU_K7_REG_2(0x00e6,0x20e6).op_0 = 0;
    RFU_K7_REG_2(0x00e6,0x20e6).op_1 = 0;
    RFU_K7_W_2(0x00e6,0x20e6);
    RFU_K7_REG_2(0x00e6,0x20e6).op_1 = 1;
    RFU_K7_W_2(0x00e6,0x20e6);

    INT_CHECK(SetTxAtt(dAtt0,dAtt1,dAtt2,dAtt3));
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

int32_t CSP1401R1C::SetRxFreq(uint64_t uiFreq)
{
	uint64_t uiLO1 = 0,uiLO2 = 0;
	RxFreq2LO(uiFreq,uiLO1,uiLO2);
    INT_CHECK(SetADF5355(Rx_LO_1,uiLO1));
	return 0;
}

int32_t CSP1401R1C::SetRxIOSw1(RxIOSw1 Mode)
{
	uint32_t uiGPB = 0;
    INT_CHECK(GetMCP23S17Reg(0x13,uiGPB));
	GPIOB GPIO = *(GPIOB *)&uiGPB;
	GPIO.rx_io_sw1 = (unsigned)Mode;
	uiGPB = *(uint32_t *)&GPIO;
    INT_CHECK(SetMCP23S17Reg(0x13,uiGPB));
	return 0;
}

int32_t CSP1401R1C::SetRxIOSw2(RxIOSw2 Mode)
{
	uint32_t uiGPB = 0;
    INT_CHECK(GetMCP23S17Reg(0x13,uiGPB));
	GPIOB GPIO = *(GPIOB *)&uiGPB;
	GPIO.rx_io_sw2 = (unsigned)Mode;
	uiGPB = *(uint32_t *)&GPIO;
    INT_CHECK(SetMCP23S17Reg(0x13,uiGPB));
	return 0;
}

int32_t CSP1401R1C::SetRxIOSw3(RxIOSw3 Mode)
{
	uint32_t uiGPB = 0;
    INT_CHECK(GetMCP23S17Reg(0x13,uiGPB));
	GPIOB GPIO = *(GPIOB *)&uiGPB;
	GPIO.rx_io_sw3 = (unsigned)Mode;
	uiGPB = *(uint32_t *)&GPIO;
    INT_CHECK(SetMCP23S17Reg(0x13,uiGPB));
	return 0;
}

int32_t CSP1401R1C::SetRxLO1Reg(uint32_t uiRegData)
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

int32_t CSP1401R1C::DetRxLO1Lock(bool &bLock)
{
    RFU_K7_REG_DECLARE_2(0x1603,0x1683);
    RFU_K7_R_2(0x1603,0x1683);
    bLock = (RFU_K7_REG_2(0x1603,0x1683).muxout == 0 ? false : true);
	return 0;
}

int32_t CSP1401R1C::SetRxAtt019Sw(SP1401::RxAtt019 Att)
{
	uint32_t uiGPB = 0;
    INT_CHECK(GetMCP23S17Reg(0x13,uiGPB));
	GPIOB GPIO = *(GPIOB*)&uiGPB;
	GPIO.rx_att019_sw = (unsigned)Att;
	uiGPB = *(uint32_t *)&GPIO;
    INT_CHECK(SetMCP23S17Reg(0x13,uiGPB));
	return 0;
}

int32_t CSP1401R1C::SetRxLNAAttSw(SP1401::RxLNAAtt LNAAtt)
{
	uint32_t uiGPB = 0;
    INT_CHECK(GetMCP23S17Reg(0x13,uiGPB));
	GPIOB GPIO = *(GPIOB *)&uiGPB;
	GPIO.rx_lnaatt_sw = (unsigned)LNAAtt;
	uiGPB = *(uint32_t *)&GPIO;
    INT_CHECK(SetMCP23S17Reg(0x13,uiGPB));
	return 0;
}

int32_t CSP1401R1C::SetRxAtt1(double dAtt)
{
	int32_t D0_5 = 63 - (int32_t)(63 * dAtt / 31.5);

    RFU_K7_REG_DECLARE_2(0x1820,0x2820);
    RFU_K7_REG_DECLARE_2(0x1821,0x2821);
    RFU_K7_REG_2(0x1821,0x2821).data_word = (unsigned)D0_5;
    RFU_K7_WAIT_IDLE_2(0x1820,0x2820,0,1000);
    RFU_K7_W_2(0x1821,0x2821);
    RFU_K7_WAIT_IDLE_2(0x1820,0x2820,0,1000);
    RFU_K7_OP_2(0x1820,0x2820);
    RFU_K7_WAIT_IDLE_2(0x1820,0x2820,0,1000);
	return 0;
}

int32_t CSP1401R1C::SetRxAtt2(double dAtt)
{
	int32_t D0_5 = 63 - (int32_t)(63 * dAtt / 31.5);

    RFU_K7_REG_DECLARE_2(0x1824,0x2824);
    RFU_K7_REG_DECLARE_2(0x1825,0x2825);
    RFU_K7_REG_2(0x1825,0x2825).data_word = (unsigned)D0_5;
    RFU_K7_WAIT_IDLE_2(0x1824,0x2824,0,1000);
    RFU_K7_W_2(0x1825,0x2825);
    RFU_K7_WAIT_IDLE_2(0x1824,0x2824,0,1000);
    RFU_K7_OP_2(0x1824,0x2824);
    RFU_K7_WAIT_IDLE_2(0x1824,0x2824,0,1000);
	return 0;
}

int32_t CSP1401R1C::SetRxAtt3(double dAtt)
{
	int32_t D0_5 = 63 - (int32_t)(63 * dAtt / 31.5);

    RFU_K7_REG_DECLARE_2(0x182b,0x282b);
    RFU_K7_REG_DECLARE_2(0x182c,0x282c);
    RFU_K7_REG_2(0x182c,0x282c).data_word = (unsigned)D0_5;
    RFU_K7_WAIT_IDLE_2(0x182b,0x282b,0,1000);
    RFU_K7_W_2(0x182c,0x282c);
    RFU_K7_WAIT_IDLE_2(0x182b,0x282b,0,1000);
    RFU_K7_OP_2(0x182b,0x282b);
    RFU_K7_WAIT_IDLE_2(0x182b,0x282b,0,1000);
	return 0;
}

int32_t CSP1401R1C::SetRxAtt(double dAtt1,double dAtt2,double dAtt3)
{
    INT_CHECK(SetRxAtt1(dAtt1));
    INT_CHECK(SetRxAtt2(dAtt2));
    INT_CHECK(SetRxAtt3(dAtt3));
	return 0;
}

int32_t CSP1401R1C::GetTemp0RxLO2(double *pTemp)
{
	uint32_t uiAd = 0;
    INT_CHECK(GetMCP3208(0,uiAd));
	*pTemp = tc1407vol2temp(mcp3208ad2vol(uiAd));
	return 0;
}

int32_t CSP1401R1C::GetTemp1RxLNA(double *pTemp)
{
	uint32_t uiAd = 0;
    INT_CHECK(GetMCP3208(1,uiAd));
	*pTemp = tc1407vol2temp(mcp3208ad2vol(uiAd));
	return 0;
}

int32_t CSP1401R1C::GetTemp2RxLO1(double *pTemp)
{
	uint32_t uiAd = 0;
    INT_CHECK(GetMCP3208(2,uiAd));
	*pTemp = tc1407vol2temp(mcp3208ad2vol(uiAd));
	return 0;
}

int32_t CSP1401R1C::GetTemp3RxPA1(double *pTemp)
{
	uint32_t uiAd = 0;
    INT_CHECK(GetMCP3208(3,uiAd));
	*pTemp = tc1407vol2temp(mcp3208ad2vol(uiAd));
	return 0;
}

int32_t CSP1401R1C::GetTemp4TxLO2(double *pTemp)
{
	uint32_t uiAd = 0;
    INT_CHECK(GetMCP3208(4,uiAd));
	*pTemp = tc1407vol2temp(mcp3208ad2vol(uiAd));
	return 0;
}

int32_t CSP1401R1C::GetTemp5TxLO1(double *pTemp)
{
	uint32_t uiAd = 0;
    INT_CHECK(GetMCP3208(5,uiAd));
	*pTemp = tc1407vol2temp(mcp3208ad2vol(uiAd));
	return 0;
}

int32_t CSP1401R1C::GetTemp6TxPA4(double *pTemp)
{
	uint32_t uiAd = 0;
    INT_CHECK(GetMCP3208(6,uiAd));
	*pTemp = tc1407vol2temp(mcp3208ad2vol(uiAd));
	return 0;
}

int32_t CSP1401R1C::GetTemp7TxPA3(double *pTemp)
{
	uint32_t uiAd = 0;
    INT_CHECK(GetMCP3208(7,uiAd));
	*pTemp = tc1407vol2temp(mcp3208ad2vol(uiAd));
	return 0;
}

int32_t CSP1401R1C::GetAD7680TxIF2(uint32_t &uiDet)
{
    RFU_K7_REG_DECLARE_2(0x1827,0x2827);

	uint32_t uiGPA = 0;
    INT_CHECK(GetMCP23S17Reg(0x12,uiGPA));
	GPIOA GPIO = *(GPIOA *)&uiGPA;
    RFU_K7_REG_2(0x1827,0x2827).sw = 0;
	GPIO.pd_rxif12_txif2 = 1;
	GPIO.pd_rxcomb_other = 0;
	uiGPA = *(uint32_t *)&GPIO;
    RFU_K7_W_2(0x1827,0x2827);
    INT_CHECK(SetMCP23S17Reg(0x12,uiGPA));
    INT_CHECK(GetAD7680(uiDet));
	return 0;
}

int32_t CSP1401R1C::GetAD7680RxComb(uint32_t &uiDet)
{
    RFU_K7_REG_DECLARE_2(0x1827,0x2827);

	uint32_t uiGPA = 0;
    INT_CHECK(GetMCP23S17Reg(0x12,uiGPA));
	GPIOA GPIO = *(GPIOA *)&uiGPA;
    RFU_K7_REG_2(0x1827,0x2827).sw = 1;
	GPIO.pd_rxif12_txif2 = 0;
	GPIO.pd_rxcomb_other = 1;
	uiGPA = *(uint32_t *)&GPIO;
    RFU_K7_W_2(0x1827,0x2827);
    INT_CHECK(SetMCP23S17Reg(0x12,uiGPA));
    INT_CHECK(GetAD7680(uiDet));
	return 0;
}

int32_t CSP1401R1C::GetAD7680RxIF1(uint32_t &uiDet)
{
    RFU_K7_REG_DECLARE_2(0x1827,0x2827);

	uint32_t uiGPA = 0;
    INT_CHECK(GetMCP23S17Reg(0x12,uiGPA));
	GPIOA GPIO = *(GPIOA *)&uiGPA;
    RFU_K7_REG_2(0x1827,0x2827).sw = 0;
	GPIO.pd_rxif12_txif2 = 0;
	GPIO.pd_rxcomb_other = 0;
	uiGPA = *(uint32_t *)&GPIO;
    RFU_K7_W_2(0x1827,0x2827);
    INT_CHECK(SetMCP23S17Reg(0x12,uiGPA));
    INT_CHECK(GetAD7680(uiDet));
	return 0;
}

int32_t CSP1401R1C::GetAD7680RxIF2(uint32_t &uiDet)
{
    RFU_K7_REG_DECLARE_2(0x1827,0x2827);

	uint32_t uiGPA = 0;
    INT_CHECK(GetMCP23S17Reg(0x12,uiGPA));
	GPIOA GPIO = *(GPIOA *)&uiGPA;
    RFU_K7_REG_2(0x1827,0x2827).sw = 1;
	GPIO.pd_rxif12_txif2 = 0;
	GPIO.pd_rxcomb_other = 0;
	uiGPA = *(uint32_t *)&GPIO;
    RFU_K7_W_2(0x1827,0x2827);
    INT_CHECK(SetMCP23S17Reg(0x12,uiGPA));
    INT_CHECK(GetAD7680(uiDet));
	return 0;
}

int32_t CSP1401R1C::SetMCP23S17Reg(uint32_t uiAddr,uint32_t uiData)
{
    RFU_K7_REG_DECLARE_2(0x182d,0x282d);
    RFU_K7_REG_DECLARE_2(0x182e,0x282e);

    RFU_K7_WAIT_IDLE_2(0x182d,0x282d,0,1000);
    RFU_K7_REG_2(0x182d,0x282d).addr = (unsigned)uiAddr;
    RFU_K7_REG_2(0x182d,0x282d).wr = 0;
    RFU_K7_REG_2(0x182e,0x282e).data = (unsigned)uiData;
    RFU_K7_W_2(0x182d,0x282d);
    RFU_K7_W_2(0x182e,0x282e);
    RFU_K7_OP_2(0x182d,0x282d);
    RFU_K7_WAIT_IDLE_2(0x182d,0x282d,0,1000);
	return 0;
}

int32_t CSP1401R1C::GetMCP23S17Reg(uint32_t uiAddr,uint32_t &uiData)
{
    RFU_K7_REG_DECLARE_2(0x182d,0x282d);
    RFU_K7_REG_DECLARE_2(0x182f,0x282f);

    RFU_K7_WAIT_IDLE_2(0x182d,0x282d,0,1000);
    RFU_K7_REG_2(0x182d,0x282d).addr = (unsigned)uiAddr;
    RFU_K7_REG_2(0x182d,0x282d).wr = 1;
    RFU_K7_W_2(0x182d,0x282d);
    RFU_K7_OP_2(0x182d,0x282d);
    RFU_K7_WAIT_IDLE_2(0x182d,0x282d,0,1000);
    RFU_K7_R_2(0x182f,0x282f);
    uiData = RFU_K7_REG_2(0x182f,0x282f).data;
	return 0;
}

int32_t CSP1401R1C::InitMCP23S17()
{
    INT_CHECK(SetMCP23S17Reg(0,0x00));
    INT_CHECK(SetMCP23S17Reg(1,0x00));
	return 0;
}

int32_t CSP1401R1C::GetMCP3208(uint32_t uiAddr,uint32_t &uiData)
{
    RFU_K7_REG_DECLARE_2(0x1832,0x2832);
    RFU_K7_REG_DECLARE_2(0x1833,0x2833);

    RFU_K7_REG_2(0x1832,0x2832).addr = (unsigned)uiAddr;
    RFU_K7_OP_2(0x1832,0x2832);
    RFU_K7_WAIT_IDLE_2(0x1832,0x2832,0,1000);
    RFU_K7_R_2(0x1833,0x2833);
    uiData = (uint32_t)(RFU_K7_REG_2(0x1833,0x2833).ad);
	return 0;
}
