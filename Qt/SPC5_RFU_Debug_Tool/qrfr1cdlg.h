#ifndef QRFR1CDLG_H
#define QRFR1CDLG_H

#include <QWidget>
#include "qrfdlg.h"

namespace Ui {
class QRfR1CDlg;
}

class QRfR1CDlg : public QRfDlg
{
    Q_OBJECT

public:
    explicit QRfR1CDlg(QWidget *parent = 0);
    ~QRfR1CDlg();

private slots:
    void on_pushButtonSetTxFreq_clicked();

    void on_pushButtonSetTxAtt0_clicked();

    void on_pushButtonSetTxAtt1_clicked();

    void on_pushButtonSetTxAtt2_clicked();

    void on_pushButtonSetTxAtt3_clicked();

    void on_pushButtonSetIOMode_clicked();

    void on_pushButtonSyncSetTxGain_clicked();

    void on_pushButtonSetTxAll_clicked();

    void on_pushButtonSetRxFreq_clicked();

    void on_pushButtonSetRxLNAAttSw_clicked();

    void on_pushButtonSetRxAtt019Sw_clicked();

    void on_pushButtonSetRxAtt1_clicked();

    void on_pushButtonSetRxAtt2_clicked();

    void on_pushButtonSetIFFilterSw_clicked();

    void on_pushButtonSetRxAtt3_clicked();

    void on_pushButtonSetRxAll_clicked();

    void on_pushButtonGetTemp0_clicked();

    void on_pushButtonGetTemp1_clicked();

    void on_pushButtonGetTemp2_clicked();

    void on_pushButtonGetTemp3_clicked();

    void on_pushButtonGetTemp4_clicked();

    void on_pushButtonGetTemp5_clicked();

    void on_pushButtonGetTemp6_clicked();

    void on_pushButtonGetTemp7_clicked();

    void on_pushButtonGetTempAll_clicked();

    void on_pushButtonGetDetTxIF2_clicked();

    void on_pushButtonGetDetRxComb_clicked();

    void on_pushButtonGetDetRxIF1_clicked();

    void on_pushButtonGetDetRxIF2_clicked();

    void on_pushButtonGetDetAll_clicked();

    void on_pushButtonSetSN_clicked();

    void on_pushButtonGetSN_clicked();

private:
    Ui::QRfR1CDlg *ui;
};

#endif // QRFR1CDLG_H
