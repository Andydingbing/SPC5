#include "sp9500pro_sp1403_r1b_widget.h"
#include "ui_sp9500pro_sp1403_r1b.h"
#include "lmx2594_dlg.h"
#include "reg_def_sp9500x.h"

using namespace rd;
using namespace rd::ns_sp1403;
using namespace rd::ns_sp1403::r1b;

Q_SP9500PRO_SP1403_R1B_Widget::Q_SP9500PRO_SP1403_R1B_Widget(QWidget *parent) :
    Q_RD_Widget(parent),
    ui(new Ui::Q_SP9500PRO_SP1403_R1B_Widget)
{
    ui->setupUi(this);

    dlg_TX_LMX2594_0 = nullptr;
    dlg_TX_LMX2594_1 = nullptr;
    dlg_RX_LMX2594_0 = nullptr;

    QString att;
    for (quint8 i = 0;i < SERIE_SIZE(0,31.75,0.25);i ++) {
        att = QString("%1").arg(i * 0.25);
        ui->comboBoxTX0Att0->addItem(att);
        ui->comboBoxTX0Att1->addItem(att);
        ui->comboBoxTX0Att2->addItem(att);
        ui->comboBoxTX0Att3->addItem(att);
        ui->comboBoxTX1Att0->addItem(att);
        ui->comboBoxTX1Att1->addItem(att);
        ui->comboBoxTX1Att2->addItem(att);
        ui->comboBoxTX1Att3->addItem(att);
        ui->comboBoxRXAtt0->addItem(att);
        ui->comboBoxRXAtt1->addItem(att);
    }

    for (size_t i = 0;i < io_mode::_size();++i) {
        ui->comboBoxTX0IOMode->addItem(QString::fromUtf8(io_mode::_names()[i]));
        ui->comboBoxTX1IOMode->addItem(QString::fromUtf8(io_mode::_names()[i]));
    }

    for (size_t i = 0;i < led_t::_size();++i) {
        ui->comboBoxTX0LED->addItem(QString::fromUtf8(led_t::_names()[i]));
        ui->comboBoxTX1LED->addItem(QString::fromUtf8(led_t::_names()[i]));
    }

    for (size_t i = 0;i < sp1403_r1b::tx_sw1_t::_size();++i) {
        ui->comboBoxTX0Sw1->addItem(QString::fromUtf8(sp1403_r1b::tx_sw1_t::_names()[i]));
        ui->comboBoxTX1Sw1->addItem(QString::fromUtf8(sp1403_r1b::tx_sw1_t::_names()[i]));
    }

    for (size_t i = 0;i < sp1403_r1b::tx_sw2_t::_size();++i) {
        ui->comboBoxTX0Sw2->addItem(QString::fromUtf8(sp1403_r1b::tx_sw2_t::_names()[i]));
        ui->comboBoxTX1Sw2->addItem(QString::fromUtf8(sp1403_r1b::tx_sw2_t::_names()[i]));
    }

    for (size_t i = 0;i < sp1403_r1b::tx_sw3_t::_size();++i) {
        ui->comboBoxTX0Sw3->addItem(QString::fromUtf8(sp1403_r1b::tx_sw3_t::_names()[i]));
        ui->comboBoxTX1Sw3->addItem(QString::fromUtf8(sp1403_r1b::tx_sw3_t::_names()[i]));
    }

    for (size_t i = 0;i < sp1403_r1b::tx_sw4_t::_size();++i) {
        ui->comboBoxTX0Sw4->addItem(QString::fromUtf8(sp1403_r1b::tx_sw4_t::_names()[i]));
        ui->comboBoxTX1Sw4->addItem(QString::fromUtf8(sp1403_r1b::tx_sw4_t::_names()[i]));
    }

    for (size_t i = 0;i < sp1403_r1b::tx0_sw5_t::_size();++i) {
        ui->comboBoxTX0Sw5->addItem(QString::fromUtf8(sp1403_r1b::tx0_sw5_t::_names()[i]));
    }

    for (size_t i = 0;i < sp1403_r1b::tx1_sw5_t::_size();++i) {
        ui->comboBoxTX1Sw5->addItem(QString::fromUtf8(sp1403_r1b::tx0_sw5_t::_names()[i]));
    }

    for (size_t i = 0;i < sp1403_r1b::tx0_sw6_t::_size();++i) {
        ui->comboBoxTX0Sw6->addItem(QString::fromUtf8(sp1403_r1b::tx0_sw6_t::_names()[i]));
    }

    for (size_t i = 0;i < sp1403_r1b::tx1_sw6_t::_size();++i) {
        ui->comboBoxTX1Sw6->addItem(QString::fromUtf8(sp1403_r1b::tx1_sw6_t::_names()[i]));
    }

    for (size_t i = 0;i < sp1403_r1b::tx0_sw7_t::_size();++i) {
        ui->comboBoxTX0Sw7->addItem(QString::fromUtf8(sp1403_r1b::tx0_sw7_t::_names()[i]));
    }

    for (size_t i = 0;i < sp1403_r1b::tx0_sw8_t::_size();++i) {
        ui->comboBoxTX0Sw8->addItem(QString::fromUtf8(sp1403_r1b::tx0_sw8_t::_names()[i]));
    }

    for (size_t i = 0;i < sp1403_r1b::tx0_sw9_t::_size();++i) {
        ui->comboBoxTX0Sw9->addItem(QString::fromUtf8(sp1403_r1b::tx0_sw9_t::_names()[i]));
    }

    for (size_t i = 0;i < sp1403_r1b::tx0_sw10_t::_size();++i) {
        ui->comboBoxTX0Sw10->addItem(QString::fromUtf8(sp1403_r1b::tx0_sw10_t::_names()[i]));
    }

    for (size_t i = 0;i < sp1403_r1b::tx0_sw11_t::_size();++i) {
        ui->comboBoxTX0Sw11->addItem(QString::fromUtf8(sp1403_r1b::tx0_sw11_t::_names()[i]));
    }

    for (size_t i = 0;i < sp1403_r1b::tx0_sw12_t::_size();++i) {
        ui->comboBoxTX0Sw12->addItem(QString::fromUtf8(sp1403_r1b::tx0_sw12_t::_names()[i]));
    }

    for (size_t i = 0;i < sp1403_r1b::rx_lna_att_t::_size();++i) {
        ui->comboBoxRXLNAAttSw->addItem(QString::fromUtf8(sp1403_r1b::rx_lna_att_t::_names()[i]));
    }

    for (size_t i = 0;i < rx_bw_t::_size();i ++) {
        ui->comboBoxBWSw->addItem(QString::fromUtf8(rx_bw_t::_names()[i]));
    }

    for (size_t i = 0;i < sp1403_r1b::rx_sw1_t::_size();++i) {
        ui->comboBoxRXSw1->addItem(QString::fromUtf8(sp1403_r1b::rx_sw1_t::_names()[i]));
    }

    for (size_t i = 0;i < sp1403_r1b::rx_sw2_t::_size();++i) {
        ui->comboBoxRXSw1->addItem(QString::fromUtf8(sp1403_r1b::rx_sw2_t::_names()[i]));
    }

    for (size_t i = 0;i < sp1403_r1b::rx_sw3_t::_size();++i) {
        ui->comboBoxRXSw1->addItem(QString::fromUtf8(sp1403_r1b::rx_sw3_t::_names()[i]));
    }

    for (size_t i = 0;i < sp1403_r1b::rx_sw7_t::_size();++i) {
        ui->comboBoxRXSw1->addItem(QString::fromUtf8(sp1403_r1b::rx_sw7_t::_names()[i]));
    }

    ui->lineEditS6_Addr->setText("0x0");
}

