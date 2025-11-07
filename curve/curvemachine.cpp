#include "curve/pch.h"
#include "curvemachine.h"

QVector<Point> CurveMachine::enlargeCurveWithIntermediatePoints(QVector<Point> points, Function1Params params) {
    FileSystem::createWorkEnvironment();
    FileSystem::fillInputWithSingleSegment(points);
    FileSystem::fillParams(&params);
    makeCurveCalculations();

    auto splittedPoints = FileSystem::readOutput().split('\n');
    return FileSystem::parsePointsFromElement(splittedPoints, ",", 1, 1);
}

void CurveMachine::makeCurveCalculations() {
    QProcess* process = new QProcess();
    QStringList list;
    list.append("-dn");
    list.append("ca/project");
    process->start(FileSystem::curveDirectory, list);
    process->waitForFinished(-1);
}

QVector<Point> CurveMachine::getMiddleCurve(QVector<Point> points, Function18Params params) {
    FileSystem::createWorkEnvironment();
    FileSystem::fillInputWithSingleSegment(points);
    FileSystem::fillParams(&params);
    makeCurveCalculations();

    auto splittedElements = FileSystem::readOutput().split("\n\n");
    QVector<Point> result;
    for(auto i = 0; i < splittedElements.length(); i++) {
        auto strList = splittedElements[i].split('\n');
        if(strList[0].contains("NAM=MCL")) {
            result = FileSystem::parsePointsFromElement(strList, ",", 1, 1);
            break;
        }
    }
    return result;
}

double CurveMachine::getChordLength(QVector<Point> points, Function18Params params) {
    FileSystem::createWorkEnvironment();
    FileSystem::fillInputWithSingleSegment(points);
    FileSystem::fillParams(&params);
    makeCurveCalculations();

    auto splittedElements = FileSystem::readOutput().split("\n\n");
    for(auto i = 0; i < splittedElements.length(); i++) {
        auto strList = splittedElements[i].split('\n');
        if(strList[0].contains("NAM=CONTACT$ENDPOINTS")) {
            return strList.at(strList.size() - 2).split(',')[7].toDouble();
        }
    }
    return -1;
}

CircleFigure CurveMachine::getMaxWidthCircle(QVector<Point> points, Function18Params params) {
    FileSystem::createWorkEnvironment();
    FileSystem::fillInputWithSingleSegment(points);
    FileSystem::fillParams(&params);
    makeCurveCalculations();

    auto splittedElements = FileSystem::readOutput().split("\n\n");
    for(auto i = 0; i < splittedElements.length(); i++) {
        auto strList = splittedElements[i].split('\n');
        if(strList[0].contains("NAM=CIRCLE$MAX")) {
            auto args = strList.at(1).split(',');
            return CircleFigure("maxWidthCircle", Point(args[0].toDouble(), args[1].toDouble()), args[7].toDouble() * 2);
        }
    }
    return CircleFigure("empty");
}

QStringList CurveMachine::getOutput8thFunction(Point puncturePoint, double diameter, QVector<Point> inputData) {
    auto params8 = Function8Params(diameter, puncturePoint.x, puncturePoint.y);
    FileSystem::fillInputWithSingleSegment(inputData);
    FileSystem::fillParams(&params8);
    makeCurveCalculations();
    return FileSystem::readOutput().split("\n\n");
}

Point CurveMachine::getNearestPoint(Point pointOfIntersection, QVector<Point> middleCurve) {
    auto distanceBetweenPointsOfCurve = sqrt(pow(middleCurve[0].x - middleCurve[1].x, 2) + pow(middleCurve[0].y - middleCurve[1].y, 2));
    Point nearestPoint;
    for(auto i = 0; i < middleCurve.length(); i++) {
        auto distance = sqrt(pow(pointOfIntersection.x - middleCurve[i].x, 2) + pow(pointOfIntersection.y - middleCurve[i].y, 2));
        if(distanceBetweenPointsOfCurve >= distance) {
            nearestPoint = middleCurve[i];
        }
    }
    return nearestPoint;
}

QStringList CurveMachine::getOutput7thFunction(Point pointOfIntersection, double angle, QVector<Point> inputData) {
    auto params7 = Function7Params(angle, pointOfIntersection.x, pointOfIntersection.y);
    FileSystem::fillInputWithSingleSegment(inputData);
    FileSystem::fillParams(&params7);
    makeCurveCalculations();
    return FileSystem::readOutput().split("\n\n");
}

QVector<double> CurveMachine::getEquationOfLine(Point firstPoint, Point secondPoint) {
    double coeffX = secondPoint.y - firstPoint.y;
    double coeffY = -secondPoint.x + firstPoint.x;
    double coeffFree = -firstPoint.x * (secondPoint.y - firstPoint.y) + firstPoint.y * (secondPoint.x - firstPoint.x);
    return { coeffX / -coeffY, coeffY / -coeffY, coeffFree / -coeffY };
}

