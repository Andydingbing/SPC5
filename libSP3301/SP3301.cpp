#include "SP3301.h"
#include "RegDef.h"

#define INS_SP1401(index)												\
	do {																\
		bool bConnect = false;											\
		char szSN[24] = {0};											\
		m_pSP1401[index]->GetSN(szSN);									\
		if (ISP1401::IsValidSN(szSN) > SN_NULL) {						\
			bConnect = true;											\
			ISP1401::GetHwVer(szSN,m_HwVer[index]);						\
			switch (m_HwVer[index]) {									\
				case ISP1401::R1A :										\
				case ISP1401::R1B : {									\
					m_pSP1401[index] = m_pSP1401R1A[index];				\
					break;												\
				}														\
				case ISP1401::R1C :										\
                case ISP1401::R1D :                                     \
				case ISP1401::HwError : {								\
					m_pSP1401[index] = m_pSP1401R1C[index];				\
					break;												\
				}														\
			}															\
		}																\
		else {															\
            if ((bConnect = m_pSP1401R1A[index]->GetConnect()))			\
				m_pSP1401[index] = m_pSP1401R1A[index];					\
            else if ((bConnect = m_pSP1401R1C[index]->GetConnect()))	\
				m_pSP1401[index] = m_pSP1401R1C[index];					\
		}																\
		if (bConnect) {													\
            INT_CHECK(m_pSP1401[index]->OpenBoard());					\
			m_Active.m_bSP1401[index] = true;							\
		}																\
	} while(0)

#define DECLARE_DYNAMIC_SP1401_SP2401			\
    ISP1401 *pSP1401;							\
    CSP2401R1A *pSP2401;						\
    if (false == m_Active.m_bSP1401[uiRfIdx])	\
        return -1;								\
    pSP1401 = m_pSP1401[uiRfIdx];               \
    pSP2401 = m_pSP2401[uiRfIdx];               \

#define R1A_CAL_FILE ((CalFileR1A *)(pSP1401->GetCalFile()))
#define R1C_CAL_FILE ((CalFileR1C *)(pSP1401->GetCalFile()))

CSP3301::CSP3301(uint32_t uiRfuIdx):m_K7_0(AssK7Name(0,uiRfuIdx).c_str())
								   ,m_K7_1(AssK7Name(1,uiRfuIdx).c_str())
								   ,m_S6(AssS6Name(uiRfuIdx).c_str())
								   ,m_bLoadBitK7_0(true)
								   ,m_bLoadBitK7_1(true)
{
	m_pSP1401.clear();
	m_pSP1401R1A.clear();
	m_pSP1401R1C.clear();
	m_pSP2401.clear();
	for (int32_t i = 0;i < MAX_RF;i ++) {
		m_pSP1401.push_back(NULL);
		m_pSP1401R1A.push_back(new CSP1401R1A(i,uiRfuIdx));
		m_pSP1401R1C.push_back(new CSP1401R1C(i,uiRfuIdx));
		m_pSP2401.push_back(new CSP2401R1A(i));
		m_pSP1401.at(i) = m_pSP1401R1A.at(i);
	}
	m_uiRFUIndex = uiRfuIdx;

	for (int32_t i = 0;i < MAX_RF;i ++) {
		m_HwVer[i] = ISP1401::HwError;
		m_IOMode[i] = OutPut;
		m_uiTxFreq[i] = 2400000000;
		m_dTxPower[i] = -20.0;
		m_uiRxFreq[i] = 2400000000;
		m_dReff[i] = -20.0;
	}
}

CSP3301::~CSP3301()
{
    for (uint32_t i = 0;i < m_pSP1401R1A.size();i ++) {
		if (m_pSP1401R1A[i] != NULL) {
			delete m_pSP1401R1A[i];
			m_pSP1401R1A[i] = NULL;
		}
	}
    for (uint32_t i = 0;i < m_pSP1401R1C.size();i ++) {
		if (m_pSP1401R1C[i] != NULL) {
			delete m_pSP1401R1C[i];
			m_pSP1401R1C[i] = NULL;
		}
	}
    for (uint32_t i = 0;i < m_pSP2401.size();i ++) {
		if (m_pSP2401[i] != NULL) {
			delete m_pSP2401[i];
			m_pSP2401[i] = NULL;
		}
	}
}

CSP3301 &CSP3301::operator = (CSP3301 &sp3301)
{
	return sp3301;
}

CSP3301 &CSP3301::Instance(uint32_t uiRfuIdx)
{
	switch (uiRfuIdx) {
		case 0 : {static CSP3301 sp3301_0(0);return sp3301_0;}
		case 1 : {static CSP3301 sp3301_1(1);return sp3301_1;}
		case 2 : {static CSP3301 sp3301_2(2);return sp3301_2;}
		case 3 : {static CSP3301 sp3301_3(3);return sp3301_3;}
		case 4 : {static CSP3301 sp3301_4(4);return sp3301_4;}
		default: {static CSP3301 reserve(-1);return reserve;}
	}
}

