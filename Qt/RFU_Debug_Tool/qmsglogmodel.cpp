#include "qmsglogmodel.h"
#include "define.h"

QMsgLogModel::QMsgLogModel(QObject *parent) : QAbstractTableModel(parent)
{

}

int QMsgLogModel::rowCount(const QModelIndex &parent) const
{
    return Log.GetMsgLog()->size();
}

int QMsgLogModel::columnCount(const QModelIndex &parent) const
{
    return 4;
}

QVariant QMsgLogModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole) {
        if (0 == index.column())
            return QString("%1").arg(Log.GetMsgLog()->at(index.row()).m_szTime);
        else if (1 == index.column())
            return QString("%1").arg(Log.GetMsgLog()->at(index.row()).m_szMsg);
        else if (2 == index.column())
            return Log.GetMsgLog()->at(index.row()).m_iResult ? "x" : "√";
        else if (3 == index.column())
            return QString("0x") +
                   QString("%1").arg(Log.GetMsgLog()->at(index.row()).m_iResult,16,16,QLatin1Char('0'))
                                .toUpper()
                                .right(8);
    }
    if (role == Qt::BackgroundColorRole) {
        if (Log.GetMsgLog()->at(index.row()).m_iResult)
            return CLR_ERROR;
        else
            return CLR_PROTECT_EYE;
    }
    return QVariant();
}

QVariant QMsgLogModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && Qt::Horizontal == orientation) {
        QString strHeader[4] = {tr("Time"),
                                tr("Message"),
                                tr("Result"),
                                tr("Errno")};
        return strHeader[section];
    }
    return QVariant();
}

void QMsgLogModel::update(int iRow)
{
    emit layoutChanged();
}
