#ifndef QRDTTABLEVIEW_H
#define QRDTTABLEVIEW_H

#include <QHeaderView>
#include <QTableView>

// RFU-Debug-Tool/RF-Driver-Tool TableView
class QResizeEvent;

class QRDTTableView : public QTableView
{
    Q_OBJECT

public:
    explicit QRDTTableView(QWidget *parent = 0);

//protected:
//    void resizeEvent(QResizeEvent *event);
};

#endif // QRDTTABLEVIEW_H
