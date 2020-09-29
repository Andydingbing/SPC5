#ifndef SP9500PRO_CAL_TX_FILTER_THREAD_H
#define SP9500PRO_CAL_TX_FILTER_THREAD_H

#include "q_winthread.h"
#include "q_model_tx_filter.h"

namespace NS_SP9500Pro {

class Q_Cal_TXFilter_Thread : public QCalBaseThread
{
public:
    explicit Q_Cal_TXFilter_Thread(const CalParam &param) :
        QCalBaseThread(param) { }
    void cal();
    void check();

    void init();
    void sweepRF(const uint64_t star,
                 const uint64_t stop,
                 const uint64_t step,
                 const int32_t table);
    void sweepIF();
    void generateFreqResponse();
    void generateCoef();
    void updateCalFile();

public:
    uint64_t BW_Max;
    ns_sp1403::port_t port;
    bool justRebuildCoef;
};


class Q_Exp_TXFilter_Thread : public QExpBaseThread
{
public:
    explicit Q_Exp_TXFilter_Thread(const CalParam &param) :
        QExpBaseThread(param) {}
    void run();
};

} // namespace NS_SP9500Pro

#endif // SP9500PRO_CAL_TX_FILTER_THREAD_H
