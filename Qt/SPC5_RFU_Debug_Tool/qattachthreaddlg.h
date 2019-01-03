#ifndef QATTACHTHREADDLG_H
#define QATTACHTHREADDLG_H

#include <QWidget>
#include "qwinthread.h"

#define THREAD_EXISTED_CHK()                                            \
    if (!QWinThread::g_threadStop) {                                    \
        threadErrorBox(QWinThread::g_threadName.toStdString().c_str()); \
        return;                                                         \
    }                                                                   \
    QWinThread::g_threadStop = false;

#define CAL_THREAD_EXISTED_CHK()  THREAD_EXISTED_CHK()
#define TEST_THREAD_EXISTED_CHK() THREAD_EXISTED_CHK()

class QAttachThreadDlg : public QWidget
{
    Q_OBJECT

public:
    QAttachThreadDlg(QWidget *parent = 0);
    void addRunningWidget(QWidget *child);
    void addIdleWidget(QWidget *child);
    void addToPauseWidget(QWidget *child);
    void addToRunningWidget(QWidget *child);
    void widgetPrepare();
    void widgetResume();
    void widgetPausing();
    void widgetRunning();
    void widgetDisable();
public slots:
    virtual void done();
private:
    vector<QWidget *> running;
    vector<QWidget *> idle;
    vector<QWidget *> pausing;
    vector<QWidget *> resumed;
};

#endif // QATTACHTHREADDLG_H
