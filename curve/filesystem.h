#pragma once

#include "functionparams.h"
#include "figure.h"

class FileSystem {
public:
    static QString curveDirectory; // to delete; only for local usages

    static void createWorkEnvironment();
    static void fillInputWithSingleSegment(QVector<Point> points);
    static void fillInputWithMultipleElements(QList<QVector<Point>> points);
    static void fillParams(FunctionParams* params);
    static QString readOutput();
    static QString readFile(QString fullFileName);
    static QVector<Point> parsePointsFromElement(QStringList element, QString separator = ",", int startLineToSkip = 0, int finishLineToSkip = 0);
    static QMap<QString, QStringList> parseOutputToElements(QStringList elements);
};

