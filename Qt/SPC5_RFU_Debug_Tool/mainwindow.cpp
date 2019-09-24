#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QMetaType>
#include <QMessageBox>
#include <QTreeWidgetItem>
#include <QProgressBar>
#include "q_log_model.h"
#include "q_rf_r1a_dlg.h"
#include "q_rf_r1a_adv_dlg.h"
#include "q_rf_r1c_dlg.h"
#include "q_rf_r1c_adv_dlg.h"
#include "q_rf_r1f_dlg.h"
#include "q_rf_r1f_adv_dlg.h"
#include "q_temp_ctrl_dlg.h"
#include "q_device_init_thread.h"
#include "q_iq_cap_thread.h"

#if (QT_VERSION < QT_VERSION_CHECK(5,0,0))
    #define setSectionResizeMode setResizeMode
#endif

MainWindow *g_MainW = nullptr;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    tabIdxRf(0),
    tabIdxRfu(0),
    tabIdxDMA(0),
    _sp3301(&SP3301_2),
    _sp3501(&SP3501),
    childDlgLayout(new QHBoxLayout),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    registerMetaType();

    Log.en(log_t::RD_LOG_ALL_ON,false);
    initMsgLogDlg();
    initRegLogDlg();
    initStatusBar();
    initMainTreeWidget();
    initMainTabWidget();
    initChildDlg();
    Log.set_default();

    connect(ui->actionInit,SIGNAL(triggered(bool)),this,SLOT(deviceInit()));
    connect(ui->actionExit,SIGNAL(triggered(bool)),this,SLOT(exit()));
    connect(ui->actionRFU0,SIGNAL(triggered(bool)),this,SLOT(selSP3301_0()));
    connect(ui->actionRFU1,SIGNAL(triggered(bool)),this,SLOT(selSP3301_1()));
    connect(ui->actionRFU2,SIGNAL(triggered(bool)),this,SLOT(selSP3301_2()));
    connect(ui->actionRFU3,SIGNAL(triggered(bool)),this,SLOT(selSP3301_3()));
    connect(ui->actionRFU4,SIGNAL(triggered(bool)),this,SLOT(selSP3301_4()));
    connect(ui->actionStarAllIQ_Capture,SIGNAL(triggered(bool)),this,SLOT(starAllIQCapture()));
    connect(ui->actionStopAllIQ_Capture,SIGNAL(triggered(bool)),this,SLOT(stopAllIQCapture()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::deviceInit()
{
    QWinThread::g_threadThread = new QDeviceInitThread(this);
    QDeviceInitThread *thread = (QDeviceInitThread *)(QWinThread::g_threadThread);
    connect(thread,SIGNAL(swhwVerReady(sp3301::rfu_info_t,const char*)),
                 this,SLOT(showSwHwVer(sp3301::rfu_info_t,const char*)));
    thread->start();
}

void MainWindow::exit()
{
    QApplication::exit();
}

void MainWindow::selSP3301_0()
{
    ui->actionRFU0->setChecked(true);
    ui->actionRFU1->setChecked(false);
    ui->actionRFU2->setChecked(false);
    ui->actionRFU3->setChecked(false);
    ui->actionRFU4->setChecked(false);
    _sp3301 = &SP3301_0;
    updateParamInChildDlg();
    Log.add_msg(0,"working on rfu0");
}

void MainWindow::selSP3301_1()
{
    ui->actionRFU0->setChecked(false);
    ui->actionRFU1->setChecked(true);
    ui->actionRFU2->setChecked(false);
    ui->actionRFU3->setChecked(false);
    ui->actionRFU4->setChecked(false);
    _sp3301 = &SP3301_1;
    updateParamInChildDlg();
    Log.add_msg(0,"working on rfu1");
}

void MainWindow::selSP3301_2()
{
    ui->actionRFU0->setChecked(false);
    ui->actionRFU1->setChecked(false);
    ui->actionRFU2->setChecked(true);
    ui->actionRFU3->setChecked(false);
    ui->actionRFU4->setChecked(false);
    _sp3301 = &SP3301_2;
    updateParamInChildDlg();
    Log.add_msg(0,"working on rfu2");
}

void MainWindow::selSP3301_3()
{
    ui->actionRFU0->setChecked(false);
    ui->actionRFU1->setChecked(false);
    ui->actionRFU2->setChecked(false);
    ui->actionRFU3->setChecked(true);
    ui->actionRFU4->setChecked(false);
    _sp3301 = &SP3301_3;
    updateParamInChildDlg();
    Log.add_msg(0,"working on rfu3");
}

void MainWindow::selSP3301_4()
{
    ui->actionRFU0->setChecked(false);
    ui->actionRFU1->setChecked(false);
    ui->actionRFU2->setChecked(false);
    ui->actionRFU3->setChecked(false);
    ui->actionRFU4->setChecked(true);
    _sp3301 = &SP3301_4;
    updateParamInChildDlg();
    Log.add_msg(0,"working on rfu4");
}

void MainWindow::starAllIQCapture()
{
    THREAD_EXISTED_CHK();
    QIQCapThread::Param param;
    param._sp3301 = _sp3301;
    for (int i = 0;i < MAX_RF;i ++) {
        param.visible[i] = (dlgIQCap[i]->isVisible());
    }
    QWinThread::g_threadThread = new QIQCapThread(param,this);
    QIQCapThread *pThread = (QIQCapThread *)(QWinThread::g_threadThread);
    for (int i = 0;i < MAX_RF;i ++) {
        connect(pThread,SIGNAL(updatePlot()),dlgIQCap[i],SLOT(updatePlot()),Qt::BlockingQueuedConnection);
    }
    connect(this,SIGNAL(tabIdxChanged(int)),pThread,SLOT(tabIdxChanged(int)));
    QWinThread::g_threadThread->start();
}

void MainWindow::stopAllIQCapture()
{
  //by byf 2018.10.22
        QWinThread::g_threadStop = true;
        //QWinThread::g_threadThread->threadLock.unlock();

}

void MainWindow::showSwHwVer(const sp3301::rfu_info_t &info, const char *driver)
{
    labelVerK7_0->setText(QString("K7_0:0x%1").arg(info.k7_0_ver,8,16));
    labelVerK7_1->setText(QString("K7_1:0x%1").arg(info.k7_1_ver,8,16));
    labelVerS6->setText(QString("S6:0x%1").arg(info.s6_ver,8,16));
    labelVerDriver->setText(QString("Driver:%1").arg(driver));
}

void MainWindow::registerMetaType()
{
    qRegisterMetaType<sp3301::rfu_info_t>("sp3301::rfu_info_t");
    qRegisterMetaType<cal_file::cal_item_t>("cal_file::cal_item_t");
    qRegisterMetaType<test_item_t>("test_item_t");
    QWinThread::registerMetaType();
}

void MainWindow::initStatusBar()
{
    mainProgressBar = new QProgressBar(ui->statusBar);
    mainProgressBar->setFixedHeight(15);
    mainProgressBar->setStyleSheet("QProgressBar{text-align:center;}");
    setProgClr(Qt::green);

    labelProgressName = new QLabel(QString("..."));
    labelVerRF = new QLabel(QString("RF:"));
    labelVerK7_0 = new QLabel(QString("K7_0:0x00000000"));
    labelVerK7_1 = new QLabel(QString("K7_1:0x00000000"));
    labelVerS6 = new QLabel(QString("S6:0x00000000"));
    labelVerDriver = new QLabel(QString("Driver:0.0.00000"));

    ui->statusBar->clearMessage();
    ui->statusBar->addWidget(labelProgressName);
    ui->statusBar->addWidget(mainProgressBar,1);
    ui->statusBar->addPermanentWidget(labelVerRF);
    ui->statusBar->addPermanentWidget(labelVerK7_0);
    ui->statusBar->addPermanentWidget(labelVerK7_1);
    ui->statusBar->addPermanentWidget(labelVerS6);
    ui->statusBar->addPermanentWidget(labelVerDriver);
}

void MainWindow::initChildDlg()
{
    QSizePolicy childDlgPolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

#define ADD_CHILD_DLG(dlg,class_name) \
    dlg = new class_name(ui->mainTab); \
    dlg->setSizePolicy(childDlgPolicy); \
    dlg->setVisible(false); \
    childDlgLayout->addWidget(dlg);

#define ADD_CHILD_DLG__RF_DLG(dlg,class_name) \
    ADD_CHILD_DLG(dlg,class_name); \
    dlg->setSP3301(_sp3301); \
    dlg->setRFIdx(i);

    for (quint8 i = 0;i < MAX_RF;i ++) {
        ADD_CHILD_DLG(dlgRFR1AContainer[i],QRFR1AContainerDlg);
        ADD_CHILD_DLG(dlgRFR1CContainer[i],QRFR1CContainerDlg);
        ADD_CHILD_DLG(dlgRFR1FContainer[i],QRFR1FContainerDlg);

        dlgRFR1AContainer[i]->dlg->setSP3301(_sp3301);
        dlgRFR1AContainer[i]->dlg->setRFIdx(i);
        dlgRFR1CContainer[i]->dlg->setSP3301(_sp3301);
        dlgRFR1CContainer[i]->dlg->setRFIdx(i);
        dlgRFR1FContainer[i]->dlg->setSP3301(_sp3301);
        dlgRFR1FContainer[i]->dlg->setRFIdx(i);

        ADD_CHILD_DLG__RF_DLG(dlgRFR1AAdv[i],QRfR1AAdvDlg);
        ADD_CHILD_DLG__RF_DLG(dlgRFR1CAdv[i],QRFR1CAdvDlg);
        ADD_CHILD_DLG__RF_DLG(dlgRFR1FAdv[i],QRFR1FAdvDlg);
        ADD_CHILD_DLG__RF_DLG(dlgArb[i],QArbDlg);
        ADD_CHILD_DLG__RF_DLG(dlgIQCap[i],QIQCapDlg);
        ADD_CHILD_DLG__RF_DLG(dlgBB[i],QBbDlg);

        ADD_CHILD_DLG(dlgCalR1C[i],QCalR1CDlg);
        ADD_CHILD_DLG(dlgCalR1CTXLOLeak[i],QCalR1CTXLOLeakDlg);
        ADD_CHILD_DLG(dlgCalR1CTXSB[i],QCalR1CTXSBDlg);
        ADD_CHILD_DLG(dlgCalR1CTXFilter[i],QCalR1CTXFilterDlg);
        ADD_CHILD_DLG(dlgCalR1CTXPwr[i],QCalR1CTXPwrDlg);
        ADD_CHILD_DLG(dlgCalR1CTXAtt[i],QCalR1CTXAttDlg);
        ADD_CHILD_DLG(dlgCalR1CTXFilterOffset[i],QCalR1CTXFilterOffsetDlg);
        ADD_CHILD_DLG(dlgCalR1CRXFilter[i],QCalR1CRXFilterDlg);
        ADD_CHILD_DLG(dlgCalR1CRXRef[i],QCalR1CRXRefDlg);
        ADD_CHILD_DLG(dlgCalR1CRXAtt[i],QCalR1CRXAttDlg);
        ADD_CHILD_DLG(dlgCalR1CRXFilterOffset[i],QCalR1CRXFilterOffsetDlg);

        ADD_CHILD_DLG(dlgTestR1C[i],QTestR1CTabWidget);
    }
    ADD_CHILD_DLG(dlgFPGA,QFPGADlg);

    updateParamInChildDlg();
}

void MainWindow::initMainTreeWidget()
{
    ui->mainTree->setHeaderHidden(true);
    ui->mainTree->setRootIsDecorated(true);
    ui->mainTree->clear();
    ui->mainTree->setColumnCount(1);
    ui->mainTree->setStyleSheet(
                "QTreeWidget {background:rgb(179,217,255)}"
                "QTreeWidget::item:selected {background:rgb(0,255,0);color:black;}");

    QStringList root;
    QStringList rfChild;
    QStringList rfChildChild;
    QStringList bbChild;
    QStringList::ConstIterator iterString;

    root.append(tr("RF-R1F"));
    root.append(tr("BB-Debug"));
    root.append(tr("RF-R1C/D/E"));
    root.append(tr("RF-R1A/B"));

    rfChild.append(tr("Overview"));
    rfChild.append(tr("Advance"));
    rfChild.append(tr("Test"));
    rfChild.append(tr("Calibration"));

    bbChild.append(tr("Overview"));
    bbChild.append(tr("Arb"));
    bbChild.append(tr("IQ-Capture"));
    bbChild.append(tr("DMA"));
    bbChild.append(tr("FPGA"));

    QList<QTreeWidgetItem *> itemRoot;
    QList<QTreeWidgetItem *> itemChild;
    QList<QTreeWidgetItem *> itemChildChild;
    QList<QTreeWidgetItem *>::ConstIterator iterRoot;
    QList<QTreeWidgetItem *>::ConstIterator iterChild;

    for (iterString = root.constBegin();iterString != root.constEnd();iterString ++) {
        itemRoot.append(new QTreeWidgetItem(ui->mainTree,QStringList(*iterString)));
    }

    for (iterRoot = itemRoot.constBegin();iterRoot != itemRoot.constEnd();iterRoot ++) {
        if ((*iterRoot)->text(0).left(2) == "RF") {
            itemChild.clear();
            for (iterString = rfChild.constBegin();iterString != rfChild.constEnd();iterString ++) {
                itemChild.append(new QTreeWidgetItem(*iterRoot,QStringList(*iterString)));
            }

            for (iterChild = itemChild.constBegin();iterChild != itemChild.constEnd();iterChild ++) {
                rfChildChild.clear();
                if ((*iterChild)->text(0).contains("Test")) {
                    if ((*iterRoot)->text(0).contains("R1F")) {
                        rfChildChild.clear();
                    } else if ((*iterRoot)->text(0).contains("R1C/D/E")) {
                        rfChildChild.clear();
                        rfChildChild.append(QString("TX-FreqResponse"));
                        rfChildChild.append(QString("RX-FreqResponse"));
                        rfChildChild.append(QString("TX-PowTest"));
                        rfChildChild.append(QString("TX-FreqTest"));
                        rfChildChild.append(QString("RX-RefTest"));
                        rfChildChild.append(QString("RX-FreqTest"));
                        rfChildChild.append(QString("Blower-MachineTemp"));
                        rfChildChild.append(QString("RFTemp-TxPower"));
                    } else if ((*iterRoot)->text(0).contains("R1A/B")) {
                        rfChildChild.clear();
                        rfChildChild.append(QString("BoardStability"));
                        rfChildChild.append(QString("TX-FreqResponse"));
                        rfChildChild.append(QString("RX-FreqResponse"));
                    }
                } else if ((*iterChild)->text(0).contains("Calibration")) {
                    if ((*iterRoot)->text(0).contains("R1F") || (*iterRoot)->text(0).contains("R1C/D/E")) {
                        rfChildChild.clear();
                        rfChildChild.append(QString("TX-LOLeakage"));
                        rfChildChild.append(QString("TX-Sideband"));
                        rfChildChild.append(QString("TX-Filter"));
                        rfChildChild.append(QString("TX-BasePower"));
                        rfChildChild.append(QString("TX-Attenuation"));
                        rfChildChild.append(QString("TX-FilterOffset"));
                        rfChildChild.append(QString("RX-Filter"));
                        rfChildChild.append(QString("RX-Reference"));
                        rfChildChild.append(QString("RX-Attenuation"));
                        rfChildChild.append(QString("RX-FilterOffset"));
                    } else if ((*iterRoot)->text(0).contains("R1A/B")) {
                        rfChildChild.clear();
                        rfChildChild.append(QString("TX-LOLeakage"));
                        rfChildChild.append(QString("TX-Sideband"));
                        rfChildChild.append(QString("TX-Attenuation"));
                        rfChildChild.append(QString("TX-BasePower"));
                        rfChildChild.append(QString("RX-SGPMMTable"));
                        rfChildChild.append(QString("RX-Reference"));
                    }
                }

                itemChildChild.clear();
                for (iterString = rfChildChild.constBegin();iterString != rfChildChild.constEnd();iterString ++) {
                    itemChildChild.append(new QTreeWidgetItem(*iterChild,QStringList(*iterString)));
                }
            }
        } else if ((*iterRoot)->text(0).left(2) == "BB") {
            itemChild.clear();
            for (iterString = bbChild.constBegin();iterString != bbChild.constEnd();iterString ++) {
                itemChild.append(new QTreeWidgetItem(*iterRoot,QStringList(*iterString)));
            }
        }
    }

    ui->mainTree->expandAll();
    for (int i = 0;i < itemRoot.size();i ++) {
        if (i >= 2) {
            itemRoot.at(i)->setExpanded(false);
        }
    }
}

void MainWindow::initMainTabWidget()
{
    ui->mainTab->setStyleSheet(
                "QTabBar::tab {height:20px; width:120px;}"
                "QTabBar::tab:selected {background:rgb(0,255,0)}");

    QHBoxLayout *mainTabLayout = new QHBoxLayout;
    mainTabLayout->addLayout(childDlgLayout);
    ui->mainTab->setLayout(mainTabLayout);
}

void MainWindow::initMsgLogDlg()
{
    msgLogModel = new QMsgLogModel;
    ui->msgTableView->setModel(msgLogModel);
    ui->msgTableView->setColumnWidth(0,125);
    ui->msgTableView->setColumnWidth(1,350);
    ui->msgTableView->setColumnWidth(2,50);
    ui->msgTableView->setColumnWidth(3,80);
    ui->msgTableView->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
    ui->msgTableView->verticalHeader()->setDefaultSectionSize(18);

    ui->msgTableView->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Fixed);
    ui->msgTableView->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch);
    ui->msgTableView->horizontalHeader()->setSectionResizeMode(2,QHeaderView::Fixed);
    ui->msgTableView->horizontalHeader()->setSectionResizeMode(3,QHeaderView::Fixed);

    connect(this,SIGNAL(addMsgList(int)),msgLogModel,SLOT(update(int)));
    connect(this,SIGNAL(addMsgList(int)),this,SLOT(updateMsgTable(int)));
}

