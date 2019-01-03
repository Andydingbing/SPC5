#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMetaType>
#include <QMessageBox>
#include <QStackedLayout>
#include "qdeviceinitthread.h"
#include "qiqcapthread.h"

MainWindow *g_MainW = NULL;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    qRegisterMetaType<sp3301::rfu_info_t>("sp3301::rfu_info_t");
    qRegisterMetaType<cal_file::cal_item_t>("cal_file::cal_item_t");

    tabIdxRf = 0;
    tabIdxDMA = 0;
    tabIdxRfu = 0;

    _sp3501 = &SP3501;
    _sp3301 = &SP3301_2;

    mainProgress = new QProgressBar(ui->statusBar);
    mainProgress->setTextVisible(false);
    mainProgress->setValue(0);
    mainProgress->setFixedHeight(15);

    labelProgressName = new QLabel(QString("..."));
    labelVerK7_0 = new QLabel(QString("K7_0:0x00000000"));
    labelVerK7_1 = new QLabel(QString("K7_1:0x00000000"));
    labelVerS6 = new QLabel(QString("S6:0x00000000"));
    labelVerDriver = new QLabel(QString("Driver:0.0.00000"));

    ui->statusBar->clearMessage();
    ui->statusBar->addWidget(labelProgressName);
    ui->statusBar->addWidget(mainProgress,1);
    ui->statusBar->addPermanentWidget(labelVerK7_0);
    ui->statusBar->addPermanentWidget(labelVerK7_1);
    ui->statusBar->addPermanentWidget(labelVerS6);
    ui->statusBar->addPermanentWidget(labelVerDriver);

    ui->mainTree->setHeaderHidden(true);
    ui->mainTree->setRootIsDecorated(true);
    ui->mainTree->clear();
    ui->mainTree->setColumnCount(1);
    ui->mainTree->setStyleSheet("QTreeWidget {background:rgb(179,217,255)}"
                                "QTreeWidget::item:selected {background:rgb(0,255,0);color:black;}");
    ui->mainTab->setStyleSheet("QTabBar::tab {height:20px; width:120px;}"
                               "QTabBar::tab:selected {background:rgb(0,255,0)}");

    QStringList strRoot;
    QStringList strChild;
    QStringList::ConstIterator constIter;
    strRoot.append(QString("RF Debug"));
    strRoot.append(QString("BB Debug"));
    strRoot.append(QString("Calibration(R1A/B)"));
    strRoot.append(QString("Calibration(R1C/D)"));
    strRoot.append(QString("Test(R1A/B)"));
    strRoot.append(QString("Test(R1C/D)"));

    QList<QTreeWidgetItem *> itemRoot;
    QList<QTreeWidgetItem *> itemChild_0;
    QList<QTreeWidgetItem *> itemChild_1;
    for (constIter = strRoot.constBegin();constIter != strRoot.constEnd();constIter ++) {
        itemRoot.append(new QTreeWidgetItem(ui->mainTree,QStringList(* constIter)));
        itemRoot.back()->setExpanded(true);
    }

    strChild.clear();
    strChild.append(QString("R1A/B"));
    strChild.append(QString("R1C/D"));
    strChild.append(QString("Arb"));
    strChild.append(QString("IQ Capture"));
    itemChild_0.clear();
    for (constIter = strChild.constBegin();constIter != strChild.constEnd();constIter ++) {
        itemChild_0.append(new QTreeWidgetItem(itemRoot.at(0),QStringList(*constIter)));
        itemChild_0.back()->setExpanded(true);
    }
    strChild.clear();
    strChild.append(QString("Overview"));
    strChild.append(QString("Advance"));
    itemChild_1.clear();
    for (constIter = strChild.constBegin();constIter != strChild.constEnd();constIter ++)
        itemChild_1.append(new QTreeWidgetItem(itemChild_0.at(0),QStringList(*constIter)));
    itemChild_1.clear();
    for (constIter = strChild.constBegin();constIter != strChild.constEnd();constIter ++)
        itemChild_1.append(new QTreeWidgetItem(itemChild_0.at(1),QStringList(*constIter)));

    strChild.clear();
    strChild.append(QString("Overview"));
    strChild.append(QString("Arb"));
    strChild.append(QString("IQ Capture"));
    strChild.append(QString("DMA"));
    strChild.append(QString("FPGA"));
    itemChild_0.clear();
    for (constIter = strChild.constBegin();constIter != strChild.constEnd();constIter ++)
        itemChild_0.append(new QTreeWidgetItem(itemRoot.at(1),QStringList(*constIter)));

    strChild.clear();
    strChild.append(QString("TxLOLeakage"));
    strChild.append(QString("TxSideband"));
    strChild.append(QString("TxAttenuation"));
    strChild.append(QString("TxBasePower"));
    strChild.append(QString("RxSGPMMTable"));
    strChild.append(QString("RxReference"));
    itemChild_0.clear();
    for (constIter = strChild.constBegin();constIter != strChild.constEnd();constIter ++)
        itemChild_0.append(new QTreeWidgetItem(itemRoot.at(2),QStringList(*constIter)));

    strChild.clear();
    strChild.append(QString("TxLOLeakage"));
    strChild.append(QString("TxSideband"));
    strChild.append(QString("TxCompensateFilter"));
    strChild.append(QString("TxBasePower"));
    strChild.append(QString("TxAttenuation"));
    strChild.append(QString("TxFilterOffset"));
    strChild.append(QString("RxCompensateFilter"));
    strChild.append(QString("RxReference"));
    strChild.append(QString("RxAttenuation"));
    strChild.append(QString("RxFilterOffset"));
    itemChild_0.clear();
    for (constIter = strChild.constBegin();constIter != strChild.constEnd();constIter ++)
        itemChild_0.append(new QTreeWidgetItem(itemRoot.at(3),QStringList(*constIter)));

    strChild.clear();
    strChild.append(QString("BoardStability"));
    strChild.append(QString("TxFreqResponse"));
    strChild.append(QString("RxFreqResponse"));
    itemChild_0.clear();
    for (constIter = strChild.constBegin();constIter != strChild.constEnd();constIter ++)
        itemChild_0.append(new QTreeWidgetItem(itemRoot.at(4),QStringList(*constIter)));

    strChild.clear();
    strChild.append(QString("TxFreqResponse"));
    strChild.append(QString("RxFreqResponse"));
    strChild.append(QString("Blower-MachineTemp"));
    strChild.append(QString("RFTemp-TxPower"));
    itemChild_0.clear();
    for (constIter = strChild.constBegin();constIter != strChild.constEnd();constIter ++)
        itemChild_0.append(new QTreeWidgetItem(itemRoot.at(5),QStringList(*constIter)));

    QSizePolicy childDlgPolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    QHBoxLayout *childDlgLayout = new QHBoxLayout;
    QHBoxLayout *mainTabLayout = new QHBoxLayout;


