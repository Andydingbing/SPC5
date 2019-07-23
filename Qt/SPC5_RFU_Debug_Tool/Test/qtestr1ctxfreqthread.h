#ifndef QTESTR1CTXFREQTHREAD_H
#define QTESTR1CTXFREQTHREAD_H

#include "q_winthread.h"

class QTestR1CTxFreqThread : public QTestBaseThread
{
    Q_OBJECT

public:
    explicit QTestR1CTxFreqThread(TestParam *param,QObject *parent = 0);
    void run();
};

// class QExportR1CTxFilterThread : public QExportBaseThread
// {
//     Q_OBJECT

// public:
//     explicit QExportR1CTxFilterThread(CalParam *param,QObject *parent = 0);
//     void run();
// };

// class QGetsetR1CTxFilterThread : public QGetsetBaseThread
// {
//     Q_OBJECT
// signals:
//     void updateCalUI(QList<QString> list);
// public:
//     explicit QGetsetR1CTxFilterThread(CalParam *param,QObject *parent = 0);
//     void run();
// };
#endif // QTESTR1CTXFREQTHREAD_H
