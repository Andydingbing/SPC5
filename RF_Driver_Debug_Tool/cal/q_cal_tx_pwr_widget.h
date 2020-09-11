#ifndef Q_CAL_TX_PWR_WIDGET_H
#define Q_CAL_TX_PWR_WIDGET_H

#include <QWidget>

struct CalParam;

class QCalPlot;
class QwtPlotCurve;
class QRDTTableView;
class QTXPwrModel;

class QCalR1CTXPwrWidget : public QWidget
{
public:
    QCalR1CTXPwrWidget(QWidget *parent = nullptr);
    void init(QTXPwrModel *model_OP,QTXPwrModel *model_IO);
    void resetShowWidget(CalParam *param);

private:
    void resetCurve(QTXPwrModel *model,
                    QVector<QwtPlotCurve *> *curve,
                    quint32 pts,
                    qint32 sec,
                    QColor color);

public:
    QCalPlot *plotDGain;

    QVector<QwtPlotCurve *> *curveOP;
    QVector<QwtPlotCurve *> *curveIO;

    QRDTTableView *tableViewOP;
    QRDTTableView *tableViewIO;
    QTXPwrModel *modelOP;
    QTXPwrModel *modelIO;
};

#endif // Q_CAL_TX_PWR_WIDGET_H
