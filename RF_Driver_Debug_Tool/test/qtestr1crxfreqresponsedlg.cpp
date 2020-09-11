#include "ui_qtestr1crxfreqresponsedlg.h"
#include "qtestr1crxfreqresponsethread.h"
#include "qtestr1crxfreqresponsedlg.h"
#include "q_rdt_tableview.h"
#include "q_cal_plot.h"
#include <QDebug>
IMPLEMENT_TEST_R1C_DLG(QTestR1CRxFreqResponseDlg,QTestR1CRxFreqResponseThread,QExportR1CRxFreqResponseThread,QGetsetR1CRxFreqResponseThread)

QTestR1CRxFreqResponseDlg::QTestR1CRxFreqResponseDlg(QWidget *parent) :
    QTestR1CBaseDlg(parent),
    ui(new Ui::QTestR1CRxFreqResponseDlg)
{
    ui->setupUi(this);

    _model->append(new QR1CRxFreqResponseModel(this));
    _model->append(new QR1CRxFreqResponseModel(this));
    ui->tabWidget->init((QR1CRxFreqResponseModel *)(_model->at(0)),(QR1CRxFreqResponseModel *)(_model->at(1)));
    ui->lineEditFreqStar->setText(QString("%1M").arg(RF_TX_FREQ_STAR / 1000000));
    ui->lineEditFreqStop->setText(QString("%1M").arg(RF_TX_FREQ_STOP / 1000000));
    ui->lineEditFreqStep->setText(QString("%1M").arg(RF_TX_FREQ_STEP_CALLED / 1000000));
    ui->comboBoxCalIOMode->setCurrentIndex(TestParam::CAL_OP);

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

QTestR1CRxFreqResponseDlg::~QTestR1CRxFreqResponseDlg()
{
    delete ui;
}

void QTestR1CRxFreqResponseDlg::resetShowWidget(TestParam *param)
{
    ui->tabWidget->resetShowWidget(param);

    emit reset();
}

TestParam QTestR1CRxFreqResponseDlg::ui2CalParam()
{
    TestParam p;
    p.parent = this;
    p.mode = (TestParam::TestIOMode)(ui->comboBoxCalIOMode->currentIndex());
    p.model_0 = _model->at(0);
    p.model_1 = _model->at(1);
    p._sp1401 = SP1401;
    p._sp2401 = SP2401;
    p.rfFreqStar = ui->lineEditFreqStar->text();
    p.rfFreqStop = ui->lineEditFreqStop->text();
    p.rfFreqStep = ui->lineEditFreqStep->text();
    p.testFlag = 1;
    return p;
}

void QTestR1CRxFreqResponseDlg::update(const QModelIndex &tl,const QModelIndex &br,cal_file::cal_item_t item,int sec)
{
    if (cal_file::RX_RF_FR == item) {
//        ui->tabWidget->widgetOP_80->tableView->selectRow(tl.row());
        ui->tabWidget->widgetOP_80->plot->replot();
    }
    else if (cal_file::RX_IF_FR == item) {
//        ui->tabWidget->widgetIO_80->tableView->selectRow(tl.row());
        ui->tabWidget->widgetIO_80->plot->replot();
    }
}
void QTestR1CRxFreqResponseDlg::updateCalUI(QList<QString> list)
{

}
