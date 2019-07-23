#include "qr1ctxfreqmodel.h"

QwtTxRfFrTestData::QwtTxRfFrTestData() :
    QwtTestData()
{
    table = new QVector<tx_rf_fr_table::data_f_t>;
    table->clear();
}

size_t QwtTxRfFrTestData::size() const
{
    return table->size();
}

QPointF QwtTxRfFrTestData::sample(size_t i) const
{
    return QPointF(table->at(i).freq / 1000000.0,table->at(i).pwr);
}

QVector<tx_rf_fr_table::data_f_t> *QwtTxRfFrTestData::calTable() const
{
    return table;
}


QwtTxIfFrTestData::QwtTxIfFrTestData() :
    QwtTestData()
{
    table = new QVector<tx_if_fr_table::data_f_t>;
    table->clear();
}

size_t QwtTxIfFrTestData::size() const
{
    return table->size();
}

QPointF QwtTxIfFrTestData::sample(size_t i) const
{
    return QPointF(table->at(i).freq / 1000000.0,table->at(i).pwr);
}

QVector<tx_if_fr_table::data_f_t> *QwtTxIfFrTestData::calTable() const
{
    return table;
}


QR1CTxFreqModel::QR1CTxFreqModel(QObject *parent) : QTestBaseModel(parent)
{
    table = new QVector<tx_filter_80m_table::data_f_t>;
    table->clear();
}

int QR1CTxFreqModel::rowCount(const QModelIndex &parent) const
{
    return table->size();
}

int QR1CTxFreqModel::columnCount(const QModelIndex &parent) const
{
    return 8;
}

QVariant QR1CTxFreqModel::data(const QModelIndex &index, int role) const
{
    // if (Qt::DisplayRole == role) {
    //     if (0 == index.column())
    //         return QString("%1").arg(table->at(index.row()).freq / 1000000);
    //     else if (1 == index.column())
    //         return Coef2QString(table->at(index.row()).coef_real,TX_FILTER_ORDER_2I);
    //     else if (2 == index.column())
    //         return Coef2QString(table->at(index.row()).coef_imag,TX_FILTER_ORDER_2I);
    //     else if (3 == index.column())
    //         return QString("%1").arg(table->at(index.row()).temp[0]);
    //     else if (4 == index.column())
    //         return QString("%1").arg(table->at(index.row()).temp[1]);
    //     else if (5 == index.column())
    //         return QString("%1").arg(table->at(index.row()).temp[2]);
    //     else if (6 == index.column())
    //         return QString("%1").arg(table->at(index.row()).temp[3]);
    //     else if (7 == index.column())
    //         return tm2QString(table->at(index.row()).time);
    // }
    // else if (Qt::BackgroundColorRole == role)
     //   return CLR_PROTECT_EYE;
    return QVariant();
}

QVariant QR1CTxFreqModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    // if (Qt::DisplayRole == role && Qt::Horizontal == orientation) {
    //     QString strHeader[8] = {
    //         tr("Freq(MHz)"),
    //         tr("Real"),
    //         tr("Imag"),
    //         tr("Temp4_TxLO2"),
    //         tr("Temp5_TxLO1"),
    //         tr("Temp6_TxPA4"),
    //         tr("Temp7_TxPA3"),
    //         tr("End Time")
    //     };
    //     return strHeader[section];
    // }
    return QVariant();
}

QVector<tx_filter_table::data_f_t> *QR1CTxFreqModel::calTable() const
{
    return table;
}
