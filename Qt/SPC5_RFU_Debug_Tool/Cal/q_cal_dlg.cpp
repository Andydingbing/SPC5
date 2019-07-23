#include "q_cal_dlg.h"
#include "q_plot.h"
#include "q_model_tx_loleak.h"
#include "q_model_tx_sb.h"
#include "q_model_tx_pwr.h"
#include "q_model_tx_att.h"
#include "q_model_rx_ref.h"
#include "q_model_rx_att.h"
#include "q_model_rx_filter_offset.h"
#include "algorithm.h"

void QCalR1CTXLOLeakDlg::init()
{
    model->append(new QTXLOLeakModel(reinterpret_cast<QObject *>(this)));
    ui->tableViewData->setModel(model->at(0));
    ui->tableViewData->setColumnWidth(0,80);
    ui->tableViewData->setColumnWidth(1,100);
    ui->tableViewData->setColumnWidth(2,100);
    ui->tableViewData->setColumnWidth(3,80);
    ui->tableViewData->setColumnWidth(4,120);
    ui->tableViewData->setColumnWidth(5,100);
    ui->tableViewData->setColumnWidth(6,100);
    ui->tableViewData->setColumnWidth(7,100);
    ui->tableViewData->setColumnWidth(8,100);
    ui->tableViewData->setColumnWidth(9,120);
    ui->lineEditFreqStar->setText(QString("%1M").arg(RF_TX_FREQ_STAR / 1000000));
    ui->lineEditFreqStop->setText(QString("%1M").arg(RF_TX_FREQ_STOP / 1000000));
    ui->lineEditFreqStep->setText(QString("%1M").arg(RF_TX_FREQ_STEP_CALLED / 1000000));
    ui->lineEditFreq->setText("2G");

    addIdleWidget(ui->lineEditFreqStar);
    addIdleWidget(ui->lineEditFreqStop);
    addIdleWidget(ui->lineEditFreqStep);
    addIdleWidget(ui->pushButtonExport);
    addIdleWidget(ui->checkBoxCalRef);
    addIdleWidget(ui->radioButtonSpec);
    addIdleWidget(ui->radioButtonLoop);
    addIdleWidget(ui->pushButtonStar);
    addIdleWidget(ui->lineEditFreq);
    addIdleWidget(ui->lineEditDC_I);
    addIdleWidget(ui->lineEditDC_Q);
    addIdleWidget(ui->pushButtonGetSet);
    addRunningWidget(ui->pushButtonStop);
    addToPauseWidget(ui->pushButtonPaus);
    addToRunningWidget(ui->pushButtonCont);
    widgetResume();
    QCalBaseDlg::init();
}

void QCalR1CTXLOLeakDlg::resetShowWidget(CalParam *param)
{
    QTXLOLeakModel *model = dynamic_cast<QTXLOLeakModel *>(this->model->at(0));
    model->calTable()->clear();
    tx_lol_table_r1cd::data_f_t data;
    for (qint32 i = 0;i < RF_TX_FREQ_PTS_CALLED;i ++) {
        model->calTable()->append(data);
    }
    emit reset();
    ui->tableViewData->selectRow(0);
}

void QCalR1CTXLOLeakDlg::uiToCalParam(CalParam *param)
{
    param->parent = this;
    param->model_0 = model->at(0);
    param->_sp1401 = SP1401;
    param->_sp2401 = SP2401;
    param->_sp3501 = &SP3501;
    param->rfFreqStar = ui->lineEditFreqStar->text();
    param->rfFreqStop = ui->lineEditFreqStop->text();
    param->rfFreqStep = ui->lineEditFreqStep->text();
    param->calX9119 = ui->checkBoxCalRef->isChecked();
    param->methodLOLeak = ui->radioButtonSpec->isChecked() ? M_Spectrum : M_Loopback;
}

void QCalR1CTXLOLeakDlg::updateFromParam(const CalR1CParam &param)
{
    ui->checkBoxCalRef->setChecked(true);
    ui->radioButtonSpec->setChecked(true);
    ui->radioButtonLoop->setChecked(false);
}

void QCalR1CTXLOLeakDlg::update(const QModelIndex &tl,const QModelIndex &br,cal_file::cal_item_t item,int sec)
{
    ui->tableViewData->selectRow(tl.row());
}

void QCalR1CTXLOLeakDlg::getset()
{
    tx_lol_table_r1cd::data_m_t data;
    quint64 freq = freq_string_to<quint64>(ui->lineEditFreq->text().toStdString());
    ((cal_file_r1cd *)(SP1401->cf()))->m_tx_lol->get(freq,&data);

    ui->lineEditDC_I->setText(QString("%1").arg(data.dc_i));
    ui->lineEditDC_Q->setText(QString("%1").arg(data.dc_q));

    SP1401->set_tx_freq(freq);
    SP2401->set_tx_dc_offset(quint16(data.dc_i),quint16(data.dc_q));
}


