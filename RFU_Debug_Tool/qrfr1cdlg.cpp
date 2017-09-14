#include "qrfr1cdlg.h"
#include "ui_qrfr1cdlg.h"

QRfR1CDlg::QRfR1CDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QRfR1CDlg)
{
    ui->setupUi(this);

    double dAtt[SERIE_SIZE(0.0,31.5,0.5)] = {0.0};
    for (uint16_t i = 0;i < ARRAY_SIZE(dAtt);i ++) {
        dAtt[i] = i * 0.5;
        QString strAtt = QString("%1").arg(dAtt[i],2,'f',1);
        ui->m_CBTxAtt0->addItem(strAtt);
        ui->m_CBTxAtt1->addItem(strAtt);
        ui->m_CBTxAtt2->addItem(strAtt);
        ui->m_CBTxAtt3->addItem(strAtt);
        ui->m_CBRxAtt1->addItem(strAtt);
        ui->m_CBRxAtt2->addItem(strAtt);
        ui->m_CBRxAtt3->addItem(strAtt);
    }
    ui->m_CBTxAtt0->setMaxVisibleItems(20);
    ui->m_CBTxAtt1->setMaxVisibleItems(20);
    ui->m_CBTxAtt2->setMaxVisibleItems(20);
    ui->m_CBTxAtt3->setMaxVisibleItems(20);
    ui->m_CBRxAtt1->setMaxVisibleItems(20);
    ui->m_CBRxAtt2->setMaxVisibleItems(20);
    ui->m_CBRxAtt3->setMaxVisibleItems(20);
    ui->m_CBIOMode->addItem(QString("IO"));
    ui->m_CBIOMode->addItem(QString("Output"));
    ui->m_CBIOMode->addItem(QString("Loopback"));
    ui->m_CBRxLNAAttSw->addItem(QString("Attenuation"));
    ui->m_CBRxLNAAttSw->addItem(QString("LNA"));
    ui->m_CBRxAtt019Sw->addItem(QString("Att 19"));
    ui->m_CBRxAtt019Sw->addItem(QString("Att 0"));
    ui->m_CBRxIFFilterSw->addItem(QString("100M"));
    ui->m_CBRxIFFilterSw->addItem(QString("160M"));

    ui->m_LETxFreq->setText("2G");
    ui->m_LETxFreq->setToolTip(STRING_FREQ_TIPS);
    ui->m_CBTxAtt0->setCurrentIndex(int(5.0 / 0.5));
    ui->m_CBTxAtt1->setCurrentIndex(int(20.0 / 0.5));
    ui->m_CBTxAtt2->setCurrentIndex(int(0.0 / 0.5));
    ui->m_CBTxAtt3->setCurrentIndex(int(1.0 / 0.5));
    ui->m_CBIOMode->setCurrentIndex(int(SP1401::IOMode::OutPut));
    ui->m_LEDGain->setText("0");
    ui->m_LERxFreq->setText("2G");
    ui->m_LERxFreq->setToolTip(STRING_FREQ_TIPS);
    ui->m_CBRxLNAAttSw->setCurrentIndex(int(SP1401::RxLNAAtt::Rx_Att));
    ui->m_CBRxAtt019Sw->setCurrentIndex(int(SP1401::RxAtt019::Rx_Att_19));
    ui->m_CBRxAtt1->setCurrentIndex(int(5.0 / 0.5));
    ui->m_CBRxAtt2->setCurrentIndex(int(15.0 / 0.5));
    ui->m_CBRxAtt3->setCurrentIndex(int(10.0 / 0.5));
    ui->m_CBRxIFFilterSw->setCurrentIndex(int(ISP1401::RxIFFilter::_160MHz));

    ui->m_LETemp0->setText("0.0");
    ui->m_LETemp1->setText("0.0");
    ui->m_LETemp2->setText("0.0");
    ui->m_LETemp3->setText("0.0");
    ui->m_LETemp4->setText("0.0");
    ui->m_LETemp5->setText("0.0");
    ui->m_LETemp6->setText("0.0");
    ui->m_LETemp7->setText("0.0");

    ui->m_LEDetTxIF2->setText("0");
    ui->m_LEDetRxComb->setText("0");
    ui->m_LEDetRxIF1->setText("0");
    ui->m_LEDetRxIF2->setText("0");

    ui->m_LEHwVer->setText("SP1401R1C");
    ui->m_LEOrdinal->setText("00000000");
}

QRfR1CDlg::~QRfR1CDlg()
{
    delete ui;
}

void QRfR1CDlg::on_m_PBSetTxFreq_clicked()
{
    uint64_t uiFreq = 0;
    str2freq(ui->m_LETxFreq->text().toStdString().c_str(),uiFreq);
    INT_CHECKV(m_pSP1401->SetTxFreq(uiFreq));
}

