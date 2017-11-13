#include "SP2401R1A.h"
#include "RegDef.h"
#include "CalTable.h"

#define NCO_DDS_32 0
#define NCO_DDS_64 1

#define NCO_DUC_32 0
#define NCO_DUC_64 1

#define NCO_DDC_32 0
#define NCO_DDC_64 1

CSP2401R1A::CSP2401R1A(uint32_t uiRfIdx)
{
	m_pK7 = NULL;
	m_pS6 = NULL;
	m_uiRfIdx = uiRfIdx;
}

int32_t CSP2401R1A::OpenBoard(viPCIDev *pK7, viPCIDev *pS6)
{
    m_pK7 = pK7;
    m_pS6 = pS6;

    INT_CHECK(SetDAFreqMHz(491.53));
    INT_CHECK(SetADFreqMHz(368.65));
    INT_CHECK(SetDDSSrc(Inter_Filter));
    INT_CHECK(SetDDS1(20e6));
    INT_CHECK(SetDDS2(20e6));
    INT_CHECK(SetDDSPowerComp());
    INT_CHECK(SetDUCDDS(0.0));
    INT_CHECK(SetTxFilterTruncation(12));
    INT_CHECK(SetTxFilterSw(true));
    INT_CHECK(SetTxPowerComp(0.0));

    INT_CHECK(SetDDC(-92.64e6));
    INT_CHECK(SetRxPowerComp(0));
    INT_CHECK(SetRxFilterTruncation(12));
    INT_CHECK(SetRxFilterSw(_2I3D));
    INT_CHECK(SetRxFilter2Default());
	return 0;
}

int32_t CSP2401R1A::OpenBoard(viPCIDev *pS6)
{
    m_pS6 = pS6;
	return 0;
}

uint32_t CSP2401R1A::GetS6Ver(uint32_t &uiS6Ver)
{
    RFU_S6_REG_DECLARE(0x0000);
    RFU_S6_R(0x0000);
    uiS6Ver = RFU_S6_REG_U32(0x0000);
	return 0;
}

uint32_t CSP2401R1A::GetRfIdx()
{
	return m_uiRfIdx;
}

int32_t CSP2401R1A::FpgaReset()
{
    RFU_K7_REG_DECLARE(0x0000);
    RFU_K7_OP(0x0000);
	return 0;
}

IPCIDev* CSP2401R1A::GetK7()
{
    return m_pK7;
}

IPCIDev* CSP2401R1A::GetS6()
{
    return m_pS6;
}

int32_t CSP2401R1A::SetDAFreqMHz(double dFreq)
{
    RFU_K7_REG_DECLARE(0x0035);
    RFU_K7_REG_DECLARE(0x0036);
    RFU_K7_REG(0x0036).freq_int = (unsigned)dFreq;
    RFU_K7_REG(0x0036).freq_frac = (unsigned)((dFreq - (unsigned)dFreq) * 100);
    RFU_K7_W(0x0036);
    RFU_K7_REG(0x0035).ad_da = 0;
    RFU_K7_OP(0x0035);
	return 0;
}

int32_t CSP2401R1A::SetDASw(ADAPort Port)
{
    RFU_S6_REG_DECLARE(0x000b);
    RFU_S6_R(0x000b);
	switch (m_uiRfIdx) {
        case 0 : {RFU_S6_REG(0x000b).rf_ch_0 = (unsigned)Port;break;}
        case 1 : {RFU_S6_REG(0x000b).rf_ch_1 = (unsigned)Port;break;}
        case 2 : {RFU_S6_REG(0x000b).rf_ch_2 = (unsigned)Port;break;}
        case 3 : {RFU_S6_REG(0x000b).rf_ch_3 = (unsigned)Port;break;}
		default:break;
	}
    RFU_S6_W(0x000b);
	return 0;
}

