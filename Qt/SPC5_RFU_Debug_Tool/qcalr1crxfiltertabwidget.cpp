#include "qcalr1crxfiltertabwidget.h"
#include <QVBoxLayout>
#include "qcalplot.h"
#include "qwt_plot_curve.h"
#include "qrdttableview.h"
#include "qr1crxfiltermodel.h"

QCalR1CRxFilterTabWidget::QCalR1CRxFilterTabWidget(QWidget *parent) :
    QTabWidget(parent)
{
    _model_80 = NULL;
    _model_160 = NULL;
    QWidget *plotWidget = new QWidget;
    plotRf = new QCalPlot(plotWidget);
    plotRf->init(RF_RX_FILTER_FREQ_STAR/1000000.0,RF_RX_FILTER_FREQ_STOP/1000000.0,-30.0,-5.0);
    QwtText titlePlotRf = plotRf->title();
    titlePlotRf.setText(tr("RF Freq Response Curve(X:Freq(MHz) Y:Power(dBm))"));
    titlePlotRf.setColor(QColor(Qt::blue));
    plotRf->setTitle(titlePlotRf);
    plotRf->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    plotIf = new QCalPlot(plotWidget);
    plotIf->init(IF_RX_FILTER_160M_FREQ_STAR/1000000.0,IF_RX_FILTER_160M_FREQ_STOP/1000000.0,-25.0,-5.0);
    QwtText titlePlotIf = plotIf->title();
    titlePlotIf.setText(tr("IF Freq Response(@RF 2GHz) Curve(X:Freq(MHz) Y:Power(dBm))"));
    titlePlotIf.setColor(QColor(Qt::blue));
    plotIf->setTitle(titlePlotIf);
    plotIf->title().setColor(QColor(Qt::blue));
    plotIf->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    QVBoxLayout *plotLayout = new QVBoxLayout(plotWidget);
    plotLayout->addWidget(plotRf);
    plotLayout->addWidget(plotIf);
    plotLayout->setStretch(0,1);
    plotLayout->setStretch(1,1);

    dataRfFr = new QwtRxRfFrData;
    dataRfFr->calTable()->clear();
    dataIfFr = new QwtRxIfFrData;
    dataIfFr->calTable()->clear();

    curveRfFr = new QwtPlotCurve("RF Freq Response");
    curveRfFr->setPen(QColor(Qt::red));
    curveRfFr->setVisible(true);
    curveRfFr->attach(plotRf);
    curveRfFr->setSamples(dataRfFr);

    curveIfFr = new QwtPlotCurve("IF Freq Response");
    curveIfFr->setPen(QColor(Qt::red));
    curveIfFr->setVisible(true);
    curveIfFr->attach(plotIf);
    curveIfFr->setSamples(dataIfFr);

    tableViewData_80 = new QRDTTableView(this);
    tableViewData_160 = new QRDTTableView(this);

    this->clear();
    this->addTab(plotWidget,QString("Freq Response"));
    this->addTab(tableViewData_80,QString("Coef_80M"));
    this->addTab(tableViewData_160,QString("Coef_160M"));
}

void QCalR1CRxFilterTabWidget::init(QR1CRxFilterModel_80M *model_80,QR1CRxFilterModel_160M *model_160)
{
    _model_80 = model_80;
    _model_160 = model_160;

    tableViewData_80->setModel(_model_80);
    tableViewData_80->setColumnWidth(0,80);
    tableViewData_80->setColumnWidth(1,1000);
    tableViewData_80->setColumnWidth(2,1000);
    tableViewData_80->setColumnWidth(3,100);
    tableViewData_80->setColumnWidth(4,100);
    tableViewData_80->setColumnWidth(5,100);
    tableViewData_80->setColumnWidth(6,100);
    tableViewData_80->setColumnWidth(7,120);

    tableViewData_160->setModel(_model_160);
    tableViewData_160->setColumnWidth(0,80);
    tableViewData_160->setColumnWidth(1,1000);
    tableViewData_160->setColumnWidth(2,1000);
    tableViewData_160->setColumnWidth(3,100);
    tableViewData_160->setColumnWidth(4,100);
    tableViewData_160->setColumnWidth(5,100);
    tableViewData_160->setColumnWidth(6,100);
    tableViewData_160->setColumnWidth(7,120);
}

void QCalR1CRxFilterTabWidget::resetShowWidget()
{
    if (_model_80)
        _model_80->calTable()->clear();
    if (_model_160)
        _model_160->calTable()->clear();

    dataRfFr->calTable()->clear();
    dataIfFr->calTable()->clear();
    plotRf->replot();
    plotIf->replot();
}