void QCalR1CTXSBDlg::init()
{
    model->append(new QTXSBModel(reinterpret_cast<QObject *>(this)));
    ui->tableViewData->setModel(model->at(0));
    ui->tableViewData->setColumnWidth(0,80);
    ui->tableViewData->setColumnWidth(1,100);
    ui->tableViewData->setColumnWidth(2,100);
    ui->tableViewData->setColumnWidth(3,100);
    ui->tableViewData->setColumnWidth(4,80);
    ui->tableViewData->setColumnWidth(5,120);
    ui->tableViewData->setColumnWidth(6,100);
    ui->tableViewData->setColumnWidth(7,100);
    ui->tableViewData->setColumnWidth(8,100);
    ui->tableViewData->setColumnWidth(9,100);
    ui->tableViewData->setColumnWidth(10,120);
    ui->lineEditFreqStar->setText(QString("%1M").arg(RF_TX_FREQ_STAR / 1000000));
    ui->lineEditFreqStop->setText(QString("%1M").arg(RF_TX_FREQ_STOP / 1000000));
    ui->lineEditFreqStep->setText(QString("%1M").arg(RF_TX_FREQ_STEP_CALLED / 1000000));
    ui->lineEditFreq->setText("2G");

    addIdleWidget(ui->lineEditFreqStar);
    addIdleWidget(ui->lineEditFreqStop);
    addIdleWidget(ui->lineEditFreqStep);
    addIdleWidget(ui->pushButtonExport);
    addIdleWidget(ui->radioButtonSpec);
    addIdleWidget(ui->radioButtonLoop);
    addIdleWidget(ui->pushButtonStar);
    addIdleWidget(ui->lineEditFreq);
    addIdleWidget(ui->lineEditTh);
    addIdleWidget(ui->lineEditAm_I);
    addIdleWidget(ui->lineEditAm_Q);
    addIdleWidget(ui->pushButtonGetSet);
    addRunningWidget(ui->pushButtonStop);
    addToPauseWidget(ui->pushButtonPaus);
    addToRunningWidget(ui->pushButtonCont);
    widgetResume();
    QCalBaseDlg::init();
}

void QCalR1CTXSBDlg::resetShowWidget(CalParam *param)
{
    QTXSBModel *model = dynamic_cast<QTXSBModel *>(this->model->at(0));
    model->calTable()->clear();
    tx_sb_table_r1cd::data_f_t data;
    for (int32_t i = 0;i < RF_TX_FREQ_PTS_CALLED;i ++)
        model->calTable()->append(data);
    emit reset();
    ui->tableViewData->selectRow(0);
}

void QCalR1CTXSBDlg::uiToCalParam(CalParam *param)
{
    param->parent = this;
    param->model_0 = model->at(0);
    param->_sp1401 = SP1401;
    param->_sp2401 = SP2401;
    param->rfFreqStar = ui->lineEditFreqStar->text();
    param->rfFreqStop = ui->lineEditFreqStop->text();
    param->rfFreqStep = ui->lineEditFreqStep->text();
    param->methodSB   = ui->radioButtonSpec->isChecked() ? M_Spectrum : M_Loopback;
}

void QCalR1CTXSBDlg::updateFromParam(const CalR1CParam &param)
{
    ui->radioButtonSpec->setChecked(true);
    ui->radioButtonLoop->setChecked(false);
}

void QCalR1CTXSBDlg::update(const QModelIndex &tl,const QModelIndex &br,cal_file::cal_item_t item,int sec)
{
    ui->tableViewData->selectRow(tl.row());
}

void QCalR1CTXSBDlg::getset()
{
    tx_sb_table_r1cd::data_m_t data;
    quint64 freq = freq_string_to<quint64>(ui->lineEditFreq->text().toStdString());
    ((cal_file_r1cd *)(SP1401->cf()))->m_tx_sb->get(freq,&data);

    ui->lineEditTh->setText(QString("%1").arg(data.th));
    ui->lineEditAm_I->setText(QString("%1").arg(data.am_i));
    ui->lineEditAm_Q->setText(QString("%1").arg(data.am_q));

    SP1401->set_tx_freq(freq);
    SP2401->set_tx_phase_rotate_I(double(data.th));
    SP2401->set_tx_amplitude_balance(data.am_i,data.am_q);
}


void QCalR1CTXFilterDlg::init()
{
    QTXFilterModel *model_80  = new QTXFilterModel(reinterpret_cast<QObject *>(this));
    QTXFilterModel *model_160 = new QTXFilterModel(reinterpret_cast<QObject *>(this));
    model->append(model_80);
    model->append(model_160);
    ui->tabWidget->init(model_80,model_160);

    ui->lineEditFreq->setText("2G");
    ui->comboBoxBw->setCurrentIndex(sp1401::_160M);

    addIdleWidget(ui->pushButtonExport);
    addIdleWidget(ui->pushButtonStar);
    addIdleWidget(ui->lineEditFreq);
    addIdleWidget(ui->comboBoxBw);
    addIdleWidget(ui->pushButtonGetSet);
    addRunningWidget(ui->pushButtonStop);
    addToPauseWidget(ui->pushButtonPaus);
    addToRunningWidget(ui->pushButtonCont);
    widgetResume();
    QCalBaseDlg::init();
}

void QCalR1CTXFilterDlg::resetShowWidget(CalParam *param)
{
    ui->tabWidget->resetShowWidget();
    tx_filter_80m_table::data_f_t data_80;
    tx_filter_160m_table::data_f_t data_160;
    for (uint32_t i = 0;i < RF_TX_FREQ_PTS_CALLED;i ++) {
        ui->tabWidget->model_80->calTable()->append(data_80);
        ui->tabWidget->model_160->calTable()->append(data_160);
    }
    emit reset();
    ui->tabWidget->tableView_80->selectRow(0);
    ui->tabWidget->tableView_160->selectRow(0);
}