int32_t CSP2401R1A::DAReset()
{
    RFU_S6_REG_DECLARE(0x000b);
    RFU_S6_R(0x000b);
    RFU_S6_REG(0x000b).rst = 1;
    RFU_S6_W(0x000b);
	Sleep(10);
    RFU_S6_REG(0x000b).rst = 0;
    RFU_S6_W(0x000b);
	return 0;
}

int32_t CSP2401R1A::SetADFreqMHz(double dFreq)
{
    RFU_K7_REG_DECLARE(0x0035);
    RFU_K7_REG_DECLARE(0x0036);
    RFU_K7_REG(0x0036).freq_int = (unsigned)dFreq;
    RFU_K7_REG(0x0036).freq_frac = (unsigned)((dFreq - (unsigned)dFreq) * 100);
    RFU_K7_W(0x0036);
    RFU_K7_REG(0x0035).ad_da = 1;
    RFU_K7_OP(0x0035);
	return 0;
}

int32_t CSP2401R1A::SetADSw(ADAPort Port)
{
    RFU_S6_REG_DECLARE(0x000a);
    RFU_S6_R(0x000a);
	switch (m_uiRfIdx) {
        case 0 : {RFU_S6_REG(0x000a).rf_ch_0 = (unsigned)Port;break;}
        case 1 : {RFU_S6_REG(0x000a).rf_ch_1 = (unsigned)Port;break;}
        case 2 : {RFU_S6_REG(0x000a).rf_ch_2 = (unsigned)Port;break;}
        case 3 : {RFU_S6_REG(0x000a).rf_ch_3 = (unsigned)Port;break;}
		default:break;
	}
    RFU_S6_W(0x000a);
	return 0;
}

int32_t CSP2401R1A::SetTxPowerComp(double dOffset)
{
    RFU_K7_REG_DECLARE_2(0x0094,0x2094);
    RFU_K7_REG_2(0x0094,0x2094).i = (uint16_t)(dOffset * 512.0);
    RFU_K7_REG_2(0x0094,0x2094).q = (uint16_t)(dOffset * 512.0);
    RFU_K7_W_2(0x0094,0x2094);
	return 0;
}

int32_t CSP2401R1A::SetDDSSrc(DASrc Source)
{
    RFU_K7_REG_DECLARE_2(0x0072,0x2072);
    RFU_K7_REG_2(0x0072,0x2072).bb_tx_source = Source;
    RFU_K7_W_2(0x0072,0x2072);
	return 0;
}

int32_t CSP2401R1A::SetDDS1(double dFreq)
{
    RFU_K7_REG_DECLARE_2(0x0079,0x2079);
    RFU_K7_REG_DECLARE_2(0x007a,0x207a);
#if (NCO_DDS_32)
    RFU_K7_REG_2(0x007a,0x207a).freq = (unsigned)(4294967296.0 * dFreq / 1000000.0 / 245.76);
#elif (NCO_DDS_64)
    RFU_K7_REG_DECLARE_2(0x00e9,0x20e9);
	unsigned int uiDDSH = 0,uiDDSL = 0;
	int64_t iFreq = int64_t(round(dFreq,0));
	char szFreq[16];
    sprintf(szFreq,"%I64i",iFreq);
	CGeneralInteger DDS = CGeneralInteger("18446744073709551616") * CGeneralInteger(szFreq) / 245760000;
	DDS.toui64(uiDDSH,uiDDSL);
    RFU_K7_REG_2(0x007a,0x207a).freq = uiDDSL;
    RFU_K7_REG_2(0x00e9,0x20e9).freq = uiDDSH;
    RFU_K7_W_2(0x00e9,0x20e9);
#endif
    RFU_K7_W_2(0x007a,0x207a);
    RFU_K7OP_2(0x0079,0x2079,dds1_op);
	return 0;
}

