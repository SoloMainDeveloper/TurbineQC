#include "curve/pch.h"
#include "curvelibrary.h"

Function1Result CurveLibrary::function1(QVector<CurvePoint> points, Function1Params params) {
    makeCurveCalculations(QList<QVector<CurvePoint>> { points }, &params);
    auto splittedPoints = FileSystem::readOutput().split('\n');
    auto parsedPoints = FileSystem::parsePointsFromElement(splittedPoints, ",", 1, 1);
    return Function1Result(CurveFigure(QString(), parsedPoints));
}

Function2Result CurveLibrary::function2(QVector<CurvePoint> points, Function2Params params) {
    return Function2Result();
}

Function3Result CurveLibrary::function3(QVector<CurvePoint> points, Function3Params params) {
    makeCurveCalculations(QList<QVector<CurvePoint>> { points }, &params);
    auto splittedPoints = FileSystem::readOutput().split('\n');
    auto parsedPoints = FileSystem::parsePointsFromElement(splittedPoints, ",", 1, 1);
    return Function3Result(CurveFigure(QString(), parsedPoints));
}

Function4Result CurveLibrary::function4(QVector<CurvePoint> curve1, QVector<CurvePoint> curve2, Function4Params params) {
    makeCurveCalculations(QList<QVector<CurvePoint>> { curve1, curve2 }, &params);
    auto output = FileSystem::readOutput().split('\n');
    auto points = FileSystem::parsePointsFromElement(output, ",", 1, 1);
    return Function4Result(CurveFigure(QString(), points));
}

Function5Result CurveLibrary::function5(QVector<CurvePoint> points, Function5Params params) {
    return Function5Result();
}

Function6Result CurveLibrary::function6(QVector<CurvePoint> curve1, QVector<CurvePoint> curve2, Function6Params params) {
    makeCurveCalculations(QList<QVector<CurvePoint>> { curve1, curve2 }, &params);
    auto output = FileSystem::readOutput().split("\n\n");
    auto points = FileSystem::parsePointsFromElement(output[0].split('\n'), ",", 1, 1);
    auto bestfitStr = output[1].split('\n')[1].split(',');
    return Function6Result(CurveFigure(QString(), points), bestfitStr[0].toDouble(), bestfitStr[1].toDouble(),
        bestfitStr[2].toDouble());
}

Function7Result CurveLibrary::function7(QVector<CurvePoint> points, Function7Params params) {
    makeCurveCalculations(QList<QVector<CurvePoint>> { points }, &params);
    auto output = FileSystem::readOutput().split('\n');
    return Function7Result(output);
}

Function8Result CurveLibrary::function8(QVector<CurvePoint> points, Function8Params params) {
    makeCurveCalculations(QList<QVector<CurvePoint>> { points }, &params);
    auto output = FileSystem::readOutput().split('\n');
    return Function8Result(output);
}

Function9Result CurveLibrary::function9(QVector<CurvePoint> points, Function9Params params) {
    return Function9Result();
}

Function10Result CurveLibrary::function10(QVector<CurvePoint> points, Function10Params params) {
    return Function10Result();
}

Function11Result CurveLibrary::function11(QVector<CurvePoint> points, Function11Params params) {
    makeCurveCalculations(QList<QVector<CurvePoint>> { points }, &params);
    auto output = FileSystem::readOutput().split("\n\n");
    auto elements = FileSystem::parseOutputToElements(output);
    return Function11Result(elements);
}

Function12Result CurveLibrary::function12(QVector<CurvePoint> points, Function12Params params) {
    makeCurveCalculations(QList<QVector<CurvePoint>> { points }, &params);
    auto output = FileSystem::readOutput().split("\n\n");
    auto elements = FileSystem::parseOutputToElements(output);
    return Function12Result(elements);
}