void QCalR1CTXFilterDlg::uiToCalParam(CalParam *param)
{
    param->parent = this;
    param->cal = ui->checkBoxCal->isChecked();
    param->check = ui->checkBoxCheck->isChecked();
    param->model_0 = ui->tabWidget->model_80;
    param->model_1 = ui->tabWidget->model_160;
    param->plotData_0 = ui->tabWidget->dataRF_FR_0;
    param->plotData_1 = ui->tabWidget->dataRF_FR_1;
    param->plotData_2 = ui->tabWidget->dataIF_FR;
    param->_sp1401 = SP1401;
    param->_sp2401 = SP2401;
    param->justRebuildCoef = ui->checkBoxRebuildCoef->isChecked();
}

void QCalR1CTXFilterDlg::updateFromParam(const CalR1CParam &param)
{
    ui->checkBoxRebuildCoef->setChecked(false);
}

void QCalR1CTXFilterDlg::update(const QModelIndex &tl,const QModelIndex &br,cal_file::cal_item_t item,int sec)
{
    if (cal_file::TX_RF_FR_0 == item) {
        ui->tabWidget->plotRF->replot();
    } else if (cal_file::TX_RF_FR_1 == item) {
        ui->tabWidget->plotRF->replot();
    } else if (cal_file::TX_IF_FR == item) {
        ui->tabWidget->plotIF->replot();
    } else if (cal_file::TX_FILTER_80 == item) {
        ui->tabWidget->tableView_80->selectRow(tl.row());
    } else if (cal_file::TX_FILTER_160 == item) {
        ui->tabWidget->tableView_160->selectRow(tl.row());
    } else {
        return;
    }
}

void QCalR1CTXFilterDlg::getset()
{
    bw_t bw = bw_t(ui->comboBoxBw->currentIndex());
    double real[TX_FILTER_ORDER_2I] = {0.0};
    double imag[TX_FILTER_ORDER_2I] = {0.0};
    ZERO_ARRAY(real);
    ZERO_ARRAY(imag);
    quint64 freq = freq_string_to_uint64_t(ui->lineEditFreq->text().toStdString());

    SP1401->set_tx_freq(freq);
    SP1401->cf()->set_bw(bw);
    SP2401->set_tx_filter_sw(true);

    if (bw == _80M) {
        tx_filter_80m_table::data_m_t data;
        dynamic_cast<cal_file_r1cd *>(SP1401->cf())
                ->m_tx_filter_80m
                ->get(freq,&data);

        data._2double(real,imag);
    }
    if (bw == _160M) {
        tx_filter_160m_table::data_m_t data;
        dynamic_cast<cal_file_r1cd *>(SP1401->cf())
                ->m_tx_filter_160m
                ->get(freq,&data);

        data._2double(real,imag);
    }

    SP2401->set_tx_filter(real,imag);
}


void QCalR1CTXPwrDlg::init()
{
    QTXPwrModel *modelOP = new QTXPwrModel(reinterpret_cast<QObject *>(this));
    QTXPwrModel *modelIO = new QTXPwrModel(reinterpret_cast<QObject *>(this));
    model->append(modelOP);
    model->append(modelIO);

    ui->widgetData->init(modelOP,modelIO);
    ui->lineEditFreqStar->setText(QString("%1M").arg(RF_TX_FREQ_STAR / 1000000));
    ui->lineEditFreqStop->setText(QString("%1M").arg(RF_TX_FREQ_STOP / 1000000));
    ui->lineEditFreqStep->setText(QString("%1M").arg(RF_TX_FREQ_STEP_CALLED / 1000000));
    ui->lineEditFreq->setText("2G");
    ui->lineEditPower->setText("-12");
    ui->comboBoxCalIOMode->setCurrentIndex(CAL_IO_OP);

    addIdleWidget(ui->lineEditFreqStar);
    addIdleWidget(ui->lineEditFreqStop);
    addIdleWidget(ui->comboBoxCalIOMode);
    addIdleWidget(ui->pushButtonExport);
    addIdleWidget(ui->pushButtonStar);
    addIdleWidget(ui->lineEditFreq);
    addIdleWidget(ui->comboBoxIOMode);
    addIdleWidget(ui->lineEditPower);
    addIdleWidget(ui->pushButtonGetSet);
    addRunningWidget(ui->pushButtonStop);
    addToPauseWidget(ui->pushButtonPaus);
    addToRunningWidget(ui->pushButtonCont);
    widgetResume();
    QCalBaseDlg::init();
}

void QCalR1CTXPwrDlg::resetShowWidget(CalParam *param)
{
    ui->widgetData->resetShowWidget(param);
    emit reset();
}

void QCalR1CTXPwrDlg::uiToCalParam(CalParam *param)
{
    param->parent = this;
    param->mode = CalIOMode(ui->comboBoxCalIOMode->currentIndex());
    param->model_0 = model->at(0);
    param->model_1 = model->at(1);
    param->_sp1401 = SP1401;
    param->_sp2401 = SP2401;
    param->rfFreqStar = ui->lineEditFreqStar->text();
    param->rfFreqStop = ui->lineEditFreqStop->text();
    param->rfFreqStep = ui->lineEditFreqStep->text();
}

void QCalR1CTXPwrDlg::updateFromParam(const CalR1CParam &param)
{
    ui->lineEditFreqStar->setText(param.rfTxFreqStar);
    ui->lineEditFreqStop->setText(param.rfTxFreqStop);
    ui->comboBoxCalIOMode->setCurrentIndex(param.txMode);
}

