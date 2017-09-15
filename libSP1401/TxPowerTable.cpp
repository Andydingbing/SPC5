#include "TxPowerTable.h"

//r1a/b
void TxPowerTableR1A::Add(DataF *pData)
{
    uint32_t uiIdx = (uint32_t)SERIE_INDEX(pData->m_State[0].m_uiFreq,RF_TX_FREQ_STAR,RF_TX_FREQ_STEP_INFILE);
	m_pDataF[uiIdx] = *pData;
}

void TxPowerTableR1A::Map2Mem()
{
	if (NULL == m_pDataF)
		return;
	uint32_t uiIdxF = 0,uiIdxM = 0;
	uint64_t uiFreq = 0;
    for (uiIdxM = 0;uiIdxM < RF_TX_FREQ_PTS_CALLED;uiIdxM ++) {
        uiFreq = (uint64_t)RF_TX_FREQ_STAR + uiIdxM * (uint64_t)RF_TX_FREQ_STEP_CALLED;
        uiIdxF = (uint32_t)SERIE_INDEX(uiFreq,RF_TX_FREQ_STAR,RF_TX_FREQ_STEP_INFILE);
        for (uint32_t i = 0;i < sizeof(DataM) / sizeof(TxStateM);i ++) {
			m_pDataM[uiIdxM].m_State[i].m_fDGain = (float)(m_pDataF[uiIdxF].m_State[i].m_dDGain);
			m_pDataM[uiIdxM].m_State[i].m_iAtt1 = m_pDataF[uiIdxF].m_State[i].m_iAtt1;
			m_pDataM[uiIdxM].m_State[i].m_iAtt2 = m_pDataF[uiIdxF].m_State[i].m_iAtt2;
			m_pDataM[uiIdxM].m_State[i].m_iPAAtt= m_pDataF[uiIdxF].m_State[i].m_iPAAtt;
			m_pDataM[uiIdxM].m_State[i].m_iAtt3 = m_pDataF[uiIdxF].m_State[i].m_iAtt3;
		}
	}
}

uint32_t TxPowerTableR1A::GetPowerIndex(double dPower)
{
	double dPowerL = 0.0,dPowerR = 0.0;
    discretept(dPower,R1A_TX_POWER_STEP,dPowerL,dPowerR);
    return (uint32_t)SERIE_INDEX(dPowerL,R1A_TX_POWER_STAR,R1A_TX_POWER_STEP);
}

void TxPowerTableR1A::GuessBaseOP(int32_t &iAtt1,int32_t &iAtt2,int32_t &iAtt3)
{
// 		{0, 0.0,   0,   0,   (int32_t)(SP1401::Tx_PA),  0  },	//10
// 		{0, 0.0,   -10, 0,   (int32_t)(SP1401::Tx_PA),  0  },	//0
// 		{0, 0.0,   -20, 0,   (int32_t)(SP1401::Tx_PA),  0  },	//-10
// 
// 		{0, -10.0, -20, 0,   (int32_t)(SP1401::Tx_PA),  0  },	//-20
// 		{0, -10.0, -10, 0,   (int32_t)(SP1401::Tx_Att), 0  },	//-20
// 		/**********************************************/
// 
// 		{0, -10.0, -10, 0,   (int32_t)(SP1401::Tx_Att), -10},	//-30
// 		{0, -10.0, -10, -10, (int32_t)(SP1401::Tx_Att), -10},	//-40
// 		{0, -10.0, -10, -10, (int32_t)(SP1401::Tx_Att), -20},	//-50
// 		{0, -10.0, -20, -10, (int32_t)(SP1401::Tx_Att), -20},	//-60
// 		{0, -10.0, -20, -20, (int32_t)(SP1401::Tx_Att), -20},	//-70
// 		{0, -10.0, -20, -20, (int32_t)(SP1401::Tx_Att), -30},	//-80
// 		{0, -10.0, -30, -20, (int32_t)(SP1401::Tx_Att), -30},	//-90
// 		{0, -10.0, -30, -30, (int32_t)(SP1401::Tx_Att), -30},	//-100

// 	iAtt1 = TxStateOP[SERIE_INDEX(0,R1A_TX_POWER_STAR,R1A_TX_POWER_STEP)].att_1;
// 	iAtt2 = TxStateOP[SERIE_INDEX(0,R1A_TX_POWER_STAR,R1A_TX_POWER_STEP)].att_2;
// 	iAtt3 = TxStateOP[SERIE_INDEX(0,R1A_TX_POWER_STAR,R1A_TX_POWER_STEP)].att_3;
	iAtt1 = -10;
	iAtt2 = 0;
	iAtt3 = 0;
}

void TxPowerTableR1A::GuessBaseIO(int32_t &iAtt1,int32_t &iAtt2,int32_t &iAtt3)
{
// 		{0, 0.0,   0,   0,   (int32_t)(SP1401::Tx_PA),  0  },	//10
// 		{0, 0.0,   -10, 0,   (int32_t)(SP1401::Tx_PA),  0  },	//0
// 		{0, 0.0,   -20, 0,   (int32_t)(SP1401::Tx_PA),  0  },	//-10
// 
// 		{0, -10.0, -20, 0,   (int32_t)(SP1401::Tx_PA),  0  },	//-20
// 		{0, -10.0, -10, 0,   (int32_t)(SP1401::Tx_Att), 0  },	//-20
// 		/**********************************************/
// 
// 		{0, -10.0, -10, 0,   (int32_t)(SP1401::Tx_Att), -10},	//-30
// 		{0, -10.0, -10, -10, (int32_t)(SP1401::Tx_Att), -10},	//-40
// 		{0, -10.0, -10, -10, (int32_t)(SP1401::Tx_Att), -20},	//-50
// 		{0, -10.0, -20, -10, (int32_t)(SP1401::Tx_Att), -20},	//-60
// 		{0, -10.0, -20, -20, (int32_t)(SP1401::Tx_Att), -20},	//-70
// 		{0, -10.0, -20, -20, (int32_t)(SP1401::Tx_Att), -30},	//-80
// 		{0, -10.0, -30, -20, (int32_t)(SP1401::Tx_Att), -30},	//-90
// 		{0, -10.0, -30, -30, (int32_t)(SP1401::Tx_Att), -30},	//-100

// 	iAtt1 = TxStateIO[SERIE_INDEX(0,R1A_TX_POWER_STAR,R1A_TX_POWER_STEP)].att_1;
// 	iAtt2 = TxStateIO[SERIE_INDEX(0,R1A_TX_POWER_STAR,R1A_TX_POWER_STEP)].att_2;
// 	iAtt3 = TxStateIO[SERIE_INDEX(0,R1A_TX_POWER_STAR,R1A_TX_POWER_STEP)].att_3;
	iAtt1 = -10;
	iAtt2 = 0;
	iAtt3 = 0;
}

