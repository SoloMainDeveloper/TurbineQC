#include "curve/pch.h"
#include "tree.h"

Tree::Tree(QWidget *parent) {

    renameItem = new QAction(QIcon("icons/rename.ico"), "Rename");
    removeItem = new QAction(QIcon("icons/delete.ico"), "Remove");
    editItem = new QAction(QIcon("icons/edit.ico"), "Edit");

    connect(renameItem, &QAction::triggered, this, &Tree::onRenameItemTriggered);
    connect(removeItem, &QAction::triggered, this, &Tree::onRemoveItemTriggered);
    connect(editItem, &QAction::triggered, this, &Tree::onEditItemTriggered);

    connect(this, &QTreeWidget::currentItemChanged, this, &Tree::changeCurrentFigure);
    connect(this, &QTreeWidget::itemDoubleClicked, this, &Tree::onItemDoubleClicked);

    treeCurves = new QTreeWidgetItem;
    treeCurves->setText(0, "Curves");
    treeCurves->setIcon(0, QIcon("icons/curve.ico"));
    treeCurves->setBackground(0, QBrush("#f7f7f7"));
    addTopLevelItem(treeCurves);
    treeCurves->setExpanded(true);

    treeCircles = new QTreeWidgetItem;
    treeCircles->setText(0, "Circles");
    treeCircles->setIcon(0, QIcon("icons/circle.ico"));
    treeCircles->setBackground(0, QBrush("#f7f7f7"));
    addTopLevelItem(treeCircles);
    treeCircles->setExpanded(true);

    treeLines = new QTreeWidgetItem;
    treeLines->setText(0, "Lines");
    treeLines->setIcon(0, QIcon("icons/line.ico"));
    treeLines->setBackground(0, QBrush("#f7f7f7"));
    addTopLevelItem(treeLines);
    treeLines->setExpanded(true);

    treePoints = new QTreeWidgetItem;
    treePoints->setText(0, "Points");
    treePoints->setIcon(0, QIcon("icons/point.ico"));
    treePoints->setBackground(0, QBrush("#f7f7f7"));
    addTopLevelItem(treePoints);
    treePoints->setExpanded(true);

    treeDimensions = new QTreeWidgetItem;
    treeDimensions->setText(0, "Dimensions");
    treeDimensions->setIcon(0, QIcon("icons/dimension.ico"));
    treeDimensions->setBackground(0, QBrush("#f7f7f7"));
    addTopLevelItem(treeDimensions);
    treeDimensions->setExpanded(true);

    treeTexts = new QTreeWidgetItem;
    treeTexts->setText(0, "Texts");
    treeTexts->setIcon(0, QIcon("icons/text.ico"));
    treeTexts->setBackground(0, QBrush("#f7f7f7"));
    addTopLevelItem(treeTexts);
    treeTexts->setExpanded(true);
}

void Tree::setProject(Project* mainProject) {
    project = mainProject;

    connect(this, &Tree::figureRenameRequested, project, [&](const QString& name, const QString& newName) {
        try {
            project->renameFigure(name, newName);   
        } catch(...){ }
    });
    connect(this, &Tree::figureRemoveRequested, project, &Project::removeFigure);
    connect(this, &Tree::currentFigureChanged, project, &Project::changeCurrentFigure);
    connect(this, &Tree::figureToggleVisibilityRequested, project, &Project::toggleFigureVisibility);
    connect(this, &Tree::figureEditRequested, project, &Project::requestFigureEditDialog);

    connect(project, &Project::figureAdded, this, &Tree::addFigure);
    connect(project, &Project::figureAboutToBeRemoved, this, &Tree::removeFigure);
    connect(project, &Project::figureRenamed, this, &Tree::renameFigure);
    connect(project, &Project::currentFigureChanged, this, &Tree::changeCurrentFigureByName);
    connect(project, &Project::figureVisibilityChanged, [&](QString figureName, bool visible) {
        visible ? findChild(figureName)->setIcon(0, QIcon("icons/showAll.ico")) : findChild(figureName)->setIcon(0, QIcon("icons/empty.ico"));
    });
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
    QTreeWidgetItem *child = new QTreeWidgetItem;
    child->setText(0, figure->name());
    if(dynamic_cast<CurveFigure*>(figure)) {
        treeCurves->addChild(child);
        treeCurves->setText(0, "Curves (" + QString::number(treeCurves->childCount()) + ")");
    } else if(dynamic_cast<CircleFigure*>(figure)) {
        treeCircles->addChild(child);
        treeCircles->setText(0, "Circles (" + QString::number(treeCircles->childCount()) + ")");
    } else if(dynamic_cast<LineFigure*>(figure)) {
        treeLines->addChild(child);
        treeLines->setText(0, "Lines (" + QString::number(treeLines->childCount()) + ")");
    } else if(dynamic_cast<PointFigure*>(figure)) {
        treePoints->addChild(child);
        treePoints->setText(0, "Points (" + QString::number(treePoints->childCount()) + ")");
    } else if(dynamic_cast<DimFigure*>(figure)) {
        treeDimensions->addChild(child);
        treeDimensions->setText(0, "Dimensions (" + QString::number(treeDimensions->childCount()) + ")");
    } else if(dynamic_cast<TextFigure*>(figure)) {
        treeTexts->addChild(child);
        treeTexts->setText(0, "Texts (" + QString::number(treeTexts->childCount()) + ")");
    } else {
        delete child;
    }
    figure->isVisible() ? child->setIcon(0, QIcon("icons/showAll.ico")) : child->setIcon(0, QIcon("icons/empty.ico"));
}

void Tree::removeFigure(const QString &name) {
    QTreeWidgetItem *child = findChild(name);
    QTreeWidgetItem *tree = child->parent();
    int index = tree->indexOfChild(child);
    tree->takeChild(index);
    delete child;
    tree->setText(0, tree->text(0).section(" (", 0, 0) + " (" + QString::number(tree->childCount()) + ")");
}

void Tree::renameFigure(const QString &name, const QString &newName) {
    QTreeWidgetItem *child = findChild(name);
    child->setText(0, newName);
}

void Tree::updateTree(QList<Figure*> figures) {

    for(int i = 0; i < topLevelItemCount(); i++) {
        qDeleteAll(topLevelItem(i)->takeChildren());
    }

    for(auto figure : figures) {
        addFigure(figure);
    }
}

QTreeWidgetItem* Tree::findChild(const QString &name) {
    auto items = findItems(name, Qt::MatchFlag::MatchFixedString | Qt::MatchFlag::MatchCaseSensitive | Qt::MatchFlag::MatchRecursive | Qt::MatchFlag::MatchExactly);
    assert(items.size() == 1);
    return items[0];
}

void Tree::onRenameItemTriggered() {
    auto text = QInputDialog::getText(this, "Rename element", "Input new name", QLineEdit::Normal, currentItem()->text(0));
    if(text == "") {
        return;
    }
    emit figureRenameRequested(currentItem()->text(0), text);
}

void Tree::onRemoveItemTriggered() {
    auto figureName = currentItem()->text(0);
    
    if(project->confirmRemoveFigure(figureName)) {
         emit figureRemoveRequested(figureName);
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
    if(oldCurrentFigureName != currentFigureName && currentFigureName != QString()) {
        QTreeWidgetItem *newItem = findChild(currentFigureName);

        if(newItem != nullptr) {
            blockSignals(true);
            setCurrentItem(newItem);
            scrollToItem(newItem);
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