CSP3301::RFUInfo::RFUInfo(const char *pK7_0,const char *pK7_1,const char *pS6,const char *SN):m_uiK7_0_Ver(0)
                                                                                             ,m_uiK7_1_Ver(0)
                                                                                             ,m_uiS6_Ver(0)
{
    strcpy(m_RsrcName.m_K7_0,pK7_0);
    strcpy(m_RsrcName.m_K7_1,pK7_1);
    strcpy(m_RsrcName.m_S6,pS6);
	strcpy(m_SN,SN);
}

CSP3301::Active CSP3301::GetActive()
{
	return m_Active;
}

CSP3301::RFUInfo CSP3301::GetInfo()
{
	return m_RFUInfo;
}

int32_t CSP3301::GetOCXO(int16_t &iValue)
{
	switch (m_HwVer[0]) {
		case ISP1401::R1A : 
		case ISP1401::R1B : {
            m_pSP1401R1A[0]->GetCalFile()->m_pX9119->Get(iValue);
			return 0;
							}
		case ISP1401::R1C : 
		case ISP1401::R1D : {
			float fTemp = 0.0;
            m_pSP1401R1C[0]->GetCalFile()->m_pX9119->Get(iValue,fTemp);
			return 0;
							}
		default:return 0;
	}
	return 0;
}

int32_t CSP3301::Boot()
{
	vector<string> strRsrcRfu;
	vector<string>::iterator iterRsrcRfu;
	viPCIDev viDev;

	char szProductForm[64];
	char szRsrcSection[64];
	char szDevKey[64];
	memset(szProductForm,0,ARRAY_SIZE(szProductForm));
	memset(szRsrcSection,0,ARRAY_SIZE(szRsrcSection));
	memset(szDevKey,0,ARRAY_SIZE(szDevKey));

	CGeneralIniFile IniFile(CONFIG_FILE_PATH);

    INT_CHECK(DDR.RAlloc());
    INT_CHECK(DDR.WAlloc());

	IniFile.GetConfigStringValue("Product Form","Form",szProductForm);
	strcpy(szRsrcSection,"RESOURCE");
	strcat(szRsrcSection,szProductForm);

	sprintf(szDevKey,"RFU%u_K7_0",m_uiRFUIndex);
	IniFile.GetConfigStringValue(szRsrcSection,szDevKey,m_RFUInfo.m_RsrcName.m_K7_0);
	sprintf(szDevKey,"RFU%u_K7_1",m_uiRFUIndex);
	IniFile.GetConfigStringValue(szRsrcSection,szDevKey,m_RFUInfo.m_RsrcName.m_K7_1);
	sprintf(szDevKey,"RFU%u_S6",m_uiRFUIndex);
	IniFile.GetConfigStringValue(szRsrcSection,szDevKey,m_RFUInfo.m_RsrcName.m_S6);

	viDev.GetDevices(strRsrcRfu);

	for (iterRsrcRfu = strRsrcRfu.begin();iterRsrcRfu != strRsrcRfu.end();iterRsrcRfu ++ ) {
		if (!strcmp(m_RFUInfo.m_RsrcName.m_K7_0,iterRsrcRfu->c_str()))
			m_Active.m_bK7_0 = true;
		if (!strcmp(m_RFUInfo.m_RsrcName.m_K7_1,iterRsrcRfu->c_str()))
			m_Active.m_bK7_1 = true;
		if (!strcmp(m_RFUInfo.m_RsrcName.m_S6,iterRsrcRfu->c_str()))
			m_Active.m_bS6 = true;
	}

	if (false == m_Active.m_bK7_0 && false == m_Active.m_bK7_1 && false == m_Active.m_bS6) {
        Log->SetLastError("missing rfu%d",m_uiRFUIndex);
		return -1;
	}

	if (m_Active.m_bK7_0) {
        INT_CHECK(m_K7_0.Init(m_RFUInfo.m_RsrcName.m_K7_0,0x10ee,0x0007));

		m_pSP1401R1A[2]->Connect(&m_K7_0);
		m_pSP1401R1A[3]->Connect(&m_K7_0);
		m_pSP1401R1C[2]->Connect(&m_K7_0);
		m_pSP1401R1C[3]->Connect(&m_K7_0);
		INS_SP1401(2);
		INS_SP1401(3);

        INT_CHECK(DDR.RSendPhyAddr(&m_K7_0));
        INT_CHECK(DDR.WSendPhyAddr(&m_K7_0));
        INT_CHECK(m_pSP1401[2]->GetK7Ver(m_RFUInfo.m_uiK7_0_Ver));
	}

	if (m_Active.m_bK7_1) {
        INT_CHECK(m_K7_1.Init(m_RFUInfo.m_RsrcName.m_K7_1,0x10ee,0x0007));

		m_pSP1401R1A[0]->Connect(&m_K7_1);
		m_pSP1401R1A[1]->Connect(&m_K7_1);
		m_pSP1401R1C[0]->Connect(&m_K7_1);
		m_pSP1401R1C[1]->Connect(&m_K7_1);
		INS_SP1401(0);
		INS_SP1401(1);

        INT_CHECK(DDR.RSendPhyAddr(&m_K7_1));
        INT_CHECK(DDR.WSendPhyAddr(&m_K7_1));
        INT_CHECK(m_pSP1401[0]->GetK7Ver(m_RFUInfo.m_uiK7_1_Ver));
	}
	
	if (m_Active.m_bS6) {
        INT_CHECK(m_S6.Init(m_RFUInfo.m_RsrcName.m_S6,0x10ee,0x2411));
		if (m_Active.m_bK7_0) {
            INT_CHECK(m_pSP2401[2]->OpenBoard(&m_K7_0,&m_S6));
            INT_CHECK(m_pSP2401[3]->OpenBoard(&m_K7_0,&m_S6));
            INT_CHECK(m_pSP2401[2]->GetS6Ver(m_RFUInfo.m_uiS6_Ver));
		}
		if (m_Active.m_bK7_1) {
            INT_CHECK(m_pSP2401[0]->OpenBoard(&m_K7_1,&m_S6));
            INT_CHECK(m_pSP2401[1]->OpenBoard(&m_K7_1,&m_S6));
            INT_CHECK(m_pSP2401[0]->GetS6Ver(m_RFUInfo.m_uiS6_Ver));
		}
	}

	sprintf(m_RFUInfo.m_SN,"SP2401R1BRFUD160%08x%08x",m_RFUInfo.m_uiK7_0_Ver,m_RFUInfo.m_uiK7_1_Ver);

	for (int32_t i = 0;i < 4;i ++) {
		RF_SetIOMode(i,OutPut);
		RF_SetTxFreq(i,m_uiTxFreq[i]);
		RF_SetTxPower(i,(float)m_dTxPower[i]);
		RF_SetRxFreq(i,m_uiRxFreq[i]);
		RF_SetRxLevel(i,m_dReff[i]);
		m_pSP1401[i]->SetPowerMeasTimeout(6553600);
		m_pSP1401[i]->SetPowerMesaTrigGap(0);
	}

	return 0;
}

