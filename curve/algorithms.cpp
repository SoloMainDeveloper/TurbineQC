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

std::array<double, 2> Algorithms::getWidthOfEdges(QString figureName, double distanceFromLeadingEdge, double distanceFromTrailingEgde,
    Project *project, bool createSegmentLE, bool createSegmentTE) {
    auto figure = project->findFigure(figureName);
    auto curve = dynamic_cast<const CurveFigure*>(figure);

    ARGUMENT_ASSERT(curve, "Width of edges: curve`s not found");

    auto points = CurveMachine::getWidthOfEdges(curve->points(), distanceFromLeadingEdge, distanceFromTrailingEgde);
    auto widthLE = CurveMachine::getDistanceBetweenPoints(points[0].first, points[0].second);
    auto widthTE = CurveMachine::getDistanceBetweenPoints(points[1].first, points[1].second);

    if(createSegmentLE) {
        auto segmentName = figureName + "_LE_width";
        auto point1 = new PointFigure(segmentName + "_1", points[0].first);
        project->safeInsert(point1->name(), point1);
        auto point2 = new PointFigure(segmentName + "_2", points[0].second);
        project->safeInsert(point2->name(), point2);

        auto middlePoint = Point((point1->point().x + point2->point().x) / 2, (point1->point().y + point2->point().y) / 2);
        auto value = DimFigure::Value(DimFigure::ValueType::Length);
        value.measurement = widthLE;
        auto dimFigure = new DimFigure(segmentName, middlePoint, point1, point2);
        dimFigure->setDimType(DimFigure::DimType::Distance);
        dimFigure->setRenderType(DimFigure::RenderType::DistanceBetweenCurvePoints);
        dimFigure->addValue(value);
        project->safeInsert(segmentName, dimFigure);
    }
    if(createSegmentTE) {
        auto segmentName = figureName + "_TE_width";
        auto point1 = new PointFigure(segmentName + "_1", points[1].first);
        project->safeInsert(point1->name(), point1);    
        auto point2 = new PointFigure(segmentName + "_2", points[1].second);
        project->safeInsert(point2->name(), point2);

        auto middlePoint = Point((point1->point().x + point2->point().x) / 2, (point1->point().y + point2->point().y) / 2);
        auto value = DimFigure::Value(DimFigure::ValueType::Length);
        value.measurement = widthTE;
        auto dimFigure = new DimFigure(segmentName, middlePoint, point1, point2);
        dimFigure->setDimType(DimFigure::DimType::Distance);
        dimFigure->setRenderType(DimFigure::RenderType::DistanceBetweenCurvePoints);
        dimFigure->addValue(value);
        project->safeInsert(segmentName, dimFigure);
    }
    MacrosManager::log(MacrosManager::GetWidthOfEdges, {
        { "figureName", figureName },
        { "distanceLE", QString::number(distanceFromLeadingEdge) },
        { "distanceTE", QString::number(distanceFromTrailingEgde) },
        { "createSegmentLE", createSegmentLE ? "true" : "false" },
        { "createSegmentTE", createSegmentTE ? "true" : "false"}
    });
    
    return { widthLE, widthTE };
}

std::array<double, 2> Algorithms::getRadiusOfEdges(QString figureName, const Function18Params *params, Project *project) {
    auto figure = project->findFigure(figureName);
    auto curve = dynamic_cast<const CurveFigure*>(figure);

    ARGUMENT_ASSERT(curve, "Radius of edges: curve`s not found");

    auto result = CurveMachine::getRadiusOfEdges(curve->points(), *params);
    return result;
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

bool Algorithms::tryMergePointClouds(QString first, QString second, QString resName, double threshold, bool needSorted, Project *project) {
    auto firstCloud = project->findFigure(first);
    auto secondCloud = project->findFigure(second);
    auto firstCurve = dynamic_cast<const CurveFigure*>(firstCloud);
    auto secondCurve = dynamic_cast<const CurveFigure*>(secondCloud);

    ARGUMENT_ASSERT(firstCurve && secondCurve, "Merge curves: curve`s not found");

    auto result = CurveMachine::mergePointClouds(firstCurve->points(), secondCurve->points(), threshold, needSorted);
    if(result.length() == 0)
        return false;
    auto curve = new CurveFigure(resName, result);
    project->safeInsert(resName, curve);
    MacrosManager::log(MacrosManager::MergeScans, {
        { "firstName", first },
        { "secondName", second },
        { "resultName", resName },
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

void Algorithms::calculateBestFit(QString nomCurveName, QString measCurveName, QString resultCurveName, const Function6Params *params, Project *project) {
    auto nominalFigure = project->findFigure(nomCurveName);
    auto measuredFigure = project->findFigure(measCurveName);
    auto nominalCurve = dynamic_cast<const CurveFigure*>(nominalFigure);
    auto measuredCurve = dynamic_cast<const CurveFigure*>(measuredFigure);

    ARGUMENT_ASSERT(nominalCurve && measuredCurve, "Calculate BestFit: curve`s not found");

    auto result = CurveMachine::calculateBestFit(nominalCurve->points(), measuredCurve->points(), *params);
    auto resultCurve = new CurveFigure;
    resultCurve->changePoints(result.points());
    resultCurve->setName(resultCurveName);
    if(measuredCurve->isClosed()) {
        resultCurve->setClosed(true);
    }
    project->safeInsert(resultCurveName, resultCurve);

    auto log = QMap<QString, QString>(const_cast<Function6Params*>(params)->toQMap());
    log.insert({ { "nominal", nomCurveName }, { "measured", measCurveName }, { "resultName", resultCurveName } });
    MacrosManager::log(MacrosManager::BestFit, log);
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