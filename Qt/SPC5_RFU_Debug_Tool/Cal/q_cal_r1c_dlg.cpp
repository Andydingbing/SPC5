#include "q_cal_r1c_dlg.h"
#include "ui_q_cal_r1c_dlg.h"
#include "q_cal_base_dlg.h"
#include "q_cal_r1c_thread.h"
#include "mainwindow.h"
#include "q_cal_r1c_tx_loleak_thread.h"
#include "q_cal_r1c_tx_sb_thread.h"
#include "q_cal_r1c_tx_filter_thread.h"
#include "q_cal_r1c_tx_pwr_thread.h"
#include "q_cal_r1c_tx_att_thread.h"
#include "q_cal_r1c_tx_filter_offset_thread.h"
#include "q_cal_r1c_rx_filter_thread.h"
#include "q_cal_r1c_rx_ref_thread.h"
#include "q_cal_r1c_rx_att_thread.h"
#include "q_cal_r1c_rx_filter_offset_thread.h"

#include "q_trouble_shoot_model.hpp"

#include <QtNetwork/QNetworkInterface>
#include <QList>

QCalR1CDlg::QCalR1CDlg(QWidget *parent) :
    QWidget(parent),
    QAttachThreadDlg(),
    ui(new Ui::QCalR1CDlg)
{
    ui->setupUi(this);
    modelTxTS = new QTxTroubleShootModel(this);
    modelRxTS = new QRxTroubleShootModel(this);

    ui->tableViewTxTroubleShoot->setModel(modelTxTS);
    ui->tableViewTxTroubleShoot->setColumnWidth(0,0);

    ui->tableViewRxTroubleShoot->setModel(modelRxTS);
    ui->tableViewRxTroubleShoot->setColumnWidth(0,0);

    ui->lineEditTxFreqStar->setText(QString("%1M").arg(RF_TX_FREQ_STAR / 1000000));
    ui->lineEditTxFreqStop->setText(QString("%1M").arg(RF_TX_FREQ_STOP / 1000000));
    ui->lineEditTxFreqStep->setText(QString("%1M").arg(RF_TX_FREQ_STEP_CALLED / 1000000));
    ui->lineEditRxFreqStar->setText(QString("%1M").arg(RF_RX_FREQ_STAR / 1000000));
    ui->lineEditRxFreqStop->setText(QString("%1M").arg(RF_RX_FREQ_STOP / 1000000));
    ui->lineEditRxFreqStep->setText(QString("%1M").arg(RF_RX_FREQ_STEP_CALLED_R1C / 1000000));
    ui->comboBoxTxCalIOMode->setCurrentIndex(CAL_IO_OP);
    ui->comboBoxRxCalIOMode->setCurrentIndex(CAL_IO_OP);
    ui->ledTxLOLeakage->setColor(Qt::red);
    ui->ledTxSideband->setColor(Qt::red);
    ui->ledTxCompensateFilter->setColor(Qt::red);
    ui->ledTxBasePower->setColor(Qt::red);
    ui->ledTxAttenuation->setColor(Qt::red);
    ui->ledTxFilterOffset->setColor(Qt::red);
    ui->ledRxCompensateFilter->setColor(Qt::red);
    ui->ledRxReference->setColor(Qt::red);
    ui->ledRxAttenuation->setColor(Qt::red);
    ui->ledRxFilterOffset->setColor(Qt::red);
    ui->lineEditFreq->setText("2G");

    ui->comboBoxHwVer->setCurrentIndex(R1E);
    ui->lineEditOrdinal->setText("00000000");
    QList<QNetworkInterface> net = QNetworkInterface::allInterfaces();
    QList<QString> macAddr = {"00-00-00-00-00-00","00-00-00-00-00-00"};
    for(int i = 0; i < net.count(); i ++) {
        if(net.at(i).flags().testFlag(QNetworkInterface::IsUp) &&
           net.at(i).flags().testFlag(QNetworkInterface::IsRunning) &&
          !net.at(i).flags().testFlag(QNetworkInterface::IsLoopBack)) {
            if (i < 2)
                macAddr.replace(i,net.at(i).hardwareAddress().replace(QString(":"),QString("-")));
            else
                macAddr.push_back(net.at(i).hardwareAddress().replace(QString(":"),QString("-")));
        }
    }
    ui->lineEditMAC_0->setText(macAddr.at(0));
    ui->lineEditMAC_1->setText(macAddr.at(1));

    addIdleWidget(ui->lineEditTxFreqStar);
    addIdleWidget(ui->lineEditTxFreqStop);
    addIdleWidget(ui->lineEditTxFreqStep);
    addIdleWidget(ui->lineEditRxFreqStar);
    addIdleWidget(ui->lineEditRxFreqStop);
    addIdleWidget(ui->lineEditRxFreqStep);
    addIdleWidget(ui->pushButtonStar);
    addIdleWidget(ui->pushButtonGetSet);
    addRunningWidget(ui->pushButtonStop);
    addToPauseWidget(ui->pushButtonPaus);
    addToRunningWidget(ui->pushButtonCont);
    widgetResume();
}

