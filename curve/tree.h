#pragma once

#include "figure.h"
#include "project.h"
#include "dialogservice.h"

class Tree : public QTreeWidget {

    Q_OBJECT

public:
    Tree(QWidget *parent = nullptr);
    ~Tree();

    void updateTree(const QList<Figure*> figures);
    QTreeWidgetItem* findChild(const QString &name);
    void setProject(Project* mainProject);

private:
    DialogService* _dialogService;

    QTreeWidgetItem* treeCurves;
    QTreeWidgetItem* treeCircles;
    QTreeWidgetItem* treeLines;
    QTreeWidgetItem* treePoints;
    QTreeWidgetItem* treeDimensions;
    QTreeWidgetItem* treeTexts;

    QAction* renameItem;
    QAction* editItem;
    QAction* removeItem;

    Project* project = nullptr;

    void contextMenuEvent(QContextMenuEvent *event);
    void onRenameItemTriggered();
    void onRemoveItemTriggered();
    void changeCurrentFigure(const QTreeWidgetItem *item);
    void changeCurrentFigureByName(const QString currentFigureName);
    void onItemDoubleClicked(const QTreeWidgetItem *item);
    void onEditItemTriggered();

public slots:
    void addFigure(Figure* figure);
    void removeFigure(const QString &name);
    void renameFigure(const QString &name, const QString &newName);

signals:
    void figureRenameRequested(const QString &name, const QString &newName);
    void figureRemoveRequested(const QString &name);
    void figureEditRequested(const QString &name);
    void figureToggleVisibilityRequested(const QString &name);
    void currentFigureChanged(const QString &name);
};

