#include "qtestr1ctxfreqresponsetabwidget.h"
#include <QHBoxLayout>
#include "q_cal_plot.h"
#include "qwt_plot_curve.h"
#include "q_rdt_tableview.h"
#include "qr1ctxfreqresponsemodel.h"
#include "q_winthread.h"
#include "algorithm.h"

QTestR1CTxFreqResponseWidget::QTestR1CTxFreqResponseWidget(QWidget *parent) : QWidget(parent)
{
    model = NULL;

    plot = new QCalPlot(this);
    plot->init(RF_TX_FREQ_STAR/1000000.0,RF_TX_FREQ_STOP/1000000.0,-20.0,15.0);
    QwtText textTitle = plot->title();
    textTitle.setColor(QColor(Qt::blue));
    plot->setTitle(textTitle);
    plot->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

//    tableView = new QRDTTableView(this);
//    tableView->setColumnWidth(0,80);
//    tableView->setColumnWidth(1,70);
//    tableView->setColumnWidth(2,100);
//    tableView->setColumnWidth(3,100);
//    tableView->setColumnWidth(4,100);
//    tableView->setColumnWidth(5,100);
//    tableView->setColumnWidth(6,140);

    QVBoxLayout *layout = new QVBoxLayout(this);
    //    layout->addWidget(tableView);
    layout->addWidget(plot);
    layout->setStretch(0,10);
    layout->setStretch(1,1);
    layout->setSpacing(0);
    layout->setMargin(0);

    curve = new QVector<QwtPlotCurve *>;
}

void QTestR1CTxFreqResponseWidget::init(QR1CTxFreqResponseModel *model)
{
    this->model = model;
 //   tableView->setModel(model);
}

void QTestR1CTxFreqResponseWidget::resetShowWidget(qint32 pts, qint32 sec)
{
    tx_if_fr_table::data_f_t data;

    model->resetData();

    for (qint32 i = 0;i < pts;i ++)
        model->calTable()->append(data);

    for (qint32 i = 0;i < curve->size();i ++)
        curve->at(i)->detach();
    curve->clear();

    for (qint32 i = 0;i < sec;i ++) {
        model->iterTable()->append(new QwtTxFreqResponseData);
        curve->append(new QwtPlotCurve);
        curve->at(i)->setPen(QColor(Qt::red));
        curve->at(i)->attach(plot);
        curve->at(i)->setSamples(model->iterTable()->at(i));
    }

    //tableView->selectRow(0);
    plot->replot();
}

// QCalR1CTxFreqResponseTabWidget
QTestR1CTxFreqResponseTabWidget::QTestR1CTxFreqResponseTabWidget(QWidget *parent) :
    QTabWidget(parent)
{
    QWidget *widget_80 = new QWidget;
    widgetOP_80 = new QTestR1CTxFreqResponseWidget(widget_80);
    widgetIO_80 = new QTestR1CTxFreqResponseWidget(widget_80);
    QwtText textTitle = widgetOP_80->plot->title();
    textTitle.setText(tr("TxRfFreqResponse Curve(X:Freq(MHz) Y:Pow(dB))"));
    widgetOP_80->plot->setTitle(textTitle);
    textTitle.setText(tr("TxIfFreqResponse Curve(X:Freq(MHz) Y:Pow(dB))"));
    widgetIO_80->plot->setTitle(textTitle);
    widgetIO_80->plot->init(IF_TX_FILTER_160M_FREQ_STAR/1000000.0,IF_TX_FILTER_160M_FREQ_STOP/1000000.0,-15.0,18.0);
    QHBoxLayout *layout_80 = new QHBoxLayout(widget_80);
    layout_80->addWidget(widgetOP_80);
    layout_80->addWidget(widgetIO_80);
    layout_80->setStretch(0,1);
    layout_80->setStretch(1,1);
    layout_80->setSpacing(0);
    layout_80->setMargin(0);

    this->clear();
    this->addTab(widget_80,QString("--"));
}

void QTestR1CTxFreqResponseTabWidget::init(QR1CTxFreqResponseModel *modelOP,QR1CTxFreqResponseModel *modelIO)
{
    widgetOP_80->init(modelOP);
    widgetIO_80->init(modelIO);
}

void QTestR1CTxFreqResponseTabWidget::resetShowWidget(TestParam *param)
{
    int64_t freqStar = 0;
    int64_t freqStop = 0;
    int32_t pts = freq2array(param->rfFreqStar,param->rfFreqStop,param->rfFreqStep,nullptr,nullptr,nullptr,&freqStop,&freqStar);
    int32_t sec = str_sections(param->rfFreqStar.toStdString().c_str());

//    if (calOP(param->mode))
//        widgetOP_80->resetShowWidget(pts, sec);

//    if (calIO(param->mode))
//        widgetIO_80->resetShowWidget(pts, sec);
    widgetOP_80->resetShowWidget(pts, sec);
    widgetIO_80->resetShowWidget(pts, sec);
}
