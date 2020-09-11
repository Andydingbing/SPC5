#ifndef QTESTR1CRXATTTABWIDGET_H
#define QTESTR1CRXATTTABWIDGET_H

#include <QTabWidget>

struct TestParam;
class QCalPlot;
class QwtPlotCurve;
class QRDTTableView;
class QR1CRxAttTestOPModel;
class QR1CRxAttTestIOModel;
class QwtRxAttTestOPData;
class QwtRxAttTestIOData;

class QTestR1CRxAttTabWidget : public QTabWidget
{
    Q_OBJECT

public:
    QCalPlot *plotOP;
    QCalPlot *plotIO;
    QVector<QwtPlotCurve *> *curveOP;
    QVector<QwtPlotCurve *> *curveIO;
    QRDTTableView *tableViewOP;
    QRDTTableView *tableViewIO;
    QR1CRxAttTestOPModel *_modelOP;
    QR1CRxAttTestIOModel *_modelIO;
public:
    void init(QR1CRxAttTestOPModel *modelOP,QR1CRxAttTestIOModel *modelIO);
    void resetShowWidget(TestParam *param);

public:
    QTestR1CRxAttTabWidget(QWidget *parent = 0);
};

#endif // QTESTR1CRXATTTABWIDGET_H
