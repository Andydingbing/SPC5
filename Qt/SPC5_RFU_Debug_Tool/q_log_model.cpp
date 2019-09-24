#include "q_log_model.h"
#include "define.h"

int QMsgLogModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return int(Log.msgs()->size());
}

QVariant QMsgLogModel::data(const QModelIndex &index, int role) const
{
    quint32 row = quint32(index.row());
    if (role == Qt::DisplayRole) {
        if (index.column() == 0)
            return QString::fromStdString(Log.msgs()->at(row).time);
        else if (index.column() == 1)
            return QString::fromStdString(Log.msgs()->at(row).msg);
        else if (index.column() == 2)
            return Log.msgs()->at(row).result ? "x" : "-";
        else if (index.column() == 3)
            return QString("0x") +
                   QString("%1").arg(Log.msgs()->at(row).result,16,16,QLatin1Char('0'))
                                .toUpper()
                                .right(8);
    } else if (role == Qt::BackgroundColorRole) {
        return Log.msgs()->at(row).result ? CLR_ERROR : CLR_PROTECT_EYE;
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

void QMsgLogModel::update(int row)
{
    beginInsertRows(QModelIndex(),row,row);
    insertRow(row);
    endInsertRows();
    emit dataChanged(this->index(row,0),this->index(row,3));
}


int QRegLogModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return int(Log.regs()->size());
}

QVariant QRegLogModel::data(const QModelIndex &index, int role) const
{
    quint32 row = quint32(index.row());
    if (role == Qt::DisplayRole) {
        if (index.column() == 0)
            return QString::fromStdString(Log.regs()->at(row).time);
        else if (index.column() == 1)
            return QString::fromStdString(Log.regs()->at(row).fpga);
        else if (index.column() == 2)
            return QString("0x") + QString("%1").arg(Log.regs()->at(row).addr,4,16,QLatin1Char('0')).toUpper();
        else if (index.column() == 3 && Log.regs()->at(row).w != 0xffffffff)
            return QString("0x") + QString("%1").arg(Log.regs()->at(row).w,8,16,QLatin1Char('0')).toUpper();
        else if (index.column() == 4 && Log.regs()->at(row).r != 0xffffffff)
            return QString("0x") + QString("%1").arg(Log.regs()->at(row).r,8,16,QLatin1Char('0')).toUpper();
        else if (index.column() == 5)
            return Log.regs()->at(row).result ? QString("x") : QString("-");
    } else if (role == Qt::BackgroundColorRole) {
        return Log.regs()->at(row).result ? CLR_ERROR : CLR_PROTECT_EYE;
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

void QRegLogModel::update(int row)
{
    beginInsertRows(QModelIndex(),row,row);
    insertRow(row);
    endInsertRows();
}

