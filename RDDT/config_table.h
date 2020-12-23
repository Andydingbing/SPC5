#ifndef Q_CONFIG_TABLE_H
#define Q_CONFIG_TABLE_H

#include <QTableView>
#include <QStandardItemModel>
#include <QStyledItemDelegate>

class Q_Config_Table_View : public QTableView
{
public:
    Q_Config_Table_View(QWidget *parent = nullptr);
};


class Q_Config_Table_Model : public QStandardItemModel
{
public:
    Q_Config_Table_Model(QObject *parent = nullptr) : QStandardItemModel(parent) {}

    int rowCount(const QModelIndex &) const { return _item.size(); }

    int columnCount(const QModelIndex &) const { return 2; }

    QVariant data(const QModelIndex &index, int role) const;

protected:
    QStringList _item;
};


class Q_Config_Table_Delegate : public QStyledItemDelegate
{
public:
    Q_Config_Table_Delegate(QObject *parent = nullptr) : QStyledItemDelegate(parent) {}

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    void setEditorData(QWidget *editor, const QModelIndex &index) const;

    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    virtual QWidget **first() const = 0;

#define CONFIG_TABLE_FIRST_WIDGET(type,widget) \
    public: QWidget **first() const { return reinterpret_cast<QWidget **>(const_cast<type **>(&widget)); }
};

#endif // Q_CONFIG_TABLE_H
