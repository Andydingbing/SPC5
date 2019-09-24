#ifndef Q_LOG_MODEL_H
#define Q_LOG_MODEL_H

#include <QAbstractTableModel>

class QMsgLogModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit QMsgLogModel(QObject *parent = nullptr) : QAbstractTableModel(parent) {}
    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override { Q_UNUSED(parent); return 4; }
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

public slots:
    void update(int row);
};

class QRegLogModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit QRegLogModel(QObject *parent = nullptr) : QAbstractTableModel(parent) {}
    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override { Q_UNUSED(parent); return 6; }
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

public slots:
    void update(int row);
};

#endif // Q_LOG_MODEL_H
