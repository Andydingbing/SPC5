#include "qrfr1ccontainerdlg.h"
#include <QVBoxLayout>

QRfR1CContainerDlg::QRfR1CContainerDlg(QWidget *parent) :
    QWidget(parent)
{
    dlgRfR1C = new QRfR1CDlg(this);
    dlgRfR1C->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    if (!dlgTempCtrl) {
        dlgTempCtrl = new QTempCtrlDlg;
        dlgTempCtrl->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    }
    layout = new QVBoxLayout(this);
    layout->addWidget(dlgRfR1C);
    layout->addWidget(dlgTempCtrl);
}

QRfR1CContainerDlg::~QRfR1CContainerDlg()
{

}

void QRfR1CContainerDlg::showEvent(QShowEvent *event)
{
    if (event->type() == QEvent::Show)
        layout->addWidget(dlgTempCtrl);

    QWidget::showEvent(event);
}