int32_t CSP3301::SetLoadBit(bool bLoadK7_0,bool bLoadK7_1,char *pBitPath_0,char *pBitPath_1)
{
	m_bLoadBitK7_0 = bLoadK7_0;
	m_bLoadBitK7_1 = bLoadK7_1;
	if (pBitPath_0 != NULL)
		strcpy(m_szBitPath_0,pBitPath_0);
	if (pBitPath_1 != NULL)
		strcpy(m_szBitPath_1,pBitPath_1);
	return 0;
}

int32_t CSP3301::LoadBit()
{
	if (m_Active.m_bS6) {
		if (m_bLoadBitK7_0) {
            INT_CHECK(m_pSP2401[2]->SetFpgaBit(m_szBitPath_0));
		}
		if (m_bLoadBitK7_1) {
            INT_CHECK(m_pSP2401[0]->SetFpgaBit(m_szBitPath_1));
		}
	}
	return 0;
}

int32_t CSP3301::GetRfPort()
{
	int32_t iRfNo = 0;
	for (int i = 0;i < 4;i ++) {
		if (m_Active.m_bSP1401[i] == true)
			iRfNo ++;
	}
	return iRfNo;
}

int32_t CSP3301::GetSN(char *pSN)
{
	strcpy(pSN,m_RFUInfo.m_SN);
	return 0;
}

int32_t CSP3301::GetVer(char *pVer)
{
	strcpy(pVer,"SP2401R1BRFUD160");
	return 0;
}

const char *CSP3301::GetDriverVer()
{
	extern const char *pDriveVer;
	return pDriveVer;
}

string CSP3301::AssK7Name(uint32_t uiK7Idx,uint32_t uiRfuIdx)
{
	char szName[16] = {0};
	memset(szName,0,sizeof(szName));
	sprintf(szName,"rfu%u_k7_%u",uiRfuIdx,uiK7Idx);
	string strName = szName;
	return strName;
}

