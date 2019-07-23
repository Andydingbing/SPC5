#ifndef QRFR1FDLG_H
#define QRFR1FDLG_H

#include <QWidget>
#include "q_rf_dlg.h"

namespace Ui {
class QRFR1FDlg;
}

class QRFR1FDlg : public QRFDlg
{
    Q_OBJECT

public:
    explicit QRFR1FDlg(QWidget *parent = 0);
    ~QRFR1FDlg();

private slots:
    void on_lineEditTXFreq_returnPressed();

    void on_comboBoxTXAtt0_currentIndexChanged(int index);

    void on_comboBoxTXAtt1_currentIndexChanged(int index);

    void on_comboBoxTXAtt2_currentIndexChanged(int index);

    void on_comboBoxIOMode_currentIndexChanged(int index);

    void on_lineEditDGain_textChanged(const QString &arg1);

    void on_pushButtonSetTXAll_clicked();

    void on_lineEditRXFreq_returnPressed();

    void on_comboBoxRXLNAAttSw_currentIndexChanged(int index);

    void on_comboBoxRXAtt019Sw_currentIndexChanged(int index);

    void on_comboBoxRXAtt1_currentIndexChanged(int index);

    void on_comboBoxIFFilterSw_currentIndexChanged(int index);

    void on_comboBoxRXAtt2_currentIndexChanged(int index);

    void on_pushButtonSetRXAll_clicked();

    void on_pushButtonGetTempDetAll_clicked();

    void on_pushButtonSetSN_clicked();

    void on_pushButtonGetSN_clicked();

private:
    Ui::QRFR1FDlg *ui;
};

#endif // QRFR1FDLG_H
