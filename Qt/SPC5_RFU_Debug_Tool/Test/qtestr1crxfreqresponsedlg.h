#ifndef QTESTR1CRXFREQRESPONSEDLG_H
#define QTESTR1CRXFREQRESPONSEDLG_H

#include "qtestbasedlg.h"
#include "qr1crxfreqresponsemodel.h"
#include "define.h"

namespace Ui {
class QTestR1CRxFreqResponseDlg;
}

class QTestR1CRxFreqResponseDlg : public QTestR1CBaseDlg
{
    Q_OBJECT
    DECLARE_TEST_R1C_DLG

public:
    explicit QTestR1CRxFreqResponseDlg(QWidget *parent = 0);
    ~QTestR1CRxFreqResponseDlg();
public slots:
     void updateCalUI(QList<QString> list);
private:
    Ui::QTestR1CRxFreqResponseDlg *ui;
};

#endif // QCALR1CTXFILTEROFFSETDLG_H