#define ADD_CHILD_DLG(dlg,class_name)                       \
    dlg = new class_name(ui->mainTab);                      \
    dlg->setSizePolicy(childDlgPolicy);                     \
    dlg->setVisible(false);                                 \
    childDlgLayout->addWidget(dlg);

#define ADD_CHILD_DLG__RF_DLG(dlg,class_name)               \
    ADD_CHILD_DLG(dlg,class_name);                          \
    dlg->setSP3301(_sp3301);                                \
    dlg->setRfIdx(quint8(i));


    for (qint8 i = 0;i < MAX_RF;i ++) {
        ADD_CHILD_DLG(dlgRfR1AContainer[i],QRfR1AContainerDlg);
        ADD_CHILD_DLG(dlgRfR1CContainer[i],QRfR1CContainerDlg);
        dlgRfR1AContainer[i]->dlgRfR1A->setSP3301(_sp3301);
        dlgRfR1AContainer[i]->dlgRfR1A->setRfIdx(i);
        dlgRfR1CContainer[i]->dlgRfR1C->setSP3301(_sp3301);
        dlgRfR1CContainer[i]->dlgRfR1C->setRfIdx(i);

        ADD_CHILD_DLG__RF_DLG(dlgRfR1AAdv[i],QRfR1AAdvDlg);
        ADD_CHILD_DLG__RF_DLG(dlgRfR1CAdv[i],QRfR1CAdvDlg);
        ADD_CHILD_DLG__RF_DLG(dlgRfArb[i],QArbDlg);
        ADD_CHILD_DLG__RF_DLG(dlgBbArb[i],QArbDlg);
        ADD_CHILD_DLG__RF_DLG(dlgRfIQCap[i],QIQCapDlg);
        ADD_CHILD_DLG__RF_DLG(dlgBbIQCap[i],QIQCapDlg);
        ADD_CHILD_DLG__RF_DLG(dlgBb[i],QBbDlg);
        ADD_CHILD_DLG(dlgCalR1CTxLOLeak[i],QCalR1CTxLOLeakDlg);
        ADD_CHILD_DLG(dlgCalR1CTxSB[i],QCalR1CTxSBDlg);
        ADD_CHILD_DLG(dlgCalR1CTxFilter[i],QCalR1CTxFilterDlg);
        ADD_CHILD_DLG(dlgCalR1CTxPwr[i],QCalR1CTxPwrDlg);
        ADD_CHILD_DLG(dlgCalR1CTxAtt[i],QCalR1CTxAttDlg);
        ADD_CHILD_DLG(dlgCalR1CTxFilterOffset[i],QCalR1CTxFilterOffsetDlg);
        ADD_CHILD_DLG(dlgCalR1CRxFilter[i],QCalR1CRxFilterDlg);
        ADD_CHILD_DLG(dlgCalR1CRxRef[i],QCalR1CRxRefDlg);
        ADD_CHILD_DLG(dlgCalR1CRxAtt[i],QCalR1CRxAttDlg);
        ADD_CHILD_DLG(dlgCalR1CRxFilterOffset[i],QCalR1CRxFilterOffsetDlg);

        connect(this,SIGNAL(sp3301Changed()),dlgCalR1CTxLOLeak[i],SLOT(sp3301Change()));
        connect(this,SIGNAL(sp3301Changed()),dlgCalR1CTxSB[i],SLOT(sp3301Change()));
        connect(this,SIGNAL(sp3301Changed()),dlgCalR1CTxFilter[i],SLOT(sp3301Change()));
        connect(this,SIGNAL(sp3301Changed()),dlgCalR1CTxPwr[i],SLOT(sp3301Change()));
        connect(this,SIGNAL(sp3301Changed()),dlgCalR1CTxAtt[i],SLOT(sp3301Change()));
        connect(this,SIGNAL(sp3301Changed()),dlgCalR1CTxFilterOffset[i],SLOT(sp3301Change()));
        connect(this,SIGNAL(sp3301Changed()),dlgCalR1CRxFilter[i],SLOT(sp3301Change()));
        connect(this,SIGNAL(sp3301Changed()),dlgCalR1CRxRef[i],SLOT(sp3301Change()));
        connect(this,SIGNAL(sp3301Changed()),dlgCalR1CRxAtt[i],SLOT(sp3301Change()));
        connect(this,SIGNAL(sp3301Changed()),dlgCalR1CRxFilterOffset[i],SLOT(sp3301Change()));
    }
    ADD_CHILD_DLG(dlgFpga,QFPGADlg);

    mainTabLayout->addLayout(childDlgLayout);
    ui->mainTab->setLayout(mainTabLayout);

    updateParamInChildDlg();

    msgLogModel = new QMsgLogModel;
    ui->msgTableView->setModel(msgLogModel);
    ui->msgTableView->setColumnWidth(0,125);
    ui->msgTableView->setColumnWidth(1,350);
    ui->msgTableView->setColumnWidth(2,50);
    ui->msgTableView->setColumnWidth(3,80);
    ui->msgTableView->horizontalHeader()->setFixedHeight(20);
    ui->msgTableView->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
    ui->msgTableView->verticalHeader()->setDefaultSectionSize(18);

