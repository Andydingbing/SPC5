#ifndef Q_TEST_DLG_H
#define Q_TEST_DLG_H

#include "q_cal_base_dlg.h"
#include "ui_q_test_freq_res_dlg.h"
#include "ui_q_test_rf_dlg.h"
#include "ui_q_test_temp_pwr_dlg.h"
#include "q_test_freq_res_thread.h"
#include "q_test_rf_thread.h"
#include "q_test_temp_pwr_thread.h"

DECL_TEST_R1C_DLG(FreqRes)
DECL_TEST_R1C_DLG(RF)
DECL_TEST_R1C_DLG(TempPwr)

class QTestR1CTxTempPwrDlg : public QTestR1CTempPwrDlg
{
public:
    explicit QTestR1CTxTempPwrDlg(QWidget *parent = nullptr) :
        QTestR1CTempPwrDlg(parent)
    {
        ui->lineEditFreqStar->setText(QString("%1M").arg(RF_TX_FREQ_STAR / 1e6));
        ui->lineEditFreqStop->setText(QString("%1M").arg(RF_TX_FREQ_STOP / 1e6));
        ui->lineEditFreqStep->setText(QString("200M"));
        QTestR1CTempPwrDlg::init();
    }
    void uiToTestParam(TestBaseParam *param)
    {
        TestTempPwrParam *p = static_cast<TestTempPwrParam *>(param);
        p->ch = basic_sp1401::CH_TX;
        QTestR1CTempPwrDlg::uiToTestParam(param);
    }
};


class QTestR1CRxTempPwrDlg : public QTestR1CTempPwrDlg
{
public:
    explicit QTestR1CRxTempPwrDlg(QWidget *parent = nullptr) :
        QTestR1CTempPwrDlg(parent)
    {
        ui->lineEditFreqStar->setText(QString("%1M").arg(RF_RX_FREQ_STAR / 1e6));
        ui->lineEditFreqStop->setText(QString("%1M").arg(RF_RX_FREQ_STOP / 1e6));
        ui->lineEditFreqStep->setText(QString("200M"));
        QTestR1CTempPwrDlg::init();
    }
    void uiToTestParam(TestBaseParam *param)
    {
        TestTempPwrParam *p = static_cast<TestTempPwrParam *>(param);
        p->ch = basic_sp1401::CH_RX;
        QTestR1CTempPwrDlg::uiToTestParam(param);
    }
};


class QTestR1CTabWidget : public QTabWidget
{
public:
    explicit QTestR1CTabWidget(QWidget *parent = nullptr) :
        QTabWidget(parent)
    {
        dlgFreqRes = new QTestR1CFreqResDlg(this);
        dlgRF = new QTestR1CRFDlg(this);
        dlgTxTempPwr = new QTestR1CTxTempPwrDlg(this);
        dlgRxTempPwr = new QTestR1CRxTempPwrDlg(this);

        setTabPosition(South);
        addTab(dlgFreqRes,tr("Freq Res"));
        addTab(dlgRF,tr("RF"));
        addTab(dlgTxTempPwr,tr("Tx Temp Pwr"));
        addTab(dlgRxTempPwr,tr("Rx Temp Pwr"));
    }

public:
    QTestR1CFreqResDlg *dlgFreqRes;
    QTestR1CRFDlg *dlgRF;
    QTestR1CTxTempPwrDlg *dlgTxTempPwr;
    QTestR1CRxTempPwrDlg *dlgRxTempPwr;
};


#endif // Q_TEST_DLG_H