void TxPowerTableR1A::Split(double dAtt,double *pRf,double *pBb)
{	//"pRf > 0" means RF Att;"pBb > 0" means Digital Gain & pBb < 0 means Digital Att
	*pRf = ((int32_t)(dAtt / 10.0)) * 10.0;
    *pRf = (*pRf >= /**(double *)*/R1A_TX_ATT_DR ? /**(double *)*/R1A_TX_ATT_DR : *pRf);
	*pBb = *pRf - dAtt;
	if (*pBb < (R1A_DGAIN_MIN + 1)) {
        *pRf = *pRf + (*pRf >= R1A_TX_ATT_DR ? 0 : R1A_TX_ATT_STEP_MIN);
		*pBb = *pRf - dAtt;
	}
	if (*pBb > R1A_DGAIN_MAX) {
        *pRf = *pRf - (*pRf > 0 ? R1A_TX_ATT_STEP_MIN : 0);
		*pBb = *pRf - dAtt;
	}
}

void TxPowerTableR1A::Split(int32_t iAtt,int32_t &iAtt1,int32_t &iAtt2,int32_t &iAtt3)
{
	iAtt1 = iAtt >= 30.0 ? 30 : ((int32_t)(iAtt / 10.0)) * 10;
	iAtt2 = (iAtt - iAtt1) >= 30 ? 30 : ((int32_t)((iAtt - iAtt1) / 10.0)) * 10;
	iAtt3 = (iAtt - iAtt1 - iAtt2) >= 30 ? 30 : ((int32_t)((iAtt - iAtt1 - iAtt2) / 10.0)) * 10;
}

void TxPowerTableR1A::GetOP(uint64_t uiFreq,double dPower,double &dBBGain,int32_t &iAtt1,int32_t &iAtt2,int32_t &iPAAtt,int32_t &iAtt3)
{
    uiFreq = uiFreq < RF_TX_FREQ_STAR  ? RF_TX_FREQ_STAR  : uiFreq;
    uiFreq = uiFreq > RF_TX_FREQ_STOP  ? RF_TX_FREQ_STOP  : uiFreq;
    dPower = dPower > R1A_TX_POWER_STAR ? R1A_TX_POWER_STAR : dPower;
	int32_t iDGainMin = R1A_DGAIN_MIN + 1;
	int32_t iDGainMax = R1A_DGAIN_MAX;
	uint64_t uiFreqL = 0,uiFreqR = 0;
	double dPowerL = 0.0,dPowerR = 0.0;
	double dBBGainL = 0.0,dBBGainR = 0.0;	//compare which calibration point to use
	bool bUseL = true;						//decide if using the calibration point which power > Power
    discretept(uiFreq,RF_TX_FREQ_STEP_CALLED,uiFreqL,uiFreqR);
    discretept(dPower,R1A_TX_POWER_STEP,dPowerL,dPowerR);

    uint32_t uiFreqIdxL = (uint32_t)SERIE_INDEX(uiFreqL,RF_TX_FREQ_STAR,RF_TX_FREQ_STEP_CALLED);
    uint32_t uiFreqIdxR = (uint32_t)SERIE_INDEX(uiFreqR,RF_TX_FREQ_STAR,RF_TX_FREQ_STEP_CALLED);
	uint32_t uiPowerIdxL = GetPowerIndex(dPowerL);
	uint32_t uiPowerIdxR = GetPowerIndex(dPowerR);

	DataM DataMM;
	DataM DataML = m_pDataM[uiFreqIdxL];
	DataM DataMR = m_pDataM[uiFreqIdxR];
	
	for (int32_t i = 0;i < 5;i ++) {
		if ((DataML.m_State[i].m_iAtt1 == DataMR.m_State[i].m_iAtt1) && (DataML.m_State[i].m_iAtt2 == DataMR.m_State[i].m_iAtt2) && 
			(DataML.m_State[i].m_iAtt3 == DataMR.m_State[i].m_iAtt3) && (DataML.m_State[i].m_iPAAtt== DataMR.m_State[i].m_iPAAtt)) {
			linear(uiFreqL,DataML.m_State[i].m_fDGain,uiFreqR,DataMR.m_State[i].m_fDGain,uiFreq,DataMM.m_State[i].m_fDGain);
		}
		else {
			DataMM.m_State[i].m_fDGain = DataML.m_State[i].m_fDGain;
		}
		DataMM.m_State[i].m_iAtt1  = DataML.m_State[i].m_iAtt1;
		DataMM.m_State[i].m_iAtt2  = DataML.m_State[i].m_iAtt2;
		DataMM.m_State[i].m_iAtt3  = DataML.m_State[i].m_iAtt3;
		DataMM.m_State[i].m_iPAAtt = DataML.m_State[i].m_iPAAtt;
	}

    if (dPower >= R1A_TX_POWER_STOP + 10) {
		dBBGainL = DataMM.m_State[uiPowerIdxL].m_fDGain - (dPowerL - dPower);		 //may < R1A_DGAIN_MIN
		if (dBBGainL < iDGainMin) {
			dBBGainR = DataMM.m_State[uiPowerIdxR].m_fDGain + (dPower - dPowerR);	 //may > R1A_DGAIN_MAX
			if (dBBGainR > R1A_DGAIN_MAX)
				bUseL = ((iDGainMin - dBBGainL) <= (dBBGainR - R1A_DGAIN_MAX)) ? true : false;
			else
				bUseL = false;
		}
		else
			bUseL = true;
		dBBGain = bUseL ? dBBGainL : dBBGainR;
		iAtt1  = DataMM.m_State[bUseL ? uiPowerIdxL : uiPowerIdxR].m_iAtt1;
		iAtt2  = DataMM.m_State[bUseL ? uiPowerIdxL : uiPowerIdxR].m_iAtt2;
		iAtt3  = DataMM.m_State[bUseL ? uiPowerIdxL : uiPowerIdxR].m_iAtt3;
		iPAAtt = DataMM.m_State[bUseL ? uiPowerIdxL : uiPowerIdxR].m_iPAAtt;
	}
	else {
        uiPowerIdxL = GetPowerIndex(R1A_TX_POWER_STOP + 10);									//digital gain all based from -20
		int32_t iAtt = 0;																		//all rf att
        int32_t iExtraAttL = (int32_t)((int32_t)(R1A_TX_POWER_STOP + 10 - dPower) / 10) * 10;	//extra rf att from -20(right version),0,10,20,30...
        int32_t iExtraAttR = iExtraAttL + R1A_TX_ATT_STEP_MIN;
        dBBGainL = DataMM.m_State[uiPowerIdxL].m_fDGain - (R1A_TX_POWER_STOP + 10 - iExtraAttL - dPower);			//may < R1A_DGAIN_MIN
		if (dBBGainL < iDGainMin) {
            dBBGainR = DataMM.m_State[uiPowerIdxL].m_fDGain + (dPower - (R1A_TX_POWER_STOP + 10 - iExtraAttR));     //may > R1A_DGAIN_MAX
			if (dBBGainR > R1A_DGAIN_MAX)
				bUseL = ((iDGainMin - dBBGainL) <= (dBBGainR - R1A_DGAIN_MAX)) ? true : false;
			else
				bUseL = false;
		}
		else
			bUseL = true;
		dBBGain = bUseL ? dBBGainL : dBBGainR;
		iAtt = DataMM.m_State[uiPowerIdxL].m_iAtt1 + DataMM.m_State[uiPowerIdxL].m_iAtt2 + DataMM.m_State[uiPowerIdxL].m_iAtt3 + (bUseL ? iExtraAttL : iExtraAttR);
        dBBGain = iAtt > R1A_TX_ATT_DR ? dBBGain - (iAtt - R1A_TX_ATT_DR) : dBBGain;
        iAtt = iAtt > R1A_TX_ATT_DR ? R1A_TX_ATT_DR : iAtt;
		Split(iAtt,iAtt1,iAtt2,iAtt3);					//redistribute att
		iPAAtt = DataMM.m_State[uiPowerIdxL].m_iPAAtt;
	}
}