int32_t CSP2401R1A::SetDDS2(double dFreq)
{
    RFU_K7_REG_DECLARE_2(0x0079,0x2079);
#if (NCO_DDS_32)
    RFU_K7_REG_DECLARE_2(0x007a,0x207a);
    RFU_K7_REG_2(0x007a,0x207a).freq = (unsigned)(4294967296.0 * dFreq / 1000000.0 / 245.76);
    RFU_K7_W_2(0x007a,0x207a);
#elif (NCO_DDS_64)
    RFU_K7_REG_DECLARE_2(0x00ea,0x20ea);
    RFU_K7_REG_DECLARE_2(0x00eb,0x20eb);
	unsigned int uiDDSH = 0,uiDDSL = 0;
	int64_t iFreq = int64_t(round(dFreq,0));
	char szFreq[16];
    sprintf(szFreq,"%I64i",iFreq);
	CGeneralInteger DDS = CGeneralInteger("18446744073709551616") * CGeneralInteger(szFreq) / 245760000;
	DDS.toui64(uiDDSH,uiDDSL);
    RFU_K7_REG_2(0x00eb,0x20eb).freq = uiDDSH;
    RFU_K7_REG_2(0x00ea,0x20ea).freq = uiDDSL;
    RFU_K7_W_2(0x00eb,0x20eb);
    RFU_K7_W_2(0x00ea,0x20ea);
#endif
    RFU_K7OP_2(0x0079,0x2079,dds2_op);
	return 0;
}

int32_t CSP2401R1A::SetDDSPowerComp(double dOffset)
{
    RFU_K7_REG_DECLARE_2(0x0085,0x2085);
    RFU_K7_REG_2(0x0085,0x2085).dds1_amp = (unsigned)(sqrt((double)_0dBFS) * 8192 / 32768);
    RFU_K7_REG_2(0x0085,0x2085).dds2_amp = (unsigned)(sqrt((double)_0dBFS) * 8192 / 32768);
    RFU_K7_W_2(0x0085,0x2085);
	return 0;
}

int32_t CSP2401R1A::SetDUCDDS(double dFreq)
{
    RFU_K7_REG_DECLARE_2(0x007b,0x207b);
    RFU_K7_REG_DECLARE_2(0x007c,0x207c);
#if (NCO_DUC_32)
    RFU_K7_REG_2(0x007c,0x207c).duc = (unsigned)(4294967296.0 * dFreq / 1000000.0 / 245.76);
#elif (NCO_DUC_64)
    RFU_K7_REG_DECLARE_2(0x00e8,0x20e8);
	unsigned int uiDUCH = 0,uiDUCL = 0;
	int64_t iFreq = int64_t(round(dFreq,0));
	char szFreq[16];
    sprintf(szFreq,"%I64i",iFreq);
	CGeneralInteger DUC = CGeneralInteger("18446744073709551616") * CGeneralInteger(szFreq) / 245760000;
	DUC.toui64(uiDUCH,uiDUCL);
    RFU_K7_REG_2(0x007c,0x207c).duc = uiDUCL;
    RFU_K7_REG_2(0x00e8,0x20e8).duc = uiDUCH;
    RFU_K7_W_2(0x00e8,0x20e8);
#endif
    RFU_K7_W_2(0x007c,0x207c);
    RFU_K7_OP_2(0x007b,0x207b);
	return 0;
}

int32_t CSP2401R1A::SetTxFilterTruncation(uint32_t uiDigit)
{
    RFU_K7_REG_DECLARE_2(0x00ff,0x20ff);
    RFU_K7_R_2(0x00ff,0x20ff);
    RFU_K7_REG_2(0x00ff,0x20ff).digit_tx = (unsigned)uiDigit;
    RFU_K7_W_2(0x00ff,0x20ff);
	return 0;
}

int32_t CSP2401R1A::SetTxFilterSw(bool bEn)
{
    RFU_K7_REG_DECLARE_2(0x00fa,0x20fa)
    RFU_K7_R_2(0x00fa,0x20fa);
    RFU_K7_REG_2(0x00fa,0x20fa).en = bEn ? 1 : 0;
    RFU_K7_W_2(0x00fa,0x20fa);
	return 0;
}

