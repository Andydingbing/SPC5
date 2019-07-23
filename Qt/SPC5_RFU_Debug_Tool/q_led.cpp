#include "q_led.h"

QLed::QLed(QWidget *parent) : QFrame(parent)
{

}

void QLed::setColor(QColor clr)
{
    setStyleSheet(QString("background:rgb(%1,%2,%3)")
                  .arg(clr.red())
                  .arg(clr.green())
                  .arg(clr.black()));
}
