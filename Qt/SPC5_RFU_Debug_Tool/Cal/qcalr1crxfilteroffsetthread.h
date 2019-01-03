#ifndef QCALR1CRXFILTEROFFSETTHREAD_H
#define QCALR1CRXFILTEROFFSETTHREAD_H

#include "qwinthread.h"

class QCalR1CRxFilterOffsetThread : public QCalBaseThread
{
    Q_OBJECT

public:
    explicit QCalR1CRxFilterOffsetThread(CalParam *param,QObject *parent = 0);
    void run();
};

class QExportR1CRxFilterOffsetThread : public QExportBaseThread
{
    Q_OBJECT

public:
    explicit QExportR1CRxFilterOffsetThread(CalParam *param,QObject *parent = 0);
    void run();
};

#endif // QCALR1CRXFILTEROFFSETTHREAD_H
