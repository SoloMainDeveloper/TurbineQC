#include "curve/pch.h"
#include "project.h"

const QList<Figure*>& Project::figures() const {
    return _figures;
}

const Figure* Project::findFigure(const QString &name) const {
    for(auto i = 0; i < _figures.length(); i++) {
        if(_figures[i]->name() == name) {
            return _figures[i];
        }
    }

    return nullptr;
}

void Project::insertFigure(Figure * figure) {
    _figures.append(figure);
    emit figureAdded(figure);
}

void Project::renameFigure(const QString &name, const QString &newName) {
    findFigureMutable(name)->setName(newName);
    emit figureRenamed(name, newName);
}

void Project::removeFigure(const QString &name) {
    emit figureAboutToBeRemoved(name);
    _figures.removeOne(findFigure(name));
}

void Project::changeCurrentFigure(const QString &currentFigureName) {
    emit currentFigureChanged(currentFigureName);
}

void Project::changeFigureVisibility(const QString figureName, bool visible) {
    auto figure = findFigureMutable(figureName);
    figure->setVisible(visible);
    emit figureVisibilityChanged(figureName, visible);
}

void Project::toggleFigureVisibility(const QString figureName) {
    auto figure = findFigureMutable(figureName);
    figure->toggleVisible();
    auto visible = figure->isVisible();
    emit figureVisibilityChanged(figureName, visible);
}

void Project::changeCurveParameters(const QString figureName, bool showPoints, bool connectPoints,
    bool showVectors, bool closed, bool showNumbering, int numberingInterval) {
    auto curve = findFigureMutable(figureName);
    auto currentCurve = dynamic_cast<CurveFigure*>(curve);
    currentCurve->setShowPoints(showPoints);
    currentCurve->setConnectPoints(connectPoints);
    currentCurve->setShowVectors(showVectors);
    currentCurve->setClosed(closed);
    currentCurve->setShowNumbering(showNumbering);
    currentCurve->setNumberingInterval(numberingInterval);
    emit curveParametersChanged(figureName, showPoints, connectPoints,
        showVectors, closed, showNumbering, numberingInterval);
}

Figure* Project::findFigureMutable(const QString &name) {
    return const_cast<Figure*>(findFigure(name));
}
