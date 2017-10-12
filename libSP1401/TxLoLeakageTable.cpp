#include "TxLoLeakageTable.h"

//r1a/b
IMPLEMENT_TX_CAL_ITEM_TABLE(TxLOLeakageTableR1A)

TxLOLeakageTableR1A::DataF::DataF()
{
	m_uiFreq = 0;
	m_iDCI = 0;
	m_iDCQ = 0;
}

void TxLOLeakageTableR1A::Map2Mem()
{
	if (NULL == m_pDataF)
		return;
	uint32_t uiIdxF = 0,uiIdxM = 0;
    uint64_t uiFreq = 0;
    for (uiIdxM = 0;uiIdxM < RF_TX_FREQ_PTS_CALLED;uiIdxM ++) {
        uiFreq = (uint64_t)RF_TX_FREQ_STAR + uiIdxM * (uint64_t)RF_TX_FREQ_STEP_CALLED;
        uiIdxF = (uint32_t)SERIE_INDEX(uiFreq,RF_TX_FREQ_STAR,RF_TX_FREQ_STEP_INFILE);
		m_pDataM[uiIdxM].m_iDCI = m_pDataF[uiIdxF].m_iDCI;
		m_pDataM[uiIdxM].m_iDCQ = m_pDataF[uiIdxF].m_iDCQ;
	}
}

void TxLOLeakageTableR1A::Get(uint64_t uiFreq,int16_t &iDCI,int16_t &iDCQ)
{
	uint64_t uiFreqL = 0,uiFreqR = 0;
    discretept(uiFreq,RF_TX_FREQ_STEP_CALLED,uiFreqL,uiFreqR);

    uint32_t uiIdxL = (uint32_t)SERIE_INDEX(uiFreqL,RF_TX_FREQ_STAR,RF_TX_FREQ_STEP_CALLED);
    uint32_t uiIdxR = (uint32_t)SERIE_INDEX(uiFreqR,RF_TX_FREQ_STAR,RF_TX_FREQ_STEP_CALLED);
	DataM DataL = m_pDataM[uiIdxL];
	DataM DataR = m_pDataM[uiIdxR];
	
	linear(uiFreqL,DataL.m_iDCI,uiFreqR,DataR.m_iDCI,uiFreq,iDCI);
	linear(uiFreqL,DataL.m_iDCQ,uiFreqR,DataR.m_iDCQ,uiFreq,iDCQ);
}

void TxLOLeakageTableR1A::SaveAs(char *pPath)
{
	FILE *fp = fopen(pPath,"w");
	if (fp == NULL)
		return;
	int16_t iDCI = 0,IDCQ = 0;
    for (uint64_t uiFreq = RF_TX_FREQ_STAR;uiFreq <= RF_TX_FREQ_STOP;uiFreq += RF_TX_FREQ_STEP_INFILE) {
		Get(uiFreq,iDCI,IDCQ);
        fprintf(fp,"freq : %I64u \t dc_i : %5d \t dc_q : %5d\n",uiFreq,iDCI,IDCQ);
	}
	fclose(fp);
}

//r1c
TxLOLeakageTableR1C::TxLOLeakageTableR1C()
{
	m_pDataM = new DataM[1];
}

TxLOLeakageTableR1C::~TxLOLeakageTableR1C()
{
	if (m_pDataM) {
		delete []m_pDataM;
		m_pDataM = NULL;
	}
}

