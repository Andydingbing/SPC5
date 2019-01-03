#include "qr1ctxfilteroffsetmodel.h"
#include <QPointF>

// QwtTxFilterOffsetData
QwtTxFilterOffsetData::QwtTxFilterOffsetData() :
    QwtCalData()
{
    n = 0;
}

size_t QwtTxFilterOffsetData::size() const
{
    return n;
}

QPointF QwtTxFilterOffsetData::sample(size_t i) const
{
    return QPointF((*(iter + i)).freq / 1000000.0,(*(iter + i)).offset);
}

void QwtTxFilterOffsetData::locate2CalTable(start_iter iter)
{
    this->iter = iter;
}

void QwtTxFilterOffsetData::addOneData()
{
    n += 1;
}

// QR1CTxFilterOffsetModel
QR1CTxFilterOffsetModel::QR1CTxFilterOffsetModel(QObject *parent) : QCalBaseModel(parent)
{
    table = new QVector<tx_filter_offset_table::data_f_t>;
    table->clear();
    iter = new QVector<QwtTxFilterOffsetData *>;
    iter->clear();
}

int QR1CTxFilterOffsetModel::rowCount(const QModelIndex &parent) const
{
    return table->size();
}

int QR1CTxFilterOffsetModel::columnCount(const QModelIndex &parent) const
{
    return 7;
}

QVariant QR1CTxFilterOffsetModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (Qt::DisplayRole == role && Qt::Horizontal == orientation) {
        QString strHeader[7] = {
            tr("Freq(MHz)"),
            tr("Offset"),
            tr("Temp4_TxLO2"),
            tr("Temp5_TxLO1"),
            tr("Temp6_TxPA4"),
            tr("Temp7_TxPA3"),
            tr("End Time")
        };
        return strHeader[section];
    }

    return QVariant();
}

QVariant QR1CTxFilterOffsetModel::data(const QModelIndex &index, int role) const
{
    if (Qt::DisplayRole == role) {
        if (0 == index.column())
            return QString("%1").arg(table->at(index.row()).freq / 1000000);
        else if (1 == index.column())
            return QString("%1").arg(table->at(index.row()).offset,0,'f',3);
        else if (2 == index.column())
            return QString("%1").arg(table->at(index.row()).temp[0]);
        else if (3 == index.column())
            return QString("%1").arg(table->at(index.row()).temp[1]);
        else if (4 == index.column())
            return QString("%1").arg(table->at(index.row()).temp[2]);
        else if (5 == index.column())
            return QString("%1").arg(table->at(index.row()).temp[3]);
        else if (6 == index.column())
            return tm2QString(table->at(index.row()).time);
    }
    else if (Qt::BackgroundColorRole == role)
        return CLR_PROTECT_EYE;
    return QVariant();
}

QVector<tx_filter_offset_table::data_f_t> *QR1CTxFilterOffsetModel::calTable()
{
    return table;
}

QVector<QwtTxFilterOffsetData *> *QR1CTxFilterOffsetModel::iterTable()
{
    return iter;
}

void QR1CTxFilterOffsetModel::resetData()
{
    QVector<QwtTxFilterOffsetData *>::const_iterator ptrIter;
    for (ptrIter = iter->constBegin();ptrIter != iter->constEnd();ptrIter ++)
        delete *ptrIter;
    iter->clear();
    table->clear();
}
