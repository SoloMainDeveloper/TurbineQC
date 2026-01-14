#pragma once

#include "ui_sashasecretwindow.h"
#include "functionparams.h"
#include "figure.h"
#include "filesystem.h"
#include "curvemachine.h"
#include "curvelibrary.h"
#include "loadingcloudwindow.h"
#include "project.h"

class SashaSecretWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit SashaSecretWindow(Project *project = nullptr, QWidget *parent = nullptr);
    virtual ~SashaSecretWindow();

private:
    Ui::SashaSecretWindowClass *_ui;

    Project *_project;
    LoadingCloudWindow *_loadingCloudWindow;
    QString _pathToInputFile;
    QString _pathToTestDataFile;

    static QVector<CurvePoint> getPointsOfFigure(const QString &fullPathToFile);
    static void makeCurveCalculations();
    static QString getResultOfComparingTwoFiles(const QString &pathToTestDataFile, const QString &pathToRealFile);

    void printText(const QString &header, const QString &pathToTestDataFile, const QString &pathToRealFile);
};
