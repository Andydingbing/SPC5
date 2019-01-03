#ifndef QCALR1CTXATTTABWIDGET_H
#define QCALR1CTXATTTABWIDGET_H

#include <QTabWidget>

struct CalParam;
class QCalPlot;
class QwtPlotCurve;
class QRDTTableView;
class QR1CTxAttOPModel;
class QR1CTxAttIOModel;
class QwtTxAttOPData;
class QwtTxAttIOData;

class QCalR1CTxAttTabWidget : public QTabWidget
{
    Q_OBJECT

public:
    QCalPlot *plotOP;
    QCalPlot *plotIO;
    QVector<QwtPlotCurve *> *curveOP;
    QVector<QwtPlotCurve *> *curveIO;
    QRDTTableView *tableViewOP;
    QRDTTableView *tableViewIO;
    QR1CTxAttOPModel *_modelOP;
    QR1CTxAttIOModel *_modelIO;
public:
    void init(QR1CTxAttOPModel *modelOP,QR1CTxAttIOModel *modelIO);
    void resetShowWidget(CalParam *param);

public:
    QCalR1CTxAttTabWidget(QWidget *parent = 0);
};

#endif // QCALR1CTXATTTABWIDGET_H
