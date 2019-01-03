#ifndef QR1CTXPWRMODEL_H
#define QR1CTXPWRMODEL_H

#include "qcalbasemodel.h"
#include "define.h"

class QwtTxPwrData : public QwtCalData
{
    DECLARE_CAL_QWT_DATA(QwtTxPwrData)

    typedef QVector<tx_pwr_table_r1cd::data_f_t>::iterator start_iter;

public:
    void locate2CalTable(start_iter iter);
    void addOneData();

private:
    start_iter iter;
    int n;
};


class QR1CTxPwrModel : public QCalBaseModel
{
    Q_OBJECT
    DECLARE_CAL_TABLE_MODEL(QR1CTxPwrModel)

public:
    QVector<tx_pwr_table_r1cd::data_f_t> *calTable();
    QVector<QwtTxPwrData *> *iterTable();
    void resetData();

private:
    QVector<tx_pwr_table_r1cd::data_f_t> *table;
    QVector<QwtTxPwrData *> *iter;
};

#endif // QR1CTXPWRMODEL_H
