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

    auto &pointsToRegen = curve->points();
    auto regenCurve = CurveMachine::regenerateCurve(pointsToRegen, *params);
    auto &regeneratedPoints = regenCurve.points();

    QVector<CurvePoint> resultPoints;
    for(auto i = 0; i < regeneratedPoints.length(); i++) {
        resultPoints.append(CurvePoint(
            regeneratedPoints[i].x, regeneratedPoints[i].y, pointsToRegen[0].z,
            regeneratedPoints[i].i, regeneratedPoints[i].j, regeneratedPoints[i].k
        ));
    }

    auto resultCurve = new CurveFigure;
    *resultCurve = *curve;
    resultCurve->changePoints(resultPoints);
    resultCurve->setName(newFigureName);
    project->safeInsert(newFigureName, resultCurve);

    auto log = QMap<QString, QString>(const_cast<Function19Params*>(params)->toQMap());
    log.insert({ { "figureName", figureName }, { "newFigureName", newFigureName } });
    MacrosManager::log(MacrosManager::RegenerateCurve, log);
}

void Algorithms::createMiddleCurve(QString parentName, QString figureName, const Function18Params *params, Project *project, QColor color) {
    auto result = getMiddleCurve(parentName, params, project);
    auto middleCurve = new CurveFigure(figureName, result.points());
    middleCurve->setColor(color);
    project->safeInsert(figureName, middleCurve);

    auto log = QMap<QString, QString>(const_cast<Function18Params*>(params)->toQMap());
    log.insert({ { "parentName", parentName }, { "figureName", figureName } });
    MacrosManager::log(MacrosManager::CreateMiddleCurve, log);
}

CurveFigure Algorithms::getMiddleCurve(QString curveName, const Function18Params *params, Project *project) {
    auto figure = project->findFigure(curveName);
    auto curve = dynamic_cast<const CurveFigure*>(figure);

    ARGUMENT_ASSERT(curve, "Get middle curve: curve`s not found");

    return CurveMachine::getMiddleCurve(curve->points(), *params);
}

double Algorithms::getMiddleCurveLength(QString curveName, const Function18Params *params, Project * project) {
    auto figure = project->findFigure(curveName);
    auto curve = dynamic_cast<const CurveFigure*>(figure);

    ARGUMENT_ASSERT(curve, "Get middle curve: curve`s not found");

    return CurveMachine::getMiddleCurveLength(curve->points(), *params);
}

void Algorithms::createContactLine(QString parentName, QString figureName, const Function18Params *params, Project *project, QColor color) {
    auto contactLine = getContactLine(parentName, params, project);
    auto lineFigure = new LineFigure(figureName, contactLine.origin(), contactLine.direction(), qInf());
    lineFigure->setColor(color);
    project->safeInsert(figureName, lineFigure);

    auto log = QMap<QString, QString>(const_cast<Function18Params*>(params)->toQMap());
    log.insert({ { "parentName", parentName }, { "figureName", figureName } });
    MacrosManager::log(MacrosManager::CreateContactLine, log);
}

LineFigure Algorithms::getContactLine(QString curveName, const Function18Params *params, Project *project) {
    auto figure = project->findFigure(curveName);
    auto curve = dynamic_cast<const CurveFigure*>(figure);

    ARGUMENT_ASSERT(curve, "Get contact line: curve`s not found");

    return CurveMachine::getContactLine(curve->points(), *params);
}

void Algorithms::createMaxCircle(QString parentName, QString figureName, const Function18Params *params, Project *project, QColor color) {
    auto result = getMaxCircle(parentName, params, project);
    auto circle = new CircleFigure(figureName, result.center(), Point(0, 0, 1), result.radius());
    circle->setColor(color);
    project->safeInsert(figureName, circle);

    auto log = QMap<QString, QString>(const_cast<Function18Params*>(params)->toQMap());
    log.insert({ { "parentName", parentName }, { "figureName", figureName } });
    MacrosManager::log(MacrosManager::CreateMaxCircle, log);
}

CircleFigure Algorithms::getMaxCircle(QString curveName, const Function18Params *params, Project *project) {
    auto figure = project->findFigure(curveName);
    auto curve = dynamic_cast<const CurveFigure*>(figure);

    ARGUMENT_ASSERT(curve, "Get max circle: curve`s not found");

    return CurveMachine::getMaxCircle(curve->points(), *params);
}

QPair<PointFigure, PointFigure> Algorithms::getChord(QString curveName, const Function18Params * params, Project * project) {
    auto figure = project->findFigure(curveName);
    auto curve = dynamic_cast<const CurveFigure*>(figure);

    ARGUMENT_ASSERT(curve, "Get chord: curve`s not found");

    return CurveMachine::getChord(curve->points(), *params);
}