void MainWindow::initRegLogDlg()
{
    regLogModel = new QRegLogModel;
    ui->regTableView->setModel(regLogModel);
    ui->regTableView->setColumnWidth(0,68);
    ui->regTableView->setColumnWidth(1,68);
    ui->regTableView->setColumnWidth(2,60);
    ui->regTableView->setColumnWidth(3,85);
    ui->regTableView->setColumnWidth(4,85);
    ui->regTableView->setColumnWidth(5,50);
    ui->regTableView->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
    ui->regTableView->verticalHeader()->setDefaultSectionSize(18);
    ui->regTableView->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Fixed);
    ui->regTableView->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Fixed);
    ui->regTableView->horizontalHeader()->setSectionResizeMode(2,QHeaderView::Fixed);
    ui->regTableView->horizontalHeader()->setSectionResizeMode(3,QHeaderView::Fixed);
    ui->regTableView->horizontalHeader()->setSectionResizeMode(4,QHeaderView::Fixed);
    ui->regTableView->horizontalHeader()->setSectionResizeMode(5,QHeaderView::Stretch);

    connect(this,SIGNAL(addRegList(int)),regLogModel,SLOT(update(int)));
    connect(this,SIGNAL(addRegList(int)),this,SLOT(updateRegTable(int)));
}

