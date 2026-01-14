#pragma once
#include "functionresults.h"
#include "functionparams.h"
#include "figure.h"
#include "filesystem.h"
#include "ui_curvewindow.h"

class CurveLibrary {
public:
    static Function1Result function1(QVector<CurvePoint> points, Function1Params params); //enlarging
    static Function2Result function2(QVector<CurvePoint> points, Function2Params params); //invert operations
    static Function3Result function3(QVector<CurvePoint> points, Function3Params params); //2D offset (radius correction)
    static Function4Result function4(QVector<CurvePoint> curve1, QVector<CurvePoint> curve2, Function4Params params); //2D Comparison
    static Function5Result function5(QVector<CurvePoint> points, Function5Params params); //Display curves
    static Function6Result function6(QVector<CurvePoint> curve1, QVector<CurvePoint> curve2, Function6Params params); //2D best-fit
    static Function7Result function7(QVector<CurvePoint> points, Function7Params params); //intersection with line
    static Function8Result function8(QVector<CurvePoint> points, Function8Params params); //intersection with circle
    static Function9Result function9(QVector<CurvePoint> points, Function9Params params); //middle curve
    static Function10Result function10(QVector<CurvePoint> points, Function10Params params); //circle centering
    static Function11Result function11(QVector<CurvePoint> points, Function11Params params); //line fit
    static Function12Result function12(QVector<CurvePoint> points, Function12Params params); //circle fit
    static Function13Result function13(QVector<CurvePoint> points, Function13Params params); //filter fourier
    static Function14Result function14(QVector<CurvePoint> curve1, QVector<CurvePoint> curve2, Function14Params params); //intersection between 2 curves
    static Function15Result function15(QVector<CurvePoint> points, Function15Params params); //perimeter, square, mass centre
    static Function16Result function16(QVector<CurvePoint> points, Function16Params params); //auto curve split
    static Function17Result function17(QVector<CurvePoint> points, Function17Params params); //shift points
    static Function18Result function18(QVector<CurvePoint> points, Function18Params params); //blade
    static Function19Result function19(QVector<CurvePoint> points, Function19Params params); //specified number of points
    static Function21Result function21(QVector<CurvePoint> curve1, QVector<CurvePoint> curve2, QVector<CurvePoint> curve3, Function21Params params); //best-fit analog
    static Function31Result function31(QVector<CurvePoint> curve1, QVector<CurvePoint> curve2, Function31Params params); //stretch LE, TE

private:
    static void runCurve();
    static void makeCurveCalculations(QList<QVector<CurvePoint>> elements, FunctionParams* params);
};
