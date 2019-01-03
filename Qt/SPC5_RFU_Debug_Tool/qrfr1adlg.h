#ifndef QRFR1ADLG_H
#define QRFR1ADLG_H

#include <QWidget>
#include "qrfdlg.h"

namespace Ui {
class QRfR1ADlg;
}

class QRfR1ADlg : public QRfDlg
{
    Q_OBJECT

public:
    explicit QRfR1ADlg(QWidget *parent = 0);
    ~QRfR1ADlg();

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
    Ui::QRfR1ADlg *ui;
};

#endif // QRFR1ADLG_H