void QCalR1CTXPwrDlg::update(const QModelIndex &tl,const QModelIndex &br,cal_file::cal_item_t item,int sec)
{
    if (cal_file::TX_PWR_OP == item) {
        ui->widgetData->tableViewOP->selectRow(tl.row());
        ui->widgetData->plotDGain->replot();
    }
    else {
        ui->widgetData->tableViewIO->selectRow(tl.row());
        ui->widgetData->plotDGain->replot();
    }
}

void QCalR1CTXPwrDlg::getset()
{
    io_mode_t mode = io_mode_t(ui->comboBoxIOMode->currentIndex());
    bw_t bw = SP1401->get_bw();
    double pwr = ui->lineEditPower->text().toDouble();
    double dGain = 0.0;
    quint64 freq = freq_string_to<quint64>(ui->lineEditFreq->text().toStdString());

    if (mode == OUTPUT) {
        tx_pwr_op_table_r1c::data_m_t dataPwr;
        ((cal_file_r1cd *)(SP1401->cf()))->m_tx_pwr_op->get(SP1401->get_hw_ver(),freq,pwr,&dataPwr);
        dGain = dataPwr.d_gain + ((cal_file_r1cd *)(SP1401->cf()))->m_tx_att_op->get(freq,pwr);
        if (bw == _80M) {
            tx_filter_offset_table::data_m_t dataFilOff;
            ((cal_file_r1cd *)(SP1401->cf()))->m_tx_filter_offset_op_80m->get(freq,&dataFilOff);
            dGain += double(dataFilOff.offset);
        }

        ui->lineEditAtt0->setText(QString("%1").arg(dataPwr.att0 / 2.0));
        ui->lineEditAtt1->setText(QString("%1").arg(dataPwr.att1 / 2.0));
        ui->lineEditAtt2->setText(QString("%1").arg(dataPwr.att2 / 2.0));
        ui->lineEditAtt3->setText(QString("%1").arg(dataPwr.att3 / 2.0));
    }
    if (mode == IO) {
        tx_pwr_io_table_r1c::data_m_t dataPwr;
        ((cal_file_r1cd *)(SP1401->cf()))->m_tx_pwr_io->get(SP1401->get_hw_ver(),freq,pwr,&dataPwr);
        dGain = dataPwr.d_gain + ((cal_file_r1cd *)(SP1401->cf()))->m_tx_att_io->get(freq,pwr);
        if (bw == _80M) {
            tx_filter_offset_table::data_m_t dataFilOff;
            ((cal_file_r1cd *)(SP1401->cf()))->m_tx_filter_offset_op_80m->get(freq,&dataFilOff);
            dGain += double(dataFilOff.offset);
        }

        ui->lineEditAtt0->setText(QString("%1").arg(dataPwr.att0 / 2.0));
        ui->lineEditAtt1->setText(QString("%1").arg(dataPwr.att1 / 2.0));
        ui->lineEditAtt2->setText(QString("%1").arg(dataPwr.att2 / 2.0));
        ui->lineEditAtt3->setText(QString("%1").arg(dataPwr.att3 / 2.0));
    }
    ui->lineEditDGain->setText(QString("%1").arg(dGain));

    SP1401->set_tx_freq(freq);
    SP1401->set_io_mode(mode);
    DYNAMIC_SP1401_R1CE_SP1401(SP1401)->
            set_tx_att(ui->lineEditAtt0->text().toDouble(),
                       ui->lineEditAtt1->text().toDouble(),
                       ui->lineEditAtt2->text().toDouble(),
                       ui->lineEditAtt3->text().toDouble());
    SP2401->set_tx_pwr_comp(dGain);
}


void QCalR1CTXAttDlg::init()
{
    model->append(new QR1CTXAttOPModel(SP1401,reinterpret_cast<QObject *>(this)));
    model->append(new QR1CTXAttIOModel(SP1401,reinterpret_cast<QObject *>(this)));
    ui->tabWidget->init((QR1CTXAttOPModel *)(model->at(0)),(QR1CTXAttIOModel *)(model->at(1)));
    ui->lineEditFreqStar->setText(QString("%1M").arg(RF_TX_FREQ_STAR / 1000000));
    ui->lineEditFreqStop->setText(QString("%1M").arg(RF_TX_FREQ_STOP / 1000000));
    ui->lineEditFreqStep->setText(QString("%1M").arg(RF_TX_FREQ_STEP_CALLED / 1000000));
    ui->lineEditFreq->setText("2G");
    ui->comboBoxCalIOMode->setCurrentIndex(CAL_IO_OP);

    addIdleWidget(ui->lineEditFreqStar);
    addIdleWidget(ui->lineEditFreqStop);
    addIdleWidget(ui->lineEditFreqStep);
    addIdleWidget(ui->pushButtonExport);
    addIdleWidget(ui->pushButtonStar);
    addIdleWidget(ui->pushButtonGetSet);
    addRunningWidget(ui->pushButtonStop);
    addToPauseWidget(ui->pushButtonPaus);
    addToRunningWidget(ui->pushButtonCont);
    widgetResume();
    QCalBaseDlg::init();
}

