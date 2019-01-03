#ifndef QCALR1CRXFILTERDLG_H
#define QCALR1CRXFILTERDLG_H

#include "qcalbasedlg.h"
#include "define.h"

namespace Ui {
class QCalR1CRxFilterDlg;
}

class QCalR1CRxFilterDlg : public QCalR1CBaseDlg
{
    Q_OBJECT
    DECLARE_CAL_R1C_DLG

public:
    explicit QCalR1CRxFilterDlg(QWidget *parent = 0);
    ~QCalR1CRxFilterDlg();

private:
    Ui::QCalR1CRxFilterDlg *ui;
};

#endif // QCALR1CRXFILTERDLG_H
