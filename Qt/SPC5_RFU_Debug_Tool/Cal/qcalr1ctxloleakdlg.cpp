#include "qcalr1ctxloleakdlg.h"
#include "ui_qcalr1ctxloleakdlg.h"
#include "mainwindow.h"
#include "qcalr1ctxloleakthread.h"

IMPLEMENT_CAL_R1C_DLG(QCalR1CTxLOLeakDlg,QCalR1CTxLOLeakThread,QExportR1CTxLOLeakThread)

QCalR1CTxLOLeakDlg::QCalR1CTxLOLeakDlg(QWidget *parent) :
    QCalR1CBaseDlg(parent),
    ui(new Ui::QCalR1CTxLOLeakDlg)
{
    ui->setupUi(this);
    _model->append(new QR1CTxLOLeakModel(this));
    ui->tableViewData->setModel(_model->at(0));
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
    init();
}

QCalR1CTxLOLeakDlg::~QCalR1CTxLOLeakDlg()
{
    delete ui;
}

void QCalR1CTxLOLeakDlg::resetShowWidget(CalParam *param)
{
    QR1CTxLOLeakModel *model = (QR1CTxLOLeakModel *)(_model->at(0));
    model->calTable()->clear();
    tx_lol_table_r1cd::data_f_t data;
    int pts = freq2array(param->rfFreqStar,param->rfFreqStop,param->rfFreqStep);
    for (int32_t i = 0;i < pts;i ++)
        model->calTable()->append(data);
    emit reset();
    ui->tableViewData->selectRow(0);
}

CalParam QCalR1CTxLOLeakDlg::ui2CalParam()
{
    CalParam p;
    p.parent = this;
    p.model_0 = _model->at(0);
    p._sp1401 = _sp1401;
    p._sp2401 = _sp2401;
    p._sp3501 = _sp3501;
    p.rfFreqStar = ui->lineEditFreqStar->text();
    p.rfFreqStop = ui->lineEditFreqStop->text();
    p.rfFreqStep = ui->lineEditFreqStep->text();
    p.calX9119 = ui->checkBoxCalRef->isChecked();
    p.methodLOLeak = ui->radioButtonSpec->isChecked() ? CalParam::M_Spectrum : CalParam::M_Loopback;
    return p;
}

void QCalR1CTxLOLeakDlg::update(const QModelIndex &tl,const QModelIndex &br,cal_file::cal_item_t item,int sec)
{
    ui->tableViewData->selectRow(tl.row());
}
