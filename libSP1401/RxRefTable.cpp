#include "RxRefTable.h"

//r1a/b
IMPLEMENT_R1A_RX_CAL_ITEM_TABLE(RxRefTableR1A)

RxRefTableR1A::DataF::DataF()
{
	m_uiFreq = 0;
    for (int32_t i = 0;i < R1A_RX_REF_OP_PTS;i ++) {
		m_StateOP[i].m_dAtt1 = 0.0;
		m_StateOP[i].m_iADOffset = 0;
		m_StateOP[i].m_iAtt2 = 0;
		m_StateOP[i].m_iLNAAtt = 0;
	}
    for (int32_t i = 0;i < R1A_RX_REF_IO_PTS;i ++) {
		m_StateIO[i].m_dAtt1 = 0.0;
		m_StateIO[i].m_iADOffset = 0;
		m_StateIO[i].m_iAtt2 = 0;
		m_StateIO[i].m_iLNAAtt = 0;
	}
}

void RxRefTableR1A::Map2Mem()	//this is reserved for changing calibration step later
{
	if (NULL == m_pDataF)
		return;
	uint32_t uiIdxF = 0,uiIdxM = 0;
	uint64_t uiFreq = 0;
    for (uiIdxM = 0;uiIdxM < RF_RX_FREQ_PTS_CALLED_R1A;uiIdxM ++) {
        uiFreq = (uint64_t)RF_RX_FREQ_STAR + uiIdxM * (uint64_t)RF_RX_FREQ_STEP_CALLED_R1A;
        uiIdxF = (uint32_t)SERIE_INDEX(uiFreq,RF_RX_FREQ_STAR,RF_RX_FREQ_STEP_INFILE);
        for (uint32_t i = 0;i < R1A_RX_REF_OP_PTS;i ++) {
			m_pDataM[uiIdxM].m_StateOP[i].m_fAtt1	 = (float)(m_pDataF[uiIdxF].m_StateOP[i].m_dAtt1);
			m_pDataM[uiIdxM].m_StateOP[i].m_iADOffset = (int32_t)(m_pDataF[uiIdxF].m_StateOP[i].m_iADOffset);
			m_pDataM[uiIdxM].m_StateOP[i].m_iAtt2	 = m_pDataF[uiIdxF].m_StateOP[i].m_iAtt2;
			m_pDataM[uiIdxM].m_StateOP[i].m_iLNAAtt	 = m_pDataF[uiIdxF].m_StateOP[i].m_iLNAAtt;
		}
        for (uint32_t i = 0;i < R1A_RX_REF_IO_PTS;i ++) {
			m_pDataM[uiIdxM].m_StateIO[i].m_fAtt1	 = (float)(m_pDataF[uiIdxF].m_StateIO[i].m_dAtt1);
			m_pDataM[uiIdxM].m_StateIO[i].m_iADOffset = (int32_t)(m_pDataF[uiIdxF].m_StateIO[i].m_iADOffset);
			m_pDataM[uiIdxM].m_StateIO[i].m_iAtt2	 = m_pDataF[uiIdxF].m_StateIO[i].m_iAtt2;
			m_pDataM[uiIdxM].m_StateIO[i].m_iLNAAtt	 = m_pDataF[uiIdxF].m_StateIO[i].m_iLNAAtt;
		}
	}
}

int32_t RxRefTableR1A::GetSGPower(int32_t iRef)
{
    if (iRef >= 10)
		return 10;
// 	if (iRef <= -30)
// 		return -30;
    return iRef;
}

void RxRefTableR1A::GuessBaseOP(int32_t &iLNAAtt,double &dAtt1,int32_t &iAtt2)
{
	RxRefTableR1A::RxStateF RxStateOP[] = {			//Ref	//GS	//
		/*********************************************************************/
        {0,(int32_t)(SP1401::Rx_Att),-30,-20},	//30	//10	//_0dBFS_L - 20dBm
        {0,(int32_t)(SP1401::Rx_Att),-20,-20},	//20	//10	//_0dBFS_L - 10dBm
        {0,(int32_t)(SP1401::Rx_Att),-20,-10},	//10	//10	//_0dBFS_L
        {0,(int32_t)(SP1401::Rx_Att),-15,-10},	//0		//0		//_0dBFS_L
        {0,(int32_t)(SP1401::Rx_Att),-10, 0 },	//-10	//-10	//_0dBFS_L (IL)
        {0,(int32_t)(SP1401::Rx_Att), 0,  0 },	//-20	//-20	//_0dBFS_L

        {0,(int32_t)(SP1401::Rx_LNA), 0,  0 },	//-30	//-30	//_0dBFS_H
		/*********************************************************************/
        {0,(int32_t)(SP1401::Rx_Att), 0,  0 },	//-30			//_0dBFS_L - 10dB
        {0,(int32_t)(SP1401::Rx_Att), 0,  0 },	//-40			//_0dBFS_L - 20dB
        {0,(int32_t)(SP1401::Rx_Att), 0,  0 },	//-50			//_0dBFS_L - 30dB

        {0,(int32_t)(SP1401::Rx_LNA), 0,  0 },	//-60			//_0dBFS_H - 30dB
        {0,(int32_t)(SP1401::Rx_LNA), 0,  0 },	//-70			//_0dBFS_H - 40dB
        {0,(int32_t)(SP1401::Rx_LNA), 0,  0 },	//-80			//_0dBFS_H - 50dB
        {0,(int32_t)(SP1401::Rx_LNA), 0,  0 },	//-90			//_0dBFS_H - 60dB
        {0,(int32_t)(SP1401::Rx_LNA), 0,  0 },	//-100			//_0dBFS_H - 70dB
	};

    iLNAAtt = RxStateOP[SERIE_INDEX(0,R1A_RX_REF_OP_STAR,-10)].m_iLNAAtt;
    dAtt1   = RxStateOP[SERIE_INDEX(0,R1A_RX_REF_OP_STAR,-10)].m_dAtt1;
    iAtt2   = RxStateOP[SERIE_INDEX(0,R1A_RX_REF_OP_STAR,-10)].m_iAtt2;
}

