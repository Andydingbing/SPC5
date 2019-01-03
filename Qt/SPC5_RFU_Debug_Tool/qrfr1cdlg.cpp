#include "qrfr1cdlg.h"
#include "ui_qrfr1cdlg.h"

QRfR1CDlg::QRfR1CDlg(QWidget *parent) :
    QRfDlg(parent),
    ui(new Ui::QRfR1CDlg)
{
    ui->setupUi(this);

    double att[SERIE_SIZE(0.0,31.5,0.5)] = {0.0};
    for (qint8 i = 0;i < ARRAY_SIZE(att);i ++) {
        att[i] = i * 0.5;
        QString strAtt = QString("%1").arg(att[i],2,'f',1);
        ui->comboBoxTxAtt0->addItem(strAtt);
        ui->comboBoxTxAtt1->addItem(strAtt);
        ui->comboBoxTxAtt2->addItem(strAtt);
        ui->comboBoxTxAtt3->addItem(strAtt);
        ui->comboBoxRxAtt1->addItem(strAtt);
        ui->comboBoxRxAtt2->addItem(strAtt);
        ui->comboBoxRxAtt3->addItem(strAtt);
    }
    ui->comboBoxTxAtt0->setMaxVisibleItems(20);
    ui->comboBoxTxAtt1->setMaxVisibleItems(20);
    ui->comboBoxTxAtt2->setMaxVisibleItems(20);
    ui->comboBoxTxAtt3->setMaxVisibleItems(20);
    ui->comboBoxTxAtt0->setCurrentIndex(int(5.0 / 0.5));
    ui->comboBoxTxAtt1->setCurrentIndex(int(20.0 / 0.5));
    ui->comboBoxTxAtt2->setCurrentIndex(int(0.0 / 0.5));
    ui->comboBoxTxAtt3->setCurrentIndex(int(1.0 / 0.5));

    ui->comboBoxRxAtt1->setMaxVisibleItems(20);
    ui->comboBoxRxAtt2->setMaxVisibleItems(20);
    ui->comboBoxRxAtt3->setMaxVisibleItems(20);
    ui->comboBoxRxAtt1->setCurrentIndex(int(5.0 / 0.5));
    ui->comboBoxRxAtt2->setCurrentIndex(int(15.0 / 0.5));
    ui->comboBoxRxAtt3->setCurrentIndex(int(10.0 / 0.5));

    ui->comboBoxIOMode->addItem(QString("IO"));
    ui->comboBoxIOMode->addItem(QString("Output"));
    ui->comboBoxIOMode->addItem(QString("Loopback"));
    ui->comboBoxIOMode->setCurrentIndex(int(sp1401::OUTPUT));

    ui->comboBoxRxLNAAttSw->addItem(QString("Attenuation"));
    ui->comboBoxRxLNAAttSw->addItem(QString("LNA"));

    ui->comboBoxRxAtt019Sw->addItem(QString("Att 19"));
    ui->comboBoxRxAtt019Sw->addItem(QString("Att 0"));

    ui->comboBoxIFFilterSw->addItem(QString("100M"));
    ui->comboBoxIFFilterSw->addItem(QString("160M"));

    ui->lineEditTxFreq->setText("2G");
    ui->lineEditTxFreq->setToolTip(STRING_FREQ_TIPS);
    ui->lineEditDGain->setText("0");

    ui->lineEditRxFreq->setText("2G");
    ui->lineEditRxFreq->setToolTip(STRING_FREQ_TIPS);
    ui->comboBoxRxLNAAttSw->setCurrentIndex(int(sp1401::RX_ATT));
    ui->comboBoxRxAtt019Sw->setCurrentIndex(int(sp1401::RX_ATT_19));
    ui->comboBoxIFFilterSw->setCurrentIndex(int(basic_sp1401::_160MHz));

    ui->lineEditTemp0->setText("0.0");
    ui->lineEditTemp1->setText("0.0");
    ui->lineEditTemp2->setText("0.0");
    ui->lineEditTemp3->setText("0.0");
    ui->lineEditTemp4->setText("0.0");
    ui->lineEditTemp5->setText("0.0");
    ui->lineEditTemp6->setText("0.0");
    ui->lineEditTemp7->setText("0.0");

    ui->lineEditDetTxIF2->setText("0");
    ui->lineEditDetRxComb->setText("0");
    ui->lineEditDetRxIF1->setText("0");
    ui->lineEditDetRxIF2->setText("0");

    ui->lineEditHwVer->setText("SP1401R1C");
    ui->lineEditOrdinal->setText("00000000");
}

QRfR1CDlg::~QRfR1CDlg()
{
    delete ui;
}

void QRfR1CDlg::on_pushButtonSetTxFreq_clicked()
{
    uint64_t freq = 0;
    str2freq(ui->lineEditTxFreq->text().toStdString().c_str(),freq);
    INT_CHECKV(getSP1401R1C()->set_tx_freq(freq));
}

