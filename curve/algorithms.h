#pragma once

#include "project.h"
#include "functionparams.h"
#include "curvemachine.h"
#include "curvelibrary.h"

class Algorithms {
public:
    static void enlargeCurveWithIntermediatePoints(QString figureName, const Function1Params *params, Project *project);
    static void getMiddleCurve(QString figureName, const Function18Params *params, Project *project);
    static double getChordLength(QString figureName, const Function18Params *params, Project *project);
    static void getMaxWidthCircle(QString figureName, const Function18Params *params, Project *project);
    static std::array<double, 2> getWidthOfEdges(QString figureName, double distanceFromLeadingEdge, double distanceFromTrailingEgde,
        Project *project, bool createSegmentLE = true, bool createSegmentTE = true);
    static std::array<double, 2> getRadiusOfEdges(QString figureName, const Function18Params *params, Project *project);
    static void makeRadiusCorrection(QString figureName, QString figureNewName, const Function3Params *params, Project *project);
    static QVector<CurvePoint> mergePointClouds(QString nameOfFirstCloud, QString nameOfSecondCloud, double threshold, bool needSorted, Project *project);
    static void turbineRun(QString figureName, const Function18Params *params, Project *project, bool needMaxDia, bool needMiddleCurve, bool needChord);
    static void calculateDeviations(QString nomCurveName, QString measCurveName, QString resultCurveName, const Function4Params *params, Project *project);

};
