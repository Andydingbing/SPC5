#ifndef QTESTR1CTXFREQDLG_H
#define QTESTR1CTxFREQDLG_H

#include "qtestbasedlg.h"
#include "define.h"

namespace Ui {
class QTestR1CTxFreqDlg;
}

class QTestR1CTxFreqDlg : public QTestR1CBaseDlg
{
    Q_OBJECT
  //  DECLARE_CAL_R1C_DLG
public slots:
    void sp3301Change();
    void update(const QModelIndex &tl,const QModelIndex &br,cal_file::cal_item_t item,int sec);
private slots:
    void on_pushButtonStop_clicked();
    void on_pushButtonPaus_clicked();
    void on_pushButtonCont_clicked();
    void pushButtonStar_clicked();

public:
    void resetShowWidget(TestParam *param);
    TestParam ui2CalParam();
public:
    explicit QTestR1CTxFreqDlg(QWidget *parent = 0);
    ~QTestR1CTxFreqDlg();
    qint8 tabIdxRf;
//public slots:
//     void updateCalUI(QList<QString> list);
private:
    Ui::QTestR1CTxFreqDlg *ui;
};

#endif // QTESTR1CTXFREQDLG_H
