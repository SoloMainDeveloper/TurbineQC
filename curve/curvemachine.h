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

private:
    static void makeCurveCalculations();
};

