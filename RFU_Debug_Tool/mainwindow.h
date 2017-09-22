#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets/QTreeWidgetItem>
#include <QtWidgets/QLabel>
#include <QtWidgets/QProgressBar>
#include "define.h"
#include "qmsglogmodel.h"
#include "qreglogmodel.h"
#include "qrfr1cdlg.h"
#include "qrfr1cadvdlg.h"
#include "qfpgadlg.h"
#include "qcalr1ctxloleakdlg.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    CSP3301 *m_pSP3301;
    CSP3501 *m_pSP3501;
    QMsgLogModel *m_pMsgModel;
    QRegLogModel *m_pRegModel;
    QProgressBar *m_pMainProg;
    QLabel *m_pProgName;
    QLabel *m_pVerK7_0;
    QLabel *m_pVerK7_1;
    QLabel *m_pVerS6;
    QLabel *m_pVerDriver;
    QRfR1CDlg *m_pRfR1CDlg[MAX_RF];
    QRfR1CAdvDlg *m_pRfR1CAdvDlg[MAX_RF];
    QFPGADlg *m_pFPGADlg;
    QCalR1CTxLOLeakDlg *m_pCalR1CTxLOLeakDlg[MAX_RF];
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
public slots:
    void deviceInitialization();
    void showSwHwVer(const CSP3301::RFUInfo &Info,const char *pDriver);
    void initProg(const QString strName, int iPts);
    void setProgPos(int iPos);
    void updateMsgTable(int iRow);
    void updateRegTable(int iRow);
    void threadCheckBox(const QString strMsg);
    void threadErrorBox(const QString strMsg);
public:
    void updateParamInChildDlg();
    void addMsgListCallback();
    void addRegListCallback();
signals:
    void SP3301Changed();
    void addMsgList(int iRow);
    void addRegList(int iRow);
private slots:
    void on_m_pMainTree_itemClicked(QTreeWidgetItem *item, int column);

private:
    Ui::MainWindow *ui;
};

extern MainWindow *g_pMainW;

#endif // MAINWINDOW_H
