#ifndef QCALR1CTXLOLEAKTHREAD_H
#define QCALR1CTXLOLEAKTHREAD_H

#include "qwinthread.h"
#include "libsp3301.h"

class QCalR1CTxLOLeakThread : public QCalBaseThread
{
    Q_OBJECT

public:
    explicit QCalR1CTxLOLeakThread(CalParam *param,QObject *parent = 0);
    void run();

public:
    void calUseSA(int16_t &dc_i_m,int16_t &dc_q_m,double &pwrLOL);
    void calUseLoop(int16_t &dc_i_m,int16_t &dc_q_m,double &pwrLOL);

public:
    double derivativeDCOffsetI_SA(int16_t dc_i_m,int16_t dc_q_m,int16_t delta,double &pwr);
    double derivativeDCOffsetQ_SA(int16_t dc_i_m,int16_t dc_q_m,int16_t delta,double &pwr);
    int64_t getMinDCOffsetI_Rx(int16_t step,int16_t lr_coef,int16_t *dc_i_l,int16_t *dc_i_r,int16_t *dc_i_m,int16_t *dc_q_m,int64_t *ad);
    int64_t getMinDCOffsetQ_Rx(int16_t step,int16_t lr_coef,int16_t *dc_q_l,int16_t *dc_q_r,int16_t *dc_i_m,int16_t *dc_q_m,int64_t *ad);
};

class QExportR1CTxLOLeakThread : public QExportBaseThread
{
    Q_OBJECT

public:
    explicit QExportR1CTxLOLeakThread(CalParam *param,QObject *parent = 0);
    void run();
};

#endif // QCALR1CTXLOLEAKTHREAD_H
