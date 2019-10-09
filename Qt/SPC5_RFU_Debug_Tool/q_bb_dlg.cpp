#include "q_bb_dlg.h"
#include "ui_q_bb_dlg.h"
#include "algorithm.h"
#include "algo_math.h"

QBbDlg::QBbDlg(QWidget *parent) :
    QRFDlg(parent),
    ui(new Ui::QBbDlg)
{
    ui->setupUi(this);

    ui->lineEditDAClock->setText("491.52");
    ui->comboBoxDASw->setCurrentIndex(sp2401_r1a::FROM_TO_RF);
    ui->comboBoxDDSSrc->setCurrentIndex(sp2401_r1a::SINGLE_TONE);
    ui->lineEditDDS1->setText("20");
    ui->lineEditDDS2->setText("0");
    ui->lineEditTxPowerComp->setText("0");
    ui->lineEditDUC->setText("0");
    ui->lineEditIQPhaseI->setText("0");
    ui->lineEditIQPhaseQ->setText("0");
    ui->lineEditIQAmpI->setText("8192");
    ui->lineEditIQAmpQ->setText("8192");
    ui->lineEditDCOffsetI->setText("0");
    ui->lineEditDCOffsetQ->setText("0");

    ui->lineEditADClock->setText("368.64");
    ui->comboBoxADSw->setCurrentIndex(sp2401_r1a::FROM_TO_RF);
    ui->lineEditDDC->setText("-92.64M");
    ui->lineEditManualPower->setText("0");
    ui->lineEditADManual->setText("0");
    ui->lineEditADFPGA->setText("0");
    ui->lineEditRxPowerComp->setText("0");

    QString strRxCoefReal = "";
    QString strRxCoefImag = "";
    double rxCoefReal[RX_FILTER_ORDER] = {0.0};
    double rxCoefImag[RX_FILTER_ORDER] = {0.0};
    sp2401_r1a::rx_filter_coef_default(rxCoefReal,rxCoefImag,RX_FILTER_ORDER);
    for (qint32 i = 0;i < RX_FILTER_ORDER - 1;i ++) {
        strRxCoefReal.append(QString("%1,").arg(rxCoefReal[i]));
        strRxCoefImag.append(QString("%1,").arg(rxCoefImag[i]));
    }
    strRxCoefReal.append(QString("%1").arg(rxCoefReal[RX_FILTER_ORDER - 1]));
    strRxCoefImag.append(QString("%1").arg(rxCoefImag[RX_FILTER_ORDER - 1]));
    ui->textEditRxFilterReal->setText(strRxCoefReal);
    ui->textEditRxFilterImag->setText(strRxCoefImag);
}

QBbDlg::~QBbDlg()
{
    delete ui;
}

void QBbDlg::on_pushButtonSetDAClock_clicked()
{
    PTR_CHECK
    double freq = ui->lineEditDAClock->text().toDouble();
    INT_CHECKV(SP2401R1A()->set_da_freq_MHz(freq));
}

void QBbDlg::on_pushButtonSetDASw_clicked()
{
    PTR_CHECK
    sp2401_r1a::ad_da_port_t port = sp2401_r1a::ad_da_port_t(ui->comboBoxDASw->currentIndex());
    INT_CHECKV(SP2401R1A()->set_da_sw(port));
}

void QBbDlg::on_pushButtonResetDA_clicked()
{
    PTR_CHECK
    INT_CHECKV(SP2401R1A()->da_reset());
    on_pushButtonSetDAClock_clicked();
}

void QBbDlg::on_pushButtonSetDDSSrc_clicked()
{
    PTR_CHECK
    sp2401_r1a::da_src_t src = sp2401_r1a::da_src_t(ui->comboBoxDDSSrc->currentIndex());
    INT_CHECKV(SP2401R1A()->set_dds_src(src));
}

void QBbDlg::on_pushButtonSetDDS1_clicked()
{
    PTR_CHECK
//    double freq = 0;
//    str2freq(ui->lineEditDDS1->text().toStdString().c_str(),freq);
    INT_CHECKV(SP2401R1A()->set_dds1(ui->lineEditDDS1->text().toDouble()*1000000));
}

void QBbDlg::on_pushButtonSetDDS2_clicked()
{
    PTR_CHECK
    double freq = freq_string_to_double(ui->lineEditDDS2->text().toStdString());
    INT_CHECKV(SP2401R1A()->set_dds2(freq));
}

void QBbDlg::on_pushButtonSetTxPowerComp_clicked()
{
    PTR_CHECK
    INT_CHECKV(SP2401R1A()->set_tx_pwr_comp(ui->lineEditTxPowerComp->text().toDouble()));
}

void QBbDlg::on_pushButtonSetDUC_clicked()
{
    PTR_CHECK
    double freq = freq_string_to_double(ui->lineEditDUC->text().toStdString());
    INT_CHECKV(SP2401R1A()->set_duc_dds(freq));
}

void QBbDlg::on_checkBoxByPassTxFilter_clicked(bool checked)
{
    PTR_CHECK
    INT_CHECKV(SP2401R1A()->set_tx_filter_sw(!checked));
}