void RxRefTableR1A::GuessBaseIO(int32_t &iLNAAtt,double &dAtt1,int32_t &iAtt2)
{
	RxRefTableR1A::RxStateF RxStateIO[] = {			//Ref	//SG	//
		/*********************************************************************/
        {0,(int32_t)(SP1401::Rx_Att),-30,-20},	//30	//10	//_0dBFS_L - 20dBm
        {0,(int32_t)(SP1401::Rx_Att),-20,-20},	//20	//10	//_0dBFS_L - 10dBm
        {0,(int32_t)(SP1401::Rx_Att),-20,-10},	//10	//10	//_0dBFS_L
        {0,(int32_t)(SP1401::Rx_Att),-10,-10},	//0		//0		//_0dBFS_L
        {0,(int32_t)(SP1401::Rx_Att),-10, 0 },	//-10	//-10	//_0dBFS_L (IL)
        {0,(int32_t)(SP1401::Rx_Att), 0,  0 },	//-20	//-20	//_0dBFS_L

        {0,(int32_t)(SP1401::Rx_LNA), 0,  0 },	//-30	//-30	//_0dBFS_H
		/*********************************************************************/
        {0,(int32_t)(SP1401::Rx_Att), 0,  0 },	//-30			//_0dBFS_L - 10dB
        {0,(int32_t)(SP1401::Rx_Att), 0,  0 },	//-40			//_0dBFS_L - 20dB
        {0,(int32_t)(SP1401::Rx_Att), 0,  0 },	//-50			//_0dBFS_L - 30dB

        {0,(int32_t)(SP1401::Rx_LNA), 0,  0 },	//-60			//_0dBFS_H - 30dB
        {0,(int32_t)(SP1401::Rx_LNA), 0,  0 },	//-70			//_0dBFS_H - 40dB
        {0,(int32_t)(SP1401::Rx_LNA), 0,  0 },	//-80			//_0dBFS_H - 50dB
        {0,(int32_t)(SP1401::Rx_LNA), 0,  0 },	//-90			//_0dBFS_H - 60dB
        {0,(int32_t)(SP1401::Rx_LNA), 0,  0 },	//-100			//_0dBFS_H - 70dB
	};
    iLNAAtt = RxStateIO[SERIE_INDEX(0,R1A_RX_REF_IO_STAR,-10)].m_iLNAAtt;
    dAtt1   = RxStateIO[SERIE_INDEX(0,R1A_RX_REF_IO_STAR,-10)].m_dAtt1;
    iAtt2   = RxStateIO[SERIE_INDEX(0,R1A_RX_REF_IO_STAR,-10)].m_iAtt2;
}

void RxRefTableR1A::GetIO(uint64_t uiFreq,double dRef,int64_t &iAD_0dBFS,int32_t &iLNAAtt,double &dAtt1,int32_t &iAtt2)
{
    uiFreq = uiFreq < RF_RX_FREQ_STAR ? RF_RX_FREQ_STAR : uiFreq;
    uiFreq = uiFreq > RF_RX_FREQ_STOP ? RF_RX_FREQ_STOP : uiFreq;
    dRef = dRef > R1A_RX_REF_IO_STAR ? R1A_RX_REF_IO_STAR : dRef;
    dRef = dRef < R1A_RX_REF_IO_STOP ? R1A_RX_REF_IO_STOP : dRef;

	uint64_t uiFreqL = 0,uiFreqR = 0;
	double dRefL = 0.0,dRefR = 0.0;
	
    discretept(uiFreq,RF_RX_FREQ_STEP_CALLED_R1A,uiFreqL,uiFreqR);
    discretept(dRef,R1A_RX_REF_STEP,dRefL,dRefR);

    uint32_t uiFreqIdxL = (uint32_t)SERIE_INDEX(uiFreqL,RF_RX_FREQ_STAR,RF_RX_FREQ_STEP_CALLED_R1A);
    uint32_t uiFreqIdxR = uiFreq >= RF_RX_FREQ_STOP ? uiFreqIdxL : (uint32_t)SERIE_INDEX(uiFreqR,RF_RX_FREQ_STAR,RF_RX_FREQ_STEP_CALLED_R1A);
    uint32_t uiRefIdxL = (uint32_t)SERIE_INDEX(dRefL,R1A_RX_REF_IO_STAR,R1A_RX_REF_STEP);
    uint32_t uiRefIdxR = (uint32_t)SERIE_INDEX(dRefR,R1A_RX_REF_IO_STAR,R1A_RX_REF_STEP);

	DataM DataMM;
	DataM DataML = m_pDataM[uiFreqIdxL];
	DataM DataMR = m_pDataM[uiFreqIdxR];

    for (uint32_t i = 0;i < ARRAY_SIZE(DataMM.m_StateIO);i ++) {
		if (DataML.m_StateIO[i].m_iLNAAtt == DataMR.m_StateIO[i].m_iLNAAtt &&
			DataML.m_StateIO[i].m_fAtt1 == DataMR.m_StateIO[i].m_fAtt1 &&
			DataML.m_StateIO[i].m_iAtt2 == DataMR.m_StateIO[i].m_iAtt2) {
				linear(uiFreqL,DataML.m_StateIO[i].m_iADOffset,uiFreqR,DataMR.m_StateIO[i].m_iADOffset,uiFreq,DataMM.m_StateIO[i].m_iADOffset);
		}
		else
			DataMM.m_StateIO[i].m_iADOffset = DataML.m_StateIO[i].m_iADOffset;
		
		DataMM.m_StateIO[i].m_iLNAAtt = DataML.m_StateIO[i].m_iLNAAtt;
		DataMM.m_StateIO[i].m_fAtt1   = DataML.m_StateIO[i].m_fAtt1;
		DataMM.m_StateIO[i].m_iAtt2   = DataML.m_StateIO[i].m_iAtt2;
	}
	iAD_0dBFS = dBc2ad(_0dBFS - DataML.m_StateIO[uiRefIdxL].m_iADOffset,dRefL - dRef);
	iLNAAtt  = DataMM.m_StateIO[uiRefIdxL].m_iLNAAtt;
	dAtt1    = DataMM.m_StateIO[uiRefIdxL].m_fAtt1;
	iAtt2    = DataMM.m_StateIO[uiRefIdxL].m_iAtt2;
}

