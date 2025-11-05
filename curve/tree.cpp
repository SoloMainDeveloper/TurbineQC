#include "curve/pch.h"
#include "tree.h"

Tree::Tree(QWidget *parent) {

    setHeaderHidden(true);

    renameItem = new QAction(QIcon("icons/rename.png"), "Rename");
    removeItem = new QAction(QIcon("icons/delete.png"), "Remove");

    connect(renameItem, &QAction::triggered, this, &Tree::onRenameItemTriggered);
    connect(removeItem, &QAction::triggered, this, &Tree::onRemoveItemTriggered);
    connect(this, &QTreeWidget::itemDoubleClicked, this, &Tree::onItemDoubleClicked);
    connect(this, &QTreeWidget::itemClicked, this, &Tree::onItemClicked);

    treeCurves = new QTreeWidgetItem;
    treeCurves->setText(0, "Curves");
    treeCurves->setIcon(0, QIcon("icons/curve.png"));
    this->addTopLevelItem(treeCurves);
    treeCurves->setExpanded(true);

    treeCircles = new QTreeWidgetItem;
    treeCircles->setText(0, "Circles");
    treeCircles->setIcon(0, QIcon("icons/circle.png"));
    this->addTopLevelItem(treeCircles);
    treeCircles->setExpanded(true);

    treeLines = new QTreeWidgetItem;
    treeLines->setText(0, "Lines");
    treeLines->setIcon(0, QIcon("icons/line.png"));
    this->addTopLevelItem(treeLines);
    treeLines->setExpanded(true);

    treePoints = new QTreeWidgetItem;
    treePoints->setText(0, "Points");
    treePoints->setIcon(0, QIcon("icons/point.png"));
    this->addTopLevelItem(treePoints);
    treePoints->setExpanded(true);

}

void Tree::contextMenuEvent(QContextMenuEvent *event) {
    if(currentItem()->parent()) {
        QMenu menu;
        menu.addAction(renameItem);
        menu.addAction(removeItem);
        menu.exec(event->globalPos());
    }
}

void Tree::updateTree(QList<Figure*> figures) {

    for(int i = 0; i < topLevelItemCount(); i++) {
        qDeleteAll(topLevelItem(i)->takeChildren());
    }

    for(auto figure : figures) {
        if(dynamic_cast<CurveFigure*>(figure)) {
            QTreeWidgetItem *child = new QTreeWidgetItem;
            child->setText(0, figure->name());
            treeCurves->addChild(child);
        } else if(dynamic_cast<CircleFigure*>(figure)) {
            QTreeWidgetItem *child = new QTreeWidgetItem;
            child->setText(0, figure->name());
            treeCircles->addChild(child);
        } else if(dynamic_cast<LineFigure*>(figure)) {
            QTreeWidgetItem *child = new QTreeWidgetItem;
            child->setText(0, figure->name());
            treeLines->addChild(child);
        } else if(dynamic_cast<PointFigure*>(figure)) {
            QTreeWidgetItem *child = new QTreeWidgetItem;
            child->setText(0, figure->name());
            treePoints->addChild(child);
        }
    }
}

void Tree::onRenameItemTriggered() {
    auto text = QInputDialog::getText(this, "Rename element", "Input new name");
    emit objectRenamed(currentItem()->text(0), text);
    currentItem()->setText(0, text);
}

void Tree::onRemoveItemTriggered() {
    emit objectRemoved(currentItem()->text(0));
    delete currentItem();
}

void Tree::onItemDoubleClicked(const QTreeWidgetItem *item) {
    if(item->parent()) {
        emit toggleVisibility(item->text(0));
    }
}

void Tree::onItemClicked(const QTreeWidgetItem *item) {
    if(item->parent()) {
        emit updateControls(item->text(0));
    }
}

Tree::~Tree() {
    delete renameItem;
    delete removeItem;
}
