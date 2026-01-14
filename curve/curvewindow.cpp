#include "curve/pch.h"
#include "curvewindow.h"
#include "reportgenerator.h"

CurveWindow::CurveWindow(QWidget *parent) : QMainWindow(parent), _ui(new Ui::CurveWindow) {
    _ui->setupUi(this);

    _windowTitle = "Curve 1.0";
    this->setWindowIcon(QIcon("icons/curveWindow.png"));
    setDefualtWindowTitle();

    _loadingCloudWindow = new LoadingCloudWindow(&_project);
    connect(_ui->actionLoadFile, &QAction::triggered, _loadingCloudWindow, &LoadingCloudWindow::exec);

    _airfoilTolerancesDialog = new AirfoilTolerancesDialog(&_project);
    connect(_ui->actionAssignTolerancesToNominal, &QAction::triggered, _airfoilTolerancesDialog, &AirfoilTolerancesDialog::dialogInitialization);

    _turbineWindow = new TurbineWindow(&_project, _ui->plot);
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
        auto projectPath = QFileDialog::getOpenFileName(nullptr, "Open file", "", "(*.txt *.crv)");
        changeWindowTitle(projectPath);
        FileSystem::loadProject(&_project, projectPath);
    });

    connect(_ui->actionClearProject, &QAction::triggered, &_project, [&]() {
        _project.clear();
        setDefualtWindowTitle();
    });

    _bestFitDialog = new BestFitDialog(&_project);
    connect(_ui->actionBestFit, &QAction::triggered, _bestFitDialog, &BestFitDialog::initialization);

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
    
    auto dimensionRender = new DimensionRender(_plot);
    connect(_ui->menuDimensions, &QMenu::aboutToShow, this, &CurveWindow::dimensionMenuInit);
    connect(_ui->actionRadius, &QAction::triggered, dimensionRender, &DimensionRender::addRadius);
    connect(_ui->actionDiameter, &QAction::triggered, dimensionRender, &DimensionRender::addDiameter);
    connect(_ui->actionPerimeter, &QAction::triggered, dimensionRender, &DimensionRender::addPerimeter);
    
    connect(_ui->actionZoomExtents, &QAction::triggered, _plot, &Plot::zoomExtents);
    connect(_ui->actionZoomPlus, &QAction::triggered, _plot, &Plot::zoomPlus);
    connect(_ui->actionZoomMinus, &QAction::triggered, _plot, &Plot::zoomMinus);

    _macrosWindow = new MacrosWindow(this, &_project);
    connect(_ui->actionShowMacrosWindow, &QAction::triggered, _macrosWindow, &MacrosWindow::initialization);
    connect(_macrosWindow, &MacrosWindow::needShow, _ui->actionShowMacrosWindow, &QAction::setEnabled);    
}

void CurveWindow::dimensionMenuInit() {
    disableDimensionMenu();
    auto currentFigure = _project.findFigure(_project.currentFigureName());
    if(dynamic_cast<const CircleFigure*>(currentFigure)) {
        _ui->actionRadius->setEnabled(true);
        _ui->actionDiameter->setEnabled(true);
    } else if(dynamic_cast<const CurveFigure*>(currentFigure)) {
        _ui->actionPerimeter->setEnabled(true);
    }
}

void CurveWindow::disableDimensionMenu() {
    _ui->actionPosition->setEnabled(false);
    _ui->actionTruePosition->setEnabled(false);
    _ui->actionDistance->setEnabled(false);
    _ui->actionRadius->setEnabled(false);
    _ui->actionDiameter->setEnabled(false);
    _ui->menuAngle->setEnabled(false);
    _ui->menuForm->setEnabled(false);
    _ui->actionPerimeter->setEnabled(false);
    _ui->actionBestFitData->setEnabled(false);
    _ui->actionMeasureOnScreen->setEnabled(false);
}

void CurveWindow::changeWindowTitle(const QString &projectPath) {
    if(!projectPath.isEmpty()) {
        this->setWindowTitle(_windowTitle + " - " + projectPath);
    }
}

void CurveWindow::setDefualtWindowTitle() {
    this->setWindowTitle(_windowTitle);
}

CurveWindow::~CurveWindow() {
    delete _ui;
    delete _loadingCloudWindow;
    delete _mergeCloudsWindow;
}
