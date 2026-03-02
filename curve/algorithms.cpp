#include "curve/pch.h"
#include "algorithms.h"

void Algorithms::enlargeCurveWithIntermediatePoints(QString figureName, const Function1Params *params, Project *project) {
    auto figure = project->findFigure(figureName);
    auto curve = dynamic_cast<const CurveFigure*>(figure);

    ARGUMENT_ASSERT(curve, "Enlarging with intermediate points: curve`s not found");

    auto result = CurveMachine::enlargeCurveWithIntermediatePoints(curve->points(), *params);
    auto newName = figureName + "_enlarged";
    auto newCurve = new CurveFigure(newName, result.points());
    project->safeInsert(newName, newCurve);
}

void Algorithms::calculateCurve(QString figureName, QString newFigureName, const Function1Params *params, Project *project) {
    auto figure = project->findFigure(figureName);
    auto curve = dynamic_cast<const CurveFigure*>(figure);

    ARGUMENT_ASSERT(curve, "Calculate curve: curve`s not found");

    auto result = CurveMachine::enlargeCurveWithIntermediatePoints(curve->points(), *params);
    auto resultCurve = new CurveFigure;
    *resultCurve = *curve;
    resultCurve->changePoints(result.points());
    resultCurve->setName(newFigureName);
    project->safeInsert(newFigureName, resultCurve);

    auto log = QMap<QString, QString>(const_cast<Function1Params*>(params)->toQMap());
    log.insert({ { "figureName", figureName }, { "newFigureName", newFigureName } });
    MacrosManager::log(MacrosManager::CalculateCurve, log);
}

void Algorithms::regenerateCurve(QString figureName, QString newFigureName, const Function19Params *params, Project *project) {
    auto figure = project->findFigure(figureName);
    auto curve = dynamic_cast<const CurveFigure*>(figure);

    ARGUMENT_ASSERT(curve, "Regenerate curve: curve`s not found");

    auto result = CurveMachine::regenerateCurve(curve->points(), *params);
    auto resultCurve = new CurveFigure;
    *resultCurve = *curve;
    resultCurve->changePoints(result.points());
    resultCurve->setName(newFigureName);
    project->safeInsert(newFigureName, resultCurve);

    auto log = QMap<QString, QString>(const_cast<Function19Params*>(params)->toQMap());
    log.insert({ { "figureName", figureName }, { "newFigureName", newFigureName } });
    MacrosManager::log(MacrosManager::RegenerateCurve, log);
}

void Algorithms::createMiddleCurve(QString parentName, QString figureName, const Function18Params *params, Project *project, QColor color) {
    auto figure = project->findFigure(parentName);
    auto curve = dynamic_cast<const CurveFigure*>(figure);

    ARGUMENT_ASSERT(curve, "Create middle curve: curve`s not found");

    auto result = CurveMachine::getMiddleCurve(curve->points(), *params);
    auto middleCurve = new CurveFigure(figureName, result.points());
    middleCurve->setColor(color);
    project->safeInsert(figureName, middleCurve);

    auto log = QMap<QString, QString>(const_cast<Function18Params*>(params)->toQMap());
    log.insert({ { "parentName", parentName }, { "figureName", figureName } });
    MacrosManager::log(MacrosManager::CreateMiddleCurve, log);
}

CurveFigure Algorithms::getMiddleCurve(QString figureName, const Function18Params *params, Project *project) {
    auto figure = project->findFigure(figureName);
    auto curve = dynamic_cast<const CurveFigure*>(figure);

    ARGUMENT_ASSERT(curve, "Get middle curve: curve`s not found");

    return CurveMachine::getMiddleCurve(curve->points(), *params);
}

void Algorithms::createContactLine(QString parentName, QString figureName, const Function18Params *params, Project *project, QColor color) {
    auto figure = project->findFigure(parentName);
    auto curve = dynamic_cast<const CurveFigure*>(figure);

    ARGUMENT_ASSERT(curve, "Create contact line: curve`s not found");

    auto result = CurveMachine::getContactLine(curve->points(), *params);
    auto contactLine = new LineFigure(figureName, result.origin(), result.direction(), qInf());
    contactLine->setColor(color);
    project->safeInsert(figureName, contactLine);

    auto log = QMap<QString, QString>(const_cast<Function18Params*>(params)->toQMap());
    log.insert({ { "parentName", parentName }, { "figureName", figureName } });
    MacrosManager::log(MacrosManager::CreateContactLine, log);
}