string CSP3301::AssS6Name(uint32_t uiRfuIdx)
{
	char szName[16] = {0};
	memset(szName,0,sizeof(szName));
	sprintf(szName,"rfu%u_s6",uiRfuIdx);
	string strName = szName;
	return strName;
}

int32_t CSP3301::RF_SetTxState(uint32_t uiRfIdx,bool bState)
{
	DECLARE_DYNAMIC_SP1401_SP2401;
	
	switch (m_HwVer[uiRfIdx]) {
		case ISP1401::R1A : 
		case ISP1401::R1B : {
            INT_CHECK(((CSP1401R1A *)pSP1401)->SetPowEn(bState));
            INT_CHECK(((CSP1401R1A *)pSP1401)->SetTxModulatorEn(bState));
			return 0;
							}
		case ISP1401::R1C :
		case ISP1401::R1D : {
            INT_CHECK(((CSP1401R1C *)pSP1401)->SetPowEn(bState));
            INT_CHECK(((CSP1401R1C *)pSP1401)->SetTxModulatorEn(bState));
			return 0;
							}
		default:return 0;
	}
	return 0;
}

int32_t CSP3301::RF_SetTxPower(uint32_t uiRfIdx,float fPower)
{
	DECLARE_DYNAMIC_SP1401_SP2401;
	uint64_t uiFreq = m_uiTxFreq[uiRfIdx];
	IOMode Mode = m_IOMode[uiRfIdx];

	switch (m_HwVer[uiRfIdx]) {
		case ISP1401::R1A : 
		case ISP1401::R1B : {
			double dBBGain = 0.0,dAttOffset = 0.0;
			int32_t iAtt1 = 0,iAtt2 = 0,iAtt3 = 0;
            SP1401::TxPAAtt PAAtt;

            R1A_CAL_FILE->GetTxPower(uiFreq,fPower,Mode,dBBGain,iAtt1,iAtt2,PAAtt,iAtt3);
            R1A_CAL_FILE->GetTxAtt(uiFreq,Mode,iAtt1 + iAtt2 + iAtt3,dAttOffset);

            INT_CHECK(((CSP1401R1A *)pSP1401)->SetTxPAAttSw(PAAtt));
            INT_CHECK(((CSP1401R1A *)pSP1401)->SyncSetTxGain(iAtt1,iAtt2,iAtt3,dBBGain + dAttOffset));
			break;
							}
		case ISP1401::R1C : 
		case ISP1401::R1D : {
			double dAtt0 = 0.0,dAtt1 = 0.0,dAtt2 = 0.0,dAtt3 = 0.0,dDGain = 0.0;

			if (OutPut == Mode) {
				TxPowerOPTableR1C::DataM DataPower;
                R1C_CAL_FILE->m_pTxPowerOP->Get(uiFreq,fPower,&DataPower);
				dAtt0 = DataPower.m_sAtt0 / 2.0;
				dAtt1 = DataPower.m_sAtt1 / 2.0;
				dAtt2 = DataPower.m_sAtt2 / 2.0;
				dAtt3 = DataPower.m_sAtt3 / 2.0;
                dDGain = (double)DataPower.m_fDGain + R1C_CAL_FILE->m_pTxAttOP->Get(uiFreq,fPower);
			}
			else if (IO == Mode) {
				TxPowerIOTableR1C::DataM DataPower;
                R1C_CAL_FILE->m_pTxPowerIO->Get(uiFreq,fPower,&DataPower);
				dAtt0 = DataPower.m_sAtt0 / 2.0;
				dAtt1 = DataPower.m_sAtt1 / 2.0;
				dAtt2 = DataPower.m_sAtt2 / 2.0;
				dAtt3 = DataPower.m_sAtt3 / 2.0;
                dDGain = (double)DataPower.m_fDGain + R1C_CAL_FILE->m_pTxAttIO->Get(uiFreq,fPower);
			}
// 			pSP1401->SetTxAtt(dAtt0,dAtt1,dAtt2,dAtt3);	//need to sync with baseband
// 			pSP2401->SetTxPowerComp(dDGain);
			break;
							}
		default:break;
	}
	m_dTxPower[uiRfIdx] = (double)fPower;
	return 0;
}

