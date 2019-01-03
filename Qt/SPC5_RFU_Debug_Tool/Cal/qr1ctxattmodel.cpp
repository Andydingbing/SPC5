#include "qr1ctxattmodel.h"
#include <QPointF>

// QwtTxAttOPData
QwtTxAttOPData::QwtTxAttOPData() :
    QwtCalData()
{
    n = 0;
}

QwtTxAttOPData::QwtTxAttOPData(int idx) :
    QwtCalData()
{
    attIdx = idx;
    n = 0;
}

size_t QwtTxAttOPData::size() const
{
    return n;
}

QPointF QwtTxAttOPData::sample(size_t i) const
{
    return QPointF((*(iter + i)).freq / 1000000.0,(*(iter + i)).offset[attIdx]);
}

void QwtTxAttOPData::locate2CalTable(start_iter iter)
{
    this->iter = iter;
}

void QwtTxAttOPData::addOneData()
{
    n += 1;
}

// QwtTxAttIOData
QwtTxAttIOData::QwtTxAttIOData() :
    QwtCalData()
{
    n = 0;
}

QwtTxAttIOData::QwtTxAttIOData(int idx) :
    QwtCalData()
{
    attIdx = idx;
    n = 0;
}

size_t QwtTxAttIOData::size() const
{
    return n;
}

QPointF QwtTxAttIOData::sample(size_t i) const
{
    return QPointF((*(iter + i)).freq / 1000000.0,(*(iter + i)).offset[attIdx]);
}

void QwtTxAttIOData::locate2CalTable(start_iter iter)
{
    this->iter = iter;
}

void QwtTxAttIOData::addOneData()
{
    n += 1;
}

// QR1CTxAttOPModel
QR1CTxAttOPModel::QR1CTxAttOPModel(QObject *parent) :
    QCalBaseModel(parent)
{
    init();
}

QR1CTxAttOPModel::QR1CTxAttOPModel(sp1401_r1c *sp1401, QObject *parent)
    : QCalBaseModel(parent)
    , _sp1401(sp1401)
{
    init ();
}

void QR1CTxAttOPModel::init()
{
    table = new QVector<tx_att_op_table_r1cd::data_f_t>;
    table->clear();
    iter = new QVector<QwtTxAttOPData *>[R1C_TX_ATT_OP_POWER_PTS];
    for (qint8 i = 0;i < R1C_TX_ATT_OP_POWER_PTS;i ++)
        iter[i].clear();
}

int QR1CTxAttOPModel::rowCount(const QModelIndex &parent) const
{
    return table->size() * R1C_TX_ATT_OP_POWER_PTS;
}

int QR1CTxAttOPModel::columnCount(const QModelIndex &parent) const
{
    return 13;
}

