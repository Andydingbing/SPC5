#ifndef QCALR1CRXFILTEROFFSETTABWIDGET_H
#define QCALR1CRXFILTEROFFSETTABWIDGET_H

#include <QTabWidget>

struct CalParam;
class QCalPlot;
class QwtPlotCurve;
class QRDTTableView;
class QR1CRxFilterOffsetModel;
class QwtRxFilterOffsetData;

class QCalR1CRxFilterOffsetWidget : public QWidget
{
    Q_OBJECT
    friend class QCalR1CRxFilterOffsetTabWidget;
    friend class QCalR1CRxFilterOffsetDlg;

public:
    explicit QCalR1CRxFilterOffsetWidget(QWidget *parent = 0);
    void init(QR1CRxFilterOffsetModel *model);
    void resetShowWidget(qint32 pts, qint32 sec);

private:
    QCalPlot *plot;
    QVector<QwtPlotCurve *> *curve;
    QRDTTableView *tableView;
    QR1CRxFilterOffsetModel *model;
};

class QCalR1CRxFilterOffsetTabWidget : public QTabWidget
{
    Q_OBJECT
    friend class QCalR1CRxFilterOffsetDlg;

public:
    explicit QCalR1CRxFilterOffsetTabWidget(QWidget *parent = 0);

public:
    void init(QR1CRxFilterOffsetModel *modelOP,QR1CRxFilterOffsetModel *modelIO);
    void resetShowWidget(CalParam *param);

private:
    QCalR1CRxFilterOffsetWidget *widgetOP_80;
    QCalR1CRxFilterOffsetWidget *widgetIO_80;
};

#endif // QCALR1CRXFILTEROFFSETTABWIDGET_H