Function13Result CurveLibrary::function13(QVector<CurvePoint> points, Function13Params params) {
    makeCurveCalculations(QList<QVector<CurvePoint>> { points }, &params);
    auto splittedPoints = FileSystem::readOutput().split('\n');
    auto parsedPoints = FileSystem::parsePointsFromElement(splittedPoints, ",", 1, 1);
    return Function13Result(parsedPoints);
}

Function14Result CurveLibrary::function14(QVector<CurvePoint> curve1, QVector<CurvePoint> curve2, Function14Params params) {
    makeCurveCalculations(QList<QVector<CurvePoint>> { curve1, curve2 }, &params);
    auto output = FileSystem::readOutput().split('\n');
    auto points = FileSystem::parsePointsFromElement(output, ",", 1, 1);
    return Function14Result(CurveFigure(QString(), points));
}

Function15Result CurveLibrary::function15(QVector<CurvePoint> points, Function15Params params) {
    makeCurveCalculations(QList<QVector<CurvePoint>> { points }, &params);
    auto output = FileSystem::readOutput().split("\n\n");
    auto elements = FileSystem::parseOutputToElements(output);
    return Function15Result(elements);
}

Function16Result CurveLibrary::function16(QVector<CurvePoint> points, Function16Params params) {
    return Function16Result();
}

Function17Result CurveLibrary::function17(QVector<CurvePoint> points, Function17Params params) {
    return Function17Result();
}

Function18Result CurveLibrary::function18(QVector<CurvePoint> points, Function18Params params) {
    makeCurveCalculations(QList<QVector<CurvePoint>> { points }, &params);
    auto output = FileSystem::readOutput().split("\n\n");
    auto splittedElements = FileSystem::parseOutputToElements(output);
    return Function18Result(splittedElements);
}

Function19Result CurveLibrary::function19(QVector<CurvePoint> points, Function19Params params) {
    makeCurveCalculations(QList<QVector<CurvePoint>> { points }, &params);
    auto splittedPoints = FileSystem::readOutput().split('\n');
    auto parsedPoints = FileSystem::parsePointsFromElement(splittedPoints, ",", 1, 1);
    return Function19Result(CurveFigure(QString(), parsedPoints));
}

Function21Result CurveLibrary::function21(QVector<CurvePoint> curve1, QVector<CurvePoint> curve2, QVector<CurvePoint> curve3, Function21Params params) {
    makeCurveCalculations(QList<QVector<CurvePoint>> { curve1, curve2, curve3 }, &params);
    auto output = FileSystem::readOutput().split("\n\n");
    auto points = FileSystem::parsePointsFromElement(output[0].split('\n'), ",", 1, 1);
    auto bestfitStr = output[1].split('\n')[1].split(',');
    return Function21Result(CurveFigure(QString(), points), bestfitStr[0].toDouble(), bestfitStr[1].toDouble(), bestfitStr[2].toDouble());
}

Function31Result CurveLibrary::function31(QVector<CurvePoint> curve1, QVector<CurvePoint> curve2, Function31Params params) {
    makeCurveCalculations(QList<QVector<CurvePoint>> { curve1, curve2 }, &params);
    auto output = FileSystem::readOutput().split("\n\n");
    auto points = FileSystem::parsePointsFromElement(output[0].split('\n'), ",", 1, 1);
    auto coeffLEStr = output[1].split('\n')[1].split(',')[7];
    auto coeffTEStr = output[2].split('\n')[1].split(',')[7];
    return Function31Result(CurveFigure(QString(), points), coeffLEStr.toDouble(), coeffTEStr.toDouble());
}

void CurveLibrary::runCurve() {
    QProcess* process = new QProcess();
    QStringList list;
    list.append("-dn");
    list.append("ca/project");
    process->start(FileSystem::curveDirectory, list);
    process->waitForFinished(-1);
}

void CurveLibrary::makeCurveCalculations(QList<QVector<CurvePoint>> elements, FunctionParams *params) {
    FileSystem::createWorkEnvironment();
    FileSystem::fillInputWithMultipleElements(elements);
    FileSystem::fillParams(params);
    runCurve();
}
