#ifndef QARBDLG_H
#define QARBDLG_H

#include <QWidget>
#include "q_rf_dlg.h"

namespace Ui {
class QArbDlg;
}

class QArbDlg : public QRFDlg
{
    Q_OBJECT

public:

public:
    explicit QArbDlg(QWidget *parent = 0);
    ~QArbDlg();

private slots:
    void on_pushButtonSelArbFile_clicked();

    void on_pushButtonSetAll_clicked();

    void on_pushButtonArbStart_clicked();

    void on_pushButtonArbStop_clicked();

    void on_pushButtonManualTrig_clicked();

    void on_pushButtonLoad_clicked();

    void on_pushButtonQueryState_clicked();

    void on_pushButtonQueryFrameSlot_clicked();

    void on_pushButtonFrameSlotTrigStart_clicked();

private:
    Ui::QArbDlg *ui;
};

#endif // QARBDLG_H
