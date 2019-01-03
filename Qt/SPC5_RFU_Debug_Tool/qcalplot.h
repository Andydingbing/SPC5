#ifndef QCALPLOT_H
#define QCALPLOT_H

#include "qwt_plot.h"

class QCalPlot : public QwtPlot
{
    Q_OBJECT

public:
    QCalPlot(QWidget *parent = 0);

public:
    void init();
    void init(double xMin,double xMax,double yMin,double yMax);
};

#endif // QCALPLOT_H
