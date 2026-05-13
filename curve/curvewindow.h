#pragma once

#include "dialogservice.h"
#include "figure.h"
#include "figurecontrols.h"
#include "filesystem.h"
#include "project.h"
#include "tree.h"
#include "ui_curvewindow.h"

namespace Ui {
    class CurveWindow;
}; // namespace Ui

class LoadingCloudDialog;

class PrintWindow;

class CurveWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit CurveWindow(QWidget* parent = nullptr);
    virtual ~CurveWindow();

private:
    DialogService* _dialogService;

    Ui::CurveWindow* _ui;
    Project* _project;
    FigureControls* _figureControls;
    Tree* _tree;
    Plot* _plot;

    QStatusBar* _statusBar;
    QLabel* _formLabel; // max dev - min dev
    QLabel* _coordsLabel;
    QLabel* _workPlaneLabel;
    QLabel* _currentFigureLabel;
    QLabel* _magnificationLabel;

    QString _windowTitle;

    void connectMenuItems();
    void createShowHideActions();
    void disableDimensionMenu();
    void dimensionMenuInit();
    void changeWindowTitle(const QString& projectPath);
    void setDefaultWindowTitle();
    void keyPressEvent(QKeyEvent* event);
    void onSaveProject();
};
