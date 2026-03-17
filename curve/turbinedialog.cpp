#include "curve/pch.h"

#include "turbinedialog.h"
#include "ui_turbinedialog.h"

TurbineDialog::TurbineDialog() : _ui(new Ui::TurbineDialog()) {
    _ui->setupUi(this);
    _curveGraphics = new CurveGraphicsWidget();
    _containerLayout = new QGridLayout(_ui->container);
    _containerLayout->setContentsMargins(0, 0, 0, 0);
    _containerLayout->addWidget(_curveGraphics, 0, 0);
    this->setWindowTitle("Calculate turbine");
    setupWindow();
    connect(_ui->runButton, &QPushButton::clicked, this, &TurbineDialog::run);
    connect(_ui->closeButton, &QPushButton::clicked, this, &TurbineDialog::closeWindow);
    connect(_ui->calculateButton, &QPushButton::clicked, this, &TurbineDialog::onCalculateNominals);
    connect(_ui->listNominal, &QListWidget::itemClicked, this, &TurbineDialog::changeItemOfList);
    connect(_ui->listMeasured, &QListWidget::itemClicked, this, &TurbineDialog::changeItemOfList);
    connect(_ui->paramList, &QListWidget::itemClicked, this, &TurbineDialog::updateParamOutputView);
    connect(_ui->profileType, &QComboBox::currentIndexChanged, this, &TurbineDialog::onProfileTypeChange);
    connect(_ui->bestFitLE, &QComboBox::currentIndexChanged, this, &TurbineDialog::onNoBestFitLEClick);
    connect(_ui->bestFitTE, &QComboBox::currentIndexChanged, this, &TurbineDialog::onNoBestFitTEClick);
    connect(_ui->showNumDevLE, &QComboBox::currentIndexChanged, this, &TurbineDialog::onShowNumDevLEClick);
    connect(_ui->showNumDevTE, &QComboBox::currentIndexChanged, this, &TurbineDialog::onShowNumDevTEClick);
    connect(_ui->needPreparePoints, &QCheckBox::clicked, this, &TurbineDialog::onPreparePointsClick);
    connect(_ui->needRemoveSimilar, &QCheckBox::clicked, this, &TurbineDialog::onDeleteSimilarClick);
    connect(_ui->needRadiusCorrection, &QCheckBox::clicked, this, &TurbineDialog::onRadiusCompensationClick);
    connect(_ui->needUse3DVectors, &QCheckBox::clicked, this, &TurbineDialog::onUse3DVectorsForCompClick);
    connect(_ui->needPrintTemplate, &QCheckBox::clicked, this, &TurbineDialog::onNeedPrintTemplateClick);
    connect(_ui->radiusCompensation, &QLineEdit::textChanged, this, &TurbineDialog::onRadiusCompensationChange);
}

void TurbineDialog::setupWindow() {
    _doubleValidator = new QDoubleValidator();
    _doubleValidator->setDecimals(9);
    _doubleValidator->setLocale(QLocale::C);
    _doubleValidator->setNotation(QDoubleValidator::StandardNotation);

    _intValidator = new QIntValidator();
    _intValidator->setLocale(QLocale::C);
    _intValidator->setRange(0, 1000);

    _ui->paramsTab->tabBar()->hide();

    //profiles
    _ui->zoneLE->setValidator(_doubleValidator);
    _ui->zoneTE->setValidator(_doubleValidator);

    //prepare points
    _ui->limitForEqualPoints->setValidator(_doubleValidator);
    _ui->radiusCompensation->setValidator(_doubleValidator);
    _ui->radius3DCompensation->setValidator(_doubleValidator);

    //global form
    _ui->globalApmlification->setValidator(_doubleValidator);
    _ui->globalZoom->setValidator(_doubleValidator);

    //LE form
    _ui->amplificationLE->setValidator(_doubleValidator);
    _ui->zoomLE->setValidator(_doubleValidator);
    _ui->valueOfSetLE->setValidator(_intValidator);

    //TE form
    _ui->amplificationTE->setValidator(_doubleValidator);
    _ui->zoomTE->setValidator(_doubleValidator);
    _ui->valueOfSetTE->setValidator(_intValidator);

    //parameters
    _ui->upperTolMaxWidth->setValidator(_doubleValidator);
    _ui->lowerTolMaxWidth->setValidator(_doubleValidator);
    _ui->upperTolMaxWidthX->setValidator(_doubleValidator);
    _ui->lowerTolMaxWidthX->setValidator(_doubleValidator);
    _ui->upperTolMaxWidthY->setValidator(_doubleValidator);
    _ui->lowerTolMaxWidthY->setValidator(_doubleValidator);
    _ui->upperTolChordLength->setValidator(_doubleValidator);
    _ui->lowerTolChordLength->setValidator(_doubleValidator);
    _ui->upperTolWidthLE->setValidator(_doubleValidator);
    _ui->lowerTolWidthLE->setValidator(_doubleValidator);
    _ui->upperTolWidthTE->setValidator(_doubleValidator);
    _ui->lowerTolWidthTE->setValidator(_doubleValidator);
    _ui->upperTolRadiusLE->setValidator(_doubleValidator);
    _ui->lowerTolRadiusLE->setValidator(_doubleValidator);
    _ui->upperTolRadiusTE->setValidator(_doubleValidator);
    _ui->lowerTolRadiusTE->setValidator(_doubleValidator);
    _ui->atWidthLE->setValidator(_doubleValidator);
    _ui->atWidthTE->setValidator(_doubleValidator);
    _ui->degreeAngleLE->setValidator(_doubleValidator);
    _ui->degreeAngleTE->setValidator(_doubleValidator);

    disableUnrealizedParams();
}