LineFigure Algorithms::getContactLine(QString figureName, const Function18Params *params, Project *project) {
    auto figure = project->findFigure(figureName);
    auto curve = dynamic_cast<const CurveFigure*>(figure);

    ARGUMENT_ASSERT(curve, "Get contact line: curve`s not found");

    return CurveMachine::getContactLine(curve->points(), *params);
}

double Algorithms::getContactLineLength(QString figureName, const Function18Params *params, Project *project) {
    auto figure = project->findFigure(figureName);
    auto curve = dynamic_cast<const CurveFigure*>(figure);

    ARGUMENT_ASSERT(curve, "Get contact line length: curve`s not found");

    auto contactLineLength = CurveMachine::getContactLineLength(curve->points(), *params);
    return contactLineLength;
}

void Algorithms::createMaxCircle(QString parentName, QString figureName, const Function18Params *params, Project *project, QColor color) {
    auto figure = project->findFigure(parentName);
    auto curve = dynamic_cast<const CurveFigure*>(figure);

    ARGUMENT_ASSERT(curve, "Create max circle: curve`s not found");

    auto result = CurveMachine::getMaxCircle(curve->points(), *params);
    auto circle = new CircleFigure(figureName, result.center(), Point(0, 0, 1), result.radius());
    circle->setColor(color);
    project->safeInsert(figureName, circle);

    auto log = QMap<QString, QString>(const_cast<Function18Params*>(params)->toQMap());
    log.insert({ { "parentName", parentName }, { "figureName", figureName } });
    MacrosManager::log(MacrosManager::CreateMaxCircle, log);
}

CircleFigure Algorithms::getMaxCircle(QString figureName, const Function18Params *params, Project *project) {
    auto figure = project->findFigure(figureName);
    auto curve = dynamic_cast<const CurveFigure*>(figure);

    ARGUMENT_ASSERT(curve, "Max width circle: curve`s not found");

    return CurveMachine::getMaxCircle(curve->points(), *params);
}

double Algorithms::getWidthOfLeadingEdge(QString figureName, Project *project, double distanceFromEdge, bool createSegment) {
    auto figure = project->findFigure(figureName);
    auto curve = dynamic_cast<const CurveFigure*>(figure);

    ARGUMENT_ASSERT(curve, "Width of leading edge: curve`s not found");

    auto points = CurveMachine::getWidthOfLeadingEdge(curve->points(), distanceFromEdge);
    auto widthLE = CurveMachine::getDistanceBetweenPoints(points.first, points.second);

    if(createSegment) {
        /*auto segmentName = figureName + "_LE_width_";
        auto point1 = new PointFigure(project->getFreeName(segmentName), points.first);
        project->safeInsert(point1->name(), point1);
        auto point2 = new PointFigure(project->getFreeName(segmentName), points.second);
        project->safeInsert(point2->name(), point2);

        auto middlePoint = Point((point1->point().x + point2->point().x) / 2, (point1->point().y + point2->point().y) / 2);
        auto value = DimFigure::Value(DimFigure::ValueType::Length);
        value.measurement = widthLE;
        auto dimFigure = new DimFigure(project->getFreeName(segmentName), middlePoint, point1->name(), point2->name());
        dimFigure->setDimType(DimFigure::DimType::DistanceBetweenCurvePoints);
        dimFigure->addValue(value);
        project->safeInsert(dimFigure->name(), dimFigure);*/
    }
    MacrosManager::log(MacrosManager::GetWidthOfLE, {
        { "figureName", figureName },
        { "distanceLE", QString::number(distanceFromEdge) },
        { "createSegmentLE", createSegment ? "true" : "false" }, });
    return widthLE;
}

