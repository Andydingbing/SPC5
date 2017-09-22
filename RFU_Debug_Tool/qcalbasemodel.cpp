#include "qcalbasemodel.h"

QCalBaseModel::QCalBaseModel(QObject *parent) : QAbstractTableModel(parent)
{

}

void QCalBaseModel::update(const QModelIndex topleft, const QModelIndex rightbottom)
{
    emit layoutChanged();
}
