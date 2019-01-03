#ifndef QCALR1CRXREFTHREAD_H
#define QCALR1CRXREFTHREAD_H

#include "qwinthread.h"
#include "libsp3301.h"

class QCalR1CRxRefThread : public QCalBaseThread
{
    Q_OBJECT

public:
    explicit QCalR1CRxRefThread(CalParam *param,QObject *parent = 0);
    void run();

public:
    void ajustSG(double freq,qint32 pwr);

private:
private:
    qint32 sgPwr;
    double pmIL;
};

class QExportR1CRxRefThread : public QExportBaseThread
{
    Q_OBJECT

public:
    explicit QExportR1CRxRefThread(CalParam *param,QObject *parent = 0);
    void run();
};

#endif // QCALR1CRXREFTHREAD_H
