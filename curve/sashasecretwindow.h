#pragma once

#include "ui_sashasecretwindow.h"
#include "functionparams.h"
#include "figure.h"
#include "filesystem.h"
#include "curvemachine.h"

class SashaSecretWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit SashaSecretWindow(QWidget *parent = nullptr);
    virtual ~SashaSecretWindow();

private:
    Ui::SashaSecretWindowClass *_ui;

    static QVector<Point> getPointsOfFigure(const QString &fullPathToFile);
    static void makeCurveCalculations();
};
