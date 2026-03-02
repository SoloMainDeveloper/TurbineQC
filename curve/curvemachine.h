#pragma once

#include "functionparams.h"
#include "functionresults.h"
#include "filesystem.h"
#include "figure.h"
#include "curveanalyzer.h"

class CurveMachine {
public:
    static CurveFigure enlargeCurveWithIntermediatePoints(const QVector<CurvePoint> points, const Function1Params params);
    static CurveFigure getMiddleCurve(const QVector<CurvePoint> points, const Function18Params params);
    static LineFigure getContactLine(const QVector<CurvePoint> points, const Function18Params params);
    static double getContactLineLength(const QVector<CurvePoint> points, const Function18Params params);
    static CircleFigure getMaxCircle(const QVector<CurvePoint> points, const Function18Params params);
    static std::pair<CurvePoint, CurvePoint> getWidthOfLeadingEdge(const QVector<CurvePoint> points, double distanceFromEdge);
    static std::pair<CurvePoint, CurvePoint> getWidthOfTrailingEdge(const QVector<CurvePoint> points, double distanceFromEdge);
    static double getDistanceBetweenPoints(CurvePoint firstPoint, CurvePoint secondPoint);
    static double getRadiusOfLeadingEdge(const QVector<CurvePoint> points, const Function18Params params);
    static double getRadiusOfTrailingEdge(const QVector<CurvePoint> points, const Function18Params params);
    static QVector<CurvePoint> mergePointClouds(const QVector<CurvePoint> &firstCloud, const QVector<CurvePoint> &secondCloud, double threshold = 0.02, bool needSort = true);
    static CurveFigure offsetCurve(const QVector<CurvePoint> curve, const Function3Params params);
    static CurveFigure calculateDeviations(const QVector<CurvePoint> nomCurve, const QVector<CurvePoint> measCurve, const Function4Params params);
    static Function6Result calculateBestFit(const QVector<CurvePoint> nominalCurve, const QVector<CurvePoint> measuredCurve, const Function6Params params);
    static Function21Result calculateBestFit(const QVector<CurvePoint> nominalCurve, const QVector<CurvePoint> measuredCurve, const Function21Params params);
    static CurveFigure regenerateCurve(const QVector<CurvePoint> curve, const Function19Params params);
    static Point getDirection(Point linePoint, double angularCoefficient);
    static CurveAnalyzer::CurveParts divideCurveIntoParts(const QVector<CurvePoint> curve, const Function18Params params);
    static QVector<CurvePoint> calculateStretch(const QVector<CurvePoint> &nominalCurve, const QVector<CurvePoint> &measuredCurve, const Function31Params &params);
    static QVector<CurvePoint> calculateCurveUsing3DVectors(const QVector<CurvePoint> &nominalCurve, const QVector<CurvePoint> &measuredCurve, const Function42Params &params);
    static double getMinX(const QVector<CurvePoint> points, const Function18Params params);

private:
    static Point normalizeVector(Point vector);
    static Point getOutput8thFunction(CurvePoint puncturePoint, double diameter, QVector<CurvePoint> inputData);
    static QVector<CurvePoint> getOutput7thFunction(Point pointOfIntersection, double angle, QVector<CurvePoint> inputData);
    static Point getNearestPoint(Point pointOfIntersection, QVector<CurvePoint> middleCurve);
    static QVector<double> getEquationOfLine(CurvePoint firstPoint, CurvePoint secondPoint);
    static QVector<double> getEquationOfPerpendicular(Point firstPoint, Point secondPoint);
    static double getAngleBetweenLines(QVector<double> firstLine, QVector<double> secondLine);
    static QVector<CurvePoint> removeExtraPoints(QVector<CurvePoint> edgePoints, Point pointOfIntersection, double width);
    static double correctRadius(const QVector<CurvePoint> &points, const QVector<double> &equationOfLine, CurvePoint puncturePoint, CurvePoint middleCurvePoint, double radius);
    static double getCoordY(const QVector<double> &equationOfLine, double coordX);
    static CurvePoint getNearestIntesectionPoint(const QVector<CurvePoint> &pointsOfIntersection, CurvePoint targetPoint);
    static void trimCloudFromStart(QVector<CurvePoint> &points, CurvePoint pointOfIntersection);
    static void trimCloudFromEnd(QVector<CurvePoint> &points, CurvePoint pointOfIntersection);
    static double getMinXOfCurvePointVector(const QVector<CurvePoint> &points);
};
