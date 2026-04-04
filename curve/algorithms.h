#pragma once

#include "curveanalyzer.h"
#include "functionparams.h"
#include "project.h"
#include "resultcompareflr.h"

using CurveParts = CurveAnalyzer::CurveParts;
using LEDirection = ReportSettings::LEDirection;

class Algorithms {
public:
    static void enlargeCurveWithIntermediatePoints(QString figureName, const Function1Params* params, Project* project);
    static void createMiddleCurve(QString parentName, QString figureName, const Function18Params* params, Project* project, QColor color = Qt::black);
    static CurveFigure getMiddleCurve(QString figureName, const Function18Params* params, Project* project);
    static double getMiddleCurveLength(QString curveName, const Function18Params* params, Project* project);
    static void createContactLine(QString parentName, QString figureName, const Function18Params* params, Project* project, QColor color = Qt::black);
    static LineFigure getContactLine(QString curveName, const Function18Params* params, Project* project);
    static void createMaxCircle(QString parentName, QString figureName, const Function18Params* params, Project* project, QColor color = Qt::black);
    static CircleFigure getMaxCircle(QString curveName, const Function18Params* params, Project* project);
    static QPair<PointFigure, PointFigure> getChord(QString curveName, const Function18Params* params, Project* project);
    static double getChordLength(QString curveName, const Function18Params* params, Project* project);
    static QPair<CurvePoint, CurvePoint> getWidthOfLeadingEdge(QString curveName, const Function18Params* params, double distanceFromEdge, Project* project);
    static QPair<CurvePoint, CurvePoint> getWidthOfTrailingEdge(QString curveName, const Function18Params* params, double distanceFromEdge, Project* project);
    static CircleFigure getRadiusOfLeadingEdge(QString curveName, const Function18Params* params, double degreeAngle, Project* project);
    static CircleFigure getRadiusOfTrailingEdge(QString curveName, const Function18Params* params, double degreeAngle, Project* project);
    static double getMinX(QString curveName, const Function18Params* params, Project* project);
    static void makeRadiusCorrection(QString figureName, QString figureNewName, const Function3Params* params);
    static bool tryMergePointClouds(QString firstCurveName, QString secondCurveName, QString resultName, double threshold, bool needSorted);
    static void calculateDeviations(QString nomCurveName, QString measCurveName, QString resultCurveName, const Function4Params* params);
    static void calculateBestFit(QString nomCurveName, QString measCurveName, QString resultCurveName, QString bestFitLineName, const Function6Params* params);
    static void calculateBestFit(QString nomCurveName, QString measCurveName, QString resultCurveName, QString bestFitLineName, const Function21Params* params);
    static void calculateCurve(QString figureName, QString newFigureName, const Function1Params* params, Project* project);
    static void regenerateCurve(QString figureName, QString newFigureName, const Function19Params* params);
    static void calculateConstantTolerances(QString figureName, double upperTolerance, double lowerTolerance, Project* project);
    static void calculateEdgesTolerance(QString figureName, int leadingEdgeDirection, double lEPercent, double tEPercent, double lEUpper, double lELower,
        double tEUpper, double tELower, double highEUpper, double highELower, double lowEUpper, double lowELower, Project* project);
    static void insertBestFitDimension(const QString& figureName, const QString& parentName, double x, double y, double z, bool isShowX, bool isShowY, bool isShowR);
    static CurveParts divideCurveIntoParts(const QString& curveName, const Function18Params* params, Project* project);
    static void calculateStretch(const QString& nomCurveName, const QString& measCurveName, const QString& resultCurveName, const Function31Params* params31, const Function6Params* params6, Project* project);
    static void calculateCurveUsing3DVectors(const QString& nomCurveName, const QString& measCurveName, const QString& resultCurveName, const Function42Params* params, double radiusCorrection, Project* project);
    static void calculateCurveUsing3DVectorsTest(const QString& nomCurveName, const QString& measCurveName, const QString& resultCurveName, const Function42Params* params, double radiusCorrection);
    static void calculateMeasuredParams(Project* project, std::shared_ptr<ReportSettings> reportSettings, const QString& globalMeasName);
    static double getDistanceBetweenPoints(Point firstPoint, Point secondPoint);
    static Point getMiddlePoint(Point firstPoint, Point secondPoint);
    static void reassembleWholeCurve(const QString& resultCurveName, const CurveParts& curveParts, LEDirection direction, Project* project);
    static void reassembleCurveWithoutTE(const QString& resultCurveName, const CurveParts& curveParts, LEDirection direction, Project* project);
    static void reassembleCurveWithoutEdges(const QString& resultCurveName, const CurveParts& curveParts, LEDirection direction, Project* project);
    static ResultCompareFLR* compareFLR(QString filepathFLR1, QString filepathFLR2, QString resultPath, int pointsStartIndex, double precision);

private:
    static ResultCompare2Params* compareTwoParamsFLR(QString first, QString second);
};
