#include "qcalbasedlg.h"

QCalBaseDlg::QCalBaseDlg(QWidget *parent) :
    QAttachThreadDlg(parent),
    _model(new QVector<QCalBaseModel *>)
{
    _model->clear();
}

void QCalBaseDlg::init()
{
    QVector<QCalBaseModel *>::const_iterator iter;
    for (iter = _model->constBegin();iter != _model->constEnd();iter ++) {
        if (*iter)
            connect(this,SIGNAL(reset()),*iter,SLOT(reset()));
    }
}
