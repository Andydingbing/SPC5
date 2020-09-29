#include "sp9500pro_cal_widget.h"
#include "sp9500pro_cal_tx_filter_childwidgets.h"
#include "q_plot.hpp"
#include "spec.h"

using namespace ns_sp1403;
using namespace ns_sp9500x;
using namespace NS_SP9500Pro;

void Q_Cal_TXFilter_Widget::init()
{
    childs = new Cal_TXFilter_ChildWidgets(this);
    model->push_back(childs->model_80);
    model->push_back(childs->model_160);

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
    SP1403->cal_file()->prepare_cal(cal_table_t::TX_RF_FR_0);
    SP1403->cal_file()->prepare_cal(cal_table_t::TX_RF_FR_1);
    SP1403->cal_file()->prepare_cal(cal_table_t::TX_RF_FR_2);
    SP1403->cal_file()->prepare_cal(cal_table_t::TX_RF_FR_3);
    SP1403->cal_file()->prepare_cal(cal_table_t::TX_Filter);
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
    if (cal_table == cal_table_t::TX_RF_FR_0) {
        childs->plotRF->replot();
    } else if (cal_table == cal_table_t::TX_RF_FR_1) {
        childs->plotRF->replot();
    } else if (cal_table == cal_table_t::TX_IF_FR) {
        childs->plotIF->replot();
    }
}

void Q_Cal_TXFilter_Widget::getset()
{
}
