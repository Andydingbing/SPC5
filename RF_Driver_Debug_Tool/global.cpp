#include "global.h"
#include <QWidget>
#include <QTableView>
#include <QHeaderView>

Q_Config_Table_View::Q_Config_Table_View(QWidget *parent) :
    QTableView(parent)
{
    setSelectionMode(QAbstractItemView::NoSelection);
    setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    horizontalHeader()->setHidden(true);
    verticalHeader()->setHidden(true);
    verticalHeader()->setDefaultSectionSize(30);
}

QVariant Q_Config_Table_Model::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole && index.column() == 0) {
        return _item.at(index.row());
    }
    return QVariant();
}

QWidget* Q_Config_Table_Delegate::createEditor(QWidget *parent, const QStyleOptionViewItem &/*option*/, const QModelIndex &index) const
{
    if (index.column() != 1) {
        return nullptr;
    }

    QWidget *widget = (QWidget *)(*(first() + index.row()));
    widget->setParent(parent);

    return widget;
}

void Q_Config_Table_Delegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &) const
{
    editor->setGeometry(option.rect);
}

QRectF Qwt_Data::boundingRect() const
{
    if (d_boundingRect.width() < 0) {
        d_boundingRect = qwtBoundingRect(*this);
    }
    return d_boundingRect;
}


// Common
Project project = SP9500;

quint32 RFIdx = 0;
quint32 RFUIdx = 0;

complex_sequence *complexSequence = nullptr;

// SP9500
sp3301 *SP3301 = nullptr;

sp1401 *SP1401 = nullptr;
sp1401_r1a *SP1401_R1A = nullptr;
sp1401_r1b *SP1401_R1B = nullptr;
sp1401_r1c *SP1401_R1C = nullptr;
sp1401_r1d *SP1401_R1D = nullptr;
sp1401_r1e *SP1401_R1E = nullptr;
sp1401_r1f *SP1401_R1F = nullptr;
sp2401_r1a *SP2401 = nullptr;

// DT3308

// SP9500X
sp1403 *SP1403 = nullptr;

namespace NS_SP9500X {

ns_sp9500x::sp3103 *SP3103 = nullptr;

ns_sp9500x::sp1403_r1a *SP1403_R1A = nullptr;
ns_sp9500x::sp1403_r1b *SP1403_R1B = nullptr;
ns_sp9500x::rrh *SP9500X_RRH = nullptr;
ns_sp9500x::sp2406 *SP2406 = nullptr;

}


