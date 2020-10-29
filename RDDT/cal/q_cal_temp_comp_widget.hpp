#ifndef Q_CAL_TEMP_COMP_WIDGET_HPP
#define Q_CAL_TEMP_COMP_WIDGET_HPP

#include <QWidget>

struct CalParam;
class QwtTXTempPropertyData;
class QwtTempCompData;
class QwtPlotCurve;
class QCalPlot;
class QLineEdit;

typedef class QCalTxTempCompWidget : public QWidget
{
public:
    explicit QCalTxTempCompWidget(QWidget *parent = nullptr);
    ~QCalTxTempCompWidget();
    void resetShowWidget(CalParam *param);
    void updateCoef();
    QwtTXTempPropertyData *dataProperty() { return _dataProperty; }
    QwtTempCompData *dataComp() { return _dataComp; }
    QCalPlot *plot() { return _plot; }

private:
    QwtTXTempPropertyData *_dataProperty;
    QwtTempCompData *_dataComp;
    QwtPlotCurve *_curveProperty;
    QwtPlotCurve *_curveComp;
    QCalPlot *_plot;
    QLineEdit *_lineEditPolynomail;
} QCalRXTempCompWidget;

#endif // Q_CAL_TEMP_COMP_WIDGET_HPP
