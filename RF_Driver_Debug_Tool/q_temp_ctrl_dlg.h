#ifndef QTEMPCTRLDLG_H
#define QTEMPCTRLDLG_H

#include "q_cal_base_dlg.h"
#include "q_r1c_temp_ctrl_thread.h"

namespace Ui {
class QTempCtrlDlg;
}

class QwtPlotCurve;
class QwtLegend;
class QwtTempData;

class QTempCtrlDlg : public Q_RD_Widget
{
    Q_OBJECT

public slots:
    void update(double *gettemp);

public:
    QVector<QwtPlotCurve *> curveR1ATx;
    QVector<QwtPlotCurve *> curveR1ARx;
    QVector<QVector<QwtPlotCurve *> *> curveR1C;
    QVector<QVector<QwtTempData *> *> dataR1C;

    QR1CTempCtrlThread::Param param;
    QwtLegend *d_externalLegend;

public:
    explicit QTempCtrlDlg(QWidget *parent = nullptr);
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

    void on_verticalSlider12_valueChanged(int value);

    void on_pushButtonStart_clicked();

    void on_pushButton_clicked();

private:
    Ui::QTempCtrlDlg *ui;
};

extern QTempCtrlDlg *dlgTempCtrl;

#endif // QTEMPCTRLDLG_H