void TxPowerTableR1A::GetIO(uint64_t uiFreq,double dPower,double &dBBGain,int32_t &iAtt1,int32_t &iAtt2,int32_t &iPAAtt,int32_t &iAtt3)
{
    uiFreq = uiFreq < RF_TX_FREQ_STAR ? RF_TX_FREQ_STAR : uiFreq;
    uiFreq = uiFreq > RF_TX_FREQ_STOP ? RF_TX_FREQ_STOP : uiFreq;
    dPower = dPower > (R1A_TX_POWER_STAR - 10) ? (R1A_TX_POWER_STAR - 10) : dPower;
	int32_t iDGainMin = R1A_DGAIN_MIN + 1;
	int32_t iDGainMax = R1A_DGAIN_MAX;
	uint64_t uiFreqL = 0,uiFreqR = 0;
	double dPowerL = 0.0,dPowerR = 0.0;
	double dBBGainL = 0.0,dBBGainR = 0.0;	//compare which calibration point to use
	bool bUseL = true;						//decide if using the calibration point which power > Power
    discretept(uiFreq,RF_TX_FREQ_STEP_CALLED,uiFreqL,uiFreqR);
    discretept(dPower,R1A_TX_POWER_STEP,dPowerL,dPowerR);

    uint32_t uiFreqIdxL = (uint32_t)SERIE_INDEX(uiFreqL,RF_TX_FREQ_STAR,RF_TX_FREQ_STEP_CALLED);
    uint32_t uiFreqIdxR = (uint32_t)SERIE_INDEX(uiFreqR,RF_TX_FREQ_STAR,RF_TX_FREQ_STEP_CALLED);
	uint32_t uiPowerIdxL = GetPowerIndex(dPowerL) - 1;		//io mode start from 0dBm
	uint32_t uiPowerIdxR = GetPowerIndex(dPowerR) - 1;

	DataM DataMM;
	DataM DataML = m_pDataM[uiFreqIdxL];
	DataM DataMR = m_pDataM[uiFreqIdxR];

	for (int32_t i = 0;i < 5;i ++) {
		if ((DataML.m_State[i].m_iAtt1 == DataMR.m_State[i].m_iAtt1) && (DataML.m_State[i].m_iAtt2 == DataMR.m_State[i].m_iAtt2) && 
			(DataML.m_State[i].m_iAtt3 == DataMR.m_State[i].m_iAtt3) && (DataML.m_State[i].m_iPAAtt== DataMR.m_State[i].m_iPAAtt)) {
				linear(uiFreqL,DataML.m_State[i].m_fDGain,uiFreqR,DataMR.m_State[i].m_fDGain,uiFreq,DataMM.m_State[i].m_fDGain);
		}
		else {
			DataMM.m_State[i].m_fDGain = DataML.m_State[i].m_fDGain;
		}
		DataMM.m_State[i].m_iAtt1  = DataML.m_State[i].m_iAtt1;
		DataMM.m_State[i].m_iAtt2  = DataML.m_State[i].m_iAtt2;
		DataMM.m_State[i].m_iAtt3  = DataML.m_State[i].m_iAtt3;
		DataMM.m_State[i].m_iPAAtt = DataML.m_State[i].m_iPAAtt;
	}

    if (dPower >= (R1A_TX_POWER_STOP /*- 10*/)) {
		dBBGainL = DataMM.m_State[uiPowerIdxL].m_fDGain - (dPowerL - dPower);		 //may < R1A_DGAIN_MIN
		if (dBBGainL < iDGainMin) {
			dBBGainR = DataMM.m_State[uiPowerIdxR].m_fDGain + (dPower - dPowerR);	 //may > R1A_DGAIN_MAX
			if (dBBGainR > R1A_DGAIN_MAX)
				bUseL = ((iDGainMin - dBBGainL) <= (dBBGainR - R1A_DGAIN_MAX)) ? true : false;
			else
				bUseL = false;
		}
		else
			bUseL = true;
		dBBGain = bUseL ? dBBGainL : dBBGainR;
		iAtt1  = DataMM.m_State[bUseL ? uiPowerIdxL : uiPowerIdxR].m_iAtt1;
		iAtt2  = DataMM.m_State[bUseL ? uiPowerIdxL : uiPowerIdxR].m_iAtt2;
		iAtt3  = DataMM.m_State[bUseL ? uiPowerIdxL : uiPowerIdxR].m_iAtt3;
		iPAAtt = DataMM.m_State[bUseL ? uiPowerIdxL : uiPowerIdxR].m_iPAAtt;
	}
	else {
        uiPowerIdxL = GetPowerIndex(R1A_TX_POWER_STOP /*- 10*/) - 1;                                                //digital gain all based from -20
		int32_t iAtt = 0;																							//all rf att
        int32_t iExtraAttL = (int32_t)((int32_t)(R1A_TX_POWER_STOP /*-10*/ - dPower) / 10) * 10;					//extra rf att from -20(right version),0,10,20,30...
        int32_t iExtraAttR = iExtraAttL + R1A_TX_ATT_STEP_MIN;
        dBBGainL = DataMM.m_State[uiPowerIdxL].m_fDGain - (R1A_TX_POWER_STOP /*-10*/ - iExtraAttL - dPower);		//may < R1A_DGAIN_MIN
		if (dBBGainL < iDGainMin) {
            dBBGainR = DataMM.m_State[uiPowerIdxL].m_fDGain + (dPower - (R1A_TX_POWER_STOP /*-10*/ - iExtraAttR));  //may > R1A_DGAIN_MAX
			if (dBBGainR > R1A_DGAIN_MAX)
				bUseL = ((iDGainMin - dBBGainL) <= (dBBGainR - R1A_DGAIN_MAX)) ? true : false;
			else
				bUseL = false;
		}
		else
			bUseL = true;
		dBBGain = bUseL ? dBBGainL : dBBGainR;
		iAtt = DataMM.m_State[uiPowerIdxL].m_iAtt1 + DataMM.m_State[uiPowerIdxL].m_iAtt2 + DataMM.m_State[uiPowerIdxL].m_iAtt3 + (bUseL ? iExtraAttL : iExtraAttR);
        dBBGain = iAtt > R1A_TX_ATT_DR ? dBBGain - (iAtt - R1A_TX_ATT_DR) : dBBGain;
        iAtt = iAtt > R1A_TX_ATT_DR ? R1A_TX_ATT_DR : iAtt;
		Split(iAtt,iAtt1,iAtt2,iAtt3);
		iPAAtt = DataMM.m_State[uiPowerIdxL].m_iPAAtt;
	}
}

