#include "curve/pch.h"
#include "algorithms.h"


QVector<Point> Algorithms::enlargeCurveWithIntermediatePoints(QString figureName, const Function1Params *params, Project *project) { //todo
    return QVector<Point>();
}

QVector<Point> Algorithms::getMiddleCurve(QString figureName, const Function18Params *params, Project *project) { //todo
    return QVector<Point>();
}

void Algorithms::getChordLength(QString figureName, const Function18Params *params, Project *project) { //todo
    auto figure = project->findFigure(figureName);
    auto chordLength = CurveMachine::getChordLength(figure->points(), *params);
    //auto fullAnswerStr = FileSystem::readOutput().split("\n\n");
    //auto functionAnswer = FileSystem::parseOutputToElements(fullAnswerStr);
    //AnswersHistory[name].insert(18, FunctionAnswer(functionAnswer));
    //build figures from data
    //update drawer drawer->sync();
}

CircleFigure Algorithms::getMaxWidthCircle(QString figureName, const Function18Params *params, Project *project) { //todo
    return CircleFigure("circle");
}

std::array<double, 2> Algorithms::getWidthOfEdges(QString figureName, const Function18Params *params, Project *project) {
    return { 0, 0 }; //return CurveMachine::getWidthOfEdges();
}