Q_SP9500PRO_SP1403_R1B_Widget::~Q_SP9500PRO_SP1403_R1B_Widget()
{
    delete ui;
    SAFE_DEL(dlg_TX_LMX2594_0);
    SAFE_DEL(dlg_TX_LMX2594_1);
    SAFE_DEL(dlg_RX_LMX2594_0);
}

void Q_SP9500PRO_SP1403_R1B_Widget::init()
{
    RD_TRY
    ui->lineEditTXFreq->setText(QString::fromStdString(SP1403_R1B->tx_freq_string()));
    ui->lineEditRXFreq->setText(QString::fromStdString(SP1403_R1B->rx_freq_string()));
    RD_LOG_ALL_EXCEPTION

    updateIOMode();
    updateAtt();
    updateTXSw();
    updateTXLO();
    updateRXSw();
    updateRXLO();
}

void Q_SP9500PRO_SP1403_R1B_Widget::updateIOMode()
{
    led_t ledTX0 = led_t::Red;
    led_t ledTX1 = led_t::Red;
    INT_CHECKV(SP1403_R1B->get_led(port_t::TX0,ledTX0));
    INT_CHECKV(SP1403_R1B->get_led(port_t::TX1,ledTX1));
    ui->comboBoxTX0IOMode->setCurrentIndex(SP1403_R1B->io_mode(TX0));
    ui->comboBoxTX1IOMode->setCurrentIndex(SP1403_R1B->io_mode(TX1));
    ui->comboBoxTX0LED->setCurrentIndex(ledTX0);
    ui->comboBoxTX1LED->setCurrentIndex(ledTX1);
}

