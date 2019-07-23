#ifndef Q_TEST_RF_THREAD_H
#define Q_TEST_RF_THREAD_H

#include "q_winthread.h"
#include "q_model_freq_res.h"

struct TestRFParam : TestBaseParam
{
    bool isTestTX_PhaseNoise;
    bool isTestTX_NoiseFloor;
};

// The signal : update(const QModelIndex, // Not used.
//                     const QModelIndex, // Not used.
//                     test_item_t,       // The current test item.
//                     int sec)           // The test result.0:fail;1:pass.
class QTestR1CRFThread : public QTestR1CBaseThread
{
public:
    explicit QTestR1CRFThread(TestBaseParam *param) :
        QTestR1CBaseThread(param) {}

    void run();
    void TX_PhaseNoise();

private:
    TestRFParam *param;
};

#endif // Q_TEST_RF_THREAD_H
