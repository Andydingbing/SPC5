#include "RSNRP.h"
#include <windows.h>

#ifdef NRP_DEBUG
#define NRP_CHECK(func)										\
	do {													\
		ViStatus ret;										\
		char errMsg[256];									\
		ZeroMemory(errMsg, 256);							\
		if ((ret = (func)) != 0) {							\
			FILE *fp;										\
			fopen_s(&fp, "C:\\nrp.log", "a");				\
			rsnrp_error_message(m_hVisa, ret, errMsg);		\
			fprintf(fp, "F : %s\n\t%s\n", #func, errMsg);   \
		} else {											\
			printf("T : %s\n", #func);						\
		}													\
	} while (0);
#else
#define NRP_CHECK(func) func;
#endif

#ifdef _MSC_VER
#pragma comment(lib,"rsnrpz_64.lib")
#endif

RSNRP::RSNRP() : IPM()
{	
}

RSNRP::~RSNRP()
{
}

char* RSNRP::GetDes()
{
    return const_cast<char *>("ROHDE&SCHWARZ,NRP");
}

bool RSNRP::Init(ViRsrc strDev)
{
	NRP_CHECK(rsnrpz_init(strDev,false,false,&m_viSession));
	Sleep(100);
	return true;
}

bool RSNRP::Reset()
{
	ViBoolean bMeasComplete = VI_FALSE;

	NRP_CHECK(rsnrpz_status_preset(m_viSession));
	NRP_CHECK(rsnrpz_avg_setAutoEnabled(m_viSession, 1, VI_FALSE));
	NRP_CHECK(rsnrpz_avg_setCount(m_viSession, 1, 3));
	NRP_CHECK(rsnrpz_trigger_setSource(m_viSession, 1, RSNRPZ_TRIGGER_SOURCE_IMMEDIATE));
	NRP_CHECK(rsnrpz_reset(m_viSession));
	NRP_CHECK(rsnrpz_chans_initiate(m_viSession));
	Sleep(100);
	return true;
}

bool RSNRP::SetFrequency(double dFreq)
{
	NRP_CHECK(rsnrpz_chan_setCorrectionFrequency(m_viSession, 1, dFreq));
	return true;
}

bool RSNRP::GetPwr(double &dPwr)
{
	double dMeas = 0.0;
	ViBoolean bMeasComplete = VI_FALSE;

	NRP_CHECK(rsnrpz_chan_initiate(m_viSession, 1));
	do {
		Sleep (50);
		NRP_CHECK(rsnrpz_chan_isMeasurementComplete(m_viSession, 1, &bMeasComplete));
	} while ((bMeasComplete == VI_FALSE));

	NRP_CHECK(rsnrpz_meass_readMeasurement(m_viSession, 1, 10000, &dMeas));
	dPwr = 10 * log10(fabs(dMeas * 1000));
	return true;	
}

bool RSNRP::GetPwr(double dFreq,double &dPwr)
{
	BOOL_CHECK(SetFrequency(dFreq));
	BOOL_CHECK(GetPwr(dPwr));
	return true;
}

bool RSNRP::Close()
{
	if (m_viSession) {
		rsnrpz_status_preset(m_viSession);
		rsnrpz_CloseSensor(m_viSession, 1);
		rsnrpz_close(m_viSession);
		m_viSession = 0;
	}
	return true;
}
