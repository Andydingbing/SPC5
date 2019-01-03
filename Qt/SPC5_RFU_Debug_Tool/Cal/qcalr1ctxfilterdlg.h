#ifndef QCALR1CTXFILTERDLG_H
#define QCALR1CTXFILTERDLG_H

#include "qcalbasedlg.h"
#include "define.h"

namespace Ui {
class QCalR1CTxFilterDlg;
}

class QCalR1CTxFilterDlg : public QCalR1CBaseDlg
{
    Q_OBJECT
    DECLARE_CAL_R1C_DLG

public:
    explicit QCalR1CTxFilterDlg(QWidget *parent = 0);
    ~QCalR1CTxFilterDlg();

private:
    Ui::QCalR1CTxFilterDlg *ui;
};

#endif // QCALR1CTXFILTERDLG_H
