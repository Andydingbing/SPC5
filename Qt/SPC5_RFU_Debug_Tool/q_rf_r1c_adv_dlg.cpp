#include "q_rf_r1c_adv_dlg.h"
#include "ui_q_rf_r1c_adv_dlg.h"
#include "q_adf5355_dlg.h"
#include "freq_string.hpp"

QRFR1CAdvDlg::QRFR1CAdvDlg(QWidget *parent) :
    QRFDlg(parent),
    ui(new Ui::QRFR1CAdvDlg)
{
    ui->setupUi(this);

    dlgADF5355[0] = nullptr;
    dlgADF5355[1] = nullptr;
    dlgADF5355[2] = nullptr;

    double att[SERIE_SIZE(0.0,31.5,0.5)] = {0.0};
    for (uint16_t i = 0;i < ARRAY_SIZE(att);i ++) {
        att[i] = i * 0.5;
        QString strAtt = QString("%1").arg(att[i],2,'f',1);
        ui->comboBoxTXAtt0->addItem(strAtt);
        ui->comboBoxTXAtt1->addItem(strAtt);
        ui->comboBoxTXAtt2->addItem(strAtt);
        ui->comboBoxTXAtt3->addItem(strAtt);
        ui->comboBoxRXAtt1->addItem(strAtt);
        ui->comboBoxRXAtt2->addItem(strAtt);
        ui->comboBoxRXAtt3->addItem(strAtt);
    }
    ui->comboBoxTXAtt0->setMaxVisibleItems(20);
    ui->comboBoxTXAtt1->setMaxVisibleItems(20);
    ui->comboBoxTXAtt2->setMaxVisibleItems(20);
    ui->comboBoxTXAtt3->setMaxVisibleItems(20);
    ui->comboBoxTXAtt0->setCurrentIndex(int(5.0 / 0.5));
    ui->comboBoxTXAtt1->setCurrentIndex(int(20.0 / 0.5));
    ui->comboBoxTXAtt2->setCurrentIndex(int(0.0 / 0.5));
    ui->comboBoxTXAtt3->setCurrentIndex(int(1.0 / 0.5));

    ui->comboBoxRXAtt1->setMaxVisibleItems(20);
    ui->comboBoxRXAtt2->setMaxVisibleItems(20);
    ui->comboBoxRXAtt3->setMaxVisibleItems(20);
    ui->comboBoxRXAtt1->setCurrentIndex(int(5.0 / 0.5));
    ui->comboBoxRXAtt2->setCurrentIndex(int(15.0 / 0.5));
    ui->comboBoxRXAtt3->setCurrentIndex(int(10.0 / 0.5));
}

QRFR1CAdvDlg::~QRFR1CAdvDlg()
{
    delete ui;
}

void QRFR1CAdvDlg::on_comboBoxTXIOSw2_currentIndexChanged(int index)
{
    PTR_CHECK
    RD_TRY
    SP1401R1C()->set_tx_io_sw2(sp1401_r1c::tx_io_sw2_t(index));
    RD_CATCH_ALL_EXCEPTION
}

void QRFR1CAdvDlg::on_comboBoxTXIOSw1_currentIndexChanged(int index)
{
    PTR_CHECK
    RD_TRY
    SP1401R1C()->set_tx_io_sw1(sp1401_r1c::tx_io_sw1_t(index));
    RD_CATCH_ALL_EXCEPTION
}

void QRFR1CAdvDlg::on_comboBoxTXAtt3_currentIndexChanged(int index)
{
    PTR_CHECK
    RD_TRY
    SP1401R1C()->set_tx_att3(index * 0.5);
    RD_CATCH_ALL_EXCEPTION
}

void QRFR1CAdvDlg::on_comboBoxTXAtt2_currentIndexChanged(int index)
{
    PTR_CHECK
    RD_TRY
    SP1401R1C()->set_tx_att2(index * 0.5);
    RD_CATCH_ALL_EXCEPTION
}

void QRFR1CAdvDlg::on_comboBoxTXAtt1_currentIndexChanged(int index)
{
    PTR_CHECK
    RD_TRY
    SP1401R1C()->set_tx_att1(index * 0.5);
    RD_CATCH_ALL_EXCEPTION
}

