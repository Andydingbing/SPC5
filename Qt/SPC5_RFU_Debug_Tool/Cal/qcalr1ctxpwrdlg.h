#ifndef QCALR1CTXPWRDLG_H
#define QCALR1CTXPWRDLG_H

#include "qcalbasedlg.h"
#include "qr1ctxpwrmodel.h"
#include "define.h"

namespace Ui {
class QCalR1CTxPwrDlg;
}

class QwtPlotCurve;

class QCalR1CTxPwrDlg : public QCalR1CBaseDlg
{
    Q_OBJECT
    DECLARE_CAL_R1C_DLG

public:
    QVector<QwtPlotCurve *> *curveOP;
    QVector<QwtPlotCurve *> *curveIO;

public:
    void resetCurve(QR1CTxPwrModel *model, QVector<QwtPlotCurve *> *curve, int pts, int sec, QColor color);

public:
    explicit QCalR1CTxPwrDlg(QWidget *parent = 0);
    ~QCalR1CTxPwrDlg();

private:
    Ui::QCalR1CTxPwrDlg *ui;
};

#endif // QCALR1CTXPWRDLG_H
