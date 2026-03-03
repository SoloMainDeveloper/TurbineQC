#pragma once

#include "functionparams.h"
#include "functionresults.h"
#include "filesystem.h"
#include "figure.h"
#include "curveanalyzer.h"

using CurveParts = CurveAnalyzer::CurveParts;
using LEDirection = ReportSettings::LEDirection;

class CurveMachine {
public:
    static CurveFigure enlargeCurveWithIntermediatePoints(const QVector<CurvePoint> points, const Function1Params params);
    static CurveFigure getMiddleCurve(const QVector<CurvePoint> points, const Function18Params params);
    static double getMiddleCurveLength(const QVector<CurvePoint> points, const Function18Params params);
    static LineFigure getContactLine(const QVector<CurvePoint> points, const Function18Params params);
    static CircleFigure getMaxCircle(const QVector<CurvePoint> points, const Function18Params params);
    static QPair<PointFigure, PointFigure> getChord(const QVector<CurvePoint> points, const Function18Params params);
    static double getChordLength(const QVector<CurvePoint> points, const Function18Params params);
    static QPair<CurvePoint, CurvePoint> getWidthOfLeadingEdge(const QVector<CurvePoint> points, const Function18Params params, double distanceFromEdge);
    static QPair<CurvePoint, CurvePoint> getWidthOfTrailingEdge(const QVector<CurvePoint> points, const Function18Params params, double distanceFromEdge);
    static double getDistanceBetweenPoints(Point firstPoint, Point secondPoint);
    static Point getMiddlePoint(Point firstPoint, Point secondPoint);
    static CircleFigure getRadiusOfLeadingEdge(const QVector<CurvePoint> points, const Function18Params params, double degreeAngle);
    static CircleFigure getRadiusOfTrailingEdge(const QVector<CurvePoint> points, const Function18Params params, double degreeAngle);
    static QVector<CurvePoint> mergePointClouds(const QVector<CurvePoint> &firstCloud, const QVector<CurvePoint> &secondCloud, double threshold = 0.02, bool needSort = true);
    static CurveFigure offsetCurve(const QVector<CurvePoint> curve, const Function3Params params);
    static CurveFigure calculateDeviations(const QVector<CurvePoint> nomCurve, const QVector<CurvePoint> measCurve, const Function4Params params);
    static Function6Result calculateBestFit(const QVector<CurvePoint> nominalCurve, const QVector<CurvePoint> measuredCurve, const Function6Params params);
    static Function21Result calculateBestFit(const QVector<CurvePoint> nominalCurve, const QVector<CurvePoint> measuredCurve, const Function21Params params, double constTolerance);
    static CurveFigure regenerateCurve(const QVector<CurvePoint> curve, const Function19Params params);
    static Point getDirection(Point linePoint, double angularCoefficient);
    static CurveParts divideCurveIntoParts(const QVector<CurvePoint> curve, const Function18Params params);
    static QVector<CurvePoint> calculateStretch(const QVector<CurvePoint> &nominalCurve, const QVector<CurvePoint> &measuredCurve, const Function31Params &params);
    static QVector<CurvePoint> calculateCurveUsing3DVectors(const QVector<CurvePoint> &nominalCurve, const QVector<CurvePoint> &measuredCurve, const Function42Params &params, double radiusCorrection);
    static double getMinX(const QVector<CurvePoint> points, const Function18Params params);
    static QVector<CurvePoint> reassembleWholeCurve(const CurveParts &curveParts, LEDirection direction);
    static QVector<CurvePoint> reassembleCurveWithoutTE(const CurveParts &curveParts, LEDirection direction);
    static QVector<CurvePoint> reassembleCurveWithoutEdges(const CurveParts &curveParts, LEDirection direction);

private:
    static Point normalizeVector(Point vector);
    static Point getOutput8thFunction(CurvePoint puncturePoint, double diameter, QVector<CurvePoint> inputData);
    static QVector<CurvePoint> getOutput7thFunction(Point pointOfIntersection, double angle, QVector<CurvePoint> inputData);
    static Point getNearestPoint(Point pointOfIntersection, QVector<CurvePoint> middleCurve);
    static QVector<double> getEquationOfLine(CurvePoint firstPoint, CurvePoint secondPoint);
    static QVector<double> getEquationOfPerpendicular(Point firstPoint, Point secondPoint);
    static double getAngleBetweenLines(QVector<double> firstLine, QVector<double> secondLine);
    static QPair<CurvePoint, CurvePoint> selectPairOfPoints(QVector<CurvePoint> edgePoints, Point pointOfIntersection, double maxWidth);
    static CircleFigure correctRadius(const QVector<CurvePoint> &points, const QVector<double> &equationOfLine, 
        CurvePoint puncturePoint, CurvePoint middleCurvePoint, double radius, double degreeAngle);
    static double getCoordY(const QVector<double> &equationOfLine, double coordX);
    static CurvePoint getNearestIntesectionPoint(const QVector<CurvePoint> &pointsOfIntersection, CurvePoint targetPoint);
    static void trimCloudFromStart(QVector<CurvePoint> &points, CurvePoint pointOfIntersection);
    static void trimCloudFromEnd(QVector<CurvePoint> &points, CurvePoint pointOfIntersection);
    static double getMinXOfCurvePointVector(const QVector<CurvePoint> &points);
    static QVector<CurvePoint> calculateBallCenterPoints(const QVector<CurvePoint> &points, double radiusCompensation);
    static QVector<CurvePoint> shiftPointsAtNonZeroK(const QVector<CurvePoint> &points, double radiusCompensation);
};
