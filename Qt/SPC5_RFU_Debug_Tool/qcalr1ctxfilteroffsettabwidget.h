#ifndef QCALR1CTXFILTEROFFSETTABWIDGET_H
#define QCALR1CTXFILTEROFFSETTABWIDGET_H

#include <QTabWidget>

struct CalParam;
class QCalPlot;
class QwtPlotCurve;
class QRDTTableView;
class QR1CTxFilterOffsetModel;
class QwtTxFilterOffsetData;

class QCalR1CTxFilterOffsetWidget : public QWidget
{
    Q_OBJECT
    friend class QCalR1CTxFilterOffsetTabWidget;
    friend class QCalR1CTxFilterOffsetDlg;

public:
    explicit QCalR1CTxFilterOffsetWidget(QWidget *parent = 0);
    void init(QR1CTxFilterOffsetModel *model);
    void resetShowWidget(qint32 pts, qint32 sec);

private:
    QCalPlot *plot;
    QVector<QwtPlotCurve *> *curve;
    QRDTTableView *tableView;
    QR1CTxFilterOffsetModel *model;
};

class QCalR1CTxFilterOffsetTabWidget : public QTabWidget
{
    Q_OBJECT
    friend class QCalR1CTxFilterOffsetDlg;

public:
    explicit QCalR1CTxFilterOffsetTabWidget(QWidget *parent = 0);

public:
    void init(QR1CTxFilterOffsetModel *modelOP,QR1CTxFilterOffsetModel *modelIO);
    void resetShowWidget(CalParam *param);

private:
    QCalR1CTxFilterOffsetWidget *widgetOP_80;
    QCalR1CTxFilterOffsetWidget *widgetIO_80;
};

#endif // QCALR1CTXFILTEROFFSETTABWIDGET_H