void RxRefTableR1A::GetOP(uint64_t uiFreq,double dRef,int64_t &iAD_0dBFS,int32_t &iLNAAtt,double &dAtt1,int32_t &iAtt2)
{
    uiFreq = uiFreq < RF_RX_FREQ_STAR ? RF_RX_FREQ_STAR : uiFreq;
    uiFreq = uiFreq > RF_RX_FREQ_STOP ? RF_RX_FREQ_STOP : uiFreq;
    dRef = dRef > R1A_RX_REF_OP_STAR ? R1A_RX_REF_OP_STAR : dRef;
    dRef = dRef < R1A_RX_REF_OP_STOP ? R1A_RX_REF_OP_STOP : dRef;

	uint64_t uiFreqL = 0,uiFreqR = 0;
	double dRefL = 0.0,dRefR = 0.0;

    discretept(uiFreq,RF_RX_FREQ_STEP_CALLED_R1A,uiFreqL,uiFreqR);
    discretept(dRef,R1A_RX_REF_STEP,dRefL,dRefR);

    uint32_t uiFreqIdxL = (uint32_t)SERIE_INDEX(uiFreqL,RF_RX_FREQ_STAR,RF_RX_FREQ_STEP_CALLED_R1A);
    uint32_t uiFreqIdxR = uiFreq >= RF_RX_FREQ_STOP ? uiFreqIdxL : (uint32_t)SERIE_INDEX(uiFreqR,RF_RX_FREQ_STAR,RF_RX_FREQ_STEP_CALLED_R1A);
    uint32_t uiRefIdxL = (uint32_t)SERIE_INDEX(dRefL,R1A_RX_REF_OP_STAR,R1A_RX_REF_STEP);
    uint32_t uiRefIdxR = (uint32_t)SERIE_INDEX(dRefR,R1A_RX_REF_OP_STAR,R1A_RX_REF_STEP);

	DataM DataMM;
	DataM DataML = m_pDataM[uiFreqIdxL];
	DataM DataMR = m_pDataM[uiFreqIdxR];
	
    for (uint32_t i = 0;i < ARRAY_SIZE(DataMM.m_StateOP);i ++) {
		if (DataML.m_StateOP[i].m_iLNAAtt == DataMR.m_StateOP[i].m_iLNAAtt &&
			DataML.m_StateOP[i].m_fAtt1 == DataMR.m_StateOP[i].m_fAtt1 &&
			DataML.m_StateOP[i].m_iAtt2 == DataMR.m_StateOP[i].m_iAtt2) {
				linear(uiFreqL,DataML.m_StateOP[i].m_iADOffset,uiFreqR,DataMR.m_StateOP[i].m_iADOffset,uiFreq,DataMM.m_StateOP[i].m_iADOffset);
		}
		else
			DataMM.m_StateOP[i].m_iADOffset = DataML.m_StateOP[i].m_iADOffset;

		DataMM.m_StateOP[i].m_iLNAAtt = DataML.m_StateOP[i].m_iLNAAtt;
		DataMM.m_StateOP[i].m_fAtt1	  = DataML.m_StateOP[i].m_fAtt1;
		DataMM.m_StateOP[i].m_iAtt2   = DataML.m_StateOP[i].m_iAtt2;
	}
	
	iAD_0dBFS = dBc2ad(_0dBFS - DataML.m_StateOP[uiRefIdxL].m_iADOffset,dRefL - dRef);
	iLNAAtt   = DataMM.m_StateOP[uiRefIdxL].m_iLNAAtt;
	dAtt1     = DataMM.m_StateOP[uiRefIdxL].m_fAtt1;
	iAtt2     = DataMM.m_StateOP[uiRefIdxL].m_iAtt2;
}

void RxRefTableR1A::Get(uint64_t uiFreq,double dRef,IOMode Mode,int64_t &iAD_0dBFS,SP1401::RxLNAAtt &LNAAtt,double &dAtt1,int32_t &iAtt2)
{
	int32_t iLNAAtt = 0;
	if (OutPut == Mode)
		GetOP(uiFreq,dRef,iAD_0dBFS,iLNAAtt,dAtt1,iAtt2);
	if (IO == Mode)
		GetIO(uiFreq,dRef,iAD_0dBFS,iLNAAtt,dAtt1,iAtt2);
    LNAAtt = (0 == iLNAAtt ? SP1401::Rx_Att : SP1401::Rx_LNA);
}

void RxRefTableR1A::SaveAs(char *path)
{
	FILE *fp = fopen(path,"w");
	if (fp == NULL)
		return;
	
    int32_t iReff = R1A_RX_REF_OP_STAR;
    int64_t iAD_0dBFS = 0;
    int32_t iLNAAtt = 0;
    double  dAtt1 = 0;
    int32_t iAtt2 = 0;
    for (uint64_t uiFreq = RF_RX_FREQ_STAR;uiFreq <= RF_RX_FREQ_STOP;uiFreq += RF_RX_FREQ_STEP_CALLED_R1A) {
        fprintf(fp,"freq : %8I64u\toutput mode :\n",uiFreq);
        for (iReff = R1A_RX_REF_OP_STAR;iReff >= R1A_RX_REF_OP_STOP;iReff += R1A_RX_REF_STEP) {
            GetOP(uiFreq,iReff,iAD_0dBFS,iLNAAtt,dAtt1,iAtt2);
            fprintf(fp,"ref : %4d \t ad_0dbfs : %10I64u \t lna_att : %2s \t att_1 : %5.2f \t att_2 : %3d\n",
                iReff,iAD_0dBFS,iLNAAtt == 0 ? "att" : "lna",dAtt1,iAtt2);
		}
		fprintf(fp,"\nio mode :\n");
        for (iReff = R1A_RX_REF_IO_STAR;iReff >= R1A_RX_REF_IO_STOP;iReff += R1A_RX_REF_STEP) {
            GetIO(uiFreq,iReff,iAD_0dBFS,iLNAAtt,dAtt1,iAtt2);
            fprintf(fp,"ref : %4d \t ad_0dbfs : %10I64u \t lna_att : %2s \t att_1 : %5.2f \t att_2 : %3d\n",
                iReff,iAD_0dBFS,iLNAAtt == 0 ? "att" : "lna",dAtt1,iAtt2);
		}
		fprintf(fp,"\n");
	}
	fclose(fp);
}

//r1c output mode
IMPLEMENT_R1C_RX_CAL_ITEM_TABLE(RxRefOPTableR1C)

