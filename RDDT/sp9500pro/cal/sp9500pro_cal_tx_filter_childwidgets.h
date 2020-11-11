#ifndef SP9500PRO_CAL_TX_FILTER_CHILDWIDGETS_H
#define SP9500PRO_CAL_TX_FILTER_CHILDWIDGETS_H

#include "rddt_plot.hpp"
#include "rddt_tableview.hpp"
#include "q_model_tx_filter.h"

class QWidget;
class QwtPlotCurve;

namespace NS_SP9500Pro {

typedef Qwt_FR_Data<std::vector<ns_sp9500x::fr_table_t::data_f_t>> Qwt_TX_RF_FR_Data;
typedef Qwt_FR_Data<std::vector<ns_sp9500x::fr_table_t::data_f_t>> Qwt_TX_IF_FR_Data;

class Q_TXFilter_Config_Model : public Q_Config_Table_Model
{
public:
    Q_TXFilter_Config_Model(QObject *parent = nullptr);
};

class Q_TXFilter_Config_Delegate : public Q_Config_Table_Delegate
{
public:
    Q_TXFilter_Config_Delegate(QObject *parent = nullptr);

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

    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

public:
    std::vector<ns_sp9500x::tx_filter_table_t::data_f_t> *table;
};


class Cal_TXFilter_ChildWidgets
{
public:
    Cal_TXFilter_ChildWidgets(QWidget *parent = nullptr);

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
} // namespace NS_SP9500Pro

#endif // SP9500PRO_CAL_TX_FILTER_CHILDWIDGETS_
