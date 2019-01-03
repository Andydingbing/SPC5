#ifndef QCALR1CRXFILTERTABWIDGET_H
#define QCALR1CRXFILTERTABWIDGET_H

#include <QTabWidget>

class QCalPlot;
class QwtPlotCurve;
class QRDTTableView;
class QR1CRxFilterModel_80M;
class QR1CRxFilterModel_160M;
class QwtRxRfFrData;
class QwtRxIfFrData;

class QCalR1CRxFilterTabWidget : public QTabWidget
{
    Q_OBJECT

public:
    QCalPlot *plotRf;
    QCalPlot *plotIf;
    QwtPlotCurve *curveRfFr;
    QwtPlotCurve *curveIfFr;
    QRDTTableView *tableViewData_80;
    QRDTTableView *tableViewData_160;
    QR1CRxFilterModel_80M *_model_80;
    QR1CRxFilterModel_160M *_model_160;
    QwtRxRfFrData *dataRfFr;
    QwtRxIfFrData *dataIfFr;
public:
    void init(QR1CRxFilterModel_80M *model_80,QR1CRxFilterModel_160M *model_160);
    void resetShowWidget();

public:
    QCalR1CRxFilterTabWidget(QWidget *parent = 0);
};

#endif // QCALR1CRXFILTERTABWIDGET_H
