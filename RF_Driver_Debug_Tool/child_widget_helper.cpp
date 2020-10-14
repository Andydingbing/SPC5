#include "child_widget_helper.h"
#include "mainwindow.h"
#include "rd_sp9500x.h"
#include <QString>
#include <QMenu>
#include <QAction>

ChildWidgetHelper::TreeChildItem::TreeChildItem(QStringList &_stringList,void *_tabWidgets)
{
    stringList = _stringList;

    if (_tabWidgets != nullptr) {
        QList<QWidget *> *widgets = static_cast<QList<QWidget *> *>(_tabWidgets);
        tabWidgets = *widgets;
    }
}

ChildWidgetHelper::ChildWidgetHelper(MainWindow *_parent) : QObject()
{
    parent = _parent;
    ui = _parent->ui;
}

QTreeWidgetItem *addTreeItem(const QString item,QTreeWidgetItem *parent)
{
    QList<QTreeWidgetItem *>::const_iterator iter;

    for (int i = 0;i < parent->childCount();++i) {
        if (item == parent->child(i)->text(0)) {
            return parent->child(i);
        }
    }
    QTreeWidgetItem *treeItem = new QTreeWidgetItem(parent,QStringList(item));
    return parent->child(parent->childCount() - 1);
}

QTreeWidgetItem *rootItem(const QString item,const QList<QTreeWidgetItem *> &rootItems)
{
    QList<QTreeWidgetItem *>::const_iterator iter;

    for (iter = rootItems.constBegin();iter != rootItems.constEnd();++iter) {
        if (item == (*iter)->text(0)) {
            return *iter;
        }
    }
    return nullptr;
}

quint32 parents(const QTreeWidgetItem *item)
{
    QTreeWidgetItem *parent = item->parent();
    quint32 i = 0;

    while (parent != nullptr) {
        parent = parent->parent();
        ++ i;
    }
    return i;
}

bool compare(const QStringList &stringList,const QTreeWidgetItem *item)
{
    const QTreeWidgetItem *parent = item;
    quint32 i = parents(item);

    if (i != stringList.size() - 1) {
        return false;
    }

    for (quint32 j = 0;j <= i;j ++) {
        if (parent->text(0) != stringList.at(i - j)) {
            return false;
        }
        parent = parent->parent();
    }
    return true;
}

void ChildWidgetHelper::setMainTree(const QList<TreeChildItem *> childItems)
{
    QList<TreeChildItem *>::const_iterator iterCI;
    QList<QTreeWidgetItem *>::iterator iterItem;
    QList<QTreeWidgetItem *> rootItems;
    QStringList::const_iterator iterStr;

    for (iterCI = childItems.constBegin();iterCI != childItems.constEnd();++iterCI) {
        for (iterItem = rootItems.begin();iterItem != rootItems.end();++iterItem) {
            if (*(*iterCI)->stringList.begin() == (*iterItem)->text(0)) {
                break;
            }
        }
        if (iterItem == rootItems.end()) {
            rootItems.append(new QTreeWidgetItem(ui->mainTree,QStringList(*((*iterCI)->stringList.begin()))));
        }
    }

    for (iterCI = childItems.constBegin();iterCI != childItems.constEnd();++iterCI) {
        QTreeWidgetItem *root = rootItem(*(*iterCI)->stringList.begin(),rootItems);
        for (iterStr = (*iterCI)->stringList.begin() + 1;iterStr != (*iterCI)->stringList.end();++iterStr) {
            root = addTreeItem(*iterStr,root);
        }
    }

    ui->mainTree->expandAll();
    for (int i = 0;i < rootItems.size();i ++) {
        if (i >= 2) {
            rootItems.at(i)->setExpanded(false);
        }
    }
}

void ChildWidgetHelper::mainTreeItemClicked(QTreeWidgetItem *item)
{
    QList<TreeChildItem *>::const_iterator iterItems;

    ui->mainTab->clear();

    for (iterItems = _items.constBegin();iterItems != _items.constEnd();++iterItems) {
        if (compare((*iterItems)->stringList,item)) {
            for (int i = 0;i < (*iterItems)->tabWidgets.size();++i) {
                ui->mainTab->addTab((*iterItems)->tabWidgets.at(i),tabName(i));
            }
            return;
        }
    }
}
