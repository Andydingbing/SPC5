#ifndef Q_TEST_DLG_H
#define Q_TEST_DLG_H

#include "q_cal_base_dlg.h"
#include "ui_q_test_freq_res_dlg.h"
#include "ui_q_test_rf_dlg.h"
#include "q_test_freq_res_thread.h"
#include "q_test_rf_thread.h"

DECL_TEST_R1C_DLG(FreqRes)
DECL_TEST_R1C_DLG(RF)

class QTestR1CTabWidget : public QTabWidget
{
public:
    explicit QTestR1CTabWidget(QWidget *parent = nullptr) :
        QTabWidget(parent)
    {
        dlgFreqRes = new QTestR1CFreqResDlg(this);
        dlgRF = new QTestR1CRFDlg(this);

        setTabPosition(South);
        addTab(dlgFreqRes,tr("Freq Res"));
        addTab(dlgRF,tr("RF"));
    }

public:
    QTestR1CFreqResDlg *dlgFreqRes;
    QTestR1CRFDlg *dlgRF;
};


#endif // Q_TEST_DLG_H
