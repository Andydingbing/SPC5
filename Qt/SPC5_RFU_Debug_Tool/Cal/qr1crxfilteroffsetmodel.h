#ifndef QR1CRXFILTEROFFSETMODEL_H
#define QR1CRXFILTEROFFSETMODEL_H

#include <qcalbasemodel.h>
#include "define.h"

class QwtRxFilterOffsetData : public QwtCalData
{
    DECLARE_CAL_QWT_DATA(QwtRxFilterOffsetData)

    typedef QVector<rx_filter_offset_table::data_f_t>::iterator start_iter;

public:
    explicit QwtRxFilterOffsetData(int idx);
    void locate2CalTable(start_iter iter);
    void addOneData();

private:
    start_iter iter;
    int n;
    int offsetIdx;
};

class QR1CRxFilterOffsetModel : public QCalBaseModel
{
    Q_OBJECT
    DECLARE_CAL_TABLE_MODEL(QR1CRxFilterOffsetModel)

public:
    QVector<rx_filter_offset_table::data_f_t> *calTable();
    QVector<QwtRxFilterOffsetData *> *iterTable(int idx);
    void resetData();

private:
    QVector<rx_filter_offset_table::data_f_t> *table;
    QVector<QwtRxFilterOffsetData *> *iter;
};

#endif // QR1CRXFILTEROFFSETMODEL_H
