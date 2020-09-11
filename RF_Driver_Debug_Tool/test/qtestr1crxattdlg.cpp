#include "qtestr1crxattdlg.h"
#include "ui_qtestr1crxattdlg.h"
#include "qtestr1crxattthread.h"
#include "q_cal_plot.h"
#include "q_rdt_tableview.h"

//IMPLEMENT_CAL_R1C_DLG(QTestR1CRxAttDlg,QTestR1CRxAttThread,QExportR1CRxAttThread,QGetsetR1CRxAttThread)
   void QTestR1CRxAttDlg::sp3301Change() {                               
        
    }                                                               
    void QTestR1CRxAttDlg::pushButtonStar_clicked() {
        CAL_THREAD_EXISTED_CHK();                                   
        widgetPrepare();                                            
        TestParam param = ui2CalParam();
        resetShowWidget(&param);                                    
        QWinThread::g_threadThread = new QTestR1CRxAttThread(&param,this);   
        QWinThread::g_threadThread->start();                        
    }                                                               
    void QTestR1CRxAttDlg::on_pushButtonStop_clicked() {                  
        if (ui->pushButtonCont->isEnabled())                        
            QWinThread::g_threadThread->threadLock.unlock();        
        QWinThread::g_threadStop = true;                            
    }                                                               
    void QTestR1CRxAttDlg::on_pushButtonPaus_clicked() {                  
        QWinThread::g_threadThread->threadLock.lock();              
        widgetPausing();                                            
    }                                                               
    void QTestR1CRxAttDlg::on_pushButtonCont_clicked() {                  
        QWinThread::g_threadThread->threadLock.unlock();            
        widgetRunning();                                            
    }                                                               


QTestR1CRxAttDlg::QTestR1CRxAttDlg(QWidget *parent) :
    QTestR1CBaseDlg(parent),
    ui(new Ui::QTestR1CRxAttDlg)
{
    ui->setupUi(this);
    _model->append(new QR1CRxAttTestOPModel(SP1401,this));
    _model->append(new QR1CRxAttTestIOModel(SP1401,this));
    ui->tabWidget->init((QR1CRxAttTestOPModel *)(_model->at(0)),(QR1CRxAttTestIOModel *)(_model->at(1)));
    ui->lineEditFreqStar->setText(QString("%1M").arg((RF_RX_FREQ_STAR + 1900000000) / 1000000));
    ui->lineEditFreqStop->setText(QString("%1M").arg(RF_RX_FREQ_STOP / 1000000));
    ui->lineEditFreqStep->setText(QString("%1M").arg((RF_RX_FREQ_STEP_CALLED_R1C / 1000000)*10));

    ui->comboBoxCalIOMode->setCurrentIndex(CAL_IO_OP);

    addIdleWidget(ui->lineEditFreqStar);
    addIdleWidget(ui->lineEditFreqStop);
    addIdleWidget(ui->lineEditFreqStep);

    addIdleWidget(ui->pushButtonStar);

    addRunningWidget(ui->pushButtonStop);
    addToPauseWidget(ui->pushButtonPaus);
    addToRunningWidget(ui->pushButtonCont);
    widgetResume();
    init();
    connect(ui->pushButtonStar,SIGNAL(clicked()),this,SLOT(pushButtonStar_clicked()));
}

QTestR1CRxAttDlg::~QTestR1CRxAttDlg()
{
    delete ui;
}

void QTestR1CRxAttDlg::resetShowWidget(TestParam *param)
{
    ((QR1CRxAttTestOPModel *)(_model->at(0)))->setSP1401(SP1401);
    ((QR1CRxAttTestIOModel *)(_model->at(1)))->setSP1401(SP1401);

    ui->tabWidget->resetShowWidget(param);
//    ui->tableViewAttOP->setEnabled(calOP(param->mode));
//    ui->tableViewAttIO->setEnabled(calIO(param->mode));

    emit reset();
}

TestParam QTestR1CRxAttDlg::ui2CalParam()
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
//    p.getRxAttFreq = ui->lineEditFreq->text();
//    p.getRxAttIoMode = (CalParam::CalIOMode)(ui->comboBoxIOMode->currentIndex());
    return p;
}

void QTestR1CRxAttDlg::update(const QModelIndex &tl,const QModelIndex &br,cal_file::cal_item_t item,int sec)
{
    if (cal_file::RX_ATT_OP == item) {
        //ui->tabWidget->tableViewOP->selectRow(tl.row());
        ui->tabWidget->plotOP->replot();
    }
    else {
        //ui->tabWidget->tableViewIO->selectRow(tl.row());
        ui->tabWidget->plotIO->replot();
    }
}
void QTestR1CRxAttDlg::updateCalUI(QList<QString> list)
{

}
