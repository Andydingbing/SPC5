#include "TxFilterTable.h"

TxRFFrTable::DataF::DataF()
{
	m_uiFreq = 0;
	m_dPower = 0.0;
}

//Tx RF Frequency Response( 100MHz ~~~ 3100MHz )
void TxRFFr_0_TableR1C::Add(TxRFFrTable::DataF *pData)
{
    uint32_t uiIdx = (uint32_t)SERIE_INDEX(pData->m_uiFreq,RF_TX_FILTER_FREQ_STAR_0,RF_TX_FILTER_FREQ_STEP);
	m_pDataF[uiIdx] = *pData;
}

void TxRFFr_0_TableR1C::Get(uint64_t uiFreq,TxRFFrTable::DataF *pData)
{
    pData->m_dPower = m_pDataF[SERIE_INDEX(uiFreq,RF_TX_FILTER_FREQ_STAR_0,RF_TX_FILTER_FREQ_STEP)].m_dPower;
	pData->m_uiFreq = uiFreq;
}

void TxRFFr_0_TableR1C::SaveAs(char *pPath)
{
	FILE *fp = fopen(pPath,"w");
	if (fp == NULL)
		return;
	
	TxRFFrTable::DataF Data;
    for (uint64_t uiFreq = RF_TX_FILTER_FREQ_STAR_0;uiFreq <= RF_TX_FILTER_FREQ_STOP_0;uiFreq += RF_TX_FILTER_FREQ_STEP) {
		Get(uiFreq,&Data);
        fprintf(fp,"freq : %I64u \t power : %.3f\n",uiFreq,Data.m_dPower);
	}
	fclose(fp);
}

//Tx RF Frequency Response(3100MHz ~~~ 6GHz)
void TxRFFr_1_TableR1C::Add(TxRFFrTable::DataF *pData)
{
    uint32_t uiIdx = (uint32_t)SERIE_INDEX(pData->m_uiFreq,RF_TX_FILTER_FREQ_STAR_1,RF_TX_FILTER_FREQ_STEP);
	m_pDataF[uiIdx] = *pData;
}

void TxRFFr_1_TableR1C::Get(uint64_t uiFreq,TxRFFrTable::DataF *pData)
{
    pData->m_dPower = m_pDataF[SERIE_INDEX(uiFreq,RF_TX_FILTER_FREQ_STAR_1,RF_TX_FILTER_FREQ_STEP)].m_dPower;
	pData->m_uiFreq = uiFreq;
}

void TxRFFr_1_TableR1C::SaveAs(char *pPath)
{
	FILE *fp = fopen(pPath,"w");
	if (fp == NULL)
		return;

	TxRFFrTable::DataF Data;
    for (uint64_t uiFreq = RF_TX_FILTER_FREQ_STAR_1;uiFreq <= RF_TX_FILTER_FREQ_STOP_1;uiFreq += RF_TX_FILTER_FREQ_STEP) {
		Get(uiFreq,&Data);
        fprintf(fp,"freq : %I64u \t power : %.3f\n",uiFreq,Data.m_dPower);
	}
	fclose(fp);
}

//Tx IF Frequency Response
TxIFFrTableR1C::DataF::DataF()
{
	m_dFreq = 0.0;
	m_dPower = 0.0;
}

void TxIFFrTableR1C::Add(DataF *pData)
{
    uint32_t uiIdx = (uint32_t)SERIE_INDEX(pData->m_dFreq,IF_TX_FILTER_160M_FREQ_STAR,IF_TX_FILTER_FREQ_STEP);
	m_pDataF[uiIdx] = *pData;
}

void TxIFFrTableR1C::Get(double dFreq,DataF *pData)
{
    pData->m_dPower = m_pDataF[(uint32_t)(SERIE_INDEX(dFreq,IF_TX_FILTER_160M_FREQ_STAR,IF_TX_FILTER_FREQ_STEP))].m_dPower;
	pData->m_dFreq = dFreq;
}

void TxIFFrTableR1C::SaveAs(char *pPath)
{
	FILE *fp = fopen(pPath,"w");
	if (NULL == fp)
		return;

	DataF Data;
    for (double dFreq = IF_TX_FILTER_160M_FREQ_STAR;dFreq <= IF_TX_FILTER_160M_FREQ_STOP;dFreq += IF_TX_FILTER_FREQ_STEP) {
		Get(dFreq,&Data);
		fprintf(fp, "freq : %.3f \t power : %.3f\n",dFreq,Data.m_dPower);
	}
	fclose(fp);
}

//_80M
IMPLEMENT_TX_CAL_ITEM_TABLE(TxFilter_80M_TableR1C)

TxFilter_80M_TableR1C::DataF::DataF()
{
	m_uiFreq = 0;
    for (uint32_t i = 0;i < ARRAY_SIZE(m_iCoefReal);i ++)
		m_iCoefReal[i] = 0;
    for (uint32_t i = 0;i < ARRAY_SIZE(m_iCoefImag);i ++)
		m_iCoefImag[i] = 0;
    for (uint32_t i = 0;i < ARRAY_SIZE(m_dTemp);i ++)
		m_dTemp[i] = 0.0;
    m_EndTime = {0,0,0,0,0,0,0,0,0};
}

void TxFilter_80M_TableR1C::DataM::_2Double(double *pReal,double *pImag)
{
    for (uint32_t i = 0;i < ARRAY_SIZE(m_Coef);i ++) {
		pReal[i] = (double)(m_Coef[i].m_iReal);
		pImag[i] = (double)(m_Coef[i].m_iImag);
	}
}

