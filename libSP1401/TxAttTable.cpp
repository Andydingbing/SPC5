#include "TxAttTable.h"
#include <stdlib.h>
#include <math.h>
#include <string.h>

IMPLEMENT_TX_CAL_ITEM_TABLE(TxAttTableR1A)

void TxAttTableR1A::Map2Mem()
{
	if (NULL == m_pDataF)
		return;
	uint32_t uiIdxF = 0,uiIdxM = 0;
	uint64_t uiFreq = 0;
    for (uiIdxM = 0;uiIdxM < RF_TX_FREQ_PTS_CALLED;uiIdxM ++) {
        uiFreq = (uint64_t)RF_TX_FREQ_STAR + uiIdxM * (uint64_t)RF_TX_FREQ_STEP_CALLED;
        uiIdxF = (uint32_t)SERIE_INDEX(uiFreq,RF_TX_FREQ_STAR,RF_TX_FREQ_STEP_INFILE);
		m_pDataM[uiIdxM].m_fAtt[0] = m_pDataF[uiIdxF].m_fAtt10;
		m_pDataM[uiIdxM].m_fAtt[1] = m_pDataF[uiIdxF].m_fAtt20;
		m_pDataM[uiIdxM].m_fAtt[2] = m_pDataF[uiIdxF].m_fAtt30;
		m_pDataM[uiIdxM].m_fAtt[3] = m_pDataF[uiIdxF].m_fAtt40;
		m_pDataM[uiIdxM].m_fAtt[4] = m_pDataF[uiIdxF].m_fAtt50;
		m_pDataM[uiIdxM].m_fAtt[5] = m_pDataF[uiIdxF].m_fAtt60;
		m_pDataM[uiIdxM].m_fAtt[6] = m_pDataF[uiIdxF].m_fAtt70;
		m_pDataM[uiIdxM].m_fAtt[7] = m_pDataF[uiIdxF].m_fAtt80;
		m_pDataM[uiIdxM].m_fAtt[8] = m_pDataF[uiIdxF].m_fAtt90;
	}
}

void TxAttTableR1A::Get(uint64_t uiFreq,float *pAtt)
{
	uint64_t uiFreqL = 0,uiFreqR = 0;
    discretept(uiFreq,RF_TX_FREQ_STEP_CALLED,uiFreqL,uiFreqR);

    uint32_t uiIdxL = (uint32_t)SERIE_INDEX(uiFreqL,RF_TX_FREQ_STAR,RF_TX_FREQ_STEP_CALLED);
    uint32_t uiIdxR = (uint32_t)SERIE_INDEX(uiFreqR,RF_TX_FREQ_STAR,RF_TX_FREQ_STEP_CALLED);
	DataM DataL = m_pDataM[uiIdxL];
	DataM DataR = m_pDataM[uiIdxR];

    for (uint32_t i = 0;i < sizeof(DataM) / sizeof(float);i ++)
		linear(uiFreqL,DataL.m_fAtt[i],uiFreqR,DataR.m_fAtt[i],uiFreq,pAtt[i]);
}

void TxAttTableR1A::Get(uint64_t uiFreq,int32_t iAtt,double &dOffset)
{
	dOffset = 0;
	float fAttOffset[9];
    for (uint32_t i = 0;i < ARRAY_SIZE(fAttOffset);i ++)
		fAttOffset[i] = 0.0;

	Get(uiFreq,fAttOffset);

    for (int32_t i = 0;i < (int32_t)(abs(iAtt) / R1A_TX_ATT_STEP_MIN);i ++)
		dOffset += (double)(fAttOffset[i]);
	dOffset = round(dOffset,3);
}

