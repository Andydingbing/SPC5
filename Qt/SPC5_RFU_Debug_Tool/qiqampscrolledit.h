#ifndef QIQAMPSCROLLEDIT_H
#define QIQAMPSCROLLEDIT_H

#include <QLineEdit>

class QWheelEvent;

class QIqAmpScrollEdit : public QLineEdit
{
    Q_OBJECT

public:
    explicit QIqAmpScrollEdit(QWidget *parent = 0);

protected:
    void wheelEvent(QWheelEvent *event);
};

#endif // QIQAMPSCROLLEDIT_H
