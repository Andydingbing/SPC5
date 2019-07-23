#include "q_rf_r1a_dlg.h"
#include "ui_q_rf_r1a_dlg.h"
#include "algorithm.h"

QRFR1ADlg::QRFR1ADlg(QWidget *parent) :
    QRFDlg(parent),
    ui(new Ui::QRFR1ADlg)
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
    ui->comboBoxTxPAAttSw->setCurrentIndex(TX_ATT);
    ui->comboBoxTxAtt3->setCurrentIndex(0);
    ui->lineEditTxDet->setText("0");
    ui->lineEditTxTemp->setText("0");
    ui->comboBoxIOMode->setCurrentIndex(OUTPUT);
    ui->lineEditDGain->setText("0");

    ui->lineEditRxFreq->setText("2G");
    ui->comboBoxRxLNAAttSw->setCurrentIndex(int(RX_ATT));
    ui->comboBoxRxAtt1->setCurrentIndex(int(15.0 / 0.5));
    ui->comboBoxRxAtt2->setCurrentIndex(1);
    ui->comboBoxIFFilterSw->setCurrentIndex(int(basic_sp1401::_160MHz));
    ui->lineEditRxDet->setText("0");
    ui->lineEditRxTemp->setText("0");
    ui->comboBoxHwVer->setCurrentIndex(R1A);
    ui->lineEditOrdinal->setText("00000000");
    ui->lineEditSN->setText("");
}

QRFR1ADlg::~QRFR1ADlg()
{
    delete ui;
}

void QRFR1ADlg::on_pushButtonSetTxFreq_clicked()
{
    PTR_CHECK
    quint64 freq = freq_string_to<quint64>(ui->lineEditTxFreq->text().toStdString());
    INT_CHECKV(SP1401R1A()->set_tx_freq(freq));
}

void QRFR1ADlg::on_pushButtonSetTxAtt1_clicked()
{
    PTR_CHECK
    int32_t att = ui->comboBoxTxAtt1->currentIndex() * 10;
    INT_CHECKV(SP1401R1A()->set_tx_att1(att));
}

void QRFR1ADlg::on_pushButtonSetTxAtt2_clicked()
{
    PTR_CHECK
    int32_t att = ui->comboBoxTxAtt2->currentIndex() * 10;
    INT_CHECKV(SP1401R1A()->set_tx_att2(att));
}

void QRFR1ADlg::on_pushButtonSetTxPAAttSw_clicked()
{
    PTR_CHECK
    sp1401::tx_pa_att_t pa_att = (sp1401::tx_pa_att_t)(ui->comboBoxTxPAAttSw->currentIndex());
    INT_CHECKV(SP1401R1A()->set_tx_pa_att_sw(pa_att));
}

void QRFR1ADlg::on_pushButtonSetTxAtt3_clicked()
{
    PTR_CHECK
    int32_t att = ui->comboBoxTxAtt3->currentIndex() * 10;
    INT_CHECKV(SP1401R1A()->set_tx_att3(att));
}

void QRFR1ADlg::on_pushButtonGetTxDet_clicked()
{
    PTR_CHECK
    uint32_t det = 0;
    INT_CHECKV(SP1401R1A()->set_det_sw(basic_sp1401::CH_TX));
    INT_CHECKV(SP1401R1A()->get_ad7680(det));
    ui->lineEditTxDet->setText(QString("%1").arg(det));
}

void QRFR1ADlg::on_pushButtonGetTxTemp_clicked()
{
    PTR_CHECK
    double temp = 0.0;
    INT_CHECKV(SP1401R1A()->get_tx_temp(temp));
    ui->lineEditTxTemp->setText(QString("%1").arg(temp));
}

void QRFR1ADlg::on_pushButtonSetIOMode_clicked()
{
    PTR_CHECK
    sp1401::io_mode_t mode = (sp1401::io_mode_t)(ui->comboBoxIOMode->currentIndex());
    INT_CHECKV(SP1401R1A()->set_io_mode(mode));
}

void QRFR1ADlg::on_pushButtonSyncSetTxGain_clicked()
{
    PTR_CHECK
    int32_t att1 = ui->comboBoxTxAtt1->currentIndex() * 10;
    int32_t att2 = ui->comboBoxTxAtt2->currentIndex() * 10;
    int32_t att3 = ui->comboBoxTxAtt3->currentIndex() * 10;
    double d_gain = ui->lineEditDGain->text().toDouble();
    INT_CHECKV(SP1401R1A()->sync_set_tx_gain(att1,att2,att3,d_gain));
}

void QRFR1ADlg::on_pushButtonSetRxFreq_clicked()
{
    PTR_CHECK
    quint64 freq = freq_string_to<quint64>(ui->lineEditRxFreq->text().toStdString());
    INT_CHECKV(SP1401R1A()->set_rx_freq(freq));
}

void QRFR1ADlg::on_pushButtonSetRxLNAAttSw_clicked()
{
    PTR_CHECK
    sp1401::rx_lna_att_t lna_att = (sp1401::rx_lna_att_t)(ui->comboBoxRxLNAAttSw->currentIndex());
    INT_CHECKV(SP1401R1A()->set_rx_lna_att_sw(lna_att));
}

void QRFR1ADlg::on_pushButtonSetRxAtt1_clicked()
{
    PTR_CHECK
    double att = ui->comboBoxRxAtt1->currentIndex() * 0.5;
    INT_CHECKV(SP1401R1A()->set_rx_att1(att));
}

void QRFR1ADlg::on_pushButtonSetRxAtt2_clicked()
{
    PTR_CHECK
    int32_t att = ui->comboBoxRxAtt2->currentIndex() * 10;
    INT_CHECKV(SP1401R1A()->set_rx_att2(att));
}

void QRFR1ADlg::on_pushButtonSetIFFilterSw_clicked()
{
    PTR_CHECK
    basic_sp1401::rx_if_filter_t filter = (basic_sp1401::rx_if_filter_t)(ui->comboBoxIFFilterSw->currentIndex());
    INT_CHECKV(SP1401R1A()->set_rx_if_filter_sw(filter));
}

void QRFR1ADlg::on_pushButtonGetRxDet_clicked()
{
    PTR_CHECK
    uint32_t det = 0;
    INT_CHECKV(SP1401R1A()->set_det_sw(basic_sp1401::CH_RX));
    INT_CHECKV(SP1401R1A()->get_ad7680(det));
    ui->lineEditRxDet->setText(QString("%1").arg(det));
}

void QRFR1ADlg::on_pushButtonGetRxTemp_clicked()
{
    PTR_CHECK
    double temp = 0.0;
    INT_CHECKV(SP1401R1A()->get_rx_temp(temp));
    ui->lineEditRxTemp->setText(QString("%1").arg(temp));
}

void QRFR1ADlg::on_pushButtonSetSN_clicked()
{
    PTR_CHECK
    quint32 ordinal = ui->lineEditOrdinal->text().toUInt();
    hw_ver_t ver = hw_ver_t(ui->comboBoxHwVer->currentIndex());
    INT_CHECKV(SP1401R1A()->set_sn_major(ver,ordinal));
}

void QRFR1ADlg::on_pushButtonGetSN_clicked()
{
    PTR_CHECK
    char sn[32] = {0};
    memset(sn,0,sizeof(sn));
    INT_CHECKV(SP1401R1A()->get_sn_major(sn));
    ui->lineEditSN->setText(QString("%1").arg(sn));
}
