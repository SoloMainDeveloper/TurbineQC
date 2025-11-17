#pragma once
#include "functionresults.h"
#include "functionparams.h"
#include "figure.h"
#include "filesystem.h"
#include "ui_curvewindow.h"

class CurveLibrary {
public:
    static Function1Result function1(QVector<Point> points, Function1Params params); //enlarging
    static Function2Result function2(QVector<Point> points, Function2Params params); //invert operations
    static Function3Result function3(QVector<Point> points, Function3Params params); //2D offset (radius correction)
    static Function4Result function4(QVector<Point> curve1, QVector<Point> curve2, Function4Params params); //2D Comparison
    static Function5Result function5(QVector<Point> points, Function5Params params); //Display curves
    static Function6Result function6(QVector<Point> curve1, QVector<Point> curve2, Function6Params params); //2D best-fit
    static Function7Result function7(QVector<Point> points, Function7Params params); //intersection with line
    static Function8Result function8(QVector<Point> points, Function8Params params); //intersection with circle
    static Function9Result function9(QVector<Point> points, Function9Params params); //middle curve
    static Function10Result function10(QVector<Point> points, Function10Params params); //circle centering
    static Function11Result function11(QVector<Point> points, Function11Params params); //line fit
    static Function12Result function12(QVector<Point> points, Function12Params params); //circle fit
    static Function13Result function13(QVector<Point> points, Function13Params params); //filter fourier
    static Function14Result function14(QVector<Point> curve1, QVector<Point> curve2, Function14Params params); //intersection between 2 curves
    static Function15Result function15(QVector<Point> points, Function15Params params); //perimeter, square, mass centre
    static Function16Result function16(QVector<Point> points, Function16Params params); //auto curve split
    static Function17Result function17(QVector<Point> points, Function17Params params); //shift points
    static Function18Result function18(QVector<Point> points, Function18Params params); //blade

private:
    static void runCurve();
    static void makeCurveCalculations(QList<QVector<Point>> elements, FunctionParams* params);
};