void TxFilter_80M_TableR1C::Map2Mem()
{
	if (NULL == m_pDataF)
		return;
	uint32_t uiIdxF = 0,uiIdxM = 0;
	uint64_t uiFreq = 0;
    for (uiIdxM = 0;uiIdxM < (uint32_t)(RF_TX_FREQ_PTS_CALLED);uiIdxM ++) {
        uiFreq = (uint64_t)RF_TX_FREQ_STAR + uiIdxM * (uint64_t)RF_TX_FREQ_STEP_CALLED;
        uiIdxF = (uint32_t)SERIE_INDEX(uiFreq,RF_TX_FREQ_STAR,RF_TX_FREQ_STEP_INFILE);
		for (int32_t i = 0;i < TX_FILTER_ORDER_2I;i ++) {
			m_pDataM[uiIdxM].m_Coef[i].m_iReal = m_pDataF[uiIdxF].m_iCoefReal[i];
			m_pDataM[uiIdxM].m_Coef[i].m_iImag = m_pDataF[uiIdxF].m_iCoefImag[i];
		}
	}
}

void TxFilter_80M_TableR1C::Get(uint64_t uiFreq,DataM *pData)
{
	uint64_t uiFreqL = 0,uiFreqR = 0;
    discretept(uiFreq,RF_TX_FREQ_STEP_CALLED,uiFreqL,uiFreqR);

    uint32_t uiIdxL = (uint32_t)SERIE_INDEX(uiFreqL,RF_TX_FREQ_STAR,RF_TX_FREQ_STEP_CALLED);
	*pData = m_pDataM[uiIdxL];
}

void TxFilter_80M_TableR1C::Get(uint64_t uiFreq,DataF *pData)
{
	uint64_t uiFreqL = 0,uiFreqR = 0;
    discretept(uiFreq,RF_TX_FREQ_STEP_CALLED,uiFreqL,uiFreqR);

    uint32_t uiIdxL = (uint32_t)SERIE_INDEX(uiFreqL,RF_TX_FREQ_STAR,RF_TX_FREQ_STEP_INFILE);
	*pData = m_pDataF[uiIdxL];
	pData->m_uiFreq = uiFreq;
}

void TxFilter_80M_TableR1C::Coef2Str(int16_t *pCoef,int32_t iOrder,char *pBuf)
{
    char szCoef[32] = {0};

    for (int32_t i = 0;i < iOrder;i ++) {
        sprintf(szCoef,"%d,",pCoef[i]);
        strcat(pBuf,szCoef);
	}
    pBuf[strlen(pBuf)] = '\0';
}

void TxFilter_80M_TableR1C::SaveAs(char *pPath)
{

}

//_160M
IMPLEMENT_TX_CAL_ITEM_TABLE(TxFilter_160M_TableR1C)

TxFilter_160M_TableR1C::DataF::DataF()
{
	m_uiFreq = 0;
    for (uint32_t i = 0;i < ARRAY_SIZE(m_iCoefReal);i ++)
		m_iCoefReal[i] = 0;
    for (uint32_t i = 0;i < ARRAY_SIZE(m_iCoefImag);i ++)
		m_iCoefImag[i] = 0;
    for (uint32_t i = 0;i < ARRAY_SIZE(m_dTemp);i ++)
		m_dTemp[i] = 0.0;
    m_EndTime = {0,0,0,0,0,0,0,0,0};
}

void TxFilter_160M_TableR1C::DataM::_2Double(double *pReal,double *pImag)
{
    for (uint32_t i = 0;i < ARRAY_SIZE(m_Coef);i ++) {
		pReal[i] = (double)(m_Coef[i].m_iReal);
		pImag[i] = (double)(m_Coef[i].m_iImag);
	}
}

void TxFilter_160M_TableR1C::Map2Mem()
{
	if (NULL == m_pDataF)
		return;
	uint32_t uiIdxF = 0,uiIdxM = 0;
	uint64_t uiFreq = 0;
    for (uiIdxM = 0;uiIdxM < (uint32_t)(RF_TX_FREQ_PTS_CALLED);uiIdxM ++) {
        uiFreq = (uint64_t)RF_TX_FREQ_STAR + uiIdxM * (uint64_t)RF_TX_FREQ_STEP_CALLED;
        uiIdxF = (uint32_t)SERIE_INDEX(uiFreq,RF_TX_FREQ_STAR,RF_TX_FREQ_STEP_INFILE);
		for (int32_t i = 0;i < TX_FILTER_ORDER_2I;i ++) {
			m_pDataM[uiIdxM].m_Coef[i].m_iReal = m_pDataF[uiIdxF].m_iCoefReal[i];
			m_pDataM[uiIdxM].m_Coef[i].m_iImag = m_pDataF[uiIdxF].m_iCoefImag[i];
		}
	}
}

void TxFilter_160M_TableR1C::Get(uint64_t uiFreq,DataM *pData)
{
	uint64_t uiFreqL = 0,uiFreqR = 0;
    discretept(uiFreq,RF_TX_FREQ_STEP_CALLED,uiFreqL,uiFreqR);

    uint32_t uiIdxL = (uint32_t)SERIE_INDEX(uiFreqL,RF_TX_FREQ_STAR,RF_TX_FREQ_STEP_CALLED);
	*pData = m_pDataM[uiIdxL];
}

void TxFilter_160M_TableR1C::Get(uint64_t uiFreq,DataF *pData)
{
	uint64_t uiFreqL = 0,uiFreqR = 0;
    discretept(uiFreq,RF_TX_FREQ_STEP_CALLED,uiFreqL,uiFreqR);

    uint32_t uiIdxL = (uint32_t)SERIE_INDEX(uiFreqL,RF_TX_FREQ_STAR,RF_TX_FREQ_STEP_INFILE);
	*pData = m_pDataF[uiIdxL];
	pData->m_uiFreq = uiFreq;
}

void TxFilter_160M_TableR1C::SaveAs(char *pPath)
{

}