#if (QT_VERSION < QT_VERSION_CHECK(5,0,0))
    #define setSectionResizeMode setResizeMode
#endif

    ui->msgTableView->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Fixed);
    ui->msgTableView->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch);
    ui->msgTableView->horizontalHeader()->setSectionResizeMode(2,QHeaderView::Fixed);
    ui->msgTableView->horizontalHeader()->setSectionResizeMode(3,QHeaderView::Fixed);

    regLogModel = new QRegLogModel;
    ui->regTableView->setModel(regLogModel);
    ui->regTableView->setColumnWidth(0,68);
    ui->regTableView->setColumnWidth(1,68);
    ui->regTableView->setColumnWidth(2,60);
    ui->regTableView->setColumnWidth(3,85);
    ui->regTableView->setColumnWidth(4,85);
    ui->regTableView->setColumnWidth(5,50);
    ui->regTableView->horizontalHeader()->setFixedHeight(20);
    ui->regTableView->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
    ui->regTableView->verticalHeader()->setDefaultSectionSize(18);
    ui->regTableView->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Fixed);
    ui->regTableView->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Fixed);
    ui->regTableView->horizontalHeader()->setSectionResizeMode(2,QHeaderView::Fixed);
    ui->regTableView->horizontalHeader()->setSectionResizeMode(3,QHeaderView::Fixed);
    ui->regTableView->horizontalHeader()->setSectionResizeMode(4,QHeaderView::Fixed);
    ui->regTableView->horizontalHeader()->setSectionResizeMode(5,QHeaderView::Fixed);

    connect(this,SIGNAL(addMsgList(int)),msgLogModel,SLOT(update(int)));
    connect(this,SIGNAL(addRegList(int)),regLogModel,SLOT(update(int)));
    connect(this,SIGNAL(addMsgList(int)),this,SLOT(updateMsgTable(int)));
    connect(this,SIGNAL(addRegList(int)),this,SLOT(updateRegTable(int)));

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
    connect(thread,SIGNAL(swhwVerReady(sp3301::rfu_info_t,const char*)),this,SLOT(showSwHwVer(sp3301::rfu_info_t,const char*)));
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
    Log.add_msg_list(0,"working on rfu0");
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
    Log.add_msg_list(0,"working on rfu1");
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
    Log.add_msg_list(0,"working on rfu2");
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
    Log.add_msg_list(0,"working on rfu3");
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
    Log.add_msg_list(0,"working on rfu4");
}