void Q_SP9500PRO_SP1403_R1B_Widget::updateAtt()
{
    double att = 0.0;
    INT_CHECKV(SP1403_R1B->get_att(sp1403_r1b::TX0_ATT0,att));
    ui->comboBoxTX0Att0->setCurrentIndex(int(att * 4));

    INT_CHECKV(SP1403_R1B->get_att(sp1403_r1b::TX0_ATT1,att));
    ui->comboBoxTX0Att1->setCurrentIndex(int(att * 4));

    INT_CHECKV(SP1403_R1B->get_att(sp1403_r1b::TX0_ATT2,att));
    ui->comboBoxTX0Att2->setCurrentIndex(int(att * 4));

    INT_CHECKV(SP1403_R1B->get_att(sp1403_r1b::TX0_ATT3,att));
    ui->comboBoxTX0Att3->setCurrentIndex(int(att * 4));

    INT_CHECKV(SP1403_R1B->get_att(sp1403_r1b::TX1_ATT0,att));
    ui->comboBoxTX1Att0->setCurrentIndex(int(att * 4));

    INT_CHECKV(SP1403_R1B->get_att(sp1403_r1b::TX1_ATT1,att));
    ui->comboBoxTX1Att1->setCurrentIndex(int(att * 4));

    INT_CHECKV(SP1403_R1B->get_att(sp1403_r1b::TX1_ATT2,att));
    ui->comboBoxTX1Att2->setCurrentIndex(int(att * 4));

    INT_CHECKV(SP1403_R1B->get_att(sp1403_r1b::TX1_ATT3,att));
    ui->comboBoxTX1Att3->setCurrentIndex(int(att * 4));

    INT_CHECKV(SP1403_R1B->get_att(sp1403_r1b::RX_ATT0,att));
    ui->comboBoxRXAtt0->setCurrentIndex(int(att * 4));

    INT_CHECKV(SP1403_R1B->get_att(sp1403_r1b::RX_ATT1,att));
    ui->comboBoxRXAtt1->setCurrentIndex(int(att * 4));
}

