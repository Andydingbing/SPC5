#ifndef Q_CAL_BASE_DLG_H
#define Q_CAL_BASE_DLG_H

#include <QAbstractItemModel>
#include "q_attach_thread_dlg.hpp"
#include "q_winthread.h"
#include "q_base_model.h"
#include "global.h"

/*
 *
 * We put all common signals and slots in just one base class
 * called QThreadDlg,and some simple classes with there own
 * slots such as class QCalBaseDlg/QTestBaseDlg,for the reason
 * that Qt does not support template class with Q_OBJECT macro.
 *
 * We force QAttachThreadDlg not inherit from QObject or QWidget,
 * for the reason that the finally dialog class may be casted to
 * QObject* or QWidget*,but one of its parent class(QCalBaseDlg,
 * QTestBaseDlg) has been inherited from QWidget.
 *
 *
 *                                   QWidget
 *                                      |
 *                                      |
 *                                 (with signal/slot,slots can be virtual)
 *            +------------------- QThreadDlg ----------------+
 *            |                                               |
 *            |                                               |
 *       (with particular slots)   (buttons manager)     (with particular slots)
 *       QCalBaseDlg               QAttachThreadDlg      QTestBaseDlg
 *            |                         |  |                   |
 *            |     +-------------------+  +--------------+    |
 *            |     |                                     |    |
 *    (buttons' slot actual impl)                    (buttons' slot actual impl)
 *    (with thread start/stop/pause/continue)        (just with thread start/stop)
 *    QCalDlg<......>                                QTestDlg<......>
 *            |                                                |
 *            |                                                |
 *    (finally dialogs)                              (finally dialogs)
 *    QCalR1CTXLOLeakDlg,QCalR1CTXSBDlg...           QTestR1CFreqResDlg...
 *
 *
 */

class QThreadDlg : public QWidget
{
    Q_OBJECT

public:
    explicit QThreadDlg(QWidget *parent = nullptr) :
        QWidget(parent),
        model(new QVector<QCalBaseModel *>) {}

    virtual void star() = 0; // Start the thread
    virtual void threadDone() = 0;

    virtual void init()
    {
        QVector<QCalBaseModel *>::ConstIterator iter;
        for (iter = model->constBegin();iter != model->constEnd();iter ++) {
            if (*iter != nullptr) {
                connect(this,SIGNAL(reset()),*iter,SLOT(reset()));
            }
        }
    }

signals:
    void reset();

public slots:
    void on_pushButtonStar_clicked() { star(); }
    void on_pushButtonStop_clicked()
    {
        if (QWinThread::g_threadPausing == true) {
            QWinThread::g_threadThread->threadLock.unlock();
        }
        QWinThread::g_threadStop = true;
    }

public slots:
    void done() { threadDone(); }

public:
    QVector<QCalBaseModel *> *model;
};


class QCalBaseDlg : public QThreadDlg
{
    Q_OBJECT

public:
    explicit QCalBaseDlg(QWidget *parent = nullptr) :
        QThreadDlg(parent) {}

    virtual void paus() = 0;   // Pause
    virtual void cont() = 0;   // Continue
    virtual void expt() = 0;   // Export the calibration data
    virtual void getset() = 0; // Get and Set

public slots:
    virtual void updateFromParam(const CalR1CParam &) {}
    virtual void update(const QModelIndex &/*tl*/,
                        const QModelIndex &/*br*/,
                        cal_file::cal_item_t item = cal_file::TX_LOL,
                        int sec = 0)
    { Q_UNUSED(item); Q_UNUSED(sec); }

    virtual void uiInsert(const int /*first*/,const int /*last*/,const int /*cal_table*/) {}
    virtual void uiUpdate(const int /*first*/,const int /*last*/,const int /*cal_table*/) {}

    void on_pushButtonPaus_clicked() { paus(); }
    void on_pushButtonCont_clicked() { cont(); }
    void on_pushButtonExport_clicked() { expt(); }
    void on_pushButtonGetSet_clicked() { getset(); }
};

template<typename Cal_Thread_T, typename Exp_Thread_T>
class QCalDlg : public QCalBaseDlg, public QAttachThreadDlg
{
public:
    explicit QCalDlg(QWidget *parent) :
        QCalBaseDlg(parent),
        QAttachThreadDlg(),
        CalThread(nullptr),
        ExpThread(nullptr) {}

    Cal_Thread_T *CalThread;
    Exp_Thread_T *ExpThread;

    virtual void resetShowWidget(CalParam *) = 0;
    virtual void uiToCalParam(CalParam *) = 0;

    virtual void prepare(const bool is_exp = false) { Q_UNUSED(is_exp) }

    void star() // Start
    {
        CAL_THREAD_EXISTED_CHK();
        widgetPrepare();
        CalParam param;
        uiToCalParam(&param);
        resetShowWidget(&param);
        prepare(false);
        QWinThread::g_threadThread = new Cal_Thread_T(param);
        QWinThread::g_threadThread->start();
    }

    void expt() // Export Calibration File
    {
        CAL_THREAD_EXISTED_CHK();
        widgetPrepare();
        CalParam param;
        uiToCalParam(&param);
        resetShowWidget(&param);
        prepare(true);
        QWinThread::g_threadThread = new Exp_Thread_T(param);
        QWinThread::g_threadThread->start();
    }

    void paus() // Pause
    {
        QWinThread::g_threadThread->threadLock.lock();
        widgetPausing();
        QWinThread::g_threadPausing = true;
    }

