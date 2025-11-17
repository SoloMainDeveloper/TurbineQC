#pragma once

#include "figure.h"
#include "functionparams.h"
#include "ui_curvewindow.h"
#include "filesystem.h"
#include "project.h"
#include "algorithms.h"
#include "tree.h"
#include "figurecontrols.h"

namespace Ui {
    class CurveWindow;
};

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

    void renameFigure(const QString &name, const QString &newName);
    void removeFigure(const QString &name);
    void toggleVisibility(const QString &name);

private slots:
    void openFile();

private:
    Ui::CurveWindow *_ui;

    Project _project;

    FigureControls* _figureControls;

    Tree *_tree;
    Plot *_plot;
};