int32_t CSP2401R1A::SetTxFilter(double *pCoefReal,double *pCoefImag)
{
    RFU_K7_REG_DECLARE_2(0x00ec,0x20ec);
    RFU_K7_REG_DECLARE_2(0x00ed,0x20ed);
    RFU_K7_REG_DECLARE_2(0x00ee,0x20ee);
    RFU_K7_REG_DECLARE_2(0x00ef,0x20ef);
    RFU_K7_REG_DECLARE_2(0x00f0,0x20f0);
    RFU_K7_REG_DECLARE_2(0x00f1,0x20f1);

	for (int32_t i = 0;i < TX_FILTER_ORDER_2I;i ++) {
        RFU_K7_REG_2(0x00ed,0x20ed).addr = unsigned(i);
        RFU_K7_REG_2(0x00ef,0x20ef).real = unsigned(pCoefReal[i]);
        RFU_K7_REG_2(0x00f0,0x20f0).imag = unsigned(pCoefImag[i]);
        RFU_K7_REG_2(0x00ee,0x20ee).sum  = unsigned(pCoefReal[i] + pCoefImag[i]);
        RFU_K7_W_2(0x00ed,0x20ed);
        RFU_K7_W_2(0x00ee,0x20ee);
        RFU_K7_W_2(0x00ef,0x20ef);
        RFU_K7_W_2(0x00f0,0x20f0);
        RFU_K7_OP_2(0x00ec,0x20ec);
	}
    RFU_K7_OP_2(0x00f1,0x20f1);
	return 0;
}

int32_t CSP2401R1A::SetDDC(double dFreq)
{
    RFU_K7_REG_DECLARE_2(0x007d,0x207d);
    RFU_K7_REG_DECLARE_2(0x007e,0x207e);
#if (NCO_DDC_32)
    RFU_K7_REG_2(0x007e,0x207e).ddc = (unsigned)(dFreq * 4294967296 / (368.64 * 1e6));
#elif (NCO_DDC_64)
    RFU_K7_REG_DECLARE_2(0x00e7,0x20e7);
	unsigned int uiDDCH = 0,uiDDCL = 0;
	int64_t iFreq = int64_t(round(dFreq,0));
	char szFreq[16];
    sprintf(szFreq,"%I64i",iFreq);
	CGeneralInteger DDC = CGeneralInteger("18446744073709551616") * CGeneralInteger(szFreq) / 368640000;
	DDC.toui64(uiDDCH,uiDDCL);
    RFU_K7_REG_2(0x007e,0x207e).ddc = uiDDCL;
    RFU_K7_REG_2(0x00e7,0x20e7).ddc = uiDDCH;
    RFU_K7_W_2(0x00e7,0x20e7);
#endif
    RFU_K7_W_2(0x007e,0x207e);
    RFU_K7_OP_2(0x007d,0x207d);
	return 0;
}

int32_t CSP2401R1A::SetRxFilterTruncation(uint32_t uiDigit)
{
    RFU_K7_REG_DECLARE_2(0x00ff,0x20ff);
    RFU_K7_R_2(0x00ff,0x20ff);
    RFU_K7_REG_2(0x00ff,0x20ff).digit_rx = (unsigned)uiDigit;
    RFU_K7_W_2(0x00ff,0x20ff);
	return 0;
}

int32_t CSP2401R1A::SetRxFilterSw(RxFilter Filter)
{
// 	RFU_K7_REG_DECLARE_2(0x00f9,0x20f9);
// 	RFU_K7_REG_2(0x00f9,0x20f9).filter = (_2I3D == Filter ? 0 : 1);
// 	RFU_K7_W_2(0x00f9,0x20f9);
    RFU_K7_REG_DECLARE_2(0x00f8,0x20f8);
    RFU_K7_REG_2(0x00f8,0x20f8).filter = (_2I3D == Filter ? 1 : 0);
    RFU_K7_W_2(0x00f8,0x20f8);
	return 0;
}

