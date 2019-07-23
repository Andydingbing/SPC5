#ifndef Q_DUC_SCROLL_EDIT_H
#define Q_DUC_SCROLL_EDIT_H

#include <QLineEdit>
#include <QWheelEvent>
#include "freq_string.hpp"

inline int wheelStep(QWheelEvent *event)
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

    return step;
}

template <int64_t Div_Mag,int64_t Default_Mag,int64_t Ctrl_Mag,int64_t Shift_Mag,
          int64_t UpperLimit,int64_t LowerLimit>
class QScrollLineEdit : public QLineEdit
{
public:
    explicit QScrollLineEdit(QWidget *parent = nullptr) :
        QLineEdit(parent) {}

protected:
    virtual void wheelEvent(QWheelEvent *event)
    {
        int step = wheelStep(event);
        int64_t mag = Default_Mag;

        if (event->modifiers() == Qt::ControlModifier) {
            mag = Ctrl_Mag;
        } else if (event->modifiers() == Qt::ShiftModifier) {
            mag = Shift_Mag;
        }

        double newValue = text().toDouble() + step * double(mag) / double(Div_Mag);
        newValue = newValue > double(UpperLimit) ? double(UpperLimit) : newValue;
        newValue = newValue < double(LowerLimit) ? double(LowerLimit) : newValue;

        setText(QString("%1").arg(newValue));

        event->accept();
    }
};

template <int64_t Default_Mag,int64_t Ctrl_Mag,int64_t Shift_Mag,
          int64_t UpperLimit,int64_t LowerLimit>
class QFreqScrollEdit : public QLineEdit
{
public:
    explicit QFreqScrollEdit(QWidget *parent = nullptr) :
        QLineEdit(parent) {}

protected:
    void wheelEvent(QWheelEvent *event)
    {
        int step = wheelStep(event);
        int64_t mag = Default_Mag;

        if (event->modifiers() == Qt::ControlModifier) {
            mag = Ctrl_Mag;
        } else if (event->modifiers() == Qt::ShiftModifier) {
            mag = Shift_Mag;
        }

        double newValue = freq_string_to_double(text().toStdString()) + step * double(mag);
        newValue = newValue > double(UpperLimit) ? double(UpperLimit) : newValue;
        newValue = newValue < double(LowerLimit) ? double(LowerLimit) : newValue;

        setText(QString::fromStdString(freq_string_from<double>(newValue)));

        event->accept();
    }
};

typedef QScrollLineEdit<1,1,5,10,100,-100> QDUCScrollEdit;
typedef QScrollLineEdit<1,1,5,10,100,-100> QDDS1ScrollEdit;
typedef QScrollLineEdit<1,1,5,10,100,-100> QDDS2ScrollEdit;
typedef QScrollLineEdit<1,1,5,10,8192,-8192> QDCOffsetScrollEdit;
typedef QScrollLineEdit<1,1,5,10,16384,-16384> QIQAmpScrollEdit;
typedef QScrollLineEdit<10,1,5,10,20,-20> QIQPhaseScrollEdit;

typedef QFreqScrollEdit<1000000,5000000,10000000,6200000000,10000000> QRFFreqScrollEdit;

#endif // Q_DUC_SCROLL_EDIT_H
