#ifndef Q_RF_DLG_H
#define Q_RF_DLG_H

#include <QWidget>
#include "define.h"

#define PTR_CHECK { \
    if (SP3301 == nullptr) { return; } \
    hw_ver_t ver = SP3301->get_rf_ver(rfIdx); \
    if (ver == R1F) { \
        if (SP1401R1F() == nullptr) { return; } \
    } else if (ver == R1E) { \
        if (SP1401R1E() == nullptr) { return; } \
    } else if (ver == R1C || ver == R1D) { \
        if (SP1401R1C() == nullptr) { return; } \
    } else if (ver == R1A || ver == R1B) { \
        if (SP1401R1A() == nullptr) { return; } \
    } \
    }

class QRFDlg : public QWidget
{
    Q_OBJECT

public:
    explicit QRFDlg(QWidget *parent = nullptr) :
        QWidget(parent),
        SP3301(nullptr),
        rfIdx(0) {}

    void setSP3301(sp3301 *rfu) { SP3301 = rfu; }
    void setRFIdx(quint8 idx) { rfIdx = idx; }

    basic_sp1401 *SP1401()
    { return SP3301->get_sp1401(rfIdx); }

    sp2401_r1a *SP2401R1A()
    { return SP3301->get_sp2401(rfIdx); }

    sp1401_r1a *SP1401R1A()
    { return dynamic_cast<sp1401_r1a *>(SP3301->get_sp1401(rfIdx)); }

    sp1401_r1c *SP1401R1C()
    { return dynamic_cast<sp1401_r1c *>(SP3301->get_sp1401(rfIdx)); }

    sp1401_r1e *SP1401R1E()
    { return dynamic_cast<sp1401_r1e *>(SP3301->get_sp1401(rfIdx)); }

    sp1401_r1f *SP1401R1F()
    { return dynamic_cast<sp1401_r1f *>(SP3301->get_sp1401(rfIdx)); }

protected:
    sp3301 *SP3301;
    quint8 rfIdx;
};

#endif // Q_RF_DLG_H
