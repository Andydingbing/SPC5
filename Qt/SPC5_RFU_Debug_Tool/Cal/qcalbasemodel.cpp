#include "qcalbasemodel.h"

QCalBaseModel::QCalBaseModel(QObject *parent) : QAbstractTableModel(parent)
{

}

void QCalBaseModel::reset()
{
//    emit layoutChanged();
    beginResetModel();
    endResetModel();
}

void QCalBaseModel::update(const QModelIndex &tl, const QModelIndex &br, cal_file::cal_item_t item, int sec)
{
    emit dataChanged(tl,br,QVector<int>(Qt::DisplayRole));
}


QwtCalData::QwtCalData() :
    QwtSeriesData<QPointF>()
{

}

QwtCalData::~QwtCalData()
{

}

QRectF QwtCalData::boundingRect() const
{
    if ( d_boundingRect.width() < 0 )
        d_boundingRect = qwtBoundingRect( *this );

    return d_boundingRect;
}
