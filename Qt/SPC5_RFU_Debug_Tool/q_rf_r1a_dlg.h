#ifndef Q_RF_R1A_DLG_H
#define Q_RF_R1A_DLG_H

#include <QWidget>
#include "q_rf_dlg.h"

namespace Ui {
class QRFR1ADlg;
}

class QRFR1ADlg : public QRFDlg
{
    Q_OBJECT

public:
    explicit QRFR1ADlg(QWidget *parent = 0);
    ~QRFR1ADlg();

private slots:
    void on_pushButtonSetTxFreq_clicked();

    void on_pushButtonSetTxAtt1_clicked();

    void on_pushButtonSetTxAtt2_clicked();

    void on_pushButtonSetTxPAAttSw_clicked();

    void on_pushButtonSetTxAtt3_clicked();

    void on_pushButtonGetTxDet_clicked();

    void on_pushButtonGetTxTemp_clicked();

    void on_pushButtonSetIOMode_clicked();

    void on_pushButtonSyncSetTxGain_clicked();

    void on_pushButtonSetRxFreq_clicked();

    void on_pushButtonSetRxLNAAttSw_clicked();

    void on_pushButtonSetRxAtt1_clicked();

    void on_pushButtonSetRxAtt2_clicked();

    void on_pushButtonSetIFFilterSw_clicked();

    void on_pushButtonGetRxDet_clicked();

    void on_pushButtonGetRxTemp_clicked();

    void on_pushButtonSetSN_clicked();

    void on_pushButtonGetSN_clicked();

private:
    Ui::QRFR1ADlg *ui;
};

#endif // Q_RF_R1A_DLG_H
