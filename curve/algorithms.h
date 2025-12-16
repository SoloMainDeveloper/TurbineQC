#pragma once

#include "project.h"
#include "functionparams.h"
#include "curvemachine.h"

class Algorithms {
public:
    static void enlargeCurveWithIntermediatePoints(QString figureName, const Function1Params *params, Project *project);
    static void getMiddleCurve(QString figureName, const Function18Params *params, Project *project);
    static void getChordLength(QString figureName, const Function18Params *params, Project *project);
    static void getMaxWidthCircle(QString figureName, const Function18Params *params, Project *project);
    static void getWidthOfEdges(QString figureName, double distanceFromLeadingEdge, double distanceFromTrailingEgde, Project *project);
};