void TxPowerTableR1A::SaveAs(char *pPath)
{
	FILE *fp = fopen(pPath,"w");
	if (NULL == fp)
		return;

	double dPower = 10.0,dBBGain = 0.0;
	int32_t iAtt1 = 0,iAtt2 = 0,iAtt3 = 0;
	int32_t iPAAtt = 0;

	fprintf(fp,"/**********************************Output**********************************/\n");
    for (uint64_t uiFreq = RF_TX_FREQ_STAR;uiFreq <= RF_TX_FREQ_STOP;uiFreq += RF_TX_FREQ_STEP_INFILE) {
		for (dPower = 10;dPower >= -100;dPower --) {
			GetOP(uiFreq,dPower,dBBGain,iAtt1,iAtt2,iPAAtt,iAtt3);
            fprintf(fp,"freq : %10I64u \t power : %4.1f \t bb_gain : %6.3f \t att1 : %4d \t att2 : %4d \t pa?att : %4s \t att3 : %4d\n",
				uiFreq,dPower,dBBGain,iAtt1,iAtt2,iPAAtt == 0 ? "pa" : "att",iAtt3);
		}
	}

	fprintf(fp,"/**********************************IO**********************************/\n");
    for (uint64_t uiFreq = RF_TX_FREQ_STAR;uiFreq <= RF_TX_FREQ_STOP;uiFreq += RF_TX_FREQ_STEP_INFILE) {
		for (dPower = 0;dPower >= -100;dPower --) {
			GetIO(uiFreq,dPower,dBBGain,iAtt1,iAtt2,iPAAtt,iAtt3);
            fprintf(fp,"freq : %10I64u \t power : %4.1f \t bb_gain : %6.3f \t att1 : %4d \t att2 : %4d \t pa?att : %4s \t att3 : %4d\n",
				uiFreq,dPower,dBBGain,iAtt1,iAtt2,iPAAtt == 0 ? "pa" : "att",iAtt3);
		}
	}
	fclose(fp);
}

//r1c output mode
IMPLEMENT_TX_CAL_ITEM_TABLE(TxPowerOPTableR1C)

TxPowerOPTableR1C::DataF::DataF()
{
	m_uiFreq = 0;
	m_dDGain = -5.0;
	m_dAtt0 = 5.0;
	m_dAtt1 = 20.0;
	m_dAtt2 = 0.0;
	m_dAtt3 = 1.0;
    for (uint32_t i = 0;i < ARRAY_SIZE(m_dTemp);i ++)
		m_dTemp[i] = 0.0;
    m_EndTime = {0,0,0,0,0,0,0,0,0};
}

void TxPowerOPTableR1C::Map2Mem()
{
	if (NULL == m_pDataF)
		return;
	uint32_t uiIdxF = 0,uiIdxM = 0;
	uint64_t uiFreq = 0;
    for (uiIdxM = 0;uiIdxM < RF_TX_FREQ_PTS_CALLED;uiIdxM ++) {
        uiFreq = (uint64_t)RF_TX_FREQ_STAR + uiIdxM * (uint64_t)RF_TX_FREQ_STEP_CALLED;
        uiIdxF = (uint32_t)SERIE_INDEX(uiFreq,RF_TX_FREQ_STAR,RF_TX_FREQ_STEP_INFILE);
		m_pDataM[uiIdxM].m_fDGain = (float)(m_pDataF[uiIdxF].m_dDGain);
		m_pDataM[uiIdxM].m_fTemp5 = (float)(m_pDataF[uiIdxF].m_dTemp[1]);	//temp4,5(used),6,7
		m_pDataM[uiIdxM].m_sAtt0 = (short)(m_pDataF[uiIdxF].m_dAtt0 * 2.0);
		m_pDataM[uiIdxM].m_sAtt1 = (short)(m_pDataF[uiIdxF].m_dAtt1 * 2.0);
		m_pDataM[uiIdxM].m_sAtt2 = (short)(m_pDataF[uiIdxF].m_dAtt2 * 2.0);
		m_pDataM[uiIdxM].m_sAtt3 = (short)(m_pDataF[uiIdxF].m_dAtt3 * 2.0);
	}
}

