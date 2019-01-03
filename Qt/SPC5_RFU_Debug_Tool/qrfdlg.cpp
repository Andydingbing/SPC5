#include "qrfdlg.h"

QRfDlg::QRfDlg(QWidget *parent) : QWidget(parent)
{
    _sp3301 = nullptr;
    rfIdx = 0;
}

void QRfDlg::setSP3301(sp3301 *rfu)
{
    _sp3301 = rfu;
}

void QRfDlg::setRfIdx(qint8 idx)
{
    this->rfIdx = idx;
}

basic_sp1401 *QRfDlg::getSP1401()
{
    return _sp3301->get_sp1401(rfIdx);
}

sp1401_r1a *QRfDlg::getSP1401R1A()
{
    return _sp3301->get_sp1401_r1a(rfIdx);
}

sp1401_r1c *QRfDlg::getSP1401R1C()
{
    return _sp3301->get_sp1401_r1c(rfIdx);
}

sp2401_r1a *QRfDlg::getSP2401R1A()
{
    return _sp3301->get_sp2401(rfIdx);
}