int32_t CSP2401R1A::SetRxFilter2I3D(double *pCoefReal,double *pCoefImag)
{
    RFU_K7_REG_DECLARE_2(0x1069,0x1099);
    RFU_K7_REG_DECLARE_2(0x106c,0x109c);
    RFU_K7_REG_DECLARE_2(0x1080,0x10b0);

    RFU_K7_REG_2(0x106c,0x109c).group = 0;
	for (int32_t i = 0;i < RX_FILTER_ORDER_2I3D;i ++) {
        RFU_K7_REG_2(0x106c,0x109c).op = 0;
        RFU_K7_REG_2(0x106c,0x109c).addr = unsigned(i);
        RFU_K7_REG_2(0x106c,0x109c).real = unsigned(pCoefReal[i]);
        RFU_K7_REG_2(0x1080,0x10b0).imag = unsigned(pCoefImag[i]);
        RFU_K7_W_2(0x1080,0x10b0);
        RFU_K7_OP_2(0x106c,0x109c);
	}
    RFU_K7_OP_2(0x1069,0x1099);
	return 0;
}

int32_t CSP2401R1A::SetRxFilter2I3D2Default()
{
	double dReal[RX_FILTER_ORDER_2I3D] = {-22,-35,9,21,-1,-31,-1,36,9,-46,-21,49,33,-54,-50,55,69,-53,-92,46,116,-33,-142,13,168,15,-191,-52,211,98,-224,-152,228,214,-222,-283,201,356,-165,-432,111,507,-40,-584,-59,650,181,-706,-329,745,504,-767,-718,750,958,-699,-1238,597,1565,-430,-1957,167,2444,241,-3099,-909,4098,2151,-6037,-5288,12703,32767,32767,12703,-5288,-6037,2151,4098,-909,-3099,241,2444,167,-1957,-430,1565,597,-1238,-699,958,750,-718,-767,504,745,-329,-706,181,650,-59,-584,-40,507,111,-432,-165,356,201,-283,-222,214,228,-152,-224,98,211,-52,-191,15,168,13,-142,-33,116,46,-92,-53,69,55,-50,-54,33,49,-21,-46,9,36,-1,-31,-1,21,9,-35,-22};
	double dImag[RX_FILTER_ORDER_2I3D] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	return SetRxFilter2I3D(dReal,dImag);
}

int32_t CSP2401R1A::SetRxFilter(double *pCoefReal,double *pCoefImag)
{
    RFU_K7_REG_DECLARE_2(0x00f2,0x20f2);
    RFU_K7_REG_DECLARE_2(0x00f3,0x20f3);
    RFU_K7_REG_DECLARE_2(0x00f4,0x20f4);
    RFU_K7_REG_DECLARE_2(0x00f5,0x20f5);
    RFU_K7_REG_DECLARE_2(0x00f6,0x20f6);
    RFU_K7_REG_DECLARE_2(0x00f7,0x20f7);

	for (int32_t i = 0;i < RX_FILTER_ORDER;i ++) {
        RFU_K7_REG_2(0x00f3,0x20f3).addr = unsigned(i);
        RFU_K7_REG_2(0x00f5,0x20f5).real = unsigned(pCoefReal[i]);
        RFU_K7_REG_2(0x00f6,0x20f6).imag = unsigned(pCoefImag[i]);
        RFU_K7_REG_2(0x00f4,0x20f4).sum  = unsigned(pCoefReal[i] + pCoefImag[i]);
        RFU_K7_W_2(0x00f3,0x20f3);
        RFU_K7_W_2(0x00f4,0x20f4);
        RFU_K7_W_2(0x00f5,0x20f5);
        RFU_K7_W_2(0x00f6,0x20f6);
        RFU_K7_OP_2(0x00f2,0x20f2);
	}
    RFU_K7_OP_2(0x00f7,0x20f7);
	return 0;
}

