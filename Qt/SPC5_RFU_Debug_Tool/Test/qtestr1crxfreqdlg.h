#ifndef QTESTR1CRXFREQDLG_H
#define QTESTR1CRXFREQDLG_H

#include "qtestbasedlg.h"
#include "qr1crxfreqmodel.h"
#include "define.h"

namespace Ui {
class QTestR1CRxFreqDlg;
}

class QTestR1CRxFreqDlg : public QTestR1CBaseDlg
{
    Q_OBJECT

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
    explicit QTestR1CRxFreqDlg(QWidget *parent = 0);
    ~QTestR1CRxFreqDlg();
    qint8 tabIdxRf;
public slots:
     void updateCalUI(QList<QString> list);
private:
    Ui::QTestR1CRxFreqDlg *ui;
};

#endif // QTESTR1CRXFREQDLG_H


