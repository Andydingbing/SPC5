#ifndef QTEMPCTRLDLG_H
#define QTEMPCTRLDLG_H

#include <QWidget>
#include "define.h"
#include "qwt_plot_curve.h"
#include "q_winthread.h"
#include "q_rf_dlg.h"
#include "q_cal_base_dlg.h"
#include "q_r1c_temp_ctrl_model.h"
#include <qwt_plot_legenditem.h>
#include <qwt_legend.h>
namespace Ui {
class QTempCtrlDlg;
}

class QTempCtrlDlg : public QRFDlg
{
    Q_OBJECT
public slots:

    void update(double *gettemp);
public:
    enum BlowerAjustMethod{
        Ajust_All = 0,
        Ajust_Single
    };
    sp3501 *_sp3501;
    sp3301 *_sp3301;
    sp1401_r1a *_sp1401_r1a;
    sp1401_r1c *_sp1401_r1c;
    QVector<QwtPlotCurve *> curveR1ATx;
    QVector<QwtPlotCurve *> curveR1ARx;
    QVector<QVector<QwtPlotCurve *> *> curveR1C;
    TempParam Param;
    QTempCtrlDlg *pThread;
    uint64_t n;
    QPointF point[8];
    QwtTempCtrlData  *dataTempCtr[8];
    QwtLegend *d_externalLegend;
public:
    //    QwtPointSeriesData *series;
    //    QwtPointSeriesData *series1;
    //    QwtPointSeriesData *series2;
    //    QwtPointSeriesData *series3;
    //    QwtPointSeriesData *series4;
    //    QwtPointSeriesData *series5;
    //    QwtPointSeriesData *series6;
    //    QwtPointSeriesData *series7;
    //    QVector<QPointF> vector;
    //    QVector<QPointF> vector1;
    //    QVector<QPointF> vector2;
    //    QVector<QPointF> vector3;
    //    QVector<QPointF> vector4;
    //    QVector<QPointF> vector5;
    //    QVector<QPointF> vector6;
    //    QVector<QPointF> vector7;
    //    QVector<QR1CTempCtrlModel*> *_model;
public:
    explicit QTempCtrlDlg(QWidget *parent = 0);
    ~QTempCtrlDlg();
public:
    void initAllSlider(int max);

    TempParam ui2TempParam();

private slots:
    void on_verticalSlider1_valueChanged(int value);

    void on_verticalSlider2_valueChanged(int value);

    void on_verticalSlider3_valueChanged(int value);

    void on_verticalSlider4_valueChanged(int value);

    void on_verticalSlider5_valueChanged(int value);

    void on_verticalSlider6_valueChanged(int value);

    void on_verticalSlider7_valueChanged(int value);

    void on_verticalSlider8_valueChanged(int value);

    void on_verticalSlider9_valueChanged(int value);

    void on_verticalSlider10_valueChanged(int value);

    void on_verticalSlider11_valueChanged(int value);

    void on_pushButtonStart_clicked();

    void on_pushButton_clicked();

private:
    Ui::QTempCtrlDlg *ui;
};

extern QTempCtrlDlg *dlgTempCtrl;

#endif // QTEMPCTRLDLG_H
