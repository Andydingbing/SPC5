#ifndef QRFDLG_H
#define QRFDLG_H

#include <QWidget>
#include "define.h"

class QRfDlg : public QWidget
{
    Q_OBJECT

public:
    explicit QRfDlg(QWidget *parent = 0);
    void setSP3301(sp3301 *);
    void setRfIdx(qint8 idx);
    basic_sp1401 *getSP1401();
    sp1401_r1a *getSP1401R1A();
    sp1401_r1c *getSP1401R1C();
    sp2401_r1a *getSP2401R1A();

protected:
    sp3301 *_sp3301;
    quint8 rfIdx;
};

#endif // QRFDLG_H
