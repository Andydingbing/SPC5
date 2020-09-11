#include "qtestr1ctestpowdlg.h"
#include "ui_qtestr1ctestpowdlg.h"
#include "qtestr1ctestpowthread.h"
#include "q_cal_plot.h"
#include "q_rdt_tableview.h"

//IMPLEMENT_CAL_R1C_DLG(QTestR1CTestPowDlg,QTestR1CTestPowThread,QExportR1CTestPowThread,QGetsetR1CTestPowThread)

void QTestR1CTestPowDlg::sp3301Change() {
   // ui->textEditPath->setText(QString("c:CSECalrfu%1rf%2.cal").arg(_sp1401->get_rfu_idx()).arg(_sp1401->get_rf_idx()));

}
void QTestR1CTestPowDlg::on_pushButtonStar_clicked() {
    CAL_THREAD_EXISTED_CHK();
    widgetPrepare();
    TestParam param = ui2CalParam();
    resetShowWidget(&param);
    QWinThread::g_threadThread = new QTestR1CTestPowThread(&param,this);
    QWinThread::g_threadThread->start();
}
void QTestR1CTestPowDlg::on_pushButtonStop_clicked() {
    if (ui->pushButtonCont->isEnabled())
        QWinThread::g_threadThread->threadLock.unlock();
    QWinThread::g_threadStop = true;
}
void QTestR1CTestPowDlg::on_pushButtonPaus_clicked() {
    QWinThread::g_threadThread->threadLock.lock();
    widgetPausing();
}
void QTestR1CTestPowDlg::on_pushButtonCont_clicked() {
    QWinThread::g_threadThread->threadLock.unlock();
    widgetRunning();
}
void QTestR1CTestPowDlg::on_pushButtonExport_clicked() {
    CAL_THREAD_EXISTED_CHK();
    widgetPrepare();
    TestParam Param = ui2CalParam();
    resetShowWidget(&Param);
    QWinThread::g_threadThread = new QExportR1CTestPowThread(&Param,this);
    QWinThread::g_threadThread->start();
}

QTestR1CTestPowDlg::QTestR1CTestPowDlg(QWidget *parent) :
    QTestR1CBaseDlg(parent),
    ui(new Ui::QTestR1CTestPowDlg)
{
    ui->setupUi(this);
    _model->append(new QR1CTestPowOPModel(SP1401,this));
    _model->append(new QR1CTestPowIOModel(SP1401,this));
    ui->tabWidget->init((QR1CTestPowOPModel *)(_model->at(0)),(QR1CTestPowIOModel *)(_model->at(1)));
    ui->lineEditFreqStar->setText(QString("%1M").arg((RF_TX_TEST_FREQ_STAR + 1000000000) / 1000000));
    ui->lineEditFreqStop->setText(QString("%1M").arg(RF_TX_TEST_FREQ_STOP / 1000000));
    ui->lineEditFreqStep->setText(QString("%1M").arg(RF_TX_TEST_FREQ_STEP / 1000000));

    ui->comboBoxCalIOMode->setCurrentIndex(CAL_IO_OP);

    addIdleWidget(ui->lineEditFreqStar);
    addIdleWidget(ui->lineEditFreqStop);
    addIdleWidget(ui->lineEditFreqStep);
   // addIdleWidget(ui->pushButtonExport);
    addIdleWidget(ui->pushButtonStar);

    addRunningWidget(ui->pushButtonStop);
    addToPauseWidget(ui->pushButtonPaus);
    addToRunningWidget(ui->pushButtonCont);
    widgetResume();

    init();
}

QTestR1CTestPowDlg::~QTestR1CTestPowDlg()
{
    delete ui;
}

void QTestR1CTestPowDlg::resetShowWidget(TestParam *param)
{
    ((QR1CTestPowOPModel *)(_model->at(0)))->setSP1401(SP1401);
    ((QR1CTestPowIOModel *)(_model->at(1)))->setSP1401(SP1401);

    ui->tabWidget->resetShowWidget(param);


    emit reset();
}

TestParam QTestR1CTestPowDlg::ui2CalParam()
{
    TestParam p;
    p.parent = this;
    p.mode = (TestParam::TestIOMode)(ui->comboBoxCalIOMode->currentIndex());
    p.model_0 = _model->at(0);
    p.model_1 = _model->at(1);
    p._sp1401 = SP1401;
    p._sp2401 = SP2401;
    p._sp3301 = SP3301;
    p.rfFreqStar = ui->lineEditFreqStar->text();
    p.rfFreqStop = ui->lineEditFreqStop->text();
    p.rfFreqStep = ui->lineEditFreqStep->text();
    p.tabIdxRf   = tabIdxRf;
    //p.getTestPowFreq   = ui->lineEditFreq->text();
    return p;
}

void QTestR1CTestPowDlg::update(const QModelIndex &tl,const QModelIndex &br,cal_file::cal_item_t item,int sec)
{
    if (cal_file::TX_ATT_OP == item) {
//        ui->tabWidget->tableViewOP->selectRow(tl.row());
        ui->tabWidget->plotOP->replot();
    }
    else {
//        ui->tabWidget->tableViewIO->selectRow(tl.row());
        ui->tabWidget->plotIO->replot();
    }
}
void QTestR1CTestPowDlg::updateCalUI(QList<QString> list)
{

}
