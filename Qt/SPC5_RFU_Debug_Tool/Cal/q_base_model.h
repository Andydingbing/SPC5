#ifndef Q_CAL_BASE_MODEL_H
#define Q_CAL_BASE_MODEL_H

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


typedef class QCalBaseModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    QCalBaseModel(QObject *parent = nullptr) :
        QAbstractTableModel(parent) {}

public slots:
    void reset()
    {
        beginResetModel();
        endResetModel();
    }

    void update(const QModelIndex &tl,
                const QModelIndex &br,
                cal_file::cal_item_t item,
                int sec)
    {
        Q_UNUSED(item);
        Q_UNUSED(sec);
        emit dataChanged(tl,br,QVector<int>(Qt::DisplayRole));
    }
} QCalBaseMode, QTestBaseModel;


#define DECL_QWT_CAL_DATA(class_name) \
public: \
    explicit class_name(); \
    virtual size_t size() const; \
    virtual QPointF sample(size_t i) const;

typedef class QwtCalData : public QwtSeriesData<QPointF>
{
public:
    QwtCalData() :
        QwtSeriesData<QPointF>() {}
    ~QwtCalData() {}

public:
    virtual QRectF boundingRect() const
    {
        if (d_boundingRect.width() < 0) {
            d_boundingRect = qwtBoundingRect(*this);
        }
        return d_boundingRect;
    }
} QwtCalData, QwtTestData;


template <typename Data_Type>
class QwtFreqResponseData : public QwtCalData
{
public:
    QwtFreqResponseData() :
        QwtCalData(),
        table(new QVector<Data_Type>)
    { table->clear(); }

    size_t size() const
    { return table->size(); }

    QPointF sample(size_t i) const
    { return QPointF(table->at(i).freq / 1e6,table->at(i).pwr); }

    QVector<Data_Type> *calTable() const
    { return table; }

private:
    QVector<Data_Type> *table;
};

#endif // Q_CAL_BASE_MODEL_H