TxLOLeakageTableR1C::DataF::DataF()
{
	m_bUseSA = false;
	m_dPower = 0.0;
	m_uiFreq = 0;
	m_iDCI = 0;
	m_iDCQ = 0;
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

void TxLOLeakageTableR1C::Add(DataF *pData)
{
	m_pDataF[0] = *pData;
}

void TxLOLeakageTableR1C::Map2Mem()
{
	if (NULL == m_pDataF)
		return;

	m_pDataM[0].m_iDCI = m_pDataF[0].m_iDCI;
	m_pDataM[0].m_iDCQ = m_pDataF[0].m_iDCQ;
	m_pDataM[0].m_fTemp5 = (float)(m_pDataF[0].m_dTemp[1]);	//temp4,5(used),6,7

}

void TxLOLeakageTableR1C::Get(uint64_t uiFreq,int16_t &iDCI,int16_t &iDCQ,float &fTemp5)
{
	iDCI = m_pDataM[0].m_iDCI;
	iDCQ = m_pDataM[0].m_iDCQ;
	fTemp5 = m_pDataM[0].m_fTemp5;
}

void TxLOLeakageTableR1C::Get(uint64_t uiFreq,DataM *pData)
{
	pData->m_iDCI = m_pDataM[0].m_iDCI;
	pData->m_iDCQ = m_pDataM[0].m_iDCQ;
	pData->m_fTemp5 = m_pDataM[0].m_fTemp5;
}

void TxLOLeakageTableR1C::Get(uint64_t uiFreq,DataF *pData)
{
	*pData = m_pDataF[0];
	pData->m_uiFreq = uiFreq;
}

void TxLOLeakageTableR1C::SaveAs(char *pPath)
{
	FILE *fp = fopen(pPath,"w");
	if (fp == NULL)
		return;
	DataF Data;
    uint64_t uiFreq = RF_TX_FREQ_STAR;
	Get(uiFreq,&Data);
	fprintf(fp,"dc_i : %-5d \t dc_q : %-5d \t method : %-s \t power : %-.3f \t temp4 : %-.3f \t temp5 : %-.3f \t temp6 : %-.3f \t temp7 : %-.3f \t time : %4d/%02d/%02d %02d:%02d:%02d\n",
			Data.m_iDCI,Data.m_iDCQ,Data.m_bUseSA ? "spectrum" : "loopback",Data.m_dPower,Data.m_dTemp[0],Data.m_dTemp[1],Data.m_dTemp[2],Data.m_dTemp[3],
            Data.m_EndTime.tm_year + 1900,Data.m_EndTime.tm_mon,Data.m_EndTime.tm_mday,Data.m_EndTime.tm_hour,Data.m_EndTime.tm_min,Data.m_EndTime.tm_sec);
	fclose(fp);
}

//x9119 r1a/b
void X9119TableR1A::Map2Mem()
{
	m_DataM[0].m_iValue	= m_pData[0].m_iValue;
}

void X9119TableR1A::Add(Data *pData)
{
	m_pData[0] = *pData;
}

void X9119TableR1A::Get(int16_t &iValue)
{
	iValue = m_DataM[0].m_iValue;
}

void X9119TableR1A::SaveAs(char *pPath)
{
	FILE *fp = fopen(pPath,"w");
	if (fp == NULL)
		return;
	int16_t iValue = 0;
	Get(iValue);
	fprintf(fp,"x9119 : %d",iValue);
	fclose(fp);
}

//x9119 r1c
void X9119TableR1C::Map2Mem()
{
	m_DataM[0].m_iValue = m_pDataF[0].m_iValue;
	m_DataM[0].m_fTemp = m_pDataF[0].m_fTemp[1];
}

void X9119TableR1C::Add(DataF *pData)
{
	m_pDataF[0] = *pData;
}

void X9119TableR1C::Get(int16_t &iValue,float &fTemp)
{
	iValue = m_DataM[0].m_iValue;
	fTemp = m_DataM[0].m_iValue;
}

void X9119TableR1C::Get(int16_t &iValue,float *pTemp)
{
	iValue = m_pDataF[0].m_iValue;
    for (uint32_t i = 0;i < ARRAY_SIZE(m_pDataF[0].m_fTemp);i ++)
		pTemp[i] = m_pDataF[0].m_fTemp[i];
}

void X9119TableR1C::SaveAs(char *pPath)
{
	FILE *fp = fopen(pPath,"w");
	if (NULL == fp)
		return;
	int16_t iValue = 0;
	float fTemp[8] = {0.0};
	Get(iValue,fTemp);
	fprintf(fp,"x9119 : %d \t",iValue);
    for (uint32_t i = 0;i < ARRAY_SIZE(fTemp);i ++)
		fprintf(fp,"temp%d : %5.3f \t ",i,fTemp[i]);
	fprintf(fp,"\n");
}