double Algorithms::getWidthOfTrailingEdge(QString figureName, Project *project, double distanceFromEdge, bool createSegment) {
    auto figure = project->findFigure(figureName);
    auto curve = dynamic_cast<const CurveFigure*>(figure);

    ARGUMENT_ASSERT(curve, "Width of trailing edge: curve`s not found");

    auto points = CurveMachine::getWidthOfTrailingEdge(curve->points(), distanceFromEdge);
    auto widthTE = CurveMachine::getDistanceBetweenPoints(points.first, points.second);

    if(createSegment) {
        /*auto segmentName = figureName + "_TE_width_";
        auto point1 = new PointFigure(project->getFreeName(segmentName), points.first);
        project->safeInsert(point1->name(), point1);
        auto point2 = new PointFigure(project->getFreeName(segmentName), points.second);
        project->safeInsert(point2->name(), point2);

        auto middlePoint = Point((point1->point().x + point2->point().x) / 2, (point1->point().y + point2->point().y) / 2);
        auto value = DimFigure::Value(DimFigure::ValueType::Length);
        value.measurement = widthTE;
        auto dimFigure = new DimFigure(project->getFreeName(segmentName), middlePoint, point1->name(), point2->name());
        dimFigure->setDimType(DimFigure::DimType::DistanceBetweenCurvePoints);
        dimFigure->addValue(value);
        project->safeInsert(dimFigure->name(), dimFigure);*/
    }
    MacrosManager::log(MacrosManager::GetWidthOfTE, {
        { "figureName", figureName },
        { "distanceTE", QString::number(distanceFromEdge) },
        { "createSegmentTE", createSegment ? "true" : "false" } });
    return widthTE;
}

double Algorithms::getRadiusOfLeadingEdge(QString figureName, const Function18Params *params, Project *project) {
    auto figure = project->findFigure(figureName);
    auto curve = dynamic_cast<const CurveFigure*>(figure);

    ARGUMENT_ASSERT(curve, "Radius of leading edge: curve`s not found");

    return CurveMachine::getRadiusOfLeadingEdge(curve->points(), *params);
}

double Algorithms::getRadiusOfTrailingEdge(QString figureName, const Function18Params * params, Project * project) {
    auto figure = project->findFigure(figureName);
    auto curve = dynamic_cast<const CurveFigure*>(figure);

    ARGUMENT_ASSERT(curve, "Radius of trailing edge: curve`s not found");

    return CurveMachine::getRadiusOfTrailingEdge(curve->points(), *params);
}

double Algorithms::getMinX(QString figureName, const Function18Params *params, Project *project) {
    auto figure = project->findFigure(figureName);
    auto curve = dynamic_cast<const CurveFigure*>(figure);

    ARGUMENT_ASSERT(curve, "Get MinX: curve`s not found");
    return CurveMachine::getMinX(curve->points(), *params);
}

double Algorithms::createMinX(QString figureName, const Function18Params *params, Project *project, QString dimName, QColor color) {
    auto figure = project->findFigure(figureName);
    auto curve = dynamic_cast<const CurveFigure*>(figure);

    ARGUMENT_ASSERT(curve, "Create MinX: curve`s not found");
    auto minXPoint = CurvePoint(CurveMachine::getMinX(curve->points(), *params), 0, 0);

    auto pointName = project->getFreeName(figureName + "_PMinX", false);
    auto result = new PointFigure(pointName, minXPoint);
    result->setColor(color);
    project->safeInsert(pointName, result);

    auto value = DimFigure::Value(DimFigure::ValueType::X);
    value.measurement = minXPoint.x;
    auto dimFigure = new DimFigure(project->getFreeName(dimName), Point(0, 0, 0), result->name());
    dimFigure->setDimType(DimFigure::DimType::Position); // TODO: add plot for this dimtype
    dimFigure->addValue(value);
    project->safeInsert(dimFigure->name(), dimFigure);

    return minXPoint.x;
}

void Algorithms::makeRadiusCorrection(QString figureName, QString figureNewName, const Function3Params *params, Project *project) {
    auto figure = project->findFigure(figureName);
    auto curve = dynamic_cast<const CurveFigure*>(figure);

    ARGUMENT_ASSERT(curve, "Radius correction: curve`s not found");

    auto result = CurveMachine::offsetCurve(curve->points(), *params);
    auto resultCurve = new CurveFigure;
    *resultCurve = *curve;
    resultCurve->changePoints(result.points());
    resultCurve->setName(figureNewName);
    project->safeInsert(figureNewName, resultCurve);

    auto log = QMap<QString, QString>(const_cast<Function3Params*>(params)->toQMap());
    log.insert({ { "figureName", figureName }, { "newName", figureNewName } });
    MacrosManager::log(MacrosManager::RadiusCorrection, log);
}

