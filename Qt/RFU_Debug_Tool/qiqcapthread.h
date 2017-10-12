#ifndef QIQCAPTHREAD_H
#define QIQCAPTHREAD_H

#include "qwinthread.h"

class QIQCapThread : public QWinThread
{
    Q_OBJECT
public:
    typedef struct Param {
        ISP1401 *m_pSP1401[MAX_RF];
        bool m_bVisible[MAX_RF];
    }Param;
public:
    explicit QIQCapThread(Param &param,QObject *parent = Q_NULLPTR);
    void run();
signals:
    void updatePlot();
public slots:
    void tabIdxChanged(int iIdx);
private:
    Param m_Param;
};

#endif // QIQCAPTHREAD_H
