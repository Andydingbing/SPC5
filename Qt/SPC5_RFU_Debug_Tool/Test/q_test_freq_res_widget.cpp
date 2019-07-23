#include "q_test_freq_res_widget.h"
#include "q_test_freq_res_thread.h"
#include "qwt_plot_curve.h"
#include "define.h"
#include <QVBoxLayout>

QTestFreqResWidget::QTestFreqResWidget(QWidget *parent) :
    QWidget(parent)
{
    QwtText title;
    plotRF = new QTestPlot(this);
    plotRF->init(RF_TX_FREQ_STAR / 1e6,
                 RF_TX_FREQ_STOP / 1e6,
                 -15.0,5.0);
    title = plotRF->title();
    title.setText(tr("RF T/RX Freq Response(X:Freq(MHz) Y:Power(dBm))"));
    plotRF->setTitle(title);
    plotRF->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);

    plotIF = new QTestPlot(this);
    plotIF->init(IF_TX_FILTER_160M_FREQ_STAR / 1e6,
                 IF_TX_FILTER_160M_FREQ_STOP / 1e6,
                 -15.0,5.0);
    title = plotIF->title();
    title.setText(tr("IF T/RX Freq Response(X:Freq(MHz) Y:Power(dBm))"));
    plotIF->setTitle(title);
    plotIF->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);

    dataRF_TX  = new QwtRF_TX_FreqResData;
    curveRF_TX = new QwtPlotCurve;
    curveRF_TX->attach(plotRF);
    curveRF_TX->setPen(QColor(Qt::red),2);

    dataIF_TX  = new QwtIF_TX_FreqResData;
    curveIF_TX = new QwtPlotCurve;
    curveIF_TX->attach(plotIF);
    curveIF_TX->setPen(QColor(Qt::red),2);

    dataRF_RX  = new QwtRF_RX_FreqResData;
    curveRF_RX = new QwtPlotCurve;
    curveRF_RX->attach(plotRF);
    curveRF_RX->setPen(QColor(Qt::green),2);

    dataIF_RX  = new QwtIF_RX_FreqResData;
    curveIF_RX = new QwtPlotCurve;
    curveIF_RX->attach(plotIF);
    curveIF_RX->setPen(QColor(Qt::green),2);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(plotRF);
    layout->addWidget(plotIF);
    layout->setStretch(0,1);
    layout->setStretch(1,1);
}

void QTestFreqResWidget::resetShowWidget(TestBaseParam *param)
{
    TestFreqResParam *p = static_cast<TestFreqResParam *>(param);

    if (p->isTestRF_TX) {
        param->SP1401->prepare_tr_rf_tx_freq_res_test();
        dataRF_TX->setReport(param->SP1401->tr_rf_tx_freq_res_test());
        curveRF_TX->setSamples(dataRF_TX);
    }
    if (p->isTestIF_TX) {
        param->SP1401->prepare_tr_if_tx_freq_res_test();
        dataIF_TX->setReport(param->SP1401->tr_if_tx_freq_res_test());
        curveIF_TX->setSamples(dataIF_TX);
    }
    if (p->isTestRF_RX) {
        param->SP1401->prepare_tr_rf_rx_freq_res_test();
        dataRF_RX->setReport(param->SP1401->tr_rf_rx_freq_res_test());
        curveRF_RX->setSamples(dataRF_RX);
    }
    if (p->isTestIF_RX) {
        param->SP1401->prepare_tr_if_rx_freq_res_test();
        dataIF_RX->setReport(param->SP1401->tr_if_rx_freq_res_test());
        curveIF_RX->setSamples(dataIF_RX);
    }

    if (p->isTestRF_TX || p->isTestRF_RX) {
        plotRF->replot();
    }
    if (p->isTestIF_TX || p->isTestIF_RX) {
        plotIF->replot();
    }
}
