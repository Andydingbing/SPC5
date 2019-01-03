#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QTreeWidgetItem>
#include <QLabel>
#include <QProgressBar>
#include "define.h"
#include "qmsglogmodel.h"
#include "qreglogmodel.h"
#include "qrfr1acontainerdlg.h"
#include "qrfr1aadvdlg.h"
#include "qrfr1ccontainerdlg.h"
#include "qrfr1cadvdlg.h"
#include "qarbdlg.h"
#include "qiqcapdlg.h"
#include "qbbdlg.h"
#include "qfpgadlg.h"
#include "qcalr1ctxloleakdlg.h"
#include "qcalr1ctxsbdlg.h"
#include "qcalr1ctxfilterdlg.h"
#include "qcalr1ctxpwrdlg.h"
#include "qcalr1ctxattdlg.h"
#include "qcalr1ctxfilteroffsetdlg.h"
#include "qcalr1crxfilterdlg.h"
#include "qcalr1crxrefdlg.h"
#include "qcalr1crxattdlg.h"
#include "qcalr1crxfilteroffsetdlg.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    qint8 tabIdxRf;
    qint8 tabIdxDMA;
    qint8 tabIdxRfu;
    sp3301 *_sp3301;
    sp3501 *_sp3501;
public:
    QMsgLogModel *msgLogModel;
    QRegLogModel *regLogModel;
public:
    QProgressBar *mainProgress;
    QLabel *labelProgressName;
    QLabel *labelVerK7_0;
    QLabel *labelVerK7_1;
    QLabel *labelVerS6;
    QLabel *labelVerDriver;
public:
    QRfR1AContainerDlg  *dlgRfR1AContainer[MAX_RF];
    QRfR1AAdvDlg        *dlgRfR1AAdv[MAX_RF];
    QRfR1CContainerDlg  *dlgRfR1CContainer[MAX_RF];
    QRfR1CAdvDlg        *dlgRfR1CAdv[MAX_RF];
    QArbDlg     *dlgRfArb[MAX_RF];
    QArbDlg     *dlgBbArb[MAX_RF];
    QIQCapDlg   *dlgRfIQCap[MAX_RF];
    QIQCapDlg   *dlgBbIQCap[MAX_RF];
    QBbDlg      *dlgBb[MAX_RF];
    QFPGADlg    *dlgFpga;
public:
    QCalR1CTxLOLeakDlg          *dlgCalR1CTxLOLeak[MAX_RF];
    QCalR1CTxSBDlg              *dlgCalR1CTxSB[MAX_RF];
    QCalR1CTxFilterDlg          *dlgCalR1CTxFilter[MAX_RF];
    QCalR1CTxPwrDlg             *dlgCalR1CTxPwr[MAX_RF];
    QCalR1CTxAttDlg             *dlgCalR1CTxAtt[MAX_RF];
    QCalR1CTxFilterOffsetDlg    *dlgCalR1CTxFilterOffset[MAX_RF];
    QCalR1CRxFilterDlg          *dlgCalR1CRxFilter[MAX_RF];
    QCalR1CRxRefDlg             *dlgCalR1CRxRef[MAX_RF];
    QCalR1CRxAttDlg             *dlgCalR1CRxAtt[MAX_RF];
    QCalR1CRxFilterOffsetDlg    *dlgCalR1CRxFilterOffset[MAX_RF];
public:
    explicit MainWindow(QWidget *parent = 0);
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
    void showSwHwVer(const sp3301::rfu_info_t &info,const char *driver);
    void initProg(const QString name,int pts);
    void setProgPos(int pos);
    void updateMsgTable(int row);
    void updateRegTable(int row);
    void threadCheckBox(const QString msg);
    void threadErrorBox(const QString msg);
public:
    void updateParamInChildDlg();
    void addMsgListCallback();
    void addRegListCallback();
    QString rfIdx2RfTabName(int idx);
    QString rfIdx2BbTabName(int idx);
signals:
    void sp3301Changed();
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

#endif // MAINWINDOW_H
