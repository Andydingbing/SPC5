#ifndef CHILD_WIDGET_HELPER_HPP
#define CHILD_WIDGET_HELPER_HPP

#include "mainwindow.h"
#include <QObject>
#include <QSizePolicy>

namespace Ui { class MainWindow; }

#define DECL_TREE_ITEM(str,list,widgets) \
    str.clear(); _items.append(new TreeChildItem(str << list,&widgets));

#define ADD_CHILD_WIDGET(widget_list,class_name,n) \
    if (widget_list.empty()) { \
        for (int i = 0;i < n;i ++) { \
            widget_list.push_back(new  class_name(ui->mainTab)); \
            widget_list[i]->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding); \
            widget_list[i]->setVisible(false); \
            MainWindow::childDlgLayout.addWidget(widget_list[i]); \
        } \
    }

class QMenu;
class QAction;
class QTreeWidgetItem;

class ChildWidgetHelper : public QObject
{
    Q_OBJECT

public:
    struct TreeChildItem {
        QStringList stringList;
        QList<QWidget *> tabWidgets;

        TreeChildItem(QStringList &_stringList,void *_tabWidgets = nullptr);
    };

    explicit ChildWidgetHelper(MainWindow *_parent);
    void setMainTree(const QList<TreeChildItem *> childItems);
    void mainTreeItemClicked(QTreeWidgetItem *item);

    virtual void mainTabCurrentChanged(int index) {}
    virtual void initMenu() {}
    virtual void initMainTreeWidget() {}
    virtual void addChildWidgets() {}
    virtual void updatePtr() {}
    virtual QString tabName(int idx) { return QString("RF-%1").arg(idx); }

public slots:
    virtual void initChildWidgets() {}

public:
    MainWindow *parent;
    Ui::MainWindow *ui;

    QList<TreeChildItem *> _items;
};

#endif // CHILD_WIDGET_HELPER_HPP