double Algorithms::getChordLength(QString curveName, const Function18Params *params, Project *project) {
    auto figure = project->findFigure(curveName);
    auto curve = dynamic_cast<const CurveFigure*>(figure);

    ARGUMENT_ASSERT(curve, "Get chord length: curve`s not found");

    auto chordLength = CurveMachine::getChordLength(curve->points(), *params);
    return chordLength;
}

QPair<CurvePoint, CurvePoint> Algorithms::getWidthOfLeadingEdge(QString curveName, const Function18Params *params, double distanceFromEdge, Project *project) {
    auto figure = project->findFigure(curveName);
    auto curve = dynamic_cast<const CurveFigure*>(figure);

    ARGUMENT_ASSERT(curve, "Width of leading edge: curve`s not found");

    return CurveMachine::getWidthOfLeadingEdge(curve->points(), *params, distanceFromEdge);
}

QPair<CurvePoint, CurvePoint> Algorithms::getWidthOfTrailingEdge(QString figureName, const Function18Params *params, double distanceFromEdge, Project *project) {
    auto figure = project->findFigure(figureName);
    auto curve = dynamic_cast<const CurveFigure*>(figure);

    ARGUMENT_ASSERT(curve, "Width of trailing edge: curve`s not found");

    return CurveMachine::getWidthOfTrailingEdge(curve->points(), *params, distanceFromEdge);
}

CircleFigure Algorithms::getRadiusOfLeadingEdge(QString curveName, const Function18Params *params, double degreeAngle, Project *project) {
    auto figure = project->findFigure(curveName);
    auto curve = dynamic_cast<const CurveFigure*>(figure);

    ARGUMENT_ASSERT(curve, "Get radius of leading edge: curve`s not found");

    return CurveMachine::getRadiusOfLeadingEdge(curve->points(), *params, degreeAngle);
}

CircleFigure Algorithms::getRadiusOfTrailingEdge(QString figureName, const Function18Params *params, double degreeAngle, Project * project) {
    auto figure = project->findFigure(figureName);
    auto curve = dynamic_cast<const CurveFigure*>(figure);

    ARGUMENT_ASSERT(curve, "Get radius of trailing edge: curve`s not found");

    return CurveMachine::getRadiusOfTrailingEdge(curve->points(), *params, degreeAngle);
}

double Algorithms::getMinX(QString figureName, const Function18Params *params, Project *project) {
    auto figure = project->findFigure(figureName);
    auto curve = dynamic_cast<const CurveFigure*>(figure);

    ARGUMENT_ASSERT(curve, "Get MinX: curve`s not found");
    return CurveMachine::getMinX(curve->points(), *params);
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

    auto result = CurveMachine::calculateBestFit(nominalCurve->points(), measuredCurve->points(), *params, 0.02);
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

    auto xShift = DimFigure::Value(DimFigure::ValueType::dX, isShowX, dx);
    auto yShift = DimFigure::Value(DimFigure::ValueType::dY, isShowY, dy);
    auto rotation = DimFigure::Value(DimFigure::ValueType::Rotation, isShowR, angularCoeff);
    dimension->addValues({ xShift, yShift, rotation });
    dimension->setDimType(DimFigure::DimType::BestFitData);
    dimension->setFirstReference(parentName);

    project->safeInsert(figureName, dimension, false);
}