void QRfR1CDlg::on_pushButtonSetTxAtt0_clicked()
{
    double att = ui->comboBoxTxAtt0->currentIndex() * 0.5;
    INT_CHECKV(getSP1401R1C()->set_tx_att0(att));
}

void QRfR1CDlg::on_pushButtonSetTxAtt1_clicked()
{
    double att = ui->comboBoxTxAtt1->currentIndex() * 0.5;
    INT_CHECKV(getSP1401R1C()->set_tx_att1(att));
}

void QRfR1CDlg::on_pushButtonSetTxAtt2_clicked()
{
    double att = ui->comboBoxTxAtt2->currentIndex() * 0.5;
    INT_CHECKV(getSP1401R1C()->set_tx_att2(att));
}

void QRfR1CDlg::on_pushButtonSetTxAtt3_clicked()
{
    double att = ui->comboBoxTxAtt3->currentIndex() * 0.5;
    INT_CHECKV(getSP1401R1C()->set_tx_att3(att));
}

void QRfR1CDlg::on_pushButtonSetIOMode_clicked()
{
    sp1401::io_mode_t mode = (sp1401::io_mode_t)(ui->comboBoxIOMode->currentIndex());
    INT_CHECKV(getSP1401R1C()->set_io_mode(mode));
}

void QRfR1CDlg::on_pushButtonSyncSetTxGain_clicked()
{
    double att0 = ui->comboBoxTxAtt0->currentIndex() * 0.5;
    double att1 = ui->comboBoxTxAtt1->currentIndex() * 0.5;
    double att2 = ui->comboBoxTxAtt2->currentIndex() * 0.5;
    double att3 = ui->comboBoxTxAtt3->currentIndex() * 0.5;
    double d_gain = ui->lineEditDGain->text().toDouble();
    INT_CHECKV(getSP1401R1C()->sync_set_tx_gain(att0,att1,att2,att3,d_gain));
}

void QRfR1CDlg::on_pushButtonSetTxAll_clicked()
{
    on_pushButtonSetTxFreq_clicked();
    on_pushButtonSetTxAtt0_clicked();
    on_pushButtonSetTxAtt1_clicked();
    on_pushButtonSetTxAtt2_clicked();
    on_pushButtonSetTxAtt3_clicked();
    on_pushButtonSetIOMode_clicked();
}

void QRfR1CDlg::on_pushButtonSetRxFreq_clicked()
{
    uint64_t freq = 0;
    str2freq(ui->lineEditRxFreq->text().toStdString().c_str(),freq);
    INT_CHECKV(getSP1401R1C()->set_rx_freq(freq));
}

void QRfR1CDlg::on_pushButtonSetRxLNAAttSw_clicked()
{
    sp1401::rx_lna_att_t lna_att = (sp1401::rx_lna_att_t)(ui->comboBoxRxLNAAttSw->currentIndex());
    INT_CHECKV(getSP1401R1C()->set_rx_lna_att_sw(lna_att));
}

void QRfR1CDlg::on_pushButtonSetRxAtt019Sw_clicked()
{
    sp1401::rx_att_019_t att = (sp1401::rx_att_019_t)(ui->comboBoxRxAtt019Sw->currentIndex());
    INT_CHECKV(getSP1401R1C()->set_rx_att_019_sw(att));
}

void QRfR1CDlg::on_pushButtonSetRxAtt1_clicked()
{
    double att = ui->comboBoxRxAtt1->currentIndex() * 0.5;
    INT_CHECKV(getSP1401R1C()->set_rx_att1(att));
}

void QRfR1CDlg::on_pushButtonSetRxAtt2_clicked()
{
    double att = ui->comboBoxRxAtt2->currentIndex() * 0.5;
    INT_CHECKV(getSP1401R1C()->set_rx_att2(att));
}

void QRfR1CDlg::on_pushButtonSetIFFilterSw_clicked()
{
    basic_sp1401::rx_if_filter_t filter = (basic_sp1401::rx_if_filter_t)(ui->comboBoxIFFilterSw->currentIndex());
    INT_CHECKV(getSP1401R1C()->set_rx_if_filter_sw(filter));
}

void QRfR1CDlg::on_pushButtonSetRxAtt3_clicked()
{
    double att = ui->comboBoxRxAtt3->currentIndex() * 0.5;
    INT_CHECKV(getSP1401R1C()->set_rx_att3(att));
}

void QRfR1CDlg::on_pushButtonSetRxAll_clicked()
{
    on_pushButtonSetRxFreq_clicked();
    on_pushButtonSetRxLNAAttSw_clicked();
    on_pushButtonSetRxAtt019Sw_clicked();
    on_pushButtonSetRxAtt1_clicked();
    on_pushButtonSetRxAtt2_clicked();
    on_pushButtonSetIFFilterSw_clicked();
    on_pushButtonSetRxAtt3_clicked();
}