bool Algorithms::tryMergePointClouds(QString firstCurveName, QString secondCurveName, QString resultName, double threshold, bool needSorted, Project *project) {
    auto firstCloud = project->findFigure(firstCurveName);
    auto secondCloud = project->findFigure(secondCurveName);
    auto firstCurve = dynamic_cast<const CurveFigure*>(firstCloud);
    auto secondCurve = dynamic_cast<const CurveFigure*>(secondCloud);

    ARGUMENT_ASSERT(firstCurve && secondCurve, "Merge curves: curve`s not found");

    auto result = CurveMachine::mergePointClouds(firstCurve->points(), secondCurve->points(), threshold, needSorted);
    if(result.length() == 0) {
        return false;
    }
    auto curve = new CurveFigure(resultName, result);
    project->safeInsert(resultName, curve);
    MacrosManager::executeWithoutLogging([&]() {
        project->removeFigure(firstCurveName);
        project->removeFigure(secondCurveName);
    });

    MacrosManager::log(MacrosManager::MergeScans, {
        { "firstName", firstCurveName },
        { "secondName", secondCurveName },
        { "resultName", resultName },
        { "threshold", QString::number(threshold) },
        { "needSorted", needSorted ? "true" : "false" } });
    return true;
}

void Algorithms::calculateDeviations(QString nomCurveName, QString measCurveName, QString resultCurveName, const Function4Params *params, Project *project) {
    auto measFigure = project->findFigure(measCurveName);
    auto nomFigure = project->findFigure(nomCurveName);
    auto measCurve = dynamic_cast<const CurveFigure*>(measFigure);
    auto nomCurve = dynamic_cast<const CurveFigure*>(nomFigure);

    ARGUMENT_ASSERT(measCurve && nomFigure, "Calculate deviations: curve`s not found");

    auto result = CurveMachine::calculateDeviations(nomCurve->points(), measCurve->points(), *params);
    //for(auto point : result.points()) {
    //    qDebug() << point.dev;
    //}

    auto resultCurve = new CurveFigure;
    *resultCurve = *nomCurve;
    resultCurve->changePoints(result.points());
    resultCurve->setName(resultCurveName);
    resultCurve->setShowDeviations(true);
    resultCurve->setConnectDeviations(true);
    project->safeInsert(resultCurveName, resultCurve);

    auto log = QMap<QString, QString>(const_cast<Function4Params*>(params)->toQMap());
    log.insert({ { "nominal", nomCurveName }, { "measured", measCurveName }, { "resultName", resultCurveName } });
    MacrosManager::log(MacrosManager::CalculateDeviations, log);
}

void Algorithms::calculateBestFit(QString nomCurveName, QString measCurveName, QString resultCurveName, QString bestFitLineName, const Function6Params *params, Project *project) {
    auto nominalFigure = project->findFigure(nomCurveName);
    auto measuredFigure = project->findFigure(measCurveName);
    auto nominalCurve = dynamic_cast<const CurveFigure*>(nominalFigure);
    auto measuredCurve = dynamic_cast<const CurveFigure*>(measuredFigure);

    ARGUMENT_ASSERT(nominalCurve && measuredCurve, "Calculate best-fit (6): curve`s not found");

    auto result = CurveMachine::calculateBestFit(nominalCurve->points(), measuredCurve->points(), *params);

    auto resultCurve = new CurveFigure;
    resultCurve->changePoints(result.curve.points());
    resultCurve->setName(resultCurveName);
    if(measuredCurve->isClosed()) {
        resultCurve->setClosed(true);
    }
    project->safeInsert(resultCurveName, resultCurve);

    auto linePoint = Point(result.offsetX, result.offsetY);
    auto direction = CurveMachine::getDirection(linePoint, result.rotation);
    auto bestFitLine = new LineFigure(bestFitLineName, linePoint, direction, qInf());
    bestFitLine->setVisible(false);
    project->safeInsert(bestFitLineName, bestFitLine, false);

    auto log = QMap<QString, QString>(const_cast<Function6Params*>(params)->toQMap());
    log.insert({ { "nominal", nomCurveName }, { "measured", measCurveName }, { "resultName", resultCurveName }, { "bestFitLineName", bestFitLineName } });
    MacrosManager::log(MacrosManager::BestFit, log);
}

