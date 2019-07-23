#ifndef QDEVICEINITTHREAD_H
#define QDEVICEINITTHREAD_H

#include "q_winthread.h"

class QDeviceInitThread : public QWinThread
{
    Q_OBJECT

public:
    QDeviceInitThread(QObject *parent = 0);
    void run();
signals:
    void swhwVerReady(const sp3301::rfu_info_t &info,const char *driver);
};

#endif // QDEVICEINITTHREAD_H
