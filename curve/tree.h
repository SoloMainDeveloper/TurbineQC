#pragma once

#include <QAction>
#include <QContextMenuEvent>
#include <QWidget>
#include <QTreeWidgetItem>
#include <QMenu>
#include <QInputDialog>
#include <QObject>
#include <QMessageBox>
#include "figure.h"
#include "project.h"


class Tree : public QTreeWidget {

    Q_OBJECT

public:
    Tree(QWidget *parent = nullptr);
    ~Tree();

    void updateTree(const QList<Figure*> figures);
    void setProject(Project* mainProject);

private:
    QTreeWidgetItem* treeCurves;
    QTreeWidgetItem* treeCircles;
    QTreeWidgetItem* treeLines;
    QTreeWidgetItem* treePoints;

    QAction* renameItem;
    QAction* removeItem;

    Project* project = nullptr;

    void contextMenuEvent(QContextMenuEvent *event);
    void onRenameItemTriggered();
    void onRemoveItemTriggered();
    void onItemClicked(const QTreeWidgetItem *item);
    void onItemDoubleClicked(const QTreeWidgetItem *item);

public slots:
    void addFigure(Figure* figure);
    void removeFigure(const QString &name);
    void renameFigure(const QString &name, const QString &newName);

signals:
    void figureRenameRequested(const QString &name, const QString &newName);
    void figureRemoveRequested(const QString &name);
    void figureToggleVisibilityRequested(const QString &name);
    void currentFigureChanged(const QString &name);
};

