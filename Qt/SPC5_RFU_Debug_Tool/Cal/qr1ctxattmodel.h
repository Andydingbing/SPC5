#ifndef QR1CTXATTMODEL_H
#define QR1CTXATTMODEL_H

#include <qcalbasemodel.h>
#include "define.h"

class QwtTxAttOPData : public QwtCalData
{
    DECLARE_CAL_QWT_DATA(QwtTxAttOPData)

    typedef QVector<tx_att_op_table_r1cd::data_f_t>::iterator start_iter;

public:
    explicit QwtTxAttOPData(int idx);
    void locate2CalTable(start_iter iter);
    void addOneData();

private:
    start_iter iter;
    int n;
    int attIdx;
};

class QwtTxAttIOData : public QwtCalData
{
    DECLARE_CAL_QWT_DATA(QwtTxAttIOData)

    typedef QVector<tx_att_io_table_r1cd::data_f_t>::iterator start_iter;

public:
    explicit QwtTxAttIOData(int idx);
    void locate2CalTable(start_iter iter);
    void addOneData();

private:
    start_iter iter;
    int n;
    int attIdx;
};


class QR1CTxAttOPModel : public QCalBaseModel
{
    Q_OBJECT
    DECLARE_CAL_TABLE_MODEL(QR1CTxAttOPModel)

public:
    explicit QR1CTxAttOPModel(sp1401_r1c *sp1401, QObject *parent = 0);
    QVector<tx_att_op_table_r1cd::data_f_t> *calTable();
    QVector<QwtTxAttOPData *> *iterTable(int idx);
    void init();
    void resetData();
    void setSP1401(sp1401_r1c *);

private:
    QVector<tx_att_op_table_r1cd::data_f_t> *table;
    QVector<QwtTxAttOPData *> *iter;
    sp1401_r1c *_sp1401;
};

class QR1CTxAttIOModel : public QCalBaseModel
{
    Q_OBJECT
    DECLARE_CAL_TABLE_MODEL(QR1CTxAttIOModel)

public:
    explicit QR1CTxAttIOModel(sp1401_r1c *sp1401, QObject *parent = 0);
    QVector<tx_att_io_table_r1cd::data_f_t> *calTable();
    QVector<QwtTxAttIOData *> *iterTable(int idx);
    void init();
    void resetData();
    void setSP1401(sp1401_r1c *);

private:
    QVector<tx_att_io_table_r1cd::data_f_t> *table;
    QVector<QwtTxAttIOData *> *iter;
    sp1401_r1c *_sp1401;
};

#endif // QR1CTXATTMODEL_H
