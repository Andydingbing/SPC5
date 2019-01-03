#ifndef QRFR1ACONTAINERDLG_H
#define QRFR1ACONTAINERDLG_H

#include <QWidget>
#include <QVBoxLayout>
#include <QShowEvent>
#include "qrfr1adlg.h"
#include "qtempctrldlg.h"

class QRfR1AContainerDlg : public QWidget
{
    Q_OBJECT

public:
    QRfR1ADlg *dlgRfR1A;
    QVBoxLayout *layout;

public:
    explicit QRfR1AContainerDlg(QWidget *parent = 0);
    ~QRfR1AContainerDlg();

protected:
    void showEvent(QShowEvent *event);
};

#endif // QRFR1ACONTAINERDLG_H
