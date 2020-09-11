#include "q_cal_temp_comp_widget.hpp"
#include "qwt_plot_curve.h"
#include "q_model_temp_pwr.hpp"
#include "q_plot.hpp"
#include <QLineEdit>
#include <algo_fit.hpp>

QCalTxTempCompWidget::QCalTxTempCompWidget(QWidget *parent) :
    QWidget(parent),
    _dataProperty(nullptr),
    _dataComp(nullptr),
    _curveProperty(nullptr),
    _curveComp(nullptr)
{
    _lineEditPolynomail = new QLineEdit(this);
    _plot = new QCalPlot(this);
    _plot->init<float,double>(sp1401::temperature_min(),
                              sp1401::temperature_max(),
                              -40.0,-10.0);

    QwtText title = _plot->title();
    title.setText(tr("RF Temp Property(X:Temp Y:Power(dBm))"));
    _plot->setTitle(title);
    _plot->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(_plot);
    layout->addWidget(_lineEditPolynomail);
}

QCalTxTempCompWidget::~QCalTxTempCompWidget()
{
    if (_curveProperty != nullptr) {
        _curveProperty->detach();
    }
    if (_curveComp != nullptr) {
        _curveComp->detach();
    }
}

void QCalTxTempCompWidget::resetShowWidget(CalParam *param)
{
    SAFE_NEW(_dataProperty,QwtTXTempPropertyData);
    SAFE_NEW(_dataComp,QwtTempCompData);
    SAFE_NEW(_curveProperty,QwtPlotCurve);
    SAFE_NEW(_curveComp,QwtPlotCurve);

    _curveProperty->setPen(Qt::red,2);
    _curveProperty->setSamples(_dataProperty);
    _curveProperty->attach(_plot);
    _curveComp->setPen(Qt::green,2);
    _curveComp->setSamples(_dataComp);
    _curveComp->attach(_plot);
    _plot->replot();
}

void QCalTxTempCompWidget::updateCoef()
{
    _lineEditPolynomail->setText(QString::fromStdString(polynomial<double>(_dataComp->coef()->coef,_dataComp->coef()->order)));
}
