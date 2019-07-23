#ifndef QR1CTEMPCTRLMODEL_H
#define QR1CTEMPCTRLMODEL_H

#include <q_base_model.h>
#include "define.h"

typedef struct tempdata_t {
     double   temp[8];
     qint64   i;
     tm       time;
 public:
}tempdata_t;

class QwtTempCtrlData : public QwtCalData
{
    //typedef QVector<tempdata_t>::iterator start_iter;

public:
    explicit QwtTempCtrlData();
    virtual size_t size() const;
    virtual QPointF sample(size_t i) const;

    //    void locate2CalTable(int iter);
    //    void addOneData();
    QVector<tempdata_t> *calTable() const;

public:
    QVector<tempdata_t> *table;
};


class QR1CTempCtrlModel : public QCalBaseModel
{
    Q_OBJECT

public:
    explicit QR1CTempCtrlModel(QObject *parent = 0);

public:

    QVector<tempdata_t> *calTable() const;

public:
    QVector<tempdata_t> *table;


};



#endif // QR1CTXATTMODEL_H
