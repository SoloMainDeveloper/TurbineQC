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

double CurveMachine::getMiddleCurveLength(const QVector<CurvePoint> points, const Function18Params params) {
    auto mclPoints = getMiddleCurve(points, params).points();
    auto mclLength = 0.0;
    for(auto i = 0; i < mclPoints.size() - 1; i++) {
        mclLength += getDistanceBetweenPoints(
            Point(mclPoints[i]),
            Point(mclPoints[i + 1])
        );
    }
    return mclLength;
}

LineFigure CurveMachine::getContactLine(const QVector<CurvePoint> points, const Function18Params params) {
    return CurveLibrary::function18(points, params).contactLine;
}

CircleFigure CurveMachine::getMaxCircle(const QVector<CurvePoint> points, const Function18Params params) {
    return CurveLibrary::function18(points, params).maxCircle;
}

QPair<PointFigure, PointFigure> CurveMachine::getChord(const QVector<CurvePoint> points, const Function18Params params) {
    auto res18 = CurveLibrary::function18(points, params);

    auto [first, second] = res18.chord;
    auto contactLine = getEquationOfLine(res18.contactLEPoint.point(), res18.contactTEPoint.point());
    auto lineVectorX = res18.contactLEPoint.point().x - res18.contactTEPoint.point().x;
    auto lineVectorY = res18.contactLEPoint.point().y - res18.contactTEPoint.point().y;

    auto pointTE = first.point();
    auto xCoeffTE = lineVectorX / -lineVectorY;
    auto yCoeffTE = lineVectorY / -lineVectorY;
    auto freeCoeffTE = (lineVectorX * -pointTE.x + lineVectorY * -pointTE.y) / -lineVectorY;
    auto chordXTE = (freeCoeffTE - contactLine[2]) / (contactLine[0] - xCoeffTE);
    auto chordYTE = contactLine[0] * chordXTE + contactLine[2];
    auto p1Chord = PointFigure(QString(), CurvePoint(chordXTE, chordYTE));

    auto pointLE = second.point();
    auto xCoeffLE = lineVectorX / -lineVectorY;
    auto yCoeffLE = lineVectorY / -lineVectorY;
    auto freeCoeffLE = (lineVectorX * -pointLE.x + lineVectorY * -pointLE.y) / -lineVectorY;
    auto chordXLE = (freeCoeffLE - contactLine[2]) / (contactLine[0] - xCoeffLE);
    auto chordYLE = contactLine[0] * chordXLE + contactLine[2];
    auto p2Chord = PointFigure(QString(), CurvePoint(chordXLE, chordYLE));

    return QPair<PointFigure, PointFigure>(p1Chord, p2Chord);
}

double CurveMachine::getChordLength(const QVector<CurvePoint> points, const Function18Params params) {
    return CurveLibrary::function18(points, params).chordLength;
}

QPair<CurvePoint, CurvePoint> CurveMachine::getWidthOfLeadingEdge(const QVector<CurvePoint> points, const Function18Params params, double distanceFromEdge) {
    auto res18 = CurveLibrary::function18(points, params);
    auto &middleCurve = res18.middleCurve.points();
    auto maxWidth = res18.maxCircle.radius() * 2;
    auto LEpoint = res18.leadingEdgePoint.point();
    auto contactLE = res18.contactLEPoint.point();

    auto intersectionPoint = getOutput8thFunction(LEpoint, distanceFromEdge * 2, middleCurve);
    auto nearestPoint = getNearestPoint(intersectionPoint, middleCurve);

    CurvePoint point(contactLE.x + distanceFromEdge, contactLE.y);
    auto perpendicular = getEquationOfPerpendicular(nearestPoint, intersectionPoint);
    auto mainLine = getEquationOfLine(point, contactLE);
    auto angle = getAngleBetweenLines(perpendicular, mainLine);

    auto reassembledPoints = reassembleWholeCurve(CurveParts(
        res18.curveLE.points(),
        res18.curveTE.points(),
        res18.curveHigh.points(),
        res18.curveLow.points()),
        static_cast<LEDirection>(params.getLEDirection()));
    auto edgePoints = getOutput7thFunction(intersectionPoint, angle >= 0 ? M_PI - angle : -angle, reassembledPoints);

    return selectPairOfPoints(edgePoints, intersectionPoint, maxWidth);;
}

