#ifndef QCALR1CTXATTTHREAD_H
#define QCALR1CTXATTTHREAD_H

#include "qwinthread.h"

class QCalR1CTxAttThread : public QCalBaseThread
{
    Q_OBJECT

public:
    explicit QCalR1CTxAttThread(CalParam *param,QObject *parent = 0);
    void run();

public:
    void calOneSecOP(int32_t pts,uint64_t *freq,uint64_t *freqStar,uint64_t *freqStop,int32_t *idxOffsetStar,double pwrStar,double pwrStop);
    void calOneSecIO(int32_t pts,uint64_t *freq,uint64_t *freqStar,uint64_t *freqStop,int32_t *idxOffsetStar,double pwrStar,double pwrStop);
};

class QExportR1CTxAttThread : public QExportBaseThread
{
    Q_OBJECT

public:
    explicit QExportR1CTxAttThread(CalParam *param,QObject *parent = 0);
    void run();
};

#endif // QCALR1CTXATTTHREAD_H
