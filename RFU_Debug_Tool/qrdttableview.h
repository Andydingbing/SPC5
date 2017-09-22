#ifndef QRDTTABLEVIEW_H
#define QRDTTABLEVIEW_H

#include <QHeaderView>
#include <QTableView>

class QRDTTableView : public QTableView
{
    Q_OBJECT

public:
    explicit QRDTTableView(QWidget *parent = Q_NULLPTR);
};

#endif // QRDTTABLEVIEW_H
