#include "q_r1c_temp_ctrl_model.h"
#include <QPointF>

// QwtTxAttOPData
QwtTempCtrlData::QwtTempCtrlData() :
    QwtCalData()
{
    table = new QVector<tempdata_t>;
    table->clear();

}

size_t QwtTempCtrlData::size() const
{
    return table->size();
}

QPointF QwtTempCtrlData::sample(size_t i) const
{
    return QPointF(table->at(i).i,table->at(i).temp[0]);
}
//QRectF QwtTempCtrlData::boundingRect() const
//{
//    if ( d_boundingRect.width() < 0 )
//        d_boundingRect = qwtBoundingRect( *this );

//    return d_boundingRect;
//}

//void QwtTempCtrlData::locate2CalTable(start_iter iter)
//{
//    this->iter = iter;
//}

//void QwtTempCtrlData::addOneData()
//{
//    n += 1;
//}

QVector<tempdata_t> *QwtTempCtrlData::calTable() const
{
    return table;
}



QR1CTempCtrlModel::QR1CTempCtrlModel(QObject *parent) : QCalBaseModel(parent)
{
    table = new QVector<tempdata_t>;
    table->clear();
}


QVector<tempdata_t> *QR1CTempCtrlModel::calTable() const
{
    return table;
}

