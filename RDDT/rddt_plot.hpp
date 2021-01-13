#ifndef Q_RDDT_PLOT_HPP
#define Q_RDDT_PLOT_HPP

#include "rd.h"
#include "qwt_plot.h"
#include "qwt_plot_grid.h"
#include "qwt_plot_curve.h"
#include "qwt_plot_canvas.h"
#include "qwt_plot_picker.h"
#include "qwt_plot_zoomer.h"

class Q_RDDT_Plot : public QwtPlot
{
public:
    Q_RDDT_Plot(QWidget *parent = nullptr) : QwtPlot(parent) {}

    virtual void init();

public:
    QwtPlotCanvas *canvas;
    QwtPlotPicker *picker;
    QwtPlotZoomer *zoomer;
    QwtPlotGrid   *grid;
};


class QwtPlotHelper {
public:
    static void init(QwtPlot *plot)
    {
        QwtPlotCanvas *canvas = new QwtPlotCanvas;
        canvas->setFocusIndicator(QwtPlotCanvas::CanvasFocusIndicator);
        canvas->setFocusPolicy(Qt::StrongFocus);
        canvas->setPalette(Qt::white);
        plot->setCanvas(canvas);

        QwtPlotGrid *grid = new QwtPlotGrid();
        grid->enableXMin(true);
        grid->enableYMin(true);
        grid->setMajorPen(Qt::gray,0,Qt::DotLine);
        grid->setMinorPen(Qt::darkGray,0,Qt::DotLine);
        grid->attach(plot);

        QwtPlotPicker *picker = new QwtPlotPicker(canvas);
        picker->setAxis(QwtPlot::xBottom,QwtPlot::yLeft);
        picker->setRubberBand(QwtPicker::CrossRubberBand);
        picker->setTrackerMode(QwtPicker::AlwaysOn);
        picker->setTrackerPen(QColor(Qt::black));
        picker->setRubberBandPen(QColor(Qt::black));

        QwtPlotZoomer *zoomer = new QwtPlotZoomer(canvas);
        zoomer->setTrackerMode(QwtPicker::AlwaysOff);
        zoomer->setRubberBandPen(QColor(Qt::black));
        zoomer->setMousePattern(QwtEventPattern::MouseSelect2,Qt::RightButton,Qt::ControlModifier);
        zoomer->setMousePattern(QwtEventPattern::MouseSelect3,Qt::RightButton);
    }

    static void setXAxisMinMax(QwtPlot *plot,double min,double max)
    {
        if (min - max != 0.0) {
            plot->setAxisScale(QwtPlot::xBottom,min,max);
        } else {
            plot->setAxisAutoScale(QwtPlot::xBottom);
        }
    }

    static void setYAxisMinMax(QwtPlot *plot,double min,double max)
    {
        if (min - max != 0.0) {
            plot->setAxisScale(QwtPlot::yLeft,min,max);
        } else {
            plot->setAxisAutoScale(QwtPlot::yLeft);
        }
    }

    static void init(QwtPlot *plot,double xMin,double xMax,double yMin,double yMax)
    {
        setXAxisMinMax(plot,xMin,xMax);
        setYAxisMinMax(plot,yMin,yMax);
        init(plot);
    }

    template<typename T_x,typename T_y>
    static void init(QwtPlot *plot,const T_x xMin,const T_x xMax,const T_y yMin,const T_y yMax)
    { return init(plot,double(xMin),double(xMax),double(yMin),double(yMax)); }
};

RD_INLINE static void init(QwtPlot *plot)
{ return QwtPlotHelper::init(plot); }

RD_INLINE static void setXAxisMinMax(QwtPlot *plot,double min,double max)
{ return QwtPlotHelper::setXAxisMinMax(plot,min,max); }

RD_INLINE static void setYAxisMinMax(QwtPlot *plot,double min,double max)
{ return QwtPlotHelper::setYAxisMinMax(plot,min,max); }

RD_INLINE static void init(QwtPlot *plot,double xMin,double xMax,double yMin,double yMax)
{ return QwtPlotHelper::init(plot,xMin,xMax,yMin,yMax); }

template<typename T_x,typename T_y>
RD_INLINE static void init(QwtPlot *plot,const T_x xMin,const T_x xMax,const T_y yMin,const T_y yMax)
{ return QwtPlotHelper::init<T_x,T_y>(plot,xMin,xMax,yMin,yMax); }


typedef class Q_RDDT_CalPlot : public Q_RDDT_Plot
{
public:
    Q_RDDT_CalPlot(QWidget *parent = nullptr) : Q_RDDT_Plot(parent) {}

    void init();

    template<typename T_x,typename T_y>
    void init(const T_x xMin,const T_x xMax,const T_y yMin,const T_y yMax)
    { return init(double(xMin),double(xMax),double(yMin),double(yMax)); }

    void init(double xMin,double xMax,double yMin,double yMax);

    void setXAxisMinMax(double min, double max);
    void setYAxisMinMax(double min, double max);
} Q_RDDT_CalPlot, Q_RDDT_TestPlot;


class Q_SA_Plot : public Q_RDDT_Plot
{
public:
    Q_SA_Plot(QWidget *parent = nullptr) : Q_RDDT_Plot(parent) {}

    void init();
};

#endif // Q_RDDT_PLOT_HPP
