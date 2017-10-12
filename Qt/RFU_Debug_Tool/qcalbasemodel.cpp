#include "qcalbasemodel.h"

QCalBaseModel::QCalBaseModel(QObject *parent) : QAbstractTableModel(parent)
{

}

void QCalBaseModel::reset(const QModelIndex &tl, const QModelIndex &br)
{
    emit layoutChanged();
}

void QCalBaseModel::update(const QModelIndex &tl, const QModelIndex &br)
{
    emit dataChanged(tl,br);
}

