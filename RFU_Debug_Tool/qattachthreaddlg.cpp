#include "qattachthreaddlg.h"

QAttachThreadDlg::QAttachThreadDlg(QWidget *parent) : QDialog(parent)
{
    m_Running.clear();
    m_Idle.clear();
    m_Pausing.clear();
    m_Resumed.clear();
}

void QAttachThreadDlg::addRunningWidget(QWidget *pChild)
{
    m_Running.push_back(pChild);
}

void QAttachThreadDlg::addIdleWidget(QWidget *pChild)
{
    m_Idle.push_back(pChild);
}

void QAttachThreadDlg::addToPauseWidget(QWidget *pChild)
{
    m_Resumed.push_back(pChild);
}

void QAttachThreadDlg::addToRunningWidget(QWidget *pChild)
{
    m_Pausing.push_back(pChild);
}

void QAttachThreadDlg::widgetPrepare()
{
    for (size_t i = 0;i < m_Running.size();i ++)
        m_Running.at(i)->setEnabled(true);
    for (size_t i = 0;i < m_Idle.size();i ++)
        m_Idle.at(i)->setEnabled(false);
    return widgetRunning();
}

void QAttachThreadDlg::widgetResume()
{
    for (size_t i = 0;i < m_Running.size();i ++)
        m_Running.at(i)->setEnabled(false);
    for (size_t i = 0;i < m_Idle.size();i ++)
        m_Idle.at(i)->setEnabled(true);
    for (size_t i = 0;i < m_Pausing.size();i ++)
        m_Pausing.at(i)->setEnabled(false);
    for (size_t i = 0;i < m_Resumed.size();i ++)
        m_Resumed.at(i)->setEnabled(false);
}

void QAttachThreadDlg::widgetPausing()
{
    for (size_t i = 0;i < m_Pausing.size();i ++)
        m_Pausing.at(i)->setEnabled(true);
    for (size_t i = 0;i < m_Resumed.size();i ++)
        m_Resumed.at(i)->setEnabled(false);
}

void QAttachThreadDlg::widgetRunning()
{
    for (size_t i = 0;i < m_Pausing.size();i ++)
        m_Pausing.at(i)->setEnabled(false);
    for (size_t i = 0;i < m_Resumed.size();i ++)
        m_Resumed.at(i)->setEnabled(true);
}

void QAttachThreadDlg::widgetDisable()
{
    for (size_t i = 0;i < m_Running.size();i ++)
        m_Running.at(i)->setEnabled(false);
    for (size_t i = 0;i < m_Idle.size();i ++)
        m_Idle.at(i)->setEnabled(false);
}

void QAttachThreadDlg::done()
{
    QWinThread::g_bStop = true;
    widgetResume();
}
