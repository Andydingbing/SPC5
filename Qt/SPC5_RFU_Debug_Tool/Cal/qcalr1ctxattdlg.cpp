#include "qcalr1ctxattdlg.h"
#include "ui_qcalr1ctxattdlg.h"
#include "qcalr1ctxattthread.h"
#include "qcalplot.h"
#include "qrdttableview.h"

IMPLEMENT_CAL_R1C_DLG(QCalR1CTxAttDlg,QCalR1CTxAttThread,QExportR1CTxAttThread)

QCalR1CTxAttDlg::QCalR1CTxAttDlg(QWidget *parent) :
    QCalR1CBaseDlg(parent),
    ui(new Ui::QCalR1CTxAttDlg)
{
    ui->setupUi(this);
    _model->append(new QR1CTxAttOPModel(_sp1401,this));
    _model->append(new QR1CTxAttIOModel(_sp1401,this));
    ui->tabWidget->init((QR1CTxAttOPModel *)(_model->at(0)),(QR1CTxAttIOModel *)(_model->at(1)));
    ui->lineEditFreqStar->setText(QString("%1M").arg(RF_TX_FREQ_STAR / 1000000));
    ui->lineEditFreqStop->setText(QString("%1M").arg(RF_TX_FREQ_STOP / 1000000));
    ui->lineEditFreqStep->setText(QString("%1M").arg(RF_TX_FREQ_STEP_CALLED / 1000000));
    ui->lineEditFreq->setText("2G");
    ui->comboBoxCalIOMode->setCurrentIndex(CalParam::CAL_IO_OP);

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
    init();
}

QCalR1CTxAttDlg::~QCalR1CTxAttDlg()
{
    delete ui;
}

void QCalR1CTxAttDlg::resetShowWidget(CalParam *param)
{
    ((QR1CTxAttOPModel *)(_model->at(0)))->setSP1401(_sp1401);
    ((QR1CTxAttIOModel *)(_model->at(1)))->setSP1401(_sp1401);

    ui->tabWidget->resetShowWidget(param);
    ui->tableViewAttOP->setEnabled(calOP(param->mode));
    ui->tableViewAttIO->setEnabled(calIO(param->mode));

    emit reset();
}

CalParam QCalR1CTxAttDlg::ui2CalParam()
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

void QCalR1CTxAttDlg::update(const QModelIndex &tl,const QModelIndex &br,cal_file::cal_item_t item,int sec)
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
