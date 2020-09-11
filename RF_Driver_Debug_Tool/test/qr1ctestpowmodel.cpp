#include "qr1ctestpowmodel.h"
#include <QPointF>

// QwtTestPowOPData
QwtTestPowOPData::QwtTestPowOPData() :
    QwtTestData()
{
    n = 0;
}

QwtTestPowOPData::QwtTestPowOPData(int idx) :
    QwtTestData()
{
    attIdx = idx;
    n = 0;
}

size_t QwtTestPowOPData::size() const
{
    return n;
}

QPointF QwtTestPowOPData::sample(size_t i) const
{
    return QPointF((*(iter + i)).TargetPower,(*(iter + i)).PowOffset[attIdx][i]);
}

void QwtTestPowOPData::locate2CalTable(start_iter iter)
{
    this->iter = iter;
}

void QwtTestPowOPData::addOneData()
{
    n += 1;
}

// QwtTestPowIOData
QwtTestPowIOData::QwtTestPowIOData() :
    QwtTestData()
{
    n = 0;
}

QwtTestPowIOData::QwtTestPowIOData(int idx) :
    QwtTestData()
{
    attIdx = idx;
    n = 0;
}

size_t QwtTestPowIOData::size() const
{
    return n;
}

QPointF QwtTestPowIOData::sample(size_t i) const
{
    return QPointF((*(iter + i)).TargetPower,(*(iter + i)).PowOffset[attIdx][i]);
}

void QwtTestPowIOData::locate2CalTable(start_iter iter)
{
    this->iter = iter;
}

void QwtTestPowIOData::addOneData()
{
    n += 1;
}

// QR1CTestPowOPModel
QR1CTestPowOPModel::QR1CTestPowOPModel(QObject *parent) :
    QTestBaseModel(parent)
{
    init();
}

QR1CTestPowOPModel::QR1CTestPowOPModel(basic_sp1401 *sp1401, QObject *parent)
    : QTestBaseModel(parent)
    , _sp1401(sp1401)
{
    init ();
}

void QR1CTestPowOPModel::init()
{
    table = new QVector<data_f_t>;
    table->clear();
    iter = new QVector<QwtTestPowOPData *>[RF_TX_TEST_FREQ_PTS];
    for (qint8 i = 0;i < RF_TX_TEST_FREQ_PTS;i ++)
        iter[i].clear();
}

int QR1CTestPowOPModel::rowCount(const QModelIndex &parent) const
{
    return table->size() * RF_TX_TEST_FREQ_PTS;
}

int QR1CTestPowOPModel::columnCount(const QModelIndex &parent) const
{
    return 13;
}

QVariant QR1CTestPowOPModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (Qt::DisplayRole == role && Qt::Horizontal == orientation) {
        QString strHeader[13] = {
            tr("Freq(MHz)"),
            tr("Target Power(dBm)"),
            tr("Actual Power(dBm)"),
            tr("Offset"),
            tr("End Time")
        };
        return strHeader[section];
    }

    return QVariant();
}

QVariant QR1CTestPowOPModel::data(const QModelIndex &index, int role) const
{
    if (Qt::DisplayRole == role) {
//        int freqIdx = index.row() / R1C_TX_ATT_OP_POWER_PTS;
//        int attIdx  = index.row() % R1C_TX_ATT_OP_POWER_PTS;

//        if (0 == index.column())
//            return QString("%1").arg(table->at(freqIdx).freq / 1000000);
//        else if (7 == index.column())
//            return QString("%1").arg(table->at(freqIdx).offset[attIdx],0,'f',3);
//        else if (8 == index.column())
//            return QString("%1").arg(table->at(freqIdx).temp[0]);
//        else if (9 == index.column())
//            return QString("%1").arg(table->at(freqIdx).temp[1]);
//        else if (10 == index.column())
//            return QString("%1").arg(table->at(freqIdx).temp[2]);
//        else if (11 == index.column())
//            return QString("%1").arg(table->at(freqIdx).temp[3]);
//        else if (12 == index.column())
//            return tm2QString(table->at(freqIdx).time);
//        else {
//            int pwr = attIdx < (R1C_TX_ATT_OP_POWER_STAR - R1C_TX_BASE_POWER_OP) ? (R1C_TX_ATT_OP_POWER_STAR - attIdx) : (R1C_TX_ATT_OP_POWER_STAR - attIdx - 1);
//            tx_pwr_op_table_r1cd::data_m_t data;

//            _sp1401->cf()->m_tx_pwr_op->get(table->at(freqIdx).freq,double(pwr),&data);
//            if (1 == index.column())
//                return QString("%1").arg(pwr);
//            else if (2 == index.column())
//                return QString("%1").arg(data.att0 / 2.0);
//            else if (3 == index.column())
//                return QString("%1").arg(data.att1 / 2.0);
//            else if (4 == index.column())
//                return QString("%1").arg(data.att2 / 2.0);
//            else if (5 == index.column())
//                return QString("%1").arg(data.att3 / 2.0);
//            else if (6 == index.column())
//                return QString("%1").arg(data.d_gain);
//        }
    }
    else if (Qt::BackgroundColorRole == role)
        return CLR_PROTECT_EYE;
    return QVariant();
}