void TxPowerOPTableR1C::GetConfigTable(int32_t iPower,DataM DataBase,DataM *pData)
{
	//		DGain	Att0	Att1	Att2	Att3	//	dBm

	// 		{-5,	 -5,	 -5,	  0,	 -1}	//	10
	// 		{-5,	 -5,	-10,	  0,	 -1}	//	5
	// 		{-5,	 -5,	-15,	  0,	 -1}	//	0
	////////{-5,	 -5,	-20,	  0,	 -1}//////	-5		//

	// 		{-5,	 -5,	-20,	 -5,	 -1}	//	-10
	// 		{-5,	 -5,	-20,	-10,	 -1}	//	-15
	// 		{-5,	 -5,	-20,	-15,	 -1}	//	-20		//

	// 		{-5,	 -5,	-20,	-15,	 -6}	//	-25
	// 		{-5,	 -5,	-20,	-15,	-11}	//	-30
	// 		{-5,	 -5,	-20,	-15,	-16}	//	-35		//

	// 		{-5,	 -5,	-20,	-20,	-16}	//	-40		//

	// 		{-5,	 -5,	-20,	-20,	-21}	//	-45	
	// 		{-5,	 -5,	-20,	-20,	-26}	//	-50		//

	// 		{-5,	 -5,	-20,	-25,	-26}	//	-55		//

	// 		{-5,	 -5,	-25,	-25,	-26}	//	-60		//

	// 		{-5,	-10,	-25,	-25,	-26}	//	-65		//

	// 		{-5,	-10,	-25,	-25,	-31}	//	-70		//

	// 		{-5,	-10,	-25,	-30,	-31}	//	-75		//

	// 		{-5,	-10,	-30,	-30,	-31}	//	-80		//
	// 		{-5,	-20,	-30,	-30,	-31}	//	-90
	// 		{-15,	-20,	-30,	-30,	-31}	//	-100
	// 		{-25,	-20,	-30,	-30,	-31}	//	-110
	// 		{-35,	-20,	-30,	-30,	-31}	//	-120

	double dAtt0 = DataBase.m_sAtt0 / 2.0;
	double dAtt1 = DataBase.m_sAtt1 / 2.0;
	double dAtt2 = DataBase.m_sAtt2 / 2.0;
	double dAtt3 = DataBase.m_sAtt3 / 2.0;
	double dDGain = (double)(DataBase.m_fDGain);

    if (iPower >= R1C_TX_BASE_POWER_OP) {
        dAtt1 += R1C_TX_BASE_POWER_OP - iPower;
		if (dAtt1 < 0) {
			dDGain -= dAtt1;
			dAtt1 = 0;
		}
	}
    else if (iPower >= R1C_TX_BASE_POWER_OP - 15.0)
		dAtt2 = -5.0 - iPower;
    else if (iPower >= R1C_TX_BASE_POWER_OP - 30.0) {
		dAtt2 += 15.0;
		dAtt3 = -19.0 - iPower;
	}
    else if (iPower >= R1C_TX_BASE_POWER_OP - 35.0) {
		dAtt2 = -20 - iPower;
		dAtt3 += 15.0;
	}
    else if (iPower >= R1C_TX_BASE_POWER_OP - 45.0) {
		dAtt2 += 20.0;
		dAtt3 = -24.0 - iPower;
	}
    else if (iPower >= R1C_TX_BASE_POWER_OP - 50.0) {
		dAtt2 = -30.0 - iPower;
		dAtt3 += 25.0;
	}
    else if (iPower >= R1C_TX_BASE_POWER_OP - 55.0) {
		dAtt1 += -55.0 - iPower;
		dAtt2 += 25.0;
		dAtt3 += 25.0;
	}
    else if (iPower >= R1C_TX_BASE_POWER_OP - 60.0) {
		dAtt0 += -60.0 - iPower;
		dAtt1 += 5.0;
		dAtt2 += 25.0;
		dAtt3 += 25.0;
	}
    else if (iPower >= R1C_TX_BASE_POWER_OP - 65.0) {
		dAtt0 += 5.0;
		dAtt1 += 5.0;
		dAtt2 += 25.0;
		dAtt3 = -39.0 - iPower;
	}
    else if (iPower >= R1C_TX_BASE_POWER_OP - 70.0) {
		dAtt0 += 5.0;
		dAtt1 += 5.0;
		dAtt2 = -45.0 - iPower;
		dAtt3 += 30.0;
	}
    else if (iPower >= R1C_TX_BASE_POWER_OP - 75.0) {
		dAtt0 += 5.0;
		dAtt1 += -70.0 - iPower;
		dAtt2 += 30.0;
		dAtt3 += 30.0;
	}
    else if (iPower >= R1C_TX_BASE_POWER_OP - 85.0) {
		dAtt0 += -75.0 - iPower;
		dAtt1 += 10.0;
		dAtt2 += 30.0;
		dAtt3 += 30.0;
	}
	else {
		dAtt0 += 15.0;
		dAtt1 += 10.0;
		dAtt2 += 30.0;
		dAtt3 += 30.0;
        dDGain -= R1C_TX_BASE_POWER_OP - 85.0 - iPower;
	}
    if (dAtt0 > 31.5) {
        dDGain -= (dAtt0 - 31.5);
        dAtt0 = 31.5;
    }

	pData->m_sAtt0 = (short)(dAtt0 * 2);
	pData->m_sAtt1 = (short)(dAtt1 * 2);
	pData->m_sAtt2 = (short)(dAtt2 * 2);
	pData->m_sAtt3 = (short)(dAtt3 * 2);
	pData->m_fDGain = (float)dDGain;
	pData->m_fTemp5 = DataBase.m_fTemp5;
}

void TxPowerOPTableR1C::GuessBase(double &dAtt0,double &dAtt1,double &dAtt2,double &dAtt3,double &dDGain)
{
	dAtt0 = 5.0;
	dAtt1 = 20.0;
	dAtt2 = 0.0;
	dAtt3 = 1.0;
	dDGain = -5.0;
}

