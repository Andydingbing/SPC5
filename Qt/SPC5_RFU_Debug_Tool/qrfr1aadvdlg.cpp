#include "qrfr1aadvdlg.h"
#include "ui_qrfr1aadvdlg.h"

QRfR1AAdvDlg::QRfR1AAdvDlg(QWidget *parent) :
    QRfDlg(parent),
    ui(new Ui::QRfR1AAdvDlg)
{
    ui->setupUi(this);

    dlgAdf5355[sp1401_r1a::TX_LO_1] = NULL;
    dlgAdf5355[sp1401_r1a::TX_LO_2] = NULL;
    dlgAdf5355[sp1401_r1a::RX_LO_1] = NULL;
    dlgAdf5355[sp1401_r1a::RX_LO_2] = NULL;

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

    ui->lineEditRxFreq->setText("2G");
    ui->comboBoxRxLNAAttSw->setCurrentIndex(int(sp1401::RX_ATT));
    ui->comboBoxRxAtt1->setCurrentIndex(int(15.0 / 0.5));
    ui->comboBoxRxAtt2->setCurrentIndex(1);
    ui->comboBoxIFFilterSw->setCurrentIndex(int(basic_sp1401::_160MHz));
    ui->lineEditRxDet->setText("0");
    ui->lineEditRxTemp->setText("0");
}

QRfR1AAdvDlg::~QRfR1AAdvDlg()
{
    delete ui;
}

void QRfR1AAdvDlg::on_comboBoxTxIOSw_currentIndexChanged(int index)
{
    PTR_CHECKV(_sp3301);
    sp1401_r1a::tx_io_sw_t sw = (sp1401_r1a::tx_io_sw_t)(ui->comboBoxTxIOSw->currentIndex());
    INT_CHECKV(getSP1401R1A()->set_tx_io_sw(sw));
}

void QRfR1AAdvDlg::on_comboBoxTxAtt3_currentIndexChanged(int index)
{
    PTR_CHECKV(_sp3301);
    int32_t att = ui->comboBoxTxAtt3->currentIndex() * 10;
    INT_CHECKV(getSP1401R1A()->set_tx_att3(att));
}

void QRfR1AAdvDlg::on_comboBoxTxPAAttSw_currentIndexChanged(int index)
{
    PTR_CHECKV(_sp3301);
    sp1401::tx_pa_att_t pa_att = (sp1401::tx_pa_att_t)(ui->comboBoxTxPAAttSw->currentIndex());
    INT_CHECKV(getSP1401R1A()->set_tx_pa_att_sw(pa_att));
}

void QRfR1AAdvDlg::on_comboBoxTxAtt2_currentIndexChanged(int index)
{
    PTR_CHECKV(_sp3301);
    int32_t att = ui->comboBoxTxAtt2->currentIndex() * 10;
    INT_CHECKV(getSP1401R1A()->set_tx_att2(att));
}

void QRfR1AAdvDlg::on_comboBoxTxAtt1_currentIndexChanged(int index)
{
    PTR_CHECKV(_sp3301);
    int32_t att = ui->comboBoxTxAtt1->currentIndex() * 10;
    INT_CHECKV(getSP1401R1A()->set_tx_att1(att));
}

void QRfR1AAdvDlg::on_comboBoxTxBandSw_currentIndexChanged(int index)
{
    PTR_CHECKV(_sp3301);
    sp1401_r1a::tx_band_t band = (sp1401_r1a::tx_band_t)(ui->comboBoxTxBandSw->currentIndex());
    INT_CHECKV(getSP1401R1A()->set_tx_band_sw(band));
}

void QRfR1AAdvDlg::on_pushButtonModEn_clicked()
{
    PTR_CHECKV(_sp3301);
    bool en = ui->pushButtonModEn->isChecked();
    if (getSP1401R1A()->set_tx_modulator_en(en)) {
        ui->pushButtonModEn->setChecked(false);
        ui->pushButtonModEn->setText(tr("ADL5375 Dis"));
    }
    else
        ui->pushButtonModEn->setText(QString("ADL5375 %1").arg(en ? "En" : "Dis"));
}

void QRfR1AAdvDlg::on_pushButtonPowerOn_clicked()
{
    PTR_CHECKV(_sp3301);
    bool en = ui->pushButtonPowerOn->isChecked();
    if (getSP1401R1A()->set_pwr_en(en)) {
        ui->pushButtonPowerOn->setChecked(false);
        ui->pushButtonPowerOn->setText(tr("Power Off"));
    }
    else
        ui->pushButtonPowerOn->setText(QString("Power %1").arg(en ? "On" : "Off"));
}

