#include "curve/pch.h"
#include "algorithms.h"


void Algorithms::enlargeCurveWithIntermediatePoints(QString figureName, const Function1Params *params, Project *project) {
    auto figure = project->findFigure(figureName);
    auto result = CurveMachine::enlargeCurveWithIntermediatePoints(figure->points(), *params);
    auto newName = figureName + "_enlarged";
    auto newCurve = new CurveFigure(newName, result.points());
    if(project->containsFigure(newName)) {
        project->removeFigure(newName);
    }
    project->insertFigure(newCurve);
}

void Algorithms::getMiddleCurve(QString figureName, const Function18Params *params, Project *project) {
    auto figure = project->findFigure(figureName);
    auto result = CurveMachine::getMiddleCurve(figure->points(), *params);
    auto newName = figureName + "_MCL";
    auto middleCurve = new CurveFigure(newName, result.points());
    if(project->containsFigure(newName)) {
        project->removeFigure(newName);
    }
    project->insertFigure(middleCurve);
}

void Algorithms::getChordLength(QString figureName, const Function18Params *params, Project *project) {
    auto figure = project->findFigure(figureName);
    auto chordLength = CurveMachine::getChordLength(figure->points(), *params);
    //need special interface to show the result
}

void Algorithms::getMaxWidthCircle(QString figureName, const Function18Params *params, Project *project) {
    auto figure = project->findFigure(figureName);
    auto result = CurveMachine::getMaxWidthCircle(figure->points(), *params);
    auto newName = figureName + "_MaxDia";
    auto circle = new CircleFigure(newName, result.centre(), result.radius());
    if(project->containsFigure(newName)) {
        project->removeFigure(newName);
    }
    project->insertFigure(circle);
}

void Algorithms::getWidthOfEdges(QString figureName, double distanceFromLeadingEdge, double distanceFromTrailingEgde, Project *project) {
    auto figure = project->findFigure(figureName);
    auto result = CurveMachine::getWidthOfEdges(figure->points(), distanceFromLeadingEdge, distanceFromTrailingEgde);
    //need special interface to show the result
}

