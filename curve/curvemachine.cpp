#include "curve/pch.h"
#include "curvemachine.h"
#include "curvelibrary.h"

CurveFigure CurveMachine::enlargeCurveWithIntermediatePoints(const QVector<CurvePoint> points, const Function1Params params) {
    return CurveLibrary::function1(points, params).curve;
}

CurveFigure CurveMachine::regenerateCurve(const QVector<CurvePoint> curve, const Function19Params params) {
    return CurveLibrary::function19(curve, params).curve;
}

CurveFigure CurveMachine::getMiddleCurve(const QVector<CurvePoint> points, const Function18Params params) {
    return CurveLibrary::function18(points, params).middleCurve;
}

LineFigure CurveMachine::getContactLine(const QVector<CurvePoint> points, const Function18Params params) {
    return CurveLibrary::function18(points, params).contactLine;
}

double CurveMachine::getContactLineLength(const QVector<CurvePoint> points, const Function18Params params) {
    return CurveLibrary::function18(points, params).contactLineLength;
}

CircleFigure CurveMachine::getMaxCircle(const QVector<CurvePoint> points, const Function18Params params) {
    return CurveLibrary::function18(points, params).maxCircle;
}

std::pair<CurvePoint, CurvePoint> CurveMachine::getWidthOfLeadingEdge(const QVector<CurvePoint> points, double distanceFromEdge) {
    auto res18 = CurveLibrary::function18(points, Function18Params());
    auto middleCurve = res18.middleCurve.points();
    auto widthOfThickestPart = res18.maxCircle.radius();
    auto LEpoint = res18.leadingEdgePoint.point();
    auto contactLE = res18.contactLEPoint.point();

    auto intersectionPoint = getOutput8thFunction(LEpoint, distanceFromEdge * 2, middleCurve);
    auto nearestPoint = getNearestPoint(intersectionPoint, middleCurve);

    CurvePoint point(contactLE.x + distanceFromEdge, contactLE.y);
    auto perpendicular = getEquationOfPerpendicular(nearestPoint, intersectionPoint);
    auto mainLine = getEquationOfLine(point, contactLE);
    auto angle = getAngleBetweenLines(perpendicular, mainLine);

    auto pointsOfEdge = getOutput7thFunction(intersectionPoint, angle >= 0 ? M_PI - angle : -angle, points);
    pointsOfEdge = removeExtraPoints(pointsOfEdge, intersectionPoint, widthOfThickestPart);

    return std::make_pair(pointsOfEdge[0], pointsOfEdge[1]);
}

std::pair<CurvePoint, CurvePoint> CurveMachine::getWidthOfTrailingEdge(const QVector<CurvePoint> points, double distanceFromEdge) {
    auto res18 = CurveLibrary::function18(points, Function18Params());
    auto middleCurve = res18.middleCurve.points();
    auto widthOfThickestPart = res18.maxCircle.radius();
    auto TEpoint = res18.trailingEdgePoint.point();
    auto contactTE = res18.contactTEPoint.point();

    auto intersectionPoint = getOutput8thFunction(TEpoint, distanceFromEdge * 2, middleCurve);
    auto nearestPoint = getNearestPoint(intersectionPoint, middleCurve);

    CurvePoint point(contactTE.x + distanceFromEdge, contactTE.y);
    auto perpendicular = getEquationOfPerpendicular(nearestPoint, intersectionPoint);
    auto mainLine = getEquationOfLine(point, contactTE);
    auto angle = getAngleBetweenLines(perpendicular, mainLine);

    auto pointsOfEdge = getOutput7thFunction(intersectionPoint, angle >= 0 ? M_PI - angle : -angle, points);
    pointsOfEdge = removeExtraPoints(pointsOfEdge, intersectionPoint, widthOfThickestPart);

    return std::make_pair(pointsOfEdge[0], pointsOfEdge[1]);
}

