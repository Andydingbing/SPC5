#include "q_winthread.h"
#include <QMessageBox>
#include "mainwindow.h"
#include <QDateTime>
#include "algorithm.h"

bool QWinThread::g_threadStop = true;
bool QWinThread::g_threadPausing = false;
QString QWinThread::g_threadName = QString("");
QWinThread *QWinThread::g_threadThread = nullptr;

CalParam::CalParam()
{
    parent = nullptr;
    rfFreqStar = QString("%1").arg(RF_TX_FREQ_STAR);
    rfFreqStop = QString("%1").arg(RF_TX_FREQ_STOP);
    rfFreqStep = QString("%1").arg(RF_TX_FREQ_STEP_CALLED);
    model_0 = nullptr;
    model_1 = nullptr;
    _sp1401 = nullptr;
    _sp2401 = nullptr;
    _sp3501 = nullptr;
    calX9119 = true;
    methodLOLeak = M_Spectrum;
    methodSB = M_Spectrum;
    justRebuildCoef = false;
}

CalR1CParam::CalR1CParam()
{
    txMode = CAL_IO_OP;
    rxMode = CAL_IO_OP;
}

TestBaseParam::TestBaseParam()
{
    parent = nullptr;
    model_0 = nullptr;
    model_1 = nullptr;
    SP1401 = nullptr;
    SP2401 = nullptr;
    SP3301 = nullptr;
}

QWinThread::QWinThread(QObject *parent) :
    QThread(parent)
{
    connect(this,SIGNAL(finished()),this,SLOT(deleteLater()));
    connect(this,SIGNAL(initProg(QString,quint32)),g_MainW,SLOT(initProg(QString,quint32)));
    connect(this,SIGNAL(setProgPos(quint32)),g_MainW,SLOT(setProgPos(quint32)));

    connect(this,SIGNAL(threadCheckBox(QString)),
           g_MainW,SLOT(threadCheckBox(QString)),Qt::BlockingQueuedConnection);

    connect(this,SIGNAL(threadErrorBox(QString)),
           g_MainW,SLOT(threadErrorBox(QString)),Qt::BlockingQueuedConnection);
}

QCalBaseThread::QCalBaseThread(const CalParam &param) :
    QWinThread(param.parent) ,
    calParam(param)
{
    freqString.star = param.rfFreqStar.toStdString();
    freqString.stop = param.rfFreqStop.toStdString();
    freqString.step = param.rfFreqStep.toStdString();

    parse_range_freq_string(freqString,freqRange);

    RFVer = param._sp1401->get_hw_ver();

    if (this->parent() != nullptr) {
        connect(this,SIGNAL(update(QModelIndex,QModelIndex,cal_file::cal_item_t,int)),
        this->parent(),SLOT(update(QModelIndex,QModelIndex,cal_file::cal_item_t,int)));

        connect(this,SIGNAL(done(bool)),this->parent(),SLOT(done()));
    }
    if (calParam.model_0 != nullptr) {
        connect(this,SIGNAL(update(QModelIndex,QModelIndex,cal_file::cal_item_t,int)),
      calParam.model_0,SLOT(update(QModelIndex,QModelIndex,cal_file::cal_item_t,int)));
    }
    if (calParam.model_1 != nullptr) {
        connect(this,SIGNAL(update(QModelIndex,QModelIndex,cal_file::cal_item_t,int)),
      calParam.model_1,SLOT(update(QModelIndex,QModelIndex,cal_file::cal_item_t,int)));
    }
}

tm QCalBaseThread::getCurTime()
{
    time_t curTime;
    time(&curTime);
    return *localtime(&curTime);
}

bool calOP(CalIOMode mode)
{
    return (mode == CAL_OP || mode == CAL_IO_OP);
}

bool calIO(CalIOMode mode)
{
    return (mode == CAL_IO || mode == CAL_IO_OP);
}

int exeFirProcess(char *path)
{
    PROCESS_INFORMATION pinfo;
    STARTUPINFOA sinfo;

    ZeroMemory(&sinfo,sizeof(sinfo) );
    sinfo.cb = sizeof(sinfo);
    ZeroMemory(&pinfo,sizeof(pinfo) );

    if(!CreateProcessA(nullptr,LPSTR(path),
                       nullptr,nullptr,FALSE,
                       NORMAL_PRIORITY_CLASS,
                       nullptr,nullptr,&sinfo,&pinfo)) {
        Log.set_last_err(-1,"cannot execute \"%s\"(%d)",path,GetLastError());
        return -1;
    }

    DWORD ret = WaitForSingleObject(pinfo.hProcess,5 * 60 * 1000);
    if (WAIT_TIMEOUT == ret) {
        Log.set_last_err(-1,"process %s timeout");
        return -1;
    }

    CloseHandle(pinfo.hProcess);
    CloseHandle(pinfo.hThread);
    return 0;
}

void getADS5474(basic_sp1401 *sp1401,qint64 &ad,qint32 avgTime)
{
    ad = 0;
    qint64 adAvg = 0;
    for (qint32 avg = 0;avg < avgTime;avg ++) {
        sp1401->get_ads5474(adAvg);
        ad += adAvg;
    }
    ad /= avgTime;
}

#define MSG_BOX_MSG_FIXED_LENGTH 50

void fmtMsg(QString &msg)
{
    int length = msg.length();

    if (length < MSG_BOX_MSG_FIXED_LENGTH) {
        int spaceL = (MSG_BOX_MSG_FIXED_LENGTH - length) / 2;
        int spaceR = (MSG_BOX_MSG_FIXED_LENGTH - length - spaceL);
        for (int i = 0;i < spaceL;i ++) {
            msg.insert(0," ");
        }
        for (int i = 0;i < spaceR;i ++) {
            msg.append(" ");
        }
    }
}

void threadCheckBox(const char *format, ...)
{
    char msg[512] = {0};
    ZERO_ARRAY(msg);

    va_list ap;
    va_start(ap,format);
    vsprintf(msg,format,ap);
    va_end(ap);

    QString strMsg = QString("%1").arg(msg);
    fmtMsg(strMsg);

    QMessageBox msgBox;
    msgBox.setText(strMsg);
    msgBox.setIcon(QMessageBox::Question);
    msgBox.setStandardButtons(QMessageBox::No | QMessageBox::Yes);
    msgBox.setDefaultButton(QMessageBox::Yes);
    msgBox.setModal(true);

    if (msgBox.exec() == QMessageBox::No) {
        QWinThread::g_threadStop = true;
    }
}

void threadErrorBox(const char *format, ...)
{
    char msg[512] = {0};
    ZERO_ARRAY(msg);

    va_list ap;
    va_start(ap,format);
    vsprintf(msg,format,ap);
    va_end(ap);

    QMessageBox msgBox;
    if (strlen(msg) < 1) {
        msgBox.setText(QString("Error When Creating Thread"));
        msgBox.setInformativeText(QString("Another thread may running.Try to stop it!"));
    } else {
        msgBox.setText(QString("        %1        ").arg(msg));
    }
    msgBox.setIcon(QMessageBox::Critical);
    msgBox.setModal(true);
    msgBox.exec();
}

bool ftpRetryBox()
{
    QMessageBox msgBox;
    msgBox.setText("FTP server disconnected!Retry?");
    msgBox.setIcon(QMessageBox::Question);
    msgBox.setStandardButtons(QMessageBox::No | QMessageBox::Yes);
    msgBox.setDefaultButton(QMessageBox::Yes);
    msgBox.setModal(true);

    return msgBox.exec() == QMessageBox::Yes;
}
