#ifndef QCALR1CTXFILTEROFFSETDLG_H
#define QCALR1CTXFILTEROFFSETDLG_H

#include "qcalbasedlg.h"
#include "qr1ctxfilteroffsetmodel.h"
#include "define.h"

namespace Ui {
class QCalR1CTxFilterOffsetDlg;
}

class QCalR1CTxFilterOffsetDlg : public QCalR1CBaseDlg
{
    Q_OBJECT
    DECLARE_CAL_R1C_DLG

public:
    explicit QCalR1CTxFilterOffsetDlg(QWidget *parent = 0);
    ~QCalR1CTxFilterOffsetDlg();

private:
    Ui::QCalR1CTxFilterOffsetDlg *ui;
};

#endif // QCALR1CTXFILTEROFFSETDLG_H
