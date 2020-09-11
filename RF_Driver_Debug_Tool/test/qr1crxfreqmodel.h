#ifndef QR1CRXFREQMODEL_H
#define QR1CRXFREQMODEL_H

#include <qtestbasemodel.h>
#include "global.h"

class QwtRxFreqTestData : public QwtTestData
{
    DECLARE_TEST_QWT_DATA(QwtRxFreqTestData)

    typedef QVector<rx_if_fr_table::data_f_t>::iterator start_iter;

public:
    void locate2CalTable(start_iter iter);
    void addOneData();

private:
    start_iter iter;
    int n;
};

class QR1CRxFreqModel : public QTestBaseModel
{
    Q_OBJECT
    DECLARE_TEST_TABLE_MODEL(QR1CRxFreqModel)

public:
    QVector<rx_if_fr_table::data_f_t> *calTable();
    QVector<QwtRxFreqTestData *> *iterTable();
    void resetData();

private:
    QVector<rx_if_fr_table::data_f_t> *table;
    QVector<QwtRxFreqTestData *> *iter;
};

#endif // QR1CRxFreqResponseMODEL_H