void QRFR1CAdvDlg::on_comboBoxTXFilterSw1_currentIndexChanged(int index)
{
    Q_UNUSED(index);
    PTR_CHECK
    RD_TRY
    sp1401_r1c::tx_filter_t filter1 =
            sp1401_r1c::tx_filter_t(ui->comboBoxTXFilterSw1->currentIndex());
    sp1401_r1c::tx_filter_t filter2 =
            sp1401_r1c::tx_filter_t(ui->comboBoxTXFilterSw2->currentIndex());
    SP1401R1C()->set_tx_filter_sw(filter1,filter2);
    RD_CATCH_ALL_EXCEPTION
}

void QRFR1CAdvDlg::on_comboBoxTXFilterSw2_currentIndexChanged(int index)
{
    Q_UNUSED(index);
    PTR_CHECK
    RD_TRY
    sp1401_r1c::tx_filter_t filter1 =
            sp1401_r1c::tx_filter_t(ui->comboBoxTXFilterSw1->currentIndex());
    sp1401_r1c::tx_filter_t filter2 =
            sp1401_r1c::tx_filter_t(ui->comboBoxTXFilterSw2->currentIndex());
    SP1401R1C()->set_tx_filter_sw(filter1,filter2);
    RD_CATCH_ALL_EXCEPTION
}

void QRFR1CAdvDlg::on_comboBoxTXAtt0_currentIndexChanged(int index)
{
    PTR_CHECK
    RD_TRY
    SP1401R1C()->set_tx_att0(index * 0.5);
    RD_CATCH_ALL_EXCEPTION
}

void QRFR1CAdvDlg::on_pushButtonModEn_clicked()
{
    PTR_CHECK
    RD_TRY
    bool en = ui->pushButtonModEn->isChecked();
    SP1401R1C()->set_tx_modulator_en(en);
    ui->pushButtonModEn->setChecked(en);
    ui->pushButtonModEn->setText(en ? tr("En") : tr("Dis"));
    RD_CATCH_ALL_EXCEPTION
}

void QRFR1CAdvDlg::on_pushButtonPowerOn_clicked()
{
    PTR_CHECK
    RD_TRY
    bool en = ui->pushButtonPowerOn->isChecked();
    SP1401R1C()->set_pwr_en(en);
    ui->pushButtonPowerOn->setChecked(en);
    ui->pushButtonPowerOn->setText(en ? tr("On") : tr("Off"));
    RD_CATCH_ALL_EXCEPTION
}

void QRFR1CAdvDlg::on_comboBoxLoopSw_currentIndexChanged(int index)
{
    PTR_CHECK
    RD_TRY
    SP1401R1C()->set_loop_sw(sp1401_r1c::loop_sw_t(index));
    RD_CATCH_ALL_EXCEPTION
}

void QRFR1CAdvDlg::on_pushButtonSetTXLO3_clicked()
{
    PTR_CHECK
    RD_TRY
    quint64 freq = 0;
    quint64 freqLO1 = freq_string_to_uint64_t(ui->lineEditTXLO1->text().toStdString());
    quint64 freqLO2 = freq_string_to_uint64_t(ui->lineEditTXLO2->text().toStdString());
    quint64 freqLO3 = freq_string_to_uint64_t(ui->lineEditTXLO3->text().toStdString());
    bool isLock = false;

    sp1401_r1c::tx_lo2freq(freqLO1,freqLO2,freqLO3,freq);

    SP1401R1C()->set_adf5355(sp1401_r1c::TX_LO_3,freqLO3);
    SP1401R1C()->det_lo(sp1401_r1c::TX_LO_3,isLock);

    ui->lineEditTXFreq->setText(QString::fromStdString(freq_string_from_uint64_t(freq)));
    ui->checkBoxTXLO3->setChecked(isLock);
    RD_CATCH_ALL_EXCEPTION
}

