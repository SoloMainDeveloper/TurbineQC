#pragma once
#include "figure.h"
#include "filesystem.h"

struct Function1Result {
public:
    Function1Result(CurveFigure curve);
    ~Function1Result() = default;

    CurveFigure curve;
};

struct Function2Result {
public:
    Function2Result();
    ~Function2Result() = default;
};

struct Function3Result {
public:
    Function3Result(CurveFigure curve);
    ~Function3Result() = default;

    CurveFigure curve;
};

struct Function4Result {
public:
    Function4Result(CurveFigure curve);
    ~Function4Result() = default;

    CurveFigure curve;
};

struct Function5Result {
public:
    Function5Result();
    ~Function5Result() = default;
};

struct Function6Result {
public:
    Function6Result(CurveFigure curve, double offsetX, double offsetY, double rotation);
    ~Function6Result() = default;

    CurveFigure curve;
    double offsetX;
    double offsetY;
    double rotation;
};

struct Function7Result {
public:
    Function7Result(QStringList points);
    ~Function7Result() = default;

    QVector<Point> contactPoints;
};

struct Function8Result {
public:
    Function8Result(QStringList points);
    ~Function8Result() = default;

    Point point;
};

struct Function9Result {
public:
    Function9Result();
    ~Function9Result() = default;
};

struct Function10Result {
public:
    Function10Result();
    ~Function10Result() = default;
};

struct Function11Result {
public:
    Function11Result(QMap<QString, QStringList> elements);
    ~Function11Result() = default;

    LineFigure line;
    QVector<Point> points;
};

struct Function12Result {
public:
    Function12Result(QMap<QString, QStringList> elements);
    ~Function12Result() = default;

    CircleFigure circle;
    QVector<Point> points;
};

struct Function13Result {
public:
    Function13Result(QVector<Point> points);
    ~Function13Result() = default;

    QVector<Point> points;
};

struct Function14Result {
public:
    Function14Result(CurveFigure curve);
    ~Function14Result() = default;

    CurveFigure curve;
};

struct Function15Result {
public:
    Function15Result(QMap<QString, QStringList> elements);
    ~Function15Result() = default;

    double perimeter;
    double square;
    Point massCentre;
};

struct Function16Result {
public:
    Function16Result();
    ~Function16Result() = default;
};

struct Function17Result {
public:
    Function17Result();
    ~Function17Result() = default;
};

struct Function18Result {
public:
    Function18Result(QMap<QString, QStringList> elements);
    ~Function18Result() = default;

    CurveFigure middleCurve; //MCL
    PointFigure leadingEdgePoint; //LE
    PointFigure trailingEdgePoint; //TE
    CircleFigure maxCircle; //CIRCLE$MAX
    LineFigure chordLine; //LINE$FIT
    PointFigure contactLEPoint; //CONTACT$LE
    PointFigure contactTEPoint; //CONTACT$TE
    CurveFigure curveTE; //PART$CURVE_TE
    CurveFigure curveLE; //PART$CURVE_LE
    CurveFigure curveLow; //PART$CURVE_LO
    CurveFigure curveHigh; //PART$CURVE_HI
    QVector<Point> contactEndPoints; //CONTACT$ENDPOINTS
    double chordLength = 0;
};
