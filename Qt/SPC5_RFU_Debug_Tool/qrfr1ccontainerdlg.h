#ifndef QRFR1CCONTAINERDLG_H
#define QRFR1CCONTAINERDLG_H

#include <QWidget>
#include <QVBoxLayout>
#include <QShowEvent>
#include "qrfr1cdlg.h"
#include "qtempctrldlg.h"

class QRfR1CContainerDlg : public QWidget
{
    Q_OBJECT

public:
    QRfR1CDlg *dlgRfR1C;
    QVBoxLayout *layout;

public:
    explicit QRfR1CContainerDlg(QWidget *parent = 0);
    ~QRfR1CContainerDlg();

protected:
    void showEvent(QShowEvent *event);
};

#endif // QRFR1CCONTAINERDLG_H
