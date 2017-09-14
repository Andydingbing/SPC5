#include "TxSidebandTable.h"

//r1a/b
IMPLEMENT_TX_CAL_ITEM_TABLE(TxSidebandTableR1A)

TxSidebandTableR1A::DataF::DataF()
{
	m_uiFreq = 0;
	m_dTh = 0.0;
	m_uiAmI = 0;
	m_uiAmQ = 0;
}

void TxSidebandTableR1A::Map2Mem()
{
	if (NULL == m_pDataF)
		return;
	uint32_t uiIdxF = 0,uiIdxM = 0;
    uint64_t uiFreq = 0;
    for (uiIdxM = 0;uiIdxM < RF_TX_FREQ_PTS_CALLED;uiIdxM ++) {
        uiFreq = (uint64_t)RF_TX_FREQ_STAR + uiIdxM * (uint64_t)RF_TX_FREQ_STEP_CALLED;
        uiIdxF = (uint32_t)SERIE_INDEX(uiFreq,RF_TX_FREQ_STAR,RF_TX_FREQ_STEP_INFILE);
		m_pDataM[uiIdxM].m_fTh   = (float)(m_pDataF[uiIdxF].m_dTh);
		m_pDataM[uiIdxM].m_uiAmI = m_pDataF[uiIdxF].m_uiAmI;
		m_pDataM[uiIdxM].m_uiAmQ = m_pDataF[uiIdxF].m_uiAmQ;
	}
}

void TxSidebandTableR1A::Get(uint64_t uiFreq,double &dTh,uint16_t &uiAmI,uint16_t &uiAmQ)
{
	uint64_t uiFreqL = 0,uiFreqR = 0;
	float fTh = 0.0;
    discretept(uiFreq,RF_TX_FREQ_STEP_CALLED,uiFreqL,uiFreqR);

    uint32_t uiIdxL = (uint32_t)SERIE_INDEX(uiFreqL,RF_TX_FREQ_STAR,RF_TX_FREQ_STEP_CALLED);
    uint32_t uiIdxR = (uint32_t)SERIE_INDEX(uiFreqR,RF_TX_FREQ_STAR,RF_TX_FREQ_STEP_CALLED);
	DataM DataL = m_pDataM[uiIdxL];
	DataM DataR = m_pDataM[uiIdxR];

	linear(uiFreqL,DataL.m_fTh,uiFreqR,DataR.m_fTh,uiFreq,fTh);
	dTh = (double)fTh;
	linear(uiFreqL,DataL.m_uiAmI,uiFreqR,DataR.m_uiAmI,uiFreq,uiAmI);
	linear(uiFreqL,DataL.m_uiAmQ,uiFreqR,DataR.m_uiAmQ,uiFreq,uiAmQ);
}

void TxSidebandTableR1A::SaveAs(char *pPath)
{
	FILE *fp = fopen(pPath,"w");
	if (fp == NULL)
		return;
	double dTh = 0.0;
	uint16_t uiAmI = 0,uiAmQ = 0;
    for (uint64_t uiFreq = RF_TX_FREQ_STAR;uiFreq <= RF_TX_FREQ_STOP;uiFreq += RF_TX_FREQ_STEP_INFILE) {
		Get(uiFreq,dTh,uiAmI,uiAmQ);
        fprintf(fp,"freq : %I64u \t th : %6.3f \t am_i : %u \t am_q : %u\n",uiFreq,dTh,uiAmI,uiAmQ);
	}
	fclose(fp);
}

//r1c
TxSidebandTableR1C::TxSidebandTableR1C()
{
	m_pDataM = new DataM[1];
}

TxSidebandTableR1C::~TxSidebandTableR1C()
{
	if (m_pDataM) {
		delete []m_pDataM;
		m_pDataM = NULL;
	}
}

TxSidebandTableR1C::DataF::DataF()
{
	m_bUseSA = false;
	m_dPower = 0.0;
	m_uiFreq = 0;
	m_dTh = 0.0;
	m_uiAmI = 0;
	m_uiAmQ = 0;
    for (uint32_t i = 0;i < ARRAY_SIZE(m_dTemp);i ++)
		m_dTemp[i] = 0.0;
    m_EndTime = {0,0,0,0,0,0,0,0,0};
}

void TxSidebandTableR1C::Add(DataF *pData)
{
	m_pDataF[0] = *pData;
}

void TxSidebandTableR1C::Map2Mem()
{
	if (NULL == m_pDataF)
		return;

	m_pDataM[0].m_fTh   = (float)(m_pDataF[0].m_dTh);
	m_pDataM[0].m_uiAmI = m_pDataF[0].m_uiAmI;
	m_pDataM[0].m_uiAmQ = m_pDataF[0].m_uiAmQ;
	m_pDataM[0].m_fTemp5 = (float)(m_pDataF[0].m_dTemp[1]);	//temp4,5(used),6,7
}

void TxSidebandTableR1C::Get(uint64_t uiFreq,double &dTh,uint16_t &uiAmI,uint16_t &uiAmQ,float &fTemp5)
{
	dTh = (double)(m_pDataM[0].m_fTh);
	uiAmI = m_pDataM[0].m_uiAmI;
	uiAmQ = m_pDataM[0].m_uiAmQ;
	fTemp5 = m_pDataM[0].m_fTemp5;
}

void TxSidebandTableR1C::Get(uint64_t uiFreq,DataM *pData)
{
	pData->m_fTh = m_pDataM[0].m_fTh;
	pData->m_uiAmI = m_pDataM[0].m_uiAmI;
	pData->m_uiAmQ = m_pDataM[0].m_uiAmQ;
	pData->m_fTemp5 = m_pDataM[0].m_fTemp5;
}

void TxSidebandTableR1C::Get(uint64_t uiFreq,DataF *pData)
{
	*pData = m_pDataF[0];
	pData->m_uiFreq = uiFreq;
}

void TxSidebandTableR1C::SaveAs(char *pPath)
{
	FILE *fp = fopen(pPath,"w");
	if (fp == NULL)
		return;
	DataF Data;
    uint64_t uiFreq = RF_TX_FREQ_STAR;
	Get(uiFreq,&Data);
	fprintf(fp,"th : %-6.3f \t am_i : %-4u \t am_q : %-4u \t method : %-s \t power : %-.3f \t temp4 : %-.3f \t temp5 : %-.3f \t temp6 : %-.3f \t temp7 : %-.3f \t time : %4d/%02d/%02d %02d:%02d:%02d\n",
			Data.m_dTh,Data.m_uiAmI,Data.m_uiAmQ,Data.m_bUseSA ? "spectrum" : "loopback",Data.m_dPower,Data.m_dTemp[0],Data.m_dTemp[1],Data.m_dTemp[2],Data.m_dTemp[3],
            Data.m_EndTime.tm_year + 1900,Data.m_EndTime.tm_mon,Data.m_EndTime.tm_mday,Data.m_EndTime.tm_hour,Data.m_EndTime.tm_min,Data.m_EndTime.tm_sec);
	fclose(fp);
}
