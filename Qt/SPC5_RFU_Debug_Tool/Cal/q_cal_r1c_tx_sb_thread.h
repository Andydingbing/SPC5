#ifndef Q_CAL_R1C_TX_SB_THREAD_H
#define Q_CAL_R1C_TX_SB_THREAD_H

#include "q_winthread.h"

class QCalR1CTXSBThread : public QCalR1CBaseThread
{
public:
    explicit QCalR1CTXSBThread(const CalParam &param) :
        QCalR1CBaseThread(param) {}
    void run();

public:
    void initTXChain();
    void calUseSA(double &thM,uint16_t &am_i_m,uint16_t &am_q_m,double &pwrSb);
    void calUseLoop(double &thM,uint16_t &am_i_m,uint16_t &am_q_m,double &pwrSb);

public:
    double slopeTh_SA(double th,double delta,double &pwr);
    double slopeAmI_SA(uint16_t am_i,uint16_t am_q,uint16_t delta,double &pwr);
    double slopeAmQ_SA(uint16_t am_i,uint16_t am_q,uint16_t delta,double &pwr);
    double getMinThI_Rx(double step,double lrCoef,double *thL,double *thR,double *thM,double *pwr);
    double getMinAmI_Rx(uint16_t step,uint16_t lrCoef,uint16_t *am_i_l,uint16_t *am_i_r,uint16_t *am_i_m,uint16_t *am_q_m,double *pwr);
    double getMinAmQ_Rx(uint16_t step,uint16_t lrCoef,uint16_t *am_q_l,uint16_t *am_q_r,uint16_t *am_i_m,uint16_t *am_q_m,double *pwr);
    void measOnce_Rx(double *pwrAvg);
};

class QExpR1CTXSBThread : public QExpR1CBaseThread
{
public:
    explicit QExpR1CTXSBThread(const CalParam &param) :
        QExpR1CBaseThread(param) {}
    void run();
};

#endif // Q_CAL_R1C_TX_SB_THREAD_H
