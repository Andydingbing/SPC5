#include "qiqcapdlg.h"
#include "ui_qiqcapdlg.h"
#include <QtCharts>
#include <QChartView>
#include <QLineSeries>
#include "qwt_plot_curve.h"
#include "qwt_plot_canvas.h"
#include "qwt_plot_grid.h"
#include "qwt_plot_picker.h"
#include "qwt_plot_zoomer.h"
#include "qwt_scale_div.h"

using namespace QtCharts;

QIQCapDlg::QIQCapDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QIQCapDlg)
{
    ui->setupUi(this);
    m_pSP1401 = NULL;
    m_pSP2401 = NULL;
    m_pCurveI = new QwtPlotCurve("I");
    m_pCurveQ = new QwtPlotCurve("Q");
    m_pCurveDFT = new QwtPlotCurve("DFT");

    changePlot2DFT();
    m_bShowingDFT = true;

    QwtPlotCanvas *pCanvas = new QwtPlotCanvas;
    pCanvas->setFocusIndicator(QwtPlotCanvas::CanvasFocusIndicator);
    pCanvas->setFocusPolicy(Qt::StrongFocus);
    pCanvas->setPalette(CLR_SA_BLGD);
    ui->m_Plot->setCanvas(pCanvas);

    m_pCurveI->setPen(QColor(Qt::red));
    m_pCurveI->setVisible(false);
    m_pCurveI->attach(ui->m_Plot);
    m_pCurveQ->setPen(QColor(Qt::green));
    m_pCurveQ->setVisible(false);
    m_pCurveQ->attach(ui->m_Plot);
    m_pCurveDFT->setPen(QColor(Qt::yellow));
    m_pCurveDFT->setVisible(true);
    m_pCurveDFT->attach(ui->m_Plot);

    QwtPlotGrid *pGrid = new QwtPlotGrid();
    pGrid->enableXMin(true);
    pGrid->enableYMin(true);
    pGrid->setMajorPen(Qt::gray,0,Qt::DotLine);
    pGrid->setMinorPen(Qt::darkGray,0,Qt::DotLine);
    pGrid->attach(ui->m_Plot);

    QwtPlotPicker *pPicker = new QwtPlotPicker(QwtPlot::xBottom,QwtPlot::yLeft,QwtPlotPicker::CrossRubberBand,QwtPicker::AlwaysOn,pCanvas);
    pPicker->setRubberBandPen(QColor(Qt::white));
    pPicker->setRubberBand(QwtPicker::CrossRubberBand);
    pPicker->setTrackerPen(QColor(Qt::green));

    QwtPlotZoomer *pZoomer = new QwtPlotZoomer(pCanvas);
    pZoomer->setTrackerMode(QwtPicker::AlwaysOff);
    pZoomer->setRubberBandPen(QColor(Qt::white));
    pZoomer->setMousePattern( QwtEventPattern::MouseSelect2,Qt::RightButton,Qt::ControlModifier);
    pZoomer->setMousePattern( QwtEventPattern::MouseSelect3,Qt::RightButton);

    ui->m_LETrigOffset->setText("0");
    ui->m_LETrigGap->setText("0");
    ui->m_LECapOffset->setText("0");
    ui->m_LETimeout->setText("6553600");
    ui->m_LESamples->setText("40960");
    ui->m_LEThreshold->setText("0");
    ui->m_CBShowI->setChecked(true);
    ui->m_CBShowQ->setChecked(true);
    ui->m_CBShowI->setEnabled(false);
    ui->m_CBShowQ->setEnabled(false);
    ui->m_CBMaxHold->setChecked(false);
    ui->m_CBMinHold->setChecked(false);
    ui->m_CBMaxHold->setEnabled(true);
    ui->m_CBMinHold->setEnabled(true);
    ui->m_LEPath->setText("c:\\iq.txt");
}

QIQCapDlg::~QIQCapDlg()
{
    delete ui;
}

void QIQCapDlg::changePlot2TD()
{
    m_pCurveI->setSamples(IQBUF->GetI(),IQBUF->TDGetSamples());
    m_pCurveQ->setSamples(IQBUF->GetQ(),IQBUF->TDGetSamples());
    ui->m_Plot->setTitle("I Q");
    ui->m_Plot->setAxisScale(QwtPlot::xBottom,0.0,double(IQBUF->TDGetSamples()));
    ui->m_Plot->setAxisScale(QwtPlot::yLeft,-15000.0,15000.0);
}

