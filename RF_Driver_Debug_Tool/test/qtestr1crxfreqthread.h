#ifndef QTESTR1CRXFREQTHREAD_H
#define QTESTR1CRXFREQTHREAD_H

#include "q_winthread.h"

class QTestR1CRxFreqThread : public QTestBaseThread
{
    Q_OBJECT

public:
    explicit QTestR1CRxFreqThread(TestParam *param,QObject *parent = 0);
    void run();
public:
    void ajustSG(double freq,qint32 pwr);
    double getRxPwr();
};

class QExportR1CRxFreqThread : public QTestExportBaseThread
{
//     Q_OBJECT

// public:
//     explicit QExportR1CRxFreqThread(CalParam *param,QObject *parent = 0);
//     void run();
};
//class QGetsetR1CRxFreqThread : public QTestGetsetBaseThread
//{
////     Q_OBJECT
//// signals:
////     void updateCalUI(QList<QString> list);
//// public:
////     explicit QGetsetR1CRxFreqThread(CalParam *param,QObject *parent = 0);
////     void run();
//};


#endif // QTESTR1CTXFILTEROFFSETTHREAD_H
