#include "qr1crxfilteroffsetmodel.h"
#include <QPointF>

// QwtRxFilterOffsetData
QwtRxFilterOffsetData::QwtRxFilterOffsetData() :
    QwtCalData()
{
    n = 0;
}

QwtRxFilterOffsetData::QwtRxFilterOffsetData(int idx) :
    QwtCalData()
{
    offsetIdx = 0;
    n = 0;
}

size_t QwtRxFilterOffsetData::size() const
{
    return n;
}

QPointF QwtRxFilterOffsetData::sample(size_t i) const
{
    double freq = (*(iter + i)).freq / 1000000.0;
    double offset = ad2dBc(_0dBFS - (*(iter + i)).offset[offsetIdx],_0dBFS);
    return QPointF(freq,offset);
}

void QwtRxFilterOffsetData::locate2CalTable(start_iter iter)
{
    this->iter = iter;
}

void QwtRxFilterOffsetData::addOneData()
{
    n += 1;
}

// QR1CRxFilterOffsetModel
QR1CRxFilterOffsetModel::QR1CRxFilterOffsetModel(QObject *parent) :
    QCalBaseModel(parent)
{
    table = new QVector<rx_filter_offset_table::data_f_t>;
    table->clear();
    iter = new QVector<QwtRxFilterOffsetData *>[3];
    iter->clear();
}

int QR1CRxFilterOffsetModel::rowCount(const QModelIndex &parent) const
{
    return table->size();
}

int QR1CRxFilterOffsetModel::columnCount(const QModelIndex &parent) const
{
    return 9;
}

QVariant QR1CRxFilterOffsetModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (Qt::DisplayRole == role && Qt::Horizontal == orientation) {
        QString strHeader[9] = {
            tr("Freq(MHz)"),
            tr("Offset 0"),
            tr("Offset 1"),
            tr("Offset 2"),
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

QVariant QR1CRxFilterOffsetModel::data(const QModelIndex &index, int role) const
{
    if (Qt::DisplayRole == role) {
        if (0 == index.column())
            return QString("%1").arg(table->at(index.row()).freq / 1000000);
        else if (1 == index.column())
            return QString("%1").arg(table->at(index.row()).offset[0]);
        else if (2 == index.column())
            return QString("%1").arg(table->at(index.row()).offset[1]);
        else if (3 == index.column())
            return QString("%1").arg(table->at(index.row()).offset[2]);
        else if (4 == index.column())
            return QString("%1").arg(table->at(index.row()).temp[0]);
        else if (5 == index.column())
            return QString("%1").arg(table->at(index.row()).temp[1]);
        else if (6 == index.column())
            return QString("%1").arg(table->at(index.row()).temp[2]);
        else if (7 == index.column())
            return QString("%1").arg(table->at(index.row()).temp[3]);
        else if (8 == index.column())
            return tm2QString(table->at(index.row()).time);
    }
    else if (Qt::BackgroundColorRole == role)
        return CLR_PROTECT_EYE;
    return QVariant();
}

QVector<rx_filter_offset_table::data_f_t> *QR1CRxFilterOffsetModel::calTable()
{
    return table;
}

QVector<QwtRxFilterOffsetData *> *QR1CRxFilterOffsetModel::iterTable(int idx)
{
    return &iter[idx];
}

void QR1CRxFilterOffsetModel::resetData()
{
    QVector<QwtRxFilterOffsetData *>::const_iterator ptrIter;
    for (qint8 i = 0;i < 3;i ++) {
        for (ptrIter = iter[i].constBegin();ptrIter != iter[i].constEnd();ptrIter ++)
            delete *ptrIter;
        iter[i].clear();
    }
    table->clear();
}
