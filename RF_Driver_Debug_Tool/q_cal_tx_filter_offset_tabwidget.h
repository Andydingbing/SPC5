#ifndef Q_CAL_TX_FILTER_OFFSET_TABWIDGET_H
#define Q_CAL_TX_FILTER_OFFSET_TABWIDGET_H

#include "q_model_filter_offset.h"
#include <QTabWidget>

struct CalParam;
class QCalPlot;
class QwtPlotCurve;
class QRDTTableView;

class QCalTXFilterOffsetWidget : public QWidget
{
    Q_OBJECT
    friend class QCalTXFilterOffsetTabWidget;
    friend class QCalR1CTXFilterOffsetDlg;

public:
    explicit QCalTXFilterOffsetWidget(QWidget *parent = nullptr);
    void init(QTXFilterOffsetModel *model);
    void resetShowWidget(qint32 pts, qint32 sec);

private:
    QCalPlot *plot;
    QVector<QwtPlotCurve *> *curve;
    QRDTTableView *tableView;
    QTXFilterOffsetModel *model;
};


class QCalTXFilterOffsetTabWidget : public QTabWidget
{
    Q_OBJECT
    friend class QCalR1CTXFilterOffsetDlg;

public:
    explicit QCalTXFilterOffsetTabWidget(QWidget *parent = nullptr);
    void init(QTXFilterOffsetModel *modelOP);
    void resetShowWidget(CalParam *param);

private:
    QCalTXFilterOffsetWidget *widget_80;
};

#endif // Q_CAL_TX_FILTER_OFFSET_TABWIDGET_H
