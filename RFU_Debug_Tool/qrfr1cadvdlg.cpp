#include "qrfr1cadvdlg.h"
#include "ui_qrfr1cadvdlg.h"

QRfR1CAdvDlg::QRfR1CAdvDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QRfR1CAdvDlg)
{
    ui->setupUi(this);
}

QRfR1CAdvDlg::~QRfR1CAdvDlg()
{
    delete ui;
}
