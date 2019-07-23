#ifndef QTESTR1CRXFREQRESPONSETABWIDGET_H
#define QTESTR1CRXFREQRESPONSETABWIDGET_H

#include <QTabWidget>

struct TestParam;
class QCalPlot;
class QwtPlotCurve;
class QRDTTableView;
class QR1CRxFreqResponseModel;
class QwtRxFreqResponseData;

class QTestR1CRxFreqResponseWidget : public QWidget
{
    Q_OBJECT
    friend class QTestR1CRxFreqResponseTabWidget;
    friend class QTestR1CRxFreqResponseDlg;

public:
    explicit QTestR1CRxFreqResponseWidget(QWidget *parent = 0);
    void init(QR1CRxFreqResponseModel *model);
    void resetShowWidget(qint32 pts, qint32 sec);

private:
    QCalPlot *plot;
    QVector<QwtPlotCurve *> *curve;
    QRDTTableView *tableView;
    QR1CRxFreqResponseModel *model;
};

class QTestR1CRxFreqResponseTabWidget : public QTabWidget
{
    Q_OBJECT
    friend class QTestR1CRxFreqResponseDlg;

public:
    explicit QTestR1CRxFreqResponseTabWidget(QWidget *parent = 0);

public:
    void init(QR1CRxFreqResponseModel *modelOP,QR1CRxFreqResponseModel *modelIO);
    void resetShowWidget(TestParam *param);

public:
    QTestR1CRxFreqResponseWidget *widgetOP_80;
    QTestR1CRxFreqResponseWidget *widgetIO_80;
};

#endif // QCALR1CTXFILTEROFFSETTABWIDGET_H