double CurveMachine::getDistanceBetweenPoints(CurvePoint firstPoint, CurvePoint secondPoint) {
    return hypot(firstPoint.x - secondPoint.x, firstPoint.y - secondPoint.y);
}

Point CurveMachine::normalizeVector(Point vector) {
    auto length = hypot(vector.x, vector.y);
    return Point(vector.x / length, vector.y / length);
}

Point CurveMachine::getOutput8thFunction(CurvePoint puncturePoint, double diameter, QVector<CurvePoint> inputData) {
    auto params8 = Function8Params(diameter, puncturePoint.x, puncturePoint.y);
    return CurveLibrary::function8(inputData, params8).point;
}

Point CurveMachine::getNearestPoint(Point pointOfIntersection, QVector<CurvePoint> middleCurve) {
    auto distanceBetweenPointsOfCurve = hypot(middleCurve[0].x - middleCurve[1].x, middleCurve[0].y - middleCurve[1].y);
    Point nearestPoint;
    for(auto i = 0; i < middleCurve.length(); i++) {
        auto distance = hypot(pointOfIntersection.x - middleCurve[i].x, pointOfIntersection.y - middleCurve[i].y);
        if(distanceBetweenPointsOfCurve >= distance) {
            nearestPoint = Point(middleCurve[i]); // TODO: extra data?
        }
    }
    return nearestPoint;
}

QVector<CurvePoint> CurveMachine::getOutput7thFunction(Point pointOfIntersection, double angle, QVector<CurvePoint> inputData) {
    auto params7 = Function7Params(angle, pointOfIntersection.x, pointOfIntersection.y, 0, true);
    return CurveLibrary::function7(inputData, params7).contactPoints;
}

