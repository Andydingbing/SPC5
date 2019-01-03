#include "qattachthreaddlg.h"

QAttachThreadDlg::QAttachThreadDlg(QWidget *parent) : QWidget(parent)
{
    running.clear();
    idle.clear();
    pausing.clear();
    resumed.clear();
}

void QAttachThreadDlg::addRunningWidget(QWidget *child)
{
    running.push_back(child);
}

void QAttachThreadDlg::addIdleWidget(QWidget *child)
{
    idle.push_back(child);
}

void QAttachThreadDlg::addToPauseWidget(QWidget *child)
{
    resumed.push_back(child);
}

void QAttachThreadDlg::addToRunningWidget(QWidget *child)
{
    pausing.push_back(child);
}

void QAttachThreadDlg::widgetPrepare()
{
    for (size_t i = 0;i < running.size();i ++)
        running.at(i)->setEnabled(true);
    for (size_t i = 0;i < idle.size();i ++)
        idle.at(i)->setEnabled(false);
    return widgetRunning();
}

void QAttachThreadDlg::widgetResume()
{
    for (size_t i = 0;i < running.size();i ++)
        running.at(i)->setEnabled(false);
    for (size_t i = 0;i < idle.size();i ++)
        idle.at(i)->setEnabled(true);
    for (size_t i = 0;i < pausing.size();i ++)
        pausing.at(i)->setEnabled(false);
    for (size_t i = 0;i < resumed.size();i ++)
        resumed.at(i)->setEnabled(false);
}

void QAttachThreadDlg::widgetPausing()
{
    for (size_t i = 0;i < pausing.size();i ++)
        pausing.at(i)->setEnabled(true);
    for (size_t i = 0;i < resumed.size();i ++)
        resumed.at(i)->setEnabled(false);
}

void QAttachThreadDlg::widgetRunning()
{
    for (size_t i = 0;i < pausing.size();i ++)
        pausing.at(i)->setEnabled(false);
    for (size_t i = 0;i < resumed.size();i ++)
        resumed.at(i)->setEnabled(true);
}

void QAttachThreadDlg::widgetDisable()
{
    for (size_t i = 0;i < running.size();i ++)
        running.at(i)->setEnabled(false);
    for (size_t i = 0;i < idle.size();i ++)
        idle.at(i)->setEnabled(false);
}

void QAttachThreadDlg::done()
{
    QWinThread::g_threadStop = true;
    widgetResume();
}
