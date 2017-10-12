#ifndef QRFR1CDLG_H
#define QRFR1CDLG_H

#include <QDialog>
#include "define.h"
#include "libSP1401.h"

namespace Ui {
class QRfR1CDlg;
}

class QRfR1CDlg : public QDialog
{
    Q_OBJECT

public:
    CSP1401R1C *m_pSP1401;
public:
    explicit QRfR1CDlg(QWidget *parent = 0);
    ~QRfR1CDlg();

private slots:
    void on_m_PBSetTxFreq_clicked();
    void on_m_PBSetTxAtt0_clicked();
    void on_m_PBSetTxAtt1_clicked();
    void on_m_PBSetTxAtt2_clicked();
    void on_m_PBSetTxAtt3_clicked();
    void on_m_PBSetIOMode_clicked();
    void on_m_PBSyncSetTxGain_clicked();
    void on_m_PBSetTxAll_clicked();
    void on_m_PBSetRxFreq_clicked();
    void on_m_PBSetRxLNAAttSw_clicked();
    void on_m_PBSetRxAtt019Sw_clicked();
    void on_m_PBSetRxAtt1_clicked();
    void on_m_PBSetRxAtt2_clicked();
    void on_m_PBSetRxIFFilterSw_clicked();
    void on_m_PBSetRxAtt3_clicked();
    void on_m_PBSetRxAll_clicked();
    void on_m_PBGetTemp0_clicked();
    void on_m_PBGetTemp1_clicked();
    void on_m_PBGetTemp2_clicked();
    void on_m_PBGetTemp3_clicked();
    void on_m_PBGetTemp4_clicked();
    void on_m_PBGetTemp5_clicked();
    void on_m_PBGetTemp6_clicked();
    void on_m_PBGetTemp7_clicked();
    void on_m_PBGetTempAll_clicked();
    void on_m_PBGetDetTxIF2_clicked();
    void on_m_PBGetDetRxComb_clicked();
    void on_m_PBGetDetRxIF1_clicked();
    void on_m_PBGetDetRxIF2_clicked();
    void on_m_PBGetDetAll_clicked();
    void on_m_PBSetSN_clicked();
    void on_m_PBGetSN_clicked();

private:
    Ui::QRfR1CDlg *ui;
};

#endif // QRFR1CDLG_H