void TurbineDialog::disableUnrealizedParams() {
    auto globalAxisModel = dynamic_cast<QStandardItemModel*>(_ui->globalAxis->model());
    auto globalAxisCenter = globalAxisModel->item(1);
    globalAxisCenter->setEnabled(false);

    auto axisLEModel = dynamic_cast<QStandardItemModel*>(_ui->axisLE->model());
    auto axisLECenter = axisLEModel->item(1);
    axisLECenter->setEnabled(false);

    auto axisTEModel = dynamic_cast<QStandardItemModel*>(_ui->axisTE->model());
    auto axisTECenter = axisTEModel->item(1);
    axisTECenter->setEnabled(false);

    auto globalProfileTypeModel = dynamic_cast<QStandardItemModel*>(_ui->profileType->model());
    auto wholeGlobal = globalProfileTypeModel->item(0);
    wholeGlobal->setEnabled(false);
    auto globalWithoutEdges = globalProfileTypeModel->item(1);
    globalWithoutEdges->setEnabled(false);
    auto globalWithoutTE = globalProfileTypeModel->item(2);
    globalWithoutTE->setEnabled(false);
    auto globalWithoutEdgesLSQLocal = globalProfileTypeModel->item(3);
    globalWithoutEdgesLSQLocal->setEnabled(false);
    _ui->profileType->setCurrentIndex(4);

    auto globalBestFitModel = dynamic_cast<QStandardItemModel*>(_ui->globalBestFit->model());
    auto globalNoFit = globalBestFitModel->item(0);
    globalNoFit->setEnabled(false);
    auto wholeGlobalLSQ = globalBestFitModel->item(1);
    wholeGlobalLSQ->setEnabled(false);
    auto globalWithoutEdgesLSQ = globalBestFitModel->item(2);
    globalWithoutEdgesLSQ->setEnabled(false);
    auto globalWithoutTELSQ = globalBestFitModel->item(3);
    globalWithoutTELSQ->setEnabled(false);
    auto globalTwoPointsAt = globalBestFitModel->item(4);
    globalTwoPointsAt->setEnabled(false);
    auto globalFitInTolBand = globalBestFitModel->item(5);
    globalFitInTolBand->setEnabled(false);
    _ui->globalBestFit->setCurrentIndex(6);

    _ui->bestFitType->setCurrentIndex(6);

    _ui->needFormLE->setChecked(false);
    _ui->needFormLE->setEnabled(false);
    _ui->needFormTE->setChecked(false);
    _ui->needFormTE->setEnabled(false);
}

