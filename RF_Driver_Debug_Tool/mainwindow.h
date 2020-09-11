#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include "ui_mainwindow.h"
#include "rf_container_widget.h"
#include "q_cal_dlg.h"
#include "q_cal_r1c_dlg.h"
#include "q_test_dlg.h"
#include "sp3103.h"

class ChildWidgetHelper;

class QLabel;
class QTreeWidgetItem;
class QProgressBar;
class QHBoxLayout;
class QMenu;
class QAction;

class QMsgLogModel;
class QRegLogModel;

class QRfR1AAdvDlg;
class QRFR1CAdvDlg;
class QRFR1FAdvDlg;

class Q_ARB_Widget;
class Q_SA_Widget;
class QBbDlg;
class QFPGADlg;

class Q_BB_DT3308_Dlg;
class Q_RF_DT3308_F_Dlg;

class Q_SP1403_R1A_Dlg;

class MainWindow : public QMainWindow
{
    Q_OBJECT

    friend class ChildWidgetHelper;

public:
    QLabel *labelVerRF;
    QLabel *labelVerK7_0;
    QLabel *labelVerK7_1;
    QLabel *labelVerS6;

public slots:
    void testTXIOSwBox(const QColor TX, const QColor RX, int &exec)
    { return ::testTXIOSwBox(TX,RX,exec); }

public:
    QList<ChildWidgetHelper *> childWidgets;
    ChildWidgetHelper *currentWidgets;

    NS_SP9500::ChildWidgets  *widgetsSP9500();
    NS_DT3308::ChildWidgets  *widgetsDT3308();
    NS_SP9500X::ChildWidgets *widgetsSP9500X();

public:
    QMsgLogModel *msgLogModel;
    QRegLogModel *regLogModel;

    QProgressBar *mainProgressBar;
    QLabel *labelProgressName;
    QLabel *labelVerDriver;

    static QHBoxLayout childDlgLayout;
    QHBoxLayout *mainTabLayout;

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void switchToSP9500();
    void switchToDT3308();
    void switchToSP9500X();
    void switchProject();
    void deviceInit();
    void deviceInitSilent();
    void exit();
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

    bool ftpRetryBox() { return ::ftpRetryBox(); }

public:
    void addMsgListCallback() { emit addMsgList(int(Log.msgs())); }
    void addRegListCallback() { emit addRegList(int(Log.regs())); }

private:
    void registerMetaType();
    void initStatusBar();
    void initMainTreeWidget();
    void initMainTabWidget();
    void initMsgLogDlg();
    void initRegLogDlg();

signals:
    void addMsgList(int row);
    void addRegList(int row);

private slots:
    void on_mainTree_itemClicked(QTreeWidgetItem *item, int column);

    void on_mainTab_currentChanged(int index);

private:
    Ui::MainWindow *ui;

    QMenu *menuProject;
    QAction *actionSP9500;
    QAction *actionDT3308;
    QAction *actionSP9500X;
};

extern MainWindow *g_MainW;
extern Project project;

#endif // MAIN_WINDOW_H