void Algorithms::calculateBestFit(QString nomCurveName, QString measCurveName, QString resultCurveName, QString bestFitLineName, const Function21Params *params, Project *project) {
    auto nominalFigure = project->findFigure(nomCurveName);
    auto measuredFigure = project->findFigure(measCurveName);
    auto nominalCurve = dynamic_cast<const CurveFigure*>(nominalFigure);
    auto measuredCurve = dynamic_cast<const CurveFigure*>(measuredFigure);

    ARGUMENT_ASSERT(nominalCurve && measuredCurve, "Calculate best-fit (21): curve`s not found");

    auto result = CurveMachine::calculateBestFit(nominalCurve->points(), measuredCurve->points(), *params);

    auto resultCurve = new CurveFigure;
    resultCurve->changePoints(result.curve.points());
    resultCurve->setName(resultCurveName);
    if(measuredCurve->isClosed()) {
        resultCurve->setClosed(true);
    }
    project->safeInsert(resultCurveName, resultCurve);

    auto linePoint = Point(result.offsetX, result.offsetY);
    auto direction = CurveMachine::getDirection(linePoint, result.rotation);
    auto bestFitLine = new LineFigure(bestFitLineName, linePoint, direction, qInf());
    bestFitLine->setVisible(false);
    project->safeInsert(bestFitLineName, bestFitLine, false);
}

void Algorithms::calculateConstantTolerances(QString figureName, double upperTolerance, double lowerTolerance, Project *project) {
    auto figure = project->findFigure(figureName);
    auto curve = dynamic_cast<const CurveFigure*>(figure);

    ARGUMENT_ASSERT(curve, "Calculate constant tolerances: curve not found");

    auto curveWithTolerances = *curve;
    curveWithTolerances.assignToleranceToSegment(upperTolerance, lowerTolerance);

    auto f1params = Function1Params(0, 0, 0, true, true, FunctionParams::Direction::Left, true);
    auto f1result = CurveLibrary::function1(curveWithTolerances.points(), f1params);

    project->changeCurveTolerance(figureName, f1result.curve.points());

    MacrosManager::log(MacrosManager::ConstantTolerance, {
        { "figureName", figureName },
        { "upperTolerance", QString::number(upperTolerance) },
        { "lowerTolerance", QString::number(lowerTolerance) } });
}