QVector<double> CurveMachine::getEquationOfLine(CurvePoint firstPoint, CurvePoint secondPoint) {
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

QVector<CurvePoint> CurveMachine::removeExtraPoints(QVector<CurvePoint> edgePoints, Point pointOfIntersection, double width) {
    QVector<CurvePoint> edgeNewPoints;
    for(auto i = 0; i < edgePoints.length(); i++) {
        auto point = edgePoints[i];
        if(hypot(pointOfIntersection.x - point.x, pointOfIntersection.y - point.y) < width * 2) {
            edgeNewPoints.append(edgePoints[i]);
        }
    }
    return edgeNewPoints;
}

double CurveMachine::getRadiusOfLeadingEdge(const QVector<CurvePoint> points, const Function18Params params) {
    auto res18 = CurveLibrary::function18(points, params);
    auto puncturePointLE = res18.leadingEdgePoint.point();
    auto maxRadius = res18.maxCircle.radius();
    auto middleCurve = res18.middleCurve.points();
    auto middleCurvePointLE = middleCurve[middleCurve.length() - 2];

    auto equationOfLine = getEquationOfLine(puncturePointLE, middleCurvePointLE);
    auto radiusOfLE = correctRadius(points, equationOfLine, puncturePointLE, middleCurvePointLE, maxRadius);

    return radiusOfLE;
}

double CurveMachine::getRadiusOfTrailingEdge(const QVector<CurvePoint> points, const Function18Params params) {
    auto res18 = CurveLibrary::function18(points, params);
    auto puncturePointTE = res18.trailingEdgePoint.point();
    auto maxRadius = res18.maxCircle.radius();
    auto middleCurve = res18.middleCurve.points();
    auto middleCurvePointTE = middleCurve[1];

    auto equationOfLineTE = getEquationOfLine(puncturePointTE, middleCurvePointTE);
    auto radiusOfLE = correctRadius(points, equationOfLineTE, puncturePointTE, middleCurvePointTE, maxRadius);

    return radiusOfLE;
}

double CurveMachine::correctRadius(const QVector<CurvePoint> &points, const QVector<double> &equationOfLine, CurvePoint puncturePoint, CurvePoint middleCurvePoint, double radius) {
    auto angle = 60;
    auto previousRadius = 0.0;
    auto angularCoefficient = equationOfLine[0];

    while(true) {
        auto coordXOfPoint = 0.0;
        if(puncturePoint.x < middleCurvePoint.x) {
            coordXOfPoint = puncturePoint.x + radius * qCos(abs(qAtan(angularCoefficient)));
        } else {
            coordXOfPoint = puncturePoint.x - radius * qCos(abs(qAtan(angularCoefficient)));
        }
        Point pointOfLine(coordXOfPoint, getCoordY(equationOfLine, coordXOfPoint));

        auto k1 = qTan(qDegreesToRadians(angle) + qAtan(angularCoefficient));
        auto intersectionPoints = CurveLibrary::function7(points, Function7Params(qAtan(k1), pointOfLine.x, pointOfLine.y)).contactPoints;
        auto firstNearestPoint = getNearestIntesectionPoint(intersectionPoints, puncturePoint);

        auto k2 = qTan(-qDegreesToRadians(angle) + qAtan(angularCoefficient));
        intersectionPoints = CurveLibrary::function7(points, Function7Params(qAtan(k2), pointOfLine.x, pointOfLine.y)).contactPoints;
        auto secondNearstPoint = getNearestIntesectionPoint(intersectionPoints, puncturePoint);

        radius = CurveLibrary::function12(QVector<CurvePoint>{ firstNearestPoint, puncturePoint, secondNearstPoint }, Function12Params()).circle.radius();

        if(abs(previousRadius - radius) <= 0.0005) {
            break;
        } else {
            previousRadius = radius;
        }
    }
    return radius;
}

double CurveMachine::getCoordY(const QVector<double> &equationOfLine, double coordX) {
    auto coeffX = equationOfLine[0];
    auto coeffFree = equationOfLine[2];
    auto coordY = coeffX * coordX + coeffFree;

    return coordY;
}

CurvePoint CurveMachine::getNearestIntesectionPoint(const QVector<CurvePoint> &pointsOfIntersection, CurvePoint targetPoint) {
    CurvePoint nearestPoint;
    auto minDistance = 10000.0;

    for(auto i = 0; i < pointsOfIntersection.length(); i++) {
        auto distance = hypot(pointsOfIntersection[i].x - targetPoint.x, pointsOfIntersection[i].y - targetPoint.y);
        if(minDistance >= distance) {
            nearestPoint = pointsOfIntersection[i];
            minDistance = distance;
        }
    }
    return nearestPoint;
}

QVector<CurvePoint> CurveMachine::mergePointClouds(const QVector<CurvePoint> &firstCloud, const QVector<CurvePoint> &secondCloud, double threshold, bool needSort) {
    auto params1 = Function1Params(0, threshold, 0, false, true, FunctionParams::Direction::Left, needSort);
    auto pointsOfFirstCloud = needSort ? CurveLibrary::function1(firstCloud, params1).curve.points() : firstCloud;
    auto pointsOfSecondCloud = needSort ? CurveLibrary::function1(secondCloud, params1).curve.points() : secondCloud;

    auto lengthOfFirstCloud = pointsOfFirstCloud.length();
    auto leftPoints = pointsOfFirstCloud.mid(0, lengthOfFirstCloud / 2);
    auto rightPoints = pointsOfFirstCloud.mid(lengthOfFirstCloud / 2, lengthOfFirstCloud);

    auto leftPointsOfIntersection = CurveLibrary::function14(leftPoints, pointsOfSecondCloud, Function14Params()).curve.points();
    auto rightPointsOfIntersection = CurveLibrary::function14(rightPoints, pointsOfSecondCloud, Function14Params()).curve.points();

    if(leftPointsOfIntersection.length() == 0 || rightPointsOfIntersection.length() == 0) {
        return QVector<CurvePoint>();
    }

    QVector<CurvePoint> processedLeftPoints = { leftPointsOfIntersection[0] };
    for(auto i = 1; i < leftPointsOfIntersection.length(); i++) {
        auto previousPoint = leftPointsOfIntersection[i - 1];
        auto currentPoint = leftPointsOfIntersection[i];
        auto distance = hypot(previousPoint.x - currentPoint.x, previousPoint.y - currentPoint.y);
        if(distance > threshold) processedLeftPoints.append(currentPoint);
    }
    QVector<CurvePoint> processedRightPoints = rightPointsOfIntersection;
    for(auto i = 1; i < rightPointsOfIntersection.length(); i++) {
        auto previousPoint = rightPointsOfIntersection[i - 1];
        auto currentPoint = rightPointsOfIntersection[i];
        auto distance = hypot(previousPoint.x - currentPoint.x, previousPoint.y - currentPoint.y);
        if(distance > threshold) processedRightPoints.append(currentPoint);
    }

    auto leftMiddlePoint = processedLeftPoints.length() % 2 == 0 ?
        processedLeftPoints[processedLeftPoints.length() / 2 - 1] :
        processedLeftPoints[processedLeftPoints.length() / 2];
    auto rightMiddlePoint = processedRightPoints.length() % 2 == 0 ?
        processedRightPoints[processedRightPoints.length() / 2 - 1] :
        processedRightPoints[processedRightPoints.length() / 2];

    trimCloudFromStart(pointsOfFirstCloud, leftMiddlePoint);
    trimCloudFromEnd(pointsOfFirstCloud, rightMiddlePoint);

    auto firstPointOfFirstCloud = pointsOfFirstCloud[0];
    trimCloudFromStart(pointsOfSecondCloud, rightMiddlePoint);
    trimCloudFromEnd(pointsOfSecondCloud, leftMiddlePoint);

    auto mergedCloud = pointsOfFirstCloud + pointsOfSecondCloud;
    if(needSort) {
        params1 = Function1Params(0, threshold, 0, true, true, FunctionParams::Direction::Left, needSort);
        mergedCloud = CurveLibrary::function1(mergedCloud, params1).curve.points();
    }
    params1 = Function1Params(0, threshold, 0, true, true, FunctionParams::Direction::Left, !needSort);
    mergedCloud = CurveLibrary::function1(mergedCloud, params1).curve.points();

    return mergedCloud;
}

void CurveMachine::trimCloudFromStart(QVector<CurvePoint> &points, CurvePoint pointOfIntersection) {
    for(auto i = 1; i < points.length(); i++) {
        auto previousPoint = points[i - 1];
        auto currentPoint = points[i];
        auto previousDistance = hypot(pointOfIntersection.x - previousPoint.x, pointOfIntersection.y - previousPoint.y);
        auto currentDistance = hypot(pointOfIntersection.x - currentPoint.x, pointOfIntersection.y - currentPoint.y);
        auto centerDistance = hypot(previousPoint.x - currentPoint.x, previousPoint.y - currentPoint.y);
        auto isInInterval = previousDistance < centerDistance && currentDistance < centerDistance ? true : false;

        if((previousPoint.x > pointOfIntersection.x && pointOfIntersection.x > currentPoint.x ||
            previousPoint.x < pointOfIntersection.x && pointOfIntersection.x < currentPoint.x) && isInInterval) {
            points = previousDistance < currentDistance ? points.mid(i - 1) : points.mid(i);
            break;
        }
    }
}

void CurveMachine::trimCloudFromEnd(QVector<CurvePoint> &points, CurvePoint pointOfIntersection) {
    for(auto i = points.length() - 2; i >= 0; i--) {
        auto currentPoint = points[i];
        auto previousPoint = points[i + 1];
        auto previousDistance = hypot(pointOfIntersection.x - previousPoint.x, pointOfIntersection.y - previousPoint.y);
        auto currentDistance = hypot(pointOfIntersection.x - currentPoint.x, pointOfIntersection.y - currentPoint.y);
        auto centerDistance = hypot(previousPoint.x - currentPoint.x, previousPoint.y - currentPoint.y);
        auto isInInterval = previousDistance < centerDistance && currentDistance < centerDistance ? true : false;

        if((previousPoint.x > pointOfIntersection.x && pointOfIntersection.x > currentPoint.x ||
            previousPoint.x < pointOfIntersection.x && pointOfIntersection.x < currentPoint.x) && isInInterval) {
            points = previousDistance < currentDistance ? points.mid(0, i + 2) : points.mid(0, i + 1);
            break;
        }
    }
}

double CurveMachine::getMinXOfCurvePointVector(const QVector<CurvePoint>& points) {
    auto minX = std::numeric_limits<double>::max();
    for(auto &point : points) {
        if(point.x < minX) {
            minX = point.x;
        }
    }
    return minX;
}

CurveFigure CurveMachine::offsetCurve(const QVector<CurvePoint> curve, const Function3Params params) {
    return CurveLibrary::function3(curve, params).curve;
}

CurveFigure CurveMachine::calculateDeviations(const QVector<CurvePoint> nomCurve, const QVector<CurvePoint> measCurve, const Function4Params params) {
    return CurveLibrary::function4(nomCurve, measCurve, params).curve;
}

Function6Result CurveMachine::calculateBestFit(const QVector<CurvePoint> nominalCurve, const QVector<CurvePoint> measuredCurve, const Function6Params params) {
    return CurveLibrary::function6(measuredCurve, nominalCurve, params);
}

Function21Result CurveMachine::calculateBestFit(const QVector<CurvePoint> nominalCurve, const QVector<CurvePoint> measuredCurve, const Function21Params params) {
    return CurveLibrary::function21(measuredCurve, nominalCurve, nominalCurve, params);
}

Point CurveMachine::getDirection(Point linePoint, double angularCoefficient) {
    auto coeffB = linePoint.y - linePoint.x * angularCoefficient;
    auto x = 1;
    auto y = x * angularCoefficient + coeffB;
    auto nextLinePoint = Point(x, y);

    auto direction = Point(nextLinePoint.x - linePoint.x, nextLinePoint.y - linePoint.y);
    return normalizeVector(direction);
}

CurveAnalyzer::CurveParts CurveMachine::divideCurveIntoParts(const QVector<CurvePoint> curve, const Function18Params params) {
    auto res18 = CurveLibrary::function18(curve, params);
    return CurveAnalyzer::CurveParts(
        res18.curveLE.points(),
        res18.curveTE.points(),
        res18.curveHigh.points(),
        res18.curveLow.points()
    );
}

QVector<CurvePoint> CurveMachine::calculateStretch(const QVector<CurvePoint> &nominalCurve, const QVector<CurvePoint> &measuredCurve, const Function31Params &params) {
    auto res31 = CurveLibrary::function31(nominalCurve, measuredCurve, params);
    return res31.curve.points();
}

QVector<CurvePoint> CurveMachine::calculateCurveUsing3DVectors(const QVector<CurvePoint> &nominalCurve, const QVector<CurvePoint> &measuredCurve, const Function42Params &params) {
    auto res42 = CurveLibrary::function42(measuredCurve, nominalCurve, params);
    return res42.curve.points();
}

double CurveMachine::getMinX(const QVector<CurvePoint> points, const Function18Params params) {
    auto res18 = CurveLibrary::function18(points, params);
    auto minX = getMinXOfCurvePointVector(res18.curveLE.points());
    minX = std::min(minX, getMinXOfCurvePointVector(res18.curveTE.points()));
    minX = std::min(minX, getMinXOfCurvePointVector(res18.curveHigh.points()));
    minX = std::min(minX, getMinXOfCurvePointVector(res18.curveLow.points()));
    return minX;
}