QPair<CurvePoint, CurvePoint> CurveMachine::getWidthOfTrailingEdge(const QVector<CurvePoint> points, const Function18Params params, double distanceFromEdge) {
    auto res18 = CurveLibrary::function18(points, params);
    auto &middleCurve = res18.middleCurve.points();
    auto maxWidth = res18.maxCircle.radius() * 2;
    auto TEpoint = res18.trailingEdgePoint.point();
    auto contactTE = res18.contactTEPoint.point();

    auto intersectionPoint = getOutput8thFunction(TEpoint, distanceFromEdge * 2, middleCurve);
    auto nearestPoint = getNearestPoint(intersectionPoint, middleCurve);

    CurvePoint point(contactTE.x + distanceFromEdge, contactTE.y);
    auto perpendicular = getEquationOfPerpendicular(nearestPoint, intersectionPoint);
    auto mainLine = getEquationOfLine(point, contactTE);
    auto angle = getAngleBetweenLines(perpendicular, mainLine);

    auto reassembledPoints = reassembleWholeCurve(CurveParts(
        res18.curveLE.points(),
        res18.curveTE.points(),
        res18.curveHigh.points(),
        res18.curveLow.points()), 
        static_cast<LEDirection>(params.getLEDirection()));
    auto pointsOfEdge = getOutput7thFunction(intersectionPoint, angle >= 0 ? M_PI - angle : -angle, reassembledPoints);

    return selectPairOfPoints(pointsOfEdge, intersectionPoint, maxWidth);
}

double CurveMachine::getDistanceBetweenPoints(Point firstPoint, Point secondPoint) {
    return hypot(firstPoint.x - secondPoint.x, firstPoint.y - secondPoint.y);
}

