#pragma once

#include "project.h"
#include "functionparams.h"
#include "curvemachine.h"
#include "curvelibrary.h"
#include "curveanalyzer.h"

class Algorithms {
public:
    static void enlargeCurveWithIntermediatePoints(QString figureName, const Function1Params *params, Project *project);
    static void createMiddleCurve(QString parentName, QString figureName, const Function18Params *params, Project *project, QColor color = Qt::black);
    static CurveFigure getMiddleCurve(QString figureName, const Function18Params *params, Project *project);
    static void createContactLine(QString parentName, QString figureName, const Function18Params * params, Project * project, QColor color = Qt::black);
    static LineFigure getContactLine(QString figureName, const Function18Params *params, Project *project);
    static double getContactLineLength(QString figureName, const Function18Params *params, Project *project);
    static void createMaxCircle(QString parentName, QString figureName, const Function18Params *params, Project *project, QColor color = Qt::black);
    static CircleFigure getMaxCircle(QString figureName, const Function18Params *params, Project *project);
    static double getWidthOfLeadingEdge(QString figureName, Project *project, double distanceFromEdge, bool createSegment = true);
    static double getWidthOfTrailingEdge(QString figureName, Project *project, double distanceFromEdge, bool createSegment = true);
    static double getRadiusOfLeadingEdge(QString figureName, const Function18Params *params, Project *project);
    static double getRadiusOfTrailingEdge(QString figureName, const Function18Params *params, Project *project);
    static double getMinX(QString figureName, const Function18Params *params, Project *project);
    static double createMinX(QString figureName, const Function18Params *params, Project *project, QString dimName, QColor color = Qt::black);
    static void makeRadiusCorrection(QString figureName, QString figureNewName, const Function3Params *params, Project *project);
    static bool tryMergePointClouds(QString firstCurveName, QString secondCurveName, QString resultName, double threshold, bool needSorted, Project *project);
    static void calculateDeviations(QString nomCurveName, QString measCurveName, QString resultCurveName, const Function4Params *params, Project *project);
    static void calculateBestFit(QString nomCurveName, QString measCurveName, QString resultCurveName, QString bestFitLineName, const Function6Params *params, Project *project);
    static void calculateBestFit(QString nomCurveName, QString measCurveName, QString resultCurveName, QString bestFitLineName, const Function21Params *params, Project *project);
    static void calculateCurve(QString figureName, QString newFigureName, const Function1Params *params, Project *project);
    static void regenerateCurve(QString figureName, QString newFigureName, const Function19Params *params, Project *project);
    static void calculateConstantTolerances(QString figureName, double upperTolerance, double lowerTolerance, Project *project);
    static void calculateEdgesTolerance(QString figureName, int leadingEdgeDirection, double lEPercent, double tEPercent, double lEUpper, double lELower,
        double tEUpper, double tELower, double highEUpper, double highELower, double lowEUpper, double lowELower, Project *project);
    static void insertBestFitDimension(const QString &figureName, const QString &parentName, double x, double y, double z, bool isShowX, bool isShowY, bool isShowR, Project *project);
    static CurveAnalyzer::CurveParts divideCurveIntoParts(const QString &curveName, const Function18Params *params, Project *project);
    static void calculateStretch(const QString &nomCurveName, const QString &measCurveName, const QString &resultCurveName, const Function31Params *params31, const Function6Params *params6, Project *project);
    static void calculateCurveUsing3DVectors(const QString &nomCurveName, const QString &measCurveName, const QString &resultCurveName, const Function42Params *params, Project *project);
    static void calculateMeasuredParams(Project *project, std::shared_ptr<ReportSettings> reportSettings, const QString &globalMeasName);
};