void Q_SP9500PRO_SP1403_R1B_Widget::updateTXSw()
{
    SP1403_S6_REG_DECL(0x1);
    SP1403_S6_REG_DECL(0x2);
    INT_CHECKV(SP1403_R1B->get_s6_reg(0x1,SP1403_S6_REG_DATA(0x1)));
    INT_CHECKV(SP1403_R1B->get_s6_reg(0x2,SP1403_S6_REG_DATA(0x2)));

    ui->comboBoxTX0Sw1->setCurrentIndex(SP1403_S6_REG(0x1).tx0_sw1);
    ui->comboBoxTX0Sw2->setCurrentIndex(SP1403_S6_REG(0x1).tx0_sw2);
    ui->comboBoxTX0Sw3->setCurrentIndex(SP1403_S6_REG(0x1).tx0_sw3);
    ui->comboBoxTX0Sw4->setCurrentIndex(SP1403_S6_REG(0x1).tx0_sw4);
    ui->comboBoxTX0Sw5->setCurrentIndex(SP1403_S6_REG(0x1).tx0_sw5);
    ui->comboBoxTX0Sw6->setCurrentIndex(SP1403_S6_REG(0x1).tx0_sw6);
    ui->comboBoxTX0Sw7->setCurrentIndex(SP1403_S6_REG(0x1).tx0_sw7);
    ui->comboBoxTX0Sw8->setCurrentIndex(SP1403_S6_REG(0x1).tx0_sw8);
    ui->comboBoxTX0Sw9->setCurrentIndex(SP1403_S6_REG(0x1).tx0_sw9);
    ui->comboBoxTX0Sw10->setCurrentIndex(SP1403_S6_REG(0x1).tx0_sw10);
    ui->comboBoxTX0Sw11->setCurrentIndex(SP1403_S6_REG(0x1).tx0_sw11);
    ui->comboBoxTX0Sw12->setCurrentIndex(SP1403_S6_REG(0x1).tx0_sw12);

    ui->comboBoxTX1Sw1->setCurrentIndex(SP1403_S6_REG(0x2).tx1_sw1);
    ui->comboBoxTX1Sw2->setCurrentIndex(SP1403_S6_REG(0x2).tx1_sw2);
    ui->comboBoxTX1Sw3->setCurrentIndex(SP1403_S6_REG(0x2).tx1_sw3);
    ui->comboBoxTX1Sw4->setCurrentIndex(SP1403_S6_REG(0x2).tx1_sw4);
    ui->comboBoxTX1Sw5->setCurrentIndex(SP1403_S6_REG(0x2).tx1_sw5);
    ui->comboBoxTX1Sw6->setCurrentIndex(SP1403_S6_REG(0x2).tx1_sw6);
}

void Q_SP9500PRO_SP1403_R1B_Widget::updateTXLO()
{
    RD_TRY
    ui->lineEditTXLMX2594_0_Freq->setText(QString::fromStdString(SP1403_R1B->tx_lmx2594_0_freq_string()));
    ui->lineEditTXLMX2594_1_Freq->setText(QString::fromStdString(SP1403_R1B->tx_lmx2594_1_freq_string()));
    RD_LOG_ALL_EXCEPTION
}

void Q_SP9500PRO_SP1403_R1B_Widget::updateRXSw()
{
    sp1403_r1b::rx_lna_att_t RX_LNAAttSw = sp1403_r1b::rx_lna_att_t::RX_ATT_0;
    rx_bw_t RX_BW = rx_bw_t::_800M;
    sp1403_r1b::rx_sw1_t RX_Sw1 = sp1403_r1b::rx_sw1_t::TO_ATT;
    sp1403_r1b::rx_sw2_t RX_Sw2 = sp1403_r1b::rx_sw2_t::TO_ATT_0;
    sp1403_r1b::rx_sw3_t RX_Sw3 = sp1403_r1b::rx_sw3_t::_300_1000;
    sp1403_r1b::rx_sw7_t RX_Sw7 = sp1403_r1b::rx_sw7_t::_300_1000;

    INT_CHECKV(SP1403_R1B->get_rx_lna_att_sw(RX_LNAAttSw));
    INT_CHECKV(SP1403_R1B->get_rx_bw(RX_BW));
    INT_CHECKV(SP1403_R1B->get_rx_sw1(RX_Sw1));
    INT_CHECKV(SP1403_R1B->get_rx_sw2(RX_Sw2));
    INT_CHECKV(SP1403_R1B->get_rx_sw3(RX_Sw3));
    INT_CHECKV(SP1403_R1B->get_rx_sw7(RX_Sw7));

    ui->comboBoxRXLNAAttSw->setCurrentIndex(RX_LNAAttSw._to_integral());
    ui->comboBoxBWSw->setCurrentIndex(RX_BW._to_integral());
    ui->comboBoxRXSw1->setCurrentIndex(RX_Sw1._to_index());
    ui->comboBoxRXSw2->setCurrentIndex(RX_Sw2._to_index());
    ui->comboBoxRXSw3->setCurrentIndex(RX_Sw3._to_index());
    ui->comboBoxRXSw7->setCurrentIndex(RX_Sw7._to_index());
}

