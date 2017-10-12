#include "RxFilterOffTable.h"

IMPLEMENT_R1C_RX_CAL_ITEM_TABLE(RxFilterOffTableR1C)

RxFilterOffTableR1C::DataF::DataF()
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

void RxFilterOffTableR1C::Map2Mem()
{
	if (NULL == m_pDataF)
		return;
	uint32_t uiIdxF = 0,uiIdxM = 0;
	uint64_t uiFreq = 0;
    for (uiIdxM = 0;uiIdxM < RF_RX_FREQ_PTS_CALLED_R1C;uiIdxM ++) {
        uiFreq = (uint64_t)RF_RX_FREQ_STAR + uiIdxM * (uint64_t)RF_RX_FREQ_STEP_CALLED_R1C;
        uiIdxF = (uint32_t)SERIE_INDEX(uiFreq,RF_RX_FREQ_STAR,RF_RX_FREQ_STEP_INFILE);
        for (uint32_t i = 0;i < ARRAY_SIZE(m_pDataM[uiIdxM].m_iOffset);i ++)
			m_pDataM[uiIdxM].m_iOffset[i] = (int32_t)(m_pDataF[uiIdxF].m_iOffset[i]);
		m_pDataM[uiIdxM].m_fTemp5 = (float)(m_pDataF[uiIdxF].m_dTemp[1]);	//temp4,5(used),6,7
	}
}

void RxFilterOffTableR1C::Get(uint64_t uiFreq,DataM *pData)
{
	uint64_t uiFreqL = 0,uiFreqR = 0;
    discretept(uiFreq,RF_RX_FREQ_STEP_CALLED_R1C,uiFreqL,uiFreqR);

    uint32_t uiIdxL = (uint32_t)SERIE_INDEX(uiFreqL,RF_RX_FREQ_STAR,RF_RX_FREQ_STEP_CALLED_R1C);
	*pData = m_pDataM[uiIdxL];
}

void RxFilterOffTableR1C::Get(uint64_t uiFreq,DataF *pData)
{
	uint64_t uiFreqL = 0,uiFreqR = 0;
    discretept(uiFreq,RF_RX_FREQ_STEP_CALLED_R1C,uiFreqL,uiFreqR);

    uint32_t uiIdxL = (uint32_t)SERIE_INDEX(uiFreqL,RF_RX_FREQ_STAR,RF_RX_FREQ_STEP_INFILE);
	*pData = m_pDataF[uiIdxL];
	pData->m_uiFreq = uiFreq;
}

void RxFilterOffTableR1C::SaveAs(char *pPath)
{
	FILE *fp = fopen(pPath,"w");
	if (NULL == fp)
		return;

	RxFilterOffTableR1C::DataF Data;

    for (uint64_t uiFreq = RF_RX_FREQ_STAR;uiFreq <= RF_RX_FREQ_STOP;uiFreq += RF_RX_FREQ_STEP_INFILE) {
		Get(uiFreq,&Data);
        fprintf(fp,"freq : %10I64u \t offset : %8I64i %8I64i %8I64i \t temp4 : %-.3f temp5 : %-.3f temp6 : %-.3f temp7 : %-.3f time : %4d/%02d/%02d %02d:%02d:%02d\n",
				uiFreq,Data.m_iOffset[0],Data.m_iOffset[1],Data.m_iOffset[2],Data.m_dTemp[0],Data.m_dTemp[1],Data.m_dTemp[2],Data.m_dTemp[3],
                Data.m_EndTime.tm_year + 1900,Data.m_EndTime.tm_mon,Data.m_EndTime.tm_mday,Data.m_EndTime.tm_hour,Data.m_EndTime.tm_min,Data.m_EndTime.tm_sec);
	}
	fclose(fp);
}
