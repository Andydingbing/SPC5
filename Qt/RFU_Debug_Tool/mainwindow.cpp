#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMetaType>
#include <QMessageBox>
#include "qdeviceinitthread.h"
#include "qiqcapthread.h"

MainWindow *g_pMainW = NULL;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    qRegisterMetaType<CSP3301::RFUInfo>("CSP3301::RFUInfo");

    m_iTabIdxRf = 0;
    m_iTabIdxDMA = 0;
    m_iTabIdxRfu = 0;

    m_pMainProg = new QProgressBar(ui->m_pStatusBar);
    m_pMainProg->setTextVisible(false);
    m_pMainProg->setValue(0);
    m_pMainProg->setFixedHeight(15);
    m_pProgName = new QLabel(QString("..."));
    m_pVerK7_0 = new QLabel(QString("K7_0:0x00000000"));
    m_pVerK7_1 = new QLabel(QString("K7_1:0x00000000"));
    m_pVerS6 = new QLabel(QString("S6:0x00000000"));
    m_pVerDriver = new QLabel(QString("Driver:0.0.00000"));
    ui->m_pStatusBar->clearMessage();
    ui->m_pStatusBar->addWidget(m_pProgName);
    ui->m_pStatusBar->addWidget(m_pMainProg,1);
    ui->m_pStatusBar->addPermanentWidget(m_pVerK7_0);
    ui->m_pStatusBar->addPermanentWidget(m_pVerK7_1);
    ui->m_pStatusBar->addPermanentWidget(m_pVerS6);
    ui->m_pStatusBar->addPermanentWidget(m_pVerDriver);
    ui->m_pMainTree->setHeaderHidden(true);
    ui->m_pMainTree->setRootIsDecorated(true);
    ui->m_pMainTree->clear();
    ui->m_pMainTree->setColumnCount(1);
    ui->m_pMainTree->setStyleSheet("QTreeWidget{background:rgb(179,217,255)}"
                                   "QTreeWidget::item:selected{background:rgb(0,255,0);color:black}");

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
        itemRoot.append(new QTreeWidgetItem(ui->m_pMainTree,QStringList(* constIter)));
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
    strChild.append(QString("发本振泄漏"));
    strChild.append(QString("发边带抑制"));
    strChild.append(QString("发衰减器误差"));
    strChild.append(QString("发基准功率"));
    strChild.append(QString("收SGPMM建表"));
    strChild.append(QString("收参考电平"));
    itemChild_0.clear();
    for (constIter = strChild.constBegin();constIter != strChild.constEnd();constIter ++)
        itemChild_0.append(new QTreeWidgetItem(itemRoot.at(2),QStringList(*constIter)));

    strChild.clear();
    strChild.append(QString("发本振泄漏"));
    strChild.append(QString("发边带抑制"));
    strChild.append(QString("发补偿滤波器"));
    strChild.append(QString("发基准功率"));
    strChild.append(QString("发衰减器误差"));
    strChild.append(QString("发滤波器误差"));
    strChild.append(QString("收补偿滤波器"));
    strChild.append(QString("收参考电平"));
    strChild.append(QString("收衰减器误差"));
    strChild.append(QString("收滤波器误差"));
    itemChild_0.clear();
    for (constIter = strChild.constBegin();constIter != strChild.constEnd();constIter ++)
        itemChild_0.append(new QTreeWidgetItem(itemRoot.at(3),QStringList(*constIter)));

    strChild.clear();
    strChild.append(QString("单板稳定性"));
    strChild.append(QString("发频率响应"));
    strChild.append(QString("收频率响应"));
    itemChild_0.clear();
    for (constIter = strChild.constBegin();constIter != strChild.constEnd();constIter ++)
        itemChild_0.append(new QTreeWidgetItem(itemRoot.at(4),QStringList(*constIter)));

    strChild.clear();
    strChild.append(QString("单板稳定性"));
    strChild.append(QString("发频率响应"));
    strChild.append(QString("收频率响应"));
    strChild.append(QString("风扇转速与温度的关系"));
    strChild.append(QString("温度变化与发功率的关系"));
    itemChild_0.clear();
    for (constIter = strChild.constBegin();constIter != strChild.constEnd();constIter ++)
        itemChild_0.append(new QTreeWidgetItem(itemRoot.at(5),QStringList(*constIter)));

    QSizePolicy childDlgPolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    QHBoxLayout *childDlgLayout = new QHBoxLayout;
    QHBoxLayout *mainTabLayout = new QHBoxLayout;

    for (int8_t i = 0;i < MAX_RF;i ++) {
        m_pRfR1CDlg[i] = new QRfR1CDlg(ui->m_pMainTab);
        m_pRfR1CDlg[i]->setSizePolicy(childDlgPolicy);
        childDlgLayout->addWidget(m_pRfR1CDlg[i]);

        m_pRfR1CAdvDlg[i] = new QRfR1CAdvDlg(ui->m_pMainTab);
        m_pRfR1CAdvDlg[i]->setSizePolicy(childDlgPolicy);
        childDlgLayout->addWidget(m_pRfR1CAdvDlg[i]);

        m_pRfIQCapDlg[i] = new QIQCapDlg(ui->m_pMainTab);
        m_pBbIQCapDlg[i] = new QIQCapDlg(ui->m_pMainTab);
        m_pRfIQCapDlg[i]->setSizePolicy(childDlgPolicy);
        m_pBbIQCapDlg[i]->setSizePolicy(childDlgPolicy);
        childDlgLayout->addWidget(m_pRfIQCapDlg[i]);
        childDlgLayout->addWidget(m_pBbIQCapDlg[i]);

        m_pCalR1CTxLOLeakDlg[i] = new QCalR1CTxLOLeakDlg(ui->m_pMainTab);
        m_pCalR1CTxLOLeakDlg[i]->setSizePolicy(childDlgPolicy);
        childDlgLayout->addWidget(m_pCalR1CTxLOLeakDlg[i]);
        connect(this,SIGNAL(SP3301Changed()),m_pCalR1CTxLOLeakDlg[i],SLOT(SP3301Change()));
    }
    m_pFPGADlg = new QFPGADlg(ui->m_pMainTab);
    m_pFPGADlg->setSizePolicy(childDlgPolicy);
    childDlgLayout->addWidget(m_pFPGADlg);

    mainTabLayout->addLayout(childDlgLayout);
    ui->m_pMainTab->setLayout(mainTabLayout);

    m_pSP3501 = &SP3501;
    m_pSP3301 = &SP3301_2;
    updateParamInChildDlg();

    m_pMsgModel = new QMsgLogModel;
    ui->m_TVMsg->setModel(m_pMsgModel);
    ui->m_TVMsg->setColumnWidth(0,125);
    ui->m_TVMsg->setColumnWidth(1,350);
    ui->m_TVMsg->setColumnWidth(2,50);
    ui->m_TVMsg->setColumnWidth(3,80);
    ui->m_TVMsg->horizontalHeader()->setFixedHeight(20);
    ui->m_TVMsg->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
    ui->m_TVMsg->verticalHeader()->setDefaultSectionSize(18);
    ui->m_TVMsg->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Fixed);
    ui->m_TVMsg->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch);
    ui->m_TVMsg->horizontalHeader()->setSectionResizeMode(2,QHeaderView::Fixed);
    ui->m_TVMsg->horizontalHeader()->setSectionResizeMode(3,QHeaderView::Fixed);

    m_pRegModel = new QRegLogModel;
    ui->m_TVReg->setModel(m_pRegModel);
    ui->m_TVReg->setColumnWidth(0,68);
    ui->m_TVReg->setColumnWidth(1,68);
    ui->m_TVReg->setColumnWidth(2,60);
    ui->m_TVReg->setColumnWidth(3,85);
    ui->m_TVReg->setColumnWidth(4,85);
    ui->m_TVReg->setColumnWidth(5,50);
    ui->m_TVReg->horizontalHeader()->setFixedHeight(20);
    ui->m_TVReg->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
    ui->m_TVReg->verticalHeader()->setDefaultSectionSize(18);
    ui->m_TVReg->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Fixed);
    ui->m_TVReg->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Fixed);
    ui->m_TVReg->horizontalHeader()->setSectionResizeMode(2,QHeaderView::Fixed);
    ui->m_TVReg->horizontalHeader()->setSectionResizeMode(3,QHeaderView::Fixed);
    ui->m_TVReg->horizontalHeader()->setSectionResizeMode(4,QHeaderView::Fixed);
    ui->m_TVReg->horizontalHeader()->setSectionResizeMode(5,QHeaderView::Fixed);

    connect(this,SIGNAL(addMsgList(int)),m_pMsgModel,SLOT(update(int)));
    connect(this,SIGNAL(addRegList(int)),m_pRegModel,SLOT(update(int)));
    connect(this,SIGNAL(addMsgList(int)),this,SLOT(updateMsgTable(int)));
    connect(this,SIGNAL(addRegList(int)),this,SLOT(updateRegTable(int)));

    connect(ui->actionInitialization,SIGNAL(triggered(bool)),this,SLOT(deviceInitialization()));
    connect(ui->actionStart_All_IQ_Capture_R,SIGNAL(triggered(bool)),this,SLOT(startAllIQCapture()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::deviceInitialization()
{
    g_pThread = new QDeviceInitThread(this);
    QDeviceInitThread *pThread = (QDeviceInitThread *)g_pThread;
    connect(pThread,&QDeviceInitThread::swhwVerReady,this,&MainWindow::showSwHwVer);
    pThread->start();
}

void MainWindow::startAllIQCapture()
{
    THREAD_EXISTED_CHK();
    QIQCapThread::Param param;
    for (int i = 0;i < MAX_RF;i ++) {
        param.m_pSP1401[i] = m_pSP3301->m_pSP1401.at(i);
        if (m_pRfIQCapDlg[i]->isVisible() | m_pBbIQCapDlg[i]->isVisible())
            param.m_bVisible[i] = true;
        else
            param.m_bVisible[i] = false;
    }
    g_pThread = new QIQCapThread(param,this);
    QIQCapThread *pThread = (QIQCapThread *)g_pThread;
    for (int i = 0;i < MAX_RF;i ++) {
        connect(pThread,&QIQCapThread::updatePlot,m_pRfIQCapDlg[i],&QIQCapDlg::updatePlot);
        connect(pThread,&QIQCapThread::updatePlot,m_pBbIQCapDlg[i],&QIQCapDlg::updatePlot);
    }
    connect(this,&MainWindow::tabIdxChanged,pThread,&QIQCapThread::tabIdxChanged);
    g_pThread->start();
}

void MainWindow::showSwHwVer(const CSP3301::RFUInfo &Info,const char *pDriver)
{
    m_pVerK7_0->setText(QString("K7_0:0x%1").arg(Info.m_uiK7_0_Ver,8,16));
    m_pVerK7_1->setText(QString("K7_1:0x%1").arg(Info.m_uiK7_1_Ver,8,16));
    m_pVerS6->setText(QString("S6:0x%1").arg(Info.m_uiS6_Ver,8,16));
    m_pVerDriver->setText(QString("Driver:%1").arg(pDriver));
}

void MainWindow::updateParamInChildDlg()
{
    for (int8_t i = 0;i < MAX_RF;i ++) {
        m_pRfR1CDlg[i]->m_pSP1401 = m_pSP3301->m_pSP1401R1C[i];
        m_pRfIQCapDlg[i]->m_pSP1401 = m_pSP3301->m_pSP1401[i];
        m_pRfIQCapDlg[i]->m_pSP2401 = m_pSP3301->m_pSP2401[i];
        m_pBbIQCapDlg[i]->m_pSP1401 = m_pSP3301->m_pSP1401[i];
        m_pBbIQCapDlg[i]->m_pSP2401 = m_pSP3301->m_pSP2401[i];
        m_pCalR1CTxLOLeakDlg[i]->m_pSP1401 = m_pSP3301->m_pSP1401R1C[i];
        m_pCalR1CTxLOLeakDlg[i]->m_pSP2401 = m_pSP3301->m_pSP2401[i];
        m_pCalR1CTxLOLeakDlg[i]->m_pSP3501 = &SP3501;
    }
    m_pFPGADlg->m_pSP2401[0] = m_pSP3301->m_pSP2401[0];
    m_pFPGADlg->m_pSP2401[1] = m_pSP3301->m_pSP2401[2];

    emit SP3301Changed();
}

void MainWindow::addMsgListCallback()
{
    int iRow = Log.GetMsgLog()->size();
    emit addMsgList(iRow);
}

void MainWindow::addRegListCallback()
{
    int iRow = Log.GetRegLog()->size();
    emit addRegList(iRow);
}

QString MainWindow::rfIdx2RfTabName(int iIdx)
{
    return QString("RF_%1").arg(iIdx);
}

QString MainWindow::rfIdx2BbTabName(int iIdx)
{
    return QString("k7-%1--->RF%2").arg((MAX_RF - 1 - iIdx)/2).arg(iIdx);
}

void MainWindow::initProg(const QString strName, int iPts)
{
    QWinThread::g_strProc = strName;
    m_pProgName->setText(strName + "%0");
    m_pMainProg->setRange(0,iPts);
    m_pMainProg->setValue(0);
}

void MainWindow::setProgPos(int iPos)
{
    int iRange = m_pMainProg->maximum();
    if (iPos > iRange)
        return;
    QString strProgName = m_pProgName->text();
    QString strPercent = QString(" %%1").arg(double(iPos) / double(iRange) * 100.0);
    int iPercentPos = strProgName.indexOf("%",0) - 1;
    strProgName.replace(iPercentPos,4,strPercent);
    m_pProgName->setText(strProgName);
    m_pMainProg->setValue(iPos);
}

void MainWindow::updateMsgTable(int iRow)
{
    ui->m_TVMsg->scrollToBottom();
    ui->m_TVMsg->selectRow(iRow - 1);
}

void MainWindow::updateRegTable(int iRow)
{
    ui->m_TVReg->scrollToBottom();
    ui->m_TVReg->selectRow(iRow - 1);
}

void MainWindow::on_m_pMainTree_itemClicked(QTreeWidgetItem *item, int column)
{
    QString strItem = item->text(column);

    ui->m_pMainTab->clear();
    emit tabIdxChanged(-1);
    if (strItem == QString("Overview")) {
        if (item->parent()->text(0) == "R1C/D") {
            for (int8_t i = 0;i < MAX_RF;i ++)
                ui->m_pMainTab->addTab(m_pRfR1CDlg[i],rfIdx2RfTabName(i));
        }
    }
    else if (strItem == QString("Advance")) {
        if (item->parent()->text(0) == "R1C/D") {
            for (int8_t i = 0;i < MAX_RF;i ++)
                ui->m_pMainTab->addTab(m_pRfR1CAdvDlg[i],rfIdx2RfTabName(i));
        }
    }
    else if (strItem == QString("IQ Capture")) {
        if (item->parent()->text(0) == "RF Debug") {
            for (int8_t i = 0;i < MAX_RF;i ++) {
                ui->m_pMainTab->addTab(m_pRfIQCapDlg[i],rfIdx2RfTabName(i));
                m_pRfIQCapDlg[i]->m_pSP2401->SetADSw(CSP2401R1A::FromRf);
            }
            on_m_pMainTab_tabBarClicked(int(m_iTabIdxRf));
        }
        else if (item->parent()->text(0) == "BB Debug") {
            for (int8_t i = 0;i < MAX_RF;i ++) {
                ui->m_pMainTab->addTab(m_pBbIQCapDlg[i],rfIdx2BbTabName(i));
                m_pBbIQCapDlg[i]->m_pSP2401->SetADSw(CSP2401R1A::FromBb);
            }
            on_m_pMainTab_tabBarClicked(int(m_iTabIdxRf));
        }
    }
    else if (strItem == tr("发本振泄漏")) {
        if (item->parent()->text(0) == "Calibration(R1C/D)") {
            for (int8_t i = 0;i < MAX_RF;i ++)
                ui->m_pMainTab->addTab(m_pCalR1CTxLOLeakDlg[i],tr("RF_%1").arg(i));
        }
    }
    ui->m_pMainTab->setCurrentIndex(int(m_iTabIdxRf));

    if (strItem == QString("FPGA")) {
        ui->m_pMainTab->addTab(m_pFPGADlg,QString("FPGA"));
    }
}

void MainWindow::threadCheckBox(const QString strMsg)
{
    ::threadCheckBox(strMsg.toStdString().c_str());
}

void MainWindow::threadErrorBox(const QString strMsg)
{
    ::threadErrorBox(strMsg.toStdString().c_str());
}

void MainWindow::on_m_pMainTab_tabBarClicked(int index)
{
    ui->m_pMainTab->setCurrentIndex(index);
    m_iTabIdxRf = int8_t(index);

    if (m_pRfIQCapDlg[index]->isVisible() | m_pBbIQCapDlg[index]->isVisible())
        emit tabIdxChanged(index);
    else
        emit tabIdxChanged(-1);
}
