#ifndef SP9500X_CAL_TX_FILTER_CHILDWIDGETS_H
#define SP9500X_CAL_TX_FILTER_CHILDWIDGETS_H

#include "cal_widget.h"
#include "rddt_plot.hpp"
#include "rddt_tableview.hpp"
#include "q_model_tx_filter.h"
#include "utilities.hpp"
#include <QCheckBox>
#include <QVBoxLayout>

namespace NS_SP9500X {

typedef Qwt_FR_Data<std::vector<ns_sp9500x::fr_table_t::data_f_t>> Qwt_TX_RF_FR_Data;
typedef Qwt_FR_Data<std::vector<ns_sp9500x::fr_table_t::data_f_t>> Qwt_TX_IF_FR_Data;

class Q_TXFilter_Config_Model : public Q_Config_Table_Model
{
public:
    Q_TXFilter_Config_Model(QObject *parent = nullptr) :
        Q_Config_Table_Model(parent)
    {
        _item.clear();
        _item << "JustRebuildCoef";

        setRowCount(rowCount(QModelIndex()));
        setColumnCount(columnCount(QModelIndex()));
    }
};


class Q_TXFilter_Config_Delegate : public Q_Config_Table_Delegate
{
public:
    Q_TXFilter_Config_Delegate(QObject *parent = nullptr) :
        Q_Config_Table_Delegate(parent)
    { checkBoxJustRebuildCoef = new QCheckBox; }

    QWidget **first() const { return (QWidget **)(&checkBoxJustRebuildCoef); }

public:
    QCheckBox *checkBoxJustRebuildCoef;
};


class Q_TXFilter_Model : public QCalBaseModel
{
public:
    Q_TXFilter_Model(QObject *parent = nullptr) :
        QCalBaseModel(parent) { table = nullptr; }

    int rowCount(const QModelIndex &/*parent*/) const
    { return table == nullptr ? 0 : int(table->size()); }

    int columnCount(const QModelIndex &/*parent*/) const
    { return 8; }

    QVariant data(const QModelIndex &index, int role) const
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

    QVariant headerData(int section, Qt::Orientation orientation, int role) const
    {
        if (Qt::DisplayRole == role && Qt::Horizontal == orientation) {
            QString strHeader[8] = { "Freq(MHz)","Real","Imag","Temp4","Temp5","Temp6","Temp7","Time" };
            return strHeader[section];
        }
        return QVariant();
    }

public:
    std::vector<ns_sp9500x::tx_filter_table_t::data_f_t> *table;
};


class Cal_TXFilter_ChildWidgets
{
public:
    Cal_TXFilter_ChildWidgets(QWidget *parent = nullptr)
    {
        _parent = parent;

        QWidget *plotWidget = new QWidget;

        plotRF = new Q_RDDT_CalPlot(plotWidget);
        plotRF->init(ns_sp1403::tx_freq_star/1e6,ns_sp1403::tx_freq_stop/1e6,-20.0,10.0);
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

        tableView_80 = new Q_RDDT_TableView(p->ui->tabWidget);
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

    QWidget *_parent;

    // The 2(RF & IF) plots.
    Q_RDDT_CalPlot *plotRF;
    Q_RDDT_CalPlot *plotIF;

    // The curves in particular plot.
    QwtPlotCurve *curveRF_FR_0;
    QwtPlotCurve *curveRF_FR_1;
    QwtPlotCurve *curveIF_FR;

    // The curve data.
    Qwt_TX_RF_FR_Data *dataRF_FR_0;
    Qwt_TX_RF_FR_Data *dataRF_FR_1;
    Qwt_TX_IF_FR_Data *dataIF_FR;

    // The filter coef View/Model.
    Q_RDDT_TableView *tableView_80;
    Q_RDDT_TableView *tableView_160;
    Q_TXFilter_Model *model_80;
    Q_TXFilter_Model *model_160;

    Q_TXFilter_Config_Model *configModel;
    Q_TXFilter_Config_Delegate *configDelegate;
};
} // namespace NS_SP9500X

#endif // SP9500X_CAL_TX_FILTER_CHILDWIDGETS_