void QRfR1CDlg::on_m_PBSetTxAtt0_clicked()
{
    double dAtt = ui->m_CBTxAtt0->currentIndex() * 0.5;
    INT_CHECKV(m_pSP1401->SetTxAtt0(dAtt));
}

void QRfR1CDlg::on_m_PBSetTxAtt1_clicked()
{
    double dAtt = ui->m_CBTxAtt1->currentIndex() * 0.5;
    INT_CHECKV(m_pSP1401->SetTxAtt1(dAtt));
}

void QRfR1CDlg::on_m_PBSetTxAtt2_clicked()
{
    double dAtt = ui->m_CBTxAtt2->currentIndex() * 0.5;
    INT_CHECKV(m_pSP1401->SetTxAtt2(dAtt));
}

void QRfR1CDlg::on_m_PBSetTxAtt3_clicked()
{
    double dAtt = ui->m_CBTxAtt3->currentIndex() * 0.5;
    INT_CHECKV(m_pSP1401->SetTxAtt3(dAtt));
}

void QRfR1CDlg::on_m_PBSetIOMode_clicked()
{
    SP1401::IOMode Mode = (SP1401::IOMode)(ui->m_CBIOMode->currentIndex());
    INT_CHECKV(m_pSP1401->SetIOMode(Mode));
}

void QRfR1CDlg::on_m_PBSyncSetTxGain_clicked()
{
    double dAtt0 = ui->m_CBTxAtt0->currentIndex() * 0.5;
    double dAtt1 = ui->m_CBTxAtt1->currentIndex() * 0.5;
    double dAtt2 = ui->m_CBTxAtt2->currentIndex() * 0.5;
    double dAtt3 = ui->m_CBTxAtt3->currentIndex() * 0.5;
    double dDGain = ui->m_LEDGain->text().toDouble();
    INT_CHECKV(m_pSP1401->SyncSetTxGain(dAtt0,dAtt1,dAtt2,dAtt3,dDGain));
}

void QRfR1CDlg::on_m_PBSetTxAll_clicked()
{
    on_m_PBSetTxFreq_clicked();
    on_m_PBSetTxAtt0_clicked();
    on_m_PBSetTxAtt1_clicked();
    on_m_PBSetTxAtt2_clicked();
    on_m_PBSetTxAtt3_clicked();
    on_m_PBSetIOMode_clicked();
}

void QRfR1CDlg::on_m_PBSetRxFreq_clicked()
{
    uint64_t uiFreq = 0;
    str2freq(ui->m_LERxFreq->text().toStdString().c_str(),uiFreq);
    INT_CHECKV(m_pSP1401->SetRxFreq(uiFreq));
}

void QRfR1CDlg::on_m_PBSetRxLNAAttSw_clicked()
{
    SP1401::RxLNAAtt LNAAtt = (SP1401::RxLNAAtt)(ui->m_CBRxLNAAttSw->currentIndex());
    INT_CHECKV(m_pSP1401->SetRxLNAAttSw(LNAAtt));
}

void QRfR1CDlg::on_m_PBSetRxAtt019Sw_clicked()
{
    SP1401::RxAtt019 Att = (SP1401::RxAtt019)(ui->m_CBRxAtt019Sw->currentIndex());
    INT_CHECKV(m_pSP1401->SetRxAtt019Sw(Att));
}

void QRfR1CDlg::on_m_PBSetRxAtt1_clicked()
{
    double dAtt = ui->m_CBRxAtt1->currentIndex() * 0.5;
    INT_CHECKV(m_pSP1401->SetRxAtt1(dAtt));
}

void QRfR1CDlg::on_m_PBSetRxAtt2_clicked()
{
    double dAtt = ui->m_CBRxAtt2->currentIndex() * 0.5;
    INT_CHECKV(m_pSP1401->SetRxAtt2(dAtt));
}

void QRfR1CDlg::on_m_PBSetRxIFFilterSw_clicked()
{
    ISP1401::RxIFFilter Filter = (ISP1401::RxIFFilter)(ui->m_CBRxIFFilterSw->currentIndex());
    INT_CHECKV(m_pSP1401->SetRxIFFilterSw(Filter));
}

void QRfR1CDlg::on_m_PBSetRxAtt3_clicked()
{
    double dAtt = ui->m_CBRxAtt3->currentIndex() * 0.5;
    INT_CHECKV(m_pSP1401->SetRxAtt3(dAtt));
}

void QRfR1CDlg::on_m_PBSetRxAll_clicked()
{
    on_m_PBSetRxFreq_clicked();
    on_m_PBSetRxLNAAttSw_clicked();
    on_m_PBSetRxAtt019Sw_clicked();
    on_m_PBSetRxAtt1_clicked();
    on_m_PBSetRxAtt2_clicked();
    on_m_PBSetRxIFFilterSw_clicked();
    on_m_PBSetRxAtt3_clicked();
}

