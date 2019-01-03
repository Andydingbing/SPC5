#ifndef QCALR1CRXFILTERTHREAD_H
#define QCALR1CRXFILTERTHREAD_H

#include "qwinthread.h"

class QCalR1CRxFilterThread : public QCalBaseThread
{
    Q_OBJECT

public:
    explicit QCalR1CRxFilterThread(CalParam *param,QObject *parent = 0);
    void run();

public:
    void ajustSG(double freq,qint32 pwr);
    double getRxPwr();
};


class QExportR1CRxFilterThread : public QExportBaseThread
{
    Q_OBJECT

public:
    explicit QExportR1CRxFilterThread(CalParam *param,QObject *parent = 0);
    void run();
};

#endif // QCALR1CRXFILTERTHREAD_H
