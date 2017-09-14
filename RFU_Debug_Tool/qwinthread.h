#ifndef QWINTHREAD_H
#define QWINTHREAD_H

#include <QThread>
#include "define.h"
#include "libSP3301.h"

class QWinThread : public QThread
{
    Q_OBJECT

public:
    QWinThread(QObject *parent = Q_NULLPTR);
signals:
    void InitProg(const QString strName,int iPts);
    void SetProgPos(int iPos);
public:
    int32_t m_iFuncIdx;
    static bool m_bStop;
    static QString  m_strProc;
    static QWinThread *g_pThread;
};

extern QWinThread *g_pThread;
#endif // QWINTHREAD_H