void MainWindow::starAllIQCapture()
{
    THREAD_EXISTED_CHK();
    QIQCapThread::Param param;
    for (int i = 0;i < MAX_RF;i ++) {
        param._sp1401[i] = _sp3301->m_sp1401->at(i).get();
        if (dlgRfIQCap[i]->isVisible() | dlgBbIQCap[i]->isVisible())
            param.visible[i] = true;
        else
            param.visible[i] = false;
    }
    QWinThread::g_threadThread = new QIQCapThread(param,this);
    QIQCapThread *pThread = (QIQCapThread *)(QWinThread::g_threadThread);
    for (int i = 0;i < MAX_RF;i ++) {
        connect(pThread,SIGNAL(updatePlot()),dlgRfIQCap[i],SLOT(updatePlot()),Qt::BlockingQueuedConnection);
        connect(pThread,SIGNAL(updatePlot()),dlgBbIQCap[i],SLOT(updatePlot()),Qt::BlockingQueuedConnection);
    }
    connect(this,SIGNAL(tabIdxChanged(int)),pThread,SLOT(tabIdxChanged(int)));
    QWinThread::g_threadThread->start();
}

void MainWindow::stopAllIQCapture()
{

}

void MainWindow::showSwHwVer(const sp3301::rfu_info_t &info, const char *driver)
{
    labelVerK7_0->setText(QString("K7_0:0x%1").arg(info.k7_0_ver,8,16));
    labelVerK7_1->setText(QString("K7_1:0x%1").arg(info.k7_1_ver,8,16));
    labelVerS6->setText(QString("S6:0x%1").arg(info.s6_ver,8,16));
    labelVerDriver->setText(QString("Driver:%1").arg(driver));
}