void TurbineDialog::initialize() {
    _reportSettings = std::make_shared<ReportSettings>();
    _ui->valueOfSetLE->hide();
    _ui->valueOfSetTE->hide();

    //_ui->globalBestFit->setCurrentIndex(1);
    _ui->globalAxis->setCurrentIndex(2);
    _ui->axisLE->setCurrentIndex(2);
    _ui->axisTE->setCurrentIndex(2);

    _ui->toliranceLE->hide();
    _ui->toliranceTE->hide();

    _ui->reportList->setCurrentRow(0);
    _ui->commentLabel->show();
    _ui->comment->show();
    _ui->reportList->show();

    auto figures = Project::instance().figures();
    _curveGraphics->initialization();

    for(auto item : figures) {
        if(dynamic_cast<CurveFigure*>(item)) {
            auto name = item->name();
            _ui->listMeasured->addItem(new QListWidgetItem(name));
            _ui->listNominal->addItem(new QListWidgetItem(name));
        }
    }

    _ui->paramList->setCurrentItem(nullptr);

    onPreparePointsClick();
    exec();
}

void TurbineDialog::run() {
    auto selectedNominalItems = _ui->listNominal->selectedItems();
    auto currentNominal = selectedNominalItems.length() == 1 ? selectedNominalItems[0] : nullptr;
    auto selectedMeasuredItems = _ui->listMeasured->selectedItems();
    auto currentMeasured = selectedMeasuredItems.length() == 1 ? selectedMeasuredItems[0] : nullptr;

    if(currentNominal == nullptr) {
        QMessageBox::warning(nullptr, "Warning", "Please select the nominal curve!", "Ok");
        return;
    }
    if(currentMeasured == nullptr) {
        QMessageBox::warning(nullptr, "Warning", "Please select the measured curve!", "Ok");
        return;
    }
    auto nominalFigure = Project::instance().findFigure(currentNominal->text());
    auto nominalCurve = dynamic_cast<const CurveFigure*>(nominalFigure);
    auto measuredFigure = Project::instance().findFigure(currentMeasured->text());
    auto measuredCurve = dynamic_cast<const CurveFigure*>(measuredFigure);

    assert(nominalCurve && measuredCurve);

    if(_ui->needUse3DVectors->isChecked()) {
        auto &nominalPoint = nominalCurve->points()[0];
        auto &measuredPoint = measuredCurve->points()[0];

        if(!nominalPoint.i && !nominalPoint.j || !measuredPoint.i && !measuredPoint.j) {
            auto message = "If you want to use 3D vectors for comp.,\nthe vectors of the curves should not be zero!";
            QMessageBox::warning(nullptr, "Warning", message, "Ok");
            return;
        }
    }
    _reportSettings->clearTurbineParameters();
    //MacrosManager::executeWithoutLogging([&]() { onCalculateNominals(); });

    setSettings();
    try {
        ReportGenerator::createReport(_reportSettings);
    } catch(...) {
    }
}

void TurbineDialog::onCalculateNominals() {
    auto selectedNominalItems = _ui->listNominal->selectedItems();
    auto currentNominal = selectedNominalItems.length() != 0 ? selectedNominalItems[0] : nullptr;

    if(currentNominal == nullptr) {
        QMessageBox::warning(nullptr, "Warning", "Please select the nominal curve!", "Ok");
        return;
    }
    auto nomFigureName = _ui->listNominal->currentItem()->text();
    calculateNominals(nomFigureName);
}

