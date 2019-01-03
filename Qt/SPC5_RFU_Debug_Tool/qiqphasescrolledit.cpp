#include "qiqphasescrolledit.h"
#include <QWheelEvent>
#include <QtGlobal>

QIqPhaseScrollEdit::QIqPhaseScrollEdit(QWidget *parent) :
    QLineEdit(parent)
{
    this->setToolTip(tr("Step:: Normal:0.1 ; Ctrl:0.2 ; Shift:0.5"));
}

void QIqPhaseScrollEdit::wheelEvent(QWheelEvent *event)
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
        mag = 2;
    if (Qt::ShiftModifier == event->modifiers())
        mag = 5;

    double th = this->text().toDouble() + 0.1 * step * mag;

    th = th > 20.0  ? 20.0  : th;
    th = th < -20.0 ? -20.0 : th;
    this->setText(QString("%1").arg(th));

    event->accept();
}
