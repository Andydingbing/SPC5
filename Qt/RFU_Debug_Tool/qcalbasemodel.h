#ifndef QCALBASEMODEL_H
#define QCALBASEMODEL_H

#include <QAbstractTableModel>

class QCalBaseModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    QCalBaseModel(QObject *parent = Q_NULLPTR);
public slots:
    void reset(const QModelIndex &tl, const QModelIndex &br);
    void update(const QModelIndex &tl, const QModelIndex &br);
};

#endif // QCALBASEMODEL_H
