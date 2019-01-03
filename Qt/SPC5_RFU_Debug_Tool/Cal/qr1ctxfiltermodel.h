#ifndef QR1CTXFILTERMODEL_H
#define QR1CTXFILTERMODEL_H

#include <QPointF>
#include "qcalbasemodel.h"
#include "define.h"

class QwtTxRfFrData : public QwtCalData
{
    DECLARE_CAL_QWT_DATA(QwtTxRfFrData)

public:
    QVector<tx_rf_fr_table::data_f_t> *calTable() const;

private:
    QVector<tx_rf_fr_table::data_f_t> *table;
};


class QwtTxIfFrData : public QwtCalData
{
    DECLARE_CAL_QWT_DATA(QwtTxIfFrData)

public:
    QVector<tx_if_fr_table::data_f_t> *calTable() const;

public:
    QVector<tx_if_fr_table::data_f_t> *table;
};


class QR1CTxFilterModel : public QCalBaseModel
{
    Q_OBJECT
    DECLARE_CAL_TABLE_MODEL(QR1CTxFilterModel)

public:
    QVector<tx_filter_table::data_f_t> *calTable() const;

public:
    QVector<tx_filter_table::data_f_t> *table;
};

#endif // QR1CTXFILTERMODEL_H
