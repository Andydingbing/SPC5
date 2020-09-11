#ifndef QR1CTESPPOWMODEL_H
#define QR1CTESPPOWMODEL_H

#include <qtestbasemodel.h>
#include "global.h"
typedef struct data_f_t {
    uint64_t freq;
    double cableloss;
    double TargetPower;
    double ActualPower;
    double   PowOffset[95][95];
    tm       time;
}data_f_t;
class QwtTestPowOPData : public QwtTestData
{
    DECLARE_TEST_QWT_DATA(QwtTestPowOPData)

    typedef QVector<data_f_t>::iterator start_iter;

public:
    explicit QwtTestPowOPData(int idx);
    void locate2CalTable(start_iter iter);
    void addOneData();

private:
    start_iter iter;
    int n;
    int attIdx;
};

class QwtTestPowIOData : public QwtTestData
{
    DECLARE_TEST_QWT_DATA(QwtTestPowIOData)

    typedef QVector<data_f_t>::iterator start_iter;

public:
    explicit QwtTestPowIOData(int idx);
    void locate2CalTable(start_iter iter);
    void addOneData();

private:
    start_iter iter;
    int n;
    int attIdx;
};


class QR1CTestPowOPModel : public QTestBaseModel
{
    Q_OBJECT
    DECLARE_TEST_TABLE_MODEL(QR1CTestPowOPModel)

public:
    explicit QR1CTestPowOPModel(basic_sp1401 *sp1401, QObject *parent = 0);
    QVector<data_f_t> *calTable();
    QVector<QwtTestPowOPData *> *iterTable(int idx);
    void init();
    void resetData();
    void setSP1401(basic_sp1401 *);

private:
    QVector<data_f_t> *table;
    QVector<QwtTestPowOPData *> *iter;
    basic_sp1401 *_sp1401;
};

class QR1CTestPowIOModel : public QTestBaseModel
{
    Q_OBJECT
    DECLARE_TEST_TABLE_MODEL(QR1CTestPowIOModel)

public:
    explicit QR1CTestPowIOModel(basic_sp1401 *sp1401, QObject *parent = 0);
    QVector<data_f_t> *calTable();
    QVector<QwtTestPowIOData *> *iterTable(int idx);
    void init();
    void resetData();
    void setSP1401(basic_sp1401 *);

private:
    QVector<data_f_t> *table;
    QVector<QwtTestPowIOData *> *iter;
    basic_sp1401 *_sp1401;
};

#endif // QR1CTESTPOWMODEL_H
