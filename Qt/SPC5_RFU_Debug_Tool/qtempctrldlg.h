#ifndef QTEMPCTRLDLG_H
#define QTEMPCTRLDLG_H

#include <QWidget>
#include "define.h"
#include "qwt_plot_curve.h"

namespace Ui {
class QTempCtrlDlg;
}

class QTempCtrlDlg : public QWidget
{
    Q_OBJECT
public:
    enum BlowerAjustMethod{
        Ajust_All = 0,
        Ajust_Single
    };
    sp3501 *_sp3501;
    sp1401_r1a *_sp1401_r1a;
    sp1401_r1c *_sp1401_r1c;
    QVector<QwtPlotCurve *> curveR1ATx;
    QVector<QwtPlotCurve *> curveR1ARx;
    QVector<QVector<QwtPlotCurve *> *> curveR1C;
public:
    explicit QTempCtrlDlg(QWidget *parent = 0);
    ~QTempCtrlDlg();
public:
    void initAllSlider(int max);
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

private:
    Ui::QTempCtrlDlg *ui;
};

extern QTempCtrlDlg *dlgTempCtrl;

#endif // QTEMPCTRLDLG_H