QVector<double> CurveMachine::getEquationOfPerpendicular(Point firstPoint, Point secondPoint) {
    auto dX = secondPoint.x - firstPoint.x;
    auto dY = secondPoint.y - firstPoint.y;
    auto coeffX = -dX / dY;
    auto coeffY = -1.0;
    auto coeffFree = -coeffX * secondPoint.x + secondPoint.y;
    return { coeffX, coeffY, coeffFree };
}

double CurveMachine::getAngleBetweenLines(QVector<double> firstLine, QVector<double> secondLine) {
    return qAtan((secondLine[0] - firstLine[0]) / (1 + firstLine[0] * secondLine[0]));
}

QVector<Point> CurveMachine::removeExtraPoints(QVector<Point> edgePoints, Point pointOfIntersection, double width) {
    QVector<Point> edgeNewPoints;
    for(auto i = 0; i < edgePoints.length(); i++) {
        auto point = edgePoints[i];
        if(hypot(pointOfIntersection.x - point.x, pointOfIntersection.y - point.y) < width * 2) {
            edgeNewPoints.append(edgePoints[i]);
        }
    }
    return edgeNewPoints;
}

std::array<double, 2> CurveMachine::getWidthOfEdges(QVector<Point> points, double distanceFromLeadingEdge, double distanceFromTrailingEgde) {
    Function18Params params18;
    FileSystem::createWorkEnvironment();
    FileSystem::fillInputWithSingleSegment(points);
    FileSystem::fillParams(&params18);
    makeCurveCalculations();
    auto outputData = FileSystem::readOutput().split("\n\n");
    auto middleCurve = FileSystem::parsePointsFromElement(outputData[0].split("\n"), ",", 1, 1);
    auto widthOfThickestPart = outputData[2].split("\n")[1].split(",")[7].toDouble();
    auto puncturePoints = FileSystem::parsePointsFromElement(outputData[1].split("\n"), ",", 1, 1);
    auto touchPoints = FileSystem::parsePointsFromElement(outputData[4].split("\n"), ",", 1, 1);

    auto outputData8thFunction = getOutput8thFunction(puncturePoints[1], distanceFromLeadingEdge * 2, middleCurve);
    auto firstPointOfIntesection = FileSystem::parsePointsFromElement(outputData8thFunction[0].split("\n"), ",", 1, 1)[0];
    auto nearestPointForFirstPoint = getNearestPoint(firstPointOfIntesection, middleCurve);

    outputData8thFunction = getOutput8thFunction(puncturePoints[0], distanceFromTrailingEgde * 2, middleCurve);
    auto secondPointOfIntesection = FileSystem::parsePointsFromElement(outputData8thFunction[0].split("\n"), ",", 1, 1)[0];
    auto nearestPointForSecondPoint = getNearestPoint(secondPointOfIntesection, middleCurve);

    Point point(touchPoints[1].x + distanceFromLeadingEdge, touchPoints[1].y);
    auto firstLine = getEquationOfPerpendicular(nearestPointForFirstPoint, firstPointOfIntesection);
    auto secondLine = getEquationOfPerpendicular(nearestPointForSecondPoint, secondPointOfIntesection);
    auto mainLine = getEquationOfLine(point, touchPoints[1]);
    double firstAngle = getAngleBetweenLines(firstLine, mainLine);
    double secondAngle = getAngleBetweenLines(secondLine, mainLine);

    auto outputData7thFunction = getOutput7thFunction(firstPointOfIntesection, firstAngle >= 0 ? 3.14 - firstAngle : -firstAngle, points);
    auto pointsOfLeadingEdge = FileSystem::parsePointsFromElement(outputData7thFunction[0].split("\n"), ",", 1, 1);
    pointsOfLeadingEdge = removeExtraPoints(pointsOfLeadingEdge, firstPointOfIntesection, widthOfThickestPart);

    outputData7thFunction = getOutput7thFunction(secondPointOfIntesection, secondAngle >= 0 ? 3.14 - secondAngle : -secondAngle, points);
    auto pointsOfTrailingEdge = FileSystem::parsePointsFromElement(outputData7thFunction[0].split("\n"), ",", 1, 1);
    pointsOfTrailingEdge = removeExtraPoints(pointsOfTrailingEdge, secondPointOfIntesection, widthOfThickestPart);

    auto widthOfLeadingEdge = hypot(pointsOfLeadingEdge[0].x - pointsOfLeadingEdge[1].x, pointsOfLeadingEdge[0].y - pointsOfLeadingEdge[1].y);
    auto widthOfTrailingEdge = hypot(pointsOfTrailingEdge[0].x - pointsOfTrailingEdge[1].x, pointsOfTrailingEdge[0].y - pointsOfTrailingEdge[1].y);

    return { widthOfLeadingEdge, widthOfTrailingEdge };
}
