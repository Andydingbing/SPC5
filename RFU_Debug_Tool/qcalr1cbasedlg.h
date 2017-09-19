#ifndef QCALR1CBASEDLG_H
#define QCALR1CBASEDLG_H

#include <QAbstractItemModel>
#include "qattachthreaddlg.h"

#define DECLARE_CAL_R1C_DLG     \
    public slots:               \
        void SP3301Change();    \
        void resetShowWidget(CalParam *pParam);

#define IMPLEMENT_CAL_R1C_DLG(class_name)   \
    void class_name::SP3301Change() {       \
        ui->m_TEPath->setText(QString("c:\\CSECal\\rfu%1rf%2.cal").arg(m_pSP1401->GetRfuIdx()).arg(m_pSP1401->GetRfIdx())); \
    }

namespace Ui {
class QCalR1CBaseDlg;
}

class QCalR1CBaseDlg : public QAttachThreadDlg
{
    Q_OBJECT

public:
    QCalR1CBaseDlg(QWidget *parent = 0) : QAttachThreadDlg(parent) {}
    virtual void resetShowWidget(CalParam *pParam) = 0;
signals:
    void updateShowWidget(const QModelIndex topleft,const QModelIndex rightbottom);
public slots:
    virtual void SP3301Change() {}
public:
    CSP1401R1C *m_pSP1401;
    CSP2401R1A *m_pSP2401;
};

#endif // QCALR1CBASEDLG_H
