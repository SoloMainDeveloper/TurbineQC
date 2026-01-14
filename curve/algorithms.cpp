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
    if(createSegmentLE) {
        auto segmentName = figureName + "_LE_width";
        auto point1 = new PointFigure(segmentName + "_1", points[0].first);
        project->safeInsert(point1->name(), point1);
        auto point2 = new PointFigure(segmentName + "_2", points[0].second);
        project->safeInsert(point2->name(), point2);
        project->safeInsert(segmentName, dynamic_cast<LineFigure*>(CurveMachine::getLineSegment(segmentName, point1, point2)));
    }
    if(createSegmentTE) {
        auto segmentName = figureName + "_TE_width";
        auto point1 = new PointFigure(segmentName + "_1", points[1].first);
        project->safeInsert(point1->name(), point1);    
        auto point2 = new PointFigure(segmentName + "_2", points[1].second);
        project->safeInsert(point2->name(), point2);
        project->safeInsert(segmentName, dynamic_cast<LineFigure*>(CurveMachine::getLineSegment(segmentName, point1, point2)));
    }
    auto widthLE = CurveMachine::getDistanceBetweenPoints(points[0].first, points[0].second);
    auto widthTE = CurveMachine::getDistanceBetweenPoints(points[1].first, points[1].second);
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
}

QVector<CurvePoint> Algorithms::mergePointClouds(QString nameOfFirstCloud, QString nameOfSecondCloud, double threshold, bool needSorted, Project *project) {
    auto firstCloud = project->findFigure(nameOfFirstCloud);
    auto secondCloud = project->findFigure(nameOfSecondCloud);
    auto firstCurve = dynamic_cast<const CurveFigure*>(firstCloud);
    auto secondCurve = dynamic_cast<const CurveFigure*>(secondCloud);

    assert(firstCurve && secondCurve);

    auto result = CurveMachine::mergePointClouds(firstCurve->points(), secondCurve->points(), threshold, needSorted);
    return result;
}

void Algorithms::turbineRun(QString figureName, const Function18Params *params, Project *project, bool needMaxDia, bool needMiddleCurve, bool needChord) {
    auto figure = project->findFigure(figureName);
    auto curve = dynamic_cast<const CurveFigure*>(figure);

    assert(curve);

    auto result = CurveLibrary::function18(curve->points(), *params);
    if(needMaxDia) {
        auto newName = figureName + "_MaxDia";
        auto circle = new CircleFigure(newName, result.maxCircle.center(), Point(0, 0, 1), result.maxCircle.radius());
        circle->setColor(QColor::fromRgb(255, 0, 255));
        project->safeInsert(newName, circle);
    }
    if(needMiddleCurve) {
        auto newName = figureName + "_MCL";
        auto middleCurve = new CurveFigure(newName, result.middleCurve.points());
        middleCurve->setColor(QColorConstants::Blue);
        project->safeInsert(newName, middleCurve);
    }
    if(needChord) {
        auto newName = figureName + "_Chord";
        auto chord = new LineFigure(newName, result.chordLine.origin(), result.chordLine.direction());
        chord->setColor(QColorConstants::Red);
        project->safeInsert(newName, chord);
    }
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
    project->safeInsert(resultCurveName, resultCurve);
}
