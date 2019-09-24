#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include "define.h"
#include "q_rf_container_dlg.h"
#include "q_arb_dlg.h"
#include "q_iq_cap_dlg.h"
#include "q_bb_dlg.h"
#include "q_fpga_dlg.h"
#include "q_cal_dlg.h"
#include "q_cal_r1c_dlg.h"
#include "q_test_dlg.h"

namespace Ui {
class MainWindow;
}

class QLabel;
class QTreeWidgetItem;
class QProgressBar;
class QHBoxLayout;

class QMsgLogModel;
class QRegLogModel;

class QRfR1AAdvDlg;
class QRFR1CAdvDlg;
class QRFR1FAdvDlg;

//class QCalR1CDlg;
//class QCalR1CTXLOLeakDlg;
//class QCalR1FTXLOLeakDlg;
//class QCalR1CTXSBDlg;
//class QCalR1FTXSBDlg;
//class QCalR1CTXFilterDlg;
//class QCalR1FTXFilterDlg;
//class QCalR1CTXPwrDlg;
//class QCalR1FTXPwrDlg;
//class QCalR1CTxAttDlg;
//class QCalR1CTxFilterOffsetDlg;
//class QCalR1FTXFilterOffsetDlg;
//class QCalR1CRXFilterDlg;
//class QCalR1FRXFilterDlg;
//class QCalR1CRxRefDlg;
//class QCalR1CRxAttDlg;
//class QCalR1CRxFilterOffsetDlg;
//class QCalR1FRXFilterOffsetDlg;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    qint8 tabIdxRf;
    qint8 tabIdxRfu;
    qint8 tabIdxDMA;
    sp3301 *_sp3301;
    sp3501 *_sp3501;
public:
    QMsgLogModel *msgLogModel;
    QRegLogModel *regLogModel;
public:
    QProgressBar *mainProgressBar;
    QLabel *labelProgressName;
    QLabel *labelVerRF;
    QLabel *labelVerK7_0;
    QLabel *labelVerK7_1;
    QLabel *labelVerS6;
    QLabel *labelVerDriver;
public:
    QHBoxLayout *childDlgLayout;
    QHBoxLayout *mainTabLayout;
public:
    QRFR1AContainerDlg *dlgRFR1AContainer[MAX_RF];
    QRfR1AAdvDlg       *dlgRFR1AAdv[MAX_RF];
    QRFR1CContainerDlg *dlgRFR1CContainer[MAX_RF];
    QRFR1CAdvDlg       *dlgRFR1CAdv[MAX_RF];
    QRFR1FContainerDlg *dlgRFR1FContainer[MAX_RF];
    QRFR1FAdvDlg       *dlgRFR1FAdv[MAX_RF];
    QArbDlg   *dlgArb[MAX_RF];
    QIQCapDlg *dlgIQCap[MAX_RF];
    QBbDlg    *dlgBB[MAX_RF];
    QFPGADlg  *dlgFPGA;
public:
    QCalR1CDlg               *dlgCalR1C[MAX_RF];
    QCalR1CTXLOLeakDlg       *dlgCalR1CTXLOLeak[MAX_RF];
    QCalR1CTXSBDlg           *dlgCalR1CTXSB[MAX_RF];
    QCalR1CTXFilterDlg       *dlgCalR1CTXFilter[MAX_RF];
    QCalR1CTXPwrDlg          *dlgCalR1CTXPwr[MAX_RF];
    QCalR1CTXAttDlg          *dlgCalR1CTXAtt[MAX_RF];
    QCalR1CTXFilterOffsetDlg *dlgCalR1CTXFilterOffset[MAX_RF];
    QCalR1CRXFilterDlg       *dlgCalR1CRXFilter[MAX_RF];
    QCalR1CRXRefDlg          *dlgCalR1CRXRef[MAX_RF];
    QCalR1CRXAttDlg          *dlgCalR1CRXAtt[MAX_RF];
    QCalR1CRXFilterOffsetDlg *dlgCalR1CRXFilterOffset[MAX_RF];
    QTestR1CTabWidget *dlgTestR1C[MAX_RF];

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public slots:
    void deviceInit();
    void exit();
    void selSP3301_0();
    void selSP3301_1();
    void selSP3301_2();
    void selSP3301_3();
    void selSP3301_4();
    void starAllIQCapture();
    void stopAllIQCapture();
    void showSwHwVer(const sp3301::rfu_info_t &info, const char *driver);

    void initProg(const QString name);
    void initProg(const QString name,quint32 pts);
    void setProgPos(quint32 pos);
    void addProgPos(quint32 off);
    void setProgClr(const QColor clr);

    void updateMsgTable(int row);
    void updateRegTable(int row);

    void threadCheckBox(const QString msg) { ::threadCheckBox(msg.toStdString().c_str()); }
    void threadErrorBox(const QString msg) { ::threadErrorBox(msg.toStdString().c_str()); }
    void threadProcess(const QWinThread::Process p);

    bool ftpRetryBox()
    { return ::ftpRetryBox(); }

    void testTXIOSwBox(const QColor TX, const QColor RX, int &exec)
    { return ::testTXIOSwBox(TX,RX,exec); }

public:
    void updateParamInChildDlg();
    void addMsgListCallback() { emit addMsgList(Log.msgs()->size()); }
    void addRegListCallback() { emit addRegList(Log.regs()->size()); }

private:
    void registerMetaType();
    void initStatusBar();
    void initChildDlg();
    void initMainTreeWidget();
    void initMainTabWidget();
    void initMsgLogDlg();
    void initRegLogDlg();
    QString rfIdx2RFTabName(int idx);
    QString rfIdx2BBTabName(int idx);
signals:
    void tabIdxChanged(int idx);
    void addMsgList(int row);
    void addRegList(int row);

private slots:
    void on_mainTree_itemClicked(QTreeWidgetItem *item, int column);

    void on_mainTab_currentChanged(int index);

private:
    Ui::MainWindow *ui;
};

extern MainWindow *g_MainW;

#endif // MAIN_WINDOW_H