int32_t CSP3301::RF_SetTxFreq(uint32_t uiRfIdx,uint64_t uiFreq)
{
	DECLARE_DYNAMIC_SP1401_SP2401;
	uint64_t uiFreqRf = uiFreq / (int64_t)RF_FREQ_SPACE * (int64_t)RF_FREQ_SPACE;
	double dFreqDUC = double(uiFreq - uiFreqRf);

	switch (m_HwVer[uiRfIdx]) {
		case ISP1401::R1A : 
		case ISP1401::R1B : {
			double dTh = 0.0;
			uint16_t uiAmI = 0,uiAmQ = 0;
			int16_t iDCI = 0,iDCQ = 0;

            R1A_CAL_FILE->m_pTxSideband->Get(uiFreqRf,dTh,uiAmI,uiAmQ);
            R1A_CAL_FILE->m_pTxLOLeak->Get(uiFreqRf,iDCI,iDCQ);
            INT_CHECK(pSP2401->SetTxPhaseRotateI(dTh));
            INT_CHECK(pSP2401->SetTxAmplitudeBalance(uiAmI,uiAmQ));
            INT_CHECK(pSP2401->SetTxDCOffset(iDCI,iDCQ));
			break;
							}
		case ISP1401::R1C : 
		case ISP1401::R1D : {
			TxLOLeakageTableR1C::DataM DataLOLeak;
			TxSidebandTableR1C::DataM DataSideband;

            R1C_CAL_FILE->m_pTxLOLeak->Get(uiFreqRf,&DataLOLeak);
            R1C_CAL_FILE->m_pTxSideband->Get(uiFreqRf,&DataSideband);
            INT_CHECK(pSP2401->SetTxDCOffset((uint16_t)(DataLOLeak.m_iDCI),(uint16_t)(DataLOLeak.m_iDCQ)));
            INT_CHECK(pSP2401->SetTxPhaseRotateI((double)(DataSideband.m_fTh)));
            INT_CHECK(pSP2401->SetTxAmplitudeBalance(DataSideband.m_uiAmI,DataSideband.m_uiAmQ));
			break;
							}
		default:break;
	}
    INT_CHECK(pSP1401->SetTxFreq(uiFreqRf));
    INT_CHECK(pSP2401->SetDUCDDS(dFreqDUC));
	m_uiTxFreq[uiRfIdx] = uiFreq;
    INT_CHECK(RF_SetTxPower(uiRfIdx,(float)m_dTxPower[uiRfIdx]));
	return 0;
}

int32_t CSP3301::RF_SetTxBW(uint32_t uiRfIdx,BW Bw)
{
	DECLARE_DYNAMIC_SP1401_SP2401;

	switch (m_HwVer[uiRfIdx]) {
		case ISP1401::R1A : 
		case ISP1401::R1B : {
			return 0;
							}
		case ISP1401::R1C : 
		case ISP1401::R1D : {
			double dCoefReal[TX_FILTER_ORDER_2I] = {0.0};
			double dCoefImag[TX_FILTER_ORDER_2I] = {0.0};

            R1C_CAL_FILE->SetBW(Bw);
			if (_80M == Bw) {
				TxFilter_80M_TableR1C::DataM DataFilter;
                R1C_CAL_FILE->m_pTxFilter_80M->Get(m_uiTxFreq[uiRfIdx],&DataFilter);
				DataFilter._2Double(dCoefReal,dCoefImag);
				pSP2401->SetTxFilter(dCoefReal,dCoefImag);
			}
			else if (_160M == Bw) {
				TxFilter_160M_TableR1C::DataM DataFilter;
                R1C_CAL_FILE->m_pTxFilter_160M->Get(m_uiTxFreq[uiRfIdx],&DataFilter);
				DataFilter._2Double(dCoefReal,dCoefImag);
				pSP2401->SetTxFilter(dCoefReal,dCoefImag);
			}
			return 0;
							}
		default:return 0;
	}
	return 0;
}

int32_t CSP3301::RF_SetTxDelay(uint32_t uiRfIdx,double dDelayns)
{
	return 0;
}

int32_t CSP3301::RF_SetTxSource(uint32_t uiRfIdx,CSP2401R1A::DASrc Source)
{
	DECLARE_DYNAMIC_SP1401_SP2401;
    INT_CHECK(pSP2401->SetDDSSrc(Source));
	return 0;
}

int32_t CSP3301::RF_SetSrcFreq(uint32_t uiRfIdx,uint64_t uiFreq)
{
	DECLARE_DYNAMIC_SP1401_SP2401;
    INT_CHECK(pSP2401->SetDDS1((double)uiFreq));
	return 0;
}

