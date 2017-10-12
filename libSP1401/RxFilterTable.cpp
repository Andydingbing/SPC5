#include "RxFilterTable.h"

//Rx RF Frequency Response
RxRFFrTableR1C::DataF::DataF()
{
	m_uiFreq = 0;
	m_dPower = 0.0;
}

void RxRFFrTableR1C::Add(DataF *pData)
{
    uint32_t uiIdx = (uint32_t)SERIE_INDEX(pData->m_uiFreq,RF_RX_FILTER_FREQ_STAR,RF_RX_FILTER_FREQ_STEP);
	m_pDataF[uiIdx] = *pData;
}

void RxRFFrTableR1C::Get(uint64_t uiFreq,DataF *pData)
{
    pData->m_dPower = m_pDataF[SERIE_INDEX(uiFreq,RF_RX_FILTER_FREQ_STAR,RF_RX_FILTER_FREQ_STEP)].m_dPower;
	pData->m_uiFreq = uiFreq;
}

void RxRFFrTableR1C::SaveAs(char *pPath)
{
	FILE *fp = fopen(pPath,"w");
	if (fp == NULL)
		return;

	DataF Data;
    for (uint64_t uiFreq = RF_RX_FILTER_FREQ_STAR;uiFreq <= RF_RX_FILTER_FREQ_STOP;uiFreq += RF_RX_FILTER_FREQ_STEP) {
		Get(uiFreq,&Data);
        fprintf(fp,"freq : %I64u \t power : %.3f\n",uiFreq,Data.m_dPower);
	}
	fclose(fp);
}

//Rx IF Frequency Response
RxIFFrTableR1C::DataF::DataF()
{
	m_dFreq = 0.0;
	m_dPower = 0.0;
}

void RxIFFrTableR1C::Add(DataF *pData)
{
    uint32_t uiIdx = (uint32_t)SERIE_INDEX(pData->m_dFreq,IF_RX_FILTER_160M_FREQ_STAR,IF_RX_FILTER_FREQ_STEP);
	m_pDataF[uiIdx] = *pData;
}

void RxIFFrTableR1C::Get(double dFreq,DataF *pData)
{
    pData->m_dPower = m_pDataF[(uint32_t)(SERIE_INDEX(dFreq,IF_RX_FILTER_160M_FREQ_STAR,IF_RX_FILTER_FREQ_STEP))].m_dPower;
	pData->m_dFreq = dFreq;
}

void RxIFFrTableR1C::SaveAs(char *pPath)
{
	FILE *fp = fopen(pPath,"w");
	if (NULL == fp)
		return;

	DataF Data;
    for (double dFreq = IF_RX_FILTER_160M_FREQ_STAR;dFreq <= IF_RX_FILTER_160M_FREQ_STOP;dFreq += IF_RX_FILTER_FREQ_STEP) {
		Get(dFreq,&Data);
		fprintf(fp, "freq : %.3f \t power : %.3f\n",dFreq,Data.m_dPower);
	}
	fclose(fp);
}

//_80M
IMPLEMENT_R1C_RX_CAL_ITEM_TABLE(RxFilter_80M_TableR1C)

