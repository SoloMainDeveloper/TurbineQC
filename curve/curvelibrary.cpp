#include "curve/pch.h"
#include "curvelibrary.h"

Function1Result CurveLibrary::function1(QVector<Point> points, Function1Params params) { 
    makeCurveCalculations(QList<QVector<Point>> { points }, &params);
    auto splittedPoints = FileSystem::readOutput().split('\n');
    auto parsedPoints = FileSystem::parsePointsFromElement(splittedPoints, ",", 1, 1);
    return Function1Result(CurveFigure(QString(), parsedPoints));
}

Function2Result CurveLibrary::function2(QVector<Point> points, Function2Params params) { 
    return Function2Result();
}

Function3Result CurveLibrary::function3(QVector<Point> points, Function3Params params) { 
    makeCurveCalculations(QList<QVector<Point>> { points }, &params);
    auto splittedPoints = FileSystem::readOutput().split('\n');
    auto parsedPoints = FileSystem::parsePointsFromElement(splittedPoints, ",", 1, 1);
    return Function3Result(CurveFigure(QString(), parsedPoints));
}

Function4Result CurveLibrary::function4(QVector<Point> curve1, QVector<Point> curve2, Function4Params params) {
    makeCurveCalculations(QList<QVector<Point>> { curve1, curve2 }, &params);
    auto output = FileSystem::readOutput().split('\n');
    auto points = FileSystem::parsePointsFromElement(output, ",", 1, 1);
    return Function4Result(CurveFigure(QString(), points));
}

Function5Result CurveLibrary::function5(QVector<Point> points, Function5Params params) { 
    return Function5Result();
}

Function6Result CurveLibrary::function6(QVector<Point> curve1, QVector<Point> curve2, Function6Params params) { 
    makeCurveCalculations(QList<QVector<Point>> { curve1, curve2 }, &params);
    auto output = FileSystem::readOutput().split("\n\n");
    auto points = FileSystem::parsePointsFromElement(output[0].split('\n'), ",", 1, 1);
    auto bestfitStr = output[1].split('\n')[1].split(',');
    return Function6Result(CurveFigure(QString(), points), bestfitStr[0].toDouble(), bestfitStr[1].toDouble(),
        bestfitStr[2].toDouble());
}

Function7Result CurveLibrary::function7(QVector<Point> points, Function7Params params) { 
    makeCurveCalculations(QList<QVector<Point>> { points }, &params);
    auto output = FileSystem::readOutput().split('\n');
    return Function7Result(output);
}

Function8Result CurveLibrary::function8(QVector<Point> points, Function8Params params) { 
    makeCurveCalculations(QList<QVector<Point>> { points }, &params);
    auto output = FileSystem::readOutput().split('\n');
    return Function8Result(output);
}

Function9Result CurveLibrary::function9(QVector<Point> points, Function9Params params) { 
    return Function9Result();
}

Function10Result CurveLibrary::function10(QVector<Point> points, Function10Params params) { 
    return Function10Result();
}

Function11Result CurveLibrary::function11(QVector<Point> points, Function11Params params) { 
    makeCurveCalculations(QList<QVector<Point>> { points }, &params);
    auto output = FileSystem::readOutput().split("\n\n");
    auto elements = FileSystem::parseOutputToElements(output);
    return Function11Result(elements);
}

Function12Result CurveLibrary::function12(QVector<Point> points, Function12Params params) { 
    makeCurveCalculations(QList<QVector<Point>> { points }, &params);
    auto output = FileSystem::readOutput().split("\n\n");
    auto elements = FileSystem::parseOutputToElements(output);
    return Function12Result(elements);
}

Function13Result CurveLibrary::function13(QVector<Point> points, Function13Params params) { 
    makeCurveCalculations(QList<QVector<Point>> { points }, &params);
    auto splittedPoints = FileSystem::readOutput().split('\n');
    auto parsedPoints = FileSystem::parsePointsFromElement(splittedPoints, ",", 1, 1);
    return Function13Result(parsedPoints);
}

Function14Result CurveLibrary::function14(QVector<Point> curve1, QVector<Point> curve2, Function14Params params) {
    makeCurveCalculations(QList<QVector<Point>> { curve1, curve2 }, &params);
    auto output = FileSystem::readOutput().split('\n');
    auto points = FileSystem::parsePointsFromElement(output, ",", 1, 1);
    return Function14Result(CurveFigure(QString(), points));
}

Function15Result CurveLibrary::function15(QVector<Point> points, Function15Params params) { 
    makeCurveCalculations(QList<QVector<Point>> { points }, &params);
    auto output = FileSystem::readOutput().split("\n\n");
    auto elements = FileSystem::parseOutputToElements(output);
    return Function15Result(elements);
}

Function16Result CurveLibrary::function16(QVector<Point> points, Function16Params params) { 
    return Function16Result();
}

Function17Result CurveLibrary::function17(QVector<Point> points, Function17Params params) { 
    return Function17Result();
}

Function18Result CurveLibrary::function18(QVector<Point> points, Function18Params params) { 
    makeCurveCalculations(QList<QVector<Point>> { points }, &params);
    auto output = FileSystem::readOutput().split("\n\n");
    auto splittedElements = FileSystem::parseOutputToElements(output);
    return Function18Result(splittedElements);
}

void CurveLibrary::runCurve() {
    QProcess* process = new QProcess();
    QStringList list;
    list.append("-dn");
    list.append("ca/project");
    process->start(FileSystem::curveDirectory, list);
    process->waitForFinished(-1);
}

void CurveLibrary::makeCurveCalculations(QList<QVector<Point>> elements, FunctionParams *params) {
    FileSystem::createWorkEnvironment();
    FileSystem::fillInputWithMultipleElements(elements);
    FileSystem::fillParams(params);
    runCurve();
}
