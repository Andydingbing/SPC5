#include "q_plot.h"
#include "qwt_plot_canvas.h"
#include "qwt_plot_picker.h"
#include "qwt_plot_zoomer.h"
#include "qwt_plot_grid.h"

void QPlot::init()
{
    _canvas = new QwtPlotCanvas;
    _canvas->setFocusIndicator(QwtPlotCanvas::CanvasFocusIndicator);
    _canvas->setFocusPolicy(Qt::StrongFocus);
    setCanvas(_canvas);

    _grid = new QwtPlotGrid();
    _grid->enableXMin(true);
    _grid->enableYMin(true);
    _grid->setMajorPen(Qt::gray,0,Qt::DotLine);
    _grid->setMinorPen(Qt::darkGray,0,Qt::DotLine);
    _grid->attach(this);

    _picker = new QwtPlotPicker(QwtPlot::xBottom, QwtPlot::yLeft,
                                QwtPlotPicker::CrossRubberBand,
                                QwtPicker::AlwaysOn,_canvas);
    _picker->setRubberBand(QwtPicker::CrossRubberBand);

    _zoomer = new QwtPlotZoomer(_canvas);
    _zoomer->setTrackerMode(QwtPicker::AlwaysOff);
    _zoomer->setMousePattern(QwtEventPattern::MouseSelect2,Qt::RightButton,Qt::ControlModifier);
    _zoomer->setMousePattern(QwtEventPattern::MouseSelect3,Qt::RightButton);
}


void QCalPlot::init(double xMin, double xMax, double yMin, double yMax)
{
    setXAxisMinMax(xMin,xMax);
    setYAxisMinMax(yMin,yMax);
    init();
}

void QCalPlot::init()
{
    QPlot::init();

    canvas()->setPalette(Qt::white);
    picker()->setRubberBandPen(QColor(Qt::black));
    picker()->setTrackerPen(QColor(Qt::black));
    zoomer()->setRubberBandPen(QColor(Qt::black));

    QwtText textTitle = title();

    QFont textFont = font();
    textFont.setPointSize(10);
    textFont.setBold(false);

    textTitle.setFont(textFont);
    textTitle.setColor(QColor(Qt::blue));
    setTitle(textTitle); 
}

void QCalPlot::setXAxisMinMax(double min, double max)
{
    if (min != max) {
        setAxisScale(QwtPlot::xBottom,min,max);
    } else {
        setAxisAutoScale(QwtPlot::xBottom);
    }
}

void QCalPlot::setYAxisMinMax(double min, double max)
{
    if (min != max) {
        setAxisScale(QwtPlot::yLeft,min,max);
    } else {
        setAxisAutoScale(QwtPlot::yLeft);
    }
}


void QIQCapturePlot::init()
{
    QPlot::init();

    canvas()->setPalette(Qt::black);
    picker()->setRubberBandPen(QColor(Qt::white));
    picker()->setTrackerPen(QColor(Qt::green));
    zoomer()->setRubberBandPen(QColor(Qt::white));
}
