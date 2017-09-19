#ifndef QCALR1CTXLOLEAKDLG_H
#define QCALR1CTXLOLEAKDLG_H

#include "qcalr1cbasedlg.h"
#include "qr1ctxloleakmodel.h"
#include "libSP1401.h"

namespace Ui {
class QCalR1CTxLOLeakDlg;
}

class QCalR1CTxLOLeakDlg : public QCalR1CBaseDlg
{
    Q_OBJECT
    DECLARE_CAL_R1C_DLG

public:
    CSP3501 *m_pSP3501;
    QR1CTxLOLeakModel *m_pModel;
public:
    explicit QCalR1CTxLOLeakDlg(QWidget *parent = 0);
    ~QCalR1CTxLOLeakDlg();

private slots:
    void on_m_PBStar_clicked();

    void on_m_PBStop_clicked();

    void on_m_PBPaus_clicked();

    void on_m_PBCont_clicked();

    void on_m_PBExport_clicked();

    void on_m_PBGetSet_clicked();

private:
    Ui::QCalR1CTxLOLeakDlg *ui;
};

#endif // QCALR1CTXLOLEAKDLG_H