void QRfR1AAdvDlg::on_comboBoxIOMode_currentIndexChanged(int index)
{
    PTR_CHECKV(_sp3301);
    sp1401::io_mode_t mode = (sp1401::io_mode_t)(ui->comboBoxIOMode->currentIndex());
    INT_CHECKV(getSP1401R1A()->set_io_mode(mode));
}

void QRfR1AAdvDlg::on_checkBoxTxLED_clicked()
{
    PTR_CHECKV(_sp3301);
    bool txLED = ui->checkBoxTxLED->isChecked();
    bool rxLED = ui->checkBoxRxLED->isChecked();
    INT_CHECKV(getSP1401R1A()->set_led(txLED,rxLED));
}

void QRfR1AAdvDlg::on_checkBoxRxLED_clicked()
{
    PTR_CHECKV(_sp3301);
    bool txLED = ui->checkBoxTxLED->isChecked();
    bool rxLED = ui->checkBoxRxLED->isChecked();
    INT_CHECKV(getSP1401R1A()->set_led(txLED,rxLED));
}

void QRfR1AAdvDlg::on_pushButtonSetTxFreq_clicked()
{

}

void QRfR1AAdvDlg::on_pushButtonSetRxFreq_clicked()
{

}

void QRfR1AAdvDlg::on_pushButtonGetTxTemp_clicked()
{
    PTR_CHECKV(_sp3301);
    double temp = 0.0;
    INT_CHECKV(getSP1401R1A()->get_tx_temp(temp));
    ui->lineEditTxTemp->setText(QString("%1").arg(temp));
}

void QRfR1AAdvDlg::on_pushButtonGetRxTemp_clicked()
{
    PTR_CHECKV(_sp3301);
    double temp = 0.0;
    INT_CHECKV(getSP1401R1A()->get_rx_temp(temp));
    ui->lineEditRxTemp->setText(QString("%1").arg(temp));
}

void QRfR1AAdvDlg::on_pushButtonGetTxDet_clicked()
{
    PTR_CHECKV(_sp3301);
    uint32_t det = 0;
    INT_CHECKV(getSP1401R1A()->set_det_sw(basic_sp1401::CH_TX));
    INT_CHECKV(getSP1401R1A()->get_ad7680(det));
    ui->lineEditTxDet->setText(QString("%1").arg(det));
}

void QRfR1AAdvDlg::on_pushButtonGetRxDet_clicked()
{
    PTR_CHECKV(_sp3301);
    uint32_t det = 0;
    INT_CHECKV(getSP1401R1A()->set_det_sw(basic_sp1401::CH_RX));
    INT_CHECKV(getSP1401R1A()->get_ad7680(det));
    ui->lineEditRxDet->setText(QString("%1").arg(det));
}

void QRfR1AAdvDlg::on_comboBoxRxIOSw1_currentIndexChanged(int index)
{
    PTR_CHECKV(_sp3301);
    sp1401_r1a::rx_io_sw1_t sw = (sp1401_r1a::rx_io_sw1_t)(ui->comboBoxRxIOSw1->currentIndex());
    INT_CHECKV(getSP1401R1A()->set_rx_io_sw1(sw));
}

void QRfR1AAdvDlg::on_comboBoxRxIOSw2_currentIndexChanged(int index)
{
    PTR_CHECKV(_sp3301);
    sp1401_r1a::rx_io_sw2_t sw = (sp1401_r1a::rx_io_sw2_t)(ui->comboBoxRxIOSw2->currentIndex());
    INT_CHECKV(getSP1401R1A()->set_rx_io_sw2(sw));
}

void QRfR1AAdvDlg::on_comboBoxRxIOSw_currentIndexChanged(int index)
{
    PTR_CHECKV(_sp3301);
    sp1401_r1a::rx_io_sw_t sw = (sp1401_r1a::rx_io_sw_t)(ui->comboBoxRxIOSw->currentIndex());
    INT_CHECKV(getSP1401R1A()->set_rx_io_sw(sw));
}

void QRfR1AAdvDlg::on_comboBoxRxLNAAttSw_currentIndexChanged(int index)
{
    PTR_CHECKV(_sp3301);
    sp1401::rx_lna_att_t lna_att = (sp1401::rx_lna_att_t)(ui->comboBoxRxLNAAttSw->currentIndex());
    INT_CHECKV(getSP1401R1A()->set_rx_lna_att_sw(lna_att));
}

void QRfR1AAdvDlg::on_comboBoxRxAtt1_currentIndexChanged(int index)
{
    PTR_CHECKV(_sp3301);
    double att = ui->comboBoxRxAtt1->currentIndex() * 0.5;
    INT_CHECKV(getSP1401R1A()->set_rx_att1(att));
}

void QRfR1AAdvDlg::on_comboBoxRxAtt2_currentIndexChanged(int index)
{
    PTR_CHECKV(_sp3301);
    int32_t att = ui->comboBoxRxAtt2->currentIndex() * 10;
    INT_CHECKV(getSP1401R1A()->set_rx_att2(att));
}

