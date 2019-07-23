#include "q_rdt_tableview.h"
#include <QResizeEvent>

QRDTTableView::QRDTTableView(QWidget *parent) : QTableView(parent)
{
    this->horizontalHeader()->setHighlightSections(false);
    this->verticalHeader()->setDefaultSectionSize(20);
    this->verticalHeader()->setVisible(false);
    this->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    this->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    this->setStyleSheet("QTableView{selection-background-color:rgb(0,162,232);selection-color:white}");
}

//void QRDTTableView::resizeEvent(QResizeEvent *event)
//{
//    if (event->type() == QEvent::Resize)
//        scrollToBottom();

//    QWidget::resizeEvent(event);
//}
