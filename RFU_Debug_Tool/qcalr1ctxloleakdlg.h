#ifndef QCALR1CTXLOLEAKDLG_H
#define QCALR1CTXLOLEAKDLG_H

#include <QDialog>

namespace Ui {
class QCalR1CTxLOLeakDlg;
}

class QCalR1CTxLOLeakDlg : public QDialog
{
    Q_OBJECT

public:
    explicit QCalR1CTxLOLeakDlg(QWidget *parent = 0);
    ~QCalR1CTxLOLeakDlg();

private:
    Ui::QCalR1CTxLOLeakDlg *ui;
};

#endif // QCALR1CTXLOLEAKDLG_H
