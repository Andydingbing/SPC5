#ifndef QRFR1CADVDLG_H
#define QRFR1CADVDLG_H

#include <QWidget>
#include "qrfdlg.h"
#include "qadf5355dlg.h"

namespace Ui {
class QRfR1CAdvDlg;
}

class QRfR1CAdvDlg : public QRfDlg
{
    Q_OBJECT
public:
    QAdf5355Dlg *dlgAdf5355[3];
public:
    explicit QRfR1CAdvDlg(QWidget *parent = 0);
    ~QRfR1CAdvDlg();

private slots:
    void on_comboBoxTxIOSw2_currentIndexChanged(int index);

    void on_comboBoxTxIOSw1_currentIndexChanged(int index);

    void on_comboBoxTxAtt3_currentIndexChanged(int index);

    void on_comboBoxTxAtt2_currentIndexChanged(int index);

    void on_comboBoxTxAtt1_currentIndexChanged(int index);

    void on_comboBoxTxFilterSw1_currentIndexChanged(int index);

    void on_comboBoxTxFilterSw2_currentIndexChanged(int index);

    void on_comboBoxTxAtt0_currentIndexChanged(int index);

    void on_pushButtonModEn_clicked();

    void on_pushButtonPowerOn_clicked();

    void on_comboBoxLoopSw_currentIndexChanged(int index);

    void on_pushButtonSetTxLO3_clicked();

    void on_pushButtonSetTxLO2_clicked();

    void on_pushButtonSetTxLO1_clicked();

    void on_pushButtonSetRxLO1_clicked();

    void on_pushButtonAdvTxLO3_clicked();

    void on_pushButtonAdvTxLO2_clicked();

    void on_pushButtonAdvTxLO1_clicked();

    void on_pushButtonAdvRxLO1_clicked();

    void on_comboBoxIOMode_currentIndexChanged(int index);

    void on_checkBoxTxLED_clicked(bool checked);

    void on_checkBoxRxLED_clicked(bool checked);

    void on_pushButtonSetTxFreq_clicked();

    void on_pushButtonSetRxFreq_clicked();

    void on_pushButtonEEPROM_clicked();

    void on_pushButtonMCP23S17_clicked();

    void on_pushButtonGetAllTemp_clicked();

    void on_pushButtonGetAllDet_clicked();

    void on_comboBoxRxIOSw1_currentIndexChanged(int index);

    void on_comboBoxRxIOSw2_currentIndexChanged(int index);

    void on_comboBoxRxIOSw3_currentIndexChanged(int index);

    void on_comboBoxLNAAttSw_currentIndexChanged(int index);

    void on_comboBoxAtt019Sw_currentIndexChanged(int index);

    void on_comboBoxRxAtt1_currentIndexChanged(int index);

    void on_comboBoxRxAtt2_currentIndexChanged(int index);

    void on_comboBoxIFFilterSw_currentIndexChanged(int index);

    void on_comboBoxRxAtt3_currentIndexChanged(int index);

private:
    Ui::QRfR1CAdvDlg *ui;
};

#endif // QRFR1CADVDLG_H