void MainWindow::updateParamInChildDlg()
{
    for (int8_t i = 0;i < MAX_RF;i ++) {
        dlgCalR1CTxLOLeak[i]->_sp1401 = _sp3301->get_sp1401_r1c(i);
        dlgCalR1CTxLOLeak[i]->_sp2401 = _sp3301->get_sp2401(i);
        dlgCalR1CTxLOLeak[i]->_sp3501 = &SP3501;
        dlgCalR1CTxSB[i]->_sp1401 = _sp3301->get_sp1401_r1c(i);
        dlgCalR1CTxSB[i]->_sp2401 = _sp3301->get_sp2401(i);
        dlgCalR1CTxFilter[i]->_sp1401 = _sp3301->get_sp1401_r1c(i);
        dlgCalR1CTxFilter[i]->_sp2401 = _sp3301->get_sp2401(i);
        dlgCalR1CTxPwr[i]->_sp1401 = _sp3301->get_sp1401_r1c(i);
        dlgCalR1CTxPwr[i]->_sp2401 = _sp3301->get_sp2401(i);
        dlgCalR1CTxAtt[i]->_sp1401 = _sp3301->get_sp1401_r1c(i);
        dlgCalR1CTxAtt[i]->_sp2401 = _sp3301->get_sp2401(i);
        dlgCalR1CTxFilterOffset[i]->_sp1401 = _sp3301->get_sp1401_r1c(i);
        dlgCalR1CTxFilterOffset[i]->_sp2401 = _sp3301->get_sp2401(i);
        dlgCalR1CRxFilter[i]->_sp1401 = _sp3301->get_sp1401_r1c(i);
        dlgCalR1CRxFilter[i]->_sp2401 = _sp3301->get_sp2401(i);
        dlgCalR1CRxRef[i]->_sp1401 = _sp3301->get_sp1401_r1c(i);
        dlgCalR1CRxRef[i]->_sp2401 = _sp3301->get_sp2401(i);
        dlgCalR1CRxAtt[i]->_sp1401 = _sp3301->get_sp1401_r1c(i);
        dlgCalR1CRxAtt[i]->_sp2401 = _sp3301->get_sp2401(i);
        dlgCalR1CRxFilterOffset[i]->_sp1401 = _sp3301->get_sp1401_r1c(i);
        dlgCalR1CRxFilterOffset[i]->_sp2401 = _sp3301->get_sp2401(i);
    }
    dlgTempCtrl->_sp3501 = &SP3501;
    dlgFpga->_sp2401[0] = _sp3301->get_sp2401(0);
    dlgFpga->_sp2401[1] = _sp3301->get_sp2401(2);

    emit sp3301Changed();
}

void MainWindow::addMsgListCallback()
{
    int row = Log.get_msg_log()->size();
    emit addMsgList(row);
}

void MainWindow::addRegListCallback()
{
    int row = Log.get_reg_log()->size();
    emit addRegList(row);
}

QString MainWindow::rfIdx2RfTabName(int idx)
{
    return QString("RF-%1").arg(idx);
}

QString MainWindow::rfIdx2BbTabName(int idx)
{
    return QString("k7-%1--->RF%2").arg((MAX_RF - 1 - idx)/2).arg(idx);
}

