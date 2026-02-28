#pragma once

#include "project.h"
#include "functionparams.h"
#include "curvemachine.h"
#include "curvelibrary.h"

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
    static std::array<double, 2> getWidthOfEdges(QString figureName, double distanceFromLeadingEdge, double distanceFromTrailingEgde,
        Project *project, bool createSegmentLE = true, bool createSegmentTE = true);
    static std::array<double, 2> getRadiusOfEdges(QString figureName, const Function18Params *params, Project *project);
    static void makeRadiusCorrection(QString figureName, QString figureNewName, const Function3Params *params, Project *project);
    static bool tryMergePointClouds(QString first, QString second, QString resName, double threshold, bool needSorted, Project *project);
    static void calculateDeviations(QString nomCurveName, QString measCurveName, QString resultCurveName, const Function4Params *params, Project *project);
    static void calculateBestFit(QString nomCurveName, QString measCurveName, QString resultCurveName, const Function6Params *params, Project *project);
    static void calculateCurve(QString figureName, QString newFigureName, const Function1Params *params, Project *project);
    static void regenerateCurve(QString figureName, QString newFigureName, const Function19Params *params, Project *project);
    static void calculateConstantTolerances(QString figureName, double upperTolerance, double lowerTolerance, Project *project);
    static void calculateEdgesTolerance(QString figureName, int leadingEdgeDirection, double lEPercent, double tEPercent, double lEUpper, double lELower,
        double tEUpper, double tELower, double highEUpper, double highELower, double lowEUpper, double lowELower, Project *project);
};
