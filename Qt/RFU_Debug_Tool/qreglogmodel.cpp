#include "qreglogmodel.h"
#include "define.h"

QRegLogModel::QRegLogModel(QObject *parent) : QAbstractTableModel(parent)
{

}

int QRegLogModel::rowCount(const QModelIndex &parent) const
{
    return Log->GetRegLog()->size();
}

int QRegLogModel::columnCount(const QModelIndex &parent) const
{
    return 6;
}

QVariant QRegLogModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole) {
        if (0 == index.column())
            return QString("%1").arg(Log->GetRegLog()->at(index.row()).m_szTime);
        else if (1 == index.column())
            return QString("%1").arg(Log->GetRegLog()->at(index.row()).m_szFpga);
        else if (2 == index.column())
            return QString("0x") +
                   QString("%1").arg(Log->GetRegLog()->at(index.row()).m_iAddr,4,16,QLatin1Char('0')).toUpper();
        else if (3 == index.column() && 0xffffffff != Log->GetRegLog()->at(index.row()).m_uiWValue)
            return QString("0x") +
                   QString("%1").arg(Log->GetRegLog()->at(index.row()).m_uiWValue,8,16,QLatin1Char('0')).toUpper();
        else if (4 == index.column() && 0xffffffff != Log->GetRegLog()->at(index.row()).m_uiRValue)
            return QString("0x") +
                   QString("%1").arg(Log->GetRegLog()->at(index.row()).m_uiRValue,8,16,QLatin1Char('0')).toUpper();
        else if (5 == index.column())
            return Log->GetRegLog()->at(index.row()).m_iResult ? "x" : "√";
    }
    if (role == Qt::BackgroundColorRole) {
        if (Log->GetRegLog()->at(index.row()).m_iResult)
            return CLR_ERROR;
        else
            return CLR_PROTECT_EYE;
    }
    return QVariant();
}

QVariant QRegLogModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && Qt::Horizontal == orientation) {
        QString strHeader[6] = {tr("Time"),
                                tr("FPGA"),
                                tr("Addr"),
                                tr("Write"),
                                tr("Read"),
                                tr("Result")};
        return strHeader[section];
    }
    return QVariant();
}

void QRegLogModel::update(int iRow)
{
    emit layoutChanged();
}