CurveParts Algorithms::divideCurveIntoParts(const QString &figureName, const Function18Params *params, Project *project) {
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

void Algorithms::calculateCurveUsing3DVectors(const QString &nomCurveName, const QString &measCurveName, const QString &resultCurveName, const Function42Params *params, double radiusCorrection, Project *project) {
    auto nominalFigure = project->findFigure(nomCurveName);
    auto nominalCurve = dynamic_cast<const CurveFigure*>(nominalFigure);
    auto measuredFigure = project->findFigure(measCurveName);
    auto measuredCurve = dynamic_cast<const CurveFigure*>(measuredFigure);

    ARGUMENT_ASSERT(nominalCurve && measuredCurve, "Calculate curve using 3D vectors: best fit line not found");

    auto nominalPoints = nominalCurve->points();
    auto measuredPoints = measuredCurve->points();
    auto resultPoints = CurveMachine::calculateCurveUsing3DVectors(nominalPoints, measuredPoints, *params, radiusCorrection);

    auto resultCurve = new CurveFigure(resultCurveName, resultPoints);
    project->safeInsert(resultCurveName, resultCurve);
}

void Algorithms::calculateMeasuredParams(Project *project, std::shared_ptr<ReportSettings> reportSettings, const QString &globalMeasCurve) {
    auto nominalCurve = reportSettings->nominalName();
    auto &turbineParams = reportSettings->turbineParameters();
    auto params18 = FunctionParamsGenerator::params18(project, reportSettings);
    for(auto [type, paramList] : turbineParams.asKeyValueRange()) {
        for(auto i = 0; i < paramList.count(); i++) {
            paramList[i]->createMeasured(nominalCurve, globalMeasCurve, params18, project);
        }
    }
}

double Algorithms::getDistanceBetweenPoints(Point firstPoint, Point secondPoint) {
    return CurveMachine::getDistanceBetweenPoints(firstPoint, secondPoint);
}

Point Algorithms::getMiddlePoint(Point firstPoint, Point secondPoint) {
    return CurveMachine::getMiddlePoint(firstPoint, secondPoint);
}

void Algorithms::reassembleWholeCurve(const QString &resultCurveName, const CurveParts &curveParts, LEDirection direction, Project *project) {
    auto reassembledPoints = CurveMachine::reassembleWholeCurve(curveParts, direction);
    auto curve = new CurveFigure(resultCurveName, reassembledPoints);
    project->safeInsert(resultCurveName, curve);
}

void Algorithms::reassembleCurveWithoutTE(const QString &resultCurveName, const CurveParts &curveParts, LEDirection direction, Project *project) {
    auto reassembledPoints = CurveMachine::reassembleCurveWithoutTE(curveParts, direction);
    auto curve = new CurveFigure(resultCurveName, reassembledPoints);
    project->safeInsert(resultCurveName, curve);
}

void Algorithms::reassembleCurveWithoutEdges(const QString &resultCurveName, const CurveParts &curveParts, LEDirection direction, Project *project) {
    auto reassembledPoints = CurveMachine::reassembleCurveWithoutEdges(curveParts, direction);
    auto curve = new CurveFigure(resultCurveName, reassembledPoints);
    project->safeInsert(resultCurveName, curve);
}

ResultCompareFLR* Algorithms::compareFLR(QString filepathFLR1, QString filepathFLR2, QString resultPath, int pointsStartIndex, double precision) {
    ARGUMENT_ASSERT(QFile::exists(filepathFLR1), "Compare FLR: FLR 1 not found");
    ARGUMENT_ASSERT(QFile::exists(filepathFLR2), "Compare FLR: FLR 2 not found");
    auto flr1 = FileSystem::readFile(filepathFLR1).split("$END$")[1].trimmed().split("\n");
    auto flr2 = FileSystem::readFile(filepathFLR2).split("$END$")[1].trimmed().split("\n");

    int dimsFails = 0, pointsFails = 0, totalParams = 0;
    double dimsFailsDeviationSum = 0.0, pointsFailsDeviationSum = 0.0, maxFailDeviation = 0.0;
    QList<ResultCompare2Params*> result;

    for(auto i = 0; i < pointsStartIndex; i++) {
        auto compare2 = compareTwoParamsFLR(flr1[i], flr2[i]);
        if(compare2->isDiffBiggerThanPrecision(precision)) {
            dimsFails++;
            dimsFailsDeviationSum += compare2->diff();
            if(compare2->diff() > maxFailDeviation)
                maxFailDeviation = compare2->diff();
        }
        totalParams++;
        result.append(compare2);
    }

    for(auto i = pointsStartIndex, j = pointsStartIndex; i < flr1.size(); i++, j++) {
        auto compare2 = compareTwoParamsFLR(flr1[i], flr2[j]);
        while(!compare2->isComparable()) {
            auto num1 = compare2->name1().split("-")[1].toInt();
            auto num2 = compare2->name2().split("-")[1].toInt();
            if(num1 > num2) {
                i++;
            } else {
                j++;
            }
            compare2 = compareTwoParamsFLR(flr1[i], flr2[j]);
        }
        totalParams++;
        if(compare2->isDiffBiggerThanPrecision(precision)) {
            pointsFails++;
            pointsFailsDeviationSum += compare2->diff();
            if(compare2->diff() > maxFailDeviation)
                maxFailDeviation = compare2->diff();
        }
        result.append(compare2);
    }

    if(!resultPath.isEmpty()) {
        FileSystem::writeCompareFLR(resultPath, result, precision);
        MacrosManager::log(MacrosManager::CompareFLR, {
            { "filepathFLR1", filepathFLR1 },
            { "filepathFLR2", filepathFLR2 },
            { "resultPath", resultPath },
            { "pointsStartIndex", QString::number(pointsStartIndex) },
            { "precision", QString::number(precision) }
            });
    }
    return new ResultCompareFLR(totalParams, dimsFails, dimsFailsDeviationSum, pointsFails, pointsFailsDeviationSum, maxFailDeviation);
}

ResultCompare2Params* Algorithms::compareTwoParamsFLR(QString first, QString second) {
    auto splitted1 = first.split(", ");
    auto splitted2 = second.split(", ");
    return new ResultCompare2Params(splitted1[1], splitted2[1], splitted1[7].toDouble(), splitted2[7].toDouble());
}