void MainWindow::updateParamInChildDlg()
{
#define UPDATE_CAL_PARAM_IN(dlg) \
    for (quint8 i = 0;i < MAX_RF;i ++) { \
        dlg[i]->SP1401 = _sp3301->get_sp1401(i); \
        dlg[i]->SP2401 = _sp3301->get_sp2401(i); \
        dlg[i]->SP3301 = _sp3301; \
    }

#define UPDATE_TEST_PARAM_IN(dlg,daughterDlg) \
    for (quint8 i = 0;i < MAX_RF;i ++) { \
        dlg[i]->daughterDlg->SP1401 = _sp3301->get_sp1401(i); \
        dlg[i]->daughterDlg->SP2401 = _sp3301->get_sp2401(i); \
}

    UPDATE_CAL_PARAM_IN(dlgCalR1C);
    UPDATE_CAL_PARAM_IN(dlgCalR1CTXLOLeak);
    UPDATE_CAL_PARAM_IN(dlgCalR1CTXSB);
    UPDATE_CAL_PARAM_IN(dlgCalR1CTXFilter);
    UPDATE_CAL_PARAM_IN(dlgCalR1CTXPwr);
    UPDATE_CAL_PARAM_IN(dlgCalR1CTXAtt);
    UPDATE_CAL_PARAM_IN(dlgCalR1CTXFilterOffset);
    UPDATE_CAL_PARAM_IN(dlgCalR1CRXFilter);
    UPDATE_CAL_PARAM_IN(dlgCalR1CRXRef);
    UPDATE_CAL_PARAM_IN(dlgCalR1CRXAtt);
    UPDATE_CAL_PARAM_IN(dlgCalR1CRXFilterOffset);

    UPDATE_TEST_PARAM_IN(dlgTestR1C,dlgFreqRes);
    UPDATE_TEST_PARAM_IN(dlgTestR1C,dlgRF);
    UPDATE_TEST_PARAM_IN(dlgTestR1C,dlgTxTempPwr);
    UPDATE_TEST_PARAM_IN(dlgTestR1C,dlgRxTempPwr);

    dlgTempCtrl->_sp3301 = _sp3301;
    dlgTempCtrl->_sp1401_r1c = _sp3301->get_sp1401_r1c(tabIdxRf);
    dlgFPGA->_sp2401[0] = _sp3301->get_sp2401(0);
    dlgFPGA->_sp2401[1] = _sp3301->get_sp2401(2);
    for (quint8 i = 0;i < MAX_RF;i ++) {
        dlgRFR1CContainer[i]->dlg->setSP3301(_sp3301);
        dlgRFR1CContainer[i]->dlg->setRFIdx(i);
    }
}