void TurbineDialog::calculateNominals(const QString &curveName) {
    _reportSettings->clearTurbineParameters();
    auto precision = Project::instance().precision();
    for(auto i = 0; i < _ui->paramList->count(); i++) {
        auto item = _ui->paramList->item(i);
        if(item->checkState() == Qt::Checked) {
            try {
                using TurbineParamType = TurbineParameter::Type;
                auto turbineParamType = static_cast<TurbineParamType>(i);
                auto project = &Project::instance();
                switch(turbineParamType) {
                    case TurbineParamType::MaxWidth:
                    {
                        auto maxDia = new MaxDiameter(0, _ui->upperTolMaxWidth->text().toDouble(),
                            _ui->lowerTolMaxWidth->text().toDouble());
                        maxDia->calculateNominal(curveName, Function18Params(), project);
                        _ui->nominalMaxWidth->setText(QString::number(maxDia->nominal, 'f', precision));
                        _reportSettings->appendTurbineParameter(maxDia);
                        break;
                    }
                    case TurbineParamType::MaxWidthX:
                    {
                        auto xMaxDia = new XMaxDiameter(0, _ui->upperTolMaxWidthX->text().toDouble(),
                            _ui->lowerTolMaxWidthX->text().toDouble());
                        xMaxDia->calculateNominal(curveName, Function18Params(), project);
                        _ui->nominalMaxWidthX->setText(QString::number(xMaxDia->nominal, 'f', precision));
                        _reportSettings->appendTurbineParameter(xMaxDia);
                        break;
                    }
                    case TurbineParamType::MaxWidthY:
                    {
                        auto yMaxDia = new YMaxDiameter(0, _ui->upperTolMaxWidthX->text().toDouble(),
                            _ui->lowerTolMaxWidthX->text().toDouble());
                        yMaxDia->calculateNominal(curveName, Function18Params(), project);
                        _ui->nominalMaxWidthY->setText(QString::number(yMaxDia->nominal, 'f', precision));
                        _reportSettings->appendTurbineParameter(yMaxDia);
                        break;
                    }
                    case TurbineParamType::ChordLength:
                    {
                        auto chordLength = new ChordLength(0, _ui->upperTolChordLength->text().toDouble(),
                            _ui->lowerTolChordLength->text().toDouble());
                        chordLength->calculateNominal(curveName, Function18Params(), project);
                        _ui->nominalChordLength->setText(QString::number(chordLength->nominal, 'f', precision));
                        _reportSettings->appendTurbineParameter(chordLength);
                        break;
                    }
                    case TurbineParamType::LEWidth:
                    {
                        auto widthLE = new WidthLE(0, _ui->upperTolWidthLE->text().toDouble(),
                            _ui->lowerTolWidthLE->text().toDouble(), _ui->atWidthLE->text());
                        widthLE->calculateNominal(curveName, Function18Params(), project);
                        _ui->nominalWidthLE->setText(QString::number(widthLE->nominal, 'f', precision));
                        _reportSettings->appendTurbineParameter(widthLE);
                        break;
                    }
                    case TurbineParamType::TEWidth:
                    {
                        auto widthTE = new WidthTE(0, _ui->upperTolWidthTE->text().toDouble(),
                            _ui->lowerTolWidthTE->text().toDouble(), _ui->atWidthTE->text());
                        widthTE->calculateNominal(curveName, Function18Params(), project);
                        _ui->nominalWidthTE->setText(QString::number(widthTE->nominal, 'f', precision));
                        _reportSettings->appendTurbineParameter(widthTE);
                        break;
                    }
                    case TurbineParamType::LERadius:
                    {
                        auto radiusLE = new RadiusLE(0, _ui->upperTolRadiusLE->text().toDouble(),
                            _ui->lowerTolRadiusLE->text().toDouble(), _ui->degreeAngleLE->text());
                        radiusLE->calculateNominal(curveName, Function18Params(), project);
                        _ui->nominalRadiusLE->setText(QString::number(radiusLE->nominal, 'f', precision));
                        _reportSettings->appendTurbineParameter(radiusLE);
                        break;
                    }
                    case TurbineParamType::TERadius:
                    {
                        auto radiusTE = new RadiusTE(0, _ui->upperTolRadiusTE->text().toDouble(),
                            _ui->lowerTolRadiusTE->text().toDouble(), _ui->degreeAngleTE->text());
                        radiusTE->calculateNominal(curveName, Function18Params(), project);
                        _ui->nominalRadiusTE->setText(QString::number(radiusTE->nominal, 'f', precision));
                        _reportSettings->appendTurbineParameter(radiusTE);
                        break;
                    }
                    case TurbineParamType::MinX:
                    {
                        auto minX = new MinX(0, _ui->upperTolMinX->text().toDouble(),
                            _ui->lowerTolMinX->text().toDouble());
                        minX->calculateNominal(curveName, Function18Params(), project);
                        _ui->nominalMinX->setText(QString::number(minX->nominal, 'f', precision));
                        _reportSettings->appendTurbineParameter(minX);
                        break;
                    }
                }
            } catch(...) {
            }
        }
    }
}