RxFilter_80M_TableR1C::DataF::DataF()
{
	m_uiFreq = 0;
    for (uint32_t i = 0;i < ARRAY_SIZE(m_iCoefReal);i ++)
		m_iCoefReal[i] = 0;
    for (uint32_t i = 0;i < ARRAY_SIZE(m_iCoefImag);i ++)
		m_iCoefImag[i] = 0;
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

void RxFilter_80M_TableR1C::DataM::_2Double(double *pReal,double *pImag)
{
    for (uint32_t i = 0;i < ARRAY_SIZE(m_Coef);i ++) {
		pReal[i] = (double)(m_Coef[i].m_iReal);
		pImag[i] = (double)(m_Coef[i].m_iImag);
	}
}

void RxFilter_80M_TableR1C::Map2Mem()
{
	if (NULL == m_pDataF)
		return;
	uint32_t uiIdxF = 0,uiIdxM = 0;
	uint64_t uiFreq = 0;
    for (uiIdxM = 0;uiIdxM < (uint32_t)(RF_RX_FREQ_PTS_CALLED_R1C);uiIdxM ++) {
        uiFreq = (uint64_t)RF_RX_FREQ_STAR + uiIdxM * (uint64_t)RF_RX_FREQ_STEP_CALLED_R1C;
        uiIdxF = (uint32_t)SERIE_INDEX(uiFreq,RF_RX_FREQ_STAR,RF_RX_FREQ_STEP_INFILE);
		for (int32_t i = 0;i < RX_FILTER_ORDER;i ++) {
			m_pDataM[uiIdxM].m_Coef[i].m_iReal = m_pDataF[uiIdxF].m_iCoefReal[i];
			m_pDataM[uiIdxM].m_Coef[i].m_iImag = m_pDataF[uiIdxF].m_iCoefImag[i];
		}
	}
}

void RxFilter_80M_TableR1C::Get(uint64_t uiFreq,DataM *pData)
{
	uint64_t uiFreqL = 0,uiFreqR = 0;
    discretept(uiFreq,RF_RX_FREQ_STEP_CALLED_R1C,uiFreqL,uiFreqR);

    uint32_t uiIdxL = (uint32_t)SERIE_INDEX(uiFreqL,RF_RX_FREQ_STAR,RF_RX_FREQ_STEP_CALLED_R1C);
	*pData = m_pDataM[uiIdxL];
}

void RxFilter_80M_TableR1C::Get(uint64_t uiFreq,DataF *pData)
{
	uint64_t uiFreqL = 0,uiFreqR = 0;
    discretept(uiFreq,RF_RX_FREQ_STEP_CALLED_R1C,uiFreqL,uiFreqR);

    uint32_t uiIdxL = (uint32_t)SERIE_INDEX(uiFreqL,RF_RX_FREQ_STAR,RF_RX_FREQ_STEP_INFILE);
	*pData = m_pDataF[uiIdxL];
	pData->m_uiFreq = uiFreq;
}

void RxFilter_80M_TableR1C::SaveAs(char *pPath)
{

}

//_160M
IMPLEMENT_R1C_RX_CAL_ITEM_TABLE(RxFilter_160M_TableR1C)

RxFilter_160M_TableR1C::DataF::DataF()
{
	m_uiFreq = 0;
    for (uint32_t i = 0;i < ARRAY_SIZE(m_iCoefReal);i ++)
		m_iCoefReal[i] = 0;
    for (uint32_t i = 0;i < ARRAY_SIZE(m_iCoefImag);i ++)
		m_iCoefImag[i] = 0;
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

void RxFilter_160M_TableR1C::DataM::_2Double(double *pReal,double *pImag)
{
    for (uint32_t i = 0;i < ARRAY_SIZE(m_Coef);i ++) {
		pReal[i] = (double)(m_Coef[i].m_iReal);
		pImag[i] = (double)(m_Coef[i].m_iImag);
	}
}

void RxFilter_160M_TableR1C::Map2Mem()
{
	if (NULL == m_pDataF)
		return;
	uint32_t uiIdxF = 0,uiIdxM = 0;
	uint64_t uiFreq = 0;
    for (uiIdxM = 0;uiIdxM < (uint32_t)(RF_RX_FREQ_PTS_CALLED_R1C);uiIdxM ++) {
        uiFreq = (uint64_t)RF_RX_FREQ_STAR + uiIdxM * (uint64_t)RF_RX_FREQ_STEP_CALLED_R1C;
        uiIdxF = (uint32_t)SERIE_INDEX(uiFreq,RF_RX_FREQ_STAR,RF_RX_FREQ_STEP_INFILE);
		for (int32_t i = 0;i < RX_FILTER_ORDER;i ++) {
			m_pDataM[uiIdxM].m_Coef[i].m_iReal = m_pDataF[uiIdxF].m_iCoefReal[i];
			m_pDataM[uiIdxM].m_Coef[i].m_iImag = m_pDataF[uiIdxF].m_iCoefImag[i];
		}
	}
}

void RxFilter_160M_TableR1C::Get(uint64_t uiFreq,DataM *pData)
{
	uint64_t uiFreqL = 0,uiFreqR = 0;
    discretept(uiFreq,RF_RX_FREQ_STEP_CALLED_R1C,uiFreqL,uiFreqR);

    uint32_t uiIdxL = (uint32_t)SERIE_INDEX(uiFreqL,RF_RX_FREQ_STAR,RF_RX_FREQ_STEP_CALLED_R1C);
	*pData = m_pDataM[uiIdxL];
}

void RxFilter_160M_TableR1C::Get(uint64_t uiFreq,DataF *pData)
{
	uint64_t uiFreqL = 0,uiFreqR = 0;
    discretept(uiFreq,RF_RX_FREQ_STEP_CALLED_R1C,uiFreqL,uiFreqR);

    uint32_t uiIdxL = (uint32_t)SERIE_INDEX(uiFreqL,RF_RX_FREQ_STAR,RF_RX_FREQ_STEP_INFILE);
	*pData = m_pDataF[uiIdxL];
	pData->m_uiFreq = uiFreq;
}

void RxFilter_160M_TableR1C::SaveAs(char *pPath)
{

}