RxRefOPTableR1C::DataF::DataF()
{
	m_uiFreq = 0;
    for (uint32_t i = 0;i < ARRAY_SIZE(m_State);i ++) {
		m_State[i].m_iLNAAtt = 0;
		m_State[i].m_iAtt019 = 1;
		m_State[i].m_iADOffset = 0;
		m_State[i].m_fAtt1 = 0.0;
		m_State[i].m_fAtt2 = 0.0;
		m_State[i].m_fAtt3 = 0.0;
        for (uint32_t j = 0;j < ARRAY_SIZE(m_State[i].m_dTemp);j ++)
			m_State[i].m_dTemp[j] = 0.0;
	}
	m_EndTime.tm_sec = 0;
	m_EndTime.tm_min = 0;
	m_EndTime.tm_hour = 0;
	m_EndTime.tm_mday = 0;
	m_EndTime.tm_mon = 0;
	m_EndTime.tm_year = 0;
	m_EndTime.tm_wday = 0;
	m_EndTime.tm_yday = 0;
	m_EndTime.tm_isdst = 0;
}

void RxRefOPTableR1C::Map2Mem()
{
	if (NULL == m_pDataF)
		return;
	uint32_t uiIdxF = 0,uiIdxM = 0;
	uint64_t uiFreq = 0;
    for (uiIdxM = 0;uiIdxM < RF_RX_FREQ_PTS_CALLED_R1C;uiIdxM ++) {
        uiFreq = (uint64_t)RF_RX_FREQ_STAR + uiIdxM * (uint64_t)RF_RX_FREQ_STEP_CALLED_R1C;
        uiIdxF = (uint32_t)SERIE_INDEX(uiFreq,RF_RX_FREQ_STAR,RF_RX_FREQ_STEP_INFILE);
        for (uint32_t i = 0;i < ARRAY_SIZE(m_pDataM[uiIdxM].m_State);i ++) {
			m_pDataM[uiIdxM].m_State[i].m_fAtt1	= m_pDataF[uiIdxF].m_State[i].m_fAtt1;
			m_pDataM[uiIdxM].m_State[i].m_fAtt2 = m_pDataF[uiIdxF].m_State[i].m_fAtt2;
			m_pDataM[uiIdxM].m_State[i].m_fAtt3 = m_pDataF[uiIdxF].m_State[i].m_fAtt3;
			m_pDataM[uiIdxM].m_State[i].m_fTemp = (float)(m_pDataF[uiIdxF].m_State[i].m_dTemp[0]);
			m_pDataM[uiIdxM].m_State[i].m_iADOffset = (int32_t)(m_pDataF[uiIdxF].m_State[i].m_iADOffset);
			m_pDataM[uiIdxM].m_State[i].m_sLNAAtt = (short)(m_pDataF[uiIdxF].m_State[i].m_iLNAAtt);
			m_pDataM[uiIdxM].m_State[i].m_sAtt019 = (short)(m_pDataF[uiIdxF].m_State[i].m_iAtt019);
		}
	}
}

void RxRefOPTableR1C::GetConfigTable(int32_t iRef,DataM DataRef,RxStateM *pState)
{
	//	ref		att0	att1	att2	att3
	//	 30		 -20	 -25	 -15	 -10
	//	 20		 -20	 -15	 -15	 -10
	//	 10		 -20	  -5	 -15	 -10////m_State[0]
	//
	//	 10		   0	 -25	 -15	 -10////m_State[1]
	//	  0		   0	 -15	 -15	 -10
	//	-10		   0	  -5	 -15	 -10
	//	-20		   0	  -5	  -5	 -10

	//	-20		  14	 -10	 -15	 -10////m_State[2]
	//	-30		  14	   0	 -15	 -10
	//	-40		  14	   0	  -5	 -10

	short sLNAAtt = 0;
	short sAtt019 = 0;
	float fAtt1 = 0.0,fAtt2 = 0.0,fAtt3 = 0.0;
	int32_t iADOffset = 0;
	float fTemp = 0.0;

	if (iRef > 10) {
		sLNAAtt = DataRef.m_State[0].m_sLNAAtt;
		sAtt019 = DataRef.m_State[0].m_sAtt019;
		fAtt1 = DataRef.m_State[0].m_fAtt1 + (float)(iRef - 10);
		fAtt2 = DataRef.m_State[0].m_fAtt2;
		fAtt3 = DataRef.m_State[0].m_fAtt3;
		iADOffset = DataRef.m_State[0].m_iADOffset;
		fTemp = DataRef.m_State[0].m_fTemp;
	}
	else if (iRef >= -10) {
		sLNAAtt = DataRef.m_State[1].m_sLNAAtt;
		sAtt019 = DataRef.m_State[1].m_sAtt019;
		fAtt1 = DataRef.m_State[1].m_fAtt1 + (float)(iRef - 10);
		fAtt2 = DataRef.m_State[1].m_fAtt2;
		fAtt3 = DataRef.m_State[1].m_fAtt3;
		iADOffset = DataRef.m_State[1].m_iADOffset;
		fTemp = DataRef.m_State[1].m_fTemp;
	}
	else if (iRef > -20) {
		sLNAAtt = DataRef.m_State[1].m_sLNAAtt;
		sAtt019 = DataRef.m_State[1].m_sAtt019;
		fAtt1 = DataRef.m_State[1].m_fAtt1 - 20.0f;
		fAtt2 = DataRef.m_State[1].m_fAtt2 + (float)(iRef + 10);
		fAtt3 = DataRef.m_State[1].m_fAtt3;
		iADOffset = DataRef.m_State[1].m_iADOffset;
		fTemp = DataRef.m_State[1].m_fTemp;
	}
	else if (iRef >= -30) {
		sLNAAtt = DataRef.m_State[2].m_sLNAAtt;
		sAtt019 = DataRef.m_State[2].m_sAtt019;
		fAtt1 = DataRef.m_State[2].m_fAtt1 + (float)(iRef + 20);
		fAtt2 = DataRef.m_State[2].m_fAtt2;
		fAtt3 = DataRef.m_State[2].m_fAtt3;
		iADOffset = DataRef.m_State[2].m_iADOffset;
		fTemp = DataRef.m_State[2].m_fTemp;
	}
	else if (iRef >= -40) {
		sLNAAtt = DataRef.m_State[2].m_sLNAAtt;
		sAtt019 = DataRef.m_State[2].m_sAtt019;
		fAtt1 = DataRef.m_State[2].m_fAtt1 - 10.0f;
		fAtt2 = DataRef.m_State[2].m_fAtt2 + (float)(iRef + 30);
		fAtt3 = DataRef.m_State[2].m_fAtt3;
		iADOffset = DataRef.m_State[2].m_iADOffset;
		fTemp = DataRef.m_State[2].m_fTemp;
	}
	if (fAtt1 < 0) {
		iADOffset += (int32_t)(_0dBFS - dBc2ad(_0dBFS,-fAtt1));
		fAtt1 = 0;
	}
	if (fAtt2 < 0) {
		iADOffset += (int32_t)(_0dBFS - dBc2ad(_0dBFS,-fAtt2));
		fAtt2 = 0;
	}

	pState->m_sLNAAtt = sLNAAtt;
	pState->m_sAtt019 = sAtt019;
	pState->m_fAtt1 = fAtt1;
	pState->m_fAtt2 = fAtt2;
	pState->m_fAtt3 = fAtt3;
	pState->m_iADOffset = iADOffset;
	pState->m_fTemp = fTemp;
}