void QRfR1AAdvDlg::on_comboBoxIFFilterSw_currentIndexChanged(int index)
{
    PTR_CHECKV(_sp3301);
    basic_sp1401::rx_if_filter_t filter = (basic_sp1401::rx_if_filter_t)(ui->comboBoxIFFilterSw->currentIndex());
    INT_CHECKV(getSP1401R1A()->set_rx_if_filter_sw(filter));
}

void QRfR1AAdvDlg::on_pushButtonAdvTxLO1_clicked()
{
    qint8 loIdx = sp1401_r1a::TX_LO_1;
    if (!dlgAdf5355[loIdx]) {
        dlgAdf5355[loIdx] = new QAdf5355Dlg;
        dlgAdf5355[loIdx]->_sp1401_r1a = getSP1401R1A();
        dlgAdf5355[loIdx]->_sp1401_r1c = NULL;
        dlgAdf5355[loIdx]->loIdx = loIdx;
        dlgAdf5355[loIdx]->setWindowFlags(Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint);
        dlgAdf5355[loIdx]->setFixedSize(dlgAdf5355[loIdx]->width(),dlgAdf5355[loIdx]->height());
        dlgAdf5355[loIdx]->setWindowTitle(QString("R1A/B_ADF5355_RFU%1_RF%2_TX_LO1").arg(_sp3301->get_rfu_idx()).arg(rfIdx));
    }

    dlgAdf5355[loIdx]->setVisible(true);
    dlgAdf5355[loIdx]->showNormal();
    dlgAdf5355[loIdx]->activateWindow();
}

void QRfR1AAdvDlg::on_pushButtonAdvTxLO2_clicked()
{
    qint8 loIdx = sp1401_r1a::TX_LO_2;
    if (!dlgAdf5355[loIdx]) {
        dlgAdf5355[loIdx] = new QAdf5355Dlg;
        dlgAdf5355[loIdx]->_sp1401_r1a = getSP1401R1A();
        dlgAdf5355[loIdx]->_sp1401_r1c = NULL;
        dlgAdf5355[loIdx]->loIdx = loIdx;
        dlgAdf5355[loIdx]->setWindowFlags(Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint);
        dlgAdf5355[loIdx]->setFixedSize(dlgAdf5355[loIdx]->width(),dlgAdf5355[loIdx]->height());
        dlgAdf5355[loIdx]->setWindowTitle(QString("R1A/B_ADF5355_RFU%1_RF%2_TX_LO2").arg(_sp3301->get_rfu_idx()).arg(rfIdx));
    }

    dlgAdf5355[loIdx]->setVisible(true);
    dlgAdf5355[loIdx]->showNormal();
    dlgAdf5355[loIdx]->activateWindow();
}

void QRfR1AAdvDlg::on_pushButtonAdvRxLO1_clicked()
{
    qint8 loIdx = sp1401_r1a::RX_LO_1;
    if (!dlgAdf5355[loIdx]) {
        dlgAdf5355[loIdx] = new QAdf5355Dlg;
        dlgAdf5355[loIdx]->_sp1401_r1a = getSP1401R1A();
        dlgAdf5355[loIdx]->_sp1401_r1c = NULL;
        dlgAdf5355[loIdx]->loIdx = loIdx;
        dlgAdf5355[loIdx]->setWindowFlags(Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint);
        dlgAdf5355[loIdx]->setFixedSize(dlgAdf5355[loIdx]->width(),dlgAdf5355[loIdx]->height());
        dlgAdf5355[loIdx]->setWindowTitle(QString("R1A/B_ADF5355_RFU%1_RF%2_RX_LO1").arg(_sp3301->get_rfu_idx()).arg(rfIdx));
    }

    dlgAdf5355[loIdx]->setVisible(true);
    dlgAdf5355[loIdx]->showNormal();
    dlgAdf5355[loIdx]->activateWindow();
}

void QRfR1AAdvDlg::on_pushButtonAdvRxLO2_clicked()
{
    qint8 loIdx = sp1401_r1a::RX_LO_2;
    if (!dlgAdf5355[loIdx]) {
        dlgAdf5355[loIdx] = new QAdf5355Dlg;
        dlgAdf5355[loIdx]->_sp1401_r1a = getSP1401R1A();
        dlgAdf5355[loIdx]->_sp1401_r1c = NULL;
        dlgAdf5355[loIdx]->loIdx = loIdx;
        dlgAdf5355[loIdx]->setWindowFlags(Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint);
        dlgAdf5355[loIdx]->setFixedSize(dlgAdf5355[loIdx]->width(),dlgAdf5355[loIdx]->height());
        dlgAdf5355[loIdx]->setWindowTitle(QString("R1A/B_ADF5355_RFU%1_RF%2_RX_LO2").arg(_sp3301->get_rfu_idx()).arg(rfIdx));
    }

    dlgAdf5355[loIdx]->setVisible(true);
    dlgAdf5355[loIdx]->showNormal();
    dlgAdf5355[loIdx]->activateWindow();
}

