#include "qiqcapdlg.h"
#include "qframeslottrigwidget.h"
#include "ui_qframeslottrigwidget.h"
#include "ui_qiqcapdlg.h"
#include "qwt_plot_curve.h"
#include "qwt_plot_canvas.h"
#include "qwt_plot_grid.h"
#include "qwt_plot_picker.h"
#include "qwt_plot_zoomer.h"
#include "qwt_scale_div.h"
#include <QFileDialog>

QIQCapDlg::QIQCapDlg(QWidget *parent) :
    QRfDlg(parent),
    ui(new Ui::QIQCapDlg)
{
    ui->setupUi(this);

    curveI = new QwtPlotCurve("I");
    curveQ = new QwtPlotCurve("Q");
    curveDFT = new QwtPlotCurve("DFT");
    iqBuf = new dsp_buf;

    changePlot2DFT();
    showingDFT = true;

    QwtPlotCanvas *canvas = new QwtPlotCanvas;
    canvas->setFocusIndicator(QwtPlotCanvas::CanvasFocusIndicator);
    canvas->setFocusPolicy(Qt::StrongFocus);
    canvas->setPalette(CLR_SA_BLGD);
    ui->plot->setCanvas(canvas);

    curveI->setPen(QColor(Qt::red));
    curveI->setVisible(false);
    curveI->attach(ui->plot);
    curveQ->setPen(QColor(Qt::green));
    curveQ->setVisible(false);
    curveQ->attach(ui->plot);
    curveDFT->setPen(QColor(Qt::yellow));
    curveDFT->setVisible(true);
    curveDFT->attach(ui->plot);

    QwtPlotGrid *grid = new QwtPlotGrid();
    grid->enableXMin(true);
    grid->enableYMin(true);
    grid->setMajorPen(Qt::gray,0,Qt::DotLine);
    grid->setMinorPen(Qt::darkGray,0,Qt::DotLine);
    grid->attach(ui->plot);

    QwtPlotPicker *picker = new QwtPlotPicker(QwtPlot::xBottom,QwtPlot::yLeft,QwtPlotPicker::CrossRubberBand,QwtPicker::AlwaysOn,canvas);
    picker->setRubberBandPen(QColor(Qt::white));
    picker->setRubberBand(QwtPicker::CrossRubberBand);
    picker->setTrackerPen(QColor(Qt::green));

    QwtPlotZoomer *zoomer = new QwtPlotZoomer(canvas);
    zoomer->setTrackerMode(QwtPicker::AlwaysOff);
    zoomer->setRubberBandPen(QColor(Qt::white));
    zoomer->setMousePattern( QwtEventPattern::MouseSelect2,Qt::RightButton,Qt::ControlModifier);
    zoomer->setMousePattern( QwtEventPattern::MouseSelect3,Qt::RightButton);

    ui->lineEditTrigOffset->setText("0");
    ui->lineEditTrigGap->setText("0");
    ui->lineEditCapOffset->setText("0");
    ui->lineEditTimeout->setText("6553600");
    ui->lineEditSamples->setText("40960");
    ui->lineEditThreshold->setText("0");
    ui->lineEditPath->setText("c:\\iq.txt");
}

QIQCapDlg::~QIQCapDlg()
{
    delete ui;
}

void QIQCapDlg::changePlot2TD()
{
    curveI->setSamples(iqBuf->get_i(),iqBuf->td_get_samples());
    curveQ->setSamples(iqBuf->get_q(),iqBuf->td_get_samples());
    ui->plot->setTitle("I/Q Time Domain");
    ui->plot->setAxisScale(QwtPlot::xBottom,0.0,double(iqBuf->td_get_samples()));
    ui->plot->setAxisScale(QwtPlot::yLeft,-15000.0,15000.0);
}

void QIQCapDlg::changePlot2DFT()
{
    curveDFT->setSamples(iqBuf->get_normalize_freq(),iqBuf->get_pwr(),iqBuf->dft_get_pts());
    ui->plot->setTitle("DFT");
    ui->plot->setAxisScale(QwtPlot::xBottom,iqBuf->get_sr() / 1e6 / -2.0,iqBuf->get_sr() / 1e6 / 2.0);
    ui->plot->setAxisScale(QwtPlot::yLeft,-180.0,10.0);
}

void QIQCapDlg::updatePlotFromBuf()
{
    if (showingDFT) {
        iqBuf->dft();
        curveDFT->setSamples(iqBuf->get_normalize_freq(),iqBuf->get_pwr(),iqBuf->dft_get_pts());
    }
    else {
        curveI->setSamples(iqBuf->get_i(),iqBuf->td_get_samples());
        curveQ->setSamples(iqBuf->get_q(),iqBuf->td_get_samples());
    }
    ui->plot->replot();
}

void QIQCapDlg::updatePlot()
{
    if (!(this->isVisible()))
        return;
    updatePlotFromBuf();
}

void QIQCapDlg::on_pushButtonCap_clicked()
{
    PTR_CHECKV(_sp3301);
    uint32_t samples = ui->lineEditSamples->text().toLong();
    INT_CHECKV(_sp3301->iq_cap_start(rfIdx));
    INT_CHECKV(_sp3301->iq_cap_iq2buf(rfIdx, samples));
    updatePlotFromBuf();
}

void QIQCapDlg::on_comboBoxTrigSrc_currentIndexChanged(int index)
{
    PTR_CHECKV(_sp3301);
    basic_sp1401::iq_cap_src_t src = (basic_sp1401::iq_cap_src_t)(ui->comboBoxTrigSrc->currentIndex());
    bool edge = (0 == ui->comboBoxTrigEdge->currentText() ? false : true);
    INT_CHECKV(getSP1401()->set_iq_cap_src(src,edge));
}

