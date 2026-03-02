#include "curve/pch.h"
#include "project.h"

Project::~Project() {
    for(auto item : _figures) {
        delete item;
    }
}

const QMap<QString, Figure*>& Project::figures() const {
    return _figures;
}

const QList<CurveFigure*>& Project::curveFigures() const {
    auto result = new QList<CurveFigure*>();
    for(auto figure : _figures) {
        if(auto curveFigure = dynamic_cast<CurveFigure*>(figure)) {
            result->append(curveFigure);
        }
    }
    return *result;
}

const QList<DimFigure*>& Project::dimFigures() const {
    auto result = new QList<DimFigure*>();
    for(auto figure : _figures) {
        if(auto dimFigure = dynamic_cast<DimFigure*>(figure)) {
            result->append(dimFigure);
        }
    }
    return *result;
}

const QList<TextFigure*>& Project::textFigures() const {
    auto result = new QList<TextFigure*>();
    for(auto figure : _figures) {
        if(auto textFigure = dynamic_cast<TextFigure*>(figure)) {
            result->append(textFigure);
        }
    }
    return *result;
}

const Figure* Project::findFigure(const QString &name) const {
    return _figures.contains(name) ? _figures[name] : nullptr;
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

int Project::precision() const {
    return _precision;
}

double Project::scaleFactor() const {
    return _scaleFactor;
}

Point Project::centerPoint() const {
    return _centerPoint;
}

void Project::setProjectPath(const QString &path) {
    _projectPath = path;
    emit projectPathChanged(_projectPath);
}

const QString Project::projectPath() {
    return _projectPath;
}

void Project::insertFigure(Figure *figure) {
    _figures[figure->name()] = figure;
    emit figureAdded(figure);
    initiateParentChildReference(figure);
}

void Project::initiateParentChildReference(Figure *figure) {
    if(auto dim = dynamic_cast<DimFigure*>(figure)) {
        attachChildToParent(dim, dim->firstReference());
        attachChildToParent(dim, dim->secondReference());
        emit figureEdited(dim->name());
    } else if(auto text = dynamic_cast<TextFigure*>(figure)) {
        attachChildToParent(text, text->reference());
        emit figureEdited(text->name());
    } else {
        if(_lostParents.contains(figure->name())) {
            for(auto child : _lostParents[figure->name()]) {
                figure->addChild(child);
                emit figureEdited(child->name());
            }
            _lostParents.remove(figure->name());
        }
    }
}

void Project::updateParent(Figure *child, QString newParent1, QString newParent2) { //edit references
    if(auto dim = dynamic_cast<DimFigure*>(child)) {
        detachChildFromParent(dim, dim->firstReference());
        detachChildFromParent(dim, dim->secondReference());
        attachChildToParent(dim, newParent1);
        attachChildToParent(dim, newParent2);
    } else if(auto text = dynamic_cast<TextFigure*>(child)) {
        detachChildFromParent(text, text->reference());
        attachChildToParent(text, newParent1);
    }
    emit figureEdited(child->name());
}

void Project::attachChildToParent(Figure *child, QString parentName) {
    if(parentName != nullptr) {
        auto parent1 = findFigure(parentName);
        if(parent1 != nullptr)
            parent1->addChild(child);
        else
            _lostParents[parentName].append(child);
    }
}

void Project::detachChildFromParent(Figure *child, QString parentName) {
    if(parentName != nullptr) {
        auto parent1 = findFigure(parentName);
        if(parent1) {
            parent1->removeChild(child);
        } else {
            _lostParents[parentName].removeOne(child);
        }
    }
}

void Project::renameFigure(const QString &name, const QString &newName) {
    ARGUMENT_ASSERT(containsFigure(name), "Rename Figure: figure not found");
    ARGUMENT_ASSERT(!containsFigure(newName), "Rename Figure: other figure with the same name already exists");
    auto figure = findFigureMutable(name);
    figure->setName(newName);

    _figures.remove(name);
    _figures.insert(newName, figure);

    emit figureRenamed(name, newName);

    if(name == _currentFigureName) {
        _currentFigureName = newName;
        currentFigureChanged(newName, name);
    }
    for(auto child : figure->childrenMutable())
        child->updateRefToParent(name, newName);
    MacrosManager::log(MacrosManager::RenameFigure, { { "figureName", name }, { "newName", newName } });
}

void Project::removeFigure(const QString &name) {
    ARGUMENT_ASSERT(containsFigure(name), "Remove Figure: figure`s not found");

    if(_currentFigureName == name) {
        changeCurrentFigure(QString());
    }
    auto figure = findFigure(name);
    for(auto child : figure->childrenMutable()) {
        child->updateRefToParent(name, QString());
        emit figureEdited(child->name());
    }

    emit figureAboutToBeRemoved(name);
    _figures.remove(figure->name());
    MacrosManager::log(MacrosManager::RemoveFigure, { { "figureName", name } });
}

void Project::editFigure(const QString& name, QMap<QString, QString>& paramsChanged) {
    ARGUMENT_ASSERT(containsFigure(name), "Edit Figure: figure`s not found");
    auto figure = findFigureMutable(name);
    if(paramsChanged.contains("newName")) {
        MacrosManager::executeWithoutLogging([&]() {
            renameFigure(name, paramsChanged.value("newName"));
        });
    }
    if(paramsChanged.contains("newColor")) {
        MacrosManager::executeWithoutLogging([&]() {
            auto color = ColorTranslator::getColorFromInt(paramsChanged.value("newColor").toInt());
            changeFigureColor(figure->name(), *color);
        });
    }
    if(auto dim = dynamic_cast<DimFigure*>(figure)) {
        updateParent(dim, paramsChanged["Ref1"], paramsChanged["Ref2"]);
    } else if(auto text = dynamic_cast<TextFigure*>(figure)) {
        updateParent(text, paramsChanged["Ref"]);
    }
    figure->edit(paramsChanged);
    MacrosManager::log(MacrosManager::EditFigure, paramsChanged);
    emit figureEdited(figure->name());
}

void Project::changeCurrentFigure(const QString &currentFigureName) {
    auto previousFigureName = _currentFigureName;
    _currentFigureName = currentFigureName;
    emit currentFigureChanged(currentFigureName, previousFigureName);
}

void Project::changeFigureVisibility(const QString figureName, bool visible) {
    ARGUMENT_ASSERT(containsFigure(figureName), "Change Figure Visibility: figure`s not found");
    auto figure = findFigureMutable(figureName);
    figure->setVisible(visible);
    emit figureVisibilityChanged(figureName, visible);
    MacrosManager::log(MacrosManager::ChangeFigureVisibility, {
        { "figureName", figureName },
        { "visible", visible == true ? "true" : "false" } });
}

void Project::toggleFigureVisibility(const QString figureName) {
    auto figure = findFigureMutable(figureName);
    figure->toggleVisible();
    auto visible = figure->isVisible();
    emit figureVisibilityChanged(figureName, visible);
    MacrosManager::log(MacrosManager::ChangeFigureVisibility, {
        { "figureName", figureName },
        { "visible", visible == true ? "true" : "false" } });
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
    MacrosManager::log(MacrosManager::ChangeCurveParameters, {
        { "figureName", figureName },
        { "showPoints", showPoints == true ? "true" : "false" },
        { "connectPoints", connectPoints == true ? "true" : "false" },
        { "showVectors", showVectors == true ? "true" : "false" },
        { "closed", closed == true ? "true" : "false" },
        { "showNumbering", showNumbering == true ? "true" : "false" },
        { "numberingInterval", QString::number(numberingInterval) },
        { "amplification", QString::number(amplification) },
        { "showTolerances", showTolerances == true ? "true" : "false" },
        { "showDeviations", showDeviations == true ? "true" : "false" },
        { "connectDeviations", connectDeviations == true ? "true" : "false" },
        { "highLightOut", highLightOut == true ? "true" : "false" }, });
}

void Project::changeDimensionParameters(const QString dimName, bool onlyLabel, bool showTols, bool freePosition) {
    auto dimFigure = dynamic_cast<DimFigure*>(findFigureMutable(dimName));
    dimFigure->setOnlyLabel(onlyLabel);
    dimFigure->setShowTolerances(showTols);
    dimFigure->setFreePosition(freePosition);
    emit dimensionParametersChanged(dimName, onlyLabel, showTols, freePosition);
    MacrosManager::log(MacrosManager::ChangeDimensionParameters, {
        { "dimName", dimName },
        { "onlyLabel", onlyLabel == true ? "true" : "false" },
        { "showTols", showTols == true ? "true" : "false" },
        { "freePosition", freePosition == true ? "true" : "false" } });
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

bool Project::confirmRemoveFigure(const QString &figureName) {
    QMessageBox mBox;
    mBox.setText(QString("Delete %1 and its children?").arg(figureName));
    mBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    int dialogResponse = mBox.exec();
    return dialogResponse == QMessageBox::Ok;
}

const QString Project::currentFigureName() const {
    return _currentFigureName;
}

void Project::requestFigureEditDialog(const QString figureName) {
    emit figureEditDialogRequested(figureName);
}

void Project::safeInsert(QString figureName, Figure *figure, bool needToChangeCurrentFigure) {
    std::lock_guard<std::mutex> lock(mtx);
    if(containsFigure(figureName)) {
        MacrosManager::executeWithoutLogging([&]() {
            removeFigure(figureName);
        });
    }
    insertFigure(figure);
    if(needToChangeCurrentFigure) {
        changeCurrentFigure(figureName);
    }
}

void Project::changeCurvePoints(QString curveName, QVector<CurvePoint> newPoints) {
    auto figure = findFigureMutable(curveName);
    if(auto curve = dynamic_cast<CurveFigure*>(figure)) {
        curve->changePoints(newPoints);
        emit curvePointsChanged(curveName);
    }
}

void Project::changeFigureColor(const QString figureName, QColor color) {
    ARGUMENT_ASSERT(containsFigure(figureName), "Change Figure Color: figure`s not found");
    auto figure = findFigureMutable(figureName);
    figure->setColor(color);
    MacrosManager::log(MacrosManager::ChangeFigureColor, {
        { "figureName", figureName },
        { "color", QString::number(ColorTranslator::getIntFromColor(&color)) } });
    emit figureColorChanged(figureName);
}

void Project::changeScale(double scaleFactor, const Point center) {
    _scaleFactor = scaleFactor;
    _centerPoint = center;
    emit scaleChanged(scaleFactor, center);
}

void Project::clear() {
    MacrosManager::executeWithoutLogging([&]() {
        for(auto name : _figures.keys()) {
            removeFigure(name);
        }
        changePartData(QString(), QString(), QString(), QString(), QString(), QString(),
            QString(), QString(), QString(), QString(), QString(), QString(), QString(), false);
    });
    MacrosManager::log(MacrosManager::ClearProject);
}

void Project::shiftFigure(QString figureName, double x, double y, double z) {
    auto figure = findFigureMutable(figureName);
    figure->shift(x, y, z);
    MacrosManager::log(MacrosManager::ShiftFigure, {
        { "figureName", figureName },
        { "x", QString::number(x) },
        { "y", QString::number(y) },
        { "z", QString::number(z) },
        });
    emit figureCoordsChanged(figureName);
}

void Project::shiftFigure(QString figureName, QString x, QString y, QString z) {
    auto shiftX = 0.0;
    if(x.toDouble()) {
        shiftX = x.toDouble();
    } else if(!x.toDouble()) {
        auto xSourceFigureNames = x.split(".X-");
        if(xSourceFigureNames.length() == 2) {
            auto firstXSourceFigureName = xSourceFigureNames.at(0);
            auto secondXSourceFigureName = xSourceFigureNames.at(1);
            secondXSourceFigureName = secondXSourceFigureName.left(secondXSourceFigureName.length() - 2);
            if(auto firstFigure = dynamic_cast<const PointFigure*>(findFigure(firstXSourceFigureName))) {
                if(auto secondFigure = dynamic_cast<const PointFigure*>(findFigure(secondXSourceFigureName))) {
                    shiftX = firstFigure->point().x - secondFigure->point().x;
                }
            }
        }
    }

    auto shiftY = 0.0;
    if(y.toDouble()) {
        shiftY = y.toDouble();
    } else if(!y.toDouble()) {
        auto ySourceFigureNames = y.split(".Y-");
        if(ySourceFigureNames.length() == 2) {
            auto firstYSourceFigureName = ySourceFigureNames.at(0);
            auto secondYSourceFigureName = ySourceFigureNames.at(1);
            secondYSourceFigureName = secondYSourceFigureName.left(secondYSourceFigureName.length() - 2);
            if(auto firstFigure = dynamic_cast<const PointFigure*>(findFigure(firstYSourceFigureName))) {
                if(auto secondFigure = dynamic_cast<const PointFigure*>(findFigure(secondYSourceFigureName))) {
                    shiftY = firstFigure->point().y - secondFigure->point().y;
                }
            }
        }
    }

    auto shiftZ = 0.0;
    if(z.toDouble()) {
        shiftZ = z.toDouble();
    } else if(!z.toDouble()) {
        auto zSourceFigureNames = z.split(".Z-");
        if(zSourceFigureNames.length() == 2) {
            auto firstZSourceFigureName = zSourceFigureNames.at(0);
            auto secondZSourceFigureName = zSourceFigureNames.at(1);
            secondZSourceFigureName = secondZSourceFigureName.left(secondZSourceFigureName.length() - 2);
            if(auto firstFigure = dynamic_cast<const PointFigure*>(findFigure(firstZSourceFigureName))) {
                if(auto secondFigure = dynamic_cast<const PointFigure*>(findFigure(secondZSourceFigureName))) {
                    shiftZ = firstFigure->point().z - secondFigure->point().z;
                }
            }
        }
    }

    auto figure = findFigureMutable(figureName);
    figure->shift(shiftX, shiftY, shiftZ);
    MacrosManager::log(MacrosManager::ShiftFigure, {
        { "figureName", figureName },
        { "x", x },
        { "y", y },
        { "z", z },
        });
    emit figureCoordsChanged(figureName);
}

void Project::rotateFigure(QString figureName, double angle, double x, double y, double z) {
    auto figure = findFigureMutable(figureName);
    figure->rotate(angle, x, y, z);
    MacrosManager::log(MacrosManager::RotateFigure, {
        { "figureName", figureName },
        { "angle", QString::number(angle) },
        { "x", QString::number(x) },
        { "y", QString::number(y) },
        { "z", QString::number(z) },
        });
    emit figureCoordsChanged(figureName);
}

void Project::rotateFigure(QString figureName, double angle, QString x, QString y, QString z) {
    auto rotateX = 0.0;
    if(x.toDouble()) {
        rotateX = x.toDouble();
    } else if(!x.toDouble()) {
        auto xSourceFigureName = x.left(x.length() - 2);
        if(auto pointFigure = dynamic_cast<const PointFigure*>(findFigure(xSourceFigureName))) {
            rotateX = pointFigure->point().x;
        } else if(auto circleFigure = dynamic_cast<const CircleFigure*>(findFigure(xSourceFigureName))) {
            rotateX = circleFigure->center().x;
        }
    }

    auto rotateY = 0.0;
    if(y.toDouble()) {
        rotateY = y.toDouble();
    } else if(!y.toDouble()) {
        auto ySourceFigureName = y.left(y.length() - 2);
        if(auto pointFigure = dynamic_cast<const PointFigure*>(findFigure(ySourceFigureName))) {
            rotateY = pointFigure->point().y;
        } else if(auto circleFigure = dynamic_cast<const CircleFigure*>(findFigure(ySourceFigureName))) {
            rotateY = circleFigure->center().y;
        }
    }

    auto rotateZ = 0.0;
    if(z.toDouble()) {
        rotateZ = z.toDouble();
    } else if(!z.toDouble()) {
        auto zSourceFigureName = z.left(z.length() - 2);
        if(auto pointFigure = dynamic_cast<const PointFigure*>(findFigure(zSourceFigureName))) {
            rotateZ = pointFigure->point().z;
        } else if(auto circleFigure = dynamic_cast<const CircleFigure*>(findFigure(zSourceFigureName))) {
            rotateZ = circleFigure->center().z;
        }
    }


    auto figure = findFigureMutable(figureName);
    figure->rotate(angle, rotateX, rotateY, rotateZ);

    MacrosManager::log(MacrosManager::RotateFigure, {
        { "figureName", figureName },
        { "angle", QString::number(angle) },
        { "x", x },
        { "y", y },
        { "z", z },
        });
    emit figureCoordsChanged(figureName);
}

void Project::alignment(double angle, double offsetX, double offsetY) {
    QThread* thread = QThread::create([&]() {
        auto itFirstThread = _figures.begin();
        for(; itFirstThread != std::next(_figures.begin(), _figures.size() / 2); ++itFirstThread) {
            std::future<void> future = std::async(std::launch::async, &Figure::alignment, *itFirstThread, angle, offsetX, offsetY);
            emit figureCoordsChanged((*itFirstThread)->name());
        }
    });
    connect(thread, &QThread::finished, thread, &QObject::deleteLater);
    thread->start();

    auto itMainThread = std::next(_figures.begin(), _figures.size() / 2);
    for(; itMainThread != _figures.end(); ++itMainThread) {
        std::future<void> future = std::async(std::launch::async, &Figure::alignment, *itMainThread, angle, offsetX, offsetY);
        emit figureCoordsChanged((*itMainThread)->name());
    }
    MacrosManager::log(MacrosManager::Alignment, {
        { "angle", QString::number(angle) },
        { "axis", "" },
        { "offsetX", QString::number(offsetX) },
        { "offsetY", QString::number(offsetY) },
        });
}

void Project::alignment(QString angle, QString axis, QString offsetX, QString offsetY) {
    auto alignmentAngle = 0.0;
    if(angle.toDouble()) {
        alignmentAngle = angle.toDouble();
    } else if(!angle.toDouble()) {
        if(auto line = dynamic_cast<const LineFigure*>(findFigure(angle))) {
            Eigen::Vector2d lineDir(line->direction().x, line->direction().y);
            Eigen::Vector2d alignmentAxis;
            int sign = 1;
            if(axis == "+X") {
                alignmentAxis << 1, 0;
                if(lineDir.y() > 0) {
                    sign = -1;
                }
            } else if(axis == "-X") {
                alignmentAxis << -1, 0;
                if(lineDir.y() < 0) {
                    sign = -1;
                }
            } else if(axis == "+Y") {
                alignmentAxis << 0, 1;
                if(lineDir.x() < 0) {
                    sign = -1;
                }
            } else if(axis == "-Y") {
                alignmentAxis << 0, -1;
                if(lineDir.x() > 0) {
                    sign = -1;
                }
            }
            auto cos = alignmentAxis.dot(lineDir) / (alignmentAxis.norm() * lineDir.norm());
            auto angle = sign * std::acos(cos) * 180 / M_PI;

            alignmentAngle = angle;
        }
    }

    auto alignmentOffsetX = 0.0;
    if(offsetX.toDouble()) {
        alignmentOffsetX = offsetX.toDouble();
    } else if(!offsetX.toDouble()) {
        if(auto pointFigure = dynamic_cast<const PointFigure*>(findFigure(offsetX))) {
            alignmentOffsetX = -pointFigure->point().x;
        } else if(auto circleFigure = dynamic_cast<const CircleFigure*>(findFigure(offsetX))) {
            alignmentOffsetX = -circleFigure->center().x;
        } else if(auto lineFigure = dynamic_cast<const LineFigure*>(findFigure(offsetX))) {
            alignmentOffsetX = -lineFigure->origin().x;
        }
    }

    auto alignmentOffsetY = 0.0;
    if(offsetY.toDouble()) {
        alignmentOffsetY = offsetY.toDouble();
    } else if(!offsetY.toDouble()) {
        if(auto pointFigure = dynamic_cast<const PointFigure*>(findFigure(offsetY))) {
            alignmentOffsetY = -pointFigure->point().y;
        } else if(auto circleFigure = dynamic_cast<const CircleFigure*>(findFigure(offsetY))) {
            alignmentOffsetY = -circleFigure->center().y;
        } else if(auto lineFigure = dynamic_cast<const LineFigure*>(findFigure(offsetY))) {
            alignmentOffsetY = -lineFigure->origin().y;
        }
    }

    QThread* thread = QThread::create([&]() {
        auto itFirstThread = _figures.begin();
        std::future<void> future;
        std::vector<std::future<void>> futures;
        for(; itFirstThread != std::next(_figures.begin(), _figures.size() / 2); ++itFirstThread) {
            future = std::async(std::launch::async, &Figure::alignment, *itFirstThread, alignmentAngle, alignmentOffsetX, alignmentOffsetY);
            emit figureCoordsChanged((*itFirstThread)->name());
            futures.push_back(std::move(future));
        }
        for(int i = 0; i < futures.size(); i++) {
            std::future<void> &itFuture = futures[i];
            itFuture.wait();
        }
    });
    connect(thread, &QThread::finished, thread, &QObject::deleteLater);
    thread->start();

    auto itMainThread = std::next(_figures.begin(), _figures.size() / 2);
    std::future<void> future;
    std::vector<std::future<void>> futures;
    for(; itMainThread != _figures.end(); ++itMainThread) {
        future = std::async(std::launch::async, &Figure::alignment, *itMainThread, alignmentAngle, alignmentOffsetX, alignmentOffsetY);
        emit figureCoordsChanged((*itMainThread)->name());
        futures.push_back(std::move(future));
    }
    for(int i = 0; i < futures.size(); i++) {
        std::future<void> &itFuture = futures[i];
        itFuture.wait();
    }

    MacrosManager::log(MacrosManager::Alignment, {
        { "angle", angle },
        { "axis", axis },
        { "offsetX", offsetX },
        { "offsetY", offsetY },
        });
}

void Project::changeDimensionValue(const QString &dimName, const DimFigure::Value &value) {
    auto figure = findFigureMutable(dimName);
    if(auto dimFigure = dynamic_cast<DimFigure*>(figure)) {
        dimFigure->addValue(value);
    }
    emit dimensionValueChanged(dimName, value);
}
const QString Project::reportTitle() {
    return _reportTitle;
}
const QString Project::description() {
    return _description;
}
const QString Project::drawing() {
    return _drawing;
}
const QString Project::orderNumber() {
    return _orderNumber;
}
const QString Project::partNumber() {
    return _partNumber;
}
const QString Project::projectOperator() {
    return _projectOperator;
}
const QString Project::note() {
    return _note;
}

const QString Project::machine() {
    return _machine;
}
const QString Project::tool() {
    return _tool;
}
const QString Project::fixturing() {
    return _fixturing;
}
const QString Project::batch() {
    return _batch;
}
const QString Project::supplier() {
    return _supplier;
}
const QString Project::revision() {
    return _revision;
}
void Project::changePartData(QString reportTitle, QString description, QString drawing, QString orderNumber, QString partNumber, QString projectOperator, QString note,
    QString machine, QString tool, QString fixturing, QString batch, QString supplier, QString revision, bool needShowWindowWhenMacroRuns) {

    _reportTitle = txtFigureToText(reportTitle);
    _description = txtFigureToText(description);
    _drawing = txtFigureToText(drawing);
    _orderNumber = txtFigureToText(orderNumber);
    _partNumber = txtFigureToText(partNumber);
    _projectOperator = txtFigureToText(projectOperator);
    _note = txtFigureToText(note);
    _machine = txtFigureToText(machine);
    _tool = txtFigureToText(tool);
    _fixturing = txtFigureToText(fixturing);
    _batch = txtFigureToText(batch);
    _supplier = txtFigureToText(supplier);
    _revision = txtFigureToText(revision);

    MacrosManager::log(MacrosManager::PartData, {
        { "reportTitle", reportTitle },
        { "description", description },
        { "drawing", drawing },
        { "orderNumber", orderNumber },
        { "partNumber", partNumber },
        { "projectOperator", projectOperator },
        { "note", note },
        { "machine", machine },
        { "tool", tool },
        { "fixturing", fixturing },
        { "batch", batch },
        { "supplier", supplier },
        { "revision", revision },
        { "showPartDataWindowWhenMacroRuns", needShowWindowWhenMacroRuns == true ? "Yes" : "No" }
        });
}

void Project::constructText(QString name, QString text, double x, double y, double textSize, QString reference,
    double imageWidth, double imageHeight, double imageZoom) {
    auto textFigure = new TextFigure(name, text, Point(x, y, 0), textSize, reference, imageWidth, imageHeight, imageZoom);
    safeInsert(name, textFigure);
    MacrosManager::log(MacrosManager::InsertText, {
        { "name", name },
        { "text", text },
        { "x", QString::number(x) },
        { "y", QString::number(y) },
        { "textSize", QString::number(textSize) },
        { "reference", reference },
        { "imageWidth", QString::number(imageWidth) },
        { "imageHeight", QString::number(imageHeight) },
        { "imageZoom", QString::number(imageZoom) },
        });
}

Point Project::chooseCoordsByClick() {
    //emit controlWidgetsEnabled(false);
    //emit raiseMainWindow();
    //emit trackMousePositionRequested();
    //QEventLoop loop;
    //connect(this, &Project::mousePressed, &loop, &QEventLoop::quit);
    //loop.exec();
    //return Point(_mousePos.x, _mousePos.y, 0);
    return Point(0, 0);
}

void Project::addOperationtime(QString operation, quint64 time) {
    operationTimes[operation].append(time);
}

QString Project::txtFigureToText(QString txtFigure) const {
    if(txtFigure.left(4) == ("TXT:")) {
        auto figure = findFigure(txtFigure.sliced(4));
        if(dynamic_cast<const TextFigure*>(figure)) {
            return dynamic_cast<const TextFigure*>(figure)->text();
        } else {
            return txtFigure;
        }
    } else {
        return txtFigure;
    }
}

QString Project::getFreeName(QString startsWith, bool firstWithNumber, QString separator) {
    if(!firstWithNumber && !figures().contains(startsWith)) {
        return startsWith;
    }
    auto index = 1;
    auto flag = true;
    while(flag) {
        flag = false;
        auto newName = startsWith + separator + QString::number(index);
        if(_figures.contains(newName)) {
            index++;
            flag = true;
            break;
        }
        if(!flag) {
            return startsWith + QString::number(index);
        }
    }
    return QString();
}
