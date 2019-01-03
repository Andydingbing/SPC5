#ifndef QCALR1CRXATTTHREAD_H
#define QCALR1CRXATTTHREAD_H

#include "qwinthread.h"

class QCalR1CRxAttThread : public QCalBaseThread
{
    Q_OBJECT

public:
    explicit QCalR1CRxAttThread(CalParam *param,QObject *parent = 0);
    void run();

public:
    void setRxState(rx_ref_op_table_r1cd::rx_state_m_t state);
    void ajustSG(double freq,qint32 pwr);

private:
    qint32 sgPwr;
    double pmIL;
};

class QExportR1CRxAttThread : public QExportBaseThread
{
    Q_OBJECT

public:
    explicit QExportR1CRxAttThread(CalParam *param,QObject *parent = 0);
    void run();
};

#endif // QCALR1CRXATTTHREAD_H