void MainWindow::initProg(const QString name, int pts)
{
    QWinThread::g_threadName = name;
    labelProgressName->setText(name + " %0");
    mainProgress->setRange(0,pts);
    mainProgress->setValue(0);
}

void MainWindow::setProgPos(int pos)
{
    int iRange = mainProgress->maximum();
    if (pos > iRange)
        return;
    QString strProgName = labelProgressName->text();
    QString strPercent = QString("%%1").arg(double(pos) / double(iRange) * 100.0,0,'f',2);
    int iPercentPos = strProgName.indexOf("%",0);
    strProgName.replace(iPercentPos,strProgName.length() - iPercentPos,strPercent);
    labelProgressName->setText(strProgName);
    mainProgress->setValue(pos);
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

void MainWindow::on_mainTree_itemClicked(QTreeWidgetItem *item, int column)
{
    QString strItem = item->text(column);

    disconnect(ui->mainTab,SIGNAL(currentChanged(int)),this,SLOT(on_mainTab_currentChanged(int)));
    ui->mainTab->clear();

    emit tabIdxChanged(-1);
    if (strItem == QString("Overview")) {
        if (item->parent()->text(0) == "R1A/B") {
            for (qint8 i = 0;i < MAX_RF;i ++)
                ui->mainTab->addTab(dlgRfR1AContainer[i],rfIdx2RfTabName(i));
        }
        else if (item->parent()->text(0) == "R1C/D") {
            for (qint8 i = 0;i < MAX_RF;i ++)
                ui->mainTab->addTab(dlgRfR1CContainer[i],rfIdx2RfTabName(i));
        }
        else if (item->parent()->text(0) == "BB Debug") {
            for (qint8 i = 0;i < MAX_RF;i ++)
                ui->mainTab->addTab(dlgBb[i],rfIdx2BbTabName(i));
        }
    }
    else if (strItem == QString("Advance")) {
        if (item->parent()->text(0) == "R1A/B") {
            for (qint8 i = 0;i < MAX_RF;i ++)
                ui->mainTab->addTab(dlgRfR1AAdv[i],rfIdx2RfTabName(i));
        }
        if (item->parent()->text(0) == "R1C/D") {
            for (qint8 i = 0;i < MAX_RF;i ++)
                ui->mainTab->addTab(dlgRfR1CAdv[i],rfIdx2RfTabName(i));
        }
    }
    else if (strItem == QString("Arb")) {
        if (item->parent()->text(0) == "RF Debug")
            for (qint8 i = 0;i < MAX_RF;i ++) {
                ui->mainTab->addTab(dlgRfArb[i],rfIdx2RfTabName(i));
        }
        else if (item->parent()->text(0) == "BB Debug")
            for (qint8 i = 0;i < MAX_RF;i ++) {
                ui->mainTab->addTab(dlgBbArb[i],rfIdx2BbTabName(i));
        }
    }
    else if (strItem == QString("IQ Capture")) {
        if (item->parent()->text(0) == "RF Debug")
            for (qint8 i = 0;i < MAX_RF;i ++) {
                ui->mainTab->addTab(dlgRfIQCap[i],rfIdx2RfTabName(i));
            on_mainTab_currentChanged(int(tabIdxRf));
        }
        else if (item->parent()->text(0) == "BB Debug") {
            for (qint8 i = 0;i < MAX_RF;i ++)
                ui->mainTab->addTab(dlgBbIQCap[i],rfIdx2BbTabName(i));
            on_mainTab_currentChanged(int(tabIdxRf));
        }
    }
    else if (strItem == tr("TxLOLeakage")) {
        if (item->parent()->text(0) == "Calibration(R1C/D)") {
            for (qint8 i = 0;i < MAX_RF;i ++)
                ui->mainTab->addTab(dlgCalR1CTxLOLeak[i],rfIdx2RfTabName(i));
        }
    }
    else if (strItem == tr("TxSideband")) {
        if (item->parent()->text(0) == "Calibration(R1C/D)") {
            for (qint8 i = 0;i < MAX_RF;i ++)
                ui->mainTab->addTab(dlgCalR1CTxSB[i],rfIdx2RfTabName(i));
        }
    }
    else if (strItem == tr("TxCompensateFilter")) {
        if (item->parent()->text(0) == "Calibration(R1C/D)") {
            for (qint8 i = 0;i < MAX_RF;i ++)
                ui->mainTab->addTab(dlgCalR1CTxFilter[i],rfIdx2RfTabName(i));
        }
    }
    else if (strItem == tr("TxBasePower")) {
        if (item->parent()->text(0) == "Calibration(R1C/D)") {
            for (qint8 i = 0;i < MAX_RF;i ++)
                ui->mainTab->addTab(dlgCalR1CTxPwr[i],rfIdx2RfTabName(i));
        }
    }
    else if (strItem == tr("TxAttenuation")) {
        if (item->parent()->text(0) == "Calibration(R1C/D)") {
            for (qint8 i = 0;i < MAX_RF;i ++)
                ui->mainTab->addTab(dlgCalR1CTxAtt[i],rfIdx2RfTabName(i));
        }
	}
    else if (strItem == tr("TxFilterOffset")) {
        if (item->parent()->text(0) == "Calibration(R1C/D)") {
            for (qint8 i = 0;i < MAX_RF;i ++)
                ui->mainTab->addTab(dlgCalR1CTxFilterOffset[i],rfIdx2RfTabName(i));
        }
    }
    else if (strItem == tr("RxCompensateFilter")) {
        if (item->parent()->text(0) == "Calibration(R1C/D)") {
            for (qint8 i = 0;i < MAX_RF;i ++)
                ui->mainTab->addTab(dlgCalR1CRxFilter[i],rfIdx2RfTabName(i));
        }
    }
    else if (strItem == tr("RxReference")) {
        if (item->parent()->text(0) == "Calibration(R1C/D)") {
            for (qint8 i = 0;i < MAX_RF;i ++)
                ui->mainTab->addTab(dlgCalR1CRxRef[i],rfIdx2RfTabName(i));
        }
    }
    else if (strItem == tr("RxAttenuation")) {
        if (item->parent()->text(0) == "Calibration(R1C/D)") {
            for (qint8 i = 0;i < MAX_RF;i ++)
                ui->mainTab->addTab(dlgCalR1CRxAtt[i],rfIdx2RfTabName(i));
        }
    }
    else if (strItem == tr("RxFilterOffset")) {
        if (item->parent()->text(0) == "Calibration(R1C/D)") {
            for (qint8 i = 0;i < MAX_RF;i ++)
                ui->mainTab->addTab(dlgCalR1CRxFilterOffset[i],rfIdx2RfTabName(i));
        }
    }
	connect(ui->mainTab,SIGNAL(currentChanged(int)),this,SLOT(on_mainTab_currentChanged(int)));
    ui->mainTab->setCurrentIndex(int(tabIdxRf));

    if (strItem == QString("FPGA")) {
        ui->mainTab->addTab(dlgFpga,QString("FPGA"));
    }
}

void MainWindow::threadCheckBox(const QString msg)
{
    ::threadCheckBox(msg.toStdString().c_str());
}

void MainWindow::threadErrorBox(const QString msg)
{
    ::threadErrorBox(msg.toStdString().c_str());
}

void MainWindow::on_mainTab_currentChanged(int index)
{
	if (-1 == index)
		return;

	ui->mainTab->setCurrentIndex(index);

    if (dlgRfIQCap[index]->isVisible() | dlgBbIQCap[index]->isVisible())
        emit tabIdxChanged(index);
    else
        emit tabIdxChanged(-1);

    tabIdxRf = qint8(index);
}
