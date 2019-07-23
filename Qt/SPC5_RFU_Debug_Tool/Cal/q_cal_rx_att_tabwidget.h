#ifndef Q_CAL_RX_ATT_TABWIDGET_H
#define Q_CAL_RX_ATT_TABWIDGET_H

#include <QTabWidget>

struct CalParam;

class QCalPlot;
class QwtPlotCurve;
class QRDTTableView;
class QR1CRxAttOPModel;
class QR1CRxAttIOModel;
class QwtRxAttOPData;
class QwtRxAttIOData;

class QCalR1CRXAttTabWidget : public QTabWidget
{
public:
    QCalR1CRXAttTabWidget(QWidget *parent = nullptr);
    void init(QR1CRxAttOPModel *model_OP,QR1CRxAttIOModel *model_IO);
    void resetShowWidget(CalParam *param);

public:
    QCalPlot *plotOP;
    QCalPlot *plotIO;
    QVector<QwtPlotCurve *> *curveOP;
    QVector<QwtPlotCurve *> *curveIO;
    QRDTTableView *tableViewOP;
    QRDTTableView *tableViewIO;
    QR1CRxAttOPModel *modelOP;
    QR1CRxAttIOModel *modelIO;
};

#endif // Q_CAL_RX_ATT_TABWIDGET_H