void QIQCapDlg::changePlot2DFT()
{
    m_pCurveDFT->setSamples(IQBUF->GetNormFreq(),IQBUF->GetPower(),IQBUF->DFTGetPts());
    ui->m_Plot->setTitle("DFT");
    ui->m_Plot->setAxisScale(QwtPlot::xBottom,IQBUF->GetSr() / 1e6 / -2.0,IQBUF->GetSr() / 1e6 / 2.0);
    ui->m_Plot->setAxisScale(QwtPlot::yLeft,-180.0,10.0);
}

void QIQCapDlg::updatePlotFromBuf()
{
    if (m_bShowingDFT) {
        IQBUF->DFT();
        m_pCurveDFT->setSamples(IQBUF->GetNormFreq(),IQBUF->GetPower(),IQBUF->DFTGetPts());
    }
    else {
        m_pCurveI->setSamples(IQBUF->GetI(),IQBUF->TDGetSamples());
        m_pCurveQ->setSamples(IQBUF->GetQ(),IQBUF->TDGetSamples());
    }
    ui->m_Plot->replot();
}

void QIQCapDlg::updatePlot()
{
    if (!(this->isVisible()))
        return;
    updatePlotFromBuf();
}

void QIQCapDlg::on_m_PBCap_clicked()
{
    PTR_CHECKV(m_pSP1401);
    uint32_t uiSamples = ui->m_LESamples->text().toLong();
    IQBUF->New(uiSamples);
    INT_CHECKV(m_pSP1401->SetIQCapSamples(uiSamples));
    INT_CHECKV(m_pSP1401->IQCap());
    INT_CHECKV(DDR->IQToBuf(m_pSP1401->GetRfIdx(),IQBUF->GetI(),IQBUF->GetQ(),uiSamples));
    if (m_bShowingDFT) {
        IQBUF->DFT();
        m_pCurveDFT->setSamples(IQBUF->GetNormFreq(),IQBUF->GetPower(),uiSamples);
    }
    else {
        m_pCurveI->setSamples(IQBUF->GetI(),uiSamples);
        m_pCurveQ->setSamples(IQBUF->GetQ(),uiSamples);
    }
    ui->m_Plot->replot();
}

void QIQCapDlg::on_m_CBTrigSrc_currentIndexChanged(int index)
{
    PTR_CHECKV(m_pSP1401);
    ISP1401::IQCapSrcs Src = (ISP1401::IQCapSrcs)(ui->m_CBTrigSrc->currentIndex());
    bool bEdge = (0 == ui->m_CBTrigEdge->currentText() ? false : true);
    INT_CHECKV(m_pSP1401->SetIQCapSrc(Src,bEdge));
}

void QIQCapDlg::on_m_CBTrigEdge_currentIndexChanged(int index)
{
    on_m_CBTrigSrc_currentIndexChanged(index);
}

void QIQCapDlg::on_m_LETrigOffset_textChanged(const QString &arg1)
{

}

void QIQCapDlg::on_m_LETrigGap_textChanged(const QString &arg1)
{

}

void QIQCapDlg::on_m_LECapOffset_textChanged(const QString &arg1)
{

}

void QIQCapDlg::on_m_LETimeout_textChanged(const QString &arg1)
{
    PTR_CHECKV(m_pSP1401);
    INT_CHECKV(m_pSP1401->SetIQCapTimeout(arg1.toULong()));
}

void QIQCapDlg::on_m_LESamples_textChanged(const QString &arg1)
{
    PTR_CHECKV(m_pSP1401);
    uint32_t uiSamples = arg1.toULong();
    INT_CHECKV(m_pSP1401->SetIQCapSamples(uiSamples));
    IQBUF->New(uiSamples);
    if (m_bShowingDFT)
        changePlot2DFT();
    else
        changePlot2TD();
}

void QIQCapDlg::on_m_LEThreshold_textChanged(const QString &arg1)
{
    PTR_CHECKV(m_pSP1401);
    INT_CHECKV(m_pSP1401->SetIQCapTrigThreshold(arg1.toDouble()));
}

void QIQCapDlg::on_m_PBSetPath_clicked()
{
    QString strPath = QFileDialog::getOpenFileName(this,tr("Select Target File"),"","");
    if (!strPath.isEmpty())
        ui->m_LEPath->setText(strPath);
}
