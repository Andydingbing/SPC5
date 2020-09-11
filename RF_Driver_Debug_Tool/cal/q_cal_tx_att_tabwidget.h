#ifndef Q_CAL_TX_ATT_TABWIDGET_H
#define Q_CAL_TX_ATT_TABWIDGET_H

#include <QTabWidget>

struct CalParam;

class QCalPlot;
class QwtPlotCurve;
class QRDTTableView;
class QR1CTXAttOPModel;
class QR1CTXAttIOModel;

class QCalR1CTXAttTabWidget : public QTabWidget
{
public:
    QCalR1CTXAttTabWidget(QWidget *parent = nullptr);
    void init(QR1CTXAttOPModel *model_OP,QR1CTXAttIOModel *model_IO);
    void resetShowWidget(CalParam *param);

public:
    QCalPlot *plotOP;
    QCalPlot *plotIO;
    QVector<QwtPlotCurve *> *curveOP;
    QVector<QwtPlotCurve *> *curveIO;
    QRDTTableView *tableViewOP;
    QRDTTableView *tableViewIO;
    QR1CTXAttOPModel *modelOP;
    QR1CTXAttIOModel *modelIO;
};

#endif // Q_CAL_TX_ATT_TABWIDGET_H
