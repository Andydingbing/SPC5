#include "qcalr1crxfilterdlg.h"
#include "ui_qcalr1crxfilterdlg.h"
#include "qr1crxfiltermodel.h"
#include "qrdttableview.h"
#include "qcalr1crxfilterthread.h"
#include "qcalplot.h"

IMPLEMENT_CAL_R1C_DLG(QCalR1CRxFilterDlg,QCalR1CRxFilterThread,QExportR1CRxFilterThread)

QCalR1CRxFilterDlg::QCalR1CRxFilterDlg(QWidget *parent) :
    QCalR1CBaseDlg(parent),
    ui(new Ui::QCalR1CRxFilterDlg)
{
    ui->setupUi(this);
    _model->append(new QR1CRxFilterModel_80M(this));
    _model->append(new QR1CRxFilterModel_160M(this));
    ui->tabWidget->init((QR1CRxFilterModel_80M *)(_model->at(0)),(QR1CRxFilterModel_160M *)(_model->at(1)));
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

QCalR1CRxFilterDlg::~QCalR1CRxFilterDlg()
{
    delete ui;
}

void QCalR1CRxFilterDlg::resetShowWidget(CalParam *param)
{
    ui->tabWidget->resetShowWidget();
    rx_filter_80m_table::data_f_t data_80;
    rx_filter_160m_table::data_f_t data_160;
    for (qint32 i = 0;i < RF_RX_FREQ_PTS_CALLED_R1C;i ++) {
        ui->tabWidget->_model_80->calTable()->append(data_80);
        ui->tabWidget->_model_160->calTable()->append(data_160);
    }
    emit reset();
    ui->tabWidget->tableViewData_80->selectRow(0);
    ui->tabWidget->tableViewData_160->selectRow(0);
}

CalParam QCalR1CRxFilterDlg::ui2CalParam()
{
    CalParam p;
    p.parent = this;
    p.model_0 = ui->tabWidget->_model_80;
    p.model_1 = ui->tabWidget->_model_160;
    p.plotData_0 = ui->tabWidget->dataRfFr;
    p.plotData_1 = ui->tabWidget->dataIfFr;
    p._sp1401 = _sp1401;
    p._sp2401 = _sp2401;
    p.justRebuildCoef = ui->checkBoxRebuildCoef->isChecked();
    return p;
}

void QCalR1CRxFilterDlg::update(const QModelIndex &tl,const QModelIndex &br,cal_file::cal_item_t item,int sec)
{
    if (cal_file::RX_RF_FR == item)
        ui->tabWidget->plotRf->replot();
    else if (cal_file::RX_IF_FR == item)
        ui->tabWidget->plotIf->replot();
    else if (cal_file::RX_FILTER_80 == item)
        ui->tabWidget->tableViewData_80->selectRow(tl.row());
    else if (cal_file::RX_FILTER_160 == item)
        ui->tabWidget->tableViewData_160->selectRow(tl.row());
    else
        return;
}
