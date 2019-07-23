#ifndef Q_RF_CONTAINER_DLG_H
#define Q_RF_CONTAINER_DLG_H

#include "q_temp_ctrl_dlg.h"
#include <QWidget>
#include <QVBoxLayout>

class QShowEvent;
class QRFR1ADlg;
class QRFR1CDlg;
class QRFR1FDlg;

class QContainerDlg : public QWidget
{
    Q_OBJECT

public:
    QVBoxLayout *layout;

public:
    explicit QContainerDlg(QWidget *parent = nullptr) :
        QWidget(parent) {}
    ~QContainerDlg() {}

protected:
    void showEvent(QShowEvent *event)
    {
        if (event->type() == QEvent::Show) {
            layout->addWidget(dlgTempCtrl);
        }

        QWidget::showEvent(event);
    }
};

template <typename DlgType>
class QRFContainerDlg : public QContainerDlg
{
public:
    DlgType *dlg;

public:
    explicit QRFContainerDlg(QWidget *parent = nullptr) :
        QContainerDlg(parent)
    {
        dlg = new DlgType(this);
        dlg->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
        if (!dlgTempCtrl) {
            dlgTempCtrl = new QTempCtrlDlg;
            dlgTempCtrl->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
        }
        layout = new QVBoxLayout(this);
        layout->addWidget(dlg);
        layout->addWidget(dlgTempCtrl);
    }
};

typedef QRFContainerDlg<QRFR1ADlg> QRFR1AContainerDlg;
typedef QRFContainerDlg<QRFR1CDlg> QRFR1CContainerDlg;
typedef QRFContainerDlg<QRFR1FDlg> QRFR1FContainerDlg;

#endif // Q_RF_CONTAINER_DLG_H