void QRfR1CDlg::on_m_PBGetTemp0_clicked()
{
    double dTemp = 0.0;
    INT_CHECKV(m_pSP1401->GetTemp0RxLO2(&dTemp));
    ui->m_LETemp0->setText(QString("%1").arg(dTemp));
}

void QRfR1CDlg::on_m_PBGetTemp1_clicked()
{
    double dTemp = 0.0;
    INT_CHECKV(m_pSP1401->GetTemp1RxLNA(&dTemp));
    ui->m_LETemp1->setText(QString("%1").arg(dTemp));
}

void QRfR1CDlg::on_m_PBGetTemp2_clicked()
{
    double dTemp = 0.0;
    INT_CHECKV(m_pSP1401->GetTemp2RxLO1(&dTemp));
    ui->m_LETemp2->setText(QString("%1").arg(dTemp));
}

void QRfR1CDlg::on_m_PBGetTemp3_clicked()
{
    double dTemp = 0.0;
    INT_CHECKV(m_pSP1401->GetTemp3RxPA1(&dTemp));
    ui->m_LETemp3->setText(QString("%1").arg(dTemp));
}

void QRfR1CDlg::on_m_PBGetTemp4_clicked()
{
    double dTemp = 0.0;
    INT_CHECKV(m_pSP1401->GetTemp4TxLO2(&dTemp));
    ui->m_LETemp4->setText(QString("%1").arg(dTemp));
}

void QRfR1CDlg::on_m_PBGetTemp5_clicked()
{
    double dTemp = 0.0;
    INT_CHECKV(m_pSP1401->GetTemp5TxLO1(&dTemp));
    ui->m_LETemp5->setText(QString("%1").arg(dTemp));
}

void QRfR1CDlg::on_m_PBGetTemp6_clicked()
{
    double dTemp = 0.0;
    INT_CHECKV(m_pSP1401->GetTemp6TxPA4(&dTemp));
    ui->m_LETemp6->setText(QString("%1").arg(dTemp));
}

void QRfR1CDlg::on_m_PBGetTemp7_clicked()
{
    double dTemp = 0.0;
    INT_CHECKV(m_pSP1401->GetTemp7TxPA3(&dTemp));
    ui->m_LETemp7->setText(QString("%1").arg(dTemp));
}

void QRfR1CDlg::on_m_PBGetTempAll_clicked()
{
    on_m_PBGetTemp0_clicked();
    on_m_PBGetTemp1_clicked();
    on_m_PBGetTemp2_clicked();
    on_m_PBGetTemp3_clicked();
    on_m_PBGetTemp4_clicked();
    on_m_PBGetTemp5_clicked();
    on_m_PBGetTemp6_clicked();
    on_m_PBGetTemp7_clicked();
}

void QRfR1CDlg::on_m_PBGetDetTxIF2_clicked()
{
    uint32_t uiDet = 0;
    INT_CHECKV(m_pSP1401->GetAD7680TxIF2(uiDet));
    ui->m_LEDetTxIF2->setText(QString("%1").arg(uiDet));
}

void QRfR1CDlg::on_m_PBGetDetRxComb_clicked()
{
    uint32_t uiDet = 0;
    INT_CHECKV(m_pSP1401->GetAD7680RxComb(uiDet));
    ui->m_LEDetRxComb->setText(QString("%1").arg(uiDet));
}

void QRfR1CDlg::on_m_PBGetDetRxIF1_clicked()
{
    uint32_t uiDet = 0;
    INT_CHECKV(m_pSP1401->GetAD7680RxIF1(uiDet));
    ui->m_LEDetRxIF1->setText(QString("%1").arg(uiDet));
}

void QRfR1CDlg::on_m_PBGetDetRxIF2_clicked()
{
    uint32_t uiDet = 0;
    INT_CHECKV(m_pSP1401->GetAD7680RxIF2(uiDet));
    ui->m_LEDetRxIF2->setText(QString("%1").arg(uiDet));
}

void QRfR1CDlg::on_m_PBGetDetAll_clicked()
{
    on_m_PBGetDetTxIF2_clicked();
    on_m_PBGetDetRxComb_clicked();
    on_m_PBGetDetRxIF1_clicked();
    on_m_PBGetDetRxIF2_clicked();
}

void QRfR1CDlg::on_m_PBSetSN_clicked()
{
    const char *pVer = ui->m_LEHwVer->text().toStdString().c_str();
    int32_t iOrdinal = ui->m_LEOrdinal->text().toInt();
    INT_CHECKV(m_pSP1401->SetSN(pVer,iOrdinal));
}

void QRfR1CDlg::on_m_PBGetSN_clicked()
{
    char szSN[32] = {0};
    memset(szSN,0,sizeof(szSN));
    INT_CHECKV(m_pSP1401->GetSN(szSN));
    ui->m_LESN->setText(QString("%1").arg(szSN));
}