void Q_SP9500PRO_SP1403_R1B_Widget::updateRXLO()
{
    RD_TRY
    ui->lineEditRXLMX2594_0_Freq->setText(QString::fromStdString(SP1403_R1B->rx_lmx2594_0_freq_string()));
    RD_LOG_ALL_EXCEPTION
}

void Q_SP9500PRO_SP1403_R1B_Widget::on_lineEditTXFreq_textEdited(const QString &arg1)
{
    RD_TRY
    SP1403_R1B->set_tx_freq(freq_string_to<quint64>(arg1.toStdString()));
    RD_LOG_ALL_EXCEPTION

    updateTXSw();
    updateTXLO();
}

void Q_SP9500PRO_SP1403_R1B_Widget::on_lineEditTXLMX2594_0_Freq_textEdited(const QString &arg1)
{
    RD_TRY
    SP1403_R1B->set_lo(sp1403::lo_t::TX_LMX2594_0,freq_string_to<quint64>(arg1.toStdString()));
    RD_LOG_ALL_EXCEPTION
}

void Q_SP9500PRO_SP1403_R1B_Widget::on_pushButtonTXLMX2594_0_Adv_clicked()
{
    if (dlg_TX_LMX2594_0 == nullptr) {
        dlg_TX_LMX2594_0 = new Q_LMX2594_Dlg();
        dlg_TX_LMX2594_0->RF = SP1403_R1B;
        dlg_TX_LMX2594_0->LO = sp1403::lo_t::TX_LMX2594_0;
        dlg_TX_LMX2594_0->setWindowTitle(QString(""));
    }
    dlg_TX_LMX2594_0->setVisible(true);
    dlg_TX_LMX2594_0->showNormal();
    dlg_TX_LMX2594_0->activateWindow();
}

void Q_SP9500PRO_SP1403_R1B_Widget::on_lineEditTXLMX2594_1_Freq_textEdited(const QString &arg1)
{
    RD_TRY
    SP1403_R1B->set_lo(sp1403::lo_t::TX_LMX2594_1,freq_string_to<quint64>(arg1.toStdString()));
    RD_LOG_ALL_EXCEPTION
}

void Q_SP9500PRO_SP1403_R1B_Widget::on_pushButtonTXLMX2594_1_Adv_clicked()
{
    if (dlg_TX_LMX2594_1 == nullptr) {
        dlg_TX_LMX2594_1 = new Q_LMX2594_Dlg();
        dlg_TX_LMX2594_1->RF = SP1403_R1B;
        dlg_TX_LMX2594_1->LO = sp1403::lo_t::TX_LMX2594_1;
        dlg_TX_LMX2594_1->setWindowTitle(QString(""));
    }
    dlg_TX_LMX2594_1->setVisible(true);
    dlg_TX_LMX2594_1->showNormal();
    dlg_TX_LMX2594_1->activateWindow();
}

void Q_SP9500PRO_SP1403_R1B_Widget::on_lineEditRXFreq_textEdited(const QString &arg1)
{
    RD_TRY
    SP1403_R1B->set_rx_freq(freq_string_to<quint64>(arg1.toStdString()));
    RD_LOG_ALL_EXCEPTION

    updateRXSw();
    updateRXLO();
}

void Q_SP9500PRO_SP1403_R1B_Widget::on_comboBoxRXLNAAttSw_activated(int index)
{
    SP1403_R1B->set_rx_lna_att_sw(sp1403_r1b::rx_lna_att_t::_from_index(index));
}

void Q_SP9500PRO_SP1403_R1B_Widget::on_comboBoxRXAtt0_activated(int index)
{
    SP1403_R1B->set_att(sp1403_r1b::RX_ATT0,index / 4.0);
}

void Q_SP9500PRO_SP1403_R1B_Widget::on_comboBoxRXAtt1_activated(int index)
{
    SP1403_R1B->set_att(sp1403_r1b::RX_ATT1,index / 4.0);
}

void Q_SP9500PRO_SP1403_R1B_Widget::on_comboBoxBWSw_activated(int index)
{
    SP1403_R1B->set_rx_bw(rx_bw_t::_from_integral(index));
}

