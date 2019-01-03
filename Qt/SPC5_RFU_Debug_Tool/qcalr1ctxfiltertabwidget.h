#ifndef QCALR1CTXFILTERTABWIDGET_H
#define QCALR1CTXFILTERTABWIDGET_H

#include <QTabWidget>

class QCalPlot;
class QwtPlotCurve;
class QRDTTableView;
class QR1CTxFilterModel;
class QwtTxRfFrData;
class QwtTxIfFrData;

class QCalR1CTxFilterTabWidget : public QTabWidget
{
    Q_OBJECT

public:
    QCalPlot *plotRf;
    QCalPlot *plotIf;
    QwtPlotCurve *curveRfFr0;
    QwtPlotCurve *curveRfFr1;
    QwtPlotCurve *curveIfFr;
    QRDTTableView *tableViewData_80;
    QRDTTableView *tableViewData_160;
    QR1CTxFilterModel *_model_80;
    QR1CTxFilterModel *_model_160;
    QwtTxRfFrData *dataRfFr_0;
    QwtTxRfFrData *dataRfFr_1;
    QwtTxIfFrData *dataIfFr;
public:
    void init(QR1CTxFilterModel *model_80,QR1CTxFilterModel *model_160);
    void resetShowWidget();

public:
    QCalR1CTxFilterTabWidget(QWidget *parent = 0);
};

#endif // QCALR1CTXFILTERTABWIDGET_H
