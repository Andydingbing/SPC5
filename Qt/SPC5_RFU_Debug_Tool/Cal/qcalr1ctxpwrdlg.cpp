#include "qcalr1ctxpwrdlg.h"
#include "ui_qcalr1ctxpwrdlg.h"
#include "mainwindow.h"
#include "qcalr1ctxpwrthread.h"
#include "qwt_plot_curve.h"

IMPLEMENT_CAL_R1C_DLG(QCalR1CTxPwrDlg,QCalR1CTxPwrThread,QExportR1CTxPwrThread)

QCalR1CTxPwrDlg::QCalR1CTxPwrDlg(QWidget *parent) :
    QCalR1CBaseDlg(parent),
    ui(new Ui::QCalR1CTxPwrDlg)
{
    ui->setupUi(this);
    _model->append(new QR1CTxPwrModel(this));
    _model->append(new QR1CTxPwrModel(this));
    ui->tableViewData_OP->setModel(_model->at(0));
    ui->tableViewData_IO->setModel(_model->at(1));
    ui->tableViewData_OP->setColumnWidth(0,80);
    ui->tableViewData_IO->setColumnWidth(0,80);
    ui->tableViewData_OP->setColumnWidth(1,70);
    ui->tableViewData_IO->setColumnWidth(1,70);
    ui->tableViewData_OP->setColumnWidth(2,40);
    ui->tableViewData_IO->setColumnWidth(2,40);
    ui->tableViewData_OP->setColumnWidth(3,40);
    ui->tableViewData_IO->setColumnWidth(3,40);
    ui->tableViewData_OP->setColumnWidth(4,40);
    ui->tableViewData_IO->setColumnWidth(4,40);
    ui->tableViewData_OP->setColumnWidth(5,40);
    ui->tableViewData_IO->setColumnWidth(5,40);
    ui->tableViewData_OP->setColumnWidth(6,100);
    ui->tableViewData_IO->setColumnWidth(6,100);
    ui->tableViewData_OP->setColumnWidth(7,100);
    ui->tableViewData_IO->setColumnWidth(7,100);
    ui->tableViewData_OP->setColumnWidth(8,100);
    ui->tableViewData_IO->setColumnWidth(8,100);
    ui->tableViewData_OP->setColumnWidth(9,100);
    ui->tableViewData_IO->setColumnWidth(9,100);
    ui->tableViewData_OP->setColumnWidth(10,120);
    ui->tableViewData_IO->setColumnWidth(10,120);
    ui->lineEditFreqStar->setText(QString("%1M").arg(RF_TX_FREQ_STAR / 1000000));
    ui->lineEditFreqStop->setText(QString("%1M").arg(RF_TX_FREQ_STOP / 1000000));
    ui->lineEditFreqStep->setText(QString("%1M").arg(RF_TX_FREQ_STEP_CALLED / 1000000));
    ui->lineEditFreq->setText("2G");
    ui->comboBoxCalIOMode->setCurrentIndex(CalParam::CAL_IO_OP);

    QwtText textPlot = ui->plotDGain->title();
    textPlot.setText(tr("Base Power's Digital Gain Curve(X:Freq(MHz) Y:Digital Gain(dB))"));
    textPlot.setColor(QColor(Qt::blue));
    ui->plotDGain->init(RF_TX_FREQ_STAR / 1000000.0,RF_TX_FREQ_STOP / 1000000.0,(double)(R1C_DGAIN_MIN) + 2.0,(double)(R1C_DGAIN_MAX) - 5.0);
    ui->plotDGain->setTitle(textPlot);

    curveOP = new QVector<QwtPlotCurve *>;
    curveOP->clear();
    curveIO = new QVector<QwtPlotCurve *>;
    curveIO->clear();

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

QCalR1CTxPwrDlg::~QCalR1CTxPwrDlg()
{
    delete ui;
}

void QCalR1CTxPwrDlg::resetShowWidget(CalParam *param)
{
    int64_t freqStar = 0;
    int64_t freqStop = 0;
    int pts = freq2array(param->rfFreqStar,param->rfFreqStop,param->rfFreqStep,NULL,NULL,NULL,&freqStop,&freqStar);
    int sec = str_sections(param->rfFreqStar.toStdString().c_str());

    if (calOP(param->mode)) {
        QR1CTxPwrModel *model = (QR1CTxPwrModel *)(_model->at(0));
        resetCurve(model,curveOP,pts,sec,QColor(Qt::red));
        if (sec > 0)
            curveOP->at(0)->setTitle(QString("OP@%1dBm").arg(R1C_TX_BASE_POWER_OP));
        ui->tableViewData_OP->selectRow(0);
    }
    if (calIO(param->mode)) {
        QR1CTxPwrModel *model = (QR1CTxPwrModel *)(_model->at(1));
        resetCurve(model,curveIO,pts,sec,QColor(Qt::green));
        if (sec > 0)
            curveIO->at(0)->setTitle(QString("IO@%1dBm").arg(R1C_TX_BASE_POWER_IO));
        ui->tableViewData_IO->selectRow(0);
    }
    ui->plotDGain->replot();
}

void QCalR1CTxPwrDlg::resetCurve(QR1CTxPwrModel *model, QVector<QwtPlotCurve *> *curve, int pts, int sec, QColor color)
{
    tx_pwr_table_r1cd::data_f_t data;

    model->resetData();
    for (int i = 0;i < curve->size();i ++)
        curve->at(i)->detach();
    curve->clear();

    for (int i = 0;i < pts;i ++)
        model->calTable()->append(data);

    for (int i = 0;i < sec;i ++) {
        model->iterTable()->append(new QwtTxPwrData);
        curve->append(new QwtPlotCurve);
        curve->at(i)->setPen(QColor(color),2);
        curve->at(i)->attach(ui->plotDGain);
        curve->at(i)->setSamples(model->iterTable()->at(i));
    }

    emit reset();
}

CalParam QCalR1CTxPwrDlg::ui2CalParam()
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

void QCalR1CTxPwrDlg::update(const QModelIndex &tl,const QModelIndex &br,cal_file::cal_item_t item,int sec)
{
    if (cal_file::TX_PWR_OP == item) {
        ui->tableViewData_OP->selectRow(tl.row());
        ui->plotDGain->replot();
    }
    else {
        ui->tableViewData_IO->selectRow(tl.row());
        ui->plotDGain->replot();
    }
}
