#include "qrfr1cadvdlg.h"
#include "ui_qrfr1cadvdlg.h"

QRfR1CAdvDlg::QRfR1CAdvDlg(QWidget *parent) :
    QRfDlg(parent),
    ui(new Ui::QRfR1CAdvDlg)
{
    ui->setupUi(this);

    dlgAdf5355[0] = NULL;
    dlgAdf5355[1] = NULL;
    dlgAdf5355[2] = NULL;

    double att[SERIE_SIZE(0.0,31.5,0.5)] = {0.0};
    for (uint16_t i = 0;i < ARRAY_SIZE(att);i ++) {
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
}

QRfR1CAdvDlg::~QRfR1CAdvDlg()
{
    delete ui;
}

void QRfR1CAdvDlg::on_comboBoxTxIOSw2_currentIndexChanged(int index)
{
    PTR_CHECKV(_sp3301);
    sp1401_r1c::tx_io_sw2_t sw = (sp1401_r1c::tx_io_sw2_t)(ui->comboBoxTxIOSw2->currentIndex());
    INT_CHECKV(getSP1401R1C()->set_tx_io_sw2(sw));
}

void QRfR1CAdvDlg::on_comboBoxTxIOSw1_currentIndexChanged(int index)
{
    PTR_CHECKV(_sp3301);
    sp1401_r1c::tx_io_sw1_t sw = (sp1401_r1c::tx_io_sw1_t)(ui->comboBoxTxIOSw1->currentIndex());
    INT_CHECKV(getSP1401R1C()->set_tx_io_sw1(sw));
}

void QRfR1CAdvDlg::on_comboBoxTxAtt3_currentIndexChanged(int index)
{
    PTR_CHECKV(_sp3301);
    double att = ui->comboBoxTxAtt3->currentIndex() * 0.5;
    INT_CHECKV(getSP1401R1C()->set_tx_att3(att));
}

void QRfR1CAdvDlg::on_comboBoxTxAtt2_currentIndexChanged(int index)
{
    PTR_CHECKV(_sp3301);
    double att = ui->comboBoxTxAtt2->currentIndex() * 0.5;
    INT_CHECKV(getSP1401R1C()->set_tx_att2(att));
}

void QRfR1CAdvDlg::on_comboBoxTxAtt1_currentIndexChanged(int index)
{
    PTR_CHECKV(_sp3301);
    double att = ui->comboBoxTxAtt1->currentIndex() * 0.5;
    INT_CHECKV(getSP1401R1C()->set_tx_att1(att));
}

void QRfR1CAdvDlg::on_comboBoxTxFilterSw1_currentIndexChanged(int index)
{
    PTR_CHECKV(_sp3301);
    sp1401_r1c::tx_filter_t filter1 = (sp1401_r1c::tx_filter_t)(ui->comboBoxTxFilterSw1->currentIndex());
    sp1401_r1c::tx_filter_t filter2 = (sp1401_r1c::tx_filter_t)(ui->comboBoxTxFilterSw2->currentIndex());
    INT_CHECKV(getSP1401R1C()->set_tx_filter_sw(filter1,filter2));
}

void QRfR1CAdvDlg::on_comboBoxTxFilterSw2_currentIndexChanged(int index)
{
    PTR_CHECKV(_sp3301);
    sp1401_r1c::tx_filter_t filter1 = (sp1401_r1c::tx_filter_t)(ui->comboBoxTxFilterSw1->currentIndex());
    sp1401_r1c::tx_filter_t filter2 = (sp1401_r1c::tx_filter_t)(ui->comboBoxTxFilterSw2->currentIndex());
    INT_CHECKV(getSP1401R1C()->set_tx_filter_sw(filter1,filter2));
}

void QRfR1CAdvDlg::on_comboBoxTxAtt0_currentIndexChanged(int index)
{
    PTR_CHECKV(_sp3301);
    double att = ui->comboBoxTxAtt0->currentIndex() * 0.5;
    INT_CHECKV(getSP1401R1C()->set_tx_att0(att));
}

void QRfR1CAdvDlg::on_pushButtonModEn_clicked()
{
    PTR_CHECKV(_sp3301);
    bool en = ui->pushButtonModEn->isChecked();
    if (getSP1401R1C()->set_tx_modulator_en(en)) {
        ui->pushButtonModEn->setChecked(false);
        ui->pushButtonModEn->setText(tr("Mod Dis"));
    }
    else
        ui->pushButtonModEn->setText(QString("Mod %1").arg(en ? "En" : "Dis"));
}

void QRfR1CAdvDlg::on_pushButtonPowerOn_clicked()
{
    PTR_CHECKV(_sp3301);
    bool en = ui->pushButtonPowerOn->isChecked();
    if (getSP1401R1C()->set_pwr_en(en)) {
        ui->pushButtonPowerOn->setChecked(false);
        ui->pushButtonPowerOn->setText(tr("Power Off"));
    }
    else
        ui->pushButtonPowerOn->setText(QString("Power %1").arg(en ? "On" : "Off"));
}

void QRfR1CAdvDlg::on_comboBoxLoopSw_currentIndexChanged(int index)
{
    PTR_CHECKV(_sp3301);
    sp1401_r1c::loop_sw_t sw = (sp1401_r1c::loop_sw_t)(ui->comboBoxLoopSw->currentIndex());
    INT_CHECKV(getSP1401R1C()->set_loop_sw(sw));
}

void QRfR1CAdvDlg::on_pushButtonSetTxLO3_clicked()
{
    PTR_CHECKV(_sp3301);
    uint64_t freq = 0;
    char szFreq[32] = {0};
    uint64_t freqLO1 = 0;
    uint64_t freqLO2 = 0;
    uint64_t freqLO3 = 0;
    bool isLock = false;

    str2freq(ui->lineEditTxLO1->text().toStdString().c_str(),freqLO1);
    str2freq(ui->lineEditTxLO2->text().toStdString().c_str(),freqLO2);
    str2freq(ui->lineEditTxLO3->text().toStdString().c_str(),freqLO3);
    getSP1401R1C()->tx_lo2freq(freqLO1,freqLO2,freqLO3,freq);
    freq2str(freq,szFreq);
    ui->lineEditTxFreq->setText(QString("%1").arg(szFreq));

    INT_CHECKV(getSP1401R1C()->set_adf5355(sp1401_r1c::TX_LO_3,freqLO3));
    INT_CHECKV(getSP1401R1C()->det_lo(sp1401_r1c::TX_LO_3,isLock));
    ui->checkBoxTxLO3->setChecked(isLock);
}

void QRfR1CAdvDlg::on_pushButtonSetTxLO2_clicked()
{
    PTR_CHECKV(_sp3301);
    uint64_t freq = 0;
    char szFreq[32] = {0};
    uint64_t freqLO1 = 0;
    uint64_t freqLO2 = 0;
    uint64_t freqLO3 = 0;
    bool isLock = false;

    str2freq(ui->lineEditTxLO1->text().toStdString().c_str(),freqLO1);
    str2freq(ui->lineEditTxLO2->text().toStdString().c_str(),freqLO2);
    str2freq(ui->lineEditTxLO3->text().toStdString().c_str(),freqLO3);
    getSP1401R1C()->tx_lo2freq(freqLO1,freqLO2,freqLO3,freq);
    freq2str(freq,szFreq);
    ui->lineEditTxFreq->setText(QString("%1").arg(szFreq));

    INT_CHECKV(getSP1401R1C()->set_adf5355(sp1401_r1c::TX_LO_2,freqLO2));
    INT_CHECKV(getSP1401R1C()->det_lo(sp1401_r1c::TX_LO_2,isLock));
    ui->checkBoxTxLO2->setChecked(isLock);
}

void QRfR1CAdvDlg::on_pushButtonSetTxLO1_clicked()
{
    PTR_CHECKV(_sp3301);
    uint64_t freq = 0;
    char szFreq[32] = {0};
    uint64_t freqLO1 = 0;
    uint64_t freqLO2 = 0;
    uint64_t freqLO3 = 0;
    bool isLock = false;

    str2freq(ui->lineEditTxLO1->text().toStdString().c_str(),freqLO1);
    str2freq(ui->lineEditTxLO2->text().toStdString().c_str(),freqLO2);
    str2freq(ui->lineEditTxLO3->text().toStdString().c_str(),freqLO3);
    getSP1401R1C()->tx_lo2freq(freqLO1,freqLO2,freqLO3,freq);
    freq2str(freq,szFreq);
    ui->lineEditTxFreq->setText(QString("%1").arg(szFreq));

    INT_CHECKV(getSP1401R1C()->set_hmc1197(freqLO2));
    INT_CHECKV(getSP1401R1C()->det_lo(sp1401_r1c::TX_LO_1,isLock));
    ui->checkBoxTxLO1->setChecked(isLock);
}

void QRfR1CAdvDlg::on_pushButtonSetRxLO1_clicked()
{
    PTR_CHECKV(_sp3301);
    uint64_t freq = 0;
    char szFreq[32] = {0};
    uint64_t freqLO1 = 0;
    uint64_t freqLO2 = 0;
    bool isLock = false;

    str2freq(ui->lineEditRxLO1->text().toStdString().c_str(),freqLO1);
    str2freq(ui->lineEditTxLO2->text().toStdString().c_str(),freqLO2);
    getSP1401R1C()->rx_lo2freq(freqLO1,freqLO2,freq);
    freq2str(freq,szFreq);
    ui->lineEditRxFreq->setText(QString("%1").arg(szFreq));

    INT_CHECKV(getSP1401R1C()->set_adf5355(sp1401_r1c::RX_LO_1,freqLO1));
    INT_CHECKV(getSP1401R1C()->det_lo(sp1401_r1c::RX_LO_1,isLock));
    ui->checkBoxRxLO1->setChecked(isLock);
}

void QRfR1CAdvDlg::on_pushButtonAdvTxLO3_clicked()
{
    PTR_CHECKV(_sp3301);
    qint8 adf5355Idx = 1;
    if (!dlgAdf5355[adf5355Idx]) {
        dlgAdf5355[adf5355Idx] = new QAdf5355Dlg;
        dlgAdf5355[adf5355Idx]->_sp1401_r1a = NULL;
        dlgAdf5355[adf5355Idx]->_sp1401_r1c = getSP1401R1C();
        dlgAdf5355[adf5355Idx]->loIdx = sp1401_r1c::TX_LO_3;
        dlgAdf5355[adf5355Idx]->setWindowFlags(Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint);
        dlgAdf5355[adf5355Idx]->setFixedSize(dlgAdf5355[adf5355Idx]->width(),
                                             dlgAdf5355[adf5355Idx]->height());
        dlgAdf5355[adf5355Idx]->setWindowTitle(QString("R1C/D_ADF5355_RFU%1_RF%2_TX_LO3").arg(_sp3301->get_rfu_idx()).arg(rfIdx));
    }

    dlgAdf5355[adf5355Idx]->setVisible(true);
    dlgAdf5355[adf5355Idx]->showNormal();
    dlgAdf5355[adf5355Idx]->activateWindow();
}

void QRfR1CAdvDlg::on_pushButtonAdvTxLO2_clicked()
{
    PTR_CHECKV(_sp3301);
    qint8 adf5355Idx = 0;
    if (!dlgAdf5355[adf5355Idx]) {
        dlgAdf5355[adf5355Idx] = new QAdf5355Dlg;
        dlgAdf5355[adf5355Idx]->_sp1401_r1a = NULL;
        dlgAdf5355[adf5355Idx]->_sp1401_r1c = getSP1401R1C();
        dlgAdf5355[adf5355Idx]->loIdx = sp1401_r1c::TX_LO_2;
        dlgAdf5355[adf5355Idx]->setWindowFlags(Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint);
        dlgAdf5355[adf5355Idx]->setFixedSize(dlgAdf5355[adf5355Idx]->width(),
                                             dlgAdf5355[adf5355Idx]->height());
        dlgAdf5355[adf5355Idx]->setWindowTitle(QString("R1C/D_ADF5355_RFU%1_RF%2_TX_LO2").arg(_sp3301->get_rfu_idx()).arg(rfIdx));
    }

    dlgAdf5355[adf5355Idx]->setVisible(true);
    dlgAdf5355[adf5355Idx]->showNormal();
    dlgAdf5355[adf5355Idx]->activateWindow();
}

void QRfR1CAdvDlg::on_pushButtonAdvTxLO1_clicked()
{
    PTR_CHECKV(_sp3301);
}

void QRfR1CAdvDlg::on_pushButtonAdvRxLO1_clicked()
{
    PTR_CHECKV(_sp3301);
    qint8 adf5355Idx = 2;
    if (!dlgAdf5355[adf5355Idx]) {
        dlgAdf5355[adf5355Idx] = new QAdf5355Dlg;
        dlgAdf5355[adf5355Idx]->_sp1401_r1a = NULL;
        dlgAdf5355[adf5355Idx]->_sp1401_r1c = getSP1401R1C();
        dlgAdf5355[adf5355Idx]->loIdx = sp1401_r1c::RX_LO_1;
        dlgAdf5355[adf5355Idx]->setWindowFlags(Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint);
        dlgAdf5355[adf5355Idx]->setFixedSize(dlgAdf5355[adf5355Idx]->width(),
                                             dlgAdf5355[adf5355Idx]->height());
        dlgAdf5355[adf5355Idx]->setWindowTitle(QString("R1C/D_ADF5355_RFU%1_RF%2_RX_LO1").arg(_sp3301->get_rfu_idx()).arg(rfIdx));
    }

    dlgAdf5355[adf5355Idx]->setVisible(true);
    dlgAdf5355[adf5355Idx]->showNormal();
    dlgAdf5355[adf5355Idx]->activateWindow();
}

void QRfR1CAdvDlg::on_comboBoxIOMode_currentIndexChanged(int index)
{
    PTR_CHECKV(_sp3301);
    sp1401::io_mode_t mode = (sp1401::io_mode_t)(ui->comboBoxIOMode->currentIndex());
    INT_CHECKV(getSP1401R1C()->set_io_mode(mode));
}

void QRfR1CAdvDlg::on_checkBoxTxLED_clicked(bool checked)
{
    PTR_CHECKV(_sp3301);
    bool txLed = ui->checkBoxTxLED->isChecked();
    bool rxLed = ui->checkBoxRxLED->isChecked();
    INT_CHECKV(getSP1401R1C()->set_led(txLed,rxLed));
}

void QRfR1CAdvDlg::on_checkBoxRxLED_clicked(bool checked)
{
    PTR_CHECKV(_sp3301);
    bool txLed = ui->checkBoxTxLED->isChecked();
    bool rxLed = ui->checkBoxRxLED->isChecked();
    INT_CHECKV(getSP1401R1C()->set_led(txLed,rxLed));
}

void QRfR1CAdvDlg::on_pushButtonSetTxFreq_clicked()
{
    PTR_CHECKV(_sp3301);
}

void QRfR1CAdvDlg::on_pushButtonSetRxFreq_clicked()
{
    PTR_CHECKV(_sp3301);
}

void QRfR1CAdvDlg::on_pushButtonEEPROM_clicked()
{
    PTR_CHECKV(_sp3301);
}

void QRfR1CAdvDlg::on_pushButtonMCP23S17_clicked()
{
    PTR_CHECKV(_sp3301);
}

void QRfR1CAdvDlg::on_pushButtonGetAllTemp_clicked()
{
    PTR_CHECKV(_sp3301);
    DECLARE_R1C_GETTEMP_FUNCTION_MAP

    double temp[8] = {0.0};
    QLineEdit *lineEditTemp[8] = {
        ui->lineEditTemp0,
        ui->lineEditTemp1,
        ui->lineEditTemp2,
        ui->lineEditTemp3,
        ui->lineEditTemp4,
        ui->lineEditTemp5,
        ui->lineEditTemp6,
        ui->lineEditTemp7
    };
    for (qint8 i = 0;i < 8;i ++) {
        INT_CHECKV((getSP1401R1C()->*get_r1c_temp[i])(&temp[i]));
        lineEditTemp[i]->setText(QString("%1").arg(temp[i]));
    }
}

void QRfR1CAdvDlg::on_pushButtonGetAllDet_clicked()
{
    PTR_CHECKV(_sp3301);
    DECLARE_R1C_GETAD7680_FUNCTION_MAP

    uint32_t det[4] = {0};
    QLineEdit *lineEditDet[4] = {
        ui->lineEditDetRxComb,
        ui->lineEditDetRxIF1,
        ui->lineEditDetRxIF2,
        ui->lineEditDetTxIF2
    };
    for (qint8 i = 0;i < 4;i ++) {
        INT_CHECKV((getSP1401R1C()->*get_r1c_ad7680[i])(det[i]));
        lineEditDet[i]->setText(QString("%1").arg(det[i]));
    }
}

void QRfR1CAdvDlg::on_comboBoxRxIOSw1_currentIndexChanged(int index)
{
    PTR_CHECKV(_sp3301);
    sp1401_r1c::rx_io_sw1_t sw = (sp1401_r1c::rx_io_sw1_t)(ui->comboBoxRxIOSw1->currentIndex());
    INT_CHECKV(getSP1401R1C()->set_rx_io_sw1(sw));
}

void QRfR1CAdvDlg::on_comboBoxRxIOSw2_currentIndexChanged(int index)
{
    PTR_CHECKV(_sp3301);
    sp1401_r1c::rx_io_sw2_t sw = (sp1401_r1c::rx_io_sw2_t)(ui->comboBoxRxIOSw2->currentIndex());
    INT_CHECKV(getSP1401R1C()->set_rx_io_sw2(sw));
}

void QRfR1CAdvDlg::on_comboBoxRxIOSw3_currentIndexChanged(int index)
{
    PTR_CHECKV(_sp3301);
    sp1401_r1c::rx_io_sw3_t sw = (sp1401_r1c::rx_io_sw3_t)(ui->comboBoxRxIOSw3->currentIndex());
    INT_CHECKV(getSP1401R1C()->set_rx_io_sw3(sw));
}

void QRfR1CAdvDlg::on_comboBoxLNAAttSw_currentIndexChanged(int index)
{
    PTR_CHECKV(_sp3301);
    sp1401::rx_lna_att_t sw = (sp1401::rx_lna_att_t)(ui->comboBoxLNAAttSw->currentIndex());
    INT_CHECKV(getSP1401R1C()->set_rx_lna_att_sw(sw));
}

void QRfR1CAdvDlg::on_comboBoxAtt019Sw_currentIndexChanged(int index)
{
    PTR_CHECKV(_sp3301);
    sp1401::rx_att_019_t sw = (sp1401::rx_att_019_t)(ui->comboBoxAtt019Sw->currentIndex());
    INT_CHECKV(getSP1401R1C()->set_rx_att_019_sw(sw));
}

void QRfR1CAdvDlg::on_comboBoxRxAtt1_currentIndexChanged(int index)
{
    PTR_CHECKV(_sp3301);
    double att = ui->comboBoxRxAtt1->currentIndex() * 0.5;
    INT_CHECKV(getSP1401R1C()->set_rx_att1(att));
}

void QRfR1CAdvDlg::on_comboBoxRxAtt2_currentIndexChanged(int index)
{
    PTR_CHECKV(_sp3301);
    double att = ui->comboBoxRxAtt2->currentIndex() * 0.5;
    INT_CHECKV(getSP1401R1C()->set_rx_att2(att));
}

void QRfR1CAdvDlg::on_comboBoxIFFilterSw_currentIndexChanged(int index)
{
    PTR_CHECKV(_sp3301);
    sp1401_r1c::rx_if_filter_t filter = (sp1401_r1c::rx_if_filter_t)(ui->comboBoxIFFilterSw->currentIndex());
    INT_CHECKV(getSP1401R1C()->set_rx_if_filter_sw(filter));
}

void QRfR1CAdvDlg::on_comboBoxRxAtt3_currentIndexChanged(int index)
{
    PTR_CHECKV(_sp3301);
    double att = ui->comboBoxRxAtt3->currentIndex() * 0.5;
    INT_CHECKV(getSP1401R1C()->set_rx_att3(att));
}
