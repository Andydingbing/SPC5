#ifndef QCALR1CTXSBDLG_H
#define QCALR1CTXSBDLG_H

#include "qcalbasedlg.h"
#include "qr1ctxsbmodel.h"
#include "define.h"

namespace Ui {
class QCalR1CTxSBDlg;
}

class QCalR1CTxSBDlg : public QCalR1CBaseDlg
{
    Q_OBJECT
    DECLARE_CAL_R1C_DLG

public:
    explicit QCalR1CTxSBDlg(QWidget *parent = 0);
    ~QCalR1CTxSBDlg();

private:
    Ui::QCalR1CTxSBDlg *ui;
};

#endif // QCALR1CTXSBDLG_H