void Q_SP9500PRO_SP1403_R1B_Widget::on_lineEditRXLMX2594_0_Freq_textEdited(const QString &arg1)
{
    RD_TRY
    SP1403_R1B->set_lo(sp1403::lo_t::RX_LMX2594_0,freq_string_to<quint64>(arg1.toStdString()));
    RD_LOG_ALL_EXCEPTION
}

void Q_SP9500PRO_SP1403_R1B_Widget::on_pushButtonRXLMX2594_0_Adv_clicked()
{
    if (dlg_RX_LMX2594_0 == nullptr) {
        dlg_RX_LMX2594_0 = new Q_LMX2594_Dlg();
        dlg_RX_LMX2594_0->RF = SP1403_R1B;
        dlg_RX_LMX2594_0->LO = sp1403::lo_t::RX_LMX2594_0;
        dlg_RX_LMX2594_0->setWindowTitle(QString(""));
    }
    dlg_RX_LMX2594_0->setVisible(true);
    dlg_RX_LMX2594_0->showNormal();
    dlg_RX_LMX2594_0->activateWindow();
}

void Q_SP9500PRO_SP1403_R1B_Widget::on_pushButtonSetSN_clicked()
{
//    RF_DT3308_PTR_CHECK
//    quint32 ordinal = ui->lineEditOrdinal->text().toUInt();
//    hw_ver_t ver = hw_ver_t(ui->comboBoxHwVer->currentIndex());
//    INT_CHECKV(SP1401R1A()->set_sn_major(ver,ordinal));
}

void Q_SP9500PRO_SP1403_R1B_Widget::on_pushButtonGetSN_clicked()
{
//    RF_DT3308_PTR_CHECK
//    char sn[32] = {0};
//    memset(sn,0,sizeof(sn));
//    INT_CHECKV(SP1401R1A()->get_sn_major(sn));
//    ui->lineEditSN->setText(QString("%1").arg(sn));
}

void Q_SP9500PRO_SP1403_R1B_Widget::on_pushButtonGetTemp0_clicked()
{
    double tempPA0 = 0.0;
    double tempPA1 = 0.0;
    double tempLNA = 0.0;

    SP1403_R1B->get_temp(sp1403_r1b::temp_t::TX0_PA,tempPA0);
    SP1403_R1B->get_temp(sp1403_r1b::temp_t::TX1_PA,tempPA1);
    SP1403_R1B->get_temp(sp1403_r1b::temp_t::RX_LNA,tempLNA);
    ui->lineEditTempPA0->setText(QString("%1").arg(tempPA0));
    ui->lineEditTempPA1->setText(QString("%1").arg(tempPA1));
    ui->lineEditTempLNA->setText(QString("%1").arg(tempLNA));
}

void Q_SP9500PRO_SP1403_R1B_Widget::on_pushButtonGetTemp1_clicked()
{
    double tempLO = 0.0;
    double tempSw = 0.0;

    SP1403_R1B->get_temp(sp1403_r1b::temp_t::TX_LO1,tempLO);
    SP1403_R1B->get_temp(sp1403_r1b::temp_t::TX0_SW,tempSw);
    ui->lineEditTempLO->setText(QString("%1").arg(tempLO));
    ui->lineEditTempSw->setText(QString("%1").arg(tempSw));
}

void Q_SP9500PRO_SP1403_R1B_Widget::on_pushButtonS6_Write_clicked()
{
    quint8 addr = quint8(ui->lineEditS6_Addr->text().toUShort(nullptr,16));
    quint16 data = ui->lineEditS6_Data->text().toUShort(nullptr,16);
    SP1403_R1B->set_s6_reg(addr,data);
}

void Q_SP9500PRO_SP1403_R1B_Widget::on_pushButtonS6_Read_clicked()
{
    quint16 data = 0;
    quint8 addr = quint8(ui->lineEditS6_Addr->text().toUShort(nullptr,16));
    SP1403_R1B->get_s6_reg(addr,data);
    ui->lineEditS6_Data->setText(DecimalToHexString(data));
}
