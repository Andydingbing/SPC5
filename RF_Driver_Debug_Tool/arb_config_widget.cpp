#include "arb_config_widget.h"
#include "arb_widget.h"
#include "ui_arb.h"

void Q_ARB_Config_Widget_Helper::init()
{
    Q_ARB_Widget *p = dynamic_cast<Q_ARB_Widget *>(parent);

    p->ui->tableView_Config->setModel(model);
    p->ui->tableView_Config->setItemDelegate(delegate);
    p->ui->tableView_Config->resizeColumnsToContents();
    p->ui->tableView_Config->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Fixed);
    p->ui->tableView_Config->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch);

    for (int i = 0;i < model->rowCount(QModelIndex());++i) {
        p->ui->tableView_Config->openPersistentEditor(model->index(i,1));
    }
}