void TurbineDialog::setSettings() {
    _reportSettings->setNominalName(_ui->listNominal->currentItem()->text());
    _reportSettings->setMeasuredName(_ui->listMeasured->currentItem()->text());

    _reportSettings->setLEDirection(static_cast<ReportSettings::LEDirection>(_ui->directionLE->currentIndex()));
    _reportSettings->setZone(_ui->zoneLE->text().toInt(), _ui->zoneTE->text().toInt(),
        static_cast<ReportSettings::MeasureType>(_ui->measureType->currentIndex()));

    _reportSettings->setPreProcessSettings(_ui->needSort->isChecked(), _ui->needRemoveSimilar->isChecked(),
        _ui->needRadiusCorrection->isChecked(), _ui->needUse3DVectors->isChecked());
    if(_ui->needRemoveSimilar->isChecked()) {
        _reportSettings->setLimitForEqualPoints(_ui->limitForEqualPoints->text().toDouble());
    }
    if(_ui->needRadiusCorrection->isChecked()) {
        _reportSettings->setRadiusCompensation(_ui->radiusCompensation->text().toDouble());
    }

    _reportSettings->setProfileType(static_cast<ReportSettings::Profile>(_ui->profileType->currentIndex())), onProfileTypeChange();
    _reportSettings->setGlobalBestFit(static_cast<ReportSettings::GlobalBestFit>(_ui->globalBestFit->currentIndex()));
    _reportSettings->setEdgesBestFit(
        static_cast<ReportSettings::EdgeBestFit>(_ui->bestFitLE->currentIndex()),
        static_cast<ReportSettings::EdgeBestFit>(_ui->bestFitTE->currentIndex()));
    _reportSettings->setBestFitType(static_cast<ReportSettings::BestFitType>(_ui->bestFitType->currentIndex()));
    _reportSettings->setStretch(_ui->stretchLE->isChecked(), _ui->stretchTE->isChecked());
    _reportSettings->setAmplification(_ui->globalApmlification->text().toDouble(), _ui->amplificationLE->text().toDouble(), _ui->amplificationTE->text().toDouble());
    _reportSettings->setAxisDisplayType(
        static_cast<ReportSettings::Axis>(_ui->globalAxis->currentIndex()),
        static_cast<ReportSettings::Axis>(_ui->axisLE->currentIndex()),
        static_cast<ReportSettings::Axis>(_ui->axisTE->currentIndex()));
    _reportSettings->setNeedAdditionalFigures(_ui->needMaxDia->isChecked(), _ui->needMCL->isChecked(), _ui->needContactLine->isChecked());
    _reportSettings->setVisibilityEdges(_ui->needFormLE->isChecked(), _ui->needFormTE->isChecked());
    _reportSettings->setTypeOfShowDevs(
        static_cast<ReportSettings::TypeOfShowDevs>(_ui->showNumDevLE->currentIndex()),
        static_cast<ReportSettings::TypeOfShowDevs>(_ui->showNumDevTE->currentIndex()));
    _reportSettings->setValueOfSetShowDevs(_ui->valueOfSetLE->text().toInt(), _ui->valueOfSetTE->text().toInt());

    QString binOutputFormMode = "";
    binOutputFormMode += QString::number(static_cast<int>(_ui->infLT->isChecked()));
    binOutputFormMode += QString::number(static_cast<int>(_ui->supUT->isChecked()));
    binOutputFormMode += QString::number(static_cast<int>(_ui->maxAbs->isChecked()));
    binOutputFormMode += QString::number(static_cast<int>(_ui->max->isChecked()));
    binOutputFormMode += QString::number(static_cast<int>(_ui->min->isChecked()));
    binOutputFormMode += QString::number(static_cast<int>(_ui->form->isChecked()));
    binOutputFormMode += QString::number(static_cast<int>(_ui->minMax->isChecked()));
    _reportSettings->setOutputFormMode(binOutputFormMode.toInt(nullptr, 2));

    auto evaluationPlace = _ui->markPlaceNominal->isChecked() ? ReportSettings::Evaluation::Nominal : ReportSettings::Evaluation::Measured;
    auto evaluationDirection = _ui->markDirectionNominal->isChecked() ? ReportSettings::Evaluation::Nominal : ReportSettings::Evaluation::Measured;
    _reportSettings->setEvaluation(evaluationPlace, evaluationDirection);

    if(_ui->needPrintTemplate->isChecked()) {
        _reportSettings->setNeedPrintWithTemplate(true);
        _reportSettings->setReportTemplate(static_cast<ReportSettings::Template>(_ui->reportList->currentRow()));
        _reportSettings->setComment(_ui->comment->text());
    }
}


void TurbineDialog::updateParamOutputView() {
    auto currentParam = _ui->paramList->currentItem();

    if(currentParam) {
        auto currentIndex = _ui->paramList->currentRow();
        _ui->paramsTab->setCurrentIndex(currentIndex);
    }
}

