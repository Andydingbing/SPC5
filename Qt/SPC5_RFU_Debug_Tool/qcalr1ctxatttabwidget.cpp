#include "qcalr1ctxatttabwidget.h"
#include <QHBoxLayout>
#include "qcalplot.h"
#include "qwt_plot_curve.h"
#include "qrdttableview.h"
#include "qr1ctxattmodel.h"
#include "qwinthread.h"

QCalR1CTxAttTabWidget::QCalR1CTxAttTabWidget(QWidget *parent) :
    QTabWidget(parent)
{
    _modelOP = NULL;
    _modelIO = NULL;
    QWidget *plotWidget = new QWidget;
    QWidget *sheetWidget = new QWidget;

    plotOP = new QCalPlot(plotWidget);
    plotIO = new QCalPlot(plotWidget);
    plotOP->init(RF_TX_FREQ_STAR/1000000.0,RF_TX_FREQ_STOP/1000000.0,-4.0,3.0);
    plotIO->init(RF_TX_FREQ_STAR/1000000.0,RF_TX_FREQ_STOP/1000000.0,-4.0,3.0);
    QwtText textPlotOP = plotOP->title();
    QwtText textPlotIO = plotIO->title();
    textPlotOP.setText(tr("Att Offset Curve(X:Freq(MHz) Y:Offset(dB))(Output)"));
    textPlotIO.setText(tr("Att Offset Curve(X:Freq(MHz) Y:Offset(dB))(IO)"));
    textPlotOP.setColor(QColor(Qt::blue));
    textPlotIO.setColor(QColor(Qt::blue));
    plotOP->setTitle(textPlotOP);
    plotIO->setTitle(textPlotIO);
    plotOP->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    plotIO->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    QHBoxLayout *plotLayout = new QHBoxLayout(plotWidget);
    plotLayout->addWidget(plotOP);
    plotLayout->addWidget(plotIO);
    plotLayout->setStretch(0,1);
    plotLayout->setStretch(1,1);

    tableViewOP = new QRDTTableView(this);
    tableViewIO = new QRDTTableView(this);
    QHBoxLayout *sheetLayout = new QHBoxLayout(sheetWidget);
    sheetLayout->addWidget(tableViewOP);
    sheetLayout->addWidget(tableViewIO);
    sheetLayout->setStretch(0,1);
    sheetLayout->setStretch(1,1);

    curveOP = new QVector<QwtPlotCurve *>[R1C_TX_ATT_OP_POWER_PTS];
    curveIO = new QVector<QwtPlotCurve *>[R1C_TX_ATT_IO_POWER_PTS];
    this->clear();
    this->addTab(plotWidget,QString("Plot"));
    this->addTab(sheetWidget,QString("Sheet"));
}

void QCalR1CTxAttTabWidget::init(QR1CTxAttOPModel *modelOP,QR1CTxAttIOModel *modelIO)
{
    _modelOP = modelOP;
    _modelIO = modelIO;

    tableViewOP->setModel(_modelOP);
    tableViewOP->setColumnWidth(0,80);
    tableViewOP->setColumnWidth(1,60);
    tableViewOP->setColumnWidth(2,40);
    tableViewOP->setColumnWidth(3,40);
    tableViewOP->setColumnWidth(4,40);
    tableViewOP->setColumnWidth(5,40);
    tableViewOP->setColumnWidth(6,70);
    tableViewOP->setColumnWidth(7,70);
    tableViewOP->setColumnWidth(8,100);
    tableViewOP->setColumnWidth(9,100);
    tableViewOP->setColumnWidth(10,100);
    tableViewOP->setColumnWidth(11,100);
    tableViewOP->setColumnWidth(12,120);

    tableViewIO->setModel(_modelIO);
    tableViewIO->setColumnWidth(0,80);
    tableViewIO->setColumnWidth(1,60);
    tableViewIO->setColumnWidth(2,40);
    tableViewIO->setColumnWidth(3,40);
    tableViewIO->setColumnWidth(4,40);
    tableViewIO->setColumnWidth(5,40);
    tableViewIO->setColumnWidth(6,70);
    tableViewIO->setColumnWidth(7,70);
    tableViewIO->setColumnWidth(8,100);
    tableViewIO->setColumnWidth(9,100);
    tableViewIO->setColumnWidth(10,100);
    tableViewIO->setColumnWidth(11,100);
    tableViewIO->setColumnWidth(12,120);
}

void QCalR1CTxAttTabWidget::resetShowWidget(CalParam *param)
{
    int64_t freqStar = 0;
    int64_t freqStop = 0;
    int32_t pts = freq2array(param->rfFreqStar,param->rfFreqStop,param->rfFreqStep,NULL,NULL,NULL,&freqStop,&freqStar);
    int32_t sec = str_sections(param->rfFreqStar.toStdString().c_str());

    /*!
     * Color table taken from CChartCtrl library,which is a chart control library used in MFC.
     * Sources are in directory rd/lib/ChartCtrl.Color table defined in ChartCtrl.cpp,line 85.
     */
    QColor clrTable[] = {
        QColor(RGB(255,0,0)),
        QColor(RGB(0,150,0)),
        QColor(RGB(0,0,255)),
        QColor(RGB(255,255,0)),
        QColor(RGB(0,255,255)),
        QColor(RGB(255,128,0)),
        QColor(RGB(128,0,128)),
        QColor(RGB(128,128,0)),
        QColor(RGB(255,0,255)),
        QColor(RGB(64,128,128))
    };
    int clrTableSize = ARRAY_SIZE(clrTable);

    if (calOP(param->mode)) {
        if (_modelOP)
            _modelOP->resetData();

        tx_att_op_table_r1cd::data_f_t data;
        for (qint32 i = 0;i < pts;i ++)
            _modelOP->calTable()->append(data);

        for (qint32 i = 0;i < R1C_TX_ATT_OP_POWER_PTS;i ++) {
            for (qint32 j = 0;j < curveOP[i].size();j ++)
                curveOP[i].at(j)->detach();
            curveOP[i].clear();

            for (qint32 j = 0;j < sec;j ++) {
                _modelOP->iterTable(i)->append(new QwtTxAttOPData(i));
                curveOP[i].append(new QwtPlotCurve);
                curveOP[i].at(j)->setPen(clrTable[i % clrTableSize]);
                curveOP[i].at(j)->attach(plotOP);
                curveOP[i].at(j)->setSamples(_modelOP->iterTable(i)->at(j));
            }
        }

        tableViewOP->selectRow(0);
        plotOP->replot();
    }

    if (calIO(param->mode)) {
        if (_modelIO)
            _modelIO->resetData();

        tx_att_io_table_r1cd::data_f_t data;
        for (qint32 i = 0;i < pts;i ++)
            _modelIO->calTable()->append(data);

        for (qint32 i = 0;i < R1C_TX_ATT_IO_POWER_PTS;i ++) {
            for (qint32 j = 0;j < curveIO[i].size();j ++)
                curveIO[i].at(j)->detach();
            curveIO[i].clear();

            for (qint32 j = 0;j < sec;j ++) {
                _modelIO->iterTable(i)->append(new QwtTxAttIOData(i));
                curveIO[i].append(new QwtPlotCurve);
                curveIO[i].at(j)->setPen(clrTable[i % clrTableSize]);
                curveIO[i].at(j)->attach(plotIO);
                curveIO[i].at(j)->setSamples(_modelIO->iterTable(i)->at(j));
            }
        }

        tableViewIO->selectRow(0);
        plotIO->replot();
    }
}