void QBbDlg::on_pushButtonSetTxFilter_clicked()
{
    PTR_CHECK
    double coefReal[TX_FILTER_ORDER_2I] = {0.0};
    double coefImag[TX_FILTER_ORDER_2I] = {0.0};
    QString strCoefReal = ui->textEditTxFilterReal->toPlainText();
    QString strCoefImag = ui->textEditTxFilterImag->toPlainText();

    QString2Coef(strCoefReal,coefReal,TX_FILTER_ORDER_2I);
    QString2Coef(strCoefImag,coefImag,TX_FILTER_ORDER_2I);
    INT_CHECKV(SP2401R1A()->set_tx_filter(coefReal,coefImag));
}

void QBbDlg::on_pushButtonSetADClock_clicked()
{
    PTR_CHECK
    double freq = ui->lineEditADClock->text().toDouble();
    INT_CHECKV(SP2401R1A()->set_ad_freq_MHz(freq));
}

void QBbDlg::on_pushButtonSetADSw_clicked()
{
    PTR_CHECK
    sp2401_r1a::ad_da_port_t port = (sp2401_r1a::ad_da_port_t)(ui->comboBoxDASw->currentIndex());
    INT_CHECKV(SP2401R1A()->set_ad_sw(port));
}

void QBbDlg::on_pushButtonSetDDC_clicked()
{
    PTR_CHECK
    double freq = freq_string_to_double(ui->lineEditDDC->text().toStdString());
    INT_CHECKV(SP2401R1A()->set_ddc(freq));
}

void QBbDlg::on_pushButtonGetManualPower_clicked()
{
    on_pushButtonGetADManual_clicked();
}

void QBbDlg::on_pushButtonGetADManual_clicked()
{
    PTR_CHECK
    int64_t ad = 0;
    INT_CHECKV(SP1401()->get_ads5474_manual(ad));
    ui->lineEditADManual->setText(QString("%1").arg(ad));
    ui->lineEditManualPower->setText(QString("%1").arg(round(ad_to_dBc(_0dBFS,ad),3)));
    ui->lineEditRxPowerComp->setText(QString("%1").arg(_0dBFS - ad));
}

void QBbDlg::on_pushButtonGetADFPGA_clicked()
{
    PTR_CHECK
    int64_t ad = 0;
    INT_CHECKV(SP1401()->get_ads5474(ad));
    ui->lineEditADFPGA->setText(QString("%1").arg(ad));
}

void QBbDlg::on_pushButtonSetRxPowerComp_clicked()
{
    PTR_CHECK
    INT_CHECKV(SP2401R1A()->set_rx_pwr_comp(ui->lineEditRxPowerComp->text().toInt()));
}

void QBbDlg::on_pushButtonSetRxFilter_clicked()
{
    PTR_CHECK
    double coefReal[RX_FILTER_ORDER] = {0.0};
    double coefImag[RX_FILTER_ORDER] = {0.0};
    QString strCoefReal = ui->textEditRxFilterReal->toPlainText();
    QString strCoefImag = ui->textEditRxFilterImag->toPlainText();

    QString2Coef(strCoefReal,coefReal,RX_FILTER_ORDER);
    QString2Coef(strCoefImag,coefImag,RX_FILTER_ORDER);
    INT_CHECKV(SP2401R1A()->set_rx_filter(coefReal,coefImag));
}

void QBbDlg::on_lineEditIQPhaseI_textChanged(const QString &arg1)
{
    PTR_CHECK
    INT_CHECKV(SP2401R1A()->set_tx_phase_rotate_I(ui->lineEditIQPhaseI->text().toDouble()));
}

void QBbDlg::on_lineEditIQAmpI_textChanged(const QString &arg1)
{
    PTR_CHECK
    INT_CHECKV(SP2401R1A()->set_tx_amplitude_balance(
                   ui->lineEditIQAmpI->text().toUShort(),
                   ui->lineEditIQAmpQ->text().toUShort())
               );
}

void QBbDlg::on_lineEditIQAmpQ_textChanged(const QString &arg1)
{
    PTR_CHECK
    INT_CHECKV(SP2401R1A()->set_tx_amplitude_balance(
                   ui->lineEditIQAmpI->text().toUShort(),
                   ui->lineEditIQAmpQ->text().toUShort())
               );
}

void QBbDlg::on_lineEditDCOffsetI_textChanged(const QString &arg1)
{
    PTR_CHECK
    INT_CHECKV(SP2401R1A()->set_tx_dc_offset(
                   ui->lineEditDCOffsetI->text().toShort(),
                   ui->lineEditDCOffsetQ->text().toShort())
               );
}

void QBbDlg::on_lineEditDCOffsetQ_textChanged(const QString &arg1)
{
    PTR_CHECK
    INT_CHECKV(SP2401R1A()->set_tx_dc_offset(
                   ui->lineEditDCOffsetI->text().toUShort(),
                   ui->lineEditDCOffsetQ->text().toUShort())
               );
}

void QBbDlg::on_lineEditDUC_textChanged(const QString &arg1)
{
    PTR_CHECK
//    double freq = 0;
//    str2freq(ui->lineEditDUC->text().toStdString().c_str(),freq);

    INT_CHECKV(SP2401R1A()->set_duc_dds( ui->lineEditDUC->text().toDouble()*1000000));
}

void QBbDlg::on_lineEditDDS1_textChanged(const QString &arg1)
{
    PTR_CHECK
//    double freq = 0;
//    str2freq(ui->lineEditDDS1->text().toStdString().c_str(),freq);

    INT_CHECKV(SP2401R1A()->set_dds1(ui->lineEditDDS1->text().toDouble()*1000000));
}
