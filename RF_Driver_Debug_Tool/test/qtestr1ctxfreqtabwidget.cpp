#include "qtestr1ctxfreqtabwidget.h"
#include <QVBoxLayout>
#include "q_cal_plot.h"
#include "qwt_plot_curve.h"
#include "q_rdt_tableview.h"
#include "qr1ctxfreqmodel.h"

QTestR1CTxFreqTabWidget::QTestR1CTxFreqTabWidget(QWidget *parent) :
    QTabWidget(parent)
{
    _model_80 = nullptr;
    _model_160 = nullptr;
    QWidget *plotWidget = new QWidget;
    plotRf = new QCalPlot(plotWidget);
    plotRf->init(RF_TX_FILTER_FREQ_STAR_0/1000000.0,RF_TX_FILTER_FREQ_STOP_1/1000000.0,-2,5);
    QwtText textPlotRf = plotRf->title();
    textPlotRf.setText(tr("RF Freq Response Curve(X:Freq(MHz) Y:Power(dBm))"));
    textPlotRf.setColor(QColor(Qt::blue));
    plotRf->setTitle(textPlotRf);
    plotRf->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    plotIf = new QCalPlot(plotWidget);
    plotIf->init(IF_TX_FILTER_160M_FREQ_STAR/1000000.0,IF_TX_FILTER_160M_FREQ_STOP/1000000.0,-10,5);
    QwtText textPlotIf = plotIf->title();
    textPlotIf.setText(tr("IF Freq Response(@RF 2GHz) Curve(X:Freq(MHz) Y:Power(dBm))"));
    textPlotIf.setColor(QColor(Qt::blue));
    plotIf->setTitle(textPlotIf);
    plotIf->title().setColor(QColor(Qt::blue));
    plotIf->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    QVBoxLayout *plotLayout = new QVBoxLayout(plotWidget);
    plotLayout->addWidget(plotRf);
    plotLayout->addWidget(plotIf);
    plotLayout->setStretch(0,1);
    plotLayout->setStretch(1,1);

    dataRfFr_0 = new QwtTxRfFrTestData;
    dataRfFr_0->calTable()->clear();
    dataRfFr_1 = new QwtTxRfFrTestData;
    dataRfFr_1->calTable()->clear();
    dataIfFr = new QwtTxIfFrTestData;
    dataIfFr->calTable()->clear();

    curveRfFr0 = new QwtPlotCurve("RF Freq Response 0");
    curveRfFr0->setPen(QColor(Qt::red));
    curveRfFr0->setVisible(true);
    curveRfFr0->attach(plotRf);
    curveRfFr0->setSamples(dataRfFr_0);

    curveRfFr1 = new QwtPlotCurve("RF Freq Response 1");
    curveRfFr1->setPen(QColor(Qt::green));
    curveRfFr1->setVisible(true);
    curveRfFr1->attach(plotRf);
    curveRfFr1->setSamples(dataRfFr_1);

    curveIfFr = new QwtPlotCurve("IF Freq Response");
    curveIfFr->setPen(QColor(Qt::red));
    curveIfFr->setVisible(true);
    curveIfFr->attach(plotIf);
    curveIfFr->setSamples(dataIfFr);

    // tableViewData_80 = new QRDTTableView(this);
    // tableViewData_160 = new QRDTTableView(this);

    this->clear();
    this->addTab(plotWidget,QString("Freq Response"));
    // this->addTab(tableViewData_80,QString("Coef_80M"));
    // this->addTab(tableViewData_160,QString("Coef_160M"));
}

void QTestR1CTxFreqTabWidget::init(QR1CTxFreqModel *model_80, QR1CTxFreqModel *model_160)
{
     _model_80 = model_80;
     _model_160 = model_160;

    // tableViewData_80->setModel(_model_80);
    // tableViewData_80->setColumnWidth(0,80);
    // tableViewData_80->setColumnWidth(1,1000);
    // tableViewData_80->setColumnWidth(2,1000);
    // tableViewData_80->setColumnWidth(3,100);
    // tableViewData_80->setColumnWidth(4,100);
    // tableViewData_80->setColumnWidth(5,100);
    // tableViewData_80->setColumnWidth(6,100);
    // tableViewData_80->setColumnWidth(7,120);

    // tableViewData_160->setModel(_model_160);
    // tableViewData_160->setColumnWidth(0,80);
    // tableViewData_160->setColumnWidth(1,1000);
    // tableViewData_160->setColumnWidth(2,1000);
    // tableViewData_160->setColumnWidth(3,100);
    // tableViewData_160->setColumnWidth(4,100);
    // tableViewData_160->setColumnWidth(5,100);
    // tableViewData_160->setColumnWidth(6,100);
    // tableViewData_160->setColumnWidth(7,120);
}

void QTestR1CTxFreqTabWidget::resetShowWidget()
{
    if (_model_80)
        _model_80->calTable()->clear();
    if (_model_160)
        _model_160->calTable()->clear();

    dataRfFr_0->calTable()->clear();
    dataRfFr_1->calTable()->clear();
    dataIfFr->calTable()->clear();
    plotRf->replot();
    plotIf->replot();
}
