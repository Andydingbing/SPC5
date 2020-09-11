#ifndef Q_PLOT_H
#define Q_PLOT_H

#include "qwt_plot.h"

class QwtPlotCanvas;
class QwtPlotPicker;
class QwtPlotZoomer;
class QwtPlotGrid;

class QPlot : public QwtPlot
{
public:
    explicit QPlot(QWidget *parent = nullptr) :
        QwtPlot(parent) {}

    QwtPlotCanvas *canvas() { return _canvas; }
    QwtPlotPicker *picker() { return _picker; }
    QwtPlotZoomer *zoomer() { return _zoomer; }
    QwtPlotGrid *grid() { return _grid; }
    virtual void init();

private:
    QwtPlotCanvas *_canvas;
    QwtPlotPicker *_picker;
    QwtPlotZoomer *_zoomer;
    QwtPlotGrid *_grid;
};


typedef class QCalPlot : public QPlot
{
public:
    explicit QCalPlot(QWidget *parent = nullptr) :
        QPlot(parent) {}

    void init();

    template <typename T_x,typename T_y>
    void init(const T_x xMin,const T_x xMax,const T_y yMin,const T_y yMax)
    { return init(double(xMin),double(xMax),double(yMin),double(yMax)); }

    void init(double xMin, double xMax, double yMin, double yMax);

    void setXAxisMinMax(double min, double max);
    void setYAxisMinMax(double min, double max);
} QCalPlot, QTestPlot;


class QIQCapturePlot : public QPlot
{
public:
    explicit QIQCapturePlot(QWidget *parent = nullptr) :
        QPlot(parent) {}

    void init();
};

#endif // Q_PLOT_H
