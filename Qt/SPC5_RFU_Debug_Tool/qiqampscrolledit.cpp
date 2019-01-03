#include "qiqampscrolledit.h"
#include <QWheelEvent>

QIqAmpScrollEdit::QIqAmpScrollEdit(QWidget *parent) :
    QLineEdit(parent)
{
    this->setToolTip(tr("Step:: Normal:1 ; Ctrl:5 ; Shift:10"));
}

void QIqAmpScrollEdit::wheelEvent(QWheelEvent *event)
{
	int step = 0;

#if (QT_VERSION >= QT_VERSION_CHECK(5,0,0))
	QPoint numSteps;
	QPoint numPixels = event->pixelDelta();
	QPoint numDegrees = event->angleDelta() / 8;
	if (!numPixels.isNull())
		numSteps = numPixels;
	else if (!numDegrees.isNull())
		numSteps = numDegrees / 15;
	step = numSteps.ry();
#else
	step = event->delta() / 8 / 15;
#endif

    int mag = 1;
    if (Qt::ControlModifier == event->modifiers())
        mag = 5;
    if (Qt::ShiftModifier == event->modifiers())
        mag = 10;

    int amp = this->text().toInt() + step * mag;
    amp = amp < 0 ? 0 : amp;
    this->setText(QString("%1").arg(amp));

    event->accept();
}