void TurbineDialog::changeItemOfList() {
    auto selectedItemsOfListNominal = _ui->listNominal->selectedItems();
    auto selectedItemsOfListMeasured = _ui->listMeasured->selectedItems();
    auto currentNominalItem = selectedItemsOfListNominal.length() == 1 ? selectedItemsOfListNominal[0] : nullptr;
    auto currentMeasuredItem = selectedItemsOfListMeasured.length() == 1 ? selectedItemsOfListMeasured[0] : nullptr;

    if(currentNominalItem && !currentMeasuredItem) {
        auto nominalFigure = Project::instance().findFigure(currentNominalItem->text());
        auto nominalCurve = dynamic_cast<const CurveFigure*>(nominalFigure);
        assert(nominalCurve);
        _curveGraphics->drawCurve(nominalCurve, Qt::green, 0.1);
    } else if(!currentNominalItem && currentMeasuredItem) {
        auto measuredFigure = Project::instance().findFigure(currentMeasuredItem->text());
        auto measuredCurve = dynamic_cast<const CurveFigure*>(measuredFigure);
        assert(measuredCurve);
        _curveGraphics->drawCurve(measuredCurve, Qt::blue, 0.1);
    } else if(currentNominalItem && currentMeasuredItem) {
        auto nominalFigure = Project::instance().findFigure(currentNominalItem->text());
        auto nominalCurve = dynamic_cast<const CurveFigure*>(nominalFigure);
        auto measuredFigure = Project::instance().findFigure(currentMeasuredItem->text());
        auto measuredCurve = dynamic_cast<const CurveFigure*>(measuredFigure);
        assert(nominalCurve && measuredCurve);
        _curveGraphics->drawCurve(nominalCurve, Qt::green, 0.1, measuredCurve, Qt::blue, 0.1);
    }
}

void TurbineDialog::onProfileTypeChange() {
    auto profileType = static_cast<ReportSettings::Profile>(_ui->profileType->currentIndex());
    if(profileType == ReportSettings::Profile::WithoutEdgesLSQ || profileType == ReportSettings::Profile::WithoutEdgesForm) {
        auto edgeBestFit = ReportSettings::EdgeBestFit::NoFit;
        _ui->bestFitLE->setCurrentIndex(int(edgeBestFit));
        _ui->bestFitTE->setCurrentIndex(int(edgeBestFit));
    }
}

void TurbineDialog::onNoBestFitLEClick() {
    auto edgeBestFit = static_cast<ReportSettings::EdgeBestFit>(_ui->bestFitLE->currentIndex());
    edgeBestFit == ReportSettings::EdgeBestFit::NoFit ? _ui->toliranceLE->show() : _ui->toliranceLE->hide();
}

void TurbineDialog::onNoBestFitTEClick() {
    auto edgeBestFit = static_cast<ReportSettings::EdgeBestFit>(_ui->bestFitTE->currentIndex());
    edgeBestFit == ReportSettings::EdgeBestFit::NoFit ? _ui->toliranceTE->show() : _ui->toliranceTE->hide();
}

void TurbineDialog::onShowNumDevLEClick() {
    auto typeOfShowDevs = static_cast<ReportSettings::TypeOfShowDevs>(_ui->showNumDevLE->currentIndex());
    typeOfShowDevs == ReportSettings::TypeOfShowDevs::Set ? _ui->valueOfSetLE->show() : _ui->valueOfSetLE->hide();
}

void TurbineDialog::onShowNumDevTEClick() {
    auto typeOfShowDevs = static_cast<ReportSettings::TypeOfShowDevs>(_ui->showNumDevTE->currentIndex());
    typeOfShowDevs == ReportSettings::TypeOfShowDevs::Set ? _ui->valueOfSetTE->show() : _ui->valueOfSetTE->hide();
}

void TurbineDialog::onPreparePointsClick() {
    auto isChecked = _ui->needPreparePoints->isChecked();
    if(isChecked) {
        _ui->needSort->show();
        _ui->needRemoveSimilar->show();
        _ui->needRadiusCorrection->show();
        _ui->needUse3DVectors->show();
    } else {
        _ui->needSort->hide();
        _ui->needRemoveSimilar->hide();
        _ui->limitForEqualPoints->hide();
        _ui->needRadiusCorrection->hide();
        _ui->radiusCompensation->hide();
        _ui->radius3DCompensation->hide();
        _ui->needUse3DVectors->hide();
    }
}

