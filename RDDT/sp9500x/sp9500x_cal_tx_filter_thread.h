#ifndef SP9500X_CAL_TX_FILTER_THREAD_H
#define SP9500X_CAL_TX_FILTER_THREAD_H

#include "q_winthread.h"
#include "q_model_tx_filter.h"

namespace NS_SP9500X {

class Q_Cal_TXFilter_Thread : public QCalBaseThread
{
public:
    explicit Q_Cal_TXFilter_Thread(const CalParam &param) :
        QCalBaseThread(param) { }
    void cal();
    void check();

    void init();
    void sweepRF_IF_0000_3000(const uint64_t step);
    void sweepRF_3000_4800(const uint64_t step);
    void sweepRF_4800_6000(const uint64_t step);
    void sweepRF_6000_7500(const uint64_t step);
    void sweepIF_3000_7500(const uint64_t step);
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

} // namespace NS_SP9500X

#endif // SP9500X_CAL_TX_FILTER_THREAD_H
