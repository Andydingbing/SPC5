#include "q_attach_thread_dlg.h"

void QAttachThreadDlg::widgetPrepare()
{
    for (size_t i = 0;i < running.size();i ++) {
        running.at(i)->setEnabled(true);
    }
    for (size_t i = 0;i < idle.size();i ++) {
        idle.at(i)->setEnabled(false);
    }
    return widgetRunning();
}

void QAttachThreadDlg::widgetResume()
{
    for (size_t i = 0;i < running.size();i ++) {
        running.at(i)->setEnabled(false);
    }
    for (size_t i = 0;i < idle.size();i ++) {
        idle.at(i)->setEnabled(true);
    }
    for (size_t i = 0;i < pausing.size();i ++) {
        pausing.at(i)->setEnabled(false);
    }
    for (size_t i = 0;i < resumed.size();i ++) {
        resumed.at(i)->setEnabled(false);
    }
}

void QAttachThreadDlg::widgetPausing()
{
    for (size_t i = 0;i < pausing.size();i ++) {
        pausing.at(i)->setEnabled(true);
    }
    for (size_t i = 0;i < resumed.size();i ++) {
        resumed.at(i)->setEnabled(false);
    }
}

void QAttachThreadDlg::widgetRunning()
{
    for (size_t i = 0;i < pausing.size();i ++) {
        pausing.at(i)->setEnabled(false);
    }
    for (size_t i = 0;i < resumed.size();i ++) {
        resumed.at(i)->setEnabled(true);
    }
}

void QAttachThreadDlg::widgetDisable()
{
    for (size_t i = 0;i < running.size();i ++) {
        running.at(i)->setEnabled(false);
    }
    for (size_t i = 0;i < idle.size();i ++) {
        idle.at(i)->setEnabled(false);
    }
}
