#ifndef TREE_H
#define TREE_H

#include <QAction>
#include <QContextMenuEvent>
#include <QWidget>
#include <QTreeWidgetItem>
#include <QMenu>
#include <QInputDialog>
#include <QObject>
#include "figure.h"


class Tree : public QTreeWidget {

    Q_OBJECT

public:
    Tree(QWidget *parent = nullptr);
    ~Tree();

    void updateTree(const QList<Figure*> figures);

private:
    QTreeWidgetItem* treeCurves;
    QTreeWidgetItem* treeCircles;
    QTreeWidgetItem* treeLines;
    QTreeWidgetItem* treePoints;

    QAction* renameItem;
    QAction* removeItem;

    void contextMenuEvent(QContextMenuEvent *event);
    void onRenameItemTriggered();
    void onRemoveItemTriggered();
    void onItemDoubleClicked(const QTreeWidgetItem *item);
    void onItemClicked(const QTreeWidgetItem *item);

signals:

    void objectRenamed(const QString &name, const QString &newName);
    void objectRemoved(const QString &name);
    void toggleVisibility(const QString &name);
    void updateControls(const QString &name);

};
#endif // TREE_H
