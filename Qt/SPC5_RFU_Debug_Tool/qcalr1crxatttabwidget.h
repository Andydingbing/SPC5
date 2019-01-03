#ifndef QCALR1CRXATTTABWIDGET_H
#define QCALR1CRXATTTABWIDGET_H

#include <QTabWidget>

struct CalParam;
class QCalPlot;
class QwtPlotCurve;
class QRDTTableView;
class QR1CRxAttOPModel;
class QR1CRxAttIOModel;
class QwtRxAttOPData;
class QwtRxAttIOData;

class QCalR1CRxAttTabWidget : public QTabWidget
{
    Q_OBJECT

public:
    QCalPlot *plotOP;
    QCalPlot *plotIO;
    QVector<QwtPlotCurve *> *curveOP;
    QVector<QwtPlotCurve *> *curveIO;
    QRDTTableView *tableViewOP;
    QRDTTableView *tableViewIO;
    QR1CRxAttOPModel *_modelOP;
    QR1CRxAttIOModel *_modelIO;
public:
    void init(QR1CRxAttOPModel *modelOP,QR1CRxAttIOModel *modelIO);
    void resetShowWidget(CalParam *param);

public:
    QCalR1CRxAttTabWidget(QWidget *parent = 0);
};

#endif // QCALR1CRXATTTABWIDGET_H