void QCalR1CTXAttDlg::resetShowWidget(CalParam *param)
{
    ((QR1CTXAttOPModel *)(model->at(0)))->setSP1401(SP1401);
    ((QR1CTXAttIOModel *)(model->at(1)))->setSP1401(SP1401);

    ui->tabWidget->resetShowWidget(param);
    ui->tableViewAttOP->setEnabled(calOP(param->mode));
    ui->tableViewAttIO->setEnabled(calIO(param->mode));

    emit reset();
}

void QCalR1CTXAttDlg::uiToCalParam(CalParam *param)
{
    param->parent = this;
    param->mode = CalIOMode(ui->comboBoxCalIOMode->currentIndex());
    param->model_0 = model->at(0);
    param->model_1 = model->at(1);
    param->_sp1401 = SP1401;
    param->_sp2401 = SP2401;
    param->rfFreqStar = ui->lineEditFreqStar->text();
    param->rfFreqStop = ui->lineEditFreqStop->text();
    param->rfFreqStep = ui->lineEditFreqStep->text();
}

void QCalR1CTXAttDlg::updateFromParam(const CalR1CParam &param)
{
    ui->lineEditFreqStar->setText(param.rfTxFreqStar);
    ui->lineEditFreqStop->setText(param.rfTxFreqStop);
    ui->comboBoxCalIOMode->setCurrentIndex(param.txMode);
}

void QCalR1CTXAttDlg::update(const QModelIndex &tl,const QModelIndex &br,cal_file::cal_item_t item,int sec)
{
    if (cal_file::TX_ATT_OP == item) {
        ui->tabWidget->tableViewOP->selectRow(tl.row());
        ui->tabWidget->plotOP->replot();
    }
    else {
        ui->tabWidget->tableViewIO->selectRow(tl.row());
        ui->tabWidget->plotIO->replot();
    }
}

void QCalR1CTXAttDlg::getset()
{

}


void QCalR1CTXFilterOffsetDlg::init()
{
    model->append(new QTXFilterOffsetModel(reinterpret_cast<QObject *>(this)));
    ui->tabWidget->init((QTXFilterOffsetModel *)(model->at(0)));
    ui->lineEditFreqStar->setText(QString("%1M").arg(RF_TX_FREQ_STAR / 1000000));
    ui->lineEditFreqStop->setText(QString("%1M").arg(RF_TX_FREQ_STOP / 1000000));
    ui->lineEditFreqStep->setText(QString("%1M").arg(RF_TX_FREQ_STEP_CALLED / 1000000));
    ui->comboBoxCalIOMode->setCurrentIndex(CAL_IO_OP);

    addIdleWidget(ui->lineEditFreqStar);
    addIdleWidget(ui->lineEditFreqStop);
    addIdleWidget(ui->comboBoxCalIOMode);
    addIdleWidget(ui->pushButtonExport);
    addIdleWidget(ui->pushButtonStar);
    addRunningWidget(ui->pushButtonStop);
    addToPauseWidget(ui->pushButtonPaus);
    addToRunningWidget(ui->pushButtonCont);
    widgetResume();
    QCalBaseDlg::init();
}

void QCalR1CTXFilterOffsetDlg::resetShowWidget(CalParam *param)
{
    ui->tabWidget->resetShowWidget(param);
    emit reset();
}

void QCalR1CTXFilterOffsetDlg::uiToCalParam(CalParam *param)
{
    param->parent = this;
    param->mode = CalIOMode(ui->comboBoxCalIOMode->currentIndex());
    param->model_0 = model->at(0);
    param->_sp1401 = SP1401;
    param->_sp2401 = SP2401;
    param->rfFreqStar = ui->lineEditFreqStar->text();
    param->rfFreqStop = ui->lineEditFreqStop->text();
    param->rfFreqStep = ui->lineEditFreqStep->text();
}

void QCalR1CTXFilterOffsetDlg::updateFromParam(const CalR1CParam &param)
{
    ui->lineEditFreqStar->setText(param.rfTxFreqStar);
    ui->lineEditFreqStop->setText(param.rfTxFreqStop);
    ui->comboBoxCalIOMode->setCurrentIndex(param.txMode);
}

void QCalR1CTXFilterOffsetDlg::update(const QModelIndex &tl,const QModelIndex &br,cal_file::cal_item_t item,int sec)
{
    if (item == cal_file::TX_FILTER_OFFSET_80) {
        ui->tabWidget->widget_80->tableView->selectRow(tl.row());
        ui->tabWidget->widget_80->plot->replot();
    }
}

void QCalR1CTXFilterOffsetDlg::getset()
{

}


void QCalR1CRXFilterDlg::init()
{
    QRXFilterModel_80M  *model_80  = new QRXFilterModel_80M(reinterpret_cast<QObject *>(this));
    QRXFilterModel_160M *model_160 = new QRXFilterModel_160M(reinterpret_cast<QObject *>(this));
    model->append(model_80);
    model->append(model_160);
    ui->tabWidget->init(model_80,model_160);

    ui->lineEditFreq->setText("2G");
    ui->comboBoxBw->setCurrentIndex(sp1401::_80M);

    addIdleWidget(ui->pushButtonExport);
    addIdleWidget(ui->pushButtonStar);
    addIdleWidget(ui->lineEditFreq);
    addIdleWidget(ui->comboBoxBw);
    addIdleWidget(ui->pushButtonGetSet);
    addRunningWidget(ui->pushButtonStop);
    addToPauseWidget(ui->pushButtonPaus);
    addToRunningWidget(ui->pushButtonCont);
    widgetResume();
    QCalBaseDlg::init();
}

