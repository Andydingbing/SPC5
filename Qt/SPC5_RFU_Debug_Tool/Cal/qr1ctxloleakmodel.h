#ifndef QR1CTXLOLEAKMODEL_H
#define QR1CTXLOLEAKMODEL_H

#include "qcalbasemodel.h"
#include "define.h"

class QR1CTxLOLeakModel : public QCalBaseModel
{
    Q_OBJECT
    DECLARE_CAL_TABLE_MODEL(QR1CTxLOLeakModel)

public:
    QVector<tx_lol_table_r1cd::data_f_t> *calTable() const;

private:
    QVector<tx_lol_table_r1cd::data_f_t> *table;
};

#endif // QR1CTXLOLEAKMODEL_H