Point CurveMachine::getMiddlePoint(Point firstPoint, Point secondPoint) {
    return Point((firstPoint.x + secondPoint.x) / 2, (firstPoint.y + secondPoint.y) / 2);;
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
    Point nearestPoint;
    auto maxDistance = hypot(pointOfIntersection.x - middleCurve[0].x, pointOfIntersection.y - middleCurve[0].y);
    for(auto i = 1; i < middleCurve.length(); i++) {
        auto currentDistance = hypot(pointOfIntersection.x - middleCurve[i].x, pointOfIntersection.y - middleCurve[i].y);
        if(currentDistance < maxDistance) {
            nearestPoint = Point(middleCurve[i]);
            maxDistance = currentDistance;
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

QPair<CurvePoint, CurvePoint> CurveMachine::selectPairOfPoints(QVector<CurvePoint> edgePoints, Point pointOfIntersection, double maxWidth) {
    QVector<CurvePoint> selectedPoints;
    for(auto &point : edgePoints) {
        if(hypot(pointOfIntersection.x - point.x, pointOfIntersection.y - point.y) < maxWidth) {
            selectedPoints.append(point);
        }
    }

    if(selectedPoints.length() < 2) {
        return QPair<CurvePoint, CurvePoint>();
    }
    return QPair<CurvePoint, CurvePoint>(selectedPoints[0], selectedPoints[1]);
}

CircleFigure CurveMachine::getRadiusOfLeadingEdge(const QVector<CurvePoint> points, const Function18Params params, double degreeAngle) {
    auto res18 = CurveLibrary::function18(points, params);
    auto puncturePointLE = res18.leadingEdgePoint.point();
    auto maxRadius = res18.maxCircle.radius();
    auto middleCurve = res18.middleCurve.points();
    auto middleCurvePointLE = middleCurve[middleCurve.length() - 2];

    auto equationOfLine = getEquationOfLine(puncturePointLE, middleCurvePointLE);
    auto radiusOfLE = correctRadius(points, equationOfLine, puncturePointLE, middleCurvePointLE, maxRadius, degreeAngle / 2);

    return radiusOfLE;
}

CircleFigure CurveMachine::getRadiusOfTrailingEdge(const QVector<CurvePoint> points, const Function18Params params, double degreeAngle) {
    auto res18 = CurveLibrary::function18(points, params);
    auto puncturePointTE = res18.trailingEdgePoint.point();
    auto maxRadius = res18.maxCircle.radius();
    auto middleCurve = res18.middleCurve.points();
    auto middleCurvePointTE = middleCurve[1];

    auto equationOfLineTE = getEquationOfLine(puncturePointTE, middleCurvePointTE);
    auto radiusOfLE = correctRadius(points, equationOfLineTE, puncturePointTE, middleCurvePointTE, maxRadius, degreeAngle / 2);

    return radiusOfLE;
}

CircleFigure CurveMachine::correctRadius(const QVector<CurvePoint> &points, const QVector<double> &equationOfLine, CurvePoint puncturePoint, CurvePoint middleCurvePoint, double radius, double degreeAngle) {
    auto previousRadius = 0.0;
    auto angularCoefficient = equationOfLine[0];
    auto circle = CircleFigure();

    while(true) {
        auto coordXOfPoint = puncturePoint.x < middleCurvePoint.x ?
            puncturePoint.x + radius * qCos(abs(qAtan(angularCoefficient))) :
            puncturePoint.x - radius * qCos(abs(qAtan(angularCoefficient)));
        auto coordYOfPoint = getCoordY(equationOfLine, coordXOfPoint);
        Point pointOfLine(coordXOfPoint, coordYOfPoint);

        auto k1 = qTan(qDegreesToRadians(degreeAngle) + qAtan(angularCoefficient));
        auto intersectionPoints = CurveLibrary::function7(points, Function7Params(qAtan(k1), pointOfLine.x, pointOfLine.y)).contactPoints;
        auto firstNearestPoint = getNearestIntesectionPoint(intersectionPoints, puncturePoint);

        auto k2 = qTan(-qDegreesToRadians(degreeAngle) + qAtan(angularCoefficient));
        intersectionPoints = CurveLibrary::function7(points, Function7Params(qAtan(k2), pointOfLine.x, pointOfLine.y)).contactPoints;
        auto secondNearstPoint = getNearestIntesectionPoint(intersectionPoints, puncturePoint);

        circle = CurveLibrary::function12(QVector<CurvePoint>{ firstNearestPoint, puncturePoint, secondNearstPoint }, Function12Params()).circle;
        radius = circle.radius();

        if(abs(previousRadius - radius) <= 0.0005) {
            break;
        }
        previousRadius = radius;
    }
    return circle;
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
    auto rightPoints = pointsOfFirstCloud.mid(lengthOfFirstCloud / 2);

    auto leftPointsOfIntersection = CurveLibrary::function14(leftPoints, pointsOfSecondCloud, Function14Params()).curve.points();
    auto rightPointsOfIntersection = CurveLibrary::function14(rightPoints, pointsOfSecondCloud, Function14Params()).curve.points();

    if(leftPointsOfIntersection.length() == 0 || rightPointsOfIntersection.length() == 0) {
        return QVector<CurvePoint>();
    }

    /*QVector<CurvePoint> processedLeftPoints = { leftPointsOfIntersection[0] };
    for(auto i = 1; i < leftPointsOfIntersection.length(); i++) {
        auto previousPoint = leftPointsOfIntersection[i - 1];
        auto currentPoint = leftPointsOfIntersection[i];
        auto distance = hypot(previousPoint.x - currentPoint.x, previousPoint.y - currentPoint.y);
        if(distance > threshold) processedLeftPoints.append(currentPoint);
    }
    QVector<CurvePoint> processedRightPoints = { rightPointsOfIntersection[0] };
    for(auto i = 1; i < rightPointsOfIntersection.length(); i++) {
        auto previousPoint = rightPointsOfIntersection[i - 1];
        auto currentPoint = rightPointsOfIntersection[i];
        auto distance = hypot(previousPoint.x - currentPoint.x, previousPoint.y - currentPoint.y);
        if(distance > threshold) processedRightPoints.append(currentPoint);
    } */

    /* auto leftMiddlePoint = processedLeftPoints.length() % 2 == 0 ?
         processedLeftPoints[processedLeftPoints.length() / 2 - 1] :
         processedLeftPoints[processedLeftPoints.length() / 2];
     auto rightMiddlePoint = processedRightPoints.length() % 2 == 0 ?
         processedRightPoints[processedRightPoints.length() / 2 - 1] :
         processedRightPoints[processedRightPoints.length() / 2];

     trimCloudFromStart(pointsOfFirstCloud, leftMiddlePoint);
     trimCloudFromEnd(pointsOfFirstCloud, rightMiddlePoint);

     trimCloudFromStart(pointsOfSecondCloud, rightMiddlePoint);
     trimCloudFromEnd(pointsOfSecondCloud, leftMiddlePoint);*/

     /*trimCloudFromStart(pointsOfFirstCloud, leftPointsOfIntersection[leftPointsOfIntersection.length() / 2]);
     trimCloudFromEnd(pointsOfFirstCloud, rightPointsOfIntersection[rightPointsOfIntersection.length() / 2]);

     trimCloudFromStart(pointsOfSecondCloud, rightPointsOfIntersection[rightPointsOfIntersection.length() / 2]);
     trimCloudFromEnd(pointsOfSecondCloud, leftPointsOfIntersection[leftPointsOfIntersection.length() / 2]);*/

     /*trimCloudFromStart(pointsOfFirstCloud, leftPointsOfIntersection[leftPointsOfIntersection.length() / 2]);
     trimCloudFromStart(pointsOfSecondCloud, rightPointsOfIntersection[rightPointsOfIntersection.length() / 2]);
     trimCloudFromEnd(pointsOfFirstCloud, pointsOfSecondCloud[0]);
     trimCloudFromEnd(pointsOfSecondCloud, pointsOfFirstCloud[0]);*/

    trimCloudFromStart(pointsOfFirstCloud, leftPointsOfIntersection[leftPointsOfIntersection.length() / 2]);
    trimCloudFromEnd(pointsOfFirstCloud, rightPointsOfIntersection[rightPointsOfIntersection.length() / 2]);
    trimCloudFromStart(pointsOfSecondCloud, pointsOfFirstCloud[pointsOfFirstCloud.length() - 1]);
    trimCloudFromEnd(pointsOfSecondCloud, pointsOfFirstCloud[0]);
    if(hypot(pointsOfSecondCloud[pointsOfSecondCloud.length() - 2].x - pointsOfSecondCloud[pointsOfSecondCloud.length() - 1].x,
        pointsOfSecondCloud[pointsOfSecondCloud.length() - 2].y - pointsOfSecondCloud[pointsOfSecondCloud.length() - 1].y) >
        hypot(pointsOfSecondCloud[pointsOfSecondCloud.length() - 2].x - pointsOfFirstCloud[0].x,
        pointsOfSecondCloud[pointsOfSecondCloud.length() - 2].y - pointsOfFirstCloud[0].y)) {
        pointsOfSecondCloud.removeAt(pointsOfSecondCloud.length() - 1);
    }

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
    auto minDistance = hypot(pointOfIntersection.x - points[0].x, pointOfIntersection.y - points[0].y);
    auto index = 0;
    for(auto i = 0; i < points.length(); i++) {
        auto distance = hypot(pointOfIntersection.x - points[i].x, pointOfIntersection.y - points[i].y);
        if(distance < minDistance) {
            minDistance = distance;
            index = i;
        }
    }
    //if((hypot(pointOfIntersection.x - points[index - 1].x, pointOfIntersection.y - points[index - 1].y) <
    //    hypot(points[index - 1].x - points[index].x, points[index - 1].y - points[index].y)) /*&& (index - 1) >= 0*/) {
    //    points = points.mid(index - 1);
    //    qDebug() << "start 1";
    //} else {
    //    points = points.mid(index);
    //    qDebug() << "start 2";
    //}
    points = points.mid(index);
}

void CurveMachine::trimCloudFromEnd(QVector<CurvePoint> &points, CurvePoint pointOfIntersection) {
    auto minDistance = hypot(pointOfIntersection.x - points[points.length() - 1].x, pointOfIntersection.y - points[points.length() - 1].y);
    auto index = points.length() - 1;
    for(auto i = points.length() - 1; i >= 0; i--) {
        auto distance = hypot(pointOfIntersection.x - points[i].x, pointOfIntersection.y - points[i].y);
        if(distance < minDistance) {
            minDistance = distance;
            index = i;
        }
    }
    /*if(hypot(pointOfIntersection.x - points[index + 1].x, pointOfIntersection.y - points[index + 1].y) <
        hypot(points[index + 1].x - points[index].x, points[index + 1].y - points[index].y)) {
        points = points.mid(0, index + 1);
    } else {
        points = points.mid(0, index);
    }*/

    //if(hypot(pointOfIntersection.x - points[index - 1].x, pointOfIntersection.y - points[index - 1].y) <
    //    hypot(points[index - 1].x - points[index].x, points[index - 1].y - points[index].y)) {
    //    points = points.mid(0, index);
    //    //qDebug() << "end 1";
    //} else {
    //    points = points.mid(0, index + 1);
    //    //qDebug() << "end 2";
    //}

    points = points.mid(0, index + 1);
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

Function21Result CurveMachine::calculateBestFit(const QVector<CurvePoint> nominalCurve, const QVector<CurvePoint> measuredCurve, const Function21Params params, double constTolerance) {
    if(params.isClosedCurve()) {
        return CurveLibrary::function21(measuredCurve, nominalCurve, nominalCurve, params);
    }

    QVector<CurvePoint> upperNomPoints;
    QVector<CurvePoint> lowerNomPoints;
    for(auto &point : nominalCurve) {
        upperNomPoints.append(CurvePoint(
            point.x + constTolerance * point.i,
            point.y + constTolerance * point.j,
            point.z + constTolerance * point.k,
            point.i, point.j, point.k)
        );
        lowerNomPoints.append(CurvePoint(
            point.x - constTolerance * point.i,
            point.y - constTolerance * point.j,
            point.z - constTolerance * point.k,
            point.i, point.j, point.k)
        );
    }
    return CurveLibrary::function21(measuredCurve, upperNomPoints, lowerNomPoints, params);
}

Point CurveMachine::getDirection(Point linePoint, double angularCoefficient) {
    auto coeffB = linePoint.y - linePoint.x * angularCoefficient;
    auto x = 1;
    auto y = x * angularCoefficient + coeffB;
    auto nextLinePoint = Point(x, y);

    auto direction = Point(nextLinePoint.x - linePoint.x, nextLinePoint.y - linePoint.y);
    return normalizeVector(direction);
}

CurveParts CurveMachine::divideCurveIntoParts(const QVector<CurvePoint> curve, const Function18Params params) {
    auto res18 = CurveLibrary::function18(curve, params);
    return CurveParts(
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

QVector<CurvePoint> CurveMachine::calculateCurveUsing3DVectors(const QVector<CurvePoint> &nominalCurve, const QVector<CurvePoint> &measuredCurve, const Function42Params &params, double radiusCorrection) {
    auto nominalBallCenter = calculateBallCenterPoints(nominalCurve, radiusCorrection);
    auto resultPoints = CurveLibrary::function42(measuredCurve, nominalBallCenter, params).curve.points();

    if(resultPoints[0].k == 0) {
        return resultPoints;
    }
    auto updatedPoints = shiftPointsAtNonZeroK(resultPoints, radiusCorrection);
    return updatedPoints;
}

QVector<CurvePoint> CurveMachine::calculateBallCenterPoints(const QVector<CurvePoint> &points, double radiusCorrection) {
    QVector<CurvePoint> ballCenterPoints;
    for(auto &point : points) {
        CurvePoint resultPoint;

        auto projectionLength = sqrt(1 - pow(point.k, 2));

        resultPoint.x = point.x + radiusCorrection * (point.i / projectionLength);
        resultPoint.y = point.y + radiusCorrection * (point.j / projectionLength);
        resultPoint.z = point.z;
        resultPoint.i = point.i;
        resultPoint.j = point.j;
        resultPoint.k = point.k;

        ballCenterPoints.append(resultPoint);
    }
    return ballCenterPoints;
}

QVector<CurvePoint> CurveMachine::shiftPointsAtNonZeroK(const QVector<CurvePoint> &points, double radiusCorrection) {
    QVector<CurvePoint> updatedPoints;
    for(auto &point : points) {
        CurvePoint updatedPoint;

        auto roundedNumber = std::round(point.z);
        auto differenceZ = roundedNumber - point.z;
        auto lengthOf2DVector = hypot(point.i, point.j);
        auto normalizedK = point.k / lengthOf2DVector;
        auto offset = (normalizedK * differenceZ) + ((1 / lengthOf2DVector - 1) * radiusCorrection);

        auto normalizedI = point.i / lengthOf2DVector;
        updatedPoint.x = point.x - normalizedI * offset;

        auto normalizedJ = point.j / lengthOf2DVector;
        updatedPoint.y = point.y - normalizedJ * offset;

        updatedPoint.z = point.z;
        updatedPoint.i = point.i;
        updatedPoint.j = point.j;
        updatedPoint.k = point.k;

        updatedPoints.append(updatedPoint);
    }
    return updatedPoints;
}

double CurveMachine::getMinX(const QVector<CurvePoint> points, const Function18Params params) {
    auto res18 = CurveLibrary::function18(points, params);
    auto minX = getMinXOfCurvePointVector(res18.curveLE.points());
    minX = std::min(minX, getMinXOfCurvePointVector(res18.curveTE.points()));
    minX = std::min(minX, getMinXOfCurvePointVector(res18.curveHigh.points()));
    minX = std::min(minX, getMinXOfCurvePointVector(res18.curveLow.points()));
    return minX;
}

QVector<CurvePoint> CurveMachine::reassembleWholeCurve(const CurveParts &curveParts, LEDirection direction) {
    auto curvePointsOfLE = curveParts.pointsOfLE;
    auto curvePointsOfTE = curveParts.pointsOfTE;
    auto lowCurvePoints = curveParts.pointsOfLow;
    auto highCurvePoints = curveParts.pointsOfHigh;

    QVector<CurvePoint> resultPoints;
    if(direction == LEDirection::PlusX || direction == LEDirection::MinusY || direction == LEDirection::Auto) {
        curvePointsOfLE.removeFirst();
        lowCurvePoints.removeFirst();
        curvePointsOfTE.removeFirst();
        curvePointsOfTE.removeLast();
        resultPoints = highCurvePoints + curvePointsOfLE + lowCurvePoints + curvePointsOfTE;
    } else {           
        curvePointsOfLE.removeFirst();
        highCurvePoints.removeFirst();
        curvePointsOfTE.removeFirst();
        curvePointsOfTE.removeLast();
        resultPoints = lowCurvePoints + curvePointsOfLE + highCurvePoints + curvePointsOfTE;
    }
    return resultPoints;
}

QVector<CurvePoint> CurveMachine::reassembleCurveWithoutTE(const CurveParts &curveParts, LEDirection direction) {
    auto curvePointsOfLE = curveParts.pointsOfLE;
    auto lowCurvePoints = curveParts.pointsOfLow;
    auto highCurvePoints = curveParts.pointsOfHigh;

    QVector<CurvePoint> resultPoints;
    if(direction == LEDirection::PlusX || direction == LEDirection::MinusY || direction == LEDirection::Auto) {
        curvePointsOfLE.removeFirst();
        lowCurvePoints.removeFirst();
        resultPoints = highCurvePoints + curvePointsOfLE + lowCurvePoints;
    } else {
        curvePointsOfLE.removeFirst();
        highCurvePoints.removeFirst();
        resultPoints = lowCurvePoints + curvePointsOfLE + highCurvePoints;
    }
    return resultPoints;
}

QVector<CurvePoint> CurveMachine::reassembleCurveWithoutEdges(const CurveParts &curveParts, LEDirection direction) {
    //Maybe there is a dependence on the direction
    auto highCurvePoints = curveParts.pointsOfHigh;
    auto lowCurvePoints = curveParts.pointsOfLow;
    return highCurvePoints + lowCurvePoints;
}
