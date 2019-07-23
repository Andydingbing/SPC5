#ifndef Q_ATTACH_THREAD_DLG_H
#define Q_ATTACH_THREAD_DLG_H

#include "q_winthread.h"

#define THREAD_EXISTED_CHK() \
    if (!QWinThread::g_threadStop) { \
        threadErrorBox(QWinThread::g_threadName.toStdString().c_str()); \
        return; \
    } \
    QWinThread::g_threadStop = false;

#define CAL_THREAD_EXISTED_CHK()  THREAD_EXISTED_CHK()
#define TEST_THREAD_EXISTED_CHK() THREAD_EXISTED_CHK()

class QAttachThreadDlg
{
public:
    QAttachThreadDlg()
    {
        running.clear(); idle.clear();
        pausing.clear(); resumed.clear();
    }

    void addRunningWidget(QWidget *child)
    { running.push_back(child); }

    void addIdleWidget(QWidget *child)
    { idle.push_back(child); }

    void addToPauseWidget(QWidget *child)
    { resumed.push_back(child); }

    void addToRunningWidget(QWidget *child)
    { pausing.push_back(child); }

    void widgetPrepare();
    void widgetResume();
    void widgetPausing();
    void widgetRunning();
    void widgetDisable();

private:
    vector<QWidget *> running;
    vector<QWidget *> idle;
    vector<QWidget *> pausing;
    vector<QWidget *> resumed;
};

#endif // Q_ATTACH_THREAD_DLG_H