int32_t CSP3301::ArbLoad(uint32_t uiRfIdx,char *pPath)
{
	uint32_t uiRfIdxInter = 0;
	if (0 == uiRfIdx)
		uiRfIdxInter = 1;
	else if (1 == uiRfIdx)
		uiRfIdxInter = 0;
	else if (2 == uiRfIdx)
		uiRfIdxInter = 3;
	else
		uiRfIdxInter = 2;

	ISP1401 *pSP1401[2] = {m_pSP1401[uiRfIdx],m_pSP1401[uiRfIdxInter]};
	FILE *fp[2] = {NULL,NULL};
    viPCIDev *m_pK7 = pSP1401[0]->GetK7();

	uint32_t uiSplsTotal = 0;
	uint32_t uiSplsLeft = 0;
	uint32_t uiSplsTrsed = 0;
	uint32_t uiSplsTrsing = 0;
	uint32_t uiDDRAddr = 0;
	uint32_t uiCnt = 0;

	ArbReader::Param_t Param[2];
	list<ArbReader::SegHeader_t>::iterator iterSegHeader;
	list<ArbSeg_t>::iterator iterArbSeg;

    INT_CHECK(pSP1401[0]->GetArbReader()->Load(pPath));
	if (FALSE == pSP1401[1]->GetArbReader()->IsLoaded()) {
        INT_CHECK(pSP1401[1]->GetArbReader()->Load(pPath));
	}

	if ((fp[0] = pSP1401[0]->GetArbReader()->Load(&Param[0],NULL)) == NULL)
		return -1;
	if ((fp[1] = pSP1401[1]->GetArbReader()->Load(&Param[1],NULL)) == NULL)
		return -1;

	if (Param[0].TotalSamples != Param[1].TotalSamples) {
        Log->SetLastError("samples file0:%d != file1:%d",Param[0].TotalSamples,Param[1].TotalSamples);
		return -1;
	}

	iterSegHeader = Param[0].SegHeader.begin();
	iterArbSeg = Param[0].ArbSeg.begin();

    INT_CHECK(pSP1401[0]->ArbStop());
    INT_CHECK(pSP1401[1]->ArbStop());
    INT_CHECK(pSP1401[0]->SetArbSegments(Param[0].FileHeader.Segments));
    INT_CHECK(pSP1401[1]->SetArbSegments(Param[1].FileHeader.Segments));

	for (iterSegHeader = Param[0].SegHeader.begin();iterSegHeader != Param[0].SegHeader.end();iterSegHeader ++) {
        INT_CHECK(pSP1401[0]->SetArbSegment(*iterArbSeg));
		iterArbSeg ++;
	}

    RFU_K7_REG_DECLARE(0x0028);
    RFU_K7_REG_DECLARE(0x002f);
    RFU_K7_REG(0x0028).flag = 0;

	uiSplsTotal = Param[0].TotalSamples + Param[1].TotalSamples;
	uiSplsLeft = uiSplsTotal;
    Log->stdprintf("prepare fpga dma read,total samples(*2ed):%d\n",uiSplsTotal);
	while(uiSplsLeft > 0)
	{
		uiCnt ++;
		uiSplsTrsing = uiSplsLeft > T_BLOCK_SAMPLES ? T_BLOCK_SAMPLES : uiSplsLeft;

		uiDDRAddr = uiSplsTrsed >> 1;
        RFU_K7_REG(0x002f).samples = uiSplsTrsing;
        RFU_K7_W(0x002f);
        RFU_K7_REG(0x0028).addr = uiDDRAddr;
        RFU_K7_W(0x0028);

		for (uint32_t i = 0;i < uiSplsTrsing / 2;i ++) {
			fread(DDR.GetUsrSpc() + i * 2,    sizeof(ArbReader::Data_t),1,fp[0]);
			fread(DDR.GetUsrSpc() + i * 2 + 1,sizeof(ArbReader::Data_t),1,fp[1]);
		}

        INT_CHECK(DDR.FpgaRead(m_pK7,DDR.GetUsrSpc(),uiSplsTrsing,NULL));
		
		uiSplsTrsed += uiSplsTrsing;
		uiSplsLeft = uiSplsTotal - uiSplsTrsed;
        Log->stdprintf("dma%-4d done,samples(*2ed):%-8d total(*2ed):%-10d\r",uiCnt,uiSplsTrsing,uiSplsTrsed);
	}
    Log->stdprintf("\n");
    Log->stdprintf("all done\n");
	return 0;
}

int32_t CSP3301::RF_SetArbEn(uint32_t uiRfIdx,bool bState)
{
	return 0;
}

int32_t CSP3301::RF_SetArbTrigSrc(uint32_t uiRfIdx,ARB_TRIGGERMODE Source)
{
	return 0;
}

int32_t CSP3301::RF_SetArbTrig(uint32_t uiRfIdx)
{
	return 0;
}

int32_t CSP3301::RF_SetArbCount(uint32_t uiRfIdx,int iCnt)
{
	return 0;
}

