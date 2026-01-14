#include "curve/pch.h"
#include "tree.h"

Tree::Tree(QWidget *parent) {

    setHeaderHidden(true);

    renameItem = new QAction(QIcon("icons/rename.png"), "Rename");
    removeItem = new QAction(QIcon("icons/delete.png"), "Remove");
    editItem = new QAction("Edit");

    connect(renameItem, &QAction::triggered, this, &Tree::onRenameItemTriggered);
    connect(removeItem, &QAction::triggered, this, &Tree::onRemoveItemTriggered);
    connect(editItem, &QAction::triggered, this, &Tree::onEditItemTriggered);

    connect(this, &QTreeWidget::currentItemChanged, this, &Tree::changeCurrentFigure);
    connect(this, &QTreeWidget::itemDoubleClicked, this, &Tree::onItemDoubleClicked);

    treeCurves = new QTreeWidgetItem;
    treeCurves->setText(0, "Curves");
    treeCurves->setIcon(0, QIcon("icons/curve.png"));
    treeCurves->setBackground(0, QBrush("#f7f7f7"));
    addTopLevelItem(treeCurves);
    treeCurves->setExpanded(true);

    treeCircles = new QTreeWidgetItem;
    treeCircles->setText(0, "Circles");
    treeCircles->setIcon(0, QIcon("icons/circle.png"));
    treeCircles->setBackground(0, QBrush("#f7f7f7"));
    addTopLevelItem(treeCircles);
    treeCircles->setExpanded(true);

    treeLines = new QTreeWidgetItem;
    treeLines->setText(0, "Lines");
    treeLines->setIcon(0, QIcon("icons/line.png"));
    treeLines->setBackground(0, QBrush("#f7f7f7"));
    addTopLevelItem(treeLines);
    treeLines->setExpanded(true);

    treePoints = new QTreeWidgetItem;
    treePoints->setText(0, "Points");
    treePoints->setIcon(0, QIcon("icons/point.png"));
    treePoints->setBackground(0, QBrush("#f7f7f7"));
    addTopLevelItem(treePoints);
    treePoints->setExpanded(true);

}

void Tree::setProject(Project* mainProject) {
    project = mainProject;

    connect(this, &Tree::figureRenameRequested, project, &Project::renameFigure);
    connect(this, &Tree::figureRemoveRequested, project, &Project::removeFigure);
    connect(this, &Tree::currentFigureChanged, project, &Project::changeCurrentFigure);
    connect(this, &Tree::figureToggleVisibilityRequested, project, &Project::toggleFigureVisibility);
    connect(this, &Tree::figureEditRequested, project, &Project::requestFigureEditDialog);

    connect(project, &Project::figureAdded, this, &Tree::addFigure);
    connect(project, &Project::figureAboutToBeRemoved, this, &Tree::removeFigure);
    connect(project, &Project::figureRenamed, this, &Tree::renameFigure);
    connect(project, &Project::currentFigureChanged, this, &Tree::changeCurrentFigureByName);
}

void Tree::contextMenuEvent(QContextMenuEvent *event) {
    if(currentItem()->parent()) {
        QMenu menu;
        menu.addAction(renameItem);
        menu.addAction(editItem);
        menu.addAction(removeItem);
        menu.exec(event->globalPos());
    }
}

void Tree::addFigure(Figure* figure) {
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

void Tree::removeFigure(const QString &name) {
    for(int i = 0; i < treeCurves->childCount(); i++) {
        auto child = treeCurves->child(i);
        if(child->text(0) == name) {
            treeCurves->takeChild(i);
            return;
        }
    }
    for(int i = 0; i < treeCircles->childCount(); i++) {
        auto child = treeCircles->child(i);
        if(child->text(0) == name) {
            treeCircles->takeChild(i);
            return;
        }
    }
    for(int i = 0; i < treeLines->childCount(); i++) {
        auto child = treeLines->child(i);
        if(child->text(0) == name) {
            treeLines->takeChild(i);
            return;
        }
    }
    for(int i = 0; i < treePoints->childCount(); i++) {
        auto child = treePoints->child(i);
        if(child->text(0) == name) {
            treePoints->takeChild(i);
            return;
        }
    }
}

void Tree::renameFigure(const QString &name, const QString &newName) {
    for(int i = 0; i < treeCurves->childCount(); i++) {
        auto child = treeCurves->child(i);
        if(child->text(0) == name) {
            child->setText(0, newName);
            return;
        }
    }
    for(int i = 0; i < treeCircles->childCount(); i++) {
        auto child = treeCircles->child(i);
        if(child->text(0) == name) {
            child->setText(0, newName);
            return;
        }
    }
    for(int i = 0; i < treeLines->childCount(); i++) {
        auto child = treeLines->child(i);
        if(child->text(0) == name) {
            child->setText(0, newName);
            return;
        }
    }
    for(int i = 0; i < treePoints->childCount(); i++) {
        auto child = treePoints->child(i);
        if(child->text(0) == name) {
            child->setText(0, newName);
            return;
        }
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
    if(text == "") {
        return;
    }
    emit figureRenameRequested(currentItem()->text(0), text);
}

void Tree::onRemoveItemTriggered() {
    QMessageBox mBox;
    mBox.setText("Delete current item?");
    mBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    auto dialogWindow = mBox.exec();
    switch(dialogWindow) {
        case QMessageBox::Ok:
            emit figureRemoveRequested(currentItem()->text(0));
            break;
        case QMessageBox::Cancel:
            break;
    }
}

void Tree::onEditItemTriggered() {
    emit figureEditRequested(currentItem()->text(0));
}

void Tree::changeCurrentFigure(const QTreeWidgetItem *item) {
    if(item->parent()) {
        auto name = item->text(0);
        emit currentFigureChanged(name);
    } else {
        emit currentFigureChanged(QString());
    }
}

void Tree::changeCurrentFigureByName(const QString currentFigureName) {
    QString oldCurrentFigureName;
    if(currentItem()) {
        oldCurrentFigureName = currentItem()->text(0);
    }
    if(oldCurrentFigureName == currentFigureName || currentFigureName == QString()) {
        return;
    } else {
        QTreeWidgetItem* newItem;

        for(int i = 0; i < treeCurves->childCount(); i++) {
            auto child = treeCurves->child(i);
            if(child->text(0) == currentFigureName) {
                newItem = child;
            }
        }
        for(int i = 0; i < treeCircles->childCount(); i++) {
            auto child = treeCircles->child(i);
            if(child->text(0) == currentFigureName) {
                newItem = child;
            }
        }
        for(int i = 0; i < treeLines->childCount(); i++) {
            auto child = treeLines->child(i);
            if(child->text(0) == currentFigureName) {
                newItem = child;
            }
        }
        for(int i = 0; i < treePoints->childCount(); i++) {
            auto child = treePoints->child(i);
            if(child->text(0) == currentFigureName) {
                newItem = child;
            }
        }

        if(!newItem) {
            return;
        } else {
            blockSignals(true);
            setCurrentItem(newItem);
            blockSignals(false);
        }       
    }

}

void Tree::onItemDoubleClicked(const QTreeWidgetItem *item) {
    if(item->parent()) {
        auto name = item->text(0);
        emit figureToggleVisibilityRequested(name);
    }
}

Tree::~Tree() {
    delete renameItem;
    delete removeItem;
    delete editItem;
}
