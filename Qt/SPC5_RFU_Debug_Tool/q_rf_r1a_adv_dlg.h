#ifndef QRFR1AADVDLG_H
#define QRFR1AADVDLG_H

#include <QWidget>
#include "q_rf_dlg.h"
#include "q_adf5355_dlg.h"

namespace Ui {
class QRfR1AAdvDlg;
}

class QRfR1AAdvDlg : public QRFDlg
{
    Q_OBJECT

public:
    QAdf5355Dlg *dlgAdf5355[4];
public:
    explicit QRfR1AAdvDlg(QWidget *parent = nullptr);
    ~QRfR1AAdvDlg();

private slots:
    void on_comboBoxTxIOSw_currentIndexChanged(int index);

    void on_comboBoxTxAtt3_currentIndexChanged(int index);

    void on_comboBoxTxPAAttSw_currentIndexChanged(int index);

    void on_comboBoxTxAtt2_currentIndexChanged(int index);

    void on_comboBoxTxAtt1_currentIndexChanged(int index);

    void on_comboBoxTxBandSw_currentIndexChanged(int index);

    void on_pushButtonModEn_clicked();

    void on_pushButtonPowerOn_clicked();

    void on_comboBoxIOMode_currentIndexChanged(int index);

    void on_checkBoxTxLED_clicked();

    void on_checkBoxRxLED_clicked();

    void on_pushButtonSetTxFreq_clicked();

    void on_pushButtonSetRxFreq_clicked();

    void on_pushButtonGetTxTemp_clicked();

    void on_pushButtonGetRxTemp_clicked();

    void on_pushButtonGetTxDet_clicked();

    void on_pushButtonGetRxDet_clicked();

    void on_comboBoxRxIOSw1_currentIndexChanged(int index);

    void on_comboBoxRxIOSw2_currentIndexChanged(int index);

    void on_comboBoxRxIOSw_currentIndexChanged(int index);

    void on_comboBoxRxLNAAttSw_currentIndexChanged(int index);

    void on_comboBoxRxAtt1_currentIndexChanged(int index);

    void on_comboBoxRxAtt2_currentIndexChanged(int index);

    void on_comboBoxIFFilterSw_currentIndexChanged(int index);

    void on_pushButtonAdvTxLO1_clicked();

    void on_pushButtonAdvTxLO2_clicked();

    void on_pushButtonAdvRxLO1_clicked();

    void on_pushButtonAdvRxLO2_clicked();

    void on_pushButtonSetTxLO1_clicked();

    void on_pushButtonSetTxLO2_clicked();

    void on_pushButtonSetRxLO1_clicked();

    void on_pushButtonSetRxLO2_clicked();

private:
    Ui::QRfR1AAdvDlg *ui;
};

#endif // QRFR1AADVDLG_H
