#ifndef QR1CTXFREQMODEL_H
#define QR1CTXFREQMODEL_H

#include <QPointF>
#include "qtestbasemodel.h"
#include "define.h"

class QwtTxRfFrTestData : public QwtTestData
{
    DECLARE_TEST_QWT_DATA(QwtTxRfFrTestData)

public:
    QVector<tx_rf_fr_table::data_f_t> *calTable() const;

private:
    QVector<tx_rf_fr_table::data_f_t> *table;
};


class QwtTxIfFrTestData : public QwtTestData
{
    DECLARE_TEST_QWT_DATA(QwtTxIfFrTestData)

public:
    QVector<tx_if_fr_table::data_f_t> *calTable() const;

public:
    QVector<tx_if_fr_table::data_f_t> *table;
};


class QR1CTxFreqModel : public QTestBaseModel
{
    Q_OBJECT
    DECLARE_TEST_TABLE_MODEL(QR1CTxFreqModel)

public:
    QVector<tx_filter_table::data_f_t> *calTable() const;

public:
    QVector<tx_filter_table::data_f_t> *table;
};

#endif // QR1CTXFREQMODEL_H
