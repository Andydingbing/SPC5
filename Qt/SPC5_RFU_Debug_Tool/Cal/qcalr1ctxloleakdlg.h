#ifndef QCALR1CTXLOLEAKDLG_H
#define QCALR1CTXLOLEAKDLG_H

#include "qcalbasedlg.h"
#include "qr1ctxloleakmodel.h"
#include "define.h"

namespace Ui {
class QCalR1CTxLOLeakDlg;
}

class QCalR1CTxLOLeakDlg : public QCalR1CBaseDlg
{
    Q_OBJECT

public:
    sp3501 *_sp3501;
public:
    explicit QCalR1CTxLOLeakDlg(QWidget *parent = 0);
    ~QCalR1CTxLOLeakDlg();

    DECLARE_CAL_R1C_DLG

private:
    Ui::QCalR1CTxLOLeakDlg *ui;
};

#endif // QCALR1CTXLOLEAKDLG_H
