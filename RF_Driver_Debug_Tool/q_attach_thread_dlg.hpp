#ifndef Q_ATTACH_THREAD_DLG_H
#define Q_ATTACH_THREAD_DLG_H

#include <QWidget>
#include <QVector>

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
        running.clear();
        idle.clear();
        pausing.clear();
        resumed.clear();
    }

    void addRunningWidget(QWidget *child)
    { running.push_back(child); }

    void addIdleWidget(QWidget *child)
    { idle.push_back(child); }

    void addToPauseWidget(QWidget *child)
    { resumed.push_back(child); }

    void addToRunningWidget(QWidget *child)
    { pausing.push_back(child); }

    void widgetPrepare()
    {
        for (int i = 0;i < running.size();i ++) {
            running.at(i)->setEnabled(true);
        }
        for (int i = 0;i < idle.size();i ++) {
            idle.at(i)->setEnabled(false);
        }
        return widgetRunning();
    }

    void widgetResume()
    {
        for (int i = 0;i < running.size();i ++) {
            running.at(i)->setEnabled(false);
        }
        for (int i = 0;i < idle.size();i ++) {
            idle.at(i)->setEnabled(true);
        }
        for (int i = 0;i < pausing.size();i ++) {
            pausing.at(i)->setEnabled(false);
        }
        for (int i = 0;i < resumed.size();i ++) {
            resumed.at(i)->setEnabled(false);
        }
    }

    void widgetPausing()
    {
        for (int i = 0;i < pausing.size();i ++) {
            pausing.at(i)->setEnabled(true);
        }
        for (int i = 0;i < resumed.size();i ++) {
            resumed.at(i)->setEnabled(false);
        }
    }

    void widgetRunning()
    {
        for (int i = 0;i < pausing.size();i ++) {
            pausing.at(i)->setEnabled(false);
        }
        for (int i = 0;i < resumed.size();i ++) {
            resumed.at(i)->setEnabled(true);
        }
    }

    void widgetDisable()
    {
        for (int i = 0;i < running.size();i ++) {
            running.at(i)->setEnabled(false);
        }
        for (int i = 0;i < idle.size();i ++) {
            idle.at(i)->setEnabled(false);
        }
    }

private:
    QVector<QWidget *> running;
    QVector<QWidget *> idle;
    QVector<QWidget *> pausing;
    QVector<QWidget *> resumed;
};

#endif // Q_ATTACH_THREAD_DLG_H
