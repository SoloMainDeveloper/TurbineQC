#pragma once

#include "ui_sashasecretwindow.h"
#include "functionparams.h"
#include "figure.h"
#include "filesystem.h"
#include "curvemachine.h"
#include "curvelibrary.h"

class SashaSecretWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit SashaSecretWindow(QWidget *parent = nullptr);
    virtual ~SashaSecretWindow();

private:
    Ui::SashaSecretWindowClass *_ui;

    QString _pathToInputFile;
    QString _pathToTestDataFile;

    static QVector<Point> getPointsOfFigure(const QString &fullPathToFile);
    static void makeCurveCalculations();
    static QString getResultOfComparingTwoFiles(const QString &pathToTestDataFile, const QString &pathToRealFile);

    void printText(const QString &header, const QString &pathToTestDataFile, const QString &pathToRealFile);
};
