#ifndef QIQCAPTHREAD_H
#define QIQCAPTHREAD_H

#include "qwinthread.h"

class QIQCapThread : public QWinThread
{
    Q_OBJECT
public:
    typedef struct Param {
        basic_sp1401 *_sp1401[MAX_RF];
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
