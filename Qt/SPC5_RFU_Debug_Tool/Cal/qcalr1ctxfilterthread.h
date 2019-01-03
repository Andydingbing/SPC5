#ifndef QCALR1CTXFILTERTHREAD_H
#define QCALR1CTXFILTERTHREAD_H

#include "qwinthread.h"

class QCalR1CTxFilterThread : public QCalBaseThread
{
    Q_OBJECT

public:
    explicit QCalR1CTxFilterThread(CalParam *param,QObject *parent = 0);
    void run();
};

class QExportR1CTxFilterThread : public QExportBaseThread
{
    Q_OBJECT

public:
    explicit QExportR1CTxFilterThread(CalParam *param,QObject *parent = 0);
    void run();
};

#endif // QCALR1CTXFILTERTHREAD_H
