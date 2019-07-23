#ifndef Q_CAL_RX_FILTER_OFFSET_TABWIDGET_H
#define Q_CAL_RX_FILTER_OFFSET_TABWIDGET_H

#include <QTabWidget>

struct CalParam;
class QCalPlot;
class QwtPlotCurve;
class QRDTTableView;
class QRXFilterOffsetModel;
class QwtRxFilterOffsetData;

class QCalRXFilterOffsetWidget : public QWidget
{
    Q_OBJECT
    friend class QCalRXFilterOffsetTabWidget;
    friend class QCalR1CRXFilterOffsetDlg;

public:
    explicit QCalRXFilterOffsetWidget(QWidget *parent = nullptr);
    void init(QRXFilterOffsetModel *model);
    void resetShowWidget(qint32 pts, qint32 sec);

private:
    QCalPlot *plot;
    QVector<QwtPlotCurve *> *curve;
    QRDTTableView *tableView;
    QRXFilterOffsetModel *model;
};


class QCalRXFilterOffsetTabWidget : public QTabWidget
{
    Q_OBJECT
    friend class QCalR1CRXFilterOffsetDlg;

public:
    explicit QCalRXFilterOffsetTabWidget(QWidget *parent = nullptr);
    void init(QRXFilterOffsetModel *model);
    void resetShowWidget(CalParam *param);

private:
    QCalRXFilterOffsetWidget *widget_80;
};

#endif // Q_CAL_RX_FILTER_OFFSET_TABWIDGET_H
