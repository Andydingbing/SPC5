#include "qtestr1crxattmodel.h"
#include <QPointF>


QwtRxAttTestOPData::QwtRxAttTestOPData() :
    QwtTestData()
{
    n = 0;
}

QwtRxAttTestOPData::QwtRxAttTestOPData(int idx) :
    QwtTestData()
{
    attIdx = idx;
    n = 0;
}

size_t QwtRxAttTestOPData::size() const
{
    return n;
}

QPointF QwtRxAttTestOPData::sample(size_t i) const
{
    double ref = (*(iter + i)).ref ;
    double offset = (*(iter + i)).RefOffset[attIdx][i];
    return QPointF(ref,offset);
}

void QwtRxAttTestOPData::locate2CalTable(start_iter iter)
{
    this->iter = iter;
}

void QwtRxAttTestOPData::addOneData()
{
    n += 1;
}


QwtRxAttTestIOData::QwtRxAttTestIOData() :
    QwtTestData()
{
    n = 0;
}

QwtRxAttTestIOData::QwtRxAttTestIOData(int idx) :
    QwtTestData()
{
    attIdx = idx;
    n = 0;
}

size_t QwtRxAttTestIOData::size() const
{
    return n;
}

QPointF QwtRxAttTestIOData::sample(size_t i) const
{
    double freq = (*(iter + i)).ref ;
    double offset = (*(iter + i)).RefOffset[attIdx][i];
    return QPointF(freq,offset);
}

void QwtRxAttTestIOData::locate2CalTable(start_iter iter)
{
    this->iter = iter;
}

void QwtRxAttTestIOData::addOneData()
{
    n += 1;
}

QR1CRxAttTestOPModel::QR1CRxAttTestOPModel(QObject *parent) :
    QTestBaseModel(parent)
{
    init();
}

QR1CRxAttTestOPModel::QR1CRxAttTestOPModel(basic_sp1401 *sp1401, QObject *parent)
    : QTestBaseModel(parent)
    , _sp1401(sp1401)
{
    init ();
}

void QR1CRxAttTestOPModel::init()
{
    table = new QVector<rdata_f_t>;
    table->clear();
    iter = new QVector<QwtRxAttTestOPData *>[ RF_RX_TEST_PTS];
    for (qint8 i = 0;i < RF_RX_TEST_PTS;i ++)
        iter[i].clear();
}

int QR1CRxAttTestOPModel::rowCount(const QModelIndex &parent) const
{
    return table->size() *  RF_RX_TEST_PTS;
}

int QR1CRxAttTestOPModel::columnCount(const QModelIndex &parent) const
{
    return 14;
}

QVariant QR1CRxAttTestOPModel::headerData(int section, Qt::Orientation orientation, int role) const
{


    return QVariant();
}

QVariant QR1CRxAttTestOPModel::data(const QModelIndex &index, int role) const
{

    return QVariant();
}

QVector<rdata_f_t> *QR1CRxAttTestOPModel::calTable()
{
    return table;
}

QVector<QwtRxAttTestOPData *> *QR1CRxAttTestOPModel::iterTable(int idx)
{
    return &iter[idx];
}

void QR1CRxAttTestOPModel::resetData()
{
    QVector<QwtRxAttTestOPData *>::const_iterator ptrIter;
    for (qint8 i = 0;i <  RF_RX_TEST_PTS;i ++) {
        for (ptrIter = iter[i].constBegin();ptrIter != iter[i].constEnd();ptrIter ++)
            delete *ptrIter;
        iter[i].clear();
    }
    table->clear();
}

void QR1CRxAttTestOPModel::setSP1401(basic_sp1401 *sp1401)
{
    _sp1401 = sp1401;
}

// QR1CRxAttIOModel
QR1CRxAttTestIOModel::QR1CRxAttTestIOModel(QObject *parent) :
    QTestBaseModel(parent)
{
    init();
}

QR1CRxAttTestIOModel::QR1CRxAttTestIOModel(basic_sp1401 *sp1401, QObject *parent)
    : QTestBaseModel(parent)
    , _sp1401(sp1401)
{
    init();
}

void QR1CRxAttTestIOModel::init()
{
    table = new QVector<rdata_f_t>;
    table->clear();
    iter = new QVector<QwtRxAttTestIOData *>[RF_RX_TEST_PTS];
    for (qint8 i = 0;i <  RF_RX_TEST_PTS;i ++)
        iter[i].clear();
}

int QR1CRxAttTestIOModel::rowCount(const QModelIndex &parent) const
{
    return table->size() *  RF_RX_TEST_PTS;
}

int QR1CRxAttTestIOModel::columnCount(const QModelIndex &parent) const
{
    return 14;
}

QVariant QR1CRxAttTestIOModel::headerData(int section, Qt::Orientation orientation, int role) const
{

    return QVariant();
}

QVariant QR1CRxAttTestIOModel::data(const QModelIndex &index, int role) const
{

    return QVariant();
}

QVector<rdata_f_t> *QR1CRxAttTestIOModel::calTable()
{
    return table;
}

QVector<QwtRxAttTestIOData *> *QR1CRxAttTestIOModel::iterTable(int idx)
{
    return &iter[idx];
}

void QR1CRxAttTestIOModel::resetData()
{
    QVector<QwtRxAttTestIOData *>::const_iterator ptrIter;
    for (qint8 i = 0;i < RF_RX_TEST_PTS;i ++) {
        for (ptrIter = iter[i].constBegin();ptrIter != iter[i].constEnd();ptrIter ++)
            delete *ptrIter;
        iter[i].clear();
    }
    table->clear();
}

void QR1CRxAttTestIOModel::setSP1401(basic_sp1401 *sp1401)
{
    _sp1401 = sp1401;
}