void QRFR1CAdvDlg::on_pushButtonSetTXLO2_clicked()
{
    PTR_CHECK
    RD_TRY
    quint64 freq = 0;
    quint64 freqLO1 = freq_string_to_uint64_t(ui->lineEditTXLO1->text().toStdString());
    quint64 freqLO2 = freq_string_to_uint64_t(ui->lineEditTXLO2->text().toStdString());
    quint64 freqLO3 = freq_string_to_uint64_t(ui->lineEditTXLO3->text().toStdString());
    bool isLock = false;

    sp1401_r1c::tx_lo2freq(freqLO1,freqLO2,freqLO3,freq);

    SP1401R1C()->set_adf5355(sp1401_r1c::TX_LO_2,freqLO2);
    SP1401R1C()->det_lo(sp1401_r1c::TX_LO_2,isLock);

    ui->lineEditTXFreq->setText(QString::fromStdString(freq_string_from_uint64_t(freq)));
    ui->checkBoxTXLO2->setChecked(isLock);
    RD_CATCH_ALL_EXCEPTION
}

void QRFR1CAdvDlg::on_pushButtonSetTXLO1_clicked()
{
    PTR_CHECK
    RD_TRY
    quint64 freq = 0;
    quint64 freqLO1 = freq_string_to_uint64_t(ui->lineEditTXLO1->text().toStdString());
    quint64 freqLO2 = freq_string_to_uint64_t(ui->lineEditTXLO2->text().toStdString());
    quint64 freqLO3 = freq_string_to_uint64_t(ui->lineEditTXLO3->text().toStdString());
    bool isLock = false;

    sp1401_r1c::tx_lo2freq(freqLO1,freqLO2,freqLO3,freq);

    SP1401R1C()->set_adf5355(sp1401_r1c::TX_LO_1,freqLO1);
    SP1401R1C()->det_lo(sp1401_r1c::TX_LO_1,isLock);

    ui->lineEditTXFreq->setText(QString::fromStdString(freq_string_from_uint64_t(freq)));
    ui->checkBoxTXLO2->setChecked(isLock);
    RD_CATCH_ALL_EXCEPTION
}

void QRFR1CAdvDlg::on_pushButtonSetRXLO1_clicked()
{
    PTR_CHECK
    RD_TRY
    quint64 freq = 0;
    quint64 freqLO1 = freq_string_to_uint64_t(ui->lineEditRXLO1->text().toStdString());
    quint64 freqLO2 = freq_string_to_uint64_t(ui->lineEditTXLO2->text().toStdString());
    bool isLock = false;

    sp1401_r1c::rx_lo2freq(freqLO1,freqLO2,freq);

    SP1401R1C()->set_adf5355(sp1401_r1c::RX_LO_1,freqLO1);
    SP1401R1C()->det_lo(sp1401_r1c::RX_LO_1,isLock);

    ui->lineEditRXFreq->setText(QString::fromStdString(freq_string_from_uint64_t(freq)));
    ui->checkBoxRXLO1->setChecked(isLock);
    RD_CATCH_ALL_EXCEPTION
}

void QRFR1CAdvDlg::on_pushButtonAdvTXLO3_clicked()
{
    PTR_CHECK
    RD_TRY
    quint8 dlgIdx = 1;
    if (dlgADF5355[dlgIdx] == nullptr) {
        dlgADF5355[dlgIdx] = new QAdf5355Dlg;
        dlgADF5355[dlgIdx]->_sp1401_r1a = nullptr;
        dlgADF5355[dlgIdx]->_sp1401_r1c = SP1401R1C();
        dlgADF5355[dlgIdx]->loIdx = sp1401_r1c::TX_LO_3;
        dlgADF5355[dlgIdx]->setWindowFlags(Qt::WindowMinimizeButtonHint |
                                           Qt::WindowCloseButtonHint);
        dlgADF5355[dlgIdx]->setFixedSize(dlgADF5355[dlgIdx]->width(),
                                         dlgADF5355[dlgIdx]->height());
        dlgADF5355[dlgIdx]->setWindowTitle(QString("R1C/D/E_ADF5355_RFU%1_RF%2_TX_LO3")
                                           .arg(SP3301->get_rfu_idx())
                                           .arg(rfIdx));
    }

    dlgADF5355[dlgIdx]->setVisible(true);
    dlgADF5355[dlgIdx]->showNormal();
    dlgADF5355[dlgIdx]->activateWindow();
    RD_CATCH_ALL_EXCEPTION
}

