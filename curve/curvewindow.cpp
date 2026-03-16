#include "curve/pch.h"

#include "curvewindow.h"
#include "reportgenerator.h"

CurveWindow::CurveWindow(QWidget* parent) : QMainWindow(parent), _ui(new Ui::CurveWindow) {
    _ui->setupUi(this);

    _windowTitle = "Curve 1.0";
    this->setWindowIcon(QIcon("icons/curveWindow.png"));
    setDefualtWindowTitle();

    _project = &Project::instance();

    _dialogService = &DialogService::instance();

    connect(_ui->actionLoadFile, &QAction::triggered, this, [this]() { _dialogService->show(DialogType::LoadCloud); });
    connect(_ui->actionAssignTolerancesToNominal, &QAction::triggered, [this]() { _dialogService->show(DialogType::AirfoilTolerances); });
    connect(_ui->actionCalculateTurbineParameters, &QAction::triggered, [this]() { _dialogService->show(DialogType::Turbine); });
    connect(_ui->actionCalculateEdgeWidth, &QAction::triggered, [this]() { _dialogService->show(DialogType::WidthEdge); });
    connect(_ui->actionMergeClouds, &QAction::triggered, [this]() { _dialogService->show(DialogType::MergePointCloud); });
    connect(_ui->actionRadiusCorrection, &QAction::triggered, [this]() { _dialogService->show(DialogType::RadiusCorrection); });
    connect(_ui->actionExportToFLR, &QAction::triggered, [this]() { _dialogService->show(DialogType::ExportToFLR); });
    connect(_ui->actionShowPrintViewer, &QAction::triggered, [this]() { _dialogService->show(DialogType::PrintPreview); });
    connect(_ui->actionBestFit, &QAction::triggered, [this]() { _dialogService->show(DialogType::BestFit); });
    connect(_ui->actionCalculateDeviations, &QAction::triggered, [this]() { _dialogService->show(DialogType::CalculateDeviations); });
    connect(_ui->actionShowMacrosWindow, &QAction::triggered, [this]() { _dialogService->show(DialogType::Macros); });
    connect(_ui->actionShift, &QAction::triggered, [this]() { _dialogService->show(DialogType::Shift); });
    connect(_ui->actionRotate, &QAction::triggered, [this]() { _dialogService->show(DialogType::Rotate); });
    connect(_ui->actionAlignment, &QAction::triggered, [this]() { _dialogService->show(DialogType::Alignment); });
    connect(_ui->actionCurve_2D_calculation, &QAction::triggered, [this]() { _dialogService->show(DialogType::CalculateCurve); });
    connect(_ui->actionEdit_Part_Data, &QAction::triggered, [this]() { _dialogService->show(DialogType::PartData); });
    connect(_ui->actionText, &QAction::triggered, [this]() { _dialogService->show(DialogType::InsertText); });
    connect(_ui->actionCompareFLR, &QAction::triggered, [this]() { _dialogService->show(DialogType::CompareFLR); });

    connect(_ui->actionSaveProject, &QAction::triggered, this, &CurveWindow::onSaveProject);

    //connect(&_project, &Project::showPartDataDialogRequested, _dialogService, &PartDataDialog::initializationByMacros);
    connect(_ui->actionLoadProject, &QAction::triggered, this, [&]() {
        auto projectPath = QFileDialog::getOpenFileName(nullptr, "Open file", "", "(*.txt *.crv)");
        try {
            FileSystem::loadProject(projectPath);
        } catch(...) {
        }
    });
    connect(_project, &Project::projectPathChanged, this, &CurveWindow::changeWindowTitle);

    connect(_ui->actionClearProject, &QAction::triggered, [this]() {
        Project::instance().clear();
        Printer::instance().clear();
        setDefualtWindowTitle();
    });

    _tree = _ui->tree;
    _tree->setProject(_project);
    _plot = _ui->plot;
    _plot->setProject(_project);

    //connect(_ui->menuDimensions, &QMenu::aboutToShow, this, &CurveWindow::dimensionMenuInit);
    delete _ui->menuDimensions;
    delete _ui->menuTest;
    delete _ui->menuConstruct;

    connect(_ui->actionRadius, &QAction::triggered, _plot, &Plot::createRadiusDimension);
    connect(_ui->actionDiameter, &QAction::triggered, _plot, &Plot::createDiameterDimension);
    connect(_ui->actionPerimeter, &QAction::triggered, [&] {
        auto currentCurve = dynamic_cast<const CurveFigure*>(_project->findFigure(_project->currentFigureName()));
        double perimeter = CurveLibrary::function15(currentCurve->points(), Function15Params(currentCurve->isClosed())).perimeter;
        _plot->createPerimeterDimension(perimeter);
    });

    connect(_ui->actionZoomExtents, &QAction::triggered, _plot, &Plot::zoomExtents);
    connect(_ui->actionZoomPlus, &QAction::triggered, _plot, &Plot::zoomPlus);
    connect(_ui->actionZoomMinus, &QAction::triggered, _plot, &Plot::zoomMinus);

    _figureControls = new FigureControls(_project);
    _figureControls->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    _ui->leftBar->addWidget(_figureControls);

    _ui->leftBar->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));

    //connect(_plot, &Plot::mousePress, this, [&](QMouseEvent *event) {
    //    const auto &pixelPosition = event->pos();
    //    const auto x = _plot->xAxis->pixelToCoord(pixelPosition.x());
    //    const auto y = _plot->yAxis->pixelToCoord(pixelPosition.y());
    //    _insertTextDialog->onPlotClick(QPointF(x, y));
    //});
    createShowHideActions();

    _ui->searchCB->lineEdit()->setPlaceholderText(tr("Search figure"));
    _ui->searchCB->completer()->setCompletionMode(QCompleter::PopupCompletion);
    _ui->searchCB->completer()->setFilterMode(Qt::MatchContains);
    connect(_ui->searchCB, &QComboBox::currentTextChanged, [&](QString text) {
        if(text.isEmpty()) {
            _ui->searchCB->clear();
            return;
        }
        QStringList list;
        for(auto& figure : _project->figures()) {
            if(figure->name().toLower() == text.toLower()) {
                _project->changeCurrentFigure(figure->name());
            }
            if(figure->name().toLower().contains(text.toLower())) {
                list.append(figure->name());
            }
        }

        _ui->searchCB->blockSignals(true);
        if(list.size() == 1 && _ui->searchCB->currentText().toLower() == list[0].toLower()) {
            _ui->searchCB->clear();
            _ui->searchCB->completer()->popup()->hide();
        } else if(list.isEmpty()) {
            QToolTip::showText(_ui->searchCB->mapToGlobal(_ui->searchCB->rect().bottomLeft()), "Figure not found", nullptr, {}, 3000);
        } else {
            auto currenText = _ui->searchCB->currentText();
            _ui->searchCB->clear();
            _ui->searchCB->addItems(list);
            _ui->searchCB->setCurrentText(currenText);
        }
        _ui->searchCB->blockSignals(false);

    });

    _statusBar = new QStatusBar;
    setStatusBar(_statusBar);
    _formLabel = new QLabel;
    _coordsLabel = new QLabel;
    _workPlaneLabel = new QLabel("XY");
    _currentFigureLabel = new QLabel;
    _magnificationLabel = new QLabel("Scale: 1");
    _statusBar->addPermanentWidget(_formLabel, 3);
    _statusBar->addPermanentWidget(_coordsLabel, 5);
    _statusBar->addPermanentWidget(_currentFigureLabel, 8);
    _statusBar->addPermanentWidget(_magnificationLabel, 1);
    _statusBar->addPermanentWidget(_workPlaneLabel, 1);

    connect(_project, &Project::currentFigureChanged, [&](const QString currentFigureName, const QString previousFigureName) {
        auto figure = _project->findFigure(currentFigureName);
        if(auto curve = dynamic_cast<const CurveFigure*>(figure)) {
            auto& points = curve->points();
            auto maxDev = points[0].dev;
            auto minDev = points[0].dev;
            for(auto& point : points) {
                point.dev > maxDev ? maxDev = point.dev : maxDev = maxDev;
                point.dev < minDev ? minDev = point.dev : minDev = minDev;
            }
            _formLabel->setStyleSheet("color: black");
            _formLabel->setText(QString("Form: %1 (Max: %2, Min: %3)").arg(maxDev - minDev, 0, 'f', _project->precision()).
                arg(maxDev, 0, 'f', _project->precision()).arg(minDev, 0, 'f', _project->precision()));
            _currentFigureLabel->setText(QString("(Curve, %1 Points) %2").arg(curve->points().length()).arg(currentFigureName));
        } else {
            _formLabel->setStyleSheet("color: grey");
            _formLabel->setText(QString("Form: 0 (Max: 0, Min: 0)"));
            _currentFigureLabel->clear();
            if(auto circle = dynamic_cast<const CircleFigure*>(figure)) {
                _currentFigureLabel->setText(QString("(Circle) %1").arg(currentFigureName));
            } else if(auto line = dynamic_cast<const LineFigure*>(figure)) {
                _currentFigureLabel->setText(QString("(Line) %1").arg(currentFigureName));
            } else if(auto point = dynamic_cast<const PointFigure*>(figure)) {
                _currentFigureLabel->setText(QString("(Point) %1").arg(currentFigureName));
            } else if(auto dim = dynamic_cast<const DimFigure*>(figure)) {
                _currentFigureLabel->setText(QString("(Dimension) %1").arg(currentFigureName));
            } else if(auto txt = dynamic_cast<const TextFigure*>(figure)) {
                _currentFigureLabel->setText(QString("(Text) %1").arg(currentFigureName));
            }
        }
    });

    connect(_project, &Project::scaleChanged, [&](double scale) {
        _magnificationLabel->setText(QString("Scale: %1").arg(scale, 0, 'f', _project->precision()));
    });

    connect(_plot, &Plot::mouseMove, [&](QMouseEvent* event) {
        auto coords = _plot->pixelToCoord(Point(event->pos().x(), event->pos().y()));
        auto pr = hypot(coords.x, coords.y);
        auto pa = 0.0;
        coords.y < 0 ? pa = atan2(coords.y, coords.x) * 180 / M_PI + 360 : pa = atan2(coords.y, coords.x) * 180 / M_PI;
        if(auto curve = dynamic_cast<const CurveFigure*>(_project->findFigure(_project->currentFigureName()))) {
            CurvePoint curvePoint;
            int index;
            _plot->findNearestCurvePoint(coords, curve, curvePoint, index);
            _coordsLabel->setText(QString("X: %1; Y: %2 (PA: %3; PR: %4) / Dev = %5 (%6)")
                .arg(coords.x, 0, 'f', _project->precision()).arg(coords.y, 0, 'f', _project->precision())
                .arg(pa, 0, 'f', _project->precision()).arg(pr, 0, 'f', _project->precision())
                .arg(curvePoint.dev, 0, 'f', _project->precision()).arg(index));
        } else {
            _coordsLabel->setText(QString("X: %1; Y: %2 (PA: %3; PR: %4)").arg(coords.x, 0, 'f', _project->precision()).arg(coords.y, 0, 'f', _project->precision())
                .arg(pa, 0, 'f', _project->precision()).arg(pr, 0, 'f', _project->precision()));
        }
    });
}

