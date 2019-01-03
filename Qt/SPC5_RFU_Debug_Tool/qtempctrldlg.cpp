#include "qtempctrldlg.h"
#include "ui_qtempctrldlg.h"

QTempCtrlDlg *dlgTempCtrl = NULL;

QTempCtrlDlg::QTempCtrlDlg(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QTempCtrlDlg)
{
    ui->setupUi(this);

    _sp3501 = NULL;
    _sp1401_r1a = NULL;
    _sp1401_r1c = NULL;

    initAllSlider(255);
    ui->plot->setTitle(tr("Temperature"));
    ui->plot->setAxisAutoScale(QwtPlot::xBottom);
    ui->plot->setAxisAutoScale(QwtPlot::yLeft);

    for (qint8 i = 0;i < MAX_RF;i ++) {
        curveR1ATx.append(new QwtPlotCurve);
        curveR1ATx.at(i)->setVisible(false);
        curveR1ATx.at(i)->attach(ui->plot);

        curveR1ARx.append(new QwtPlotCurve);
        curveR1ARx.at(i)->setVisible(false);
        curveR1ARx.at(i)->attach(ui->plot);

        curveR1C.append(new QVector<QwtPlotCurve *>);
        for (qint8 j = 0;j < 8;j ++) {
            curveR1C.at(i)->append(new QwtPlotCurve);
            curveR1C.at(i)->at(j)->setVisible(false);
            curveR1C.at(i)->at(j)->attach(ui->plot);
        }
    }
}

QTempCtrlDlg::~QTempCtrlDlg()
{
    delete ui;
}

#define DECLARE_SLIDER_ARRAY        \
    QSlider *slider[NUM_BLOWER] = { \
        ui->verticalSlider1,        \
        ui->verticalSlider2,        \
        ui->verticalSlider3,        \
        ui->verticalSlider4,        \
        ui->verticalSlider5,        \
        ui->verticalSlider6,        \
        ui->verticalSlider7,        \
        ui->verticalSlider8,        \
        ui->verticalSlider9,        \
        ui->verticalSlider10,       \
        ui->verticalSlider11,       \
    };

void QTempCtrlDlg::initAllSlider(int max)
{
    DECLARE_SLIDER_ARRAY;

    for (qint8 i = 0;i < NUM_BLOWER;i ++) {
        slider[i]->setTracking(true);
        slider[i]->setRange(0,max);
        slider[i]->setValue(0);
    }
}

void QTempCtrlDlg::on_verticalSlider1_valueChanged(int value)
{
    _sp3501->set_blower(0,value);
    ui->labelSpeed1->setText(QString("%1").arg(value));
}

void QTempCtrlDlg::on_verticalSlider2_valueChanged(int value)
{
    _sp3501->set_blower(1,value);
    ui->labelSpeed2->setText(QString("%1").arg(value));
}

void QTempCtrlDlg::on_verticalSlider3_valueChanged(int value)
{
    _sp3501->set_blower(2,value);
    ui->labelSpeed3->setText(QString("%1").arg(value));
}

void QTempCtrlDlg::on_verticalSlider4_valueChanged(int value)
{
    _sp3501->set_blower(3,value);
    ui->labelSpeed4->setText(QString("%1").arg(value));
}

void QTempCtrlDlg::on_verticalSlider5_valueChanged(int value)
{
    _sp3501->set_blower(4,value);
    ui->labelSpeed5->setText(QString("%1").arg(value));
}

void QTempCtrlDlg::on_verticalSlider6_valueChanged(int value)
{
    _sp3501->set_blower(5,value);
    ui->labelSpeed6->setText(QString("%1").arg(value));
}

void QTempCtrlDlg::on_verticalSlider7_valueChanged(int value)
{
    _sp3501->set_blower(6,value);
    ui->labelSpeed7->setText(QString("%1").arg(value));
}

void QTempCtrlDlg::on_verticalSlider8_valueChanged(int value)
{
    _sp3501->set_blower(7,value);
    ui->labelSpeed8->setText(QString("%1").arg(value));
}

void QTempCtrlDlg::on_verticalSlider9_valueChanged(int value)
{
    _sp3501->set_blower(8,value);
    ui->labelSpeed9->setText(QString("%1").arg(value));
}

void QTempCtrlDlg::on_verticalSlider10_valueChanged(int value)
{
    _sp3501->set_blower(9,value);
    ui->labelSpeed10->setText(QString("%1").arg(value));
}

void QTempCtrlDlg::on_verticalSlider11_valueChanged(int value)
{
    _sp3501->set_blower(10,value);
    ui->labelSpeed11->setText(QString("%1").arg(value));
}
