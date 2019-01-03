#include "qrdttableview.h"
#include <QResizeEvent>

QRDTTableView::QRDTTableView(QWidget *parent) : QTableView(parent)
{
    this->verticalHeader()->setDefaultSectionSize(20);
    this->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->setStyleSheet("QTableView{selection-background-color:rgb(0,162,232);selection-color:white}");
}

//void QRDTTableView::resizeEvent(QResizeEvent *event)
//{
//    if (event->type() == QEvent::Resize)
//        scrollToBottom();

//    QWidget::resizeEvent(event);
//}
