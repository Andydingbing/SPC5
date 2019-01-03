#include "qcalr1crxfilteroffsettabwidget.h"
#include <QHBoxLayout>
#include "qcalplot.h"
#include "qwt_plot_curve.h"
#include "qrdttableview.h"
#include "qr1crxfilteroffsetmodel.h"
#include "qwinthread.h"

// QCalR1CRxFilterOffsetWidget
QCalR1CRxFilterOffsetWidget::QCalR1CRxFilterOffsetWidget(QWidget *parent) :
    QWidget(parent)
{
    model = NULL;

    plot = new QCalPlot(this);
    plot->init(RF_RX_FREQ_STAR/1000000.0,RF_RX_FREQ_STOP/1000000.0,-1.0,1.0);
    QwtText textTitle = plot->title();
    textTitle.setColor(QColor(Qt::blue));
    plot->setTitle(textTitle);
    plot->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    tableView = new QRDTTableView(this);
    tableView->setColumnWidth(0,80);
    tableView->setColumnWidth(1,70);
    tableView->setColumnWidth(2,100);
    tableView->setColumnWidth(3,100);
    tableView->setColumnWidth(4,100);
    tableView->setColumnWidth(5,100);
    tableView->setColumnWidth(6,140);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(tableView);
    layout->addWidget(plot);
    layout->setStretch(0,10);
    layout->setStretch(1,1);
    layout->setSpacing(0);
    layout->setMargin(0);

    curve = new QVector<QwtPlotCurve *>[3];
}

void QCalR1CRxFilterOffsetWidget::init(QR1CRxFilterOffsetModel *model)
{
    this->model = model;
    tableView->setModel(model);
}

void QCalR1CRxFilterOffsetWidget::resetShowWidget(qint32 pts, qint32 sec)
{
    rx_filter_offset_table::data_f_t data;

    model->resetData();

    for (qint32 i = 0;i < pts;i ++)
        model->calTable()->append(data);

    for (qint32 i = 0;i < ARRAY_SIZE(data.offset);i ++) {
        for (qint32 j = 0;j < curve[i].size();j ++)
            curve[i].at(j)->detach();
        curve[i].clear();

        for (qint32 j = 0;j < sec;j ++) {
            model->iterTable(i)->append(new QwtRxFilterOffsetData(i));
            curve[i].append(new QwtPlotCurve);
            curve[i].at(j)->setPen(QColor(Qt::red));
            curve[i].at(j)->attach(plot);
            curve[i].at(j)->setSamples(model->iterTable(i)->at(j));
        }
    }

    tableView->selectRow(0);
    plot->replot();
}

// QCalR1CRxFilterOffsetTabWidget
QCalR1CRxFilterOffsetTabWidget::QCalR1CRxFilterOffsetTabWidget(QWidget *parent) :
    QTabWidget(parent)
{
    QWidget *widget_80 = new QWidget;
    widgetOP_80 = new QCalR1CRxFilterOffsetWidget(widget_80);
    widgetIO_80 = new QCalR1CRxFilterOffsetWidget(widget_80);
    QwtText textTitle = widgetOP_80->plot->title();
    textTitle.setText(tr("Filter Offset Curve(X:Freq(MHz) Y:Offset(dB))(Output)"));
    widgetOP_80->plot->setTitle(textTitle);
    textTitle.setText(tr("Filter Offset Curve(X:Freq(MHz) Y:Offset(dB))(IO)"));
    widgetIO_80->plot->setTitle(textTitle);

    QHBoxLayout *layout_80 = new QHBoxLayout(widget_80);
    layout_80->addWidget(widgetOP_80);
    layout_80->addWidget(widgetIO_80);
    layout_80->setStretch(0,1);
    layout_80->setStretch(1,1);
    layout_80->setSpacing(0);
    layout_80->setMargin(0);

    this->clear();
    this->addTab(widget_80,QString("80M"));
}

void QCalR1CRxFilterOffsetTabWidget::init(QR1CRxFilterOffsetModel *modelOP,QR1CRxFilterOffsetModel *modelIO)
{
    widgetOP_80->init(modelOP);
    widgetIO_80->init(modelIO);
}

void QCalR1CRxFilterOffsetTabWidget::resetShowWidget(CalParam *param)
{
    qint64 freqStar = 0;
    qint64 freqStop = 0;
    qint32 pts = freq2array(param->rfFreqStar,param->rfFreqStop,param->rfFreqStep,NULL,NULL,NULL,&freqStop,&freqStar);
    qint32 sec = str_sections(param->rfFreqStar.toStdString().c_str());

    if (calOP(param->mode))
        widgetOP_80->resetShowWidget(pts, sec);

    if (calIO(param->mode))
        widgetIO_80->resetShowWidget(pts, sec);
}
