#include "q_temp_ctrl_dlg.h"
#include "ui_q_temp_ctrl_dlg.h"
#include <QAbstractItemModel>
#include "q_attach_thread_dlg.h"
#include "q_r1c_temp_ctrl_model.h"
#include "q_r1c_temp_ctrl_thread.h"
#include "q_plot.h"
#include <QTimer>

QTempCtrlDlg *dlgTempCtrl = nullptr;

QTempCtrlDlg::QTempCtrlDlg(QWidget *parent) :
    QRFDlg(parent),
    ui(new Ui::QTempCtrlDlg)
{
    ui->setupUi(this);

    _sp1401_r1a = nullptr;
    _sp1401_r1c = nullptr;

    initAllSlider(255);
    ui->plot->setTitle(tr("Temperature"));
    ui->plot->setAxisAutoScale(QwtPlot::xBottom);
    ui->plot->setAxisAutoScale(QwtPlot::yLeft);

    QColor clrTable[] = {
        QColor(RGB(255,0,0)),
        QColor(RGB(0,150,0)),
        QColor(RGB(0,0,255)),
        QColor(RGB(255,255,0)),
        QColor(RGB(0,255,255)),
        QColor(RGB(255,128,0)),
        QColor(RGB(128,0,128)),
        QColor(RGB(128,128,0)),
        QColor(RGB(255,0,255)),
        QColor(RGB(64,128,128))
    };
    int clrTableSize = ARRAY_SIZE(clrTable);
    for (qint8 i = 0;i < MAX_RF;i ++) {
        curveR1ATx.append(new QwtPlotCurve);
        curveR1ATx.at(i)->setVisible(false);
        curveR1ATx.at(i)->attach(ui->plot);

        curveR1ARx.append(new QwtPlotCurve);
        curveR1ARx.at(i)->setVisible(false);
        curveR1ARx.at(i)->attach(ui->plot);

        curveR1C.append(new QVector<QwtPlotCurve *>);
        curveR1C.at(i)->append(new QwtPlotCurve("Rx_LO2"));
        curveR1C.at(i)->append(new QwtPlotCurve("Rx_LNA"));
        curveR1C.at(i)->append(new QwtPlotCurve("Rx_LO1"));
        curveR1C.at(i)->append(new QwtPlotCurve("Rx_PA1"));
        curveR1C.at(i)->append(new QwtPlotCurve("Tx_LO2"));
        curveR1C.at(i)->append(new QwtPlotCurve("Tx_LO1"));
        curveR1C.at(i)->append(new QwtPlotCurve("Tx_PA4"));
        curveR1C.at(i)->append(new QwtPlotCurve("Tx_PA3"));


        for (qint8 j = 0;j < 8;j ++) {
            curveR1C.at(i)->at(j)->setPen(clrTable[j % clrTableSize]);
            curveR1C.at(i)->at(j)->setVisible(true);
            curveR1C.at(i)->at(j)->attach(ui->plot);

            dataTempCtr[j] = new QwtTempCtrlData;
            dataTempCtr[j]->calTable()->clear();
            curveR1C.at(i)->at(j)->setSamples(dataTempCtr[j]);
            //curveR1C.at(i)->at(j)->setLegendAttribute(curveR1C.at(i)->at(j)->LegendShowSymbol);
        }

    }
    ui->plot->insertLegend(new QwtLegend(),QwtPlot::RightLegend);
}

QTempCtrlDlg::~QTempCtrlDlg()
{
    delete ui;
}

#define DECLARE_SLIDER_ARRAY \
    QSlider *slider[NUM_FAN] = { \
        ui->verticalSlider1, \
        ui->verticalSlider2, \
        ui->verticalSlider3, \
        ui->verticalSlider4, \
        ui->verticalSlider5, \
        ui->verticalSlider6, \
        ui->verticalSlider7, \
        ui->verticalSlider8, \
        ui->verticalSlider9, \
        ui->verticalSlider10, \
        ui->verticalSlider11, \
        ui->verticalSlider12, \
    };

void QTempCtrlDlg::initAllSlider(int max)
{
    DECLARE_SLIDER_ARRAY

    for (quint8 i = 0;i < NUM_FAN;i ++) {
        slider[i]->setTracking(true);
        slider[i]->setRange(0,max);
        slider[i]->setValue(0);
    }
}

