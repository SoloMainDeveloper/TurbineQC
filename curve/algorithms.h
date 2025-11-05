#pragma once

#include "project.h"
#include "functionparams.h"
#include "curvemachine.h"

class Algorithms {
public:
    static QVector<Point> enlargeCurveWithIntermediatePoints(QString figureName, const Function1Params *params, Project *project);
    static QVector<Point> getMiddleCurve(QString figureName, const Function18Params *params, Project *project);
    static void getChordLength(QString figureName, const Function18Params *params, Project *project);
    static CircleFigure getMaxWidthCircle(QString figureName, const Function18Params *params, Project *project);
};