    void cont() // Continue
    {
        QWinThread::g_threadThread->threadLock.unlock();
        widgetRunning();
        QWinThread::g_threadPausing = false;
    }

    void threadDone()
    {
        QWinThread::g_threadStop = true;
        widgetResume();
    }
};


class QTestBaseDlg : public QThreadDlg
{
    Q_OBJECT

public:
    QTestBaseDlg(QWidget *parent = nullptr) : QThreadDlg(parent) {}

public slots:
    virtual void update(const QModelIndex &/*tl*/,
                        const QModelIndex &/*br*/,
                        test_item_t /*item*/,
                        int /*sec*/) {}
};

template <typename Test_Param_T, typename Test_Thread_T>
class QTestDlg : public QTestBaseDlg, public QAttachThreadDlg
{
public:
    explicit QTestDlg(QWidget *parent) :
        QTestBaseDlg(parent),
        QAttachThreadDlg() {}

    virtual void resetShowWidget(TestBaseParam *) = 0;
    virtual void uiToTestParam(TestBaseParam *) = 0;

    void star()
    {
        CAL_THREAD_EXISTED_CHK();
        widgetPrepare();
        Test_Param_T *param = new Test_Param_T;
        uiToTestParam(param);
        resetShowWidget(param);
        QWinThread::g_threadThread = new Test_Thread_T(param);
        QWinThread::g_threadThread->start();
    }

    void threadDone()
    {
        QWinThread::g_threadStop = true;
        widgetResume();
    }
};

#define DECL_CAL_DLG(Name, Name_ui, Cal_Thread, Exp_Thread) \
class Name : public QCalDlg<Cal_Thread, Exp_Thread> { \
public: \
    explicit Name(QWidget *parent) : \
        QCalDlg(parent), \
        ui(new Ui::Name_ui) \
    { ui->setupUi(this); init(); } \
    ~Name() { delete ui; } \
    Ui::Name_ui *UI() const { return ui; } \
public slots: \
    void updateFromParam(const CalR1CParam &); \
    void update(const QModelIndex &tl, const QModelIndex &br, \
                cal_file::cal_item_t item, int sec); \
    void init(); \
    void resetShowWidget(CalParam *param); \
    void uiToCalParam(CalParam *param); \
    void getset(); \
public: \
    Ui::Name_ui *ui; \
};

#define CAL_WIDGET(Name, Name_ui, Name_child, Cal_Thread, Exp_Thread) \
class Name : public QCalDlg<Cal_Thread, Exp_Thread> { \
public: \
    explicit Name(QWidget *parent) : \
        QCalDlg(parent), \
        ui(new Ui::Name_ui), \
        childs(nullptr) { ui->setupUi(this); init(); } \
    ~Name() { delete ui; } \
public slots: \
    void updateFromParam(const CalR1CParam &); \
    void uiInsert(const int first,const int last,const int cal_table); \
    void uiUpdate(const int first,const int last,const int cal_table); \
    void init(); \
    void resetShowWidget(CalParam *param); \
    void prepare(const bool is_exp = false); \
    void uiToCalParam(CalParam *param); \
    void getset(); \
public: \
    Ui::Name_ui *ui; \
    Name_child *childs; \
};

// Test dialogs definition.
#define DECL_TEST_DLG_MEMBER_FUNC \
public slots: \
    void update(const QModelIndex &tl, const QModelIndex &br, \
                test_item_t item, int sec); \
public : \
    virtual void init(); \
    virtual void resetShowWidget(TestBaseParam *param); \
    virtual void uiToTestParam(TestBaseParam *param);

#define DECL_TEST_DLG(Name, Name_ui, Test_Param, Cal_Thread) \
class Name : public QTestDlg<Test_Param, Cal_Thread> { \
public: \
    explicit Name(QWidget *parent) : \
        QTestDlg(parent), \
        ui(new Ui::Name_ui) { ui->setupUi(this);init(); } \
    ~Name() { delete ui; } \
    DECL_TEST_DLG_MEMBER_FUNC \
    Ui::Name_ui *ui; \
};


#define DECL_CAL_R1A_DLG(name) \
    DECL_CAL_DLG(QCalR1A##name##Dlg, \
                 QCal##name##Dlg, \
                 QCalR1A##name##Thread, \
                 QExpR1A##name##Thread)

#define DECL_CAL_R1C_DLG(name) \
    DECL_CAL_DLG(QCalR1C##name##Dlg, \
                 QCal##name##Dlg, \
                 QCalR1C##name##Thread, \
                 QExpR1C##name##Thread)

#define DECL_CAL_WIDGET(project,name) \
    namespace NS_##project { \
    class Cal_##name##_ChildWidgets; \
    CAL_WIDGET(Q_Cal_##name##_Widget, \
               Q_##project##_Cal_##name##_Widget, \
               Cal_##name##_ChildWidgets, \
               Q_Cal_##name##_Thread, \
               Q_Exp_##name##_Thread) \
    } // namespace NS_##project

#define DECL_TEST_R1C_DLG(name) \
    DECL_TEST_DLG(QTestR1C##name##Dlg, \
                  QTest##name##Dlg, \
                  Test##name##Param, \
                  QTestR1C##name##Thread)

#define DECL_TEST_WIDGET(project,item) \
    DECL_TEST_DLG(Q_Test_##item##_Widget, \
                  Q_Test_##project##_##item##_Widget, \
                  Test_##item##_Param, \
                  Q_Test_##item##_Thread)

#endif // Q_CAL_BASE_DLG_H
