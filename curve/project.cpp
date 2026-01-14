#include "curve/pch.h"
#include "project.h"

Project::~Project() {
    for(auto item : _figures) {
        delete item;
    }
}

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

const bool Project::containsFigure(const QString &name) const {
    return findFigure(name) ? true : false;
}

void Project::setVisibility(const QStringList &figureNames) {
    for(auto &figureName : figureNames) {
        changeFigureVisibility(figureName, true);
    }
}

void Project::resetVisibilityForAllFigures() {
    for(auto figure : _figures) {
        if(figure->isVisible()) {
            changeFigureVisibility(figure->name(), false);
        }
    }
}

void Project::setCurrentFigure(const QString &currentFigureName) {
    changeCurrentFigure(currentFigureName);
}

double Project::scaleFactor() const {
    return _scaleFactor;
}

const Point* Project::centerPoint() const {
    return _centerPoint;
}

void Project::insertFigure(Figure * figure) {
    _figures.append(figure);
    emit figureAdded(figure);
}

void Project::renameFigure(const QString &name, const QString &newName) {
    findFigureMutable(name)->setName(newName);
    emit figureRenamed(name, newName);

    if(name == _currentFigureName) {
        _currentFigureName = newName;
        currentFigureChanged(newName, name);
    }
    MacrosManager::log(MacrosManager::RenameFigure, { { "figureName", name }, { "newName", newName } });
}

void Project::removeFigure(const QString &name) {
    if(_currentFigureName == name) {
        changeCurrentFigure(QString());
    }

    emit figureAboutToBeRemoved(name);
    _figures.removeOne(findFigure(name));
    MacrosManager::log(MacrosManager::RemoveFigure, { { "figureName", name } });
}

void Project::changeCurrentFigure(const QString &currentFigureName) {
    auto previousFigureName = _currentFigureName;
    _currentFigureName = currentFigureName;
    emit currentFigureChanged(currentFigureName, previousFigureName);
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
    bool showVectors, bool closed, bool showNumbering, int numberingInterval,
    double amplification, bool showTolerances, bool showDeviations, bool connectDeviations, bool highLightOut) {
    auto curve = findFigureMutable(figureName);
    auto currentCurve = dynamic_cast<CurveFigure*>(curve);
    currentCurve->setShowPoints(showPoints);
    currentCurve->setConnectPoints(connectPoints);
    currentCurve->setShowVectors(showVectors);
    currentCurve->setClosed(closed);
    currentCurve->setShowNumbering(showNumbering);
    currentCurve->setNumberingInterval(numberingInterval);
    currentCurve->setAmplification(amplification);
    currentCurve->setShowTolerances(showTolerances);
    currentCurve->setShowDeviations(showDeviations);
    currentCurve->setConnectDeviations(connectDeviations);
    currentCurve->setHighLightOut(highLightOut);
    emit curveParametersChanged(figureName, showPoints, connectPoints,
        showVectors, closed, showNumbering, numberingInterval, 
        amplification, showTolerances, showDeviations, connectDeviations, highLightOut);
}

void Project::changeCurveTolerance(const QString curveName, QVector<CurvePoint> curveWithTolerances) {
    auto figure = findFigureMutable(curveName);
    if(auto curve = dynamic_cast<CurveFigure*>(figure)) {
        curve->changePoints(curveWithTolerances);
        curve->setClosed(true);
        curve->setShowTolerances(true);
        emit curveParametersChanged(curveName, curve->isShowPoints(), curve->isConnectPoints(), curve->isShowVectors(), curve->isClosed(), curve->isShowNumbering(), curve->numberingInterval(), 
            curve->amplification(), curve->isShowTolerances(), curve->isShowDeviations(), curve->isConnectDeviations(), curve->isHighLightOut());
        emit curveToleranceChanged(curveName);
    }
}

Figure* Project::findFigureMutable(const QString &name) {
    return const_cast<Figure*>(findFigure(name));
}

const QString Project::currentFigureName() const {
    return _currentFigureName;
}

void Project::requestFigureEditDialog(const QString figureName) {
    emit figureEditDialogRequested(figureName);
}

void Project::safeInsert(QString figureName, Figure *figure) {
    if(_currentFigureName == figureName) {
        removeFigure(figureName);
        insertFigure(figure);
        changeCurrentFigure(figureName);
        return;
    } else if(containsFigure(figureName)) {
        removeFigure(figureName);
    }
    insertFigure(figure);
}

void Project::changeCurvePoints(QString curveName, QVector<CurvePoint> newPoints) {
    auto figure = findFigureMutable(curveName);
    if(auto curve = dynamic_cast<CurveFigure*>(figure)) {
        curve->changePoints(newPoints);
        emit curvePointsChanged(curveName);
    }
}

void Project::changeFigureColor(const QString figureName, QColor color) {
    auto figure = findFigureMutable(figureName);
    figure->setColor(color);
    emit figureColorChanged(figureName);
}

void Project::changeScale(double scaleFactor, const Point &center) {
    _scaleFactor = scaleFactor;
    _centerPoint = &center;

    emit scaleChanged(scaleFactor, center);
}

void Project::clear() {
    auto length = _figures.length();
    for(auto i = length - 1; i >= 0; i--) {
        removeFigure(_figures[i]->name());
    }
    MacrosManager::log(MacrosManager::ClearProject);
    emit cleared();
}