int32_t CSP2401R1A::SetRxFilter2NB()
{
	double dReal[RX_FILTER_ORDER] = {55,-74,-117,-42,61,25,-84,-55,80,73,-86,-104,80,133,-73,-171,53,208,-26,-249,-16,285,69,-320,-139,345,223,-362,-326,362,446,-345,-589,300,755,-222,-956,95,1205,106,-1539,-440,2044,1061,-3021,-2633,6360,16383,16383,6360,-2633,-3021,1061,2044,-440,-1539,106,1205,95,-956,-222,755,300,-589,-345,446,362,-326,-362,223,345,-139,-320,69,285,-16,-249,-26,208,53,-171,-73,133,80,-104,-86,73,80,-55,-84,25,61,-42,-117,-74,55};
	double dImag[RX_FILTER_ORDER] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	return SetRxFilter(dReal,dImag);
}

int32_t CSP2401R1A::SetRxFilter2Default()
{
	double dReal[RX_FILTER_ORDER] = {12,3,-7,-1,6,6,-8,-6,7,10,-8,-12,7,16,-6,-19,3,24,0,-26,-5,31,10,-33,-18,36,27,-36,-38,36,50,-34,-65,29,83,-20,-104,6,130,16,-165,-52,219,119,-323,-287,683,1769,1769,683,-287,-323,119,219,-52,-165,16,130,6,-104,-20,83,29,-65,-34,50,36,-38,-36,27,36,-18,-33,10,31,-5,-26,0,24,3,-19,-6,16,7,-12,-8,10,7,-6,-8,6,6,-1,-7,3,12};
	double dImag[RX_FILTER_ORDER] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	return SetRxFilter(dReal,dImag);
}

int32_t CSP2401R1A::SetRxPowerComp(int32_t iOffset)
{
	double ptp_iq = sqrt((double)(_0dBFS - iOffset));
	int32_t ratio = (int32_t)round((sqrt((double)_0dBFS) * (8192 / ptp_iq)),0);

    RFU_K7_REG_DECLARE_2(0x00e4,0x20e4);
    RFU_K7_REG_2(0x00e4,0x20e4).ratio = (unsigned)ratio;
    RFU_K7_W_2(0x00e4,0x20e4);
	return 0;
}

int32_t CSP2401R1A::SetTxDCOffset(uint16_t uiI,uint16_t uiQ)
{
    RFU_K7_REG_DECLARE_2(0x0061,0x2061);
    RFU_K7_REG_2(0x0061,0x2061).i = uiI;
    RFU_K7_REG_2(0x0061,0x2061).q = uiQ;
    RFU_K7_W_2(0x0061,0x2061);
	return 0;
}

int32_t CSP2401R1A::SetTxPhaseRotateI(double dTh)
{
    RFU_K7_REG_DECLARE_2(0x006a,0x206a);
    RFU_K7_REG_2(0x006a,0x206a).cos = (uint16_t)(cos(ANGLE2RADIAN(dTh)) * 16384);
    RFU_K7_REG_2(0x006a,0x206a).sin = (uint16_t)(sin(ANGLE2RADIAN(dTh)) * 16384);
    RFU_K7_W_2(0x006a,0x206a);
	return 0;
}

int32_t CSP2401R1A::SetTxAmplitudeBalance(uint16_t uiI,uint16_t uiQ)
{
    RFU_K7_REG_DECLARE_2(0x0067,0x2067);
    RFU_K7_REG_2(0x0067,0x2067).i = uiI;
    RFU_K7_REG_2(0x0067,0x2067).q = uiQ;
    RFU_K7_W_2(0x0067,0x2067);
	return 0;
}

