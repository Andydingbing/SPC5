#ifndef QRFR1CADVDLG_H
#define QRFR1CADVDLG_H

#include <QDialog>

namespace Ui {
class QRfR1CAdvDlg;
}

class QRfR1CAdvDlg : public QDialog
{
    Q_OBJECT

public:
    explicit QRfR1CAdvDlg(QWidget *parent = 0);
    ~QRfR1CAdvDlg();

private:
    Ui::QRfR1CAdvDlg *ui;
};

#endif // QRFR1CADVDLG_H
