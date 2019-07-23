#include "q_rf_r1c_dlg.h"
#include "ui_q_rf_r1c_dlg.h"
#include "exception.hpp"
#include "freq_string.hpp"

QRFR1CDlg::QRFR1CDlg(QWidget *parent) :
    QRFDlg(parent),
    ui(new Ui::QRFR1CDlg)
{
    ui->setupUi(this);

    double att[SERIE_SIZE(0.0,31.5,0.5)] = {0.0};
    for (qint8 i = 0;i < ARRAY_SIZE(att);i ++) {
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
    ui->comboBoxRXLNAAttSw->addItem(QString("Attenuation"));
    ui->comboBoxRXLNAAttSw->addItem(QString("LNA"));
    ui->comboBoxRXLNAAttSw->setCurrentIndex(RX_ATT);
    ui->comboBoxRXAtt019Sw->addItem(QString("Att 19"));
    ui->comboBoxRXAtt019Sw->addItem(QString("Att 0"));
    ui->comboBoxRXAtt019Sw->setCurrentIndex(RX_ATT_19);
    ui->comboBoxIFFilterSw->addItem(QString("100M"));
    ui->comboBoxIFFilterSw->addItem(QString("160M"));
    ui->comboBoxIFFilterSw->setCurrentIndex(basic_sp1401::_160MHz);
    ui->comboBoxIOMode->addItem(QString("IO"));
    ui->comboBoxIOMode->addItem(QString("Output"));
    ui->comboBoxIOMode->addItem(QString("Loopback"));
    ui->comboBoxIOMode->setCurrentIndex(int(sp1401::OUTPUT));

    ui->lineEditTXFreq->setText("2G");
    ui->lineEditTXFreq->setToolTip(STRING_FREQ_TIPS);
    ui->lineEditDGain->setText("0");

    ui->lineEditRXFreq->setText("2G");
    ui->lineEditRXFreq->setToolTip(STRING_FREQ_TIPS);

    ui->lineEditTemp0->setText("0.0");
    ui->lineEditTemp1->setText("0.0");
    ui->lineEditTemp2->setText("0.0");
    ui->lineEditTemp3->setText("0.0");
    ui->lineEditTemp4->setText("0.0");
    ui->lineEditTemp5->setText("0.0");
    ui->lineEditTemp6->setText("0.0");
    ui->lineEditTemp7->setText("0.0");

    ui->lineEditDetTXIF2->setText("0");
    ui->lineEditDetRXComb->setText("0");
    ui->lineEditDetRXIF1->setText("0");
    ui->lineEditDetRXIF2->setText("0");

    ui->comboBoxHwVer->setCurrentIndex(R1D - R1C);
    ui->lineEditOrdinal->setText("00000000");
}

QRFR1CDlg::~QRFR1CDlg()
{
    delete ui;
}

void QRFR1CDlg::on_lineEditTXFreq_returnPressed()
{
    PTR_CHECK
    RD_TRY
    uint64_t freq = freq_string_to_uint64_t(ui->lineEditTXFreq->text().toStdString());
    SP1401R1C()->set_tx_freq(freq);
    RD_CATCH_ALL_EXCEPTION
}

void QRFR1CDlg::on_comboBoxTXAtt0_currentIndexChanged(int index)
{
    PTR_CHECK
    RD_TRY
    SP1401R1C()->set_tx_att0(index * 0.5);
    RD_CATCH_ALL_EXCEPTION
}

void QRFR1CDlg::on_comboBoxTXAtt1_currentIndexChanged(int index)
{
    PTR_CHECK
    RD_TRY
    SP1401R1C()->set_tx_att1(index * 0.5);
    RD_CATCH_ALL_EXCEPTION
}

void QRFR1CDlg::on_comboBoxTXAtt2_currentIndexChanged(int index)
{
    PTR_CHECK
    RD_TRY
    SP1401R1C()->set_tx_att2(index * 0.5);
    RD_CATCH_ALL_EXCEPTION
}

void QRFR1CDlg::on_comboBoxTXAtt3_currentIndexChanged(int index)
{
    PTR_CHECK
    RD_TRY
    SP1401R1C()->set_tx_att3(index * 0.5);
    RD_CATCH_ALL_EXCEPTION
}

void QRFR1CDlg::on_comboBoxIOMode_currentIndexChanged(int index)
{
    PTR_CHECK
    RD_TRY
    SP1401R1C()->set_io_mode(io_mode_t(index));
    RD_CATCH_ALL_EXCEPTION
}

void QRFR1CDlg::on_lineEditDGain_textChanged(const QString &arg1)
{
    PTR_CHECK
    RD_TRY
    double att0 = ui->comboBoxTXAtt0->currentIndex() * 0.5;
    double att1 = ui->comboBoxTXAtt1->currentIndex() * 0.5;
    double att2 = ui->comboBoxTXAtt2->currentIndex() * 0.5;
    double att3 = ui->comboBoxTXAtt3->currentIndex() * 0.5;
    double d_gain = arg1.toDouble();
    SP1401R1C()->sync_set_tx_gain(att0,att1,att2,att3,d_gain);
    RD_CATCH_ALL_EXCEPTION
}

void QRFR1CDlg::on_pushButtonSetTXAll_clicked()
{
    PTR_CHECK
    RD_TRY
    double att0 = ui->comboBoxTXAtt0->currentIndex() * 0.5;
    double att1 = ui->comboBoxTXAtt1->currentIndex() * 0.5;
    double att2 = ui->comboBoxTXAtt2->currentIndex() * 0.5;
    double att3 = ui->comboBoxTXAtt3->currentIndex() * 0.5;
    io_mode_t mode = io_mode_t(ui->comboBoxIOMode->currentIndex());

    on_lineEditTXFreq_returnPressed();
    SP1401R1C()->set_tx_att(att0,att1,att2,att3);
    SP1401R1C()->set_io_mode(mode);
    RD_CATCH_ALL_EXCEPTION
}

void QRFR1CDlg::on_lineEditRXFreq_returnPressed()
{
    PTR_CHECK
    RD_TRY
    uint64_t freq = freq_string_to_uint64_t(ui->lineEditRXFreq->text().toStdString());
    SP1401R1C()->set_rx_freq(freq);
    RD_CATCH_ALL_EXCEPTION
}

void QRFR1CDlg::on_comboBoxRXLNAAttSw_currentIndexChanged(int index)
{
    PTR_CHECK
    RD_TRY
    SP1401R1C()->set_rx_lna_att_sw(rx_lna_att_t(index));
    RD_CATCH_ALL_EXCEPTION
}

void QRFR1CDlg::on_comboBoxRXAtt019Sw_currentIndexChanged(int index)
{
    PTR_CHECK
    RD_TRY
    SP1401R1C()->set_rx_att_019_sw(rx_att_019_t(index));
    RD_CATCH_ALL_EXCEPTION
}

void QRFR1CDlg::on_comboBoxRXAtt1_currentIndexChanged(int index)
{
    PTR_CHECK
    RD_TRY
    SP1401R1C()->set_rx_att1(index * 0.5);
    RD_CATCH_ALL_EXCEPTION
}

void QRFR1CDlg::on_comboBoxRXAtt2_currentIndexChanged(int index)
{
    PTR_CHECK
    RD_TRY
    SP1401R1C()->set_rx_att2(index * 0.5);
    RD_CATCH_ALL_EXCEPTION
}

void QRFR1CDlg::on_comboBoxIFFilterSw_currentIndexChanged(int index)
{
    PTR_CHECK
    RD_TRY
    SP1401R1C()->set_rx_if_filter_sw(basic_sp1401::rx_if_filter_t(index));
    RD_CATCH_ALL_EXCEPTION
}

void QRFR1CDlg::on_comboBoxRXAtt3_currentIndexChanged(int index)
{
    PTR_CHECK
    RD_TRY
    SP1401R1C()->set_rx_att3(index * 0.5);
    RD_CATCH_ALL_EXCEPTION
}

void QRFR1CDlg::on_pushButtonSetRXAll_clicked()
{
    PTR_CHECK
    RD_TRY
    rx_lna_att_t lnaAtt = rx_lna_att_t(ui->comboBoxRXLNAAttSw->currentIndex());
    rx_att_019_t att019 = rx_att_019_t(ui->comboBoxRXAtt019Sw->currentIndex());
    double att1 = ui->comboBoxRXAtt1->currentIndex() * 0.5;
    double att2 = ui->comboBoxRXAtt2->currentIndex() * 0.5;
    basic_sp1401::rx_if_filter_t filter =
            basic_sp1401::rx_if_filter_t(ui->comboBoxIFFilterSw->currentIndex());
    double att3 = ui->comboBoxRXAtt3->currentIndex() * 0.5;

    on_lineEditRXFreq_returnPressed();
    SP1401R1C()->set_rx_att(att1,att2,att3);
    SP1401R1C()->set_rx_lna_att_sw(lnaAtt);
    SP1401R1C()->set_rx_att_019_sw(att019);
    SP1401R1C()->set_rx_if_filter_sw(filter);
    RD_CATCH_ALL_EXCEPTION
}

void QRFR1CDlg::on_pushButtonGetTempAll_clicked()
{
    PTR_CHECK
    RD_TRY
    double temp[8] = {0.0};

    for (quint8 i = 0;i < 8;i ++) {
        SP1401R1C()->get_temp(i,temp[i]);
    }

    ui->lineEditTemp0->setText(QString("%1").arg(temp[0]));
    ui->lineEditTemp1->setText(QString("%1").arg(temp[1]));
    ui->lineEditTemp2->setText(QString("%1").arg(temp[2]));
    ui->lineEditTemp3->setText(QString("%1").arg(temp[3]));
    ui->lineEditTemp4->setText(QString("%1").arg(temp[4]));
    ui->lineEditTemp5->setText(QString("%1").arg(temp[5]));
    ui->lineEditTemp6->setText(QString("%1").arg(temp[6]));
    ui->lineEditTemp7->setText(QString("%1").arg(temp[7]));
    RD_CATCH_ALL_EXCEPTION
}

void QRFR1CDlg::on_pushButtonGetDetAll_clicked()
{
    PTR_CHECK
    RD_TRY
    quint32 det[4] = {0};

    SP1401R1C()->get_ad7680_tx_if2(det[0]);
    SP1401R1C()->get_ad7680_rx_comb(det[1]);
    SP1401R1C()->get_ad7680_rx_if1(det[2]);
    SP1401R1C()->get_ad7680_rx_if2(det[3]);

    ui->lineEditDetTXIF2->setText(QString("%1").arg(det[0]));
    ui->lineEditDetRXComb->setText(QString("%1").arg(det[1]));
    ui->lineEditDetRXIF1->setText(QString("%1").arg(det[2]));
    ui->lineEditDetRXIF2->setText(QString("%1").arg(det[3]));
    RD_CATCH_ALL_EXCEPTION
}

void QRFR1CDlg::on_pushButtonSetSN_clicked()
{
    PTR_CHECK
    RD_TRY
    quint32 ordinal = ui->lineEditOrdinal->text().toUInt();
    hw_ver_t ver = hw_ver_t(ui->comboBoxHwVer->currentIndex() + R1C);
    SP1401R1C()->set_sn_major(ver,ordinal);
    RD_CATCH_ALL_EXCEPTION
}

void QRFR1CDlg::on_pushButtonGetSN_clicked()
{
    PTR_CHECK
    RD_TRY
    char sn[32] = {0};
    memset(sn,0,sizeof(sn));
    SP1401R1C()->get_sn_major(sn);
    ui->lineEditSN->setText(QString("%1").arg(sn));
    RD_CATCH_ALL_EXCEPTION
}
