#ifndef QCALR1CTXLOLEAKTHREAD_H
#define QCALR1CTXLOLEAKTHREAD_H

#include "qwinthread.h"
#include "libSP3301.h"

class QCalR1CTxLOLeakThread : public QCalBaseThread
{
    Q_OBJECT

public:
    QCalR1CTxLOLeakThread(CalParam *pParam,QObject *parent = Q_NULLPTR);
    void run();
    double DerivativeDCOffsetI_SA(CSP2401R1A *pSP2401,int16_t iDC_I_m,int16_t iDC_Q_m,int16_t iDelta,double &dPower);
    double DerivativeDCOffsetQ_SA(CSP2401R1A *pSP2401,int16_t iDC_I_m,int16_t iDC_Q_m,int16_t iDelta,double &dPower);
    int64_t GetMinDCOffsetI_Rx(CSP1401R1C *pSP1401,CSP2401R1A *pSP2401,int16_t iStep,int16_t iLRCoef,int16_t *pDC_I_L,int16_t *pDC_I_R,int16_t *pDC_I_M,int16_t *pDC_Q_M,int64_t *pAD);
    int64_t GetMinDCOffsetQ_Rx(CSP1401R1C *pSP1401,CSP2401R1A *pSP2401,int16_t iStep,int16_t iLRCoef,int16_t *pDC_Q_L,int16_t *pDC_Q_R,int16_t *pDC_I_M,int16_t *pDC_Q_M,int64_t *pAD);
};

#endif // QCALR1CTXLOLEAKTHREAD_H
