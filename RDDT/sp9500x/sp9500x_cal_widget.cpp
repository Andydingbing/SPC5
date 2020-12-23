#include "cal_widget.h"
#include "sp9500x_cal_tx_filter_childwidgets.h"
#include "spec.h"

using namespace ns_sp1403;
using namespace ns_sp9500x;
using namespace NS_SP9500X;

void Q_Cal_TXFilter_Widget::init()
{
    childs = new Cal_TXFilter_ChildWidgets(this);
    model->push_back(childs->model_100);
    model->push_back(childs->model_200);
    model->push_back(childs->model_400);
    model->push_back(childs->model_800);

    ui->lineEditRFFreqs->setText("250M:10M:3G");
    ui->lineEditIFFreqs->setText("100M,200M,400M,800M");

    addIdleWidget(ui->pushButtonExport);
    addIdleWidget(ui->pushButtonStar);
    addIdleWidget(ui->pushButtonGetSet);
    addRunningWidget(ui->pushButtonStop);
    addToPauseWidget(ui->pushButtonPaus);
    addToRunningWidget(ui->pushButtonCont);
    widgetResume();
    QCalBaseDlg::init();
}

void Q_Cal_TXFilter_Widget::resetShowWidget(CalParam *param)
{
    SP1403->cal_file()->prepare_cal(cal_table_t::TX_RF_FR_0000_3000);

//    SP1403->cal_file()->prepare_cal(cal_table_t::TX_Filter);
    childs->resetShowWidget();
    childs->plotRF->replot();
    childs->plotIF->replot();
    emit reset();
}

void Q_Cal_TXFilter_Widget::uiToCalParam(CalParam *param)
{
    param->parent = this;
    param->cal = ui->checkBoxCal->isChecked();
    param->check = ui->checkBoxCheck->isChecked();
}

void Q_Cal_TXFilter_Widget::updateFromParam(const CalR1CParam &param)
{
//    ui->checkBoxRebuildCoef->setChecked(false);
}


//    if (cal_file::TX_RF_FR_0 == item) {
//        ui->tabWidget->plotRF->replot();
//    } else if (cal_file::TX_RF_FR_1 == item) {
//        ui->tabWidget->plotRF->replot();
//    } else if (cal_file::TX_IF_FR == item) {
//        ui->tabWidget->plotIF->replot();
//    } else if (cal_file::TX_FILTER_80 == item) {
//        ui->tabWidget->tableView_80->selectRow(tl.row());
//    } else if (cal_file::TX_FILTER_160 == item) {
//        ui->tabWidget->tableView_160->selectRow(tl.row());
//    }


void Q_Cal_TXFilter_Widget::uiInsert(const int first,const int last,const int cal_table)
{

}

void Q_Cal_TXFilter_Widget::uiUpdate(const int first,const int last,const int cal_table)
{
    switch (cal_table) {
    case cal_table_t::TX_IF_FR_0000_7500 :
        childs->plotIF->replot();
        return;
    case cal_table_t::TX_RF_IF_FR_0000_3000 :
    case cal_table_t::TX_RF_FR_0000_3000 :
    case cal_table_t::TX_RF_FR_3000_4800 :
    case cal_table_t::TX_RF_FR_4800_6000 :
    case cal_table_t::TX_RF_FR_6000_7500 :
        childs->plotRF->replot();
        return;
    }
}

void Q_Cal_TXFilter_Widget::getset()
{
}