void QRFR1CAdvDlg::on_pushButtonAdvTXLO2_clicked()
{
    PTR_CHECK
    RD_TRY
    quint8 dlgIdx = 0;
    if (dlgADF5355[dlgIdx] == nullptr) {
        dlgADF5355[dlgIdx] = new QAdf5355Dlg;
        dlgADF5355[dlgIdx]->_sp1401_r1a = nullptr;
        dlgADF5355[dlgIdx]->_sp1401_r1c = SP1401R1C();
        dlgADF5355[dlgIdx]->loIdx = sp1401_r1c::TX_LO_2;
        dlgADF5355[dlgIdx]->setWindowFlags(Qt::WindowMinimizeButtonHint |
                                           Qt::WindowCloseButtonHint);
        dlgADF5355[dlgIdx]->setFixedSize(dlgADF5355[dlgIdx]->width(),
                                         dlgADF5355[dlgIdx]->height());
        dlgADF5355[dlgIdx]->setWindowTitle(QString("R1C/D/E_ADF5355_RFU%1_RF%2_TX_LO2")
                                           .arg(SP3301->get_rfu_idx())
                                           .arg(rfIdx));
    }

    dlgADF5355[dlgIdx]->setVisible(true);
    dlgADF5355[dlgIdx]->showNormal();
    dlgADF5355[dlgIdx]->activateWindow();
    RD_CATCH_ALL_EXCEPTION
}

void QRFR1CAdvDlg::on_pushButtonAdvTXLO1_clicked()
{
    PTR_CHECKV(SP3301);
}

void QRFR1CAdvDlg::on_pushButtonAdvRXLO1_clicked()
{
    PTR_CHECK
    RD_TRY
    quint8 dlgIdx = 2;
    if (dlgADF5355[dlgIdx] == nullptr) {
        dlgADF5355[dlgIdx] = new QAdf5355Dlg;
        dlgADF5355[dlgIdx]->_sp1401_r1a = nullptr;
        dlgADF5355[dlgIdx]->_sp1401_r1c = SP1401R1C();
        dlgADF5355[dlgIdx]->loIdx = sp1401_r1c::RX_LO_1;
        dlgADF5355[dlgIdx]->setWindowFlags(Qt::WindowMinimizeButtonHint |
                                           Qt::WindowCloseButtonHint);
        dlgADF5355[dlgIdx]->setFixedSize(dlgADF5355[dlgIdx]->width(),
                                         dlgADF5355[dlgIdx]->height());
        dlgADF5355[dlgIdx]->setWindowTitle(QString("R1C/D/E_ADF5355_RFU%1_RF%2_RX_LO1")
                                           .arg(SP3301->get_rfu_idx())
                                           .arg(rfIdx));
    }

    dlgADF5355[dlgIdx]->setVisible(true);
    dlgADF5355[dlgIdx]->showNormal();
    dlgADF5355[dlgIdx]->activateWindow();
    RD_CATCH_ALL_EXCEPTION
}

void QRFR1CAdvDlg::on_comboBoxIOMode_currentIndexChanged(int index)
{
    PTR_CHECK
    RD_TRY
    SP1401R1C()->set_io_mode(sp1401::io_mode_t(index));
    RD_CATCH_ALL_EXCEPTION
}

void QRFR1CAdvDlg::on_checkBoxTXLED_clicked(bool checked)
{
    Q_UNUSED(checked);
    PTR_CHECK
    RD_TRY
    bool txLed = ui->checkBoxTXLED->isChecked();
    bool rxLed = ui->checkBoxRXLED->isChecked();
    SP1401R1C()->set_led(txLed,rxLed);
    RD_CATCH_ALL_EXCEPTION
}

void QRFR1CAdvDlg::on_checkBoxRXLED_clicked(bool checked)
{
    Q_UNUSED(checked);
    PTR_CHECK
    RD_TRY
    bool txLed = ui->checkBoxTXLED->isChecked();
    bool rxLed = ui->checkBoxRXLED->isChecked();
    SP1401R1C()->set_led(txLed,rxLed);
    RD_CATCH_ALL_EXCEPTION
}

void QRFR1CAdvDlg::on_pushButtonSetTXFreq_clicked()
{
    PTR_CHECKV(SP3301);
}

void QRFR1CAdvDlg::on_pushButtonSetRXFreq_clicked()
{
    PTR_CHECKV(SP3301);
}

