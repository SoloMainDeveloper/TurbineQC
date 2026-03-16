#pragma once

#include "functionparams.h"
#include "figure.h"
#include "project.h"
#include "curvemachine.h"
#include "resultcompareflr.h"

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
    static void writeFile(QString fullFileName, QString text);
    static QVector<CurvePoint> parsePointsFromElement(QStringList element, QString separator = ",", int startLineToSkip = 0, int finishLineToSkip = 0,
        Order order = Order::CurveLibrary, QString columnNames = nullptr, QString numberOfColumns = nullptr, QString decimal = ".");
    static QMap<QString, QStringList> parseOutputToElements(QStringList elements);
    static void loadCloud(QString filePath, QString name, QString separator, int startLineToSkip,
        int finishLineToSkip, QString columnNames, QString numberOfColumns, QString decimal, Order order = Order::Default);
    static void saveProject(QString fileNameWithoutExtension);
    static void loadProject(const QString &filePath);
    static Figure* parseFigureFromCRV(Project *project, QString figureText);
    static void exportToFLR(QString filepath, QStringList *curvesToTake);
    static void writeCompareFLR(QString fullFileName, QList<ResultCompare2Params*> result, double precision);

private:
    static void inputFigure(QTextStream &stream, FigureSettings *set);
    static void fillFileWithPoints(QTextStream &stream, const QVector<CurvePoint> points, int precision = 4,
        Order order = Order::CurveLibrary);

    static Point _offsetPoint;
    static double _scaleFactor;
    static bool isColumnsCorrect(const QString &columnNames, const QString &numberOfColumns);
    static bool hasDecimalSeparator(const QStringList &pointStr, const QString &decimal);
    static CurvePoint getPoint(QStringList strPoint, Order order, const QString &decimal, int numberOfColumns = 9);
};
