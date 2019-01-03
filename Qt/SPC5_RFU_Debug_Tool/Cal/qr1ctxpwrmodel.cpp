#include "qr1ctxpwrmodel.h"
#include <QPointF>

QwtTxPwrData::QwtTxPwrData() :
    QwtCalData()
{
    n = 0;
}

size_t QwtTxPwrData::size() const
{
    return n;
}

QPointF QwtTxPwrData::sample(size_t i) const
{
    return QPointF((*(iter + i)).freq / 1000000.0,(*(iter + i)).d_gain);
}

void QwtTxPwrData::locate2CalTable(start_iter iter)
{
    this->iter = iter;
}

void QwtTxPwrData::addOneData()
{
    n += 1;
}


QR1CTxPwrModel::QR1CTxPwrModel(QObject *parent) : QCalBaseModel(parent)
{
    table = new QVector<tx_pwr_table_r1cd::data_f_t>;
    table->clear();
    iter = new QVector<QwtTxPwrData *>;
    iter->clear();
}

int QR1CTxPwrModel::rowCount(const QModelIndex &parent) const
{
    return table->size();
}

int QR1CTxPwrModel::columnCount(const QModelIndex &parent) const
{
    return 11;
}

QVariant QR1CTxPwrModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (Qt::DisplayRole == role && Qt::Horizontal == orientation) {
        QString strHeader[11] = {
            tr("Freq(MHz)"),
            tr("DGain(dBm)"),
            tr("Att0"),
            tr("Att1"),
            tr("Att2"),
            tr("Att3"),
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

QVariant QR1CTxPwrModel::data(const QModelIndex &index, int role) const
{
    if (Qt::DisplayRole == role) {
        if (0 == index.column())
            return QString("%1").arg(table->at(index.row()).freq / 1000000);
        else if (1 == index.column())
            return QString("%1").arg(table->at(index.row()).d_gain,0,'f',3);
        else if (2 == index.column())
            return QString("%1").arg(table->at(index.row()).att0);
        else if (3 == index.column())
            return QString("%1").arg(table->at(index.row()).att1);
        else if (4 == index.column())
            return QString("%1").arg(table->at(index.row()).att2);
        else if (5 == index.column())
            return QString("%1").arg(table->at(index.row()).att3);
        else if (6 == index.column())
            return QString("%1").arg(table->at(index.row()).temp[0]);
        else if (7 == index.column())
            return QString("%1").arg(table->at(index.row()).temp[1]);
        else if (8 == index.column())
            return QString("%1").arg(table->at(index.row()).temp[2]);
        else if (9 == index.column())
            return QString("%1").arg(table->at(index.row()).temp[3]);
        else if (10 == index.column())
            return tm2QString(table->at(index.row()).time);
    }
    else if (Qt::BackgroundColorRole == role)
        return CLR_PROTECT_EYE;
    return QVariant();
}

QVector<tx_pwr_table_r1cd::data_f_t> *QR1CTxPwrModel::calTable()
{
    return table;
}

QVector<QwtTxPwrData *> *QR1CTxPwrModel::iterTable()
{
    return iter;
}

void QR1CTxPwrModel::resetData()
{
    QVector<QwtTxPwrData *>::const_iterator ptrIter;
    for (ptrIter = iter->constBegin();ptrIter != iter->constEnd();ptrIter ++)
        delete *ptrIter;
    iter->clear();
    table->clear();
}
