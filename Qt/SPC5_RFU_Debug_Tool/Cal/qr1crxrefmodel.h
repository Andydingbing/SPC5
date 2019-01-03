#ifndef QR1CRXREFMODEL_H
#define QR1CRXREFMODEL_H

#include "qcalbasemodel.h"
#include "define.h"

class QwtRxRefData : public QwtCalData
{
    DECLARE_CAL_QWT_DATA(QwtRxRefData)

    typedef QVector<rx_ref_table_r1cd::data_f_t>::iterator start_iter;

public:
    explicit QwtRxRefData(int idx);
    void locate2CalTable(start_iter iter);
    void addOneData();

private:
    start_iter iter;
    int n;
    int refIdx;
};


class QR1CRxRefModel : public QCalBaseModel
{
    Q_OBJECT
    DECLARE_CAL_TABLE_MODEL(QR1CRxRefModel)

public:
    QVector<rx_ref_table_r1cd::data_f_t> *calTable();
    QVector<QwtRxRefData *> *iterTable(int idx);
    void resetData();

private:
    QVector<rx_ref_table_r1cd::data_f_t> *table;
    QVector<QwtRxRefData *> *iter;
};

#endif // QR1CRXREFMODEL_H
