#pragma once

#include "functionparams.h"
#include "figure.h"
#include "project.h"
#include "curvemachine.h"

enum class Order {
    CurveLibrary, //xyzutijkdevlt
    Default //xyzijkdevutlt
};

class FileSystem {
public:
    static QString curveDirectory; // to delete; only for local usages

    static bool checkCurve();
    static bool isValidFileName(QString fileName);
    static void createWorkEnvironment();
    static void fillInputWithSingleSegment(QVector<CurvePoint> points);
    static void fillInputWithMultipleElements(QList<QVector<CurvePoint>> points);
    static void fillParams(FunctionParams* params);
    static QString readOutput();
    static QString readFile(QString fullFileName);
    static QVector<CurvePoint> parsePointsFromElement(QStringList element, QString separator = ",", int startLineToSkip = 0,
        int finishLineToSkip = 0, Order order = Order::CurveLibrary);
    static QMap<QString, QStringList> parseOutputToElements(QStringList elements);
    static void saveProject(Project *project, QString dir, QString projectName, bool createCRV);
    static void loadProject(Project *project);

private:
    static void inputFigure(QTextStream &stream, FigureSettings *set);
    static void fillFileWithPoints(QTextStream &stream, const QVector<CurvePoint> points, int precision = 4,
        Order order = Order::CurveLibrary);
};