void QRFR1CAdvDlg::on_pushButtonEEPROM_clicked()
{
    PTR_CHECKV(SP3301);
}

void QRFR1CAdvDlg::on_pushButtonMCP23S17_clicked()
{
    PTR_CHECKV(SP3301);
}

void QRFR1CAdvDlg::on_pushButtonGetAllTemp_clicked()
{
    PTR_CHECK
    RD_TRY

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
    for (quint8 i = 0;i < 8;i ++) {
        SP1401R1C()->get_temp(i,temp[i]);
        lineEditTemp[i]->setText(QString("%1").arg(temp[i]));
    }
    RD_CATCH_ALL_EXCEPTION
}

void QRFR1CAdvDlg::on_pushButtonGetAllDet_clicked()
{
    PTR_CHECK
    RD_TRY
    DECLARE_R1C_GETAD7680_FUNCTION_MAP

    uint32_t det[4] = {0};
    QLineEdit *lineEditDet[4] = {
        ui->lineEditDetRXComb,
        ui->lineEditDetRXIF1,
        ui->lineEditDetRXIF2,
        ui->lineEditDetTXIF2
    };
    for (quint8 i = 0;i < 4;i ++) {
        (SP1401R1C()->*get_r1c_ad7680[i])(det[i]);
        lineEditDet[i]->setText(QString("%1").arg(det[i]));
    }
    RD_CATCH_ALL_EXCEPTION
}

void QRFR1CAdvDlg::on_comboBoxRXIOSw1_currentIndexChanged(int index)
{
    PTR_CHECK
    RD_TRY
    SP1401R1C()->set_rx_io_sw1(sp1401_r1c::rx_io_sw1_t(index));
    RD_CATCH_ALL_EXCEPTION
}

void QRFR1CAdvDlg::on_comboBoxRXIOSw2_currentIndexChanged(int index)
{
    PTR_CHECK
    RD_TRY
    SP1401R1C()->set_rx_io_sw2(sp1401_r1c::rx_io_sw2_t(index));
    RD_CATCH_ALL_EXCEPTION
}

void QRFR1CAdvDlg::on_comboBoxRXIOSw3_currentIndexChanged(int index)
{
    PTR_CHECK
    RD_TRY
    SP1401R1C()->set_rx_io_sw3(sp1401_r1c::rx_io_sw3_t(index));
    RD_CATCH_ALL_EXCEPTION
}

void QRFR1CAdvDlg::on_comboBoxLNAAttSw_currentIndexChanged(int index)
{
    PTR_CHECK
    RD_TRY
    SP1401R1C()->set_rx_lna_att_sw(sp1401::rx_lna_att_t(index));
    RD_CATCH_ALL_EXCEPTION
}

void QRFR1CAdvDlg::on_comboBoxAtt019Sw_currentIndexChanged(int index)
{
    PTR_CHECK
    RD_TRY
    SP1401R1C()->set_rx_att_019_sw(sp1401::rx_att_019_t(index));
    RD_CATCH_ALL_EXCEPTION
}

void QRFR1CAdvDlg::on_comboBoxRXAtt1_currentIndexChanged(int index)
{
    PTR_CHECK
    RD_TRY
    SP1401R1C()->set_rx_att1(index * 0.5);
    RD_CATCH_ALL_EXCEPTION
}

void QRFR1CAdvDlg::on_comboBoxRXAtt2_currentIndexChanged(int index)
{
    PTR_CHECK
    RD_TRY
    SP1401R1C()->set_rx_att2(index * 0.5);
    RD_CATCH_ALL_EXCEPTION
}

void QRFR1CAdvDlg::on_comboBoxIFFilterSw_currentIndexChanged(int index)
{
    PTR_CHECK
    RD_TRY
    SP1401R1C()->set_rx_if_filter_sw(sp1401_r1c::rx_if_filter_t(index));
    RD_CATCH_ALL_EXCEPTION
}

void QRFR1CAdvDlg::on_comboBoxRXAtt3_currentIndexChanged(int index)
{
    PTR_CHECK
    RD_TRY
    SP1401R1C()->set_rx_att3(index * 0.5);
    RD_CATCH_ALL_EXCEPTION
}