void QCalR1CRXFilterDlg::resetShowWidget(CalParam *param)
{
    ui->tabWidget->resetShowWidget();
    rx_filter_80m_table::data_f_t data_80;
    rx_filter_160m_table::data_f_t data_160;
    for (qint32 i = 0;i < RF_RX_FREQ_PTS_CALLED_R1C;i ++) {
        ui->tabWidget->model_80->calTable()->append(data_80);
        ui->tabWidget->model_160->calTable()->append(data_160);
    }
    emit reset();
    ui->tabWidget->tableView_80->selectRow(0);
    ui->tabWidget->tableView_160->selectRow(0);
}

void QCalR1CRXFilterDlg::uiToCalParam(CalParam *param)
{
    param->parent = this;
    param->model_0 = ui->tabWidget->model_80;
    param->model_1 = ui->tabWidget->model_160;
    param->plotData_0 = ui->tabWidget->dataRF_FR;
    param->plotData_1 = ui->tabWidget->dataIF_FR;
    param->_sp1401 = SP1401;
    param->_sp2401 = SP2401;
    param->justRebuildCoef = ui->checkBoxRebuildCoef->isChecked();
}

void QCalR1CRXFilterDlg::updateFromParam(const CalR1CParam &param)
{
    ui->checkBoxRebuildCoef->setChecked(false);
}

void QCalR1CRXFilterDlg::update(const QModelIndex &tl,const QModelIndex &br,cal_file::cal_item_t item,int sec)
{
    if (cal_file::RX_RF_FR == item)
        ui->tabWidget->plotRF->replot();
    else if (cal_file::RX_IF_FR == item)
        ui->tabWidget->plotIF->replot();
    else if (cal_file::RX_FILTER_80 == item)
        ui->tabWidget->tableView_80->selectRow(tl.row());
    else if (cal_file::RX_FILTER_160 == item)
        ui->tabWidget->tableView_160->selectRow(tl.row());
    else
        return;
}

void QCalR1CRXFilterDlg::getset()
{
    bw_t bw = bw_t(ui->comboBoxBw->currentIndex());
    double real[RX_FILTER_ORDER] = {0.0};
    double imag[RX_FILTER_ORDER] = {0.0};
    ZERO_ARRAY(real);
    ZERO_ARRAY(imag);
    quint64 freq = freq_string_to<quint64>(ui->lineEditFreq->text().toStdString());

    SP1401->set_rx_freq(freq);
    SP1401->cf()->set_bw(bw);

    if (_80M == bw) {
        rx_filter_80m_table::data_m_t data;
        ((cal_file_r1cd *)(SP1401->cf()))->m_rx_filter_80m->get(freq,&data);
        data._2double(real,imag);
        SP2401->set_rx_filter_sw(sp2401_r1a::_3D);
    }
    if (_160M == bw) {
        rx_filter_160m_table::data_m_t data;
        ((cal_file_r1cd *)(SP1401->cf()))->m_rx_filter_160m->get(freq,&data);
        data._2double(real,imag);
        SP2401->set_rx_filter_sw(sp2401_r1a::_2I3D);
    }
    SP2401->set_rx_filter(real,imag);
}


void QCalR1CRXRefDlg::init()
{
    QR1CRXRefModel *modelOP = new QR1CRXRefModel(reinterpret_cast<QObject *>(this));
    QR1CRXRefModel *modelIO = new QR1CRXRefModel(reinterpret_cast<QObject *>(this));
    model->append(modelOP);
    model->append(modelIO);

    ui->widgetData->init(modelOP,modelIO);
    ui->lineEditFreqStar->setText(QString("%1M").arg(RF_RX_FREQ_STAR / 1000000));
    ui->lineEditFreqStop->setText(QString("%1M").arg(RF_RX_FREQ_STOP / 1000000));
    ui->lineEditFreqStep->setText(QString("%1M").arg(RF_RX_FREQ_STEP_CALLED_R1C / 1000000));
    ui->lineEditFreq->setText("2G");
    ui->lineEditRef->setText("10");
    ui->comboBoxCalIOMode->setCurrentIndex(CAL_IO_OP);

    addIdleWidget(ui->lineEditFreqStar);
    addIdleWidget(ui->lineEditFreqStop);
    addIdleWidget(ui->comboBoxCalIOMode);
    addIdleWidget(ui->pushButtonExport);
    addIdleWidget(ui->pushButtonStar);
    addIdleWidget(ui->pushButtonGetSet);
    addRunningWidget(ui->pushButtonStop);
    addToPauseWidget(ui->pushButtonPaus);
    addToRunningWidget(ui->pushButtonCont);
    widgetResume();
    QCalBaseDlg::init();
}

void QCalR1CRXRefDlg::resetShowWidget(CalParam *param)
{
    ui->widgetData->resetShowWidget(param);
    emit(reset());
}

void QCalR1CRXRefDlg::uiToCalParam(CalParam *param)
{
    param->parent = this;
    param->mode = CalIOMode(ui->comboBoxCalIOMode->currentIndex());
    param->model_0 = model->at(0);
    param->model_1 = model->at(1);
    param->_sp1401 = SP1401;
    param->_sp2401 = SP2401;
    param->rfFreqStar = ui->lineEditFreqStar->text();
    param->rfFreqStop = ui->lineEditFreqStop->text();
    param->rfFreqStep = ui->lineEditFreqStep->text();
}

