#ifndef __SP2401R1A_H__
#define __SP2401R1A_H__

#define PROGRAM_BIT_ONEBLOCK_SIZE 500   // * 32bit

#include "stdint.h"
#include "libLog.h"
#include "libBusDriver.h"

class CSP2401R1A
{
public:
	CSP2401R1A(uint32_t uiRfIdx);
    virtual ~CSP2401R1A(){}
public:
	enum DASrc {
		All_Zero     = 0,
		Inter_Filter = 1,
		All_1        = 2,
		Sin_Tone     = 3,
		Dual_Tone    = 4,
		Driver_ARB   = 5,
		DDR3         = 6
	};
	enum ADAPort {
		FromRf = 0,
		FromBb = 1
	};
	enum RxFilter {
		_2I3D = 0,
		_3D = 1
	};
public:
    int32_t  OpenBoard(viPCIDev *pK7,viPCIDev *pS6);
	int32_t	 OpenBoard(viPCIDev *pS6);
	uint32_t GetS6Ver(uint32_t &uiS6Ver);
	uint32_t GetRfIdx();
	int32_t  FpgaReset();
    IPCIDev* GetK7();
    IPCIDev* GetS6();
public:
	int32_t SetDAFreqMHz(double dFreq);
	int32_t SetDASw(ADAPort Port);
	int32_t DAReset();
	int32_t SetADFreqMHz(double dFreq);
	int32_t SetADSw(ADAPort Port);
	int32_t SetTxPowerComp(double dOffset);
	int32_t SetDDSSrc(DASrc Source);
	int32_t SetDDS1(double dFreq);
	int32_t SetDDS2(double dFreq);
	int32_t SetDDSPowerComp(double dOffset = 0.0);
	int32_t SetDUCDDS(double dFreq);
	int32_t SetTxFilterTruncation(uint32_t uiDigit);
	int32_t SetTxFilterSw(bool bEn);
	int32_t SetTxFilter(double *pCoefReal,double *pCoefImag);
	int32_t SetDDC(double dFreq);
	int32_t SetRxFilterTruncation(uint32_t uiDigit);
	int32_t SetRxFilterSw(RxFilter Filter);
	int32_t SetRxFilter2I3D(double *pCoefReal,double *pCoefImag);
	int32_t SetRxFilter2I3D2Default();
	int32_t SetRxFilter(double *pCoefReal,double *pCoefImag);
	int32_t SetRxFilter2Default();
	int32_t SetRxFilter2NB();
	int32_t SetRxPowerComp(int32_t iOffset);
	int32_t SetTxDCOffset(uint16_t uiI,uint16_t uiQ);
	int32_t SetTxPhaseRotateI(double dTh);
	int32_t SetTxAmplitudeBalance(uint16_t uiI,uint16_t uiQ);
public:
    int32_t SetFpgaBit(const char *pFilePath,int32_t iBlockSize = PROGRAM_BIT_ONEBLOCK_SIZE);
private:
	uint32_t m_uiRfIdx;
    viPCIDev *m_pK7;
    viPCIDev *m_pS6;
};

int32_t SetFpgaBit(int32_t iFpgaIdx,char *pPath,viPCIDev *pS6,int32_t iBlockSize);
#endif
