#ifndef Q_RDT_TABLE_VIEW_H
#define Q_RDT_TABLE_VIEW_H

#include <QHeaderView>
#include <QTableView>

class QRDTTableView : public QTableView
{
    Q_OBJECT

public:
    explicit QRDTTableView(QWidget *parent = nullptr) :
        QTableView(parent)
    {
        horizontalHeader()->setHighlightSections(false);
        verticalHeader()->setDefaultSectionSize(20);
        verticalHeader()->setVisible(false);
        setSelectionBehavior(QAbstractItemView::SelectRows);
        setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
        setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
        setStyleSheet("QTableView{selection-background-color:rgb(0,162,232);selection-color:white}");
    }

//protected:
//    void resizeEvent(QResizeEvent *event)
//    {
//        if (event->type() == QEvent::Resize)
//            scrollToBottom();

//        QWidget::resizeEvent(event);
//    }
};

#endif // Q_RDT_TABLE_VIEW_H
