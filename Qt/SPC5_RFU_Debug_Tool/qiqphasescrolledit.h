#ifndef QIQPHASESCROLLEDIT_H
#define QIQPHASESCROLLEDIT_H

#include <QLineEdit>

class QWheelEvent;

class QIqPhaseScrollEdit : public QLineEdit
{
    Q_OBJECT

public:
    explicit QIqPhaseScrollEdit(QWidget *parent = 0);

protected:
    void wheelEvent(QWheelEvent *event);
};

#endif // QIQPHASESCROLLEDIT_H
