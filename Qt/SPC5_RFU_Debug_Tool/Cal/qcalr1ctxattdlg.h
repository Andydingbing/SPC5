#ifndef QCALR1CTXATTDLG_H
#define QCALR1CTXATTDLG_H

#include "qcalbasedlg.h"
#include "qr1ctxattmodel.h"
#include "define.h"

namespace Ui {
class QCalR1CTxAttDlg;
}

class QCalR1CTxAttDlg : public QCalR1CBaseDlg
{
    Q_OBJECT
    DECLARE_CAL_R1C_DLG

public:
    explicit QCalR1CTxAttDlg(QWidget *parent = 0);
    ~QCalR1CTxAttDlg();

private:
    Ui::QCalR1CTxAttDlg *ui;
};

#endif // QCALR1CTXATTDLG_H
