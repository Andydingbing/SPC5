#ifndef QCALR1CTXFILTEROFFSETTHREAD_H
#define QCALR1CTXFILTEROFFSETTHREAD_H

#include "qwinthread.h"

class QCalR1CTxFilterOffsetThread : public QCalBaseThread
{
    Q_OBJECT

public:
    explicit QCalR1CTxFilterOffsetThread(CalParam *param,QObject *parent = 0);
    void run();
};

class QExportR1CTxFilterOffsetThread : public QExportBaseThread
{
    Q_OBJECT

public:
    explicit QExportR1CTxFilterOffsetThread(CalParam *param,QObject *parent = 0);
    void run();
};

#endif // QCALR1CTXFILTEROFFSETTHREAD_H