void TxPowerOPTableR1C::GetBase(uint64_t uiFreq,DataM *pData)
{
    uiFreq = uiFreq < RF_TX_FREQ_STAR ? RF_TX_FREQ_STAR : uiFreq;
    uiFreq = uiFreq > RF_TX_FREQ_STOP ? RF_TX_FREQ_STOP : uiFreq;

	uint64_t uiFreqL = 0,uiFreqR = 0;
    discretept(uiFreq,RF_TX_FREQ_STEP_CALLED,uiFreqL,uiFreqR);

    uint32_t uiFreqIdxL = (uint32_t)SERIE_INDEX(uiFreqL,RF_TX_FREQ_STAR,RF_TX_FREQ_STEP_CALLED);
    uint32_t uiFreqIdxR = (uint32_t)SERIE_INDEX(uiFreqR,RF_TX_FREQ_STAR,RF_TX_FREQ_STEP_CALLED);

	DataM DataL = m_pDataM[uiFreqIdxL];
	DataM DataR = m_pDataM[uiFreqIdxR];

	if (DataL.m_sAtt0 == DataR.m_sAtt0 && 
		DataL.m_sAtt1 == DataR.m_sAtt1 && 
		DataL.m_sAtt2 == DataR.m_sAtt2 && 
		DataL.m_sAtt3 == DataR.m_sAtt3 && 
		DataL.m_fTemp5 == DataR.m_fTemp5) {
			linear(uiFreqL,DataL.m_fDGain,uiFreqR,DataR.m_fDGain,uiFreq,pData->m_fDGain);
	}
	else
		pData->m_fDGain = DataL.m_fDGain;

	pData->m_sAtt0  = DataL.m_sAtt0;
	pData->m_sAtt1  = DataL.m_sAtt1;
	pData->m_sAtt2  = DataL.m_sAtt2;
	pData->m_sAtt3  = DataL.m_sAtt3;
	pData->m_fTemp5 = DataL.m_fTemp5;
}

void TxPowerOPTableR1C::GetBase(uint64_t uiFreq,DataF *pData)
{
    uiFreq = uiFreq < RF_TX_FREQ_STAR ? RF_TX_FREQ_STAR : uiFreq;
    uiFreq = uiFreq > RF_TX_FREQ_STOP ? RF_TX_FREQ_STOP : uiFreq;

	DataM Data;
	GetBase(uiFreq,&Data);

	uint64_t uiFreqL = 0,uiFreqR = 0;
    discretept(uiFreq,RF_TX_FREQ_STEP_CALLED,uiFreqL,uiFreqR);

    *pData = m_pDataF[(uint32_t)SERIE_INDEX(uiFreqL,RF_TX_FREQ_STAR,RF_TX_FREQ_STEP_INFILE)];
	pData->m_uiFreq = uiFreq;
	pData->m_dAtt0 = (double)(Data.m_sAtt0 / 2.0);
	pData->m_dAtt1 = (double)(Data.m_sAtt1 / 2.0);
	pData->m_dAtt2 = (double)(Data.m_sAtt2 / 2.0);
	pData->m_dAtt3 = (double)(Data.m_sAtt3 / 2.0);
	pData->m_dDGain = (double)(Data.m_fDGain);
}

void TxPowerOPTableR1C::Get(uint64_t uiFreq,double dPower,DataM *pData)
{
	DataM DataBase;
    dPower = dPower > (double)R1C_TX_ATT_OP_POWER_STAR ? (double)R1C_TX_ATT_OP_POWER_STAR : dPower;
	double dPowerL = 0.0,dPowerR = 0.0;

    discretept(dPower,R1C_TX_ATT_STEP,dPowerL,dPowerR);
	GetBase(uiFreq,&DataBase);
	TxPowerOPTableR1C::GetConfigTable((int32_t)dPowerL,DataBase,pData);
	pData->m_fDGain -= (float)(dPowerL - dPower);
}

void TxPowerOPTableR1C::Get(uint64_t uiFreq,double dPower,DataF *pData)
{
	DataM Data;
	Get(uiFreq,dPower,&Data);
	GetBase(uiFreq,pData);
	pData->m_dAtt0 = (double)(Data.m_sAtt0 / 2.0);
	pData->m_dAtt1 = (double)(Data.m_sAtt1 / 2.0);
	pData->m_dAtt2 = (double)(Data.m_sAtt2 / 2.0);
	pData->m_dAtt3 = (double)(Data.m_sAtt3 / 2.0);
	pData->m_dDGain = (double)(Data.m_fDGain);
}

void TxPowerOPTableR1C::SaveAs(char *pPath)
{
	FILE *fp = fopen(pPath,"w");
	if (NULL == fp)
		return;

	double dPower = 10.0,dDGain = 0.0;
	TxPowerOPTableR1C::DataF Data;

    for (uint64_t uiFreq = RF_TX_FREQ_STAR;uiFreq <= RF_TX_FREQ_STOP;uiFreq += RF_TX_FREQ_STEP_INFILE) {
        for (dPower = R1C_TX_ATT_OP_POWER_STAR;dPower >= -120;dPower += R1C_TX_ATT_STEP) {
			Get(uiFreq,dPower,&Data);
            fprintf(fp,"freq : %10I64u \t power : %4.1f \t digital gain : %6.3f \t att0 : %3.1f \t att1 : %3.1f \t att2 : %3.1f \t att3 : %3.1f \t temp4 : %-.3f temp5 : %-.3f temp6 : %-.3f temp7 : %-.3f time : %4d/%02d/%02d %02d:%02d:%02d\n",
					uiFreq,dPower,Data.m_dDGain,Data.m_dAtt0,Data.m_dAtt1,Data.m_dAtt2,Data.m_dAtt3,Data.m_dTemp[0],Data.m_dTemp[1],Data.m_dTemp[2],Data.m_dTemp[3],
                    Data.m_EndTime.tm_year + 1900,Data.m_EndTime.tm_mon,Data.m_EndTime.tm_mday,Data.m_EndTime.tm_hour,Data.m_EndTime.tm_min,Data.m_EndTime.tm_sec);
		}
	}
	fclose(fp);
}

//r1c io mode
IMPLEMENT_TX_CAL_ITEM_TABLE(TxPowerIOTableR1C)

TxPowerIOTableR1C::DataF::DataF()
{
	m_uiFreq = 0;
	m_dDGain = -5.0;
	m_dAtt0 = 5.0;
	m_dAtt1 = 20.0;
	m_dAtt2 = 0.0;
	m_dAtt3 = 1.0;
    for (uint32_t i = 0;i < ARRAY_SIZE(m_dTemp);i ++)
		m_dTemp[i] = 0.0;
    m_EndTime = {0,0,0,0,0,0,0,0,0};
}

