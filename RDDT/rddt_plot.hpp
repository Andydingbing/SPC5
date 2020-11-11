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