QString MainWindow::rfIdx2RFTabName(int idx)
{
    return QString("RF-%1").arg(idx);
}

QString MainWindow::rfIdx2BBTabName(int idx)
{
    return QString("k7-%1--->RF%2").arg((MAX_RF - 1 - idx)/2).arg(idx);
}

void MainWindow::initProg(const QString name)
{
    QWinThread::g_threadName = name;
    labelProgressName->setText(name);
}

void MainWindow::initProg(const QString name, quint32 pts)
{
    initProg(name);
    mainProgressBar->setRange(0,int(pts));
    mainProgressBar->setValue(0);
}

void MainWindow::setProgPos(quint32 pos)
{
    mainProgressBar->setValue(int(pos));
}

void MainWindow::addProgPos(quint32 off)
{
    mainProgressBar->setValue(mainProgressBar->value() + int(off));
}

void MainWindow::setProgClr(const QColor clr)
{
    QString ss = QString("QProgressBar::chunk{background-color:rgb(%1,%2,%3)}")
                        .arg(clr.red())
                        .arg(clr.green())
                        .arg(clr.blue());
    mainProgressBar->setStyleSheet(mainProgressBar->styleSheet() + ss);
}

void MainWindow::updateMsgTable(int row)
{
    ui->msgTableView->scrollToBottom();
    ui->msgTableView->selectRow(row - 1);
}

