#include "curve/pch.h"
#include "algorithms.h"

void Algorithms::enlargeCurveWithIntermediatePoints(QString figureName, const Function1Params *params, Project *project) {
    auto figure = project->findFigure(figureName);
    auto curve = dynamic_cast<const CurveFigure*>(figure);

    assert(curve);

    auto result = CurveMachine::enlargeCurveWithIntermediatePoints(curve->points(), *params);
    auto newName = figureName + "_enlarged";
    auto newCurve = new CurveFigure(newName, result.points());
    project->safeInsert(newName, newCurve);
}

void Algorithms::getMiddleCurve(QString figureName, const Function18Params *params, Project *project) {
    auto figure = project->findFigure(figureName);
    auto curve = dynamic_cast<const CurveFigure*>(figure);

    assert(curve);

    auto result = CurveMachine::getMiddleCurve(curve->points(), *params);
    auto newName = figureName + "_MCL";
    auto middleCurve = new CurveFigure(newName, result.points());
    project->safeInsert(newName, middleCurve);
}

double Algorithms::getChordLength(QString figureName, const Function18Params *params, Project *project) {
    auto figure = project->findFigure(figureName);
    auto curve = dynamic_cast<const CurveFigure*>(figure);

    assert(curve);

    auto chordLength = CurveMachine::getChordLength(curve->points(), *params);
    return chordLength;
}

void Algorithms::getMaxWidthCircle(QString figureName, const Function18Params *params, Project *project) {
    auto figure = project->findFigure(figureName);
    auto curve = dynamic_cast<const CurveFigure*>(figure);

    assert(curve);

    auto result = CurveMachine::getMaxWidthCircle(curve->points(), *params);
    auto newName = figureName + "_MaxDia";
    auto circle = new CircleFigure(newName, result.center(), Point(0, 0, 1), result.radius());
    circle->setColor(QColorConstants::Blue);
    project->safeInsert(newName, circle);
}

std::array<double, 2> Algorithms::getWidthOfEdges(QString figureName, double distanceFromLeadingEdge, double distanceFromTrailingEgde,
    Project *project, bool createSegmentLE, bool createSegmentTE) {
    auto figure = project->findFigure(figureName);
    auto curve = dynamic_cast<const CurveFigure*>(figure);

    assert(curve);

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
        auto dimFigure = new DimFigure(segmentName, DimFigure::DimType::Distance, middlePoint, point1, point2);
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
        auto dimFigure = new DimFigure(segmentName, DimFigure::DimType::Distance, middlePoint, point1, point2);
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

    assert(curve);

    auto result = CurveMachine::getRadiusOfEdges(curve->points(), *params);
    return result;
}

void Algorithms::makeRadiusCorrection(QString figureName, QString figureNewName, const Function3Params *params, Project *project) {
    auto figure = project->findFigure(figureName);
    auto curve = dynamic_cast<const CurveFigure*>(figure);

    assert(curve);

    auto result = CurveMachine::offsetCurve(curve->points(), *params);
    auto resultCurve = new CurveFigure;
    *resultCurve = *curve;
    resultCurve->changePoints(result.points());
    resultCurve->setName(figureNewName);
    project->safeInsert(figureNewName, resultCurve);
    MacrosManager::log(MacrosManager::RadiusCorrection, {
        { "figureName", figureName },
        { "newName", figureNewName }
    }, const_cast<Function3Params*>(params)->toQString());
}

bool Algorithms::tryMergePointClouds(QString first, QString second, QString resName, double threshold, bool needSorted, Project *project) {
    auto firstCloud = project->findFigure(first);
    auto secondCloud = project->findFigure(second);
    auto firstCurve = dynamic_cast<const CurveFigure*>(firstCloud);
    auto secondCurve = dynamic_cast<const CurveFigure*>(secondCloud);

    assert(firstCurve && secondCurve);

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

    assert(measCurve);
    assert(nomCurve);

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
    MacrosManager::log(MacrosManager::CalculateDeviations, {
        { "nominal", nomCurveName },
        { "measured", measCurveName },
        { "resultName", resultCurveName }
    }, const_cast<Function4Params*>(params)->toQString());
}

void Algorithms::calculateBestFit(QString nomCurveName, QString measCurveName, QString resultCurveName, const Function6Params *params, Project *project, ReportData *reportData) {
    auto nominalFigure = project->findFigure(nomCurveName);
    auto measuredFigure = project->findFigure(measCurveName);
    auto nominalCurve = dynamic_cast<const CurveFigure*>(nominalFigure);
    auto measuredCurve = dynamic_cast<const CurveFigure*>(measuredFigure);

    assert(nominalCurve && measuredCurve);

    auto result = CurveMachine::calculateBestFit(nominalCurve->points(), measuredCurve->points(), *params, reportData);
    auto resultCurve = new CurveFigure;
    resultCurve->changePoints(result.points());
    resultCurve->setName(resultCurveName);
    if(measuredCurve->isClosed()) {
        resultCurve->setClosed(true);
    }
    project->safeInsert(resultCurveName, resultCurve);
    MacrosManager::log(MacrosManager::BestFit, {
        { "nominal", nomCurveName },
        { "measured", measCurveName },
        { "resultName", resultCurveName },
    }, const_cast<Function6Params*>(params)->toQString());
}
