#include "qrfr1adlg.h"
#include "ui_qrfr1adlg.h"

QRfR1ADlg::QRfR1ADlg(QWidget *parent) :
    QRfDlg(parent),
    ui(new Ui::QRfR1ADlg)
{
    ui->setupUi(this);

    QString strAtt[4] = {"0","10","20","30"};
    for (qint8 i = 0;i < ARRAY_SIZE(strAtt);i ++) {
        ui->comboBoxTxAtt1->addItem(strAtt[i]);
        ui->comboBoxTxAtt2->addItem(strAtt[i]);
        ui->comboBoxTxAtt3->addItem(strAtt[i]);
        ui->comboBoxRxAtt2->addItem(strAtt[i]);
    }
    double rxAtt1[SERIE_SIZE(0.0,31.5,0.5)] = {0.0};
    for (qint8 i = 0;i < ARRAY_SIZE(rxAtt1);i ++) {
        rxAtt1[i] = i * 0.5;
        QString strRxAtt1 = QString("%1").arg(rxAtt1[i],2,'f',1);
        ui->comboBoxRxAtt1->addItem(strRxAtt1);
    }
    ui->comboBoxRxAtt1->setMaxVisibleItems(20);

    ui->lineEditTxFreq->setText("2G");
    ui->comboBoxTxAtt1->setCurrentIndex(1);
    ui->comboBoxTxAtt2->setCurrentIndex(0);
    ui->comboBoxTxPAAttSw->setCurrentIndex(int(sp1401::TX_ATT));
    ui->comboBoxTxAtt3->setCurrentIndex(0);
    ui->lineEditTxDet->setText("0");
    ui->lineEditTxTemp->setText("0");
    ui->comboBoxIOMode->setCurrentIndex(int(sp1401::OUTPUT));
    ui->lineEditDGain->setText("0");

    ui->lineEditRxFreq->setText("2G");
    ui->comboBoxRxLNAAttSw->setCurrentIndex(int(sp1401::RX_ATT));
    ui->comboBoxRxAtt1->setCurrentIndex(int(15.0 / 0.5));
    ui->comboBoxRxAtt2->setCurrentIndex(1);
    ui->comboBoxIFFilterSw->setCurrentIndex(int(basic_sp1401::_160MHz));
    ui->lineEditRxDet->setText("0");
    ui->lineEditRxTemp->setText("0");
    ui->lineEditHwVer->setText("SP1401R1B");
    ui->lineEditOrdinal->setText("00000000");
    ui->lineEditSN->setText("");
}

QRfR1ADlg::~QRfR1ADlg()
{
    delete ui;
}

void QRfR1ADlg::on_pushButtonSetTxFreq_clicked()
{
    PTR_CHECKV(_sp3301);
    uint64_t freq = 0;
    str2freq(ui->lineEditTxFreq->text().toStdString().c_str(),freq);
    INT_CHECKV(getSP1401R1A()->set_tx_freq(freq));
}

void QRfR1ADlg::on_pushButtonSetTxAtt1_clicked()
{
    PTR_CHECKV(_sp3301);
    int32_t att = ui->comboBoxTxAtt1->currentIndex() * 10;
    INT_CHECKV(getSP1401R1A()->set_tx_att1(att));
}

void QRfR1ADlg::on_pushButtonSetTxAtt2_clicked()
{
    PTR_CHECKV(_sp3301);
    int32_t att = ui->comboBoxTxAtt2->currentIndex() * 10;
    INT_CHECKV(getSP1401R1A()->set_tx_att2(att));
}

void QRfR1ADlg::on_pushButtonSetTxPAAttSw_clicked()
{
    PTR_CHECKV(_sp3301);
    sp1401::tx_pa_att_t pa_att = (sp1401::tx_pa_att_t)(ui->comboBoxTxPAAttSw->currentIndex());
    INT_CHECKV(getSP1401R1A()->set_tx_pa_att_sw(pa_att));
}

void QRfR1ADlg::on_pushButtonSetTxAtt3_clicked()
{
    PTR_CHECKV(_sp3301);
    int32_t att = ui->comboBoxTxAtt3->currentIndex() * 10;
    INT_CHECKV(getSP1401R1A()->set_tx_att3(att));
}

void QRfR1ADlg::on_pushButtonGetTxDet_clicked()
{
    PTR_CHECKV(_sp3301);
    uint32_t det = 0;
    INT_CHECKV(getSP1401R1A()->set_det_sw(basic_sp1401::CH_TX));
    INT_CHECKV(getSP1401R1A()->get_ad7680(det));
    ui->lineEditTxDet->setText(QString("%1").arg(det));
}

