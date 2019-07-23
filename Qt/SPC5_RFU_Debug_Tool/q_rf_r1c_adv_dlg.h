#ifndef Q_RF_R1C_ADV_DLD_H
#define Q_RF_R1C_ADV_DLD_H

#include <QWidget>
#include "q_rf_dlg.h"

namespace Ui {
class QRFR1CAdvDlg;
}

class QAdf5355Dlg;

class QRFR1CAdvDlg : public QRFDlg
{
    Q_OBJECT
public:
    QAdf5355Dlg *dlgADF5355[3];
public:
    explicit QRFR1CAdvDlg(QWidget *parent = 0);
    ~QRFR1CAdvDlg();

private slots:
    void on_comboBoxTXIOSw2_currentIndexChanged(int index);

    void on_comboBoxTXIOSw1_currentIndexChanged(int index);

    void on_comboBoxTXAtt3_currentIndexChanged(int index);

    void on_comboBoxTXAtt2_currentIndexChanged(int index);

    void on_comboBoxTXAtt1_currentIndexChanged(int index);

    void on_comboBoxTXFilterSw1_currentIndexChanged(int index);

    void on_comboBoxTXFilterSw2_currentIndexChanged(int index);

    void on_comboBoxTXAtt0_currentIndexChanged(int index);

    void on_pushButtonModEn_clicked();

    void on_pushButtonPowerOn_clicked();

    void on_comboBoxLoopSw_currentIndexChanged(int index);

    void on_pushButtonSetTXLO3_clicked();

    void on_pushButtonSetTXLO2_clicked();

    void on_pushButtonSetTXLO1_clicked();

    void on_pushButtonSetRXLO1_clicked();

    void on_pushButtonAdvTXLO3_clicked();

    void on_pushButtonAdvTXLO2_clicked();

    void on_pushButtonAdvTXLO1_clicked();

    void on_pushButtonAdvRXLO1_clicked();

    void on_comboBoxIOMode_currentIndexChanged(int index);

    void on_checkBoxTXLED_clicked(bool checked);

    void on_checkBoxRXLED_clicked(bool checked);

    void on_pushButtonSetTXFreq_clicked();

    void on_pushButtonSetRXFreq_clicked();

    void on_pushButtonEEPROM_clicked();

    void on_pushButtonMCP23S17_clicked();

    void on_pushButtonGetAllTemp_clicked();

    void on_pushButtonGetAllDet_clicked();

    void on_comboBoxRXIOSw1_currentIndexChanged(int index);

    void on_comboBoxRXIOSw2_currentIndexChanged(int index);

    void on_comboBoxRXIOSw3_currentIndexChanged(int index);

    void on_comboBoxLNAAttSw_currentIndexChanged(int index);

    void on_comboBoxAtt019Sw_currentIndexChanged(int index);

    void on_comboBoxRXAtt1_currentIndexChanged(int index);

    void on_comboBoxRXAtt2_currentIndexChanged(int index);

    void on_comboBoxIFFilterSw_currentIndexChanged(int index);

    void on_comboBoxRXAtt3_currentIndexChanged(int index);

private:
    Ui::QRFR1CAdvDlg *ui;
};

#endif // Q_RF_R1C_ADV_DLD_H