void MainWindow::updateRegTable(int row)
{
    ui->regTableView->scrollToBottom();
    ui->regTableView->selectRow(row - 1);
}

void MainWindow::threadProcess(const QWinThread::Process p)
{
    if (p == QWinThread::PREPARE) {
        setProgClr(Qt::darkGreen);
    } else if (p == QWinThread::STARTED) {
        setProgClr(Qt::green);
    } else if (p == QWinThread::RUNNING_EXCEPT || p == QWinThread::END_EXCEPT) {
        setProgClr(Qt::red);
    } else if (p == QWinThread::PAUSED) {
        setProgClr(Qt::yellow);
    }
}

void MainWindow::on_mainTree_itemClicked(QTreeWidgetItem *item, int column)
{
#define ADD_RF_TAB(dlg) \
    for (quint8 i = 0;i < MAX_RF;i ++) { \
        ui->mainTab->addTab(dlg[i],rfIdx2RFTabName(i)); \
    }

#define ADD_BB_TAB(dlg) \
    for (quint8 i = 0;i < MAX_RF;i ++) { \
        ui->mainTab->addTab(dlg[i],rfIdx2BBTabName(i)); \
    }

    QString strItem = item->text(column);
    QString strParent;
    QString strParentParent;

    if (item->parent() != nullptr) {
        strParent = item->parent()->text(0);
        if (item->parent()->parent() != nullptr) {
            strParentParent = item->parent()->parent()->text(0);
        }
    }

    disconnect(ui->mainTab,SIGNAL(currentChanged(int)),this,SLOT(on_mainTab_currentChanged(int)));
    ui->mainTab->clear();

    emit tabIdxChanged(-1);

    if (strItem == tr("Overview")) {
        if (strParent == ("RF-R1F"))          {ADD_RF_TAB(dlgRFR1FContainer);}
        else if (strParent == ("RF-R1C/D/E")) {ADD_RF_TAB(dlgRFR1CContainer);}
        else if (strParent == ("RF-R1A/B"))   {ADD_RF_TAB(dlgRFR1AContainer);}
        else if (strParent == ("BB-Debug"))   {ADD_BB_TAB(dlgBB);}
    } else if (strItem == tr("Advance")) {
        if (strParent == ("RF-R1F"))          {ADD_RF_TAB(dlgRFR1FAdv);}
        else if (strParent == ("RF-R1C/D/E")) {ADD_RF_TAB(dlgRFR1CAdv);}
        else if (strParent == ("RF-R1A/B"))   {ADD_RF_TAB(dlgRFR1AAdv);}
    } else if (strItem == tr("Arb")) {        {ADD_RF_TAB(dlgArb);}
    } else if (strItem == tr("IQ-Capture")) { {ADD_RF_TAB(dlgIQCap);}
    } else if (strItem == tr("FPGA")) {
        ui->mainTab->addTab(dlgFPGA,tr("FPGA"));
    } else if (strItem == tr("Calibraton")) {
        if (strParentParent == "RF-R1F")          { }
        else if (strParentParent == "RF-R1C/D/E") {ADD_RF_TAB(dlgCalR1C);}
        else if (strParentParent == "RF-R1A/B")   { }
    } else if (strItem == tr("TX-LOLeakage")) {
        if (strParentParent == "RF-R1F")          {ADD_RF_TAB(dlgCalR1CTXLOLeak);}
        else if (strParentParent == "RF-R1C/D/E") {ADD_RF_TAB(dlgCalR1CTXLOLeak);}
        else if (strParentParent == "RF-R1A/B")   { }
    } else if (strItem == tr("TX-Sideband")) {
        if (strParentParent == "RF-R1F")          {ADD_RF_TAB(dlgCalR1CTXSB);}
        else if (strParentParent == "RF-R1C/D/E") {ADD_RF_TAB(dlgCalR1CTXSB);}
        else if (strParentParent == "RF-R1A/B")   { }
    } else if (strItem == tr("TX-Filter")) {
        if (strParentParent == "RF-R1F")          {ADD_RF_TAB(dlgCalR1CTXFilter);}
        else if (strParentParent == "RF-R1C/D/E") {ADD_RF_TAB(dlgCalR1CTXFilter);}
    } else if (strItem == tr("TX-BasePower")) {
        if (strParentParent == "RF-R1F")          {ADD_RF_TAB(dlgCalR1CTXPwr);}
        else if (strParentParent == "RF-R1C/D/E") {ADD_RF_TAB(dlgCalR1CTXPwr);}
        else if (strParentParent == "RF-R1A/B")   { }
    } else if (strItem == tr("TX-Attenuation")) {
        if (strParentParent == "RF-R1F")          {ADD_RF_TAB(dlgCalR1CTXAtt);}
        else if (strParentParent == "RF-R1C/D/E") {ADD_RF_TAB(dlgCalR1CTXAtt);}
        else if (strParentParent == "RF-R1A/B")   { }
    } else if (strItem == tr("TX-FilterOffset")) {
        if (strParentParent == "RF-R1F")          {ADD_RF_TAB(dlgCalR1CTXFilterOffset);}
        else if (strParentParent == "RF-R1C/D/E") {ADD_RF_TAB(dlgCalR1CTXFilterOffset);}
        else if (strParentParent == "RF-R1A/B")   { }
    } else if (strItem == tr("RX-Filter")) {
        if (strParentParent == "RF-R1F")          {ADD_RF_TAB(dlgCalR1CRXFilter);}
        else if (strParentParent == "RF-R1C/D/E") {ADD_RF_TAB(dlgCalR1CRXFilter);}
        else if (strParentParent == "RF-R1A/B")   { }
    } else if (strItem == tr("RX-Reference")) {
        if (strParentParent == "RF-R1F")          {ADD_RF_TAB(dlgCalR1CRXRef);}
        else if (strParentParent == "RF-R1C/D/E") {ADD_RF_TAB(dlgCalR1CRXRef);}
        else if (strParentParent == "RF-R1A/B")   { }
    } else if (strItem == tr("RX-Attenuation")) {
        if (strParentParent == "RF-R1F")          {ADD_RF_TAB(dlgCalR1CRXAtt);}
        else if (strParentParent == "RF-R1C/D/E") {ADD_RF_TAB(dlgCalR1CRXAtt);}
        else if (strParentParent == "RF-R1A/B")   { }
    } else if (strItem == tr("RX-FilterOffset")) {
        if (strParentParent == "RF-R1F")          {ADD_RF_TAB(dlgCalR1CRXFilterOffset);}
        else if (strParentParent == "RF-R1C/D/E") {ADD_RF_TAB(dlgCalR1CRXFilterOffset);}
        else if (strParentParent == "RF-R1A/B")   { }
    } else if (strItem == tr("Test")) {
        if (strParent == "RF-R1F") { ADD_RF_TAB(dlgTestR1C); }
    }

    connect(ui->mainTab,SIGNAL(currentChanged(int)),this,SLOT(on_mainTab_currentChanged(int)));
    ui->mainTab->setCurrentIndex(int(tabIdxRf));
}

void MainWindow::on_mainTab_currentChanged(int index)
{
    if (index == -1) {
        return;
    }

    ui->mainTab->setCurrentIndex(index);

    if (dlgIQCap[index]->isVisible()) {
        emit tabIdxChanged(index);
    } else {
        emit tabIdxChanged(-1);
    }

    tabIdxRf = qint8(index);
    updateParamInChildDlg();
}
