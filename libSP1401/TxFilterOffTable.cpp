#include "TxFilterOffTable.h"

IMPLEMENT_TX_CAL_ITEM_TABLE(TxFilterOffTableR1C)

TxFilterOffTableR1C::DataF::DataF()
{
	m_uiFreq = 0;
	m_dOffset = 0.0;
    for (uint32_t i = 0;i < ARRAY_SIZE(m_dTemp);i ++)
		m_dTemp[i] = 0.0;
    m_EndTime = {0,0,0,0,0,0,0,0,0};
}

void TxFilterOffTableR1C::Map2Mem()
{
	if (NULL == m_pDataF)
		return;
	uint32_t uiIdxF = 0,uiIdxM = 0;
	uint64_t uiFreq = 0;
    for (uiIdxM = 0;uiIdxM < RF_TX_FREQ_PTS_CALLED;uiIdxM ++) {
        uiFreq = (uint64_t)RF_TX_FREQ_STAR + uiIdxM * (uint64_t)RF_TX_FREQ_STEP_CALLED;
        uiIdxF = (uint32_t)SERIE_INDEX(uiFreq,RF_TX_FREQ_STAR,RF_TX_FREQ_STEP_INFILE);
		m_pDataM[uiIdxM].m_fOffset = (float)(m_pDataF[uiIdxF].m_dOffset);
		m_pDataM[uiIdxM].m_fTemp5 = (float)(m_pDataF[uiIdxF].m_dTemp[1]);	//temp4,5(used),6,7
	}
}

void TxFilterOffTableR1C::Get(uint64_t uiFreq,DataM *pData)
{
	uint64_t uiFreqL = 0,uiFreqR = 0;
    discretept(uiFreq,RF_TX_FREQ_STEP_CALLED,uiFreqL,uiFreqR);

    uint32_t uiIdxL = (uint32_t)SERIE_INDEX(uiFreqL,RF_TX_FREQ_STAR,RF_TX_FREQ_STEP_CALLED);
	*pData = m_pDataM[uiIdxL];
}

void TxFilterOffTableR1C::Get(uint64_t uiFreq,DataF *pData)
{
	uint64_t uiFreqL = 0,uiFreqR = 0;
    discretept(uiFreq,RF_TX_FREQ_STEP_CALLED,uiFreqL,uiFreqR);

    uint32_t uiIdxL = (uint32_t)SERIE_INDEX(uiFreqL,RF_TX_FREQ_STAR,RF_TX_FREQ_STEP_INFILE);
	*pData = m_pDataF[uiIdxL];
	pData->m_uiFreq = uiFreq;
}

void TxFilterOffTableR1C::SaveAs(char *pPath)
{
	FILE *fp = fopen(pPath,"w");
	if (NULL == fp)
		return;

	TxFilterOffTableR1C::DataF Data;

    for (uint64_t uiFreq = RF_TX_FREQ_STAR;uiFreq <= RF_TX_FREQ_STOP;uiFreq += RF_TX_FREQ_STEP_INFILE) {
		Get(uiFreq,&Data);
        fprintf(fp,"freq : %10I64u \t offset : %6.3f \t temp4 : %-.3f temp5 : %-.3f temp6 : %-.3f temp7 : %-.3f time : %4d/%02d/%02d %02d:%02d:%02d\n",
				uiFreq,Data.m_dOffset,Data.m_dTemp[0],Data.m_dTemp[1],Data.m_dTemp[2],Data.m_dTemp[3],
                Data.m_EndTime.tm_year + 1900,Data.m_EndTime.tm_mon,Data.m_EndTime.tm_mday,Data.m_EndTime.tm_hour,Data.m_EndTime.tm_min,Data.m_EndTime.tm_sec);
	}
	fclose(fp);
}
