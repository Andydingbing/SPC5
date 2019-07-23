#include "ui_qtestr1crxfreqdlg.h"
#include "test/qtestr1crxfreqthread.h"
#include "test/qtestr1crxfreqdlg.h"
#include "q_rdt_tableview.h"
#include "q_cal_plot.h"
#include <QDebug>

    void QTestR1CRxFreqDlg::sp3301Change() {

    }
    void QTestR1CRxFreqDlg::pushButtonStar_clicked() {
        CAL_THREAD_EXISTED_CHK();
        widgetPrepare();
        TestParam param = ui2CalParam();
        resetShowWidget(&param);
        QWinThread::g_threadThread = new QTestR1CRxFreqThread(&param,this);
        QWinThread::g_threadThread->start();
    }
    void QTestR1CRxFreqDlg::on_pushButtonStop_clicked() {
        if (ui->pushButtonCont->isEnabled())
            QWinThread::g_threadThread->threadLock.unlock();
        QWinThread::g_threadStop = true;
    }
    void QTestR1CRxFreqDlg::on_pushButtonPaus_clicked() {
        QWinThread::g_threadThread->threadLock.lock();
        widgetPausing();
    }
    void QTestR1CRxFreqDlg::on_pushButtonCont_clicked() {
        QWinThread::g_threadThread->threadLock.unlock();
        widgetRunning();
    }

QTestR1CRxFreqDlg::QTestR1CRxFreqDlg(QWidget *parent) :
    QTestR1CBaseDlg(parent),
    ui(new Ui::QTestR1CRxFreqDlg)
{
    ui->setupUi(this);

    _model->append(new QR1CRxFreqModel(this));
    _model->append(new QR1CRxFreqModel(this));
    _model->append(new QR1CRxFreqModel(this));
    ui->tabWidget->init((QR1CRxFreqModel *)(_model->at(0)),(QR1CRxFreqModel *)(_model->at(1)));
    ui->lineEditFreqStar->setText(QString("%1M").arg(((RF_TX_FREQ_STAR+1950000000) / 1000000)));
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
    connect(ui->pushButtonStar,SIGNAL(clicked()),this,SLOT(pushButtonStar_clicked()));
}

QTestR1CRxFreqDlg::~QTestR1CRxFreqDlg()
{
    delete ui;
}

void QTestR1CRxFreqDlg::resetShowWidget(TestParam *param)
{
    ui->tabWidget->resetShowWidget(param);

    emit reset();
}

TestParam QTestR1CRxFreqDlg::ui2CalParam()
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
    p.testFlag = 1;
    p.tabIdxRf   = tabIdxRf;
    return p;
}

void QTestR1CRxFreqDlg::update(const QModelIndex &tl,const QModelIndex &br,cal_file::cal_item_t item,int sec)
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
void QTestR1CRxFreqDlg::updateCalUI(QList<QString> list)
{

}