QVariant QR1CTxAttOPModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (Qt::DisplayRole == role && Qt::Horizontal == orientation) {
        QString strHeader[13] = {
            tr("Freq(MHz)"),
            tr("Power(dBm)"),
            tr("Att0"),
            tr("Att1"),
            tr("Att2"),
            tr("Att3"),
            tr("DGain"),
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

QVariant QR1CTxAttOPModel::data(const QModelIndex &index, int role) const
{
    if (Qt::DisplayRole == role) {
        int freqIdx = index.row() / R1C_TX_ATT_OP_POWER_PTS;
        int attIdx  = index.row() % R1C_TX_ATT_OP_POWER_PTS;

        if (0 == index.column())
            return QString("%1").arg(table->at(freqIdx).freq / 1000000);
        else if (7 == index.column())
            return QString("%1").arg(table->at(freqIdx).offset[attIdx],0,'f',3);
        else if (8 == index.column())
            return QString("%1").arg(table->at(freqIdx).temp[0]);
        else if (9 == index.column())
            return QString("%1").arg(table->at(freqIdx).temp[1]);
        else if (10 == index.column())
            return QString("%1").arg(table->at(freqIdx).temp[2]);
        else if (11 == index.column())
            return QString("%1").arg(table->at(freqIdx).temp[3]);
        else if (12 == index.column())
            return tm2QString(table->at(freqIdx).time);
        else {
            int pwr = attIdx < (R1C_TX_ATT_OP_POWER_STAR - R1C_TX_BASE_POWER_OP) ? (R1C_TX_ATT_OP_POWER_STAR - attIdx) : (R1C_TX_ATT_OP_POWER_STAR - attIdx - 1);
            tx_pwr_op_table_r1cd::data_m_t data;

            _sp1401->get_cal_file()->m_tx_pwr_op->get(table->at(freqIdx).freq,double(pwr),&data);
            if (1 == index.column())
                return QString("%1").arg(pwr);
            else if (2 == index.column())
                return QString("%1").arg(data.att0 / 2.0);
            else if (3 == index.column())
                return QString("%1").arg(data.att1 / 2.0);
            else if (4 == index.column())
                return QString("%1").arg(data.att2 / 2.0);
            else if (5 == index.column())
                return QString("%1").arg(data.att3 / 2.0);
            else if (6 == index.column())
                return QString("%1").arg(data.d_gain);
        }
    }
    else if (Qt::BackgroundColorRole == role)
        return CLR_PROTECT_EYE;
    return QVariant();
}

QVector<tx_att_op_table_r1cd::data_f_t> *QR1CTxAttOPModel::calTable()
{
    return table;
}

QVector<QwtTxAttOPData *> *QR1CTxAttOPModel::iterTable(int idx)
{
    return &iter[idx];
}

void QR1CTxAttOPModel::resetData()
{
    QVector<QwtTxAttOPData *>::const_iterator ptrIter;
    for (qint8 i = 0;i < R1C_TX_ATT_OP_POWER_PTS;i ++) {
        for (ptrIter = iter[i].constBegin();ptrIter != iter[i].constEnd();ptrIter ++)
            delete *ptrIter;
        iter[i].clear();
    }
    table->clear();
}

void QR1CTxAttOPModel::setSP1401(sp1401_r1c *sp1401)
{
    _sp1401 = sp1401;
}

// QR1CTxAttIOModel
QR1CTxAttIOModel::QR1CTxAttIOModel(QObject *parent) :
    QCalBaseModel(parent)
{
    init();
}

QR1CTxAttIOModel::QR1CTxAttIOModel(sp1401_r1c *sp1401, QObject *parent)
    : QCalBaseModel(parent)
    , _sp1401(sp1401)
{
    init();
}

void QR1CTxAttIOModel::init()
{
    table = new QVector<tx_att_io_table_r1cd::data_f_t>;
    table->clear();
    iter = new QVector<QwtTxAttIOData *>[R1C_TX_ATT_IO_POWER_PTS];
    for (qint8 i = 0;i < R1C_TX_ATT_IO_POWER_PTS;i ++)
        iter[i].clear();
}

int QR1CTxAttIOModel::rowCount(const QModelIndex &parent) const
{
    return table->size() * R1C_TX_ATT_IO_POWER_PTS;
}

int QR1CTxAttIOModel::columnCount(const QModelIndex &parent) const
{
    return 13;
}

QVariant QR1CTxAttIOModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (Qt::DisplayRole == role && Qt::Horizontal == orientation) {
        QString strHeader[13] = {
            tr("Freq(MHz)"),
            tr("Power(dBm)"),
            tr("Att0"),
            tr("Att1"),
            tr("Att2"),
            tr("Att3"),
            tr("DGain"),
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

QVariant QR1CTxAttIOModel::data(const QModelIndex &index, int role) const
{
    if (Qt::DisplayRole == role) {
        int freqIdx = index.row() / R1C_TX_ATT_IO_POWER_PTS;
        int attIdx = index.row() % R1C_TX_ATT_IO_POWER_PTS;

        if (0 == index.column())
            return QString("%1").arg(table->at(freqIdx).freq / 1000000);
        else if (7 == index.column())
            return QString("%1").arg(table->at(freqIdx).offset[attIdx],0,'f',3);
        else if (8 == index.column())
            return QString("%1").arg(table->at(freqIdx).temp[0]);
        else if (9 == index.column())
            return QString("%1").arg(table->at(freqIdx).temp[1]);
        else if (10 == index.column())
            return QString("%1").arg(table->at(freqIdx).temp[2]);
        else if (11 == index.column())
            return QString("%1").arg(table->at(freqIdx).temp[3]);
        else if (12 == index.column())
            return tm2QString(table->at(freqIdx).time);
        else {
            int pwr = attIdx < (R1C_TX_ATT_IO_POWER_STAR - R1C_TX_BASE_POWER_IO) ? (R1C_TX_ATT_IO_POWER_STAR - attIdx) : (R1C_TX_ATT_IO_POWER_STAR - attIdx - 1);
            tx_pwr_io_table_r1cd::data_m_t data;

            _sp1401->get_cal_file()->m_tx_pwr_io->get(table->at(freqIdx).freq,double(pwr),&data);
            if (1 == index.column())
                return QString("%1").arg(pwr);
            else if (2 == index.column())
                return QString("%1").arg(data.att0 / 2.0);
            else if (3 == index.column())
                return QString("%1").arg(data.att1 / 2.0);
            else if (4 == index.column())
                return QString("%1").arg(data.att2 / 2.0);
            else if (5 == index.column())
                return QString("%1").arg(data.att3 / 2.0);
            else if (6 == index.column())
                return QString("%1").arg(data.d_gain);
        }
    }
    else if (Qt::BackgroundColorRole == role)
        return CLR_PROTECT_EYE;
    return QVariant();
}

QVector<tx_att_io_table_r1cd::data_f_t> *QR1CTxAttIOModel::calTable()
{
    return table;
}

QVector<QwtTxAttIOData *> *QR1CTxAttIOModel::iterTable(int idx)
{
    return &iter[idx];
}

void QR1CTxAttIOModel::resetData()
{
    QVector<QwtTxAttIOData *>::const_iterator ptrIter;
    for (qint8 i = 0;i < R1C_TX_ATT_IO_POWER_PTS;i ++) {
        for (ptrIter = iter[i].constBegin();ptrIter != iter[i].constEnd();ptrIter ++)
            delete *ptrIter;
        iter[i].clear();
    }
    table->clear();
}

void QR1CTxAttIOModel::setSP1401(sp1401_r1c *sp1401)
{
    _sp1401 = sp1401;
}
