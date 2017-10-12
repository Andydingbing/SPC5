#ifndef QR1CTXLOLEAKMODEL_H
#define QR1CTXLOLEAKMODEL_H

#include <qcalbasemodel.h>
#include "define.h"

class QR1CTxLOLeakModel : public QCalBaseModel
{
    Q_OBJECT

public:
    explicit QR1CTxLOLeakModel(QObject *parent = Q_NULLPTR);
    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
public:
    vector<TxLOLeakageTableR1C::DataF> *m_pData;
};

#endif // QR1CTXLOLEAKMODEL_H
