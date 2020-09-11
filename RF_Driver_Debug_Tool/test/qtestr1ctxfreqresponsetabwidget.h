#ifndef QTESTR1CTXFREQRESPONSETABWIDGET_H
#define QTESTR1CTXFREQRESPONSETABWIDGET_H

#include <QTabWidget>

struct TestParam;
class QCalPlot;
class QwtPlotCurve;
class QRDTTableView;
class QR1CTxFreqResponseModel;
class QwtTxFreqResponseData;

class QTestR1CTxFreqResponseWidget : public QWidget
{
    Q_OBJECT
    friend class QTestR1CTxFreqResponseTabWidget;
    friend class QTestR1CTxFreqResponseDlg;

public:
    explicit QTestR1CTxFreqResponseWidget(QWidget *parent = 0);
    void init(QR1CTxFreqResponseModel *model);
    void resetShowWidget(qint32 pts, qint32 sec);

private:
    QCalPlot *plot;
    QVector<QwtPlotCurve *> *curve;
    QRDTTableView *tableView;
    QR1CTxFreqResponseModel *model;
};

class QTestR1CTxFreqResponseTabWidget : public QTabWidget
{
    Q_OBJECT
    friend class QTestR1CTxFreqResponseDlg;

public:
    explicit QTestR1CTxFreqResponseTabWidget(QWidget *parent = 0);

public:
    void init(QR1CTxFreqResponseModel *modelOP,QR1CTxFreqResponseModel *modelIO);
    void resetShowWidget(TestParam *param);

public:
    QTestR1CTxFreqResponseWidget *widgetOP_80;
    QTestR1CTxFreqResponseWidget *widgetIO_80;
};

#endif