void TxPowerIOTableR1C::Map2Mem()
{
	if (NULL == m_pDataF)
		return;
	uint32_t uiIdxF = 0,uiIdxM = 0;
	uint64_t uiFreq = 0;
    for (uiIdxM = 0;uiIdxM < RF_TX_FREQ_PTS_CALLED;uiIdxM ++) {
        uiFreq = (uint64_t)RF_TX_FREQ_STAR + uiIdxM * (uint64_t)RF_TX_FREQ_STEP_CALLED;
        uiIdxF = (uint32_t)SERIE_INDEX(uiFreq,RF_TX_FREQ_STAR,RF_TX_FREQ_STEP_INFILE);
		m_pDataM[uiIdxM].m_fDGain = (float)(m_pDataF[uiIdxF].m_dDGain);
		m_pDataM[uiIdxM].m_fTemp5 = (float)(m_pDataF[uiIdxF].m_dTemp[1]);	//temp4,5(used),6,7
		m_pDataM[uiIdxM].m_sAtt0 = (short)(m_pDataF[uiIdxF].m_dAtt0 * 2.0);
		m_pDataM[uiIdxM].m_sAtt1 = (short)(m_pDataF[uiIdxF].m_dAtt1 * 2.0);
		m_pDataM[uiIdxM].m_sAtt2 = (short)(m_pDataF[uiIdxF].m_dAtt2 * 2.0);
		m_pDataM[uiIdxM].m_sAtt3 = (short)(m_pDataF[uiIdxF].m_dAtt3 * 2.0);
	}
}

void TxPowerIOTableR1C::GetConfigTable(int32_t iPower,DataM DataBase,DataM *pData)
{
	//		DGain	Att0	Att1	Att2	Att3	//	dBm

	// 		{-5,	 -5,	 -3,	  0,	 -1}	//	5
	////////{-5,	 -5,	-20,	  0,	 -1}//////	-12		//
	// 		{-5,	 -5,	-20,	-15,	 -1}	//	-27		//
	// 		{-5,	 -5,	-20,	-15,	-16}	//	-42		//
	// 		{-5,	 -5,	-20,	-20,	-16}	//	-47		//
	// 		{-5,	 -5,	-20,	-20,	-26}	//	-57		//
	// 		{-5,	 -5,	-20,	-25,	-26}	//	-62		//
	//		{-5,	 -5,	-25,	-25,	-26}	//	-67		//
	// 		{-5,	-10,	-25,	-25,	-26}	//	-72		//
	// 		{-5,	-10,	-25,	-25,	-31}	//	-77		//
	// 		{-5,	-10,	-25,	-30,	-31}	//	-82		//
	//		{-5,	-10,	-30,	-30,	-31}	//	-87		//
	//		{-5,	-20,	-30,	-30,	-31}	//	-97		//
	//		{-28,	-20,	-30,	-30,	-31}	//	-120	//

	double dAtt0 = DataBase.m_sAtt0 / 2.0;
	double dAtt1 = DataBase.m_sAtt1 / 2.0;
	double dAtt2 = DataBase.m_sAtt2 / 2.0;
	double dAtt3 = DataBase.m_sAtt3 / 2.0;
	double dDGain = (double)(DataBase.m_fDGain);

    if (iPower >= R1C_TX_BASE_POWER_IO) {
        dAtt1 += R1C_TX_BASE_POWER_IO - iPower;
		if (dAtt1 < 0) {
			dDGain -= dAtt1;
			dAtt1 = 0;
		}
	}
    else if (iPower >= R1C_TX_BASE_POWER_IO - 15.0)
        dAtt2 += R1C_TX_BASE_POWER_IO - iPower;
    else if (iPower >= R1C_TX_BASE_POWER_IO - 30.0) {
		dAtt2 += 15.0;
        dAtt3 += R1C_TX_BASE_POWER_IO - iPower - 15.0;
	}
    else if (iPower >= R1C_TX_BASE_POWER_IO - 35.0) {
        dAtt2 += R1C_TX_BASE_POWER_IO - iPower - 15.0;
		dAtt3 += 15.0;
	}
    else if (iPower >= R1C_TX_BASE_POWER_IO - 45.0) {
		dAtt2 += 20.0;
        dAtt3 += R1C_TX_BASE_POWER_IO - iPower - 20.0;
	}
    else if (iPower >= R1C_TX_BASE_POWER_IO - 50.0) {
		dAtt3 += 25.0;
        dAtt2 += R1C_TX_BASE_POWER_IO - iPower - 25.0;
	}
    else if (iPower >= R1C_TX_BASE_POWER_IO - 55.0) {
		dAtt2 += 25.0;
		dAtt3 += 25.0;
        dAtt1 += R1C_TX_BASE_POWER_IO - iPower - 50.0;
	}
    else if (iPower >= R1C_TX_BASE_POWER_IO - 60.0) {
		dAtt1 += 5.0;
		dAtt2 += 25.0;
		dAtt3 += 25.0;
        dAtt0 += R1C_TX_BASE_POWER_IO - iPower - 55.0;
	}
    else if (iPower >= R1C_TX_BASE_POWER_IO - 65.0) {
		dAtt0 += 5.0;
		dAtt1 += 5.0;
		dAtt2 += 25.0;
        dAtt3 += R1C_TX_BASE_POWER_IO - iPower - 35.0;
	}
    else if (iPower >= R1C_TX_BASE_POWER_IO - 70.0) {
		dAtt0 += 5.0;
		dAtt1 += 5.0;
		dAtt3 += 30.0;
        dAtt2 += R1C_TX_BASE_POWER_IO - iPower - 40.0;
	}
    else if (iPower >= R1C_TX_BASE_POWER_IO - 75.0) {
		dAtt0 += 5.0;
		dAtt2 += 30.0;
		dAtt3 += 30.0;
        dAtt1 += R1C_TX_BASE_POWER_IO - iPower - 65.0;
	}
    else if (iPower >= R1C_TX_BASE_POWER_IO - 85.0) {
		dAtt1 += 10.0;
		dAtt2 += 30.0;
		dAtt3 += 30.0;
        dAtt0 += R1C_TX_BASE_POWER_IO - iPower - 70.0;
	}
	else {
		dAtt0 += 15.0;
		dAtt1 += 10.0;
		dAtt2 += 30.0;
		dAtt3 += 30.0;
        dDGain -= R1C_TX_BASE_POWER_IO - iPower - 85.0;
	}
    if (dAtt0 > 31.5) {
        dDGain -= (dAtt0 - 31.5);
        dAtt0 = 31.5;
    }

	pData->m_sAtt0 = (short)(dAtt0 * 2);
	pData->m_sAtt1 = (short)(dAtt1 * 2);
	pData->m_sAtt2 = (short)(dAtt2 * 2);
	pData->m_sAtt3 = (short)(dAtt3 * 2);
	pData->m_fDGain = (float)dDGain;
	pData->m_fTemp5 = DataBase.m_fTemp5;
}