int32_t RxRefOPTableR1C::GetRefSegIdx(double dRef)
{
	if (dRef > 10.0)
		return 0;
	else if (dRef > -20.0)
		return 1;
	else
		return 2;
}

void RxRefOPTableR1C::GuessBase(int32_t iIdx,int32_t &iRef,int32_t &iLNAAtt,int32_t &iAtt019,double &dAtt1,double &dAtt2,double &dAtt3)
{
	if (0 == iIdx) {
		iRef = 10;
        iLNAAtt = SP1401::Rx_Att;
        iAtt019 = SP1401::Rx_Att_19;
		dAtt1 = 5.0;
		dAtt2 = 15.0;
		dAtt3 = 10.0;
	}
	else if (1 == iIdx) {
		iRef = 10;
        iLNAAtt = SP1401::Rx_Att;
        iAtt019 = SP1401::Rx_Att_0;
		dAtt1 = 25.0;
		dAtt2 = 15.0;
		dAtt3 = 10.0;
	}
	else if (2 == iIdx) {
		iRef = -20;
        iLNAAtt = SP1401::Rx_LNA;
        iAtt019 = SP1401::Rx_Att_19;
		dAtt1 = 10.0;
		dAtt2 = 15.0;
		dAtt3 = 10.0;
	}
}

void RxRefOPTableR1C::GetBase(uint64_t uiFreq,DataM *pData)
{
    uiFreq = uiFreq < RF_RX_FREQ_STAR ? RF_RX_FREQ_STAR : uiFreq;
    uiFreq = uiFreq > RF_RX_FREQ_STOP ? RF_RX_FREQ_STOP : uiFreq;

	uint64_t uiFreqL = 0,uiFreqR = 0;
    discretept(uiFreq,RF_RX_FREQ_STEP_CALLED_R1C,uiFreqL,uiFreqR);

    uint32_t uiFreqIdxL = (uint32_t)SERIE_INDEX(uiFreqL,RF_RX_FREQ_STAR,RF_RX_FREQ_STEP_CALLED_R1C);
    uint32_t uiFreqIdxR = uiFreq >= RF_RX_FREQ_STOP ? uiFreqIdxL : (uint32_t)SERIE_INDEX(uiFreqR,RF_RX_FREQ_STAR,RF_RX_FREQ_STEP_CALLED_R1C);

	DataM DataML = m_pDataM[uiFreqIdxL];
	DataM DataMR = m_pDataM[uiFreqIdxR];

    for (uint32_t i = 0;i < ARRAY_SIZE(DataML.m_State);i ++) {
		if (DataML.m_State[i].m_sLNAAtt == DataMR.m_State[i].m_sLNAAtt &&
			DataML.m_State[i].m_sAtt019 == DataMR.m_State[i].m_sAtt019 &&
			DataML.m_State[i].m_fAtt1 == DataMR.m_State[i].m_fAtt1 &&
			DataML.m_State[i].m_fAtt2 == DataMR.m_State[i].m_fAtt2 &&
			DataML.m_State[i].m_fAtt3 == DataMR.m_State[i].m_fAtt3 &&
			DataML.m_State[i].m_fTemp == DataMR.m_State[i].m_fTemp) {
				linear(uiFreqL,DataML.m_State[i].m_iADOffset,uiFreqR,DataMR.m_State[i].m_iADOffset,uiFreq,pData->m_State[i].m_iADOffset);
		}
		else
			pData->m_State[i].m_iADOffset = DataML.m_State[i].m_iADOffset;

		pData->m_State[i].m_sLNAAtt = DataML.m_State[i].m_sLNAAtt;
		pData->m_State[i].m_sAtt019 = DataML.m_State[i].m_sAtt019;
		pData->m_State[i].m_fAtt1 = DataML.m_State[i].m_fAtt1;
		pData->m_State[i].m_fAtt2 = DataML.m_State[i].m_fAtt2;
		pData->m_State[i].m_fAtt3 = DataML.m_State[i].m_fAtt3;
		pData->m_State[i].m_fTemp = DataML.m_State[i].m_fTemp;
	}
}

void RxRefOPTableR1C::GetBase(uint64_t uiFreq,DataF *pData)
{
	DataM Data;
	GetBase(uiFreq,&Data);

	uint64_t uiFreqL = 0,uiFreqR = 0;
    discretept(uiFreq,RF_RX_FREQ_STEP_CALLED_R1C,uiFreqL,uiFreqR);

    *pData = m_pDataF[(uint32_t)SERIE_INDEX(uiFreqL,RF_RX_FREQ_STAR,RF_RX_FREQ_STEP_INFILE)];
	pData->m_uiFreq = uiFreq;
    for (uint32_t i = 0;i < ARRAY_SIZE(Data.m_State);i ++)
		pData->m_State[i].m_iADOffset = (int64_t)(Data.m_State[i].m_iADOffset);
}

void RxRefOPTableR1C::Get(uint64_t uiFreq,double dRef,RxStateM *pState)
{
	DataM DataRef;

    dRef = dRef > R1C_RX_REF_OP_STAR ? R1C_RX_REF_OP_STAR : dRef;
	double dRefL = 0.0,dRefR = 0.0;

    discretept(dRef,R1C_RX_REF_STEP,dRefL,dRefR);
	GetBase(uiFreq,&DataRef);
	RxRefOPTableR1C::GetConfigTable((int32_t)dRefL,DataRef,pState);
	pState->m_iADOffset -= (int32_t)(dBc2ad(_0dBFS,dRefL - dRef)) - _0dBFS;
}

