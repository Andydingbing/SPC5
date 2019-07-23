#ifndef Q_LED_H
#define Q_LED_H

#include <QFrame>

class QLed : public QFrame
{
    Q_OBJECT
public:
    explicit QLed(QWidget *parent = nullptr);

public:
    void setColor(QColor clr);
};

#endif // Q_LED_H