int32_t CSP2401R1A::SetFpgaBit(const char *pFilePath,int32_t iBlockSize)
{
	int32_t iFileSize = 0;
	int32_t iCnt = 0;
	int32_t iFpgaIdx = m_uiRfIdx == 0 || m_uiRfIdx == 1 ? 1 : 0;

	FILE *fp = fopen(pFilePath,"rb");
	if (NULL == fp) {
        Log.SetLastError("can't open %s",pFilePath);
		return -1;
	}

	fseek(fp,0,SEEK_END);
	iFileSize = ftell(fp);											//byte
    Log.stdprintf("file size before align = %d\n",iFileSize);
	iFileSize = (iFileSize / 4 + ((iFileSize % 4) ? 1 : 0)) * 4;	//4 bytes align
	iCnt = (iFileSize / 4 / iBlockSize) + 1;						//4 bytes once
    Log.stdprintf("file size after  align = %d\n",iFileSize);
	fseek(fp,0,SEEK_SET);

	uint32_t *pBuf = new uint32_t[iFileSize / 4];
	memset(pBuf,0,iFileSize / 4);
	fread(pBuf,1,iFileSize,fp);

    RFU_S6_REG_DECLARE_2(0x0002,0x0006);
    RFU_S6_REG_DECLARE_2(0x0003,0x0007);
    RFU_S6_REG_DECLARE_2(0x0004,0x0008);
    RFU_S6_REG_DECLARE_2(0x0005,0x0009);
    RFU_S6_REG_2(0x0004,0x0008).load_mode = 1;
    RFU_S6_W_2(0x0004,0x0008);
    RFU_S6_REG_2(0x0002,0x0006).PROGRAM_B = 0;
    RFU_S6_W_2(0x0002,0x0006);
    RFU_S6_REG_2(0x0002,0x0006).PROGRAM_B = 1;
    RFU_S6_W_2(0x0002,0x0006);

	int32_t iInitFlag = 0;
    RFU_S6_R_2(0x0003,0x0007);
    while(0 == RFU_S6_REG_2(0x0003,0x0007).pc_cfg_init) {
        RFU_S6_R_2(0x0003,0x0007);
		if ((++iInitFlag) > 1000)
			break;
	}

    Log.stdprintf("prepare programming %s,waiting fifo empty...\n",1 == iFpgaIdx ? "k7_1" : "k7_0" );
    RFU_S6_WAIT_FIFO_EMPTY_2(0x0003,0x0007,0,1000);
    Log.stdprintf("downloading...\n");

	for (int32_t i = 0;i < iCnt;i ++) {
		if (i == iCnt - 1)
            m_pS6->W32(IPCIDev::AS_BAR0,(0 == iFpgaIdx ? 0x0005 : 0x0009) << 2,(iFileSize - i * 4 * iBlockSize) / 4,&pBuf[i * iBlockSize]);
		else
            m_pS6->W32(IPCIDev::AS_BAR0,(0 == iFpgaIdx ? 0x0005 : 0x0009) << 2,iBlockSize,&pBuf[i * iBlockSize]);
        RFU_S6_WAIT_FIFO_EMPTY_2(0x0003,0x0007,0,1000);

		for (int32_t n = 0;n < 100000;n ++) {
			int j = 0;
			j ++;
		}

        Log.stdprintf("%-5d done		\r",i);
	}
    RFU_S6_R_2(0x0003,0x0007);
    Log.stdprintf("\n");
	delete []pBuf;
	fclose(fp);

    if (1 == RFU_S6_REG_2(0x0003,0x0007).c_cfg_done) {
		Sleep(10);
        Log.stdprintf("restarting...\n");
		system("devcon_x64.exe restart \"PCI\\VEN_10EE&DEV_7021\"");
		Sleep(1000);
        Log.stdprintf("restarted\n");
		return 0;
	}
	else {
        Log.SetLastError("download complete,but device start fail");
        Log.stdprintf("download fail\n");
		return -1;
	}
	return 0;
}

int32_t SetFpgaBit(int32_t iFpgaIdx,char *pPath,viPCIDev *pS6,int32_t iBlockSize)
{
	uint32_t uiRfIdx = (0 == iFpgaIdx ? 2 : 0);
	CSP2401R1A SP2401(uiRfIdx);
	SP2401.OpenBoard(pS6);
	return SP2401.SetFpgaBit(pPath,iBlockSize);
}