void QRfR1ADlg::on_pushButtonGetTxTemp_clicked()
{
    PTR_CHECKV(_sp3301);
    double temp = 0.0;
    INT_CHECKV(getSP1401R1A()->get_tx_temp(temp));
    ui->lineEditTxTemp->setText(QString("%1").arg(temp));
}

void QRfR1ADlg::on_pushButtonSetIOMode_clicked()
{
    PTR_CHECKV(_sp3301);
    sp1401::io_mode_t mode = (sp1401::io_mode_t)(ui->comboBoxIOMode->currentIndex());
    INT_CHECKV(getSP1401R1A()->set_io_mode(mode));
}

void QRfR1ADlg::on_pushButtonSyncSetTxGain_clicked()
{
    PTR_CHECKV(_sp3301);
    int32_t att1 = ui->comboBoxTxAtt1->currentIndex() * 10;
    int32_t att2 = ui->comboBoxTxAtt2->currentIndex() * 10;
    int32_t att3 = ui->comboBoxTxAtt3->currentIndex() * 10;
    double d_gain = ui->lineEditDGain->text().toDouble();
    INT_CHECKV(getSP1401R1A()->sync_set_tx_gain(att1,att2,att3,d_gain));
}

void QRfR1ADlg::on_pushButtonSetRxFreq_clicked()
{
    PTR_CHECKV(_sp3301);
    uint64_t freq = 0;
    str2freq(ui->lineEditRxFreq->text().toStdString().c_str(),freq);
    INT_CHECKV(getSP1401R1A()->set_rx_freq(freq));
}

void QRfR1ADlg::on_pushButtonSetRxLNAAttSw_clicked()
{
    PTR_CHECKV(_sp3301);
    sp1401::rx_lna_att_t lna_att = (sp1401::rx_lna_att_t)(ui->comboBoxRxLNAAttSw->currentIndex());
    INT_CHECKV(getSP1401R1A()->set_rx_lna_att_sw(lna_att));
}

void QRfR1ADlg::on_pushButtonSetRxAtt1_clicked()
{
    PTR_CHECKV(_sp3301);
    double att = ui->comboBoxRxAtt1->currentIndex() * 0.5;
    INT_CHECKV(getSP1401R1A()->set_rx_att1(att));
}

void QRfR1ADlg::on_pushButtonSetRxAtt2_clicked()
{
    PTR_CHECKV(_sp3301);
    int32_t att = ui->comboBoxRxAtt2->currentIndex() * 10;
    INT_CHECKV(getSP1401R1A()->set_rx_att2(att));
}

void QRfR1ADlg::on_pushButtonSetIFFilterSw_clicked()
{
    PTR_CHECKV(_sp3301);
    basic_sp1401::rx_if_filter_t filter = (basic_sp1401::rx_if_filter_t)(ui->comboBoxIFFilterSw->currentIndex());
    INT_CHECKV(getSP1401R1A()->set_rx_if_filter_sw(filter));
}

void QRfR1ADlg::on_pushButtonGetRxDet_clicked()
{
    PTR_CHECKV(_sp3301);
    uint32_t det = 0;
    INT_CHECKV(getSP1401R1A()->set_det_sw(basic_sp1401::CH_RX));
    INT_CHECKV(getSP1401R1A()->get_ad7680(det));
    ui->lineEditRxDet->setText(QString("%1").arg(det));
}

void QRfR1ADlg::on_pushButtonGetRxTemp_clicked()
{
    PTR_CHECKV(_sp3301);
    double temp = 0.0;
    INT_CHECKV(getSP1401R1A()->get_rx_temp(temp));
    ui->lineEditRxTemp->setText(QString("%1").arg(temp));
}

void QRfR1ADlg::on_pushButtonSetSN_clicked()
{
    PTR_CHECKV(_sp3301);
    const char *ver = ui->lineEditHwVer->text().toStdString().c_str();
    int32_t ordinal = ui->lineEditOrdinal->text().toInt();
    INT_CHECKV(getSP1401R1A()->set_sn(ver,ordinal));
}

void QRfR1ADlg::on_pushButtonGetSN_clicked()
{
    PTR_CHECKV(_sp3301);
    char sn[32] = {0};
    memset(sn,0,sizeof(sn));
    INT_CHECKV(getSP1401R1A()->get_sn(sn));
    ui->lineEditSN->setText(QString("%1").arg(sn));
}