void QIQCapDlg::on_comboBoxTrigEdge_currentIndexChanged(int index)
{
    on_comboBoxTrigSrc_currentIndexChanged(index);
}

void QIQCapDlg::on_lineEditTrigOffset_textChanged(const QString &arg1)
{

}

void QIQCapDlg::on_lineEditTrigGap_textChanged(const QString &arg1)
{

}

void QIQCapDlg::on_lineEditCapOffset_textChanged(const QString &arg1)
{

}

void QIQCapDlg::on_lineEditTimeout_textChanged(const QString &arg1)
{
    PTR_CHECKV(_sp3301);
    INT_CHECKV(getSP1401()->set_iq_cap_timeout(arg1.toULong()));
}

void QIQCapDlg::on_lineEditSamples_textChanged(const QString &arg1)
{
    PTR_CHECKV(_sp3301);
    uint32_t samples = arg1.toULong();
    INT_CHECKV(_sp3301->set_iq_cap_samples(rfIdx, samples));
    iqBuf->_new(samples);
    if (showingDFT)
        changePlot2DFT();
    else
        changePlot2TD();
}

void QIQCapDlg::on_lineEditThreshold_textChanged(const QString &arg1)
{
    PTR_CHECKV(_sp3301);
    INT_CHECKV(getSP1401()->set_iq_cap_trig_threshold(arg1.toDouble()));
}

void QIQCapDlg::on_pushButtonSetPath_clicked()
{
    QString path = QFileDialog::getOpenFileName(this,tr("Select Target File"),"","");
    if (!path.isEmpty())
        ui->lineEditPath->setText(path);
}

void QIQCapDlg::on_pushButtonShowTD_clicked()
{
    if (showingDFT) {
        curveDFT->setVisible(false);
        ui->checkBoxMaxHold->setEnabled(false);
        ui->checkBoxMinHold->setEnabled(false);

        ui->checkBoxShowI->setEnabled(true);
        ui->checkBoxShowQ->setEnabled(true);
        curveI->setVisible(ui->checkBoxShowI->isChecked());
        curveQ->setVisible(ui->checkBoxShowQ->isChecked());
    }
    changePlot2TD();
    showingDFT = false;
    updatePlotFromBuf();
}

void QIQCapDlg::on_pushButtonShowDFT_clicked()
{
    if (!showingDFT) {
        curveI->setVisible(false);
        curveQ->setVisible(false);
        ui->checkBoxShowI->setEnabled(false);
        ui->checkBoxShowQ->setEnabled(false);

        ui->checkBoxMaxHold->setEnabled(true);
        ui->checkBoxMinHold->setEnabled(true);
        curveDFT->setVisible(true);
    }
    changePlot2DFT();
    showingDFT = true;
    updatePlotFromBuf();
}

void QIQCapDlg::on_checkBoxShowI_clicked(bool checked)
{
    curveI->setVisible(checked);
    ui->plot->replot();
}

void QIQCapDlg::on_checkBoxShowQ_clicked(bool checked)
{
    curveQ->setVisible(checked);
    ui->plot->replot();
}

void QIQCapDlg::on_checkBoxMaxHold_clicked()
{
    if (ui->checkBoxMaxHold->isChecked()) {
        ui->checkBoxMinHold->setChecked(false);
        iqBuf->dft_set_hold(dsp_buf::PH_MAX_HOLD);
    }
    else
        iqBuf->dft_set_hold(dsp_buf::PH_NO_HOLD);
}

void QIQCapDlg::on_checkBoxMinHold_clicked()
{
    if (ui->checkBoxMinHold->isChecked()) {
        ui->checkBoxMaxHold->setChecked(false);
        iqBuf->dft_set_hold(dsp_buf::PH_MIN_HOLD);
    }
    else
        iqBuf->dft_set_hold(dsp_buf::PH_NO_HOLD);
}

void QIQCapDlg::on_pushButtonSetAll_clicked()
{
    PTR_CHECKV(_sp3301);

    basic_sp1401::iq_cap_src_t trigSrc = (basic_sp1401::iq_cap_src_t)(ui->comboBoxTrigSrc->currentIndex());
    bool edge = (0 == ui->comboBoxTrigEdge->currentText() ? false : true);
    INT_CHECKV(getSP1401()->set_iq_cap_src(trigSrc,edge));

    quint32 timeout = ui->lineEditTimeout->text().toUInt();
    INT_CHECKV(getSP1401()->set_iq_cap_timeout(timeout));

    uint32_t samples = ui->lineEditSamples->text().toLong();
    iqBuf->_new(samples);
    INT_CHECKV(_sp3301->set_iq_cap_samples(rfIdx, samples));
    INT_CHECKV(_sp3301->set_iq_cap_buffers(rfIdx, iqBuf->get_i(), iqBuf->get_q()));

    double threshold = ui->lineEditThreshold->text().toDouble();
    INT_CHECKV(getSP1401()->set_iq_cap_trig_threshold(threshold));

    ui->widgetFrameSlotTrig->setAll(getSP2401R1A());
}

void QIQCapDlg::on_pushButtonFrameSlotTrigStart_clicked()
{
    PTR_CHECKV(_sp3301);
    INT_CHECKV(getSP2401R1A()->set_frame_slot_trigger_link(sp2401_r1a::UL));
    INT_CHECKV(getSP2401R1A()->set_frame_slot_trigger_start());
}

void QIQCapDlg::on_pushButtonFrameSlotTrigStop_clicked()
{
    PTR_CHECKV(_sp3301);
    INT_CHECKV(getSP2401R1A()->set_frame_slot_trigger_stop());
}
