#include "qwinthread.h"
#include <QMessageBox>
#include "mainwindow.h"

bool QWinThread::g_bStop = true;
QString QWinThread::g_strProc = QString("");
QWinThread *g_pThread = NULL;

QWinThread::QWinThread(QObject *parent) : QThread(parent)
{
    connect(this,&QWinThread::finished,this,&QObject::deleteLater);
    connect(this,&QWinThread::initProg,g_pMainW,&MainWindow::initProg);
    connect(this,&QWinThread::setProgPos,g_pMainW,&MainWindow::setProgPos);
    connect(this,&QWinThread::threadCheckBox,g_pMainW,&MainWindow::threadCheckBox,Qt::BlockingQueuedConnection);
    connect(this,&QWinThread::threadErrorBox,g_pMainW,&MainWindow::threadErrorBox,Qt::BlockingQueuedConnection);
}

QCalBaseThread::QCalBaseThread(CalParam *pParam, QObject *parent) : QWinThread(parent)
{
    m_CalParam.m_pParent = pParam->m_pParent;
    m_CalParam.m_pModel = pParam->m_pModel;
    m_CalParam.m_pSP1401 = pParam->m_pSP1401;
    m_CalParam.m_pSP2401 = pParam->m_pSP2401;
    m_CalParam.m_pSP3501 = pParam->m_pSP3501;
    m_CalParam.m_strRfFreqStar = pParam->m_strRfFreqStar;
    m_CalParam.m_strRfFreqStop = pParam->m_strRfFreqStop;
    m_CalParam.m_strRfFreqStep = pParam->m_strRfFreqStep;
    m_CalParam.m_strIfFreqStar = pParam->m_strIfFreqStar;
    m_CalParam.m_strIfFreqStop = pParam->m_strIfFreqStop;
    m_CalParam.m_strIfFreqStep = pParam->m_strIfFreqStep;
    m_CalParam.m_bCalX9119 = pParam->m_bCalX9119;
    m_CalParam.m_LOLeakMethod = pParam->m_LOLeakMethod;

    QCalBaseDlg *pParent = m_CalParam.m_pParent;
    QCalBaseModel *pModel = m_CalParam.m_pModel;
    connect(this,&QCalBaseThread::update,pParent,&QCalBaseDlg::update);
    connect(this,&QCalBaseThread::done,pParent,&QCalBaseDlg::done);
    connect(this,&QCalBaseThread::update,pModel,&QCalBaseModel::update);
}

void threadCheckBox(const char *format,...)
{
    char szMsg[512];
    memset(szMsg,0,sizeof(szMsg));

    va_list ap;
    va_start(ap,format);
    vsprintf(szMsg,format,ap);
    va_end(ap);

    QMessageBox msgBox;
    msgBox.setText(QString("%1").arg(szMsg));
    msgBox.setIcon(QMessageBox::Question);
    msgBox.setStandardButtons(QMessageBox::No | QMessageBox::Yes);
    msgBox.setDefaultButton(QMessageBox::Yes);  
    msgBox.setModal(true);
    int ret = msgBox.exec();

    if (QMessageBox::No == ret)
        QWinThread::g_bStop = true;
}

void threadErrorBox(const char *format,...)
{
    char szMsg[512];
    memset(szMsg,0,sizeof(szMsg));

    va_list ap;
    va_start(ap,format);
    vsprintf(szMsg,format,ap);
    va_end(ap);

    QMessageBox msgBox;
    if (strlen(szMsg) < 1) {
        msgBox.setText(QString("Error When Creating Thread"));
        msgBox.setInformativeText(QString("Maybe another unnamed thread is running,try to stop it"));
    }
    else
        msgBox.setText(QString("        %1        ").arg(szMsg));
    msgBox.setIcon(QMessageBox::Critical);
    msgBox.setModal(true);
    msgBox.exec();
}

int freq2array(QString pin1,QString pin2,QString step,int64_t *pout,int64_t *pfreqstar,int64_t *pfreqstop,int64_t *pmax,int64_t *pmin)
{
    int64_t istep = 0;
    str2freq(step.toStdString().c_str(),istep);
    return freq2array(pin1.toStdString().c_str(),pin2.toStdString().c_str(),istep,pout,pfreqstar,pfreqstop,pmax,pmin);
}
