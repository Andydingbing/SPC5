#include "q_device_init_thread.h"
#include "mainwindow.h"

QDeviceInitThread::QDeviceInitThread(QObject *parent) : QWinThread(parent)
{

}

void QDeviceInitThread::run()
{
    initProgress(tr("Initialization"),100);
    MainWindow *parent = (MainWindow *)(this->parent());
    sp3301 *pSP3301 = parent->_sp3301;
    sp3501 *pSP3501 = parent->_sp3501;
    sp3301::rfu_info_t rfuInfo;
    const char *driverVer = pSP3301->get_driver_ver();

    emit(swhwVerReady(rfuInfo,driverVer));

    SET_PROG_POS(20);
    INT_CHECKV(pSP3501->open_board());
    SET_PROG_POS(60);
    INT_CHECKV(pSP3301->set_program_bit(nullptr,false,false));
    INT_CHECKV(pSP3301->program_bit());
    if (pSP3301->boot()) {
        Log.add_msg(-1,"boot fail");
        return;
    }
    SET_PROG_POS(90);

    parent->updateParamInChildDlg();

    rfuInfo = pSP3301->get_info();
    emit swhwVerReady(rfuInfo,driverVer);
    SET_PROG_POS(100);
    Log.add_msg(0,"booted");
    emit done(true);
}
