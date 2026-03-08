#pragma once

#include "figure.h"
#include "ui_curvewindow.h"
#include "filesystem.h"
#include "project.h"
#include "tree.h"
#include "figurecontrols.h"
#include "dialogservice.h"

namespace Ui {
	class CurveWindow;
};

class LoadingCloudDialog;

class PrintWindow;

class CurveWindow : public QMainWindow {
	Q_OBJECT

public:
	explicit CurveWindow(QWidget* parent = nullptr);
	void dimensionMenuInit();
	void disableDimensionMenu();
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
	void changeWindowTitle(const QString& projectPath);
	void setDefualtWindowTitle();
	void keyPressEvent(QKeyEvent* event);
};
