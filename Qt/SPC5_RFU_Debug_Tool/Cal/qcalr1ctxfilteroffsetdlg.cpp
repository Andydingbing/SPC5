#include "qcalr1ctxfilteroffsetdlg.h"
#include "ui_qcalr1ctxfilteroffsetdlg.h"
#include "qcalr1ctxfilteroffsetthread.h"
#include "qrdttableview.h"
#include "qcalplot.h"

IMPLEMENT_CAL_R1C_DLG(QCalR1CTxFilterOffsetDlg,QCalR1CTxFilterOffsetThread,QExportR1CTxFilterOffsetThread)

QCalR1CTxFilterOffsetDlg::QCalR1CTxFilterOffsetDlg(QWidget *parent) :
    QCalR1CBaseDlg(parent),
    ui(new Ui::QCalR1CTxFilterOffsetDlg)
{
    ui->setupUi(this);

    _model->append(new QR1CTxFilterOffsetModel(this));
    _model->append(new QR1CTxFilterOffsetModel(this));
    ui->tabWidget->init((QR1CTxFilterOffsetModel *)(_model->at(0)),(QR1CTxFilterOffsetModel *)(_model->at(1)));
    ui->lineEditFreqStar->setText(QString("%1M").arg(RF_TX_FREQ_STAR / 1000000));
    ui->lineEditFreqStop->setText(QString("%1M").arg(RF_TX_FREQ_STOP / 1000000));
    ui->lineEditFreqStep->setText(QString("%1M").arg(RF_TX_FREQ_STEP_CALLED / 1000000));
    ui->comboBoxCalIOMode->setCurrentIndex(CalParam::CAL_IO_OP);

    addIdleWidget(ui->lineEditFreqStar);
    addIdleWidget(ui->lineEditFreqStop);
    addIdleWidget(ui->comboBoxCalIOMode);
    addIdleWidget(ui->pushButtonExport);
    addIdleWidget(ui->pushButtonStar);
    addRunningWidget(ui->pushButtonStop);
    addToPauseWidget(ui->pushButtonPaus);
    addToRunningWidget(ui->pushButtonCont);
    widgetResume();
    init();
}

QCalR1CTxFilterOffsetDlg::~QCalR1CTxFilterOffsetDlg()
{
    delete ui;
}

void QCalR1CTxFilterOffsetDlg::resetShowWidget(CalParam *param)
{
    ui->tabWidget->resetShowWidget(param);

    emit reset();
}

CalParam QCalR1CTxFilterOffsetDlg::ui2CalParam()
{
    CalParam p;
    p.parent = this;
    p.mode = (CalParam::CalIOMode)(ui->comboBoxCalIOMode->currentIndex());
    p.model_0 = _model->at(0);
    p.model_1 = _model->at(1);
    p._sp1401 = _sp1401;
    p._sp2401 = _sp2401;
    p.rfFreqStar = ui->lineEditFreqStar->text();
    p.rfFreqStop = ui->lineEditFreqStop->text();
    p.rfFreqStep = ui->lineEditFreqStep->text();
    return p;
}

void QCalR1CTxFilterOffsetDlg::update(const QModelIndex &tl,const QModelIndex &br,cal_file::cal_item_t item,int sec)
{
    if (cal_file::TX_FILTER_OFFSET_OP_80 == item) {
        ui->tabWidget->widgetOP_80->tableView->selectRow(tl.row());
        ui->tabWidget->widgetOP_80->plot->replot();
    }
    else if (cal_file::TX_FILTER_OFFSET_IO_80 == item) {
        ui->tabWidget->widgetIO_80->tableView->selectRow(tl.row());
        ui->tabWidget->widgetIO_80->plot->replot();
    }
}
