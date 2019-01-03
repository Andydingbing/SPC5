#include "qcalplot.h"
#include "qwt_plot_canvas.h"
#include "qwt_plot_grid.h"
#include "qwt_plot_picker.h"
#include "qwt_plot_zoomer.h"

QCalPlot::QCalPlot(QWidget *parent) :
    QwtPlot(parent)
{
    init();
}

void QCalPlot::init(double xMin, double xMax, double yMin, double yMax)
{
    if (xMin != xMax)
        setAxisScale(QwtPlot::xBottom,xMin,xMax);
    else
        setAxisAutoScale(QwtPlot::xBottom);

    if (yMin != yMax)
        setAxisScale(QwtPlot::yLeft,yMin,yMax);
    else
        setAxisAutoScale(QwtPlot::yLeft);

    init();
}

void QCalPlot::init()
{
    QwtPlotCanvas *canvas = new QwtPlotCanvas;
    canvas->setFocusIndicator(QwtPlotCanvas::CanvasFocusIndicator);
    canvas->setFocusPolicy(Qt::StrongFocus);
    canvas->setPalette(QColor(Qt::white));
    this->setCanvas(canvas);

    QwtPlotGrid *grid = new QwtPlotGrid();
    grid->enableXMin(true);
    grid->enableYMin(true);
    grid->setMajorPen(Qt::gray,0,Qt::DotLine);
    grid->setMinorPen(Qt::darkGray,0,Qt::DotLine);
    grid->attach(this);

    QwtPlotPicker *picker = new QwtPlotPicker(QwtPlot::xBottom,QwtPlot::yLeft,QwtPlotPicker::CrossRubberBand,QwtPicker::AlwaysOn,canvas);
    picker->setRubberBandPen(QColor(Qt::black));
    picker->setRubberBand(QwtPicker::CrossRubberBand);
    picker->setTrackerPen(QColor(Qt::black));

    QwtPlotZoomer *zoomer = new QwtPlotZoomer(canvas);
    zoomer->setTrackerMode(QwtPicker::AlwaysOff);
    zoomer->setRubberBandPen(QColor(Qt::black));
    zoomer->setMousePattern( QwtEventPattern::MouseSelect2,Qt::RightButton,Qt::ControlModifier);
    zoomer->setMousePattern( QwtEventPattern::MouseSelect3,Qt::RightButton);

    QwtText textTitle = title();

    QFont textFont = font();
    textFont.setPointSize(10);
    textFont.setBold(false);

    textTitle.setFont(textFont);
    setTitle(textTitle);
}
