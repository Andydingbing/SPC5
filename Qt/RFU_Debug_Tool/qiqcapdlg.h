#ifndef QIQCAPDLG_H
#define QIQCAPDLG_H

#include "define.h"
#include "qwt_plot_curve.h"

namespace Ui {
class QIQCapDlg;
}

class QIQCapDlg : public QDialog
{
    Q_OBJECT

public:
    ISP1401 *m_pSP1401;
    CSP2401R1A *m_pSP2401;
    bool m_bShowingDFT;
    QwtPlotCurve *m_pCurveI;
    QwtPlotCurve *m_pCurveQ;
    QwtPlotCurve *m_pCurveDFT;
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
    void on_m_PBCap_clicked();

    void on_m_CBTrigSrc_currentIndexChanged(int index);

    void on_m_CBTrigEdge_currentIndexChanged(int index);

    void on_m_LETrigOffset_textChanged(const QString &arg1);

    void on_m_LETrigGap_textChanged(const QString &arg1);

    void on_m_LECapOffset_textChanged(const QString &arg1);

    void on_m_LETimeout_textChanged(const QString &arg1);

    void on_m_LESamples_textChanged(const QString &arg1);

    void on_m_LEThreshold_textChanged(const QString &arg1);

    void on_m_PBSetPath_clicked();

private:
    Ui::QIQCapDlg *ui;
};

#endif // QIQCAPDLG_H