TempParam QTempCtrlDlg::ui2TempParam()
{
    TempParam p;

    p.plot_0 = dataTempCtr[0];
    p.plot_1 = dataTempCtr[1];
    p.plot_2 = dataTempCtr[2];
    p.plot_3 = dataTempCtr[3];
    p.plot_4 = dataTempCtr[4];
    p.plot_5 = dataTempCtr[5];
    p.plot_6 = dataTempCtr[6];
    p.plot_7 = dataTempCtr[7];
    p._sp1401 = _sp1401_r1c;
    //p._sp2401 = _sp2401;
    return p;
}

void QTempCtrlDlg::on_verticalSlider1_valueChanged(int value)
{
    DECLARE_SLIDER_ARRAY

    SP3501.set_fan(0,value);
    ui->labelSpeed1->setText(QString("%1").arg(value));

    if (QApplication::keyboardModifiers() == Qt::ControlModifier) {
        for (quint8 i = 0;i < NUM_FAN;i ++) {
            if (i != 0) {
                slider[i]->setValue(value);
                sleep_ms(5);
            }
        }
    }
}

void QTempCtrlDlg::on_verticalSlider2_valueChanged(int value)
{
    SP3501.set_fan(1,value);
    ui->labelSpeed2->setText(QString("%1").arg(value));
}

void QTempCtrlDlg::on_verticalSlider3_valueChanged(int value)
{
    SP3501.set_fan(2,value);
    ui->labelSpeed3->setText(QString("%1").arg(value));
}

void QTempCtrlDlg::on_verticalSlider4_valueChanged(int value)
{
    SP3501.set_fan(3,value);
    ui->labelSpeed4->setText(QString("%1").arg(value));
}

void QTempCtrlDlg::on_verticalSlider5_valueChanged(int value)
{
    SP3501.set_fan(4,value);
    ui->labelSpeed5->setText(QString("%1").arg(value));
}

void QTempCtrlDlg::on_verticalSlider6_valueChanged(int value)
{
    SP3501.set_fan(5,value);
    ui->labelSpeed6->setText(QString("%1").arg(value));
}

void QTempCtrlDlg::on_verticalSlider7_valueChanged(int value)
{
    SP3501.set_fan(6,value);
    ui->labelSpeed7->setText(QString("%1").arg(value));
}

void QTempCtrlDlg::on_verticalSlider8_valueChanged(int value)
{
    SP3501.set_fan(7,value);
    ui->labelSpeed8->setText(QString("%1").arg(value));
}

void QTempCtrlDlg::on_verticalSlider9_valueChanged(int value)
{
    SP3501.set_fan(8,value);
    ui->labelSpeed9->setText(QString("%1").arg(value));
}

void QTempCtrlDlg::on_verticalSlider10_valueChanged(int value)
{
    SP3501.set_fan(9,value);
    ui->labelSpeed10->setText(QString("%1").arg(value));
}

void QTempCtrlDlg::on_verticalSlider11_valueChanged(int value)
{
    SP3501.set_fan(10,value);
    ui->labelSpeed11->setText(QString("%1").arg(value));
}

void QTempCtrlDlg::on_verticalSlider12_valueChanged(int value)
{
    SP3501.set_fan(11,value);
    ui->labelSpeed12->setText(QString("%1").arg(value));
}

void QTempCtrlDlg::update(double *gettemp)
{
    ui->plot->setAxisScale(QwtPlot::xBottom,0,gettemp[1]+10);
    ui->plot->setAxisScale(QwtPlot::yLeft,35,gettemp[0]+10);

//    ui->plot->setAxisScale(QwtPlot::xBottom,0,gettemp[0]+10);
//    ui->plot->setAxisScale(QwtPlot::yLeft,20,gettemp[1]+10);
    ui->plot->replot();
    ui->plot->show();
}

void QTempCtrlDlg::on_pushButtonStart_clicked()
{
    Param = ui2TempParam();

    //curveR1C[0].at(0)->detach();

    for (qint8 i = 0;i < MAX_RF;i ++) {
        for (qint8 j = 0;j < 8;j ++) {
            dataTempCtr[j]->calTable()->clear();
            curveR1C.at(i)->at(j)->setSamples(dataTempCtr[j]);
        }
        ui->plot->replot();
        ui->plot->show();
    }
    QWinThread::g_threadStop = false;
    QWinThread::g_threadThread = new QR1CTempCtrlThread(&Param,this);
    pThread = (QTempCtrlDlg *)(QWinThread::g_threadThread);
    QWinThread::g_threadThread->start();
    connect(pThread,SIGNAL(update(double*)),this,SLOT(update(double*)),Qt::BlockingQueuedConnection);//by byf
}

void QTempCtrlDlg::on_pushButton_clicked()
{
     QWinThread::g_threadStop = true;

      ui->plot->replot();

}

