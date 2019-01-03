#ifndef QR1CTXSBMODEL_H
#define QR1CTXSBMODEL_H

#include "qcalbasemodel.h"
#include "define.h"

class QR1CTxSBModel : public QCalBaseModel
{
    Q_OBJECT
    DECLARE_CAL_TABLE_MODEL(QR1CTxSBModel)

public:
    QVector<tx_sb_table_r1cd::data_f_t> *calTable() const;

private:
    QVector<tx_sb_table_r1cd::data_f_t> *table;
};

#endif // QR1CTXSBMODEL_H
