#ifndef QFRAMESLOTTRIGWIDGET_H
#define QFRAMESLOTTRIGWIDGET_H

#include <QWidget>
#include "define.h"

namespace Ui {
class QFrameSlotTrigWidget;
}

class QFrameSlotTrigWidget : public QWidget
{
    Q_OBJECT

    friend class QArbDlg;
    friend class QIQCapDlg;

public:
    explicit QFrameSlotTrigWidget(QWidget *parent = 0);
    ~QFrameSlotTrigWidget();

public:
    void setAll(sp2401_r1a *sp2401);

private:
    Ui::QFrameSlotTrigWidget *ui;
};

#endif // QFRAMESLOTTRIGWIDGET_H
