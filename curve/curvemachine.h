#pragma once

#include "functionparams.h"
#include "filesystem.h"
#include "ui_curvewindow.h"
#include "figure.h"

class CurveMachine {
public:
    static QVector<Point> enlargeCurveWithIntermediatePoints(QVector<Point> points, Function1Params params);
    static QVector<Point> getMiddleCurve(QVector<Point> points, Function18Params params);
    static double getChordLength(QVector<Point> points, Function18Params params);
    static CircleFigure getMaxWidthCircle(QVector<Point> points, Function18Params params); //
    static std::array<double, 2> getWidthOfEdges(QVector<Point> points, double distanceFromLeadingEdge, double distanceFromTrailingEgde);

private:
    static void makeCurveCalculations();
    static QStringList getOutput8thFunction(bool isClosed, Point puncturePoint, double diameter, QVector<Point> inputData);
    static QStringList getOutput7thFunction(bool isClosed, Point pointOfIntersection, double angle, QVector<Point> inputData);
    static Point getNearestPoint(Point pointOfIntersection, QVector<Point> middleCurve);
    static QVector<double> getEquationOfLine(Point firstPoint, Point secondPoint);
    static QVector<double> getEquationOfPerpendicular(Point firstPoint, Point secondPoint);
    static double getAngleBetweenLines(QVector<double> firstLine, QVector<double> secondLine);
    static QVector<Point> removeExtraPoints(QVector<Point> edgePoints, Point pointOfIntersection, double width);
};

