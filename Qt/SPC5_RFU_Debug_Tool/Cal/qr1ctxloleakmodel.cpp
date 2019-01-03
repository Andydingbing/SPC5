#include "qr1ctxloleakmodel.h"

QR1CTxLOLeakModel::QR1CTxLOLeakModel(QObject *parent) : QCalBaseModel(parent)
{
    table = new QVector<tx_lol_table_r1cd::data_f_t>;
    table->clear();
}

int QR1CTxLOLeakModel::rowCount(const QModelIndex &parent) const
{
    return table->size();
}

int QR1CTxLOLeakModel::columnCount(const QModelIndex &parent) const
{
    return 10;
}

QVariant QR1CTxLOLeakModel::data(const QModelIndex &index, int role) const
{
    if (Qt::DisplayRole == role) {
        if (0 == index.column())
            return QString("%1").arg(table->at(index.row()).freq / 1000000);
        else if (1 == index.column())
            return QString("%1").arg(table->at(index.row()).dc_i);
        else if (2 == index.column())
            return QString("%1").arg(table->at(index.row()).dc_q);
        else if (3 == index.column())
            return table->at(index.row()).use_sa ? tr("Spectrum") : tr("Loopback");
        else if (4 == index.column())
            return QString("%1").arg(table->at(index.row()).pwr);
        else if (5 == index.column())
            return QString("%1").arg(table->at(index.row()).temp[0]);
        else if (6 == index.column())
            return QString("%1").arg(table->at(index.row()).temp[1]);
        else if (7 == index.column())
            return QString("%1").arg(table->at(index.row()).temp[2]);
        else if (8 == index.column())
            return QString("%1").arg(table->at(index.row()).temp[3]);
        else if (9 == index.column())
            return tm2QString(table->at(index.row()).time);
    }
    else if (Qt::BackgroundColorRole == role)
        return CLR_PROTECT_EYE;
    return QVariant();
}

QVariant QR1CTxLOLeakModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (Qt::DisplayRole == role && Qt::Horizontal == orientation) {
        QString strHeader[10] = {
            tr("Freq(MHz)"),
            tr("DC Offset I"),
            tr("DC Offset Q"),
            tr("Method"),
            tr("LO Leakage(dBm)"),
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

QVector<tx_lol_table_r1cd::data_f_t> *QR1CTxLOLeakModel::calTable() const
{
    return table;
}
