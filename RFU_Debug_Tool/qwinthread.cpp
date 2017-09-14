#include "qwinthread.h"

bool QWinThread::m_bStop = true;
QString QWinThread::m_strProc = QString("");
QWinThread *g_pThread = NULL;

QWinThread::QWinThread(QObject *parent) : QThread(parent)
{

}
