#include "curve/pch.h"
#include "curvemachine.h"

QVector<Point> CurveMachine::enlargeCurveWithIntermediatePoints(QVector<Point> points, Function1Params params) {
    FileSystem::createWorkEnvironment();
    FileSystem::fillInputWithSingleSegment(points);
    FileSystem::fillParams(&params);
    makeCurveCalculations();

    auto splittedPoints = FileSystem::readOutput().split('\n');
    return FileSystem::parsePointsFromElement(splittedPoints, ",", 1, 1);
}

void CurveMachine::makeCurveCalculations() {
    QProcess* process = new QProcess();
    QStringList list;
    list.append("-dn");
    list.append("ca/project");
    process->start(FileSystem::curveDirectory, list);
    process->waitForFinished(-1);
}

QVector<Point> CurveMachine::getMiddleCurve(QVector<Point> points, Function18Params params) {
    FileSystem::createWorkEnvironment();
    FileSystem::fillInputWithSingleSegment(points);
    FileSystem::fillParams(&params);
    makeCurveCalculations();

    auto splittedElements = FileSystem::readOutput().split("\n\n");
    QVector<Point> result;
    for(auto i = 0; i < splittedElements.length(); i++) {
        auto strList = splittedElements[i].split('\n');
        if(strList[0].contains("NAM=MCL")) {
            result = FileSystem::parsePointsFromElement(strList, ",", 1, 1);
            break;
        }
    }
    return result;
}

double CurveMachine::getChordLength(QVector<Point> points, Function18Params params) {
    FileSystem::createWorkEnvironment();
    FileSystem::fillInputWithSingleSegment(points);
    FileSystem::fillParams(&params);
    makeCurveCalculations();

    auto splittedElements = FileSystem::readOutput().split("\n\n");
    for(auto i = 0; i < splittedElements.length(); i++) {
        auto strList = splittedElements[i].split('\n');
        if(strList[0].contains("NAM=CONTACT$ENDPOINTS")) {
            return strList.at(strList.size() - 2).split(',')[7].toDouble();
        }
    }
    return -1;
}

CircleFigure CurveMachine::getMaxWidthCircle(QVector<Point> points, Function18Params params) {
    FileSystem::createWorkEnvironment();
    FileSystem::fillInputWithSingleSegment(points);
    FileSystem::fillParams(&params);
    makeCurveCalculations();

    auto splittedElements = FileSystem::readOutput().split("\n\n");
    for(auto i = 0; i < splittedElements.length(); i++) {
        auto strList = splittedElements[i].split('\n');
        if(strList[0].contains("NAM=CIRCLE$MAX")) {
            auto args = strList.at(1).split(',');
            return CircleFigure("maxWidthCircle", Point(args[0].toDouble(), args[1].toDouble()), args[7].toDouble() * 2);
        }
    }
    return CircleFigure("empty");
}