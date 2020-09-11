#include "qtestbasedlg.h"

QTestBaseDlg::QTestBaseDlg(QWidget *parent) :
    QAttachThreadDlg(),
    _model(new QVector<QTestBaseModel *>)
{
    _model->clear();
}

void QTestBaseDlg::init()
{
    QVector<QTestBaseModel *>::const_iterator iter;
    for (iter = _model->constBegin();iter != _model->constEnd();iter ++) {
        if (*iter)
            connect(this,SIGNAL(reset()),*iter,SLOT(reset()));
    }
}
