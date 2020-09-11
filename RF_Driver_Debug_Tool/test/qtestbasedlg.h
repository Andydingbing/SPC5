#ifndef QTESTBASEDLG_H
#define QTESTBASEDLG_H

#include <QAbstractItemModel>
#include "q_attach_thread_dlg.h"
#include "qtestbasemodel.h"
#include "global.h"

#define DECLARE_TEST_R1C_DLG                                        \
    public slots:                                                   \
        void sp3301Change();                                        \
        void update(const QModelIndex &tl,const QModelIndex &br,cal_file::cal_item_t item,int sec);   \
 private slots:                                                     \
        void on_pushButtonStar_clicked();                           \
        void on_pushButtonStop_clicked();                           \
        void on_pushButtonPaus_clicked();                           \
        void on_pushButtonCont_clicked();                           \
        void on_pushButtonExport_clicked();                         \
    public:                                                         \
        void resetShowWidget(TestParam *param);                     \
        TestParam ui2CalParam();

#define IMPLEMENT_TEST_R1C_DLG(class_name,cal_thread,export_thread,getset_thread)  \
    void class_name::sp3301Change() {                               \
        ui->textEditPath->setText(QString("c:\\CSECal\\rfu%1rf%2.cal").arg(SP1401->get_rfu_idx()).arg(SP1401->get_rf_idx())); \
    }                                                               \
    void class_name::on_pushButtonStar_clicked() {                  \
        CAL_THREAD_EXISTED_CHK();                                   \
        widgetPrepare();                                            \
        TestParam param = ui2CalParam();                            \
        resetShowWidget(&param);                                    \
        QWinThread::g_threadThread = new cal_thread(&param,this);   \
        QWinThread::g_threadThread->start();                        \
    }                                                               \
    void class_name::on_pushButtonStop_clicked() {                  \
        if (ui->pushButtonCont->isEnabled())                        \
            QWinThread::g_threadThread->threadLock.unlock();        \
        QWinThread::g_threadStop = true;                            \
    }                                                               \
    void class_name::on_pushButtonPaus_clicked() {                  \
        QWinThread::g_threadThread->threadLock.lock();              \
        widgetPausing();                                            \
    }                                                               \
    void class_name::on_pushButtonCont_clicked() {                  \
        QWinThread::g_threadThread->threadLock.unlock();            \
        widgetRunning();                                            \
    }                                                               \
    void class_name::on_pushButtonExport_clicked() {                \
        CAL_THREAD_EXISTED_CHK();                                   \
        widgetPrepare();                                            \
        TestParam Param = ui2CalParam();                            \
        resetShowWidget(&Param);                                    \
        QWinThread::g_threadThread = new export_thread(&Param,this);\
        QWinThread::g_threadThread->start();                        \
    }                                                               \

namespace Ui {
class QTestR1CBaseDlg;
}

class QTestBaseDlg : public QAttachThreadDlg
{
    Q_OBJECT

public:
    explicit QTestBaseDlg(QWidget *parent = nullptr);
    virtual void resetShowWidget(TestParam *) = 0;
    virtual TestParam ui2CalParam() = 0;
    virtual void init();
signals:
    void reset();
public slots:
    virtual void sp3301Change() {}
    virtual void update(const QModelIndex &tl,const QModelIndex &br,cal_file::cal_item_t item = cal_file::TX_LOL,int sec = 0) {}
public slots:
    virtual void on_pushButtonStar_clicked() {}
    virtual void on_pushButtonStop_clicked() {}
    virtual void on_pushButtonPaus_clicked() {}
    virtual void on_pushButtonCont_clicked() {}
    virtual void on_pushButtonExport_clicked() {}
    //virtual void on_pushButtonGetSet_clicked() {}
public:
    QVector<QTestBaseModel *> *_model;
};

class QTestR1CBaseDlg : public QTestBaseDlg
{
    Q_OBJECT

public:
  
    explicit QTestR1CBaseDlg(QWidget *parent = nullptr) :
        QTestBaseDlg(parent),

        SP1401(nullptr),
        SP2401(nullptr) {}
public:
    basic_sp1401 *SP1401;
    sp2401_r1a *SP2401;
        sp3301 *SP3301;
    uint32_t   testFlag;
};

#endif // QTESTBASEDLG_H
