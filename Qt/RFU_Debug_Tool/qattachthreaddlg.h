#ifndef QATTACHTHREADDLG_H
#define QATTACHTHREADDLG_H

#include <QDialog>
#include "qwinthread.h"

#define THREAD_EXISTED_CHK()                                            \
    if (!QWinThread::g_bStop) {                                         \
        threadErrorBox(QWinThread::g_strProc.toStdString().c_str());    \
        return;                                                         \
    }                                                                   \
    QWinThread::g_bStop = false;

#define CAL_THREAD_EXISTED_CHK() THREAD_EXISTED_CHK()
#define TEST_THREAD_EXISTED_CHK() THREAD_EXISTED_CHK()

class QAttachThreadDlg : public QDialog
{
    Q_OBJECT

public:
    QAttachThreadDlg(QWidget *parent = 0);
    void addRunningWidget(QWidget *pChild);
    void addIdleWidget(QWidget *pChild);
    void addToPauseWidget(QWidget *pChild);
    void addToRunningWidget(QWidget *pChild);
    void widgetPrepare();
    void widgetResume();
    void widgetPausing();
    void widgetRunning();
    void widgetDisable();
public slots:
    virtual void done();
private:
    vector<QWidget *> m_Running;
    vector<QWidget *> m_Idle;
    vector<QWidget *> m_Pausing;
    vector<QWidget *> m_Resumed;
};

#endif // QATTACHTHREADDLG_H