void TurbineDialog::onDeleteSimilarClick() {
    auto isChecked = _ui->needRemoveSimilar->isChecked();
    isChecked ? _ui->limitForEqualPoints->show() : _ui->limitForEqualPoints->hide();
}

void TurbineDialog::onRadiusCompensationClick() {
    auto isChecked = _ui->needRadiusCorrection->isChecked();
    if(isChecked) {
        _ui->radiusCompensation->show();
        _ui->radius3DCompensation->show();
    } else {
        _ui->radiusCompensation->hide();
        _ui->radius3DCompensation->hide();
    }
}

void TurbineDialog::onUse3DVectorsForCompClick() {
    auto isChecked = _ui->needUse3DVectors->isChecked();
    if(isChecked) {
        _ui->radiusCompensation->show();
        _ui->radius3DCompensation->show();
    } else {
        _ui->radiusCompensation->hide();
        _ui->radius3DCompensation->hide();
    }
}

void TurbineDialog::onRadiusCompensationChange() {
    auto radius = _ui->radiusCompensation->text();
    _ui->radius3DCompensation->setText(radius);
}

void TurbineDialog::onNeedPrintTemplateClick() {
    if(_ui->needPrintTemplate->isChecked()) {
        _ui->commentLabel->show();
        _ui->comment->show();
        _ui->reportList->show();
    } else {
        _ui->commentLabel->hide();
        _ui->comment->hide();
        _ui->reportList->hide();
    }
}

void TurbineDialog::closeEvent(QCloseEvent * event) {
    closeWindow();
}

void TurbineDialog::closeWindow() {
    close();
    _ui->listMeasured->clear();
    _ui->listNominal->clear();

    _ui->directionLE->setCurrentIndex(0);
    _ui->zoneLE->setText("5");
    _ui->zoneTE->setText("5");
    _ui->measureType->setCurrentIndex(0);

    _ui->needPreparePoints->setChecked(false);
    _ui->needSort->setChecked(false);
    _ui->needRemoveSimilar->setChecked(false);
    _ui->needRadiusCorrection->setChecked(false);
    _ui->needUse3DVectors->setChecked(false);

    _ui->formTab->setCurrentIndex(0);
    _ui->profileType->setCurrentIndex(4);
    _ui->globalBestFit->setCurrentIndex(6);
    _ui->bestFitType->setCurrentIndex(6);
    _ui->averageDeviation->setChecked(false);
    _ui->stretchLE->setChecked(false);
    _ui->stretchTE->setChecked(false);
    _ui->globalApmlification->setText("5");
    _ui->globalZoom->setText("0");
    _ui->globalAxis->setCurrentIndex(0);
    _ui->needMCL->setChecked(false);
    _ui->needMaxDia->setChecked(false);
    _ui->needContactLine->setChecked(false);

    _ui->needFormLE->setChecked(false);
    _ui->bestFitLE->setCurrentIndex(0);
    _ui->showNumDevLE->setCurrentIndex(0);
    _ui->amplificationLE->setText("1");
    _ui->zoomLE->setText("0");
    _ui->axisLE->setCurrentIndex(0);

    _ui->needFormTE->setChecked(false);
    _ui->bestFitTE->setCurrentIndex(0);
    _ui->showNumDevTE->setCurrentIndex(0);
    _ui->amplificationTE->setText("1");
    _ui->zoomTE->setText("0");
    _ui->axisTE->setCurrentIndex(0);

    _ui->minMax->setChecked(true);
    _ui->form->setChecked(false);
    _ui->min->setChecked(true);
    _ui->max->setChecked(true);
    _ui->maxAbs->setChecked(false);
    _ui->supUT->setChecked(true);
    _ui->infLT->setChecked(true);
    _ui->markPlaceNominal->setChecked(true);
    _ui->markDirectionNominal->setChecked(true);
    _ui->atWidthLE->setText("1");
    _ui->atWidthTE->setText("1");

    _ui->peakAnalysis->setCurrentIndex(0);

    _ui->paramList->blockSignals(true);
    for(auto i = 0; i < _ui->paramList->count(); i++) {
        _ui->paramList->item(i)->setCheckState(Qt::Unchecked);
    }
    _ui->paramList->blockSignals(false);

    _ui->reportTab->setCurrentIndex(0);
    _ui->needPrintTemplate->setChecked(true);
    _ui->comment->clear();
    _ui->profileNames->clear();
}

TurbineDialog::~TurbineDialog() {
    delete _ui;
}
