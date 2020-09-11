#ifndef QR1CRXATTTESTMODEL_H
#define QR1CRXATTTESTMODEL_H

#include <qtestbasemodel.h>
#include "global.h"

typedef struct  rdata_f_t {
    uint64_t freq;
    int64_t  ref;
    double  RefOffset[100][100];
    double   temp[4];
    tm       time;
    int32_t  rsv_0;
}rdata_f_t;

class QwtRxAttTestOPData : public QwtTestData
{
    DECLARE_TEST_QWT_DATA(QwtRxAttTestOPData)

    typedef QVector<rdata_f_t>::iterator start_iter;

public:
    explicit QwtRxAttTestOPData(int idx);
    void locate2CalTable(start_iter iter);
    void addOneData();

private:
    start_iter iter;
    int n;
    int attIdx;
};

class QwtRxAttTestIOData : public QwtTestData
{
    DECLARE_TEST_QWT_DATA(QwtRxAttTestIOData)

    typedef QVector<rdata_f_t>::iterator start_iter;

public:
    explicit QwtRxAttTestIOData(int idx);
    void locate2CalTable(start_iter iter);
    void addOneData();

private:
    start_iter iter;
    int n;
    int attIdx;
};


class QR1CRxAttTestOPModel : public QTestBaseModel
{
    Q_OBJECT
    DECLARE_TEST_TABLE_MODEL(QR1CRxAttTestOPModel)

public:
    explicit QR1CRxAttTestOPModel(basic_sp1401 *sp1401, QObject *parent = 0);
    QVector<rdata_f_t> *calTable();
    QVector<QwtRxAttTestOPData *> *iterTable(int idx);
    void init();
    void resetData();
    void setSP1401(basic_sp1401 *);

private:
    QVector<rdata_f_t> *table;
    QVector<QwtRxAttTestOPData *> *iter;
    basic_sp1401 *_sp1401;
};

class QR1CRxAttTestIOModel : public QTestBaseModel
{
    Q_OBJECT
    DECLARE_TEST_TABLE_MODEL(QR1CRxAttTestIOModel)

public:
    explicit QR1CRxAttTestIOModel(basic_sp1401 *sp1401, QObject *parent = 0);
    QVector<rdata_f_t> *calTable();
    QVector<QwtRxAttTestIOData *> *iterTable(int idx);
    void init();
    void resetData();
    void setSP1401(basic_sp1401 *);

private:
    QVector<rdata_f_t> *table;
    QVector<QwtRxAttTestIOData *> *iter;
    basic_sp1401 *_sp1401;
};

#endif // QR1CRXATTTESTMODEL_H