void CurveWindow::dimensionMenuInit() {
    disableDimensionMenu();
    auto currentFigure = _project->findFigure(_project->currentFigureName());
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

void CurveWindow::createShowHideActions() {
    QAction* showAllAction = _ui->visibilityOnTB->addAction(QIcon("icons/all.ico"), QString(tr("All figures")));
    connect(showAllAction, &QAction::triggered, [&]() { _project->showAllFigures("ANY"); });
    QAction* showCRVAction = _ui->visibilityOnTB->addAction(QIcon("icons/curve.ico"), QString(tr("Curves")));
    connect(showCRVAction, &QAction::triggered, [&]() { _project->showAllFigures("CRV"); });
    QAction* showCIRAction = _ui->visibilityOnTB->addAction(QIcon("icons/circle.ico"), QString(tr("Circles")));
    connect(showCIRAction, &QAction::triggered, [&]() { _project->showAllFigures("CIR"); });
    QAction* showLINAction = _ui->visibilityOnTB->addAction(QIcon("icons/line.ico"), QString(tr("Lines")));
    connect(showLINAction, &QAction::triggered, [&]() { _project->showAllFigures("LIN"); });
    QAction* showPNTAction = _ui->visibilityOnTB->addAction(QIcon("icons/point.ico"), QString(tr("Points")));
    connect(showPNTAction, &QAction::triggered, [&]() { _project->showAllFigures("PNT"); });
    QAction* showDIMAction = _ui->visibilityOnTB->addAction(QIcon("icons/dimension.ico"), QString(tr("Dimensions")));
    connect(showDIMAction, &QAction::triggered, [&]() { _project->showAllFigures("DIM"); });
    QAction* showTXTAction = _ui->visibilityOnTB->addAction(QIcon("icons/text.ico"), QString(tr("Texts")));
    connect(showTXTAction, &QAction::triggered, [&]() { _project->showAllFigures("TXT"); });

    QAction* hideAllAction = _ui->visibilityOffTB->addAction(QIcon("icons/all.ico"), QString(tr("All figures")));
    connect(hideAllAction, &QAction::triggered, [&]() { _project->hideAllFigures("ANY"); });
    QAction* hideCRVAction = _ui->visibilityOffTB->addAction(QIcon("icons/curve.ico"), QString(tr("Curves")));
    connect(hideCRVAction, &QAction::triggered, [&]() { _project->hideAllFigures("CRV"); });
    QAction* hideCIRAction = _ui->visibilityOffTB->addAction(QIcon("icons/circle.ico"), QString(tr("Circles")));
    connect(hideCIRAction, &QAction::triggered, [&]() { _project->hideAllFigures("CIR"); });
    QAction* hideLINAction = _ui->visibilityOffTB->addAction(QIcon("icons/line.ico"), QString(tr("Lines")));
    connect(hideLINAction, &QAction::triggered, [&]() { _project->hideAllFigures("LIN"); });
    QAction* hidePNTAction = _ui->visibilityOffTB->addAction(QIcon("icons/point.ico"), QString(tr("Points")));
    connect(hidePNTAction, &QAction::triggered, [&]() { _project->hideAllFigures("PNT"); });
    QAction* hideDIMAction = _ui->visibilityOffTB->addAction(QIcon("icons/dimension.ico"), QString(tr("Dimensions")));
    connect(hideDIMAction, &QAction::triggered, [&]() { _project->hideAllFigures("DIM"); });
    QAction* hideTXTAction = _ui->visibilityOffTB->addAction(QIcon("icons/text.ico"), QString(tr("Texts")));
    connect(hideTXTAction, &QAction::triggered, [&]() { _project->hideAllFigures("TXT"); });
}

void CurveWindow::connectMenuItems() {
}

void CurveWindow::changeWindowTitle(const QString& projectPath) {
    if(!projectPath.isEmpty()) {
        this->setWindowTitle(_windowTitle + " - " + projectPath);
    }
}

void CurveWindow::setDefualtWindowTitle() {
    this->setWindowTitle(_windowTitle);
}

void CurveWindow::keyPressEvent(QKeyEvent* event) {
    if(event->key() == Qt::Key_Delete && !_project->currentFigureName().isEmpty()) {
        QMessageBox mBox;
        QString name = _project->currentFigureName();

        mBox.setText(QString("Delete %1?").arg(name));
        mBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        int dialogResponse = mBox.exec();
        if(dialogResponse == QMessageBox::Ok) {
            _project->removeFigure(name);
        }
    }
}

void CurveWindow::onSaveProject() {
    QString fileName = QFileDialog::getSaveFileName(
        this,
        tr("Save project"),
        QDir::homePath(),
        tr("CRV files (*.crv);")
    );

    if(!fileName.isEmpty()) {
        if(fileName.endsWith(".crv", Qt::CaseInsensitive)) {
            fileName.chop(4);
        }

        FileSystem::saveProject(fileName);
    }
}

CurveWindow::~CurveWindow() {
    delete _ui;
}
