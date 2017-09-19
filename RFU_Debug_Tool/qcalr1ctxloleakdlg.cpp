#include "qcalr1ctxloleakdlg.h"
#include "ui_qcalr1ctxloleakdlg.h"
#include "mainwindow.h"
#include "qcalr1ctxloleakthread.h"

IMPLEMENT_CAL_R1C_DLG(QCalR1CTxLOLeakDlg)

QCalR1CTxLOLeakDlg::QCalR1CTxLOLeakDlg(QWidget *parent) :
    QCalR1CBaseDlg(parent),
    ui(new Ui::QCalR1CTxLOLeakDlg)
{
    ui->setupUi(this);
    m_pModel = new QR1CTxLOLeakModel;
    ui->m_TVData->setModel(m_pModel);
    ui->m_TVData->setColumnWidth(0,70);
    ui->m_TVData->setColumnWidth(1,100);
    ui->m_TVData->setColumnWidth(2,100);
    ui->m_TVData->setColumnWidth(3,80);
    ui->m_TVData->setColumnWidth(4,120);
    ui->m_TVData->setColumnWidth(5,100);
    ui->m_TVData->setColumnWidth(6,100);
    ui->m_TVData->setColumnWidth(7,100);
    ui->m_TVData->setColumnWidth(8,100);
    ui->m_TVData->setColumnWidth(9,120);
    ui->m_TVData->verticalHeader()->setDefaultSectionSize(20);
    ui->m_LEFreqStar->setText(QString("%1M").arg(RF_TX_FREQ_STAR / 1000000));
    ui->m_LEFreqStop->setText(QString("%1M").arg(RF_TX_FREQ_STOP / 1000000));
    ui->m_LEFreqStep->setText(QString("%1M").arg(RF_TX_FREQ_STEP_CALLED / 1000000));

    AddIdleWidget(ui->m_LEFreqStar);
    AddIdleWidget(ui->m_LEFreqStop);
    AddIdleWidget(ui->m_LEFreqStep);
    AddIdleWidget(ui->m_PBExport);
    AddIdleWidget(ui->m_CBCalRef);
    AddIdleWidget(ui->m_RBSpec);
    AddIdleWidget(ui->m_RBLoop);
    AddIdleWidget(ui->m_PBStar);
    AddIdleWidget(ui->m_LEFreq);
    AddIdleWidget(ui->m_LEDCI);
    AddIdleWidget(ui->m_LEDCQ);
    AddRunningWidget(ui->m_PBStop);
    AddToPauseWidget(ui->m_PBPaus);
    AddToRunningWidget(ui->m_PBCont);
    WidgetResume();
    connect(this,SIGNAL(updateShowWidget(QModelIndex,QModelIndex)),m_pModel,SLOT(update(QModelIndex,QModelIndex)));
}

QCalR1CTxLOLeakDlg::~QCalR1CTxLOLeakDlg()
{
    delete ui;
}

void QCalR1CTxLOLeakDlg::resetShowWidget(CalParam *pParam)
{
    m_pModel->m_pData->clear();
    TxLOLeakageTableR1C::DataF Data;
    int iPts = freq2array(pParam->m_strRfFreqStar,pParam->m_strRfFreqStop,pParam->m_strRfFreqStep);
    for (int32_t i = 0;i < iPts;i ++)
        m_pModel->m_pData->push_back(Data);
    emit updateShowWidget(m_pModel->index(0,0),m_pModel->index(iPts - 1,9));
}

void QCalR1CTxLOLeakDlg::on_m_PBStar_clicked()
{
    CAL_THREAD_EXISTED_CHK();
    WidgetPrepare();

    CalParam Param;
    Param.m_pModel = m_pModel;
    Param.m_pSP1401 = m_pSP1401;
    Param.m_pSP2401 = m_pSP2401;
    Param.m_pSP3501 = m_pSP3501;
    Param.m_strRfFreqStar = ui->m_LEFreqStar->text();
    Param.m_strRfFreqStop = ui->m_LEFreqStop->text();
    Param.m_strRfFreqStep = ui->m_LEFreqStep->text();
    Param.m_bCalX9119 = ui->m_CBCalRef->isChecked();
    Param.m_LOLeakMethod = ui->m_RBSpec->isChecked() ? CalParam::M_Spectrum : CalParam::M_Loopback;
    resetShowWidget(&Param);

    g_pThread = new QCalR1CTxLOLeakThread(&Param,this);
    QCalR1CTxLOLeakThread *pThread = (QCalR1CTxLOLeakThread *)g_pThread;
    connect(pThread,&QCalR1CTxLOLeakThread::finished,pThread,&QObject::deleteLater);
    connect(pThread,&QCalR1CTxLOLeakThread::initProg,g_pMainW,&MainWindow::initProg);
    connect(pThread,&QCalR1CTxLOLeakThread::setProgPos,g_pMainW,&MainWindow::setProgPos);
    connect(pThread,&QCalR1CTxLOLeakThread::threadCheckBox,g_pMainW,&MainWindow::threadCheckBox,Qt::BlockingQueuedConnection);
    connect(pThread,&QCalR1CTxLOLeakThread::threadErrorBox,g_pMainW,&MainWindow::threadErrorBox,Qt::BlockingQueuedConnection);
    connect(pThread,&QCalR1CTxLOLeakThread::update,m_pModel,&QR1CTxLOLeakModel::update);
    pThread->start();
}

void QCalR1CTxLOLeakDlg::on_m_PBStop_clicked()
{

}

void QCalR1CTxLOLeakDlg::on_m_PBPaus_clicked()
{

}

void QCalR1CTxLOLeakDlg::on_m_PBCont_clicked()
{

}

void QCalR1CTxLOLeakDlg::on_m_PBExport_clicked()
{

}

void QCalR1CTxLOLeakDlg::on_m_PBGetSet_clicked()
{

}