QVector<data_f_t> *QR1CTestPowOPModel::calTable()
{
    return table;
}

QVector<QwtTestPowOPData *> *QR1CTestPowOPModel::iterTable(int idx)
{
    return &iter[idx];
}

void QR1CTestPowOPModel::resetData()
{
    QVector<QwtTestPowOPData *>::const_iterator ptrIter;
    for (qint8 i = 0;i < RF_TX_TEST_FREQ_PTS;i ++) {
        for (ptrIter = iter[i].constBegin();ptrIter != iter[i].constEnd();ptrIter ++)
            delete *ptrIter;
        iter[i].clear();
    }
    table->clear();
}

void QR1CTestPowOPModel::setSP1401(basic_sp1401 *sp1401)
{
    _sp1401 = sp1401;
}

// QR1CTestPowIOModel
QR1CTestPowIOModel::QR1CTestPowIOModel(QObject *parent) :
    QTestBaseModel(parent)
{
    init();
}

QR1CTestPowIOModel::QR1CTestPowIOModel(basic_sp1401 *sp1401, QObject *parent)
    : QTestBaseModel(parent)
    , _sp1401(sp1401)
{
    init();
}

void QR1CTestPowIOModel::init()
{
    table = new QVector<data_f_t>;
    table->clear();
    iter = new QVector<QwtTestPowIOData *>[RF_TX_TEST_FREQ_PTS];
    for (qint8 i = 0;i < RF_TX_TEST_FREQ_PTS;i ++)
        iter[i].clear();
}

int QR1CTestPowIOModel::rowCount(const QModelIndex &parent) const
{
    return table->size() * RF_TX_TEST_FREQ_PTS;
}

int QR1CTestPowIOModel::columnCount(const QModelIndex &parent) const
{
    return 13;
}

QVariant QR1CTestPowIOModel::headerData(int section, Qt::Orientation orientation, int role) const
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

QVariant QR1CTestPowIOModel::data(const QModelIndex &index, int role) const
{
    if (Qt::DisplayRole == role) {/*
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

            _sp1401->cf()->m_tx_pwr_io->get(table->at(freqIdx).freq,double(pwr),&data);
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
        }*/
    }
    else if (Qt::BackgroundColorRole == role)
        return CLR_PROTECT_EYE;
    return QVariant();
}

QVector<data_f_t> *QR1CTestPowIOModel::calTable()
{
    return table;
}

QVector<QwtTestPowIOData *> *QR1CTestPowIOModel::iterTable(int idx)
{
    return &iter[idx];
}

void QR1CTestPowIOModel::resetData()
{
    QVector<QwtTestPowIOData *>::const_iterator ptrIter;
    for (qint8 i = 0;i < RF_TX_TEST_FREQ_PTS;i ++) {
        for (ptrIter = iter[i].constBegin();ptrIter != iter[i].constEnd();ptrIter ++)
            delete *ptrIter;
        iter[i].clear();
    }
    table->clear();
}

void QR1CTestPowIOModel::setSP1401(basic_sp1401 *sp1401)
{
    _sp1401 = sp1401;
}
