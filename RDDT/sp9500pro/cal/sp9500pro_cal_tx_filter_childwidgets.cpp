#include "sp9500pro_cal_tx_filter_childwidgets.h"
#include "global.h"
#include "q_base_model.h"
#include "cal_widget.h"
#include <QVBoxLayout>
#include "utilities.hpp"

using namespace ns_sp1403;
using namespace ns_sp2406;
using namespace NS_SP9500Pro;

Q_TXFilter_Config_Model::Q_TXFilter_Config_Model(QObject *parent) :
        Q_Config_Table_Model(parent)
{
    _item.clear();
    _item << "JustRebuildCoef";

    setRowCount(rowCount(QModelIndex()));
    setColumnCount(columnCount(QModelIndex()));
}


Q_TXFilter_Config_Delegate::Q_TXFilter_Config_Delegate(QObject *parent) :
        Q_Config_Table_Delegate(parent)
{
    checkBoxJustRebuildCoef = new QCheckBox;
}


QVariant Q_TXFilter_Model::data(const QModelIndex &index, int role) const
{
    if (Qt::DisplayRole == role) {
        if (index.column() == 0) {
            return QString("%1").arg(table->at(index.row()).freq / 1000000);
        } else if (index.column() == 1) {
            return string_of<QString>(table->at(index.row()).coef_real,ns_sp2406::dl_cf_tap,",");
        } else if (index.column() == 2) {
            return string_of<QString>(table->at(index.row()).coef_imag,ns_sp2406::dl_cf_tap,",");
        } else if (index.column() == 3) {
            return QString("%1").arg(table->at(index.row()).temp[0]);
        } else if (index.column() == 4) {
            return QString("%1").arg(table->at(index.row()).temp[1]);
        } else if (index.column() == 5) {
            return QString("%1").arg(table->at(index.row()).temp[2]);
        } else if (index.column() == 6) {
            return QString("%1").arg(table->at(index.row()).temp[3]);
        } else if (index.column() == 7) {
            return string_of<QString>(table->at(index.row()).time);
        }
    } else if (Qt::BackgroundColorRole == role) {
        return CLR_PROTECT_EYE;
    }

    return QVariant();
}

QVariant Q_TXFilter_Model::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (Qt::DisplayRole == role && Qt::Horizontal == orientation) {
        QString strHeader[8] = { "Freq(MHz)","Real","Imag","Temp4","Temp5","Temp6","Temp7","Time" };
        return strHeader[section];
    }
    return QVariant();
}


Cal_TXFilter_ChildWidgets::Cal_TXFilter_ChildWidgets(QWidget *parent)
{
    _parent = parent;

    QWidget *plotWidget = new QWidget;

    plotRF = new Q_RDDT_CalPlot(plotWidget);
    plotRF->init(tx_freq_star/1e6,tx_freq_stop/1e6,-20.0,10.0);
    plotRF->setTitle("RF Freq Response(dBm/MHz)");
    plotRF->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    plotIF = new Q_RDDT_CalPlot(plotWidget);
//    plotIF->init(-/1e6,dl_filter_160M_freq_stop/1e6,-20.0,4.0);
    plotIF->setTitle("IF Freq Response(@RF 2GHz)(dBm/MHz)");
    plotIF->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    QVBoxLayout *plotLayout = new QVBoxLayout(plotWidget);
    plotLayout->addWidget(plotRF);
    plotLayout->addWidget(plotIF);
    plotLayout->setStretch(0,1);
    plotLayout->setStretch(1,1);

    dataRF_FR_0 = new Qwt_TX_RF_FR_Data;
    dataRF_FR_1 = new Qwt_TX_RF_FR_Data;
    dataIF_FR = new Qwt_TX_IF_FR_Data;

    curveRF_FR_0 = new QwtPlotCurve("RF Freq Response 0");
    curveRF_FR_0->setPen(QColor(Qt::red));
    curveRF_FR_0->setVisible(true);
    curveRF_FR_0->attach(plotRF);
    curveRF_FR_0->setSamples(dataRF_FR_0);

    curveRF_FR_1 = new QwtPlotCurve("RF Freq Response 1");
    curveRF_FR_1->setPen(QColor(Qt::green));
    curveRF_FR_1->setVisible(true);
    curveRF_FR_1->attach(plotRF);
    curveRF_FR_1->setSamples(dataRF_FR_1);

    curveIF_FR = new QwtPlotCurve("IF Freq Response");
    curveIF_FR->setPen(QColor(Qt::red));
    curveIF_FR->setVisible(true);
    curveIF_FR->attach(plotIF);
    curveIF_FR->setSamples(dataIF_FR);

    Q_Cal_TXFilter_Widget *p = dynamic_cast<Q_Cal_TXFilter_Widget *>(_parent);

    model_80  = new Q_TXFilter_Model;
    model_160 = new Q_TXFilter_Model;

    tableView_80  = new Q_RDDT_TableView(p->ui->tabWidget);
    tableView_80->setModel(model_80);

    tableView_160 = new Q_RDDT_TableView(p->ui->tabWidget);
    tableView_160->setModel(model_160);

    p->ui->tabWidget->clear();
    p->ui->tabWidget->addTab(plotWidget,QString("Freq Response"));
    p->ui->tabWidget->addTab(tableView_80, QString("Coef_80M"));
    p->ui->tabWidget->addTab(tableView_160,QString("Coef_160M"));

    configModel = new Q_TXFilter_Config_Model(p->ui->tableViewConfig);
    configDelegate = new Q_TXFilter_Config_Delegate(p->ui->tableViewConfig);

    p->ui->tableViewConfig->setModel(configModel);
    p->ui->tableViewConfig->setItemDelegate(configDelegate);
    p->ui->tableViewConfig->resizeColumnsToContents();
    p->ui->tableViewConfig->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Fixed);
    p->ui->tableViewConfig->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch);

    for (int i = 0;i < configModel->rowCount(QModelIndex());++i) {
        p->ui->tableViewConfig->openPersistentEditor(configModel->index(i,1));
    }
}
