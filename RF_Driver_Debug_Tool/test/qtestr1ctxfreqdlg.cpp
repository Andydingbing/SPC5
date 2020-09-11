#include "qtestr1ctxfreqdlg.h"
#include "ui_qtestr1ctxfreqdlg.h"
#include "qr1ctxfreqmodel.h"
#include "q_rdt_tableview.h"
#include "qtestr1ctxfreqthread.h"
#include "q_cal_plot.h"

//IMPLEMENT_CAL_R1C_DLG(QTestR1CTxFreqDlg,QTestR1CTxFreqThread,QExportR1CTxFilterThread,QGetsetR1CTxFilterThread)

    void QTestR1CTxFreqDlg::sp3301Change() {                               
       
    }                                                               
    void QTestR1CTxFreqDlg::pushButtonStar_clicked()
    {
        TEST_THREAD_EXISTED_CHK();
        widgetPrepare();
        TestParam param = ui2CalParam();
        resetShowWidget(&param);
        QWinThread::g_threadThread = new QTestR1CTxFreqThread(&param,this);
        QWinThread::g_threadThread->start();
    }
    void QTestR1CTxFreqDlg::on_pushButtonStop_clicked() {                  
        if (ui->pushButtonCont->isEnabled())                        
            QWinThread::g_threadThread->threadLock.unlock();        
        QWinThread::g_threadStop = true;                            
    }                                                               
    void QTestR1CTxFreqDlg::on_pushButtonPaus_clicked() {                  
        QWinThread::g_threadThread->threadLock.lock();              
        widgetPausing();                                            
    }                                                               
    void QTestR1CTxFreqDlg::on_pushButtonCont_clicked() {                  
        QWinThread::g_threadThread->threadLock.unlock();            
        widgetRunning();                                            
    }                                                               



QTestR1CTxFreqDlg::QTestR1CTxFreqDlg(QWidget *parent) :
    QTestR1CBaseDlg(parent),
    ui(new Ui::QTestR1CTxFreqDlg)
{
    ui->setupUi(this);
    _model->append(new QR1CTxFreqModel(this));
    _model->append(new QR1CTxFreqModel(this));
    ui->tabWidget->init((QR1CTxFreqModel *)(_model->at(0)),(QR1CTxFreqModel *)(_model->at(1)));
    ui->lineEditFreq->setText("2G");
    ui->lineEdit->setText("-30");
    ui->comboBoxBw->setCurrentIndex(sp1401::_80M);


    addIdleWidget(ui->pushButtonStar);
    addIdleWidget(ui->lineEditFreq);
    addIdleWidget(ui->comboBoxBw);

    addRunningWidget(ui->pushButtonStop);
    addToPauseWidget(ui->pushButtonPaus);
    addToRunningWidget(ui->pushButtonCont);
    widgetResume();
    init();
    connect(ui->pushButtonStar,SIGNAL(clicked()),this,SLOT(pushButtonStar_clicked()));
}

QTestR1CTxFreqDlg::~QTestR1CTxFreqDlg()
{
    delete ui;
}

void QTestR1CTxFreqDlg::resetShowWidget(TestParam *param)
{
    ui->tabWidget->resetShowWidget();
    tx_filter_80m_table::data_f_t data_80;
    tx_filter_160m_table::data_f_t data_160;
    for (int32_t i = 0;i < RF_TX_FREQ_PTS_CALLED;i ++) {
        ui->tabWidget->_model_80->calTable()->append(data_80);
        ui->tabWidget->_model_160->calTable()->append(data_160);
    }
    emit reset();

}

TestParam QTestR1CTxFreqDlg::ui2CalParam()
{
    TestParam p;
    p.parent = this;
//    p.model_0 = ui->tabWidget->_model_80;
//    p.model_1 = ui->tabWidget->_model_160;
    p.plotData_0 = ui->tabWidget->dataRfFr_0;
    p.plotData_1 = ui->tabWidget->dataRfFr_1;
    p.plotData_2 = ui->tabWidget->dataIfFr;
    p._sp1401 = SP1401;
    p._sp2401 = SP2401;
    p._sp3301 = SP3301;
    p.getTxBasePower = ui->lineEditFreq->text();
    p.getTxCompenFiltFreq = ui->lineEditFreq->text();
    p.mode = (TestParam::TestIOMode)(ui->comboBoxBw->currentIndex());
    p.tabIdxRf   = tabIdxRf;
    return p;
}

void QTestR1CTxFreqDlg::update(const QModelIndex &tl,const QModelIndex &br,cal_file::cal_item_t item,int sec)
{
    if (cal_file::TX_RF_FR_0 == item)
    {
//        ui->tabWidget->plotRf->setAxisAutoScale(QwtPlot::yLeft,true);
        ui->tabWidget->plotRf->replot();


    }
    else if (cal_file::TX_RF_FR_1 == item)
    {
//        ui->tabWidget->plotRf->setAxisAutoScale(QwtPlot::yLeft,true);
        ui->tabWidget->plotRf->replot();
    }
    else if (cal_file::TX_IF_FR == item)
        ui->tabWidget->plotIf->replot();
    else if (cal_file::TX_FILTER_80 == item)
        ui->tabWidget->tableViewData_80->selectRow(tl.row());
    else if (cal_file::TX_FILTER_160 == item)
        ui->tabWidget->tableViewData_160->selectRow(tl.row());
    else
        return;
}


