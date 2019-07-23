#ifndef QR1CTEMPCTRLTHREAD_H
#define QR1CTEMPCTRLTHREAD_H

#include "q_winthread.h"
#include <QTimer>

class QR1CTempCtrlThread : public QWinThread
{
    Q_OBJECT
public:
      TempParam param;
public:
      QTimer *TempTimer;
public:
    explicit QR1CTempCtrlThread(TempParam *param,QObject *parent = 0);
    void run();
    void updatetemper(int x);
signals:
    void update(double *temp);

protected:

};

#endif // QCALR1CTXPWRTHREAD_H
