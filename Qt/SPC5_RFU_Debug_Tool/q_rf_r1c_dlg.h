#ifndef Q_RF_R1C_DLG_H
#define Q_RF_R1C_DLG_H

#include <QWidget>
#include "q_rf_dlg.h"

namespace Ui {
class QRFR1CDlg;
}

class QRFR1CDlg : public QRFDlg
{
    Q_OBJECT

public:
    explicit QRFR1CDlg(QWidget *parent = 0);
    ~QRFR1CDlg();

private slots:
    void on_lineEditTXFreq_returnPressed();

    void on_comboBoxTXAtt0_currentIndexChanged(int index);

    void on_comboBoxTXAtt1_currentIndexChanged(int index);

    void on_comboBoxTXAtt2_currentIndexChanged(int index);

    void on_comboBoxTXAtt3_currentIndexChanged(int index);

    void on_comboBoxIOMode_currentIndexChanged(int index);

    void on_lineEditDGain_textChanged(const QString &arg1);

    void on_pushButtonSetTXAll_clicked();

    void on_lineEditRXFreq_returnPressed();

    void on_comboBoxRXLNAAttSw_currentIndexChanged(int index);

    void on_comboBoxRXAtt019Sw_currentIndexChanged(int index);

    void on_comboBoxRXAtt1_currentIndexChanged(int index);

    void on_comboBoxRXAtt2_currentIndexChanged(int index);

    void on_comboBoxIFFilterSw_currentIndexChanged(int index);

    void on_comboBoxRXAtt3_currentIndexChanged(int index);

    void on_pushButtonSetRXAll_clicked();

    void on_pushButtonGetTempAll_clicked();

    void on_pushButtonGetDetAll_clicked();

    void on_pushButtonSetSN_clicked();

    void on_pushButtonGetSN_clicked();

private:
    Ui::QRFR1CDlg *ui;
};

#endif // Q_RF_R1C_DLG_H
