#ifndef QTESTR1CRXFREQTABWIDGET_H
#define QTESTR1CRXFREQTABWIDGET_H

#include <QTabWidget>

struct TestParam;
class QCalPlot;
class QwtPlotCurve;
class QRDTTableView;
class QR1CRxFreqModel;
class QwtRxFreqTestData;

class QTestR1CRxFreqWidget : public QWidget
{
    Q_OBJECT
    friend class QTestR1CRxFreqTabWidget;
    friend class QTestR1CRxFreqDlg;

public:
    explicit QTestR1CRxFreqWidget(QWidget *parent = 0);
    void init(QR1CRxFreqModel *model);
    void resetShowWidget(qint32 pts, qint32 sec);

private:
    QCalPlot *plot;
    QVector<QwtPlotCurve *> *curve;
    QRDTTableView *tableView;
    QR1CRxFreqModel *model;
};

class QTestR1CRxFreqTabWidget : public QTabWidget
{
    Q_OBJECT
    friend class QTestR1CRxFreqDlg;

public:
    explicit QTestR1CRxFreqTabWidget(QWidget *parent = 0);

public:
    void init(QR1CRxFreqModel *modelOP,QR1CRxFreqModel *modelIO);
    void resetShowWidget(TestParam *param);

public:
    QTestR1CRxFreqWidget *widgetOP_80;
    QTestR1CRxFreqWidget *widgetIO_80;
};

#endif // QTESTR1CRXFREQTABWIDGET_H
