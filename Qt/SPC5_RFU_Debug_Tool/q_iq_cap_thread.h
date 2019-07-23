#ifndef QIQCAPTHREAD_H
#define QIQCAPTHREAD_H

#include "q_winthread.h"

class QIQCapThread : public QWinThread
{
    Q_OBJECT
public:
    typedef struct Param {
        sp3301 *_sp3301;
        bool visible[MAX_RF];
    }Param;
public:
    explicit QIQCapThread(Param &param,QObject *parent = 0);
    void run();
signals:
    void updatePlot();
public slots:
    void tabIdxChanged(int idx);
private:
    Param param;
};

#endif // QIQCAPTHREAD_H
