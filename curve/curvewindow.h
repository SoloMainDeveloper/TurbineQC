#pragma once

#include "figure.h"
#include "functionparams.h"
#include "ui_curvewindow.h"
#include "filesystem.h"
#include "project.h"
#include "algorithms.h"
#include "tree.h"
#include "figurecontrols.h"
#include "airfoiltolerancesdialog.h"
#include "loadingcloudwindow.h"

namespace Ui {
    class CurveWindow;
};

class LoadingCloudWindow;

class CurveWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit CurveWindow(QWidget *parent = nullptr);
    virtual ~CurveWindow();

    void enlargeCurveWithIntermediatePoints();
    void getMiddleCurve();
    void getChordLength();
    void getMaxWidthCircle();
    void getWidthOfEdges();

private slots:
    void loadCloud();

private:
    Ui::CurveWindow *_ui;

    Project _project;

    FigureControls* _figureControls;
    Tree *_tree;
    Plot *_plot;

    AirfoilTolerancesDialog* _airfoilTolerancesDialog;
    LoadingCloudWindow *_loadingCloudWindow;
};
