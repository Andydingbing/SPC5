#ifndef QTESTR1CRXFREQRESPONSETHREAD_H
#define QTESTR1CRXFREQRESPONSETHREAD_H

#include "q_winthread.h"

class QTestR1CRxFreqResponseThread : public QTestBaseThread
{
    Q_OBJECT

public:
    explicit QTestR1CRxFreqResponseThread(TestParam *param,QObject *parent = 0);
    void run();
public:
    void ajustSG(double freq,qint32 pwr);
    double getRxPwr();
};

class QExportR1CRxFreqResponseThread : public QTestExportBaseThread
{
    Q_OBJECT

public:
    explicit QExportR1CRxFreqResponseThread(TestParam *param,QObject *parent = 0);
    void run();
};
//class QGetsetR1CRxFreqResponseThread : public QTestGetsetBaseThread
//{
//    Q_OBJECT
//signals:
//    void updateCalUI(QList<QString> list);
//public:
//    explicit QGetsetR1CRxFreqResponseThread(TestParam *param,QObject *parent = 0);
//    void run();
//};


#endif // QCALR1CTXFILTEROFFSETTHREAD_H