void RxRefOPTableR1C::Get(uint64_t uiFreq, double dRef, RxStateF *pState, tm *pTime)
{
	RxStateM StateM;
	DataF Data;
	Get(uiFreq,dRef,&StateM);
	GetBase(uiFreq,&Data);
	pState->m_iLNAAtt = (int32_t)StateM.m_sLNAAtt;
	pState->m_iAtt019 = (int32_t)StateM.m_sAtt019;
	pState->m_fAtt1 = StateM.m_fAtt1;
	pState->m_fAtt2 = StateM.m_fAtt2;
	pState->m_fAtt3 = StateM.m_fAtt3;
	pState->m_iADOffset = (int64_t)StateM.m_iADOffset;

	if (dRef > 10.0) {
        for (uint32_t i = 0;i < ARRAY_SIZE(pState->m_dTemp);i ++)
			pState->m_dTemp[i] = Data.m_State[0].m_dTemp[i];
	}
	else if (dRef > -20.0) {
        for (uint32_t i = 0;i < ARRAY_SIZE(pState->m_dTemp);i ++)
			pState->m_dTemp[i] = Data.m_State[1].m_dTemp[i];
	}
	else {
        for (uint32_t i = 0;i < ARRAY_SIZE(pState->m_dTemp);i ++)
			pState->m_dTemp[i] = Data.m_State[2].m_dTemp[i];
	}

	if (pTime)
		*pTime = Data.m_EndTime;
}

void RxRefOPTableR1C::SaveAs(char *pPath)
{
	FILE *fp = fopen(pPath,"w");
	if (NULL == fp)
		return;

	double dRef = 10.0;
	int64_t iADOffset = 0;
	RxRefOPTableR1C::RxStateF State;
    tm EndTime;

    for (uint64_t uiFreq = RF_RX_FREQ_STAR;uiFreq <= RF_RX_FREQ_STOP;uiFreq += RF_RX_FREQ_STEP_INFILE) {
        for (dRef = R1C_RX_REF_OP_STAR;dRef >= R1C_RX_REF_OP_STOP;dRef += R1C_RX_REF_STEP) {
			Get(uiFreq,dRef,&State,&EndTime);
            fprintf(fp,"freq:%10I64u \t ref:%4.1f \t ad_offset:%I64i \t lna_att:%s \t att0_19:%s att1:%3.1f \t att2:%3.1f \t att3:%3.1f \t temp0:%-.3f temp1:%-.3f temp2:%-.3f temp3:%-.3f time:%4d/%02d/%02d %02d:%02d:%02d\n",
				uiFreq,dRef,State.m_iADOffset,State.m_iLNAAtt ? "lna" : "att",State.m_iAtt019 ? "att0" : "att 19",State.m_fAtt1,State.m_fAtt2,State.m_fAtt3,State.m_dTemp[0],State.m_dTemp[1],State.m_dTemp[2],State.m_dTemp[3],
                EndTime.tm_year + 1900,EndTime.tm_mon,EndTime.tm_mday,EndTime.tm_hour,EndTime.tm_min,EndTime.tm_sec);
        }
	}
    fclose(fp);
}

//r1c io mode
IMPLEMENT_R1C_RX_CAL_ITEM_TABLE(RxRefIOTableR1C)

RxRefIOTableR1C::DataF::DataF()
{
	m_uiFreq = 0;
    for (uint32_t i = 0;i < ARRAY_SIZE(m_State);i ++) {
		m_State[i].m_iLNAAtt = 0;
		m_State[i].m_iAtt019 = 1;
		m_State[i].m_iADOffset = 0;
		m_State[i].m_fAtt1 = 0.0;
		m_State[i].m_fAtt2 = 0.0;
		m_State[i].m_fAtt3 = 0.0;
        for (uint32_t j = 0;j < ARRAY_SIZE(m_State[i].m_dTemp);j ++)
			m_State[i].m_dTemp[j] = 0.0;
	}
	m_EndTime.tm_sec = 0;
	m_EndTime.tm_min = 0;
	m_EndTime.tm_hour = 0;
	m_EndTime.tm_mday = 0;
	m_EndTime.tm_mon = 0;
	m_EndTime.tm_year = 0;
	m_EndTime.tm_wday = 0;
	m_EndTime.tm_yday = 0;
	m_EndTime.tm_isdst = 0;
}

void RxRefIOTableR1C::Map2Mem()
{
	if (NULL == m_pDataF)
		return;
	uint32_t uiIdxF = 0,uiIdxM = 0;
	uint64_t uiFreq = 0;
    for (uiIdxM = 0;uiIdxM < RF_RX_FREQ_PTS_CALLED_R1C;uiIdxM ++) {
        uiFreq = (uint64_t)RF_RX_FREQ_STAR + uiIdxM * (uint64_t)RF_RX_FREQ_STEP_CALLED_R1C;
        uiIdxF = (uint32_t)SERIE_INDEX(uiFreq,RF_RX_FREQ_STAR,RF_RX_FREQ_STEP_INFILE);
        for (uint32_t i = 0;i < ARRAY_SIZE(m_pDataM[uiIdxM].m_State);i ++) {
			m_pDataM[uiIdxM].m_State[i].m_fAtt1	= m_pDataF[uiIdxF].m_State[i].m_fAtt1;
			m_pDataM[uiIdxM].m_State[i].m_fAtt2 = m_pDataF[uiIdxF].m_State[i].m_fAtt2;
			m_pDataM[uiIdxM].m_State[i].m_fAtt3 = m_pDataF[uiIdxF].m_State[i].m_fAtt3;
			m_pDataM[uiIdxM].m_State[i].m_fTemp = (float)(m_pDataF[uiIdxF].m_State[i].m_dTemp[0]);
			m_pDataM[uiIdxM].m_State[i].m_iADOffset = (int32_t)(m_pDataF[uiIdxF].m_State[i].m_iADOffset);
			m_pDataM[uiIdxM].m_State[i].m_sLNAAtt = (short)(m_pDataF[uiIdxF].m_State[i].m_iLNAAtt);
			m_pDataM[uiIdxM].m_State[i].m_sAtt019 = (short)(m_pDataF[uiIdxF].m_State[i].m_iAtt019);
		}
	}
}

