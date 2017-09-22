#ifndef QCALBASEMODEL_H
#define QCALBASEMODEL_H

#include <QAbstractTableModel>

class QCalBaseModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    QCalBaseModel(QObject *parent = Q_NULLPTR);
public slots:
    void update(const QModelIndex topleft,const QModelIndex rightbottom);
};

#endif // QCALBASEMODEL_H
