#include "q_iq_cap_thread.h"

QIQCapThread::QIQCapThread(Param &param, QObject *parent) : QWinThread(parent)
{
    this->param = param;
}

void QIQCapThread::run()
{
    bool capture = false;
    int idx = 0;
    while (1) {
        THREAD_TEST_CANCEL;
        capture = false;
        for (int i = 0;i < MAX_RF;i ++) {
            if (param.visible[i]) {
                capture = true;
                idx = i;
                break;
            }
        }
        if (capture) {
            INT_CHECKV(param._sp3301->iq_cap(idx));
            emit updatePlot();
        }
    }
}

void QIQCapThread::tabIdxChanged(int idx)
{
    for (int i = 0;i < MAX_RF;i ++)
        param.visible[i] = false;
    if (-1 != idx)
        param.visible[idx] = true;
}
