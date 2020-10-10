#include "q_winthread.h"
#include <QMessageBox>
#include "mainwindow.h"
#include <QDateTime>
#include "q_test_tx_io_sw_msg_dlg.h"
#include "q_cal_base_dlg.h"

#ifdef RD_PLATFORM_WIN32
    #include <Windows.h>
#endif

bool QWinThread::g_threadStop = true;
bool QWinThread::g_threadPausing = false;
QString QWinThread::g_threadName = QString("");
QWinThread *QWinThread::g_threadThread = nullptr;

QWinThread::QWinThread(QObject *parent) :
    QThread(parent)
{
    connect(this,SIGNAL(finished()),this,SLOT(deleteLater()));

    connect(this,SIGNAL(initProg(QString)),g_MainW,SLOT(initProg(QString)));
    connect(this,SIGNAL(initProg(QString,quint32)),g_MainW,SLOT(initProg(QString,quint32)));
    connect(this,SIGNAL(setProgPos(quint32)),g_MainW,SLOT(setProgPos(quint32)));
    connect(this,SIGNAL(addProgPos(quint32)),g_MainW,SLOT(addProgPos(quint32)));

    connect(this,SIGNAL(threadCheckBox(QString)),
           g_MainW,SLOT(threadCheckBox(QString)),Qt::BlockingQueuedConnection);

    connect(this,SIGNAL(threadErrorBox(QString)),
           g_MainW,SLOT(threadErrorBox(QString)),Qt::BlockingQueuedConnection);
}

QCalBaseThread::QCalBaseThread(const CalParam &param) :
    QCalTestBaseThread(param.parent),
    tempCur(0.0),
    tempMin(100.0),
    tempMax(-100.0),
    calParam(param)
{
    SP3301 = calParam.SP3301;

    freqStringCal.star = calParam.rfFreqStar.toStdString();
    freqStringCal.stop = calParam.rfFreqStop.toStdString();
    freqStringCal.step = calParam.rfFreqStep.toStdString();
    freqStringCheck = calParam.freqStringCheck;

    parse_range_freq_string(freqStringCal,freqRangeCal);
    parse_range_freq_string(freqStringCheck,freqRangeCheck);

    if (calOP(calParam.mode)) {
        if (calParam.cal) { totalPts += freqRangeCal.freqs.size(); }
        if (calParam.check) { totalPts += freqRangeCheck.freqs.size(); }
    }
    if (calIO(calParam.mode)) {
        if (calParam.cal) { totalPts += freqRangeCal.freqs.size(); }
        if (calParam.check) { totalPts += freqRangeCheck.freqs.size(); }
    }

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

    connect(this,&QCalBaseThread::threadProcess,g_MainW,&MainWindow::threadProcess);
}

bool QCalBaseThread::checkTemp()
{
    tempMin = tempCur < tempMin ? tempCur : tempMin;
    tempMax = tempCur > tempMax ? tempCur : tempMax;

    return (tempMax - tempMin < tempRange);
}

QTestThread::QTestThread(const TestBaseParam *param) :
    QCalTestBaseThread(param->parent)
{
    Param = const_cast<TestBaseParam *>(param);

    if (SP1401 != nullptr) {
        RFVer = SP1401->get_hw_ver();
    }

    if (this->parent() != nullptr) {
        connect(this,&QTestThread::update,Param->parent,&QTestBaseDlg::update);
        connect(this,&QTestThread::done,Param->parent,&QTestBaseDlg::done);
    }

    if (Param != nullptr) {
        if (Param->model_0 != nullptr) { connect(this,&QTestThread::update,Param->model_0,&QTestBaseModel::update2); }
        if (Param->model_1 != nullptr) { connect(this,&QTestThread::update,Param->model_1,&QTestBaseModel::update2); }
    }
}

tm QCalBaseThread::getCurTime()
{
    time_t curTime;
    time(&curTime);
    return *localtime(&curTime);
}

int exeFirProcess(char *path)
{
#ifdef RD_PLATFORM_WIN32
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
#else
#endif
    return 0;
}

void getADS5474(sp1401 *sp1401,qint64 &ad,qint32 avgTime)
{
    ad = 0;
    int64_t adAvg = 0;
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

void testTXIOSwBox(const QColor TX, const QColor RX, int &exec)
{
    QTestTXIOSwMsgDlg msgBox(TX,RX);
    msgBox.setWindowTitle(QObject::tr("Check LED"));

#if (QT_VERSION >= QT_VERSION_CHECK(5,9,0))
    msgBox.setWindowFlag(Qt::WindowCloseButtonHint,false);
    msgBox.setWindowFlag(Qt::WindowContextHelpButtonHint,false);
#else
    Qt::WindowFlags flags = msgBox.windowFlags();
    flags &= ~Qt::WindowCloseButtonHint;
    flags &= ~Qt::WindowContextHelpButtonHint;
    msgBox.setWindowFlags(flags);
#endif

    msgBox.setFixedSize(msgBox.size());
    msgBox.setModal(true);

    exec = msgBox.exec();
}