void Algorithms::calculateEdgesTolerance(QString figureName, int leadingEdgeDirection, double lEPercent, double tEPercent, double lEUpper, double lELower,
    double tEUpper, double tELower, double highEUpper, double highELower, double lowEUpper, double lowELower, Project *project) {
    auto figure = project->findFigure(figureName);
    auto curve = dynamic_cast<const CurveFigure*>(figure);

    ARGUMENT_ASSERT(curve, "Calculate constant tolerances: curve not found");

    auto const f18params = Function18Params(leadingEdgeDirection, lEPercent, tEPercent);
    auto f18result = CurveLibrary::function18(curve->points(), f18params);

    f18result.curveLE.assignToleranceToSegment(lEUpper, lELower);
    f18result.curveTE.assignToleranceToSegment(tEUpper, tELower);
    f18result.curveHigh.assignToleranceToSegment(highEUpper, highELower);
    f18result.curveLow.assignToleranceToSegment(lowEUpper, lowELower);

    QVector<CurvePoint> pointsWithTolerances;
    if(f18result.curveLE.points().length() + f18result.curveHigh.points().length() +
        f18result.curveTE.points().length() + f18result.curveLow.points().length() == curve->points().length() + 8) {
        auto curveLEPoints = f18result.curveLE.points();
        curveLEPoints.pop_back();
        curveLEPoints.pop_front();
        auto curveTEPoints = f18result.curveTE.points();
        curveTEPoints.pop_back();
        curveTEPoints.pop_front();
        auto curveHighPoints = f18result.curveHigh.points();
        curveHighPoints.pop_back();
        curveHighPoints.pop_front();
        auto curveLowPoints = f18result.curveLow.points();
        curveLowPoints.pop_back();
        curveLowPoints.pop_front();

        pointsWithTolerances = curveLEPoints + curveLowPoints + curveTEPoints + curveHighPoints;
    } else {
        pointsWithTolerances = f18result.curveLE.points() + f18result.curveLow.points() +
            f18result.curveTE.points() + f18result.curveHigh.points();
    }

    auto f1params = Function1Params(0, 0, 0, true, true, FunctionParams::Direction::Left, false);
    auto f1result = CurveLibrary::function1(pointsWithTolerances, f1params);

    project->changeCurveTolerance(figureName, f1result.curve.points());

    MacrosManager::log(MacrosManager::EdgesTolerance, {
        { "figureName", figureName },
        { "leadingEdgeDirection", QString::number(leadingEdgeDirection) },
        { "leadingEdgePercent", QString::number(lEPercent) },
        { "trailingEdgePercent", QString::number(tEPercent) },
        { "leadingEdgeUpperTolerance", QString::number(lEUpper) },
        { "leadingEdgeLowerTolerance", QString::number(lELower) },
        { "trailingEdgeUpperTolerance", QString::number(tEUpper) },
        { "trailingEdgeLowerTolerance", QString::number(tELower) },
        { "highEdgeUpperTolerance", QString::number(highEUpper) },
        { "highEdgeLowerTolerance", QString::number(highELower) },
        { "lowEdgeUpperTolerance", QString::number(lowEUpper) },
        { "lowEdgeLowerTolerance", QString::number(lowELower) }, });
}

void Algorithms::insertBestFitDimension(const QString &figureName, const QString &parentName, double x, double y, double z, bool isShowX, bool isShowY, bool isShowR, Project *project) {
    auto labelPoint = Point(x, y, z);
    auto dimension = new DimFigure(figureName, labelPoint);

    auto figure = project->findFigure(parentName);
    auto bestFitLine = dynamic_cast<const LineFigure*>(figure);
    ARGUMENT_ASSERT(bestFitLine, "Insert best fit dimension: best fit line not found");
    auto dx = bestFitLine->origin().x;
    auto dy = bestFitLine->origin().y;
    auto angularCoeff = bestFitLine->direction().y / bestFitLine->direction().x;

    auto xShift = DimFigure::Value(DimFigure::ValueType::X, isShowX, dx);
    auto yShift = DimFigure::Value(DimFigure::ValueType::Y, isShowY, dy);
    auto rotation = DimFigure::Value(DimFigure::ValueType::Rotation, isShowR, angularCoeff);
    dimension->addValues({ xShift, yShift, rotation });
    dimension->setDimType(DimFigure::DimType::BestFitData);
    dimension->setFirstReference(parentName);

    project->safeInsert(figureName, dimension, false);
}

CurveAnalyzer::CurveParts Algorithms::divideCurveIntoParts(const QString &figureName, const Function18Params *params, Project *project) {
    auto figure = project->findFigure(figureName);
    auto curve = dynamic_cast<const CurveFigure*>(figure);

    ARGUMENT_ASSERT(curve, "Divide curve into parts: best fit line not found");

    return CurveMachine::divideCurveIntoParts(curve->points(), *params);
}

void Algorithms::calculateStretch(const QString &nomCurveName, const QString &measCurveName, const QString &resultCurveName, const Function31Params *params31, const Function6Params *params6, Project *project) {
    auto nominalFigure = project->findFigure(nomCurveName);
    auto nominalCurve = dynamic_cast<const CurveFigure*>(nominalFigure);
    auto measuredFigure = project->findFigure(measCurveName);
    auto measuredCurve = dynamic_cast<const CurveFigure*>(measuredFigure);

    ARGUMENT_ASSERT(nominalCurve && measuredCurve, "Calculate stretch: best fit line not found");

    auto nominalPoints = nominalCurve->points();
    auto measuredPoints = measuredCurve->points();
    auto stretchPoints = CurveMachine::calculateStretch(nominalPoints, measuredPoints, *params31);
    auto bestFitPoints = CurveMachine::calculateBestFit(measuredPoints, stretchPoints, *params6).curve.points();

    auto resultCurve = new CurveFigure(resultCurveName, bestFitPoints);
    project->safeInsert(resultCurveName, resultCurve);
}

