#include "RxAttTable.h"

//r1c output mode
IMPLEMENT_R1C_RX_CAL_ITEM_TABLE(RxAttOPTableR1C)

RxAttOPTableR1C::DataF::DataF()
{
	m_uiFreq = 0;
    for (uint32_t i = 0;i < ARRAY_SIZE(m_iOffset);i ++)
		m_iOffset[i] = 0;
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

void RxAttOPTableR1C::Map2Mem()
{
	if (NULL == m_pDataF)
		return;
	uint32_t uiIdxF = 0,uiIdxM = 0;
	uint64_t uiFreq = 0;
    for (uiIdxM = 0;uiIdxM < RF_RX_FREQ_PTS_CALLED_R1C;uiIdxM ++) {
        uiFreq = (uint64_t)RF_RX_FREQ_STAR + uiIdxM * (uint64_t)RF_RX_FREQ_STEP_CALLED_R1C;
        uiIdxF = (uint32_t)SERIE_INDEX(uiFreq,RF_RX_FREQ_STAR,RF_RX_FREQ_STEP_INFILE);
        for (uint32_t i = 0;i < ARRAY_SIZE(m_pDataM[0].m_iOffset);i ++) {
			m_pDataM[uiIdxM].m_iOffset[i] = (int32_t)(m_pDataF[uiIdxF].m_iOffset[i]);
			m_pDataM[uiIdxM].m_fTemp = (float)(m_pDataF[uiIdxF].m_dTemp[0]);
		}
	}
}

int32_t RxAttOPTableR1C::Get(uint64_t uiFreq,double dRef)
{
	DataM Data;
	Get(uiFreq,&Data);
	int32_t iRef = dRef >= 0 ? (int32_t)dRef + 1 : (int32_t)dRef;
    iRef = iRef < R1C_RX_REF_OP_STOP ? R1C_RX_REF_OP_STOP : iRef;
    int32_t iIdxRef = SERIE_INDEX(iRef,R1C_RX_REF_OP_STAR,R1C_RX_REF_STEP);
	if (10 < iRef)
		return Data.m_iOffset[iIdxRef];
	else if (10 == iRef || -20 == iRef)
		return 0;
	else if (-20 < iRef)
		return Data.m_iOffset[iIdxRef - 1];
	else
		return Data.m_iOffset[iIdxRef - 2];
}

void RxAttOPTableR1C::Get(uint64_t uiFreq,DataM *pData)
{
	uint64_t uiFreqL = 0,uiFreqR = 0;
    discretept(uiFreq,RF_RX_FREQ_STEP_CALLED_R1C,uiFreqL,uiFreqR);

    uint32_t uiIdxL = (uint32_t)SERIE_INDEX(uiFreqL,RF_RX_FREQ_STAR,RF_RX_FREQ_STEP_CALLED_R1C);
	*pData = m_pDataM[uiIdxL];
}

void RxAttOPTableR1C::Get(uint64_t uiFreq,DataF *pData)
{
	DataM Data;
	Get(uiFreq,&Data);

	uint64_t uiFreqL = 0,uiFreqR = 0;
    discretept(uiFreq,RF_RX_FREQ_STEP_CALLED_R1C,uiFreqL,uiFreqR);

    *pData = m_pDataF[(uint32_t)SERIE_INDEX(uiFreqL,RF_RX_FREQ_STAR,RF_RX_FREQ_STEP_INFILE)];
	pData->m_uiFreq = uiFreq;
    for (uint32_t i = 0;i < ARRAY_SIZE((*pData).m_iOffset);i ++)
		pData->m_iOffset[i] = Data.m_iOffset[i];
}

void RxAttOPTableR1C::SaveAs(char *pPath)
{
	FILE *fp = fopen(pPath,"w");
	if (fp == NULL)
		return;
// 	float fAtt[9];
// 	memset(fAtt,0,ARRAY_SIZE(fAtt));
// 
// 	fprintf(fp,"freq \t att1_10 \t att1_20 \t att1_30 \t att2_10 \t att2_20 \t att2_30 \t att3_10 \t att3_20 \t att3_30\n");
// 	for (uint64_t uiFreq = CAL_RF_TX_FREQ_STAR;uiFreq <= CAL_RF_TX_FREQ_STOP;uiFreq += CAL_RF_TX_FREQ_STEP_INFILE) {
// 		Get(uiFreq,fAtt);
// 		fprintf(fp,"freq : %llu \t %f \t %f \t %f \t %f \t %f \t %f \t %f \t %f \t %f \n",
// 					uiFreq,fAtt[0],fAtt[1],fAtt[2],fAtt[3],fAtt[4],fAtt[5],fAtt[6],fAtt[7],fAtt[8]);
// 	}
	fclose(fp);
}

//r1c io mode
IMPLEMENT_R1C_RX_CAL_ITEM_TABLE(RxAttIOTableR1C)

RxAttIOTableR1C::DataF::DataF()
{
	m_uiFreq = 0;
    for (uint32_t i = 0;i < ARRAY_SIZE(m_iOffset);i ++)
		m_iOffset[i] = 0;
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

void RxAttIOTableR1C::Map2Mem()
{
	if (NULL == m_pDataF)
		return;
	uint32_t uiIdxF = 0,uiIdxM = 0;
	uint64_t uiFreq = 0;
    for (uiIdxM = 0;uiIdxM < RF_RX_FREQ_PTS_CALLED_R1C;uiIdxM ++) {
        uiFreq = (uint64_t)RF_RX_FREQ_STAR + uiIdxM * (uint64_t)RF_RX_FREQ_STEP_CALLED_R1C;
        uiIdxF = (uint32_t)SERIE_INDEX(uiFreq,RF_RX_FREQ_STAR,RF_RX_FREQ_STEP_INFILE);
        for (uint32_t i = 0;i < ARRAY_SIZE(m_pDataM[0].m_iOffset);i ++) {
			m_pDataM[uiIdxM].m_iOffset[i] = (int32_t)(m_pDataF[uiIdxF].m_iOffset[i]);
			m_pDataM[uiIdxM].m_fTemp = (float)(m_pDataF[uiIdxF].m_dTemp[0]);
		}
	}
}

int32_t RxAttIOTableR1C::Get(uint64_t uiFreq,double dRef)
{
	DataM Data;
	Get(uiFreq,&Data);
	int32_t iRef = dRef >= 0 ? (int32_t)dRef + 1 : (int32_t)dRef;
    iRef = iRef < R1C_RX_REF_IO_STOP ? R1C_RX_REF_IO_STOP : iRef;
    int32_t iIdxRef = SERIE_INDEX(iRef,R1C_RX_REF_IO_STAR,R1C_RX_REF_STEP);
	if (10 < iRef)
		return Data.m_iOffset[iIdxRef];
	else if (10 == iRef || -20 == iRef)
		return 0;
	else if (-20 < iRef)
		return Data.m_iOffset[iIdxRef - 1];
	else
		return Data.m_iOffset[iIdxRef - 2];
}

void RxAttIOTableR1C::Get(uint64_t uiFreq,DataM *pData)
{
	uint64_t uiFreqL = 0,uiFreqR = 0;
    discretept(uiFreq,RF_RX_FREQ_STEP_CALLED_R1C,uiFreqL,uiFreqR);

    uint32_t uiIdxL = (uint32_t)SERIE_INDEX(uiFreqL,RF_RX_FREQ_STAR,RF_RX_FREQ_STEP_CALLED_R1C);
	*pData = m_pDataM[uiIdxL];
}

void RxAttIOTableR1C::Get(uint64_t uiFreq,DataF *pData)
{
	DataM Data;
	Get(uiFreq,&Data);

	uint64_t uiFreqL = 0,uiFreqR = 0;
    discretept(uiFreq,RF_RX_FREQ_STEP_CALLED_R1C,uiFreqL,uiFreqR);

    *pData = m_pDataF[(uint32_t)SERIE_INDEX(uiFreqL,RF_RX_FREQ_STAR,RF_RX_FREQ_STEP_INFILE)];
	pData->m_uiFreq = uiFreq;
    for (uint32_t i = 0;i < ARRAY_SIZE((*pData).m_iOffset);i ++)
		pData->m_iOffset[i] = Data.m_iOffset[i];
}

void RxAttIOTableR1C::SaveAs(char *pPath)
{
	FILE *fp = fopen(pPath,"w");
	if (fp == NULL)
		return;
	// 	float fAtt[9];
	// 	memset(fAtt,0,ARRAY_SIZE(fAtt));
	// 
	// 	fprintf(fp,"freq \t att1_10 \t att1_20 \t att1_30 \t att2_10 \t att2_20 \t att2_30 \t att3_10 \t att3_20 \t att3_30\n");
	// 	for (uint64_t uiFreq = CAL_RF_TX_FREQ_STAR;uiFreq <= CAL_RF_TX_FREQ_STOP;uiFreq += CAL_RF_TX_FREQ_STEP_INFILE) {
	// 		Get(uiFreq,fAtt);
	// 		fprintf(fp,"freq : %llu \t %f \t %f \t %f \t %f \t %f \t %f \t %f \t %f \t %f \n",
	// 					uiFreq,fAtt[0],fAtt[1],fAtt[2],fAtt[3],fAtt[4],fAtt[5],fAtt[6],fAtt[7],fAtt[8]);
	// 	}
	fclose(fp);
}