void TxPowerIOTableR1C::GuessBase(double &dAtt0,double &dAtt1,double &dAtt2,double &dAtt3,double &dDGain)
{
	dAtt0 = 5.0;
	dAtt1 = 20.0;
	dAtt2 = 0.0;
	dAtt3 = 1.0;
	dDGain = -5.0;
}

void TxPowerIOTableR1C::GetBase(uint64_t uiFreq,DataM *pData)
{
    uiFreq = uiFreq < RF_TX_FREQ_STAR ? RF_TX_FREQ_STAR : uiFreq;
    uiFreq = uiFreq > RF_TX_FREQ_STOP ? RF_TX_FREQ_STOP : uiFreq;

	uint64_t uiFreqL = 0,uiFreqR = 0;
    discretept(uiFreq,RF_TX_FREQ_STEP_CALLED,uiFreqL,uiFreqR);

    uint32_t uiFreqIdxL = (uint32_t)SERIE_INDEX(uiFreqL,RF_TX_FREQ_STAR,RF_TX_FREQ_STEP_CALLED);
    uint32_t uiFreqIdxR = (uint32_t)SERIE_INDEX(uiFreqR,RF_TX_FREQ_STAR,RF_TX_FREQ_STEP_CALLED);

	DataM DataL = m_pDataM[uiFreqIdxL];
	DataM DataR = m_pDataM[uiFreqIdxR];

	if (DataL.m_sAtt0 == DataR.m_sAtt0 && 
		DataL.m_sAtt1 == DataR.m_sAtt1 && 
		DataL.m_sAtt2 == DataR.m_sAtt2 && 
		DataL.m_sAtt3 == DataR.m_sAtt3 && 
		DataL.m_fTemp5 == DataR.m_fTemp5) {
			linear(uiFreqL,DataL.m_fDGain,uiFreqR,DataR.m_fDGain,uiFreq,pData->m_fDGain);
	}
	else
		pData->m_fDGain = DataL.m_fDGain;

	pData->m_sAtt0  = DataL.m_sAtt0;
	pData->m_sAtt1  = DataL.m_sAtt1;
	pData->m_sAtt2  = DataL.m_sAtt2;
	pData->m_sAtt3  = DataL.m_sAtt3;
	pData->m_fTemp5 = DataL.m_fTemp5;
}

void TxPowerIOTableR1C::GetBase(uint64_t uiFreq,DataF *pData)
{
    uiFreq = uiFreq < RF_TX_FREQ_STAR ? RF_TX_FREQ_STAR : uiFreq;
    uiFreq = uiFreq > RF_TX_FREQ_STOP ? RF_TX_FREQ_STOP : uiFreq;

	DataM Data;
	GetBase(uiFreq,&Data);

	uint64_t uiFreqL = 0,uiFreqR = 0;
    discretept(uiFreq,RF_TX_FREQ_STEP_CALLED,uiFreqL,uiFreqR);

    *pData = m_pDataF[(uint32_t)SERIE_INDEX(uiFreqL,RF_TX_FREQ_STAR,RF_TX_FREQ_STEP_INFILE)];
	pData->m_uiFreq = uiFreq;
	pData->m_dAtt0 = (double)(Data.m_sAtt0 / 2.0);
	pData->m_dAtt1 = (double)(Data.m_sAtt1 / 2.0);
	pData->m_dAtt2 = (double)(Data.m_sAtt2 / 2.0);
	pData->m_dAtt3 = (double)(Data.m_sAtt3 / 2.0);
	pData->m_dDGain = (double)(Data.m_fDGain);
}

void TxPowerIOTableR1C::Get(uint64_t uiFreq,double dPower,DataM *pData)
{
	DataM DataBase;
    dPower = dPower > (double)R1C_TX_ATT_IO_POWER_STAR ? (double)R1C_TX_ATT_IO_POWER_STAR : dPower;
	double dPowerL = 0.0,dPowerR = 0.0;

    discretept(dPower,R1C_TX_ATT_STEP,dPowerL,dPowerR);
	GetBase(uiFreq,&DataBase);
	TxPowerIOTableR1C::GetConfigTable((int32_t)dPowerL,DataBase,pData);
	pData->m_fDGain -= (float)(dPowerL - dPower);
}

void TxPowerIOTableR1C::Get(uint64_t uiFreq,double dPower,DataF *pData)
{
	DataM Data;
	Get(uiFreq,dPower,&Data);
	GetBase(uiFreq,pData);
	pData->m_dAtt0 = (double)(Data.m_sAtt0 / 2.0);
	pData->m_dAtt1 = (double)(Data.m_sAtt1 / 2.0);
	pData->m_dAtt2 = (double)(Data.m_sAtt2 / 2.0);
	pData->m_dAtt3 = (double)(Data.m_sAtt3 / 2.0);
	pData->m_dDGain = (double)(Data.m_fDGain);
}

void TxPowerIOTableR1C::SaveAs(char *pPath)
{
	FILE *fp = fopen(pPath,"w");
	if (NULL == fp)
		return;

	double dPower = 0.0,dDGain = 0.0;
	TxPowerIOTableR1C::DataF Data;

    for (uint64_t uiFreq = RF_TX_FREQ_STAR;uiFreq <= RF_TX_FREQ_STOP;uiFreq += RF_TX_FREQ_STEP_INFILE) {
        for (dPower = R1C_TX_ATT_IO_POWER_STAR;dPower >= -120;dPower --) {
			Get(uiFreq,dPower,&Data);
            fprintf(fp,"freq : %10I64u \t power : %4.1f \t digital gain : %6.3f \t att0 : %3.1f \t att1 : %3.1f \t att2 : %3.1f \t att3 : %3.1f \t temp4 : %-.3f temp5 : %-.3f temp6 : %-.3f temp7 : %-.3f time : %4d/%02d/%02d %02d:%02d:%02d\n",
				uiFreq,dPower,Data.m_dDGain,Data.m_dAtt0,Data.m_dAtt1,Data.m_dAtt2,Data.m_dAtt3,Data.m_dTemp[0],Data.m_dTemp[1],Data.m_dTemp[2],Data.m_dTemp[3],
                Data.m_EndTime.tm_year + 1900,Data.m_EndTime.tm_mon,Data.m_EndTime.tm_mday,Data.m_EndTime.tm_hour,Data.m_EndTime.tm_min,Data.m_EndTime.tm_sec);
		}
	}
	fclose(fp);
}