QCalR1CDlg::~QCalR1CDlg()
{
    delete ui;
}

void QCalR1CDlg::on_pushButtonStar_clicked()
{
    CAL_THREAD_EXISTED_CHK();
    widgetPrepare();

    quint8 rfIdx = SP1401->get_rf_idx();
    CalR1CParam param;

    param.txMode = CalIOMode(ui->comboBoxTxCalIOMode->currentIndex());
    param.rxMode = CalIOMode(ui->comboBoxRxCalIOMode->currentIndex());
    param.rfTxFreqStar = ui->lineEditTxFreqStar->text();
    param.rfTxFreqStop = ui->lineEditTxFreqStop->text();
    param.rfRxFreqStar = ui->lineEditRxFreqStar->text();
    param.rfRxFreqStop = ui->lineEditRxFreqStop->text();

    CalParam eachParam;
    QVector<CalParam> calParams;
    QVector<QCalBaseDlg *> calDlgs;
    QVector<QCalBaseDlg *>::iterator iterCalDlgs;
    QVector<QCalBaseThread *> calThreads;

    calDlgs.push_back(g_MainW->dlgCalR1CTXLOLeak[rfIdx]);
    calDlgs.push_back(g_MainW->dlgCalR1CTXSB[rfIdx]);
    calDlgs.push_back(g_MainW->dlgCalR1CTXFilter[rfIdx]);
    calDlgs.push_back(g_MainW->dlgCalR1CTXPwr[rfIdx]);
    calDlgs.push_back(g_MainW->dlgCalR1CTXAtt[rfIdx]);
    calDlgs.push_back(g_MainW->dlgCalR1CTXFilterOffset[rfIdx]);
    calDlgs.push_back(g_MainW->dlgCalR1CRXFilter[rfIdx]);
    calDlgs.push_back(g_MainW->dlgCalR1CRXRef[rfIdx]);
    calDlgs.push_back(g_MainW->dlgCalR1CRXAtt[rfIdx]);
    calDlgs.push_back(g_MainW->dlgCalR1CRXFilterOffset[rfIdx]);

    for (iterCalDlgs = calDlgs.begin();iterCalDlgs != calDlgs.end();iterCalDlgs ++) {
        (*iterCalDlgs)->updateFromParam(param);
        dynamic_cast<QAttachThreadDlg *>(*iterCalDlgs)->widgetPrepare();
//        eachParam = (*iterCalDlgs)->uiToCalParam();
//        (*iterCalDlgs)->resetShowWidget(&eachParam);
        calParams.push_back(eachParam);
    }

    param.calThreads.push_back(new QCalR1CTXLOLeakThread(calParams.at(0)));
    param.calThreads.push_back(new QCalR1CTXSBThread(calParams.at(1)));
    param.calThreads.push_back(new QCalR1CTXFilterThread(calParams.at(2)));
    param.calThreads.push_back(new QCalR1CTXPwrThread(calParams.at(3)));
    param.calThreads.push_back(new QCalR1CTXAttThread(calParams.at(4)));
    param.calThreads.push_back(new QCalR1CTXFilterOffsetThread(calParams.at(5)));
    param.calThreads.push_back(new QCalR1CRXFilterThread(calParams.at(6)));
    param.calThreads.push_back(new QCalR1CRXRefThread(calParams.at(7)));
    param.calThreads.push_back(new QCalR1CRXAttThread(calParams.at(8)));
    param.calThreads.push_back(new QCalR1CRXFilterOffsetThread(calParams.at(9)));

    QWinThread *thread = new QCalR1CThread(this,param);
    connect(thread,SIGNAL(update(QModelIndex,QModelIndex,cal_file::cal_item_t,int)),
                this,SLOT(update(QModelIndex,QModelIndex,cal_file::cal_item_t,int)));

    connect(thread,SIGNAL(done(bool)),this,SLOT(done()));

    thread->start();
}