int32_t CSP3301::RF_SetRxLevel(uint32_t uiRfIdx,double dLevel)
{
	DECLARE_DYNAMIC_SP1401_SP2401;
	uint64_t uiFreq = m_uiRxFreq[uiRfIdx];
	IOMode Mode = m_IOMode[uiRfIdx];

	switch (m_HwVer[uiRfIdx]) {
		case ISP1401::R1A : 
		case ISP1401::R1B : {
            SP1401::RxLNAAtt LNAAtt = SP1401::Rx_Att;
			double dAtt1 = 0.0;
			int32_t iAtt2 = 0;
			int64_t iAD_0dbfs = _0dBFS;

            R1A_CAL_FILE->m_pRxRef->Get(uiFreq,dLevel,Mode,iAD_0dbfs,LNAAtt,dAtt1,iAtt2);
            INT_CHECK(((CSP1401R1A *)pSP1401)->SetRxLNAAttSw(LNAAtt));
            INT_CHECK(((CSP1401R1A *)pSP1401)->SetRxAtt(dAtt1,iAtt2));
            INT_CHECK(pSP2401->SetRxPowerComp((int32_t)(_0dBFS - iAD_0dbfs)));
			break;
							}
		case ISP1401::R1C : 
		case ISP1401::R1D : {
			RxRefOPTableR1C::RxStateM RxState;
			int32_t iOffset = 0;

			if (OutPut == Mode) {
                R1C_CAL_FILE->m_pRxRefOP->Get(uiFreq,dLevel,&RxState);
                iOffset = R1C_CAL_FILE->m_pRxAttOP->Get(uiFreq,dLevel);
				iOffset += RxState.m_iADOffset;
			}
			if (IO == Mode) {
                R1C_CAL_FILE->m_pRxRefIO->Get(uiFreq,dLevel,&RxState);
                iOffset = R1C_CAL_FILE->m_pRxAttIO->Get(uiFreq,dLevel);
				iOffset += RxState.m_iADOffset;
			}
            INT_CHECK(((CSP1401R1C *)pSP1401)->SetRxLNAAttSw((SP1401::RxLNAAtt)(RxState.m_sLNAAtt)));
            INT_CHECK(((CSP1401R1C *)pSP1401)->SetRxAtt019Sw((SP1401::RxAtt019)(RxState.m_sAtt019)));
            INT_CHECK(((CSP1401R1C *)pSP1401)->SetRxAtt((double)RxState.m_fAtt1,(double)RxState.m_fAtt2,(double)RxState.m_fAtt3));
            INT_CHECK(pSP2401->SetRxPowerComp(iOffset));
			break;
							}
		default:break;
	}
	m_dReff[uiRfIdx] = dLevel;
	return 0;
}

int32_t CSP3301::RF_SetRxFreq(uint32_t uiRfIdx,uint64_t uiFreq)
{
	DECLARE_DYNAMIC_SP1401_SP2401;
	uint64_t uiFreqRf = uiFreq / RF_FREQ_SPACE * RF_FREQ_SPACE;
	double dFreqDDC = -92640000.0 - double(uiFreq - uiFreqRf);

	switch (m_HwVer[uiRfIdx]) {
		case ISP1401::R1A : 
		case ISP1401::R1B : {
			break;
							}
		case ISP1401::R1C : 
		case ISP1401::R1D : {
			break;
							}
		default:break;
	}
    INT_CHECK(pSP1401->SetRxFreq(uiFreqRf));
    INT_CHECK(pSP2401->SetDDC(dFreqDDC));
	m_uiRxFreq[uiRfIdx] = uiFreq;
    INT_CHECK(RF_SetRxLevel(uiRfIdx,m_dReff[uiRfIdx]));
	return 0;
}

int32_t CSP3301::RF_SetRxBW(uint32_t uiRfIdx,BW Bw)
{
	DECLARE_DYNAMIC_SP1401_SP2401;

	switch (m_HwVer[uiRfIdx]) {
		case ISP1401::R1A : 
		case ISP1401::R1B : {
			return 0;
							}
		case ISP1401::R1C : 
		case ISP1401::R1D : {
            R1C_CAL_FILE->SetBW(Bw);
			double dCoefReal[RX_FILTER_ORDER] = {0.0};
			double dCoefImag[RX_FILTER_ORDER] = {0.0};

			if (_80M == Bw) {
				RxFilter_80M_TableR1C::DataM DataFilter;

                R1C_CAL_FILE->m_pRxFilter_80M->Get(m_uiTxFreq[uiRfIdx],&DataFilter);
				DataFilter._2Double(dCoefReal,dCoefImag);
				pSP2401->SetRxFilter(dCoefReal,dCoefImag);
			}
			else if (_160M == Bw) {
				RxFilter_160M_TableR1C::DataM DataFilter;

                R1C_CAL_FILE->m_pRxFilter_160M->Get(m_uiTxFreq[uiRfIdx],&DataFilter);
				DataFilter._2Double(dCoefReal,dCoefImag);
				pSP2401->SetRxFilter(dCoefReal,dCoefImag);
			}
			return 0;
							}
		default:return 0;
	}
	return 0;
}