void QRfR1CDlg::on_pushButtonGetTemp0_clicked()
{
    double temp = 0.0;
    INT_CHECKV(getSP1401R1C()->get_temp0_rx_lo2(&temp));
    ui->lineEditTemp0->setText(QString("%1").arg(temp));
}

void QRfR1CDlg::on_pushButtonGetTemp1_clicked()
{
    double temp = 0.0;
    INT_CHECKV(getSP1401R1C()->get_temp1_rx_lna(&temp));
    ui->lineEditTemp1->setText(QString("%1").arg(temp));
}

void QRfR1CDlg::on_pushButtonGetTemp2_clicked()
{
    double temp = 0.0;
    INT_CHECKV(getSP1401R1C()->get_temp2_rx_lo1(&temp));
    ui->lineEditTemp2->setText(QString("%1").arg(temp));
}

void QRfR1CDlg::on_pushButtonGetTemp3_clicked()
{
    double temp = 0.0;
    INT_CHECKV(getSP1401R1C()->get_temp3_rx_pa1(&temp));
    ui->lineEditTemp3->setText(QString("%1").arg(temp));
}

void QRfR1CDlg::on_pushButtonGetTemp4_clicked()
{
    double temp = 0.0;
    INT_CHECKV(getSP1401R1C()->get_temp4_tx_lo2(&temp));
    ui->lineEditTemp4->setText(QString("%1").arg(temp));
}

void QRfR1CDlg::on_pushButtonGetTemp5_clicked()
{
    double temp = 0.0;
    INT_CHECKV(getSP1401R1C()->get_temp5_tx_lo1(&temp));
    ui->lineEditTemp5->setText(QString("%1").arg(temp));
}

void QRfR1CDlg::on_pushButtonGetTemp6_clicked()
{
    double temp = 0.0;
    INT_CHECKV(getSP1401R1C()->get_temp6_tx_pa4(&temp));
    ui->lineEditTemp6->setText(QString("%1").arg(temp));
}

void QRfR1CDlg::on_pushButtonGetTemp7_clicked()
{
    double temp = 0.0;
    INT_CHECKV(getSP1401R1C()->get_temp7_tx_pa3(&temp));
    ui->lineEditTemp7->setText(QString("%1").arg(temp));
}

void QRfR1CDlg::on_pushButtonGetTempAll_clicked()
{
    on_pushButtonGetTemp0_clicked();
    on_pushButtonGetTemp1_clicked();
    on_pushButtonGetTemp2_clicked();
    on_pushButtonGetTemp3_clicked();
    on_pushButtonGetTemp4_clicked();
    on_pushButtonGetTemp5_clicked();
    on_pushButtonGetTemp6_clicked();
    on_pushButtonGetTemp7_clicked();
}

void QRfR1CDlg::on_pushButtonGetDetTxIF2_clicked()
{
    uint32_t det = 0;
    INT_CHECKV(getSP1401R1C()->get_ad7680_tx_if2(det));
    ui->lineEditDetTxIF2->setText(QString("%1").arg(det));
}

void QRfR1CDlg::on_pushButtonGetDetRxComb_clicked()
{
    uint32_t det = 0;
    INT_CHECKV(getSP1401R1C()->get_ad7680_rx_comb(det));
    ui->lineEditDetRxComb->setText(QString("%1").arg(det));
}

void QRfR1CDlg::on_pushButtonGetDetRxIF1_clicked()
{
    uint32_t det = 0;
    INT_CHECKV(getSP1401R1C()->get_ad7680_rx_if1(det));
    ui->lineEditDetRxIF1->setText(QString("%1").arg(det));
}

void QRfR1CDlg::on_pushButtonGetDetRxIF2_clicked()
{
    uint32_t det = 0;
    INT_CHECKV(getSP1401R1C()->get_ad7680_rx_if2(det));
    ui->lineEditDetRxIF2->setText(QString("%1").arg(det));
}

void QRfR1CDlg::on_pushButtonGetDetAll_clicked()
{
    on_pushButtonGetDetTxIF2_clicked();
    on_pushButtonGetDetRxComb_clicked();
    on_pushButtonGetDetRxIF1_clicked();
    on_pushButtonGetDetRxIF2_clicked();
}

void QRfR1CDlg::on_pushButtonSetSN_clicked()
{
    const char *ver = ui->lineEditHwVer->text().toStdString().c_str();
    int32_t ordinal = ui->lineEditOrdinal->text().toInt();
    INT_CHECKV(getSP1401R1C()->set_sn(ver,ordinal));
}

void QRfR1CDlg::on_pushButtonGetSN_clicked()
{
    char sn[32] = {0};
    memset(sn,0,sizeof(sn));
    INT_CHECKV(getSP1401R1C()->get_sn(sn));
    ui->lineEditSN->setText(QString("%1").arg(sn));
}
