#include "rddt_plot.hpp"

void Q_RDDT_Plot::init()
{
    canvas = new QwtPlotCanvas;
    canvas->setFocusIndicator(QwtPlotCanvas::CanvasFocusIndicator);
    canvas->setFocusPolicy(Qt::StrongFocus);
    setCanvas(canvas);

    grid = new QwtPlotGrid();
    grid->enableXMin(true);
    grid->enableYMin(true);
    grid->setMajorPen(Qt::gray,0,Qt::DotLine);
    grid->setMinorPen(Qt::darkGray,0,Qt::DotLine);
    grid->attach(this);

    picker = new QwtPlotPicker(canvas);
    picker->setAxis(QwtPlot::xBottom,QwtPlot::yLeft);
    picker->setRubberBand(QwtPicker::CrossRubberBand);
    picker->setTrackerMode(QwtPicker::AlwaysOn);

    zoomer = new QwtPlotZoomer(canvas);
    zoomer->setTrackerMode(QwtPicker::AlwaysOff);
    zoomer->setMousePattern(QwtEventPattern::MouseSelect2,Qt::RightButton,Qt::ControlModifier);
    zoomer->setMousePattern(QwtEventPattern::MouseSelect3,Qt::RightButton);
}


void Q_RDDT_CalPlot::init(double xMin, double xMax, double yMin, double yMax)
{
    setXAxisMinMax(xMin,xMax);
    setYAxisMinMax(yMin,yMax);
    init();
}

void Q_RDDT_CalPlot::init()
{
    Q_RDDT_Plot::init();

    canvas->setPalette(Qt::white);
    picker->setRubberBandPen(QColor(Qt::black));
    picker->setTrackerPen(QColor(Qt::black));
    zoomer->setRubberBandPen(QColor(Qt::black));

    QwtText textTitle = title();

    QFont textFont = font();
    textFont.setPointSize(12);
    textFont.setBold(false);

    textTitle.setFont(textFont);
    textTitle.setColor(QColor(Qt::blue));
    setTitle(textTitle); 
}

void Q_RDDT_CalPlot::setXAxisMinMax(double min, double max)
{
    if (min - max != 0.0) {
        setAxisScale(QwtPlot::xBottom,min,max);
    } else {
        setAxisAutoScale(QwtPlot::xBottom);
    }
}

void Q_RDDT_CalPlot::setYAxisMinMax(double min, double max)
{
    if (min - max != 0.0) {
        setAxisScale(QwtPlot::yLeft,min,max);
    } else {
        setAxisAutoScale(QwtPlot::yLeft);
    }
}


void Q_SA_Plot::init()
{
    Q_RDDT_Plot::init();

    canvas->setPalette(Qt::black);
    picker->setRubberBandPen(QColor(Qt::white));
    picker->setTrackerPen(QColor(Qt::green));
    zoomer->setRubberBandPen(QColor(Qt::white));
}
