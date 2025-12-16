#pragma once

#include "functionparams.h"
#include "filesystem.h"
#include "ui_curvewindow.h"
#include "figure.h"

class CurveMachine {
public:
    static CurveFigure enlargeCurveWithIntermediatePoints(const QVector<Point> points, const Function1Params params);
    static CurveFigure getMiddleCurve(const QVector<Point> points, const Function18Params params);
    static double getChordLength(const QVector<Point> points, const Function18Params params);
    static CircleFigure getMaxWidthCircle(const QVector<Point> points, const Function18Params params);
    static std::array<double, 2> getWidthOfEdges(const QVector<Point> points, double distanceFromLeadingEdge, double distanceFromTrailingEgde);

private:
    static Point getOutput8thFunction(Point puncturePoint, double diameter, QVector<Point> inputData);
    static QVector<Point> getOutput7thFunction(Point pointOfIntersection, double angle, QVector<Point> inputData);
    static Point getNearestPoint(Point pointOfIntersection, QVector<Point> middleCurve);
    static QVector<double> getEquationOfLine(Point firstPoint, Point secondPoint);
    static QVector<double> getEquationOfPerpendicular(Point firstPoint, Point secondPoint);
    static double getAngleBetweenLines(QVector<double> firstLine, QVector<double> secondLine);
    static QVector<Point> removeExtraPoints(QVector<Point> edgePoints, Point pointOfIntersection, double width);
};