void TxAttTableR1A::SaveAs(char *pPath)
{
	FILE *fp = fopen(pPath,"w");
	if (fp == NULL)
		return;
	float fAtt[9];
	memset(fAtt,0,ARRAY_SIZE(fAtt));

	fprintf(fp,"freq \t att1_10 \t att1_20 \t att1_30 \t att2_10 \t att2_20 \t att2_30 \t att3_10 \t att3_20 \t att3_30\n");
    for (uint64_t uiFreq = RF_TX_FREQ_STAR;uiFreq <= RF_TX_FREQ_STOP;uiFreq += RF_TX_FREQ_STEP_INFILE) {
		Get(uiFreq,fAtt);
        fprintf(fp,"freq : %I64u \t %f \t %f \t %f \t %f \t %f \t %f \t %f \t %f \t %f \n",
					uiFreq,fAtt[0],fAtt[1],fAtt[2],fAtt[3],fAtt[4],fAtt[5],fAtt[6],fAtt[7],fAtt[8]);
	}
	fclose(fp);
}

//r1c output mode
IMPLEMENT_TX_CAL_ITEM_TABLE(TxAttOPTableR1C)

TxAttOPTableR1C::DataF::DataF()
{
	m_uiFreq = 0;
    for (uint32_t i = 0;i < ARRAY_SIZE(m_fOffset);i ++)
		m_fOffset[i] = 0.0;
    for (uint32_t i = 0;i < ARRAY_SIZE(m_dTemp);i ++)
		m_dTemp[i] = 0.0;
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

void TxAttOPTableR1C::Map2Mem()
{
	if (NULL == m_pDataF)
		return;
	uint32_t uiIdxF = 0,uiIdxM = 0;
	uint64_t uiFreq = 0;
    for (uiIdxM = 0;uiIdxM < RF_TX_FREQ_PTS_CALLED;uiIdxM ++) {
        uiFreq = (uint64_t)RF_TX_FREQ_STAR + uiIdxM * (uint64_t)RF_TX_FREQ_STEP_CALLED;
        uiIdxF = (uint32_t)SERIE_INDEX(uiFreq,RF_TX_FREQ_STAR,RF_TX_FREQ_STEP_INFILE);
        for (uint32_t i = 0;i < ARRAY_SIZE(m_pDataM[0].m_fOffset);i ++)
			m_pDataM[uiIdxM].m_fOffset[i] = m_pDataF[uiIdxF].m_fOffset[i];
		m_pDataM[uiIdxM].m_fTemp5 = (float)(m_pDataF[uiIdxF].m_dTemp[1]);
	}
}

float TxAttOPTableR1C::Get(uint64_t uiFreq,double dPower)
{
	DataM Data;
	Get(uiFreq,&Data);
	int32_t iPower = dPower >= 0 ? (int32_t)dPower + 1 : (int32_t)dPower;
    iPower = iPower > R1C_TX_ATT_OP_POWER_STAR ? R1C_TX_ATT_OP_POWER_STAR : iPower;
    iPower = iPower < R1C_TX_ATT_OP_POWER_STOP ? R1C_TX_ATT_OP_POWER_STOP : iPower;
    int32_t iIdxPower = SERIE_INDEX(iPower,R1C_TX_ATT_OP_POWER_STAR,R1C_TX_ATT_STEP);
    if (R1C_TX_BASE_POWER_OP < iPower)
		return Data.m_fOffset[iIdxPower];
    else if (R1C_TX_BASE_POWER_OP == iPower)
		return 0.0;
	else
		return Data.m_fOffset[iIdxPower -1];
}

void TxAttOPTableR1C::Get(uint64_t uiFreq,DataM *pData)
{
	uint64_t uiFreqL = 0,uiFreqR = 0;
    discretept(uiFreq,RF_TX_FREQ_STEP_CALLED,uiFreqL,uiFreqR);

    uint32_t uiIdxL = (uint32_t)SERIE_INDEX(uiFreqL,RF_TX_FREQ_STAR,RF_TX_FREQ_STEP_CALLED);
	*pData = m_pDataM[uiIdxL];
}

void TxAttOPTableR1C::Get(uint64_t uiFreq,DataF *pData)
{
	DataM Data;
	Get(uiFreq,&Data);

	uint64_t uiFreqL = 0,uiFreqR = 0;
    discretept(uiFreq,RF_TX_FREQ_STEP_CALLED,uiFreqL,uiFreqR);

    *pData = m_pDataF[(uint32_t)SERIE_INDEX(uiFreqL,RF_TX_FREQ_STAR,RF_TX_FREQ_STEP_INFILE)];
	pData->m_uiFreq = uiFreq;
    for (uint32_t i = 0;i < ARRAY_SIZE((*pData).m_fOffset);i ++)
		pData->m_fOffset[i] = Data.m_fOffset[i];
}

void TxAttOPTableR1C::SaveAs(char *pPath)
{

}

//r1c io mode
IMPLEMENT_TX_CAL_ITEM_TABLE(TxAttIOTableR1C)

TxAttIOTableR1C::DataF::DataF()
{
	m_uiFreq = 0;
    for (uint32_t i = 0;i < ARRAY_SIZE(m_fOffset);i ++)
		m_fOffset[i] = 0.0;
    for (uint32_t i = 0;i < ARRAY_SIZE(m_dTemp);i ++)
		m_dTemp[i] = 0.0;
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

void TxAttIOTableR1C::Map2Mem()
{
	if (NULL == m_pDataF)
		return;
	uint32_t uiIdxF = 0,uiIdxM = 0;
	uint64_t uiFreq = 0;
    for (uiIdxM = 0;uiIdxM < RF_TX_FREQ_PTS_CALLED;uiIdxM ++) {
        uiFreq = (uint64_t)RF_TX_FREQ_STAR + uiIdxM * (uint64_t)RF_TX_FREQ_STEP_CALLED;
        uiIdxF = (uint32_t)SERIE_INDEX(uiFreq,RF_TX_FREQ_STAR,RF_TX_FREQ_STEP_INFILE);
        for (uint32_t i = 0;i < ARRAY_SIZE(m_pDataM[0].m_fOffset);i ++)
			m_pDataM[uiIdxM].m_fOffset[i] = m_pDataF[uiIdxF].m_fOffset[i];
		m_pDataM[uiIdxM].m_fTemp5 = (float)(m_pDataF[uiIdxF].m_dTemp[1]);
	}
}

float TxAttIOTableR1C::Get(uint64_t uiFreq,double dPower)
{
	DataM Data;
	Get(uiFreq,&Data);
	int32_t iPower = dPower >= 0 ? (int32_t)dPower + 1 : (int32_t)dPower;
    iPower = iPower > R1C_TX_ATT_IO_POWER_STAR ? R1C_TX_ATT_IO_POWER_STAR : iPower;
    iPower = iPower < R1C_TX_ATT_IO_POWER_STOP ? R1C_TX_ATT_IO_POWER_STOP : iPower;
    int32_t iIdxPower = SERIE_INDEX(iPower,R1C_TX_ATT_IO_POWER_STAR,R1C_TX_ATT_STEP);
    if (R1C_TX_BASE_POWER_IO < iPower)
		return Data.m_fOffset[iIdxPower];
    else if (R1C_TX_BASE_POWER_IO == iPower)
		return 0.0;
	else
		return Data.m_fOffset[iIdxPower -1];
}

void TxAttIOTableR1C::Get(uint64_t uiFreq,DataM *pData)
{
	uint64_t uiFreqL = 0,uiFreqR = 0;
    discretept(uiFreq,RF_TX_FREQ_STEP_CALLED,uiFreqL,uiFreqR);

    uint32_t uiIdxL = (uint32_t)SERIE_INDEX(uiFreqL,RF_TX_FREQ_STAR,RF_TX_FREQ_STEP_CALLED);
	*pData = m_pDataM[uiIdxL];
}

void TxAttIOTableR1C::Get(uint64_t uiFreq,DataF *pData)
{
	DataM Data;
	Get(uiFreq,&Data);

	uint64_t uiFreqL = 0,uiFreqR = 0;
    discretept(uiFreq,RF_TX_FREQ_STEP_CALLED,uiFreqL,uiFreqR);

    *pData = m_pDataF[(uint32_t)SERIE_INDEX(uiFreqL,RF_TX_FREQ_STAR,RF_TX_FREQ_STEP_INFILE)];
	pData->m_uiFreq = uiFreq;
    for (uint32_t i = 0;i < ARRAY_SIZE((*pData).m_fOffset);i ++)
		pData->m_fOffset[i] = Data.m_fOffset[i];
}

void TxAttIOTableR1C::SaveAs(char *pPath)
{

}
