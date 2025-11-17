#include "curve/pch.h"
#include "functionresults.h"

Function1Result::Function1Result(CurveFigure curve) {
    this->curve = curve;
}

Function2Result::Function2Result() {
}

Function3Result::Function3Result(CurveFigure curve) {
    this->curve = curve;
}

Function4Result::Function4Result(CurveFigure curve) {
    this->curve = curve;
}

Function5Result::Function5Result() {
}

Function6Result::Function6Result(CurveFigure curve, double offsetX, double offsetY, double rotation) {
    this->curve = curve;
    this->offsetX = offsetX;
    this->offsetY = offsetY;
    this->rotation = rotation;
}

Function7Result::Function7Result(QStringList points) {
    contactPoints = FileSystem::parsePointsFromElement(points, ",", 1, 1);
}

Function8Result::Function8Result(QStringList points) {
    point = FileSystem::parsePointsFromElement(points, ",", 1, 1)[0];
}

Function9Result::Function9Result() {
}

Function10Result::Function10Result() {
}

Function11Result::Function11Result(QMap<QString, QStringList> elements) {
    points = FileSystem::parsePointsFromElement(elements["INPUTNAME"], ",", 1, 1);
    auto lineData = FileSystem::parsePointsFromElement(elements["INPUTNAME"], ",", 1, 1);
    //line = LineFigure(lineData) waiting for new line constuctor
}

Function12Result::Function12Result(QMap<QString, QStringList> elements) {
    points = FileSystem::parsePointsFromElement(elements["INPUTNAME"], ",", 1, 1);
    auto circleData = FileSystem::parsePointsFromElement(elements["CIRCLE$FIT"], ",", 1, 1)[0];
    auto centre = Point(circleData.x, circleData.y);
    circle = CircleFigure(QString(), centre, circleData.lt);
}

Function13Result::Function13Result(QVector<Point> points) {
    this->points = points;
}

Function14Result::Function14Result(CurveFigure curve) {
    this->curve = curve;
}

Function15Result::Function15Result(QMap<QString, QStringList> elements) {
    auto perimAreaParams = elements["PERIMETER_AREA"][1].split(',');
    perimeter = perimAreaParams[0].toDouble();
    square = perimAreaParams[1].toDouble();
    massCentre = FileSystem::parsePointsFromElement(elements["CENTER_OF_MASS"], ",", 1, 1)[0];
}

Function16Result::Function16Result() {
}

Function17Result::Function17Result() {
}

Function18Result::Function18Result(QMap<QString, QStringList> elements) {
    for(auto header : elements.keys()) {
        auto points = FileSystem::parsePointsFromElement(elements[header], ",", 1, 1);
        if(header == "MCL") {
            this->middleCurve = CurveFigure(QString(), points);
        } else if(header == "TE_LE") {
            this->trailingEdgePoint = PointFigure(QString(), points[0]);
            this->leadingEdgePoint = PointFigure(QString(), points[1]);
        } else if(header == "CIRCLE$MAX") {
            auto centre = Point(points[0].x, points[0].y);
            auto radius = points[0].lt; //idk what to do with that
            this->maxCircle = CircleFigure(QString(), centre, radius);
        } else if(header == "LINE$FIT") {
            //result.chordLine = LineFigure(QString(), points[0].x, points[0].y, points[0].z,
            //  points[0].u, points[0].v, points[0].w) //waiting for new LineFigure struct
        } else if(header == "CONTACT$TE_LE") {
            this->contactTEPoint = PointFigure(QString(), points[0]);
            this->contactLEPoint = PointFigure(QString(), points[1]);
        } else if(header == "PART$CURVE_TE") {
            this->curveTE = CurveFigure(QString(), points);
        } else if(header == "PART$CURVE_LE") {
            this->curveLE = CurveFigure(QString(), points);
        } else if(header == "PART$CURVE_LO") {
            this->curveLow = CurveFigure(QString(), points);
        } else if(header == "PART$CURVE_HI") {
            this->curveHigh = CurveFigure(QString(), points);
        } else if(header == "CONTACT$ENDPOINTS") {
            this->contactEndPoints = points;
            this->chordLength = points.last().lt;
        }
    }
}
