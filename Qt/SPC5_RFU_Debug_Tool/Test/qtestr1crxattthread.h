#ifndef QTESTR1CRXATTTHREAD_H
#define QTESTR1CRXATTTHREAD_H

#include "q_winthread.h"

class QTestR1CRxAttThread : public QTestBaseThread
{
    Q_OBJECT

public:
    explicit QTestR1CRxAttThread(TestParam *param,QObject *parent = 0);
    void run();

public:
    void ajustSG(double freq,qint32 pwr);
    double getRxPwr();
private:
    qint32 sgPwr;
    double pmIL;
};



#endif // QTESTR1CRXATTTHREAD_H
