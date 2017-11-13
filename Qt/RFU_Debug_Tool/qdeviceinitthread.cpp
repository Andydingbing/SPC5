#include "qdeviceinitthread.h"
#include "mainwindow.h"

QDeviceInitThread::QDeviceInitThread(QObject *parent) : QWinThread(parent)
{

}

void QDeviceInitThread::run()
{
    INIT_PROG(tr("Initialization"),100);
    MainWindow *pParent = (MainWindow *)(this->parent());
    CSP3301 *pSP3301 = pParent->m_pSP3301;
    CSP3501 *pSP3501 = pParent->m_pSP3501;
    CSP3301::RFUInfo Info;
    const char *pDriver = pSP3301->GetDriverVer();

    emit(swhwVerReady(Info,pDriver));

    SET_PROG_POS(20);
    INT_CHECKV(pSP3501->OpenBoard());
    SET_PROG_POS(60);
    INT_CHECKV(pSP3301->SetLoadBit(false,false));
    INT_CHECKV(pSP3301->LoadBit());
    if (pSP3301->Boot()) {
        Log.AddMsgList(-1,"boot fail");
        return;
    }
    SET_PROG_POS(90);

    Info = pSP3301->GetInfo();
    emit swhwVerReady(Info,pDriver);
    SET_PROG_POS(100);
    Log.AddMsgList(0,"booted");
    emit done(true);
}
