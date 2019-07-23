#ifndef QR1CRXFREQRESPONSEMODEL_H
#define QR1CRXFREQRESPONSEMODEL_H

#include <qtestbasemodel.h>
#include "define.h"

class QwtRxFreqResponseData : public QwtTestData
{
    DECLARE_TEST_QWT_DATA(QwtRxFreqResponseData)

    typedef QVector<rx_if_fr_table::data_f_t>::iterator start_iter;

public:
    void locate2CalTable(start_iter iter);
    void addOneData();

private:
    start_iter iter;
    int n;
};

class QR1CRxFreqResponseModel : public QTestBaseModel
{
    Q_OBJECT
    DECLARE_TEST_TABLE_MODEL(QR1CRxFreqResponseModel)

public:
    QVector<rx_if_fr_table::data_f_t> *calTable();
    QVector<QwtRxFreqResponseData *> *iterTable();
    void resetData();

private:
    QVector<rx_if_fr_table::data_f_t> *table;
    QVector<QwtRxFreqResponseData *> *iter;
};

#endif // QR1CRxFreqResponseMODEL_H
