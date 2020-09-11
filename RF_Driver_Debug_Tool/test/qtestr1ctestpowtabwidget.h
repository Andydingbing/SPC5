#ifndef QTESTR1CTESTPOWTABWIDGET_H
#define QTESTR1CTESTPOWTABWIDGET_H

#include <QTabWidget>

struct TestParam;
class QCalPlot;
class QwtPlotCurve;
class QRDTTableView;
class QR1CTestPowOPModel;
class QR1CTestPowIOModel;
class QwtTestPowOPData;
class QwtTestPowIOData;

class QTestR1CTestPowTabWidget : public QTabWidget
{
    Q_OBJECT

public:
    QCalPlot *plotOP;
    QCalPlot *plotIO;
    QVector<QwtPlotCurve *> *curveOP;
    QVector<QwtPlotCurve *> *curveIO;
    QRDTTableView *tableViewOP;
    QRDTTableView *tableViewIO;
    QR1CTestPowOPModel *_modelOP;
    QR1CTestPowIOModel *_modelIO;
public:
    void init(QR1CTestPowOPModel *modelOP,QR1CTestPowIOModel *modelIO);
    void resetShowWidget(TestParam *param);

public:
    QTestR1CTestPowTabWidget(QWidget *parent = 0);
};

#endif // QCALR1CTESTPOWTABWIDGET_H
