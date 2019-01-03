#ifndef QR1CRXFILTERMODEL_H
#define QR1CRXFILTERMODEL_H

#include <QPointF>
#include "qcalbasemodel.h"
#include "define.h"

class QwtRxRfFrData : public QwtCalData
{
    DECLARE_CAL_QWT_DATA(QwtRxRfFrData)

public:
    QVector<rx_rf_fr_table::data_f_t> *calTable() const;

private:
    QVector<rx_rf_fr_table::data_f_t> *table;
};


class QwtRxIfFrData : public QwtCalData
{
    DECLARE_CAL_QWT_DATA(QwtRxIfFrData)

public:
    QVector<rx_if_fr_table::data_f_t> *calTable() const;

public:
    QVector<rx_if_fr_table::data_f_t> *table;
};


class QR1CRxFilterModel_80M : public QCalBaseModel
{
    Q_OBJECT
    DECLARE_CAL_TABLE_MODEL(QR1CRxFilterModel_80M)

public:
    QVector<rx_filter_80m_table::data_f_t> *calTable() const;

public:
    QVector<rx_filter_80m_table::data_f_t> *table;
};


class QR1CRxFilterModel_160M : public QCalBaseModel
{
    Q_OBJECT
    DECLARE_CAL_TABLE_MODEL(QR1CRxFilterModel_160M)

public:
    QVector<rx_filter_160m_table::data_f_t> *calTable() const;

public:
    QVector<rx_filter_160m_table::data_f_t> *table;
};

#endif // QR1CRXFILTERMODEL_H
