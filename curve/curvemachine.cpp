#include "curve/pch.h"
#include "curvemachine.h"
#include "curvelibrary.h"

CurveFigure CurveMachine::enlargeCurveWithIntermediatePoints(const QVector<Point> points, const Function1Params params) {
    return CurveLibrary::function1(points, params).curve;
}

CurveFigure CurveMachine::getMiddleCurve(const QVector<Point> points, const Function18Params params) {
    return CurveLibrary::function18(points, params).middleCurve;
}

double CurveMachine::getChordLength(const QVector<Point> points, const Function18Params params) {
    return CurveLibrary::function18(points, params).chordLength;
}

CircleFigure CurveMachine::getMaxWidthCircle(const QVector<Point> points, const Function18Params params) {
    return CurveLibrary::function18(points, params).maxCircle;
}

std::array<double, 2> CurveMachine::getWidthOfEdges(const QVector<Point> points, double distanceFromLeadingEdge, double distanceFromTrailingEgde) {
    auto res18 = CurveLibrary::function18(points, Function18Params());
    auto middleCurve = res18.middleCurve.points();
    auto widthOfThickestPart = res18.maxCircle.radius();
    auto &LEpoint = res18.leadingEdgePoint.point(); 
    auto &TEpoint = res18.trailingEdgePoint.point(); 
    auto &contactLE = res18.contactLEPoint.point(); 

    auto firstPointOfIntesection = getOutput8thFunction(LEpoint, distanceFromLeadingEdge * 2, middleCurve);
    auto nearestPointForFirstPoint = getNearestPoint(firstPointOfIntesection, middleCurve);

    auto secondPointOfIntesection = getOutput8thFunction(TEpoint, distanceFromTrailingEgde * 2, middleCurve);
    auto nearestPointForSecondPoint = getNearestPoint(secondPointOfIntesection, middleCurve);

    Point point(contactLE.x + distanceFromLeadingEdge, contactLE.y);
    auto firstLine = getEquationOfPerpendicular(nearestPointForFirstPoint, firstPointOfIntesection);
    auto secondLine = getEquationOfPerpendicular(nearestPointForSecondPoint, secondPointOfIntesection);
    auto mainLine = getEquationOfLine(point, LEpoint);
    auto firstAngle = getAngleBetweenLines(firstLine, mainLine);
    auto secondAngle = getAngleBetweenLines(secondLine, mainLine);

    auto pointsOfLeadingEdge = getOutput7thFunction(firstPointOfIntesection, firstAngle >= 0 ? M_PI - firstAngle : -firstAngle, points);
    pointsOfLeadingEdge = removeExtraPoints(pointsOfLeadingEdge, firstPointOfIntesection, widthOfThickestPart);

    auto pointsOfTrailingEdge = getOutput7thFunction(secondPointOfIntesection, secondAngle >= 0 ? M_PI - secondAngle : -secondAngle, points);
    pointsOfTrailingEdge = removeExtraPoints(pointsOfTrailingEdge, secondPointOfIntesection, widthOfThickestPart);

    auto widthOfLeadingEdge = hypot(pointsOfLeadingEdge[0].x - pointsOfLeadingEdge[1].x, pointsOfLeadingEdge[0].y - pointsOfLeadingEdge[1].y);
    auto widthOfTrailingEdge = hypot(pointsOfTrailingEdge[0].x - pointsOfTrailingEdge[1].x, pointsOfTrailingEdge[0].y - pointsOfTrailingEdge[1].y);

    return { widthOfLeadingEdge, widthOfTrailingEdge };
}

Point CurveMachine::getOutput8thFunction(Point puncturePoint, double diameter, QVector<Point> inputData) {
    auto params8 = Function8Params(diameter, puncturePoint.x, puncturePoint.y);
    return CurveLibrary::function8(inputData, params8).point;
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

QVector<Point> CurveMachine::getOutput7thFunction(Point pointOfIntersection, double angle, QVector<Point> inputData) {
    auto params7 = Function7Params(angle, pointOfIntersection.x, pointOfIntersection.y);
    return CurveLibrary::function7(inputData, params7).contactPoints;
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

std::array<double, 2> CurveMachine::getRadiusOfEdges(const QVector<Point> points, const Function18Params params) {
    auto res18 = CurveLibrary::function18(points, params);

    auto trailingEdgePoints = res18.curveTE.points();
    auto leadingEdgePoints = res18.curveLE.points();

    auto res12ForTE = CurveLibrary::function12(trailingEdgePoints, Function12Params());
    auto res12ForLE = CurveLibrary::function12(leadingEdgePoints, Function12Params());

    auto radiusOfTrailingEdge = res12ForTE.circle.radius();
    auto radiusOfLeadingEdge = res12ForLE.circle.radius();

    return { radiusOfTrailingEdge, radiusOfLeadingEdge };
}
