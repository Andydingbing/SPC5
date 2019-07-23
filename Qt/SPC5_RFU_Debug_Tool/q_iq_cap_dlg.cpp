#include "q_iq_cap_dlg.h"
#include "q_frame_slot_trig_widget.h"
#include "ui_q_frame_slot_trig_widget.h"
#include "ui_q_iq_cap_dlg.h"
#include "qwt_plot_curve.h"
#include "qwt_plot_canvas.h"
#include "qwt_plot_grid.h"
#include "qwt_plot_picker.h"
#include "qwt_plot_zoomer.h"
#include "qwt_scale_div.h"
#include <QFileDialog>

QIQCapDlg::QIQCapDlg(QWidget *parent) :
    QRFDlg(parent),
    ui(new Ui::QIQCapDlg)
{
    ui->setupUi(this);
    ui->plot->init();

    iqBuf = new dsp_buf;

    curveI = new QwtPlotCurve("I");
    curveI->setPen(QColor(Qt::red));
    curveI->setVisible(false);
    curveI->attach(ui->plot);

    curveQ = new QwtPlotCurve("Q");
    curveQ->setPen(QColor(Qt::green));
    curveQ->setVisible(false);
    curveQ->attach(ui->plot);

    curveDFT = new QwtPlotCurve("DFT");
    curveDFT->setPen(QColor(Qt::yellow));
    curveDFT->setVisible(true);
    curveDFT->attach(ui->plot);

    changePlot2DFT();
    showingDFT = true;

    ui->comboBoxTrigSrc->setCurrentIndex(basic_sp1401::IQ_CAP_SRC_INT_TRIG);
    ui->lineEditTrigOffset->setText("0");
    ui->lineEditTrigGap->setText("0");
    ui->lineEditCapOffset->setText("0");
    ui->lineEditTimeout->setText("6553600");
    ui->lineEditSamples->setText("245760");
    ui->lineEditThreshold->setText("0");
    ui->lineEditPath->setText("C:\\iq.txt");
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
    uint32_t samples = ui->lineEditSamples->text().toLong();
    INT_CHECKV(SP3301->iq_cap_iq2buf(rfIdx, samples));
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
    PTR_CHECK
    INT_CHECKV(SP3301->iq_cap(rfIdx));
    updatePlotFromBuf();
}

void QIQCapDlg::on_comboBoxTrigSrc_currentIndexChanged(int index)
{
    PTR_CHECK
    basic_sp1401::iq_cap_src_t src = (basic_sp1401::iq_cap_src_t)(ui->comboBoxTrigSrc->currentIndex());
    bool edge = (0 == ui->comboBoxTrigEdge->currentText() ? false : true);
    INT_CHECKV(SP1401()->set_iq_cap_src(src,edge));
}

void QIQCapDlg::on_comboBoxTrigEdge_currentIndexChanged(int index)
{
    on_comboBoxTrigSrc_currentIndexChanged(index);
}

void QIQCapDlg::on_lineEditTrigOffset_textChanged(const QString &arg1)
{
    PTR_CHECK
    uint32_t offset = ui->lineEditTrigOffset->text().toUInt();
    SP3301->set_iq_cap_frame_trig_offset(rfIdx,offset);
}

void QIQCapDlg::on_lineEditTrigGap_textChanged(const QString &arg1)
{

}

void QIQCapDlg::on_lineEditCapOffset_textChanged(const QString &arg1)
{

}

void QIQCapDlg::on_lineEditTimeout_textChanged(const QString &arg1)
{
    PTR_CHECK
    INT_CHECKV(SP1401()->set_iq_cap_timeout(arg1.toULong()));
}

void QIQCapDlg::on_lineEditSamples_textChanged(const QString &arg1)
{
    PTR_CHECK
    on_pushButtonSetAll_clicked();
    uint32_t samples = arg1.toULong();
    INT_CHECKV(SP3301->set_iq_cap_samples(rfIdx, samples));
    iqBuf->_new(samples);
    if (showingDFT)
        changePlot2DFT();
    else
        changePlot2TD();
}

void QIQCapDlg::on_lineEditThreshold_textChanged(const QString &arg1)
{
    PTR_CHECK
    INT_CHECKV(SP1401()->set_iq_cap_trig_threshold(arg1.toDouble()));
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
    PTR_CHECK

    basic_sp1401::iq_cap_src_t trigSrc = basic_sp1401::iq_cap_src_t(ui->comboBoxTrigSrc->currentIndex());
    bool edge = (0 == ui->comboBoxTrigEdge->currentText() ? false : true);
    INT_CHECKV(SP1401()->set_iq_cap_src(trigSrc,edge));

    quint32 timeout = ui->lineEditTimeout->text().toUInt();
    INT_CHECKV(SP1401()->set_iq_cap_timeout(timeout));

    uint32_t samples = ui->lineEditSamples->text().toLong();
    iqBuf->_new(samples);
    INT_CHECKV(SP3301->set_iq_cap_buffer(rfIdx,iqBuf->get_i(),iqBuf->get_q()));
    INT_CHECKV(SP3301->set_iq_cap_samples(rfIdx,samples));

    double threshold = ui->lineEditThreshold->text().toDouble();
    INT_CHECKV(SP1401()->set_iq_cap_trig_threshold(threshold));

    uint32_t offset = ui->lineEditTrigOffset->text().toUInt();
    INT_CHECKV(SP3301->set_iq_cap_frame_trig_offset(rfIdx,offset));
    ui->widgetFrameSlotTrig->setAll(SP2401R1A());
}

void QIQCapDlg::on_pushButtonAbort_clicked()
{

}
