#ifndef QR1CTXFILTEROFFSETMODEL_H
#define QR1CTXFILTEROFFSETMODEL_H

#include <qcalbasemodel.h>
#include "define.h"

class QwtTxFilterOffsetData : public QwtCalData
{
    DECLARE_CAL_QWT_DATA(QwtTxFilterOffsetData)

    typedef QVector<tx_filter_offset_table::data_f_t>::iterator start_iter;

public:
    void locate2CalTable(start_iter iter);
    void addOneData();

private:
    start_iter iter;
    int n;
};

class QR1CTxFilterOffsetModel : public QCalBaseModel
{
    Q_OBJECT
    DECLARE_CAL_TABLE_MODEL(QR1CTxFilterOffsetModel)

public:
    QVector<tx_filter_offset_table::data_f_t> *calTable();
    QVector<QwtTxFilterOffsetData *> *iterTable();
    void resetData();

private:
    QVector<tx_filter_offset_table::data_f_t> *table;
    QVector<QwtTxFilterOffsetData *> *iter;
};

#endif // QR1CTXFILTEROFFSETMODEL_H
