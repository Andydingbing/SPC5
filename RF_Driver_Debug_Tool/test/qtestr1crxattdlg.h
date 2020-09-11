#ifndef QTESTR1CRXATTDLG_H
#define QTESTR1CRXATTDLG_H

#include "qtestbasedlg.h"
#include "qtestr1crxattmodel.h"
#include "global.h"

namespace Ui {
class QTestR1CRxAttDlg;
}

class QTestR1CRxAttDlg : public QTestR1CBaseDlg
{
    Q_OBJECT
    //DECLARE_CAL_R1C_DLG
public slots:
    void sp3301Change();
    void update(const QModelIndex &tl,const QModelIndex &br,cal_file::cal_item_t item,int sec);
private slots:
    void pushButtonStar_clicked();
    void on_pushButtonStop_clicked();
    void on_pushButtonPaus_clicked();
    void on_pushButtonCont_clicked();

public:
    void resetShowWidget(TestParam *param);
    TestParam ui2CalParam();
public:
    explicit QTestR1CRxAttDlg(QWidget *parent = 0);
    ~QTestR1CRxAttDlg();
    qint8 tabIdxRf;
public slots:
     void updateCalUI(QList<QString> list);
private:
    Ui::QTestR1CRxAttDlg *ui;
};

#endif // QTESTR1CRXATTDLG_H
