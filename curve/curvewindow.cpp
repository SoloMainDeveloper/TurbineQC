#include "curve/pch.h"
#include "curvewindow.h"

CurveWindow::CurveWindow(QWidget *parent) : QMainWindow(parent), _ui(new Ui::CurveWindow) {
    _ui->setupUi(this);
    
    _loadingCloudWindow = new LoadingCloudWindow(&_project);
    connect(_ui->actionLoadFile, &QAction::triggered, _loadingCloudWindow, &LoadingCloudWindow::exec);

    _airfoilTolerancesDialog = new AirfoilTolerancesDialog(&_project);
    connect(_ui->actionAssignTolerancesToNominal, &QAction::triggered, _airfoilTolerancesDialog, &AirfoilTolerancesDialog::dialogInitialization);

    _turbineWindow = new TurbineWindow(&_project);
    connect(_ui->actionCalculateTurbineParameters, &QAction::triggered, _turbineWindow, &TurbineWindow::initialization);
    
    _widthEdgeWindow = new WidthEdgeWindow(&_project);
    connect(_ui->actionCalculateEdgeWidth, &QAction::triggered, _widthEdgeWindow, &WidthEdgeWindow::initialization);

    _mergeCloudsWindow = new MergePointCloudsWindow(&_project);
    connect(_ui->actionMergeClouds, &QAction::triggered, _mergeCloudsWindow, &MergePointCloudsWindow::windowInitialization);

    _radiusCorrectionDialog = new RadiusCorrectionDialog(&_project);
    connect(_ui->actionRadiusCorrection, &QAction::triggered, _radiusCorrectionDialog, &RadiusCorrectionDialog::initialization);

    _saveProjectWindow = new SaveProjectWindow(&_project);
    connect(_ui->actionSaveProject, &QAction::triggered, _saveProjectWindow, &SaveProjectWindow::initialization);

    connect(_ui->actionLoadProject, &QAction::triggered, this, [&]() {
        FileSystem::loadProject(&_project);
    });

    connect(_ui->actionClearProject, &QAction::triggered, &_project, &Project::clear);

    _figureEditDialog = new FigureEditDialog(&_project);

    _tree = _ui->tree;
    _tree->setProject(&_project);
    _plot = _ui->plot;
    _plot->setProject(&_project);

    _figureControls = new FigureControls(&_project);
    _figureControls->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    _ui->leftBar->addWidget(_figureControls);

    _calculateDeviationsDialog = new CalculateDeviationsDialog(&_project);
    connect(_ui->actionCalculateDeviations, &QAction::triggered, _calculateDeviationsDialog, &CalculateDeviationsDialog::initialization);
}

CurveWindow::~CurveWindow() {
    delete _ui;
    delete _loadingCloudWindow;
    delete _mergeCloudsWindow;
}
