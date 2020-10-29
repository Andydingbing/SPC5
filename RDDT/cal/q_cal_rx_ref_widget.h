#ifndef Q_CAL_RX_REF_WIDGET_H
#define Q_CAL_RX_REF_WIDGET_H

#include <QWidget>

struct CalParam;

class QCalPlot;
class QwtPlotCurve;
class QRDTTableView;
class QR1CRXRefModel;

class QCalR1CRXRefWidget : public QWidget
{
public:
    QCalR1CRXRefWidget(QWidget *parent = nullptr);
    void init(QR1CRXRefModel *model_OP,QR1CRXRefModel *model_IO);
    void resetShowWidget(CalParam *param);

private:
    void resetCurve(QR1CRXRefModel *model,
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
    QR1CRXRefModel *modelOP;
    QR1CRXRefModel *modelIO;
};

#endif // Q_CAL_RX_REF_WIDGET_H
