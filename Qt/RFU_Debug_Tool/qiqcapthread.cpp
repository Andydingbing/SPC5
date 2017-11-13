#include "qiqcapthread.h"

QIQCapThread::QIQCapThread(Param &param, QObject *parent) : QWinThread(parent)
{
    m_Param = param;
}

void QIQCapThread::run()
{
    bool bCapture = false;
    int iIdx = 0;
    ISP1401 *pSP1401;
    while (1) {
        THREAD_TEST_CANCEL();
        bCapture = false;
        for (int i = 0;i < MAX_RF;i ++) {
            if (m_Param.m_bVisible[i]) {
                bCapture = true;
                iIdx = i;
                pSP1401 = m_Param.m_pSP1401[iIdx];
                break;
            }
        }
        if (bCapture) {
            INT_CHECKV(pSP1401->IQCap());
            INT_CHECKV(DDR.IQToBuf(pSP1401->GetRfIdx(),IQBUF.GetI(),IQBUF.GetQ(),IQBUF.TDGetSamples()));
            emit updatePlot();
        }
    }
}

void QIQCapThread::tabIdxChanged(int iIdx)
{
    for (int i = 0;i < MAX_RF;i ++)
        m_Param.m_bVisible[i] = false;
    if (-1 != iIdx)
        m_Param.m_bVisible[iIdx] = true;
}
