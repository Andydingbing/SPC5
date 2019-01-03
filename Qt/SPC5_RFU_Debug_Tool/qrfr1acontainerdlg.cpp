#include "qrfr1acontainerdlg.h"

QRfR1AContainerDlg::QRfR1AContainerDlg(QWidget *parent) :
    QWidget(parent)
{
    dlgRfR1A = new QRfR1ADlg(this);
    dlgRfR1A->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    if (!dlgTempCtrl) {
        dlgTempCtrl = new QTempCtrlDlg;
        dlgTempCtrl->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    }
    layout = new QVBoxLayout(this);
    layout->addWidget(dlgRfR1A);
    layout->addWidget(dlgTempCtrl);
}

QRfR1AContainerDlg::~QRfR1AContainerDlg()
{

}

void QRfR1AContainerDlg::showEvent(QShowEvent *event)
{
    if (event->type() == QEvent::Show)
        layout->addWidget(dlgTempCtrl);

    QWidget::showEvent(event);
}