int32_t CSP3301::RF_SetRxDelay(uint32_t uiRfIdx,double dDelayns)
{
	return 0;
}

int32_t CSP3301::RF_SetIOMode(uint32_t uiRfIdx,IOMode Mode)
{
	DECLARE_DYNAMIC_SP1401_SP2401;
    INT_CHECK(pSP1401->SetIOMode(Mode));
	m_IOMode[uiRfIdx] = Mode;
	return 0;
}

int32_t CSP3301::RF_SetTrigSrc(uint32_t uiRfIdx,RFU_TRIGGERSOURCE TrigSrc)
{
	return 0;
}

int32_t CSP3301::RF_SetTrigMode(uint32_t uiRfIdx,ISP1401::PowerMeasSrcs Mode)
{
	DECLARE_DYNAMIC_SP1401_SP2401;
    INT_CHECK(pSP1401->SetPowerMeasSrc(Mode,true));
	return 0;
}

int32_t CSP3301::RF_SetTrigLevel(uint32_t uiRfIdx,float fLevel)
{
	DECLARE_DYNAMIC_SP1401_SP2401;
    INT_CHECK(pSP1401->SetPowerMeasTrigThreshold((double)fLevel));
	return 0;
}

int32_t CSP3301::RF_SetTrigOffset(uint32_t uiRfIdx,uint32_t uiOffset)
{
	DECLARE_DYNAMIC_SP1401_SP2401;
    INT_CHECK(pSP1401->SetNListIQCapTrigOffset(uiOffset));
	return 0;
}

int32_t CSP3301::RF_SetIQCapLength(uint32_t uiRfIdx,uint32_t uiLength)
{
	DECLARE_DYNAMIC_SP1401_SP2401;
    INT_CHECK(pSP1401->SetPowerMeasSamples(uiLength));
	return 0;
}

int32_t CSP3301::RF_SetIQDataMap(uint32_t uiRfIdx,uint16_t *pData)
{
	return 0;
}

int32_t CSP3301::RF_InitPowerMeas(uint32_t uiRfIdx)
{
	DECLARE_DYNAMIC_SP1401_SP2401;
    INT_CHECK(pSP1401->PowerMeasAbort());
    INT_CHECK(pSP1401->PowerMeasStart());
	return 0;
}

int32_t CSP3301::RF_AbortPowerMeas(uint32_t uiRfIdx)
{
	DECLARE_DYNAMIC_SP1401_SP2401;
    INT_CHECK(pSP1401->PowerMeasAbort());
	return 0;
}

int32_t CSP3301::RF_GetPowerProc(uint32_t uiRfIdx,ISP1401::PowerMeasState &Proc)
{
	DECLARE_DYNAMIC_SP1401_SP2401;
    INT_CHECK(pSP1401->GetPowerMeasState(Proc));
	return 0;
}

int32_t CSP3301::RF_GetMeasResult(uint32_t uiRfIdx,float &fPower,float &fCrest)
{
	DECLARE_DYNAMIC_SP1401_SP2401;
	double dAvgPower = 0.0,dPeakPower = 0.0;
	fPower = -100.0;
	fCrest = 0.0;
    INT_CHECK(pSP1401->GetPowerMeasPower(dAvgPower));
    INT_CHECK(pSP1401->GetPowerMeasPeak(dPeakPower));
	fPower = (float)dAvgPower;
	fCrest = (float)(dPeakPower - dAvgPower);
	return 0;
}

int32_t CSP3301::RF_InitIQCap(uint32_t uiRfIdx)
{
	return 0;
}

int32_t CSP3301::RF_AbortIQCap(uint32_t uiRfIdx)
{
	DECLARE_DYNAMIC_SP1401_SP2401;
    INT_CHECK(pSP1401->PowerMeasAbort());
	return 0;
}

// int32_t CSP3301::RF_GetIQCapProc(uint32_t uiRfIdx,PROCESS &Process);
// {
// 	return 0;
// }

int32_t CSP3301::RF_GetTemp(uint32_t uiRfIdx,double &dTxTemp,double &dRxTemp)
{
	DECLARE_DYNAMIC_SP1401_SP2401;

	switch (m_HwVer[uiRfIdx]) {
		case ISP1401::R1A : 
		case ISP1401::R1B : {
            INT_CHECK(((CSP1401R1A *)pSP1401)->GetTxTemp(dTxTemp));
            INT_CHECK(((CSP1401R1A *)pSP1401)->GetRxTemp(dRxTemp));
			return 0;
							}
		case ISP1401::R1C : {
			return 0;
							}
		default:return 0;
	}
	return 0;
}
