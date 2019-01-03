#ifndef QCALR1CRXFILTEROFFSETDLG_H
#define QCALR1CRXFILTEROFFSETDLG_H

#include "qcalbasedlg.h"
#include "qr1crxfilteroffsetmodel.h"
#include "define.h"

namespace Ui {
class QCalR1CRxFilterOffsetDlg;
}

class QCalR1CRxFilterOffsetDlg : public QCalR1CBaseDlg
{
    Q_OBJECT
    DECLARE_CAL_R1C_DLG

public:
    explicit QCalR1CRxFilterOffsetDlg(QWidget *parent = 0);
    ~QCalR1CRxFilterOffsetDlg();

private:
    Ui::QCalR1CRxFilterOffsetDlg *ui;
};

#endif // QCALR1CRXFILTEROFFSETDLG_H