void Algorithms::calculateCurveUsing3DVectors(const QString &nomCurveName, const QString &measCurveName, const QString & resultCurveName, const Function42Params *params, Project *project) {
    auto nominalFigure = project->findFigure(nomCurveName);
    auto nominalCurve = dynamic_cast<const CurveFigure*>(nominalFigure);
    auto measuredFigure = project->findFigure(measCurveName);
    auto measuredCurve = dynamic_cast<const CurveFigure*>(measuredFigure);

    ARGUMENT_ASSERT(nominalCurve && measuredCurve, "Calculate curve using 3D vectors: best fit line not found");

    auto nominalPoints = nominalCurve->points();
    auto measuredPoints = measuredCurve->points();
    auto resultPoints = CurveMachine::calculateCurveUsing3DVectors(nominalPoints, measuredPoints, *params);

    auto resultCurve = new CurveFigure(resultCurveName, resultPoints);
    project->safeInsert(resultCurveName, resultCurve);
}

void Algorithms::calculateMeasuredParams(Project *project, std::shared_ptr<ReportSettings> reportSettings, const QString &globalMeasName) {
    auto &turbineParams = reportSettings->turbineParameters();
    for(auto [type, paramList] : turbineParams.asKeyValueRange()) {
        if(!reportSettings->needCalculateParam(type))
            continue;
        for(auto i = 0; i < paramList.count(); i++) {
            try {
                switch(type) {
                    case TurbineParamType::MaxWidth:
                    {
                        auto maxCircle = Algorithms::getMaxCircle(globalMeasName, new Function18Params(), project);
                        paramList[i].measured = maxCircle.radius() * 2;
                        break;
                    }
                    case TurbineParamType::MaxWidthX:
                    {
                        auto maxCircle = Algorithms::getMaxCircle(globalMeasName, new Function18Params(), project);
                        paramList[i].measured = maxCircle.center().x;
                        break;
                    }
                    case TurbineParamType::MaxWidthY:
                    {
                        auto maxCircle = Algorithms::getMaxCircle(globalMeasName, new Function18Params(), project);
                        paramList[i].measured = maxCircle.center().y;
                        break;
                    }
                    case TurbineParamType::ContactLineLength:
                    {
                        auto contactLineLength = Algorithms::getContactLineLength(globalMeasName, new Function18Params(), project);
                        paramList[i].measured = contactLineLength;
                        break;
                    }
                    case TurbineParamType::LEWidth:
                    {
                        auto widthOfLE = Algorithms::getWidthOfLeadingEdge(globalMeasName, project, paramList[i].extraParam1.toDouble(), true);
                        paramList[i].measured = widthOfLE;
                        break;
                    }
                    case TurbineParamType::TEWidth:
                    {
                        auto widthOfTE = Algorithms::getWidthOfTrailingEdge(globalMeasName, project, paramList[i].extraParam1.toDouble(), true);
                        paramList[i].measured = widthOfTE;
                        break;
                    }
                    case TurbineParamType::LERadius:
                    {
                        auto radiusLE = Algorithms::getRadiusOfLeadingEdge(globalMeasName, new Function18Params(), project);
                        paramList[i].measured = radiusLE;
                        break;
                    }
                    case TurbineParamType::TERadius:
                    {
                        auto radiusTE = Algorithms::getRadiusOfTrailingEdge(globalMeasName, new Function18Params(), project);
                        paramList[i].measured = radiusTE;
                        break;
                    }
                    case TurbineParamType::MinX:
                    {
                        auto minX = Algorithms::createMinX(globalMeasName, new Function18Params(), project, reportSettings->nominalName() + " MinX");
                        paramList[i].measured = minX;
                        break;
                    }
                }
            } catch(ContinuableException exception) {
                throw exception;
            }
        }
    }
}
