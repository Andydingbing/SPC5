#ifndef QCALBASEMODEL_H
#define QCALBASEMODEL_H

#include <QAbstractTableModel>
#include "define.h"
#include "qwt_series_data.h"

#define DECLARE_CAL_TABLE_MODEL(class_name)                             \
public:                                                                 \
    explicit class_name(QObject *parent = 0);                           \
    int rowCount(const QModelIndex &parent) const override;             \
    int columnCount(const QModelIndex &parent) const override;          \
    QVariant data(const QModelIndex &index, int role) const override;   \
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

#define DECLARE_CAL_QWT_DATA(class_name)        \
public:                                         \
    explicit class_name();                      \
    virtual size_t size() const;                \
    virtual QPointF sample(size_t i) const;

class QCalBaseModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    QCalBaseModel(QObject *parent = 0);

public slots:
    void reset();
    void update(const QModelIndex &tl, const QModelIndex &br, cal_file::cal_item_t item, int sec);
};


class QwtCalData : public QwtSeriesData<QPointF>
{
public:
    QwtCalData();
    ~QwtCalData();

public:
    virtual QRectF boundingRect() const;
};

#endif // QCALBASEMODEL_H
