#ifndef QBBDLG_H
#define QBBDLG_H

#include <QWidget>
#include "q_rf_dlg.h"

namespace Ui {
class QBbDlg;
}

class QBbDlg : public QRFDlg
{
    Q_OBJECT

public:
    explicit QBbDlg(QWidget *parent = 0);
    ~QBbDlg();

private slots:
    void on_pushButtonSetDAClock_clicked();

    void on_pushButtonSetDASw_clicked();

    void on_pushButtonResetDA_clicked();

    void on_pushButtonSetDDSSrc_clicked();

    void on_pushButtonSetDDS1_clicked();

    void on_pushButtonSetDDS2_clicked();

    void on_pushButtonSetTxPowerComp_clicked();

    void on_pushButtonSetDUC_clicked();

    void on_checkBoxByPassTxFilter_clicked(bool checked);

    void on_pushButtonSetTxFilter_clicked();

    void on_pushButtonSetADClock_clicked();

    void on_pushButtonSetADSw_clicked();

    void on_pushButtonSetDDC_clicked();

    void on_pushButtonGetManualPower_clicked();

    void on_pushButtonGetADManual_clicked();

    void on_pushButtonGetADFPGA_clicked();

    void on_pushButtonSetRxPowerComp_clicked();

    void on_pushButtonSetRxFilter_clicked();

    void on_lineEditIQPhaseI_textChanged(const QString &arg1);

    void on_lineEditIQAmpI_textChanged(const QString &arg1);

    void on_lineEditIQAmpQ_textChanged(const QString &arg1);

    void on_lineEditDCOffsetI_textChanged(const QString &arg1);

    void on_lineEditDCOffsetQ_textChanged(const QString &arg1);

    void on_lineEditDUC_textChanged(const QString &arg1);

    void on_lineEditDDS1_textChanged(const QString &arg1);

private:
    Ui::QBbDlg *ui;
};

#endif // QBBDLG_H
