#include "qr1crxfiltermodel.h"

QwtRxRfFrData::QwtRxRfFrData() :
    QwtCalData()
{
    table = new QVector<rx_rf_fr_table::data_f_t>;
    table->clear();
}

size_t QwtRxRfFrData::size() const
{
    return table->size();
}

QPointF QwtRxRfFrData::sample(size_t i) const
{
    return QPointF(table->at(i).freq / 1000000.0,table->at(i).pwr);
}

QVector<rx_rf_fr_table::data_f_t> *QwtRxRfFrData::calTable() const
{
    return table;
}


QwtRxIfFrData::QwtRxIfFrData() :
    QwtCalData()
{
    table = new QVector<rx_if_fr_table::data_f_t>;
    table->clear();
}

size_t QwtRxIfFrData::size() const
{
    return table->size();
}

QPointF QwtRxIfFrData::sample(size_t i) const
{
    return QPointF(table->at(i).freq / 1000000.0,table->at(i).pwr);
}

QVector<rx_if_fr_table::data_f_t> *QwtRxIfFrData::calTable() const
{
    return table;
}


QR1CRxFilterModel_80M::QR1CRxFilterModel_80M(QObject *parent) : QCalBaseModel(parent)
{
    table = new QVector<rx_filter_80m_table::data_f_t>;
    table->clear();
}

int QR1CRxFilterModel_80M::rowCount(const QModelIndex &parent) const
{
    return table->size();
}

int QR1CRxFilterModel_80M::columnCount(const QModelIndex &parent) const
{
    return 8;
}

QVariant QR1CRxFilterModel_80M::data(const QModelIndex &index, int role) const
{
    if (Qt::DisplayRole == role) {
        if (0 == index.column())
            return QString("%1").arg(table->at(index.row()).freq / 1000000);
        else if (1 == index.column())
            return Coef2QString(table->at(index.row()).coef_real,RX_FILTER_ORDER);
        else if (2 == index.column())
            return Coef2QString(table->at(index.row()).coef_imag,RX_FILTER_ORDER);
        else if (3 == index.column())
            return QString("%1").arg(table->at(index.row()).temp[0]);
        else if (4 == index.column())
            return QString("%1").arg(table->at(index.row()).temp[1]);
        else if (5 == index.column())
            return QString("%1").arg(table->at(index.row()).temp[2]);
        else if (6 == index.column())
            return QString("%1").arg(table->at(index.row()).temp[3]);
        else if (7 == index.column())
            return tm2QString(table->at(index.row()).time);
    }
    else if (Qt::BackgroundColorRole == role)
        return CLR_PROTECT_EYE;
    return QVariant();
}

QVariant QR1CRxFilterModel_80M::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (Qt::DisplayRole == role && Qt::Horizontal == orientation) {
        QString strHeader[8] = {
            tr("Freq(MHz)"),
            tr("Real"),
            tr("Imag"),
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

QVector<rx_filter_80m_table::data_f_t> *QR1CRxFilterModel_80M::calTable() const
{
    return table;
}


QR1CRxFilterModel_160M::QR1CRxFilterModel_160M(QObject *parent) : QCalBaseModel(parent)
{
    table = new QVector<rx_filter_160m_table::data_f_t>;
    table->clear();
}

int QR1CRxFilterModel_160M::rowCount(const QModelIndex &parent) const
{
    return table->size();
}

int QR1CRxFilterModel_160M::columnCount(const QModelIndex &parent) const
{
    return 8;
}

QVariant QR1CRxFilterModel_160M::data(const QModelIndex &index, int role) const
{
    if (Qt::DisplayRole == role) {
        if (0 == index.column())
            return QString("%1").arg(table->at(index.row()).freq / 1000000);
        else if (1 == index.column())
            return Coef2QString(table->at(index.row()).coef_real,RX_FILTER_ORDER_2I3D);
        else if (2 == index.column())
            return Coef2QString(table->at(index.row()).coef_imag,RX_FILTER_ORDER_2I3D);
        else if (3 == index.column())
            return QString("%1").arg(table->at(index.row()).temp[0]);
        else if (4 == index.column())
            return QString("%1").arg(table->at(index.row()).temp[1]);
        else if (5 == index.column())
            return QString("%1").arg(table->at(index.row()).temp[2]);
        else if (6 == index.column())
            return QString("%1").arg(table->at(index.row()).temp[3]);
        else if (7 == index.column())
            return tm2QString(table->at(index.row()).time);
    }
    else if (Qt::BackgroundColorRole == role)
        return CLR_PROTECT_EYE;
    return QVariant();
}

QVariant QR1CRxFilterModel_160M::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (Qt::DisplayRole == role && Qt::Horizontal == orientation) {
        QString strHeader[8] = {
            tr("Freq(MHz)"),
            tr("Real"),
            tr("Imag"),
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

QVector<rx_filter_160m_table::data_f_t> *QR1CRxFilterModel_160M::calTable() const
{
    return table;
}