void QCalR1CDlg::update(const QModelIndex &tl,const QModelIndex &br,cal_file::cal_item_t item,int sec)
{
    QWinThread::Process p = QWinThread::Process(sec);
    QColor clr = Qt::green;

    if (item == 0)
        ui->ledTxLOLeakage->setColor(clr);
    else if (item == 1)
        ui->ledTxSideband->setColor(clr);
    else if (item == 2)
        ui->ledTxCompensateFilter->setColor(clr);
    else if (item == 3)
        ui->ledTxBasePower->setColor(clr);
    else if (item == 4)
        ui->ledTxAttenuation->setColor(clr);
    else if (item == 5)
        ui->ledTxFilterOffset->setColor(clr);
    else if (item == 6)
        ui->ledRxCompensateFilter->setColor(clr);
    else if (item == 7)
        ui->ledRxReference->setColor(clr);
    else if (item == 8)
        ui->ledRxAttenuation->setColor(clr);
    else
        ui->ledRxFilterOffset->setColor(clr);
}

void QCalR1CDlg::on_pushButtonStop_clicked()
{

}

void QCalR1CDlg::on_pushButtonPaus_clicked()
{

}

void QCalR1CDlg::on_pushButtonCont_clicked()
{

}

void QCalR1CDlg::on_pushButtonSetRFInfo_clicked()
{
    hw_ver_t ver = hw_ver_t(ui->comboBoxHwVer->currentIndex());
    quint32 ordinal = ui->lineEditOrdinal->text().toUInt();

    sn_redundant_t redInfo;
    memcpy(redInfo.mac_0,ui->lineEditMAC_0->text().toStdString().c_str(),sizeof(redInfo.mac_0));
    memcpy(redInfo.mac_1,ui->lineEditMAC_1->text().toStdString().c_str(),sizeof(redInfo.mac_1));

    QVector<tx_trouble_shoot_item> *txTSState = modelTxTS->items();
    QVector<tx_trouble_shoot_item>::iterator iterTxTS;
    QVector<tx_trouble_shoot_t> txTSReady;

    QVector<rx_trouble_shoot_item> *rxTSState = modelRxTS->items();
    QVector<rx_trouble_shoot_item>::iterator iterRxTS;
    QVector<rx_trouble_shoot_t> rxTSReady;

    for (iterTxTS = txTSState->begin();iterTxTS != txTSState->end();iterTxTS ++) {
        if ((*iterTxTS).is_attached) {
            txTSReady.push_back((*iterTxTS).t);
            redInfo.tx_ts_num ++;
        }
    }
    for (iterRxTS = rxTSState->begin();iterRxTS != rxTSState->end();iterRxTS ++) {
        if ((*iterRxTS).is_attached) {
            rxTSReady.push_back((*iterRxTS).t);
            redInfo.rx_ts_num ++;
        }
    }

    QString remark = ui->plainTextEditRemark->toPlainText();
    redInfo.remark_num = remark.length();

    INT_CHECKV(SP1401->set_sn_major(ver,ordinal));
    INT_CHECKV(SP1401->set_sn_redundant(redInfo,
                                         txTSReady.toStdVector(),
                                         rxTSReady.toStdVector(),
                                         remark.toStdString()));
}

void QCalR1CDlg::on_pushButtonGetRFInfo_clicked()
{
    char snMajor[32] = {0};
    sn_redundant_t redInfo;
    std::vector<tx_trouble_shoot_t> txTSRead;
    std::vector<rx_trouble_shoot_t> rxTSRead;
    std::string remark;

    INT_CHECKV(SP1401->get_sn_major(snMajor));
    INT_CHECKV(SP1401->get_sn_redundant(redInfo,txTSRead,rxTSRead,remark));
}
