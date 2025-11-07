#pragma once

#include "figure.h"
#include "functionparams.h"
#include "ui_curvewindow.h"
#include "filesystem.h"
#include "project.h"
#include "algorithms.h"
#include "tree.h"

namespace Ui {
    class CurveWindow;
};

class CurveWindow : public QMainWindow {
    Q_OBJECT

public:
    void enlargeCurveWithIntermediatePoints();
    void getMiddleCurve();
    void getChordLength();
    void getMaxWidthCircle();
    void getWidthOfEdges();

    void renameFigure(const QString &name, const QString &newName);
    void removeFigure(const QString &name);
    void toggleVisibility(const QString &name);

    explicit CurveWindow(QWidget *parent = nullptr);
    virtual ~CurveWindow();

    Project project;

private:
    Ui::CurveWindow *_ui;

    Tree *tree;

private slots:
    void openFile();
};