void RxRefIOTableR1C::GetConfigTable(int32_t iRef,DataM DataRef,RxRefOPTableR1C::RxStateM *pState)
{
	//	ref		att0	att1	att2	att3
	//	 30		 -20	 -20	 -13	 -10
	//	 20		 -20	 -10	 -13	 -10
	//	 10		 -20	  -0	 -13	 -10////m_State[0]
	//
	//	 10		   0	 -20	 -13	 -10////m_State[1]
	//	  0		   0	 -10	 -13	 -10
	//	-10		   0	   0	 -13	 -10
	//	-20		   0	   0	  -3	 -10

	//	-20		  14	  -4	 -13	 -10////m_State[2]
	//	-24		  14	   0	 -13	 -10
	//	-30		  14	   0	  -7	 -10

	short sLNAAtt = 0;
	short sAtt019 = 0;
	float fAtt1 = 0.0,fAtt2 = 0.0,fAtt3 = 0.0;
	int32_t iADOffset = 0;
	float fTemp = 0.0;

	if (iRef > 10) {
		sLNAAtt = DataRef.m_State[0].m_sLNAAtt;
		sAtt019 = DataRef.m_State[0].m_sAtt019;
		fAtt1 = DataRef.m_State[0].m_fAtt1 + (float)(iRef - 10);
		fAtt2 = DataRef.m_State[0].m_fAtt2;
		fAtt3 = DataRef.m_State[0].m_fAtt3;
		iADOffset = DataRef.m_State[0].m_iADOffset;
		fTemp = DataRef.m_State[0].m_fTemp;
	}
	else if (iRef >= -10) {
		sLNAAtt = DataRef.m_State[1].m_sLNAAtt;
		sAtt019 = DataRef.m_State[1].m_sAtt019;
		fAtt1 = DataRef.m_State[1].m_fAtt1 + (float)(iRef - 10);
		fAtt2 = DataRef.m_State[1].m_fAtt2;
		fAtt3 = DataRef.m_State[1].m_fAtt3;
		iADOffset = DataRef.m_State[1].m_iADOffset;
		fTemp = DataRef.m_State[1].m_fTemp;
	}
	else if (iRef > -20) {
		sLNAAtt = DataRef.m_State[1].m_sLNAAtt;
		sAtt019 = DataRef.m_State[1].m_sAtt019;
		fAtt1 = DataRef.m_State[1].m_fAtt1 - 20.0f;
		fAtt2 = DataRef.m_State[1].m_fAtt2 + (float)(iRef + 10);
		fAtt3 = DataRef.m_State[1].m_fAtt3;
		iADOffset = DataRef.m_State[1].m_iADOffset;
		fTemp = DataRef.m_State[1].m_fTemp;
	}
	else if (iRef >= -24) {
		sLNAAtt = DataRef.m_State[2].m_sLNAAtt;
		sAtt019 = DataRef.m_State[2].m_sAtt019;
		fAtt1 = DataRef.m_State[2].m_fAtt1 + (float)(iRef + 20);
		fAtt2 = DataRef.m_State[2].m_fAtt2;
		fAtt3 = DataRef.m_State[2].m_fAtt3;
		iADOffset = DataRef.m_State[2].m_iADOffset;
		fTemp = DataRef.m_State[2].m_fTemp;
	}
	else if (iRef >= -30) {
		sLNAAtt = DataRef.m_State[2].m_sLNAAtt;
		sAtt019 = DataRef.m_State[2].m_sAtt019;
		fAtt1 = DataRef.m_State[2].m_fAtt1 - 4.0f;
		fAtt2 = DataRef.m_State[2].m_fAtt2 + (float)(iRef + 24);
		fAtt3 = DataRef.m_State[2].m_fAtt3;
		iADOffset = DataRef.m_State[2].m_iADOffset;
		fTemp = DataRef.m_State[2].m_fTemp;
	}
	if (fAtt1 < 0) {
		iADOffset += (int32_t)(_0dBFS - dBc2ad(_0dBFS,-fAtt1));
		fAtt1 = 0;
	}
	if (fAtt2 < 0) {
		iADOffset += (int32_t)(_0dBFS - dBc2ad(_0dBFS,-fAtt2));
		fAtt2 = 0;
	}

	pState->m_sLNAAtt = sLNAAtt;
	pState->m_sAtt019 = sAtt019;
	pState->m_fAtt1 = fAtt1;
	pState->m_fAtt2 = fAtt2;
	pState->m_fAtt3 = fAtt3;
	pState->m_iADOffset = iADOffset;
	pState->m_fTemp = fTemp;
}

void RxRefIOTableR1C::GuessBase(int32_t iIdx,int32_t &iRef,int32_t &iLNAAtt,int32_t &iAtt019,double &dAtt1,double &dAtt2,double &dAtt3)
{
	if (0 == iIdx) {
		iRef = 10;
        iLNAAtt = SP1401::Rx_Att;
        iAtt019 = SP1401::Rx_Att_19;
		dAtt1 = 0.0;
		dAtt2 = 13.0;
		dAtt3 = 10.0;
	}
	else if (1 == iIdx) {
		iRef = 10;
        iLNAAtt = SP1401::Rx_Att;
        iAtt019 = SP1401::Rx_Att_0;
		dAtt1 = 20.0;
		dAtt2 = 13.0;
		dAtt3 = 10.0;
	}
	else if (2 == iIdx) {
		iRef = -20;
        iLNAAtt = SP1401::Rx_LNA;
        iAtt019 = SP1401::Rx_Att_19;
		dAtt1 = 4.0;
		dAtt2 = 13.0;
		dAtt3 = 10.0;
	}
}

int32_t RxRefIOTableR1C::GetRefSegIdx(double dRef)
{
	if (dRef > 10.0)
		return 0;
	else if (dRef > -20.0)
		return 1;
	else
		return 2;
}

