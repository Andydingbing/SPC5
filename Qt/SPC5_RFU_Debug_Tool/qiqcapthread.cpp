#include "qiqcapthread.h"

QIQCapThread::QIQCapThread(Param &param, QObject *parent) : QWinThread(parent)
{
    this->param = param;
}

void QIQCapThread::run()
{
    bool capture = false;
    int idx = 0;
    basic_sp1401 *pSP1401;
    while (1) {
        THREAD_TEST_CANCEL;
        capture = false;
        for (int i = 0;i < MAX_RF;i ++) {
            if (param.visible[i]) {
                capture = true;
                idx = i;
                pSP1401 = param._sp1401[idx];
                break;
            }
        }
        if (capture) {
//            INT_CHECKV(pSP1401->iq_cap());
//            INT_CHECKV(DDR.iq2buf(pSP1401->get_rf_idx(),IQBUF.get_i(),IQBUF.get_q(),IQBUF.td_get_samples()));
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
