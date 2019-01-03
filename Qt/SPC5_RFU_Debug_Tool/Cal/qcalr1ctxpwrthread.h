#ifndef QCALR1CTXPWRTHREAD_H
#define QCALR1CTXPWRTHREAD_H

#include "qwinthread.h"
#include "libsp3301.h"

class QCalR1CTxPwrThread : public QCalBaseThread
{
    Q_OBJECT

public:
    explicit QCalR1CTxPwrThread(CalParam *param,QObject *parent = 0);
    void run();
};

class QExportR1CTxPwrThread : public QExportBaseThread
{
    Q_OBJECT

public:
    explicit QExportR1CTxPwrThread(CalParam *param,QObject *parent = 0);
    void run();
};

#endif // QCALR1CTXPWRTHREAD_H