void RxRefIOTableR1C::GetBase(uint64_t uiFreq,DataM *pData)
{
    uiFreq = uiFreq < RF_RX_FREQ_STAR ? RF_RX_FREQ_STAR : uiFreq;
    uiFreq = uiFreq > RF_RX_FREQ_STOP ? RF_RX_FREQ_STOP : uiFreq;

	uint64_t uiFreqL = 0,uiFreqR = 0;
    discretept(uiFreq,RF_RX_FREQ_STEP_CALLED_R1C,uiFreqL,uiFreqR);

    uint32_t uiFreqIdxL = (uint32_t)SERIE_INDEX(uiFreqL,RF_RX_FREQ_STAR,RF_RX_FREQ_STEP_CALLED_R1C);
    uint32_t uiFreqIdxR = uiFreq >= RF_RX_FREQ_STOP ? uiFreqIdxL : (uint32_t)SERIE_INDEX(uiFreqR,RF_RX_FREQ_STAR,RF_RX_FREQ_STEP_CALLED_R1C);

	DataM DataML = m_pDataM[uiFreqIdxL];
	DataM DataMR = m_pDataM[uiFreqIdxR];

    for (uint32_t i = 0;i < ARRAY_SIZE(DataML.m_State);i ++) {
		if (DataML.m_State[i].m_sLNAAtt == DataMR.m_State[i].m_sLNAAtt &&
			DataML.m_State[i].m_sAtt019 == DataMR.m_State[i].m_sAtt019 &&
			DataML.m_State[i].m_fAtt1 == DataMR.m_State[i].m_fAtt1 &&
			DataML.m_State[i].m_fAtt2 == DataMR.m_State[i].m_fAtt2 &&
			DataML.m_State[i].m_fAtt3 == DataMR.m_State[i].m_fAtt3 &&
			DataML.m_State[i].m_fTemp == DataMR.m_State[i].m_fTemp) {
				linear(uiFreqL,DataML.m_State[i].m_iADOffset,uiFreqR,DataMR.m_State[i].m_iADOffset,uiFreq,pData->m_State[i].m_iADOffset);
		}
		else
			pData->m_State[i].m_iADOffset = DataML.m_State[i].m_iADOffset;

		pData->m_State[i].m_sLNAAtt = DataML.m_State[i].m_sLNAAtt;
		pData->m_State[i].m_sAtt019 = DataML.m_State[i].m_sAtt019;
		pData->m_State[i].m_fAtt1 = DataML.m_State[i].m_fAtt1;
		pData->m_State[i].m_fAtt2 = DataML.m_State[i].m_fAtt2;
		pData->m_State[i].m_fAtt3 = DataML.m_State[i].m_fAtt3;
		pData->m_State[i].m_fTemp = DataML.m_State[i].m_fTemp;
	}
}

void RxRefIOTableR1C::GetBase(uint64_t uiFreq,DataF *pData)
{
	DataM Data;
	GetBase(uiFreq,&Data);

	uint64_t uiFreqL = 0,uiFreqR = 0;
    discretept(uiFreq,RF_RX_FREQ_STEP_CALLED_R1C,uiFreqL,uiFreqR);

    *pData = m_pDataF[(uint32_t)SERIE_INDEX(uiFreqL,RF_RX_FREQ_STAR,RF_RX_FREQ_STEP_INFILE)];
	pData->m_uiFreq = uiFreq;
    for (uint32_t i = 0;i < ARRAY_SIZE(Data.m_State);i ++)
		pData->m_State[i].m_iADOffset = (int64_t)(Data.m_State[i].m_iADOffset);
}

void RxRefIOTableR1C::Get(uint64_t uiFreq,double dRef,RxRefOPTableR1C::RxStateM *pState)
{
	DataM DataRef;

    dRef = dRef > R1C_RX_REF_IO_STAR ? R1C_RX_REF_IO_STAR : dRef;
	double dRefL = 0.0,dRefR = 0.0;

    discretept(dRef,R1C_RX_REF_STEP,dRefL,dRefR);
	GetBase(uiFreq,&DataRef);
	RxRefIOTableR1C::GetConfigTable((int32_t)dRefL,DataRef,pState);
	pState->m_iADOffset += (int32_t)(dBc2ad(_0dBFS,dRefL - dRef)) - _0dBFS;
}

void RxRefIOTableR1C::Get(uint64_t uiFreq,double dRef,RxRefOPTableR1C::RxStateF *pState,tm *pTime)
{
	RxRefOPTableR1C::RxStateM StateM;
	DataF Data;
	Get(uiFreq,dRef,&StateM);
	GetBase(uiFreq,&Data);
	pState->m_iLNAAtt = (int32_t)StateM.m_sLNAAtt;
	pState->m_iAtt019 = (int32_t)StateM.m_sAtt019;
	pState->m_fAtt1 = StateM.m_fAtt1;
	pState->m_fAtt2 = StateM.m_fAtt2;
	pState->m_fAtt3 = StateM.m_fAtt3;
	pState->m_iADOffset = (int64_t)StateM.m_iADOffset;

	if (dRef > 10.0) {
        for (uint32_t i = 0;i < ARRAY_SIZE(pState->m_dTemp);i ++)
			pState->m_dTemp[i] = Data.m_State[0].m_dTemp[i];
	}
	else if (dRef > -20.0) {
        for (uint32_t i = 0;i < ARRAY_SIZE(pState->m_dTemp);i ++)
			pState->m_dTemp[i] = Data.m_State[1].m_dTemp[i];
	}
	else {
        for (uint32_t i = 0;i < ARRAY_SIZE(pState->m_dTemp);i ++)
			pState->m_dTemp[i] = Data.m_State[2].m_dTemp[i];
	}

	if (pTime)
		*pTime = Data.m_EndTime;
}

void RxRefIOTableR1C::SaveAs(char *pPath)
{
	FILE *fp = fopen(pPath,"w");
	if (NULL == fp)
		return;

	double dRef = 10.0;
	int64_t iADOffset = 0;
	RxRefOPTableR1C::RxStateF State;
    tm EndTime;

    for (uint64_t uiFreq = RF_RX_FREQ_STAR;uiFreq <= RF_RX_FREQ_STOP;uiFreq += RF_RX_FREQ_STEP_INFILE) {
        for (dRef = R1C_RX_REF_IO_STAR;dRef >= R1C_RX_REF_IO_STOP;dRef += R1C_RX_REF_STEP) {
			Get(uiFreq,dRef,&State,&EndTime);
            fprintf(fp,"freq:%10I64u \t ref:%4.1f \t ad_offset:%I64i \t lna_att:%s \t att0_19:%s att1:%3.1f \t att2:%3.1f \t att3:%3.1f \t temp0:%-.3f temp1:%-.3f temp2:%-.3f temp3:%-.3f time:%4d/%02d/%02d %02d:%02d:%02d\n",
				uiFreq,dRef,State.m_iADOffset,State.m_iLNAAtt ? "lna" : "att",State.m_iAtt019 ? "att0" : "att 19",State.m_fAtt1,State.m_fAtt2,State.m_fAtt3,State.m_dTemp[0],State.m_dTemp[1],State.m_dTemp[2],State.m_dTemp[3],
                EndTime.tm_year + 1900,EndTime.tm_mon,EndTime.tm_mday,EndTime.tm_hour,EndTime.tm_min,EndTime.tm_sec);
		}
	}
	fclose(fp);
}