void QCalR1CRXRefDlg::updateFromParam(const CalR1CParam &param)
{
    ui->lineEditFreqStar->setText(param.rfRxFreqStar);
    ui->lineEditFreqStop->setText(param.rfRxFreqStop);
    ui->comboBoxCalIOMode->setCurrentIndex(param.rxMode);
}

void QCalR1CRXRefDlg::update(const QModelIndex &tl,const QModelIndex &br,cal_file::cal_item_t item,int sec)
{
    if (cal_file::RX_REF_OP == item) {
        ui->widgetData->tableViewOP->selectRow(tl.row());
        ui->widgetData->plotDGain->replot();
    }
    else {
        ui->widgetData->tableViewIO->selectRow(tl.row());
        ui->widgetData->plotDGain->replot();
    }
}

void QCalR1CRXRefDlg::getset()
{
    int32_t offset = 0;
    io_mode_t mode = io_mode_t(ui->comboBoxIOMode->currentIndex());
    bw_t bw = SP1401->get_bw();
    double ref = ui->lineEditRef->text().toDouble();
    quint64 freq = freq_string_to<quint64>(ui->lineEditFreq->text().toStdString());
    double pwr = 0.0;

    rx_ref_table_r1cd::rx_state_m_t rxState;
    rx_filter_offset_table::data_m_t dataFilOff;

    if (mode == OUTPUT) {
        ((cal_file_r1cd *)(SP1401->cf()))->m_rx_ref_op->get(SP1401->get_hw_ver(),freq,ref,&rxState);
        offset = ((cal_file_r1cd *)(SP1401->cf()))->m_rx_att_op->get(SP1401->get_hw_ver(),freq,ref);
        offset += rxState.ad_offset;

        if (bw == _80M) {
            ((cal_file_r1cd *)(SP1401->cf()))->m_rx_filter_offset_op_80m->get(freq,&dataFilOff);
            offset += dataFilOff.offset[rx_ref_op_table_r1cd::get_ref_seg_idx(ref)];
        }
    }
    if (mode == IO) {
        ((cal_file_r1cd *)(SP1401->cf()))->m_rx_ref_io->get(SP1401->get_hw_ver(),freq,ref,&rxState);
        offset = ((cal_file_r1cd *)(SP1401->cf()))->m_rx_att_io->get(SP1401->get_hw_ver(),freq,ref);
        offset += rxState.ad_offset;

        if (bw == _80M) {
            ((cal_file_r1cd *)(SP1401->cf()))->m_rx_filter_offset_io_80m->get(freq,&dataFilOff);
            offset += dataFilOff.offset[rx_ref_io_table_r1cd::get_ref_seg_idx(ref)];
        }
    }

    SP1401->set_io_mode(mode);
    SP1401->set_rx_freq(freq);
    SP1401->set_rx_lna_att_sw(rx_lna_att_t(rxState.lna_att));
    DYNAMIC_SP1401_R1CE_SP1401(SP1401)->set_rx_att_019_sw(rx_att_019_t(rxState.att_019));
    DYNAMIC_SP1401_R1CE_SP1401(SP1401)->
            set_rx_att(double(rxState.att1),
                       double(rxState.att2),
                       double(rxState.att3));
    SP2401->set_rx_pwr_comp(offset);
    Sleep(2);

    basic_sp1401::pwr_meas_state_t state = basic_sp1401::PMS_IDLE;
    SP1401->set_pwr_meas_src(basic_sp1401::PWR_MEAS_FREE_RUN,false);
    SP1401->set_pwr_meas_timeout(6553600);
    SP1401->set_pwr_meas_samples(102400);
    SP1401->pwr_meas_abort();
    SP1401->pwr_meas_start();

    while (state != basic_sp1401::PMS_DONE) {
        SP1401->get_pwr_meas_state(state);
        if (state == basic_sp1401::PMS_TT)
            break;
    }
    SP1401->get_pwr_meas_pwr(pwr);
    pwr += ref;
    ui->lineEditPowerFB->setText(QString("%1").arg(pwr));

    qint64 ad = 0;
    getADS5474(SP1401,ad,AVERAGE_TIMES);
    pwr = ad2dBc(_0dBFS,ad) + ref;
    ui->lineEditPowerNB->setText(QString("%1").arg(pwr));

    ui->lineEditAtt1->setText(QString("%1").arg(rxState.att1));
    ui->lineEditAtt2->setText(QString("%1").arg(rxState.att2));
    ui->lineEditAtt3->setText(QString("%1").arg(rxState.att3));
    ui->lineEditLNAAtt->setText(QString(rxState.lna_att == RX_LNA ? "LNA" : (rxState.att_019 == RX_ATT_0 ? "Att 0" : "Att 19")));
}


