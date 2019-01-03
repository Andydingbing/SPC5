#ifndef QCALR1CRXREFDLG_H
#define QCALR1CRXREFDLG_H

#include "qcalbasedlg.h"
#include "QR1CRxRefModel.h"
#include "define.h"

namespace Ui {
class QCalR1CRxRefDlg;
}

class QwtPlotCurve;

class QCalR1CRxRefDlg : public QCalR1CBaseDlg
{
    Q_OBJECT
    DECLARE_CAL_R1C_DLG

public:
    QVector<QwtPlotCurve *> *curveOP;
    QVector<QwtPlotCurve *> *curveIO;

public:
    void resetCurve(QR1CRxRefModel *model, QVector<QwtPlotCurve *> *curve, int pts, int sec, QColor color);

public:
    explicit QCalR1CRxRefDlg(QWidget *parent = 0);
    ~QCalR1CRxRefDlg();

private:
    Ui::QCalR1CRxRefDlg *ui;
};

#endif // QCALR1CRXREFDLG_H
