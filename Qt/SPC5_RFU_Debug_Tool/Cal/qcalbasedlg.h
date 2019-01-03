#ifndef QCALBASEDLG_H
#define QCALBASEDLG_H

#include <QAbstractItemModel>
#include "qattachthreaddlg.h"
#include "qcalbasemodel.h"
#include "define.h"

#define DECLARE_CAL_R1C_DLG                                         \
    public slots:                                                   \
        void sp3301Change();                                        \
        void update(const QModelIndex &tl,const QModelIndex &br,cal_file::cal_item_t item,int sec);   \
    private slots:                                                  \
        void on_pushButtonStar_clicked();                           \
        void on_pushButtonStop_clicked();                           \
        void on_pushButtonPaus_clicked();                           \
        void on_pushButtonCont_clicked();                           \
        void on_pushButtonExport_clicked();                         \
    public:                                                         \
        void resetShowWidget(CalParam *param);                      \
        CalParam ui2CalParam();

#define IMPLEMENT_CAL_R1C_DLG(class_name,cal_thread,export_thread)  \
    void class_name::sp3301Change() {                               \
        ui->textEditPath->setText(QString("c:\\CSECal\\rfu%1rf%2.cal").arg(_sp1401->get_rfu_idx()).arg(_sp1401->get_rf_idx())); \
    }                                                               \
    void class_name::on_pushButtonStar_clicked() {                  \
        CAL_THREAD_EXISTED_CHK();                                   \
        widgetPrepare();                                            \
        CalParam param = ui2CalParam();                             \
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
        CalParam Param = ui2CalParam();                             \
        resetShowWidget(&Param);                                    \
        QWinThread::g_threadThread = new export_thread(&Param,this);\
        QWinThread::g_threadThread->start();                        \
    }


namespace Ui {
class QCalR1CBaseDlg;
}

class QCalBaseDlg : public QAttachThreadDlg
{
    Q_OBJECT

public:
    explicit QCalBaseDlg(QWidget *parent = 0);
    virtual void resetShowWidget(CalParam *) = 0;
    virtual CalParam ui2CalParam() = 0;
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
public:
    QVector<QCalBaseModel *> *_model;
};

class QCalR1CBaseDlg : public QCalBaseDlg
{
    Q_OBJECT

public:
    explicit QCalR1CBaseDlg(QWidget *parent = 0) :
        QCalBaseDlg(parent),
        _sp1401(NULL),
        _sp2401(NULL) {}
public:
    sp1401_r1c *_sp1401;
    sp2401_r1a *_sp2401;
};

#endif // QCALBASEDLG_H
