#ifndef QTESTR1CTESTPOWTHREAD_H
#define QTESTR1CTESTPOWTHREAD_H

#include "q_winthread.h"
#include "qr1ctestpowmodel.h"
class QTestR1CTestPowThread : public QTestBaseThread
{
    Q_OBJECT

public:
    explicit QTestR1CTestPowThread(TestParam *param,QObject *parent = 0);
    void run();

public:
    int test(double freqMHz,int32_t Freidx,data_f_t &data,bool io_mode_b);
    double initSpectrumAnalyzerOffset(double freqMHz, int refLevel);
    void calOneSecIO(int32_t pts,uint64_t *freq,uint64_t *freqStar,uint64_t *freqStop,int32_t *idxOffsetStar,double pwrStar,double pwrStop);
    double m_maxPower;
    double m_minPower;
    double m_stepPower;
    double m_saRefLevel;
};

class QExportR1CTestPowThread : public QTestExportBaseThread
{
    Q_OBJECT

public:
    explicit QExportR1CTestPowThread(TestParam *param,QObject *parent = 0);
    void run();
};
//class QGetsetR1CTestPowThread : public QTestGetsetBaseThread
//{
//    Q_OBJECT
//signals:
//    void updateCalUI(QList<QString> list);
//public:
//    explicit QGetsetR1CTestPowThread(TestParam *param,QObject *parent = 0);
//    void run();
//};

#endif // QCALR1CTXATTTHREAD_H
