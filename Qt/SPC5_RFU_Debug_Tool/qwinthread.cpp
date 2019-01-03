#include "qwinthread.h"
#include <QMessageBox>
#include "mainwindow.h"

bool QWinThread::g_threadStop = true;
QString QWinThread::g_threadName = QString("");
QWinThread *QWinThread::g_threadThread = NULL;

CalParam::CalParam()
{
    parent = NULL;
    model_0 = NULL;
    model_1 = NULL;
    _sp1401 = NULL;
    _sp2401 = NULL;
    _sp3501 = NULL;
    calX9119 = true;
    methodLOLeak = M_Spectrum;
    methodSB = M_Spectrum;
    justRebuildCoef = false;
}

QWinThread::QWinThread(QObject *parent) : QThread(parent)
{
    connect(this,SIGNAL(finished()),this,SLOT(deleteLater()));
    connect(this,SIGNAL(initProg(QString,int)),g_MainW,SLOT(initProg(QString,int)));
    connect(this,SIGNAL(setProgPos(int)),g_MainW,SLOT(setProgPos(int)));
    connect(this,SIGNAL(threadCheckBox(QString)),g_MainW,SLOT(threadCheckBox(QString)),Qt::BlockingQueuedConnection);
    connect(this,SIGNAL(threadErrorBox(QString)),g_MainW,SLOT(threadErrorBox(QString)),Qt::BlockingQueuedConnection);
}

QCalBaseThread::QCalBaseThread(CalParam *param, QObject *parent) : QWinThread(parent)
{
    calParam.parent             = param->parent;
    calParam.mode               = param->mode;
    calParam.model_0            = param->model_0;
    calParam.model_1            = param->model_1;
    calParam.plotData_0         = param->plotData_0;
    calParam.plotData_1         = param->plotData_1;
    calParam.plotData_2         = param->plotData_2;
    calParam._sp1401            = param->_sp1401;
    calParam._sp2401            = param->_sp2401;
    calParam._sp3501            = param->_sp3501;
    calParam.rfFreqStar         = param->rfFreqStar;
    calParam.rfFreqStop         = param->rfFreqStop;
    calParam.rfFreqStep         = param->rfFreqStep;
    calParam.ifFreqStar         = param->ifFreqStar;
    calParam.ifFreqStop         = param->ifFreqStop;
    calParam.ifFreqStep         = param->ifFreqStep;
    calParam.calX9119           = param->calX9119;
    calParam.methodLOLeak       = param->methodLOLeak;
    calParam.methodSB           = param->methodSB;
    calParam.justRebuildCoef    = param->justRebuildCoef;

    if (calParam.parent) {
        connect(this,SIGNAL(update(QModelIndex,QModelIndex,cal_file::cal_item_t,int)),calParam.parent,SLOT(update(QModelIndex,QModelIndex,cal_file::cal_item_t,int)));
        connect(this,SIGNAL(done(bool)),calParam.parent,SLOT(done()));
    }
    if (calParam.model_0)
        connect(this,SIGNAL(update(QModelIndex,QModelIndex,cal_file::cal_item_t,int)),calParam.model_0,SLOT(update(QModelIndex,QModelIndex,cal_file::cal_item_t,int)));
    if (calParam.model_1)
        connect(this,SIGNAL(update(QModelIndex,QModelIndex,cal_file::cal_item_t,int)),calParam.model_1,SLOT(update(QModelIndex,QModelIndex,cal_file::cal_item_t,int)));
}

tm QCalBaseThread::getCurTime()
{
    time_t curTime;
    time(&curTime);
    return *localtime(&curTime);
}

bool calOP(CalParam::CalIOMode mode)
{
    return (mode == CalParam::CAL_OP || mode == CalParam::CAL_IO_OP);
}

bool calIO(CalParam::CalIOMode mode)
{
    return (mode == CalParam::CAL_IO || mode == CalParam::CAL_IO_OP);
}

int exeFirProcess(char *path)
{
    PROCESS_INFORMATION pinfo;
    STARTUPINFOA sinfo;

    ZeroMemory(&sinfo,sizeof(sinfo) );
    sinfo.cb = sizeof(sinfo);
    ZeroMemory(&pinfo,sizeof(pinfo) );

    if(!CreateProcessA( NULL,(LPSTR)path,NULL,NULL,FALSE,NORMAL_PRIORITY_CLASS,NULL,NULL,&sinfo,&pinfo)) {
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

void threadCheckBox(const char *format,...)
{
    char msg[512] = {0};
    memset(msg,0,sizeof(msg));

    va_list ap;
    va_start(ap,format);
    vsprintf(msg,format,ap);
    va_end(ap);

    QMessageBox msgBox;
    msgBox.setText(QString("%1").arg(msg));
    msgBox.setIcon(QMessageBox::Question);
    msgBox.setStandardButtons(QMessageBox::No | QMessageBox::Yes);
    msgBox.setDefaultButton(QMessageBox::Yes);
    msgBox.setModal(true);
    int ret = msgBox.exec();

    if (QMessageBox::No == ret)
        QWinThread::g_threadStop = true;
}

void threadErrorBox(const char *format,...)
{
    char msg[512] = {0};
    memset(msg,0,sizeof(msg));

    va_list ap;
    va_start(ap,format);
    vsprintf(msg,format,ap);
    va_end(ap);

    QMessageBox msgBox;
    if (strlen(msg) < 1) {
        msgBox.setText(QString("Error When Creating Thread"));
        msgBox.setInformativeText(QString("Maybe another unnamed thread is running,try to stop it"));
    }
    else
        msgBox.setText(QString("        %1        ").arg(msg));
    msgBox.setIcon(QMessageBox::Critical);
    msgBox.setModal(true);
    msgBox.exec();
}

int freq2array(QString in1, QString in2, QString step, int64_t *out, int64_t *freqstar, int64_t *freqstop, int64_t *max, int64_t *min)
{
    int64_t i_step = 0;
    str2freq(step.toStdString().c_str(),i_step);
    return freq2array(in1.toStdString().c_str(),in2.toStdString().c_str(),i_step,out,freqstar,freqstop,max,min);
}
