#ifndef Q_TEST_TEMP_PWR_THREAD_H
#define Q_TEST_TEMP_PWR_THREAD_H

#include "q_winthread.h"
#include "q_model_freq_res.h"

class QwtTempPwrData;

struct TestTempPwrParam : TestBaseParam
{
    basic_sp1401::rf_ch_t ch;
    QString FreqStar;
    QString FreqStop;
    QString FreqStep;
    QVector<QVector<QwtTempPwrData *> *> data;
};

class QTestR1CTempPwrThread : public QTestR1CBaseThread
{
public:
    explicit QTestR1CTempPwrThread(TestBaseParam *param) :
        QTestR1CBaseThread(param) {}

    void run();
    void Tx();
    void Rx();

private:
    TestTempPwrParam *param;
    range_freq_string FreqString;
    range_freq<quint64> FreqRange;
};

#endif // Q_TEST_TEMP_PWR_THREAD_H
