#ifndef QCALR1CTXSBTHREAD_H
#define QCALR1CTXSBTHREAD_H

#include "qwinthread.h"
#include "libsp3301.h"

class QCalR1CTxSBThread : public QCalBaseThread
{
    Q_OBJECT

public:
    explicit QCalR1CTxSBThread(CalParam *param,QObject *parent = 0);
    void run();

public:
    void calUseSA(double &thM,uint16_t &am_i_m,uint16_t &am_q_m,double &pwrSb);
    void calUseLoop(double &thM,uint16_t &am_i_m,uint16_t &am_q_m,double &pwrSb);

public:
    double derivativeTh_SA(double th,double delta,double &pwr);
    double derivativeAmI_SA(uint16_t am_i,uint16_t am_q,uint16_t delta,double &pwr);
    double derivativeAmQ_SA(uint16_t am_i,uint16_t am_q,uint16_t delta,double &pwr);
    double getMinThI_Rx(double step,double lrCoef,double *thL,double *thR,double *thM,double *pwr);
    double getMinAmI_Rx(uint16_t step,uint16_t lrCoef,uint16_t *am_i_l,uint16_t *am_i_r,uint16_t *am_i_m,uint16_t *am_q_m,double *pwr);
    double getMinAmQ_Rx(uint16_t step,uint16_t lrCoef,uint16_t *am_q_l,uint16_t *am_q_r,uint16_t *am_i_m,uint16_t *am_q_m,double *pwr);
    void measOnce_Rx(double *pwrAvg);
};

class QExportR1CTxSBThread : public QExportBaseThread
{
    Q_OBJECT

public:
    explicit QExportR1CTxSBThread(CalParam *param,QObject *parent = 0);
    void run();
};

#endif // QCALR1CTXSBTHREAD_H
