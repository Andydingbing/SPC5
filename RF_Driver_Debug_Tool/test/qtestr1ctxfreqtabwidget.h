#ifndef QTESTR1CTXFREQTABWIDGET_H
#define QTESTR1CTXFRERQTABWIDGET_H

#include <QTabWidget>

class QCalPlot;
class QwtPlotCurve;
class QRDTTableView;
class QR1CTxFreqModel;
class QwtTxRfFrTestData;
class QwtTxIfFrTestData;

class QTestR1CTxFreqTabWidget : public QTabWidget
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
    QR1CTxFreqModel *_model_80;
    QR1CTxFreqModel *_model_160;
    QwtTxRfFrTestData *dataRfFr_0;
    QwtTxRfFrTestData *dataRfFr_1;
    QwtTxIfFrTestData *dataIfFr;

public:
    void init(QR1CTxFreqModel *model_80,QR1CTxFreqModel *model_160);
    void resetShowWidget();

public:
    QTestR1CTxFreqTabWidget(QWidget *parent = 0);
};

#endif // QTESTR1CTXFREQTABWIDGET_H
