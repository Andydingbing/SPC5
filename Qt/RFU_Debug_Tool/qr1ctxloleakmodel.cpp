#include "qr1ctxloleakmodel.h"

QR1CTxLOLeakModel::QR1CTxLOLeakModel(QObject *parent) : QCalBaseModel(parent)
{
    m_pData = new vector<TxLOLeakageTableR1C::DataF>;
    m_pData->clear();
}

int QR1CTxLOLeakModel::rowCount(const QModelIndex &parent) const
{
    return m_pData->size();
}

int QR1CTxLOLeakModel::columnCount(const QModelIndex &parent) const
{
    return 10;
}

QVariant QR1CTxLOLeakModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole) {
        if (0 == index.column())
            return QString("%1").arg(m_pData->at(index.row()).m_uiFreq / 1000000);
        else if (1 == index.column())
            return QString("%1").arg(m_pData->at(index.row()).m_iDCI);
        else if (2 == index.column())
            return QString("%1").arg(m_pData->at(index.row()).m_iDCQ);
        else if (3 == index.column())
            return m_pData->at(index.row()).m_bUseSA ? tr("Spectrum") : tr("Loopback");
        else if (4 == index.column())
            return QString("%1").arg(m_pData->at(index.row()).m_dPower);
        else if (5 == index.column())
            return QString("%1").arg(m_pData->at(index.row()).m_dTemp[0]);
        else if (6 == index.column())
            return QString("%1").arg(m_pData->at(index.row()).m_dTemp[1]);
        else if (7 == index.column())
            return QString("%1").arg(m_pData->at(index.row()).m_dTemp[2]);
        else if (8 == index.column())
            return QString("%1").arg(m_pData->at(index.row()).m_dTemp[3]);
        else if (9 == index.column())
            return tm2QString(m_pData->at(index.row()).m_EndTime);
    }
    if (role == Qt::BackgroundColorRole)
        return CLR_PROTECT_EYE;
    return QVariant();
}

QVariant QR1CTxLOLeakModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && Qt::Horizontal == orientation) {
        QString strHeader[10] = {tr("Freq(MHz)"),
                                 tr("DC Offset I"),
                                 tr("DC Offset Q"),
                                 tr("Method"),
                                 tr("LO Leakage(dBm)"),
                                 tr("Temp4_TxLO2"),
                                 tr("Temp5_TxLO1"),
                                 tr("Temp6_TxPA4"),
                                 tr("Temp7_TxPA3"),
                                 tr("End Time")};
        return strHeader[section];
    }
    return QVariant();
}