void QRfR1AAdvDlg::on_pushButtonSetTxLO1_clicked()
{
    PTR_CHECKV(_sp3301);
    uint64_t freq = 0;
    char szFreq[32] = {0};
    uint64_t freqLO1 = 0;
    uint64_t freqLO2 = 0;
    sp1401_r1a::tx_band_t band = (sp1401_r1a::tx_band_t)(ui->comboBoxTxBandSw->currentIndex());
    bool isLock = false;

    str2freq(ui->lineEditTxLO1->text().toStdString().c_str(),freqLO1);
    str2freq(ui->lineEditTxLO2->text().toStdString().c_str(),freqLO2);
    getSP1401R1A()->tx_lo2freq(freqLO1,freqLO2,band,freq);
    freq2str(freq,szFreq);
    ui->lineEditTxFreq->setText(QString("%1").arg(szFreq));

    INT_CHECKV(getSP1401R1A()->set_lo(sp1401_r1a::TX_LO_1,freqLO1));
    INT_CHECKV(getSP1401R1A()->det_lo(sp1401_r1a::TX_LO_1,isLock));
    ui->checkBoxTxLO1->setChecked(isLock);
}

void QRfR1AAdvDlg::on_pushButtonSetTxLO2_clicked()
{
    PTR_CHECKV(_sp3301);
    uint64_t freq = 0;
    char szFreq[32] = {0};
    uint64_t freqLO1 = 0;
    uint64_t freqLO2 = 0;
    sp1401_r1a::tx_band_t band = (sp1401_r1a::tx_band_t)(ui->comboBoxTxBandSw->currentIndex());
    bool isLock = false;

    str2freq(ui->lineEditTxLO1->text().toStdString().c_str(),freqLO1);
    str2freq(ui->lineEditTxLO2->text().toStdString().c_str(),freqLO2);
    getSP1401R1A()->tx_lo2freq(freqLO1,freqLO2,band,freq);
    freq2str(freq,szFreq);
    ui->lineEditTxFreq->setText(QString("%1").arg(szFreq));

    INT_CHECKV(getSP1401R1A()->set_lo(sp1401_r1a::TX_LO_2,freqLO2));
    INT_CHECKV(getSP1401R1A()->det_lo(sp1401_r1a::TX_LO_2,isLock));
    ui->checkBoxTxLO2->setChecked(isLock);
}

void QRfR1AAdvDlg::on_pushButtonSetRxLO1_clicked()
{
    PTR_CHECKV(_sp3301);
    uint64_t freq = 0;
    char szFreq[32] = {0};
    uint64_t freqLO1 = 0;
    uint64_t freqLO2 = 0;
    bool isLock = false;

    str2freq(ui->lineEditRxLO1->text().toStdString().c_str(),freqLO1);
    str2freq(ui->lineEditRxLO2->text().toStdString().c_str(),freqLO2);
    getSP1401R1A()->rx_lo2freq(freqLO1,freqLO2,freq);
    freq2str(freq,szFreq);
    ui->lineEditRxFreq->setText(QString("%1").arg(szFreq));

    INT_CHECKV(getSP1401R1A()->set_lo(sp1401_r1a::RX_LO_1,freqLO1));
    INT_CHECKV(getSP1401R1A()->det_lo(sp1401_r1a::RX_LO_1,isLock));
    ui->checkBoxRxLO1->setChecked(isLock);
}

void QRfR1AAdvDlg::on_pushButtonSetRxLO2_clicked()
{
    PTR_CHECKV(_sp3301);
    uint64_t freq = 0;
    char szFreq[32] = {0};
    uint64_t freqLO1 = 0;
    uint64_t freqLO2 = 0;
    bool isLock = false;

    str2freq(ui->lineEditRxLO1->text().toStdString().c_str(),freqLO1);
    str2freq(ui->lineEditRxLO2->text().toStdString().c_str(),freqLO2);
    getSP1401R1A()->rx_lo2freq(freqLO1,freqLO2,freq);
    freq2str(freq,szFreq);
    ui->lineEditRxFreq->setText(QString("%1").arg(szFreq));

    INT_CHECKV(getSP1401R1A()->set_lo(sp1401_r1a::RX_LO_2,freqLO2));
    INT_CHECKV(getSP1401R1A()->det_lo(sp1401_r1a::RX_LO_2,isLock));
    ui->checkBoxRxLO2->setChecked(isLock);
}