void QCalR1CRXAttDlg::init()
{
    model->append(new QR1CRxAttOPModel(SP1401,reinterpret_cast<QObject *>(this)));
    model->append(new QR1CRxAttIOModel(SP1401,reinterpret_cast<QObject *>(this)));
    ui->tabWidget->init((QR1CRxAttOPModel *)(model->at(0)),(QR1CRxAttIOModel *)(model->at(1)));
    ui->lineEditFreqStar->setText(QString("%1M").arg(RF_RX_FREQ_STAR / 1000000));
    ui->lineEditFreqStop->setText(QString("%1M").arg(RF_RX_FREQ_STOP / 1000000));
    ui->lineEditFreqStep->setText(QString("%1M").arg(RF_RX_FREQ_STEP_CALLED_R1C / 1000000));
    ui->lineEditFreq->setText("2G");
    ui->comboBoxCalIOMode->setCurrentIndex(CAL_IO_OP);

    addIdleWidget(ui->lineEditFreqStar);
    addIdleWidget(ui->lineEditFreqStop);
    addIdleWidget(ui->lineEditFreqStep);
    addIdleWidget(ui->pushButtonExport);
    addIdleWidget(ui->pushButtonStar);
    addIdleWidget(ui->pushButtonGetSet);
    addRunningWidget(ui->pushButtonStop);
    addToPauseWidget(ui->pushButtonPaus);
    addToRunningWidget(ui->pushButtonCont);
    widgetResume();
    QCalBaseDlg::init();
}

void QCalR1CRXAttDlg::resetShowWidget(CalParam *param)
{
    ((QR1CRxAttOPModel *)(model->at(0)))->setSP1401(SP1401);
    ((QR1CRxAttIOModel *)(model->at(1)))->setSP1401(SP1401);

    ui->tabWidget->resetShowWidget(param);
    ui->tableViewAttOP->setEnabled(calOP(param->mode));
    ui->tableViewAttIO->setEnabled(calIO(param->mode));

    emit reset();
}

void QCalR1CRXAttDlg::uiToCalParam(CalParam *param)
{
    param->parent = this;
    param->mode = CalIOMode(ui->comboBoxCalIOMode->currentIndex());
    param->model_0 = model->at(0);
    param->model_1 = model->at(1);
    param->_sp1401 = SP1401;
    param->_sp2401 = SP2401;
    param->rfFreqStar = ui->lineEditFreqStar->text();
    param->rfFreqStop = ui->lineEditFreqStop->text();
    param->rfFreqStep = ui->lineEditFreqStep->text();
}

void QCalR1CRXAttDlg::updateFromParam(const CalR1CParam &param)
{
    ui->lineEditFreqStar->setText(param.rfRxFreqStar);
    ui->lineEditFreqStop->setText(param.rfRxFreqStop);
    ui->comboBoxCalIOMode->setCurrentIndex(param.rxMode);
}

void QCalR1CRXAttDlg::update(const QModelIndex &tl,const QModelIndex &br,cal_file::cal_item_t item,int sec)
{
    if (cal_file::RX_ATT_OP == item) {
        ui->tabWidget->tableViewOP->selectRow(tl.row());
        ui->tabWidget->plotOP->replot();
    }
    else {
        ui->tabWidget->tableViewIO->selectRow(tl.row());
        ui->tabWidget->plotIO->replot();
    }
}

void QCalR1CRXAttDlg::getset()
{

}


void QCalR1CRXFilterOffsetDlg::init()
{
    model->append(new QRXFilterOffsetModel(reinterpret_cast<QObject *>(this)));
    ui->tabWidget->init((QRXFilterOffsetModel *)(model->at(0)));
    ui->lineEditFreqStar->setText(QString("%1M").arg(RF_RX_FREQ_STAR / 1000000));
    ui->lineEditFreqStop->setText(QString("%1M").arg(RF_RX_FREQ_STOP / 1000000));
    ui->lineEditFreqStep->setText(QString("%1M").arg(RF_RX_FREQ_STEP_CALLED_R1C / 1000000));
    ui->comboBoxCalIOMode->setCurrentIndex(CAL_IO_OP);

    addIdleWidget(ui->lineEditFreqStar);
    addIdleWidget(ui->lineEditFreqStop);
    addIdleWidget(ui->comboBoxCalIOMode);
    addIdleWidget(ui->pushButtonExport);
    addIdleWidget(ui->pushButtonStar);
    addRunningWidget(ui->pushButtonStop);
    addToPauseWidget(ui->pushButtonPaus);
    addToRunningWidget(ui->pushButtonCont);
    widgetResume();
    QCalBaseDlg::init();
}

void QCalR1CRXFilterOffsetDlg::resetShowWidget(CalParam *param)
{
    ui->tabWidget->resetShowWidget(param);

    emit reset();
}

void QCalR1CRXFilterOffsetDlg::uiToCalParam(CalParam *param)
{
    param->parent = this;
    param->mode = CalIOMode(ui->comboBoxCalIOMode->currentIndex());
    param->model_0 = model->at(0);
    param->_sp1401 = SP1401;
    param->_sp2401 = SP2401;
    param->rfFreqStar = ui->lineEditFreqStar->text();
    param->rfFreqStop = ui->lineEditFreqStop->text();
    param->rfFreqStep = ui->lineEditFreqStep->text();
}

void QCalR1CRXFilterOffsetDlg::updateFromParam(const CalR1CParam &param)
{
    ui->lineEditFreqStar->setText(param.rfRxFreqStar);
    ui->lineEditFreqStop->setText(param.rfRxFreqStop);
    ui->comboBoxCalIOMode->setCurrentIndex(param.rxMode);
}

void QCalR1CRXFilterOffsetDlg::update(const QModelIndex &tl,const QModelIndex &br,cal_file::cal_item_t item,int sec)
{
    if (item == cal_file::RX_FILTER_OFFSET_80) {
        ui->tabWidget->widget_80->tableView->selectRow(tl.row());
        ui->tabWidget->widget_80->plot->replot();
    }
}

void QCalR1CRXFilterOffsetDlg::getset()
{

}
