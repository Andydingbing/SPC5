#include "qcalr1ctxfilterdlg.h"
#include "ui_qcalr1ctxfilterdlg.h"
#include "qr1ctxfiltermodel.h"
#include "qrdttableview.h"
#include "qcalr1ctxfilterthread.h"
#include "qcalplot.h"

IMPLEMENT_CAL_R1C_DLG(QCalR1CTxFilterDlg,QCalR1CTxFilterThread,QExportR1CTxFilterThread)

QCalR1CTxFilterDlg::QCalR1CTxFilterDlg(QWidget *parent) :
    QCalR1CBaseDlg(parent),
    ui(new Ui::QCalR1CTxFilterDlg)
{
    ui->setupUi(this);
    _model->append(new QR1CTxFilterModel(this));
    _model->append(new QR1CTxFilterModel(this));
    ui->tabWidget->init((QR1CTxFilterModel *)(_model->at(0)),(QR1CTxFilterModel *)(_model->at(1)));
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
    init();
}

QCalR1CTxFilterDlg::~QCalR1CTxFilterDlg()
{
    delete ui;
}

void QCalR1CTxFilterDlg::resetShowWidget(CalParam *param)
{
    ui->tabWidget->resetShowWidget();
    tx_filter_80m_table::data_f_t data_80;
    tx_filter_160m_table::data_f_t data_160;
    for (int32_t i = 0;i < RF_TX_FREQ_PTS_CALLED;i ++) {
        ui->tabWidget->_model_80->calTable()->append(data_80);
        ui->tabWidget->_model_160->calTable()->append(data_160);
    }
    emit reset();
    ui->tabWidget->tableViewData_80->selectRow(0);
    ui->tabWidget->tableViewData_160->selectRow(0);
}

CalParam QCalR1CTxFilterDlg::ui2CalParam()
{
    CalParam p;
    p.parent = this;
    p.model_0 = ui->tabWidget->_model_80;
    p.model_1 = ui->tabWidget->_model_160;
    p.plotData_0 = ui->tabWidget->dataRfFr_0;
    p.plotData_1 = ui->tabWidget->dataRfFr_1;
    p.plotData_2 = ui->tabWidget->dataIfFr;
    p._sp1401 = _sp1401;
    p._sp2401 = _sp2401;
    p.justRebuildCoef = ui->checkBoxRebuildCoef->isChecked();
    return p;
}

void QCalR1CTxFilterDlg::update(const QModelIndex &tl,const QModelIndex &br,cal_file::cal_item_t item,int sec)
{
    if (cal_file::TX_RF_FR_0 == item)
        ui->tabWidget->plotRf->replot();
    else if (cal_file::TX_RF_FR_1 == item)
        ui->tabWidget->plotRf->replot();
    else if (cal_file::TX_IF_FR == item)
        ui->tabWidget->plotIf->replot();
    else if (cal_file::TX_FILTER_80 == item)
        ui->tabWidget->tableViewData_80->selectRow(tl.row());
    else if (cal_file::TX_FILTER_160 == item)
        ui->tabWidget->tableViewData_160->selectRow(tl.row());
    else
        return;
}
