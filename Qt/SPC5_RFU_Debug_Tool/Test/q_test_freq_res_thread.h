#ifndef Q_TEST_FREQ_RES_THREAD_H
#define Q_TEST_FREQ_RES_THREAD_H

#include "q_winthread.h"
#include "q_model_freq_res.h"

struct TestFreqResParam : TestBaseParam
{
    bool isTestRF_TX;
    bool isTestIF_TX;
    bool isTestRF_RX;
    bool isTestIF_RX;
    QString RF_TXFreqStar;
    QString RF_TXFreqStop;
    QString RF_TXFreqStep;
    QString RF_RXFreqStar;
    QString RF_RXFreqStop;
    QString RF_RXFreqStep;
};

// The signal : update(const QModelIndex, // Not used.
//                     const QModelIndex, // Not used.
//                     test_item_t,       // The current test item.
//                     int sec)           // The test result.0:fail;1:pass.
class QTestR1CFreqResThread : public QTestR1CBaseThread
{
public:
    explicit QTestR1CFreqResThread(TestBaseParam *param) :
        QTestR1CBaseThread(param),
        pwrBefore(0.0),
        pwrMin(0.0),
        pwrMax(0.0) { }

    void run();
    void sweepTX();
    void sweepRX();
    void checkRF_TX(rf_tx_freq_res_test_data &data);
    void checkIF_TX(if_tx_freq_res_test_data &data);
    void checkRF_RX(rf_rx_freq_res_test_data &data);
    void checkIF_RX(if_rx_freq_res_test_data &data);

private:
    TestFreqResParam *param;
    range_freq_string RF_TXFreqString;
    range_freq_string RF_RXFreqString;
    range_freq<quint64> RF_TXFreqRange;
    range_freq<quint64> RF_RXFreqRange;
    QwtRF_TX_FreqResData *dataRF_TX;
    QwtIF_TX_FreqResData *dataIF_TX;
    QwtRF_RX_FreqResData *dataRF_RX;
    QwtIF_RX_FreqResData *dataIF_RX;
    double pwrBefore;
    double pwrMin;
    double pwrMax;
};

#endif // Q_TEST_FREQ_RES_THREAD_H
