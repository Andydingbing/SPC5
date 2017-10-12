#ifndef QDEVICEINITTHREAD_H
#define QDEVICEINITTHREAD_H

#include "qwinthread.h"

class QDeviceInitThread : public QWinThread
{
    Q_OBJECT

public:
    QDeviceInitThread(QObject *parent = Q_NULLPTR);
    void run();
signals:
    void swhwVerReady(const CSP3301::RFUInfo &Info,const char *pDriver);
};

#endif // QDEVICEINITTHREAD_H
