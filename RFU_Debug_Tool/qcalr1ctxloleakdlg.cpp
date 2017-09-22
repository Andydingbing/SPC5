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
    ui->m_LEFreqStar->setText(QString("%1M").arg(RF_TX_FREQ_STAR / 1000000));
    ui->m_LEFreqStop->setText(QString("%1M").arg(RF_TX_FREQ_STOP / 1000000));
    ui->m_LEFreqStep->setText(QString("%1M").arg(RF_TX_FREQ_STEP_CALLED / 1000000));
    ui->m_LEFreq->setText("2G");

    addIdleWidget(ui->m_LEFreqStar);
    addIdleWidget(ui->m_LEFreqStop);
    addIdleWidget(ui->m_LEFreqStep);
    addIdleWidget(ui->m_PBExport);
    addIdleWidget(ui->m_CBCalRef);
    addIdleWidget(ui->m_RBSpec);
    addIdleWidget(ui->m_RBLoop);
    addIdleWidget(ui->m_PBStar);
    addIdleWidget(ui->m_LEFreq);
    addIdleWidget(ui->m_LEDCI);
    addIdleWidget(ui->m_LEDCQ);
    addIdleWidget(ui->m_PBGetSet);
    addRunningWidget(ui->m_PBStop);
    addToPauseWidget(ui->m_PBPaus);
    addToRunningWidget(ui->m_PBCont);
    widgetResume();
    connect(this,SIGNAL(updateShowWidget(const QModelIndex,const QModelIndex)),m_pModel,SLOT(update(const QModelIndex,const QModelIndex)));
}

QCalR1CTxLOLeakDlg::~QCalR1CTxLOLeakDlg()
{
    delete ui;
}

void QCalR1CTxLOLeakDlg::resetShowWidget(CalParam *pParam)
{
    QR1CTxLOLeakModel *pModel = (QR1CTxLOLeakModel *)m_pModel;
    pModel->m_pData->clear();
    TxLOLeakageTableR1C::DataF Data;
    int iPts = freq2array(pParam->m_strRfFreqStar,pParam->m_strRfFreqStop,pParam->m_strRfFreqStep);
    for (int32_t i = 0;i < iPts;i ++)
        pModel->m_pData->push_back(Data);
    emit updateShowWidget(pModel->index(0,0),pModel->index(iPts - 1,9));
    ui->m_TVData->selectRow(0);
}

CalParam QCalR1CTxLOLeakDlg::ui2CalParam()
{
    CalParam p;
    p.m_pParent = this;
    p.m_pModel = m_pModel;
    p.m_pSP1401 = m_pSP1401;
    p.m_pSP2401 = m_pSP2401;
    p.m_pSP3501 = m_pSP3501;
    p.m_strRfFreqStar = ui->m_LEFreqStar->text();
    p.m_strRfFreqStop = ui->m_LEFreqStop->text();
    p.m_strRfFreqStep = ui->m_LEFreqStep->text();
    p.m_bCalX9119 = ui->m_CBCalRef->isChecked();
    p.m_LOLeakMethod = ui->m_RBSpec->isChecked() ? CalParam::M_Spectrum : CalParam::M_Loopback;
    return p;
}

void QCalR1CTxLOLeakDlg::update(const QModelIndex topleft, const QModelIndex rightbottom)
{
    ui->m_TVData->selectRow(topleft.row());
}

void QCalR1CTxLOLeakDlg::on_m_PBStar_clicked()
{
    CAL_THREAD_EXISTED_CHK();
    widgetPrepare();

    CalParam Param = ui2CalParam();
    resetShowWidget(&Param);

    g_pThread = new QCalR1CTxLOLeakThread(&Param,this);
    g_pThread->start();
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
    CAL_THREAD_EXISTED_CHK();
    widgetPrepare();

    CalParam Param = ui2CalParam();
    resetShowWidget(&Param);

    g_pThread = new QExportR1CTxLOLeakThread(&Param,this);
    g_pThread->start();
}

void QCalR1CTxLOLeakDlg::on_m_PBGetSet_clicked()
{

}
