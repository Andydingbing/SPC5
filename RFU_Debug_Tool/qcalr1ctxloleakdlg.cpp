#include "qcalr1ctxloleakdlg.h"
#include "ui_qcalr1ctxloleakdlg.h"

QCalR1CTxLOLeakDlg::QCalR1CTxLOLeakDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QCalR1CTxLOLeakDlg)
{
    ui->setupUi(this);
}

QCalR1CTxLOLeakDlg::~QCalR1CTxLOLeakDlg()
{
    delete ui;
}
