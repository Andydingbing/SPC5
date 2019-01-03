#ifndef QIQCAPDLG_H
#define QIQCAPDLG_H

#include <QWidget>
#include "qrfdlg.h"
#include "qwt_plot_curve.h"

namespace Ui {
class QIQCapDlg;
}

class QIQCapDlg : public QRfDlg
{
    Q_OBJECT

public:
    bool showingDFT;
    QwtPlotCurve *curveI;
    QwtPlotCurve *curveQ;
    QwtPlotCurve *curveDFT;
    dsp_buf *iqBuf;

public:
    explicit QIQCapDlg(QWidget *parent = 0);
    ~QIQCapDlg();

public:
    void changePlot2TD();
    void changePlot2DFT();
    void updatePlotFromBuf();

public slots:
    void updatePlot();

private slots:
    void on_pushButtonCap_clicked();

    void on_comboBoxTrigSrc_currentIndexChanged(int index);

    void on_comboBoxTrigEdge_currentIndexChanged(int index);

    void on_lineEditTrigOffset_textChanged(const QString &arg1);

    void on_lineEditTrigGap_textChanged(const QString &arg1);

    void on_lineEditCapOffset_textChanged(const QString &arg1);

    void on_lineEditTimeout_textChanged(const QString &arg1);

    void on_lineEditSamples_textChanged(const QString &arg1);

    void on_lineEditThreshold_textChanged(const QString &arg1);

    void on_pushButtonSetPath_clicked();

    void on_pushButtonShowTD_clicked();

    void on_pushButtonShowDFT_clicked();

    void on_checkBoxShowI_clicked(bool checked);

    void on_checkBoxShowQ_clicked(bool checked);

    void on_checkBoxMaxHold_clicked();

    void on_checkBoxMinHold_clicked();

    void on_pushButtonSetAll_clicked();

    void on_pushButtonFrameSlotTrigStart_clicked();

    void on_pushButtonFrameSlotTrigStop_clicked();

private:
    Ui::QIQCapDlg *ui;
};

#endif // QIQCAPDLG_H
