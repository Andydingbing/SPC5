#include "QCalR1CRxRefDlg.h"
#include "ui_QCalR1CRxRefDlg.h"
#include "mainwindow.h"
#include "qcalr1crxrefthread.h"
#include "qwt_plot_curve.h"

IMPLEMENT_CAL_R1C_DLG(QCalR1CRxRefDlg,QCalR1CRxRefThread,QExportR1CRxRefThread)

QCalR1CRxRefDlg::QCalR1CRxRefDlg(QWidget *parent) :
    QCalR1CBaseDlg(parent),
    ui(new Ui::QCalR1CRxRefDlg)
{
    ui->setupUi(this);
    _model->append(new QR1CRxRefModel(this));
    _model->append(new QR1CRxRefModel(this));
    ui->tableViewData_OP->setModel(_model->at(0));
    ui->tableViewData_IO->setModel(_model->at(1));
    ui->tableViewData_OP->setColumnWidth(0,80);
    ui->tableViewData_IO->setColumnWidth(0,80);
    ui->tableViewData_OP->setColumnWidth(1,70);
    ui->tableViewData_IO->setColumnWidth(1,70);
    ui->tableViewData_OP->setColumnWidth(2,60);
    ui->tableViewData_IO->setColumnWidth(2,60);
    ui->tableViewData_OP->setColumnWidth(3,60);
    ui->tableViewData_IO->setColumnWidth(3,60);
    ui->tableViewData_OP->setColumnWidth(4,40);
    ui->tableViewData_IO->setColumnWidth(4,40);
    ui->tableViewData_OP->setColumnWidth(5,40);
    ui->tableViewData_IO->setColumnWidth(5,40);
    ui->tableViewData_OP->setColumnWidth(6,40);
    ui->tableViewData_IO->setColumnWidth(6,40);
    ui->tableViewData_OP->setColumnWidth(7,100);
    ui->tableViewData_IO->setColumnWidth(7,100);
    ui->tableViewData_OP->setColumnWidth(8,100);
    ui->tableViewData_IO->setColumnWidth(8,100);
    ui->tableViewData_OP->setColumnWidth(9,100);
    ui->tableViewData_IO->setColumnWidth(9,100);
    ui->tableViewData_OP->setColumnWidth(10,100);
    ui->tableViewData_IO->setColumnWidth(10,100);
    ui->tableViewData_OP->setColumnWidth(11,100);
    ui->tableViewData_IO->setColumnWidth(11,100);
    ui->tableViewData_OP->setColumnWidth(12,120);
    ui->tableViewData_IO->setColumnWidth(12,120);
    ui->lineEditFreqStar->setText(QString("%1M").arg(RF_RX_FREQ_STAR / 1000000));
    ui->lineEditFreqStop->setText(QString("%1M").arg(RF_RX_FREQ_STOP / 1000000));
    ui->lineEditFreqStep->setText(QString("%1M").arg(RF_RX_FREQ_STEP_CALLED_R1C / 1000000));
    ui->lineEditFreq->setText("2G");
    ui->comboBoxCalIOMode->setCurrentIndex(CalParam::CAL_IO_OP);

    QwtText textPlot = ui->plotDGain->title();
    textPlot.setText(tr("Ref's Digital Gain Curve(X:Freq(MHz) Y:Digital Gain(dB))"));
    textPlot.setColor(QColor(Qt::blue));
    ui->plotDGain->init(RF_RX_FREQ_STAR / 1000000.0,RF_RX_FREQ_STOP / 1000000.0,-1.0,2.2);
    ui->plotDGain->setTitle(textPlot);

    curveOP = new QVector<QwtPlotCurve *>[3];
    curveIO = new QVector<QwtPlotCurve *>[3];
    for (qint8 i = 0;i < 3;i ++) {
        curveOP[i].clear();
        curveIO[i].clear();
    }

    addIdleWidget(ui->lineEditFreqStar);
    addIdleWidget(ui->lineEditFreqStop);
    addIdleWidget(ui->comboBoxCalIOMode);
    addIdleWidget(ui->pushButtonExport);
    addIdleWidget(ui->pushButtonStar);
    addIdleWidget(ui->lineEditFreq);
    addIdleWidget(ui->comboBoxIOMode);
    addIdleWidget(ui->lineEditPower);
    addIdleWidget(ui->pushButtonGetSet);
    addRunningWidget(ui->pushButtonStop);
    addToPauseWidget(ui->pushButtonPaus);
    addToRunningWidget(ui->pushButtonCont);
    widgetResume();
    init();
}

QCalR1CRxRefDlg::~QCalR1CRxRefDlg()
{
    delete ui;
}

void QCalR1CRxRefDlg::resetShowWidget(CalParam *param)
{
    int64_t freqStar = 0;
    int64_t freqStop = 0;
    int pts = freq2array(param->rfFreqStar,param->rfFreqStop,param->rfFreqStep,NULL,NULL,NULL,&freqStop,&freqStar);
    int sec = str_sections(param->rfFreqStar.toStdString().c_str());

    if (calOP(param->mode)) {
        QR1CRxRefModel *model = (QR1CRxRefModel *)(_model->at(0));
        resetCurve(model,curveOP,pts,sec,QColor(Qt::red));
        ui->tableViewData_OP->selectRow(0);
    }
    if (calIO(param->mode)) {
        QR1CRxRefModel *model = (QR1CRxRefModel *)(_model->at(1));
        resetCurve(model,curveIO,pts,sec,QColor(Qt::green));
        ui->tableViewData_IO->selectRow(0);
    }
    ui->plotDGain->replot();
}

void QCalR1CRxRefDlg::resetCurve(QR1CRxRefModel *model, QVector<QwtPlotCurve *> *curve, int pts, int sec, QColor color)
{
    rx_ref_table_r1cd::data_f_t data;

    model->resetData();
    for (int i = 0;i < pts;i ++)
        model->calTable()->append(data);

    for (qint8 i = 0;i < 3;i ++) {
        for (int j = 0;j < curve[i].size();j ++)
            curve[i].at(j)->detach();
        curve[i].clear();

        for (int j = 0;j < sec;j ++) {
            model->iterTable(i)->append(new QwtRxRefData(i));
            curve[i].append(new QwtPlotCurve);
            curve[i].at(j)->setPen(QColor(color),2);
            curve[i].at(j)->attach(ui->plotDGain);
            curve[i].at(j)->setSamples(model->iterTable(i)->at(j));
        }
    }

    emit reset();
}

CalParam QCalR1CRxRefDlg::ui2CalParam()
{
    CalParam p;
    p.parent = this;
    p.mode = (CalParam::CalIOMode)(ui->comboBoxCalIOMode->currentIndex());
    p.model_0 = _model->at(0);
    p.model_1 = _model->at(1);
    p._sp1401 = _sp1401;
    p._sp2401 = _sp2401;
    p.rfFreqStar = ui->lineEditFreqStar->text();
    p.rfFreqStop = ui->lineEditFreqStop->text();
    p.rfFreqStep = ui->lineEditFreqStep->text();
    return p;
}

void QCalR1CRxRefDlg::update(const QModelIndex &tl,const QModelIndex &br,cal_file::cal_item_t item,int sec)
{
    if (cal_file::RX_REF_OP == item) {
        ui->tableViewData_OP->selectRow(tl.row());
        ui->plotDGain->replot();
    }
    else {
        ui->tableViewData_IO->selectRow(tl.row());
        ui->plotDGain->replot();
    }
}
