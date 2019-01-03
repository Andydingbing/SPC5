#ifndef QDCOFFSETSCROLLEDIT_H
#define QDCOFFSETSCROLLEDIT_H

#include <QLineEdit>

class QWheelEvent;

class QDcOffsetScrollEdit : public QLineEdit
{
    Q_OBJECT

public:
    explicit QDcOffsetScrollEdit(QWidget *parent = 0);

protected:
    void wheelEvent(QWheelEvent *event);
};

#endif // QDCOFFSETSCROLLEDIT_H
