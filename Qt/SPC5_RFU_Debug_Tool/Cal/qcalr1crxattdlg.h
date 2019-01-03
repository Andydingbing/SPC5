#ifndef QCALR1CRXATTDLG_H
#define QCALR1CRXATTDLG_H

#include "qcalbasedlg.h"
#include "qr1crxattmodel.h"
#include "define.h"

namespace Ui {
class QCalR1CRxAttDlg;
}

class QCalR1CRxAttDlg : public QCalR1CBaseDlg
{
    Q_OBJECT
    DECLARE_CAL_R1C_DLG

public:
    explicit QCalR1CRxAttDlg(QWidget *parent = 0);
    ~QCalR1CRxAttDlg();

private:
    Ui::QCalR1CRxAttDlg *ui;
};

#endif // QCALR1CRXATTDLG_H
