#include "curve/pch.h"
#include "turbinewindow.h"

TurbineWindow::TurbineWindow(Project *project, Plot *plot) : _ui(new Ui::TurbineWindow()) {
    _ui->setupUi(this);
    _project = project;
    _plot = plot;
    _reportSettings = std::make_shared<ReportSettings>();
    _curveGraphics = new CurveGraphicsWidget();
    _containerLayout = new QGridLayout(_ui->container);
    _containerLayout->setContentsMargins(0, 0, 0, 0);
    _containerLayout->addWidget(_curveGraphics, 0, 0);
    _message = new QMessageBox(this);
    _message->setWindowTitle("Error");
    this->setWindowTitle("Calculate turbine");
    setupWindow();
    connect(_ui->runButton, &QPushButton::clicked, this, &TurbineWindow::run);
    connect(_ui->closeButton, &QPushButton::clicked, this, &TurbineWindow::closeWindow);
    connect(_ui->calculateButton, &QPushButton::clicked, this, &TurbineWindow::calculateNominals);
    connect(_ui->listNominal, &QListWidget::itemClicked, this, &TurbineWindow::changeItemOfList);
    connect(_ui->listMeasured, &QListWidget::itemClicked, this, &TurbineWindow::changeItemOfList);
    connect(_ui->paramList, &QListWidget::itemClicked, this, &TurbineWindow::updateParamOutputView);
    connect(_ui->bestFitLE, &QComboBox::currentIndexChanged, this, &TurbineWindow::onNoBestFitLEClick);
    connect(_ui->bestFitTE, &QComboBox::currentIndexChanged, this, &TurbineWindow::onNoBestFitTEClick);
    connect(_ui->showNumDevLE, &QComboBox::currentIndexChanged, this, &TurbineWindow::onShowNumDevLEClick);
    connect(_ui->showNumDevTE, &QComboBox::currentIndexChanged, this, &TurbineWindow::onShowNumDevTEClick);
    connect(_ui->needPreparePoints, &QCheckBox::clicked, this, &TurbineWindow::onPreparePointsClick);
    connect(_ui->needRemoveSimilar, &QCheckBox::clicked, this, &TurbineWindow::onDeleteSimilarClick);
    connect(_ui->needRadiusCorrection, &QCheckBox::clicked, this, &TurbineWindow::onRadiusCorrectionClick);
    connect(_ui->needPrintTemplate, &QCheckBox::clicked, this, &TurbineWindow::onNeedPrintTemplateClick);
    connect(_ui->upperValue, &QLineEdit::textChanged, this, &TurbineWindow::onUpperValueChange);
}

void TurbineWindow::setupWindow() {
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
    _ui->upperValue->setValidator(_doubleValidator);
    _ui->lowerValue->setValidator(_doubleValidator);

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

    disableUnrealizedParams();
}

void TurbineWindow::disableUnrealizedParams() {
    auto model1 = dynamic_cast<QStandardItemModel*>(_ui->globalAxis->model());
    auto item1 = model1->item(1);
    item1->setEnabled(false);

    auto model2 = dynamic_cast<QStandardItemModel*>(_ui->axisLE->model());
    auto item2 = model2->item(1);
    item2->setEnabled(false);

    auto model3 = dynamic_cast<QStandardItemModel*>(_ui->axisTE->model());
    auto item3 = model3->item(1);
    item3->setEnabled(false);
}

void TurbineWindow::initialization() {
    _ui->limitForEqualPoints->hide();
    _ui->upperValue->hide();
    _ui->lowerValue->hide();
    _ui->valueOfSetLE->hide();
    _ui->valueOfSetTE->hide();

    _ui->toliranceLE->hide();
    _ui->toliranceTE->hide();

    _ui->reportList->setCurrentRow(0);
    _ui->commentLabel->show();
    _ui->comment->show();
    _ui->reportList->show();

    auto figures = _project->figures();
    _curveGraphics->initialization();

    for(auto item : figures) {
        if(dynamic_cast<CurveFigure*>(item)) {
            auto name = item->name();
            _ui->listMeasured->addItem(new QListWidgetItem(name));
            _ui->listNominal->addItem(new QListWidgetItem(name));
        }
    }

    if(_ui->paramList->item(0)->data(Qt::UserRole).isNull()) {
        for(auto i = 0; i < _ui->paramList->count(); i++) {
            _ui->paramList->item(i)->setData(Qt::UserRole, i);
        }
    }
    _ui->paramList->setCurrentItem(nullptr);

    onPreparePointsClick();
    exec();
}

void TurbineWindow::run() {
    auto selectedNominalItems = _ui->listNominal->selectedItems();
    auto currentNominal = selectedNominalItems.length() == 1 ? selectedNominalItems[0] : nullptr;
    auto selectedMeasuredItems = _ui->listMeasured->selectedItems();
    auto currentMeasured = selectedMeasuredItems.length() == 1 ? selectedMeasuredItems[0] : nullptr;

    if(currentNominal == nullptr) {
        _message->setText("Please select the nominal curve!");
        _message->exec();
        return;
    }
    if(currentMeasured == nullptr) {
        _message->setText("Please select the measured curve!");
        _message->exec();
        return;
    }

    QString filePath;
    if(_ui->needPrintTemplate->isChecked()) {
        filePath = QFileDialog::getSaveFileName(nullptr, "Save report", "", "*.html");
    }
    if(_ui->needPrintTemplate->isChecked() && filePath != nullptr || !_ui->needPrintTemplate->isChecked()) {
        MacrosManager::executeWithoutLogging([&]() { calculateMeasured(); });

        auto nominalFigure = _project->findFigure(currentNominal->text());
        auto nominalCurve = dynamic_cast<const CurveFigure*>(nominalFigure);
        auto measuredFigure = _project->findFigure(currentMeasured->text());
        auto measuredCurve = dynamic_cast<const CurveFigure*>(measuredFigure);

        assert(nominalCurve && measuredCurve);

        setSettings(filePath);

        MacrosManager::executeWithoutLogging(ReportGenerator::createReport, _project, _plot, _reportSettings);
        MacrosManager::log(MacrosManager::CreateReport, ReportSettings::convertToQMap(_reportSettings));
    }
}

void TurbineWindow::calculateNominals() {
    auto selectedNominalItems = _ui->listNominal->selectedItems();
    auto currentNominal = _ui->listNominal->currentItem();

    if(currentNominal == nullptr) {
        _message->setText("Please select the nominal curve!");
        _message->exec();
        return;
    }
    auto nomFigureName = _ui->listNominal->currentItem()->text();
    auto paramsResult = calculate(nomFigureName, CurveType::Nominal);
    setNominalParams(paramsResult);
}

void TurbineWindow::calculateMeasured() {
    auto measFigureName = _ui->listNominal->currentItem()->text();
    auto paramsResult = calculate(measFigureName, CurveType::Nominal);
    setMeasuredParams(paramsResult);
}

QMap<TurbineWindow::Operation, double> TurbineWindow::calculate(const QString &curveName, CurveType curveType) {
    QMap<Operation, double> paramsResult;

    for(auto i = 0; i < _ui->paramList->count(); i++) {
        auto item = _ui->paramList->item(i);
        if(item->checkState() == Qt::Checked) {
            try {
                auto operation = static_cast<Operation>(item->data(Qt::UserRole).toInt());
                switch(operation) {
                    case Operation::MaxWidth:
                    case Operation::MaxWidthX:
                    case Operation::MaxWidthY:
                    {
                        auto maxCircle = Algorithms::getMaxCircle(curveName, new Function18Params(), _project);
                        auto diameter = maxCircle.radius() * 2;
                        auto xCenter = maxCircle.center().x;
                        auto yCenter = maxCircle.center().y;

                        if(operation == Operation::MaxWidth) {
                            paramsResult.insert(operation, diameter);
                        }
                        if(operation == Operation::MaxWidthX) {
                            paramsResult.insert(operation, xCenter);
                        }
                        if(operation == Operation::MaxWidthY) {
                            paramsResult.insert(operation, yCenter);
                        }
                        break;
                    }
                    case Operation::ContactLineLength:
                    {
                        auto contactLine = Algorithms::getContactLineLength(curveName, new Function18Params(), _project);
                        paramsResult.insert(operation, contactLine);
                        break;
                    }
                    case Operation::WidthLE:
                    case Operation::WidthTE:
                    {
                        auto result = Algorithms::getWidthOfEdges(curveName, _ui->atWidthLE->text().toDouble(), 
                            _ui->atWidthTE->text().toDouble(), _project, operation == Operation::WidthLE, operation == Operation::WidthTE);
                        auto widthLE = result[0];
                        auto widthTE = result[1];

                        if(operation == Operation::WidthLE) {
                            paramsResult.insert(operation, widthLE);
                        }
                        if(operation == Operation::WidthTE) {
                            paramsResult.insert(operation, widthTE);
                        }
                        break;
                    }
                    case Operation::RadiusLE:
                    case Operation::RadiusTE:
                    {
                        auto result = Algorithms::getRadiusOfEdges(curveName, new Function18Params(), _project);
                        auto radiusLE = result[0];
                        auto radiusTE = result[1];

                        if(operation == Operation::RadiusLE) {
                            paramsResult.insert(operation, radiusLE);
                        }
                        if(operation == Operation::RadiusTE) {
                            paramsResult.insert(operation, radiusTE);
                        }
                        break;
                    }
                }
            } catch(...) {
            }
        }
    }
    return paramsResult;
}

void TurbineWindow::setNominalParams(const QMap<Operation, double>& params) {
    auto precision = _project->precision();

    for(auto [operation, value] : params.asKeyValueRange()) {
        switch(operation) {
            case Operation::MaxWidth:
                _ui->nominalMaxWidth->setText(QString::number(value, 'f', precision));
                _reportSettings->setNomMaxWidth(value);
                break;
            case Operation::MaxWidthX:
                _ui->nominalMaxWidthX->setText(QString::number(value, 'f', precision));
                _reportSettings->setNomXMaxWidth(value);
                break;
            case Operation::MaxWidthY:
                _ui->nominalMaxWidthY->setText(QString::number(value, 'f', precision));
                _reportSettings->setNomYMaxWidth(value);
                break;
            case Operation::ContactLineLength:
                _ui->nominalChordLength->setText(QString::number(value, 'f', precision));
                _reportSettings->setNomContactLineLength(value);
                break;
            case Operation::WidthLE:
                _ui->nominalWidthLE->setText(QString::number(value, 'f', precision));
                _reportSettings->setNomLEWidth(value);
                break;
            case Operation::WidthTE:
                _ui->nominalWidthTE->setText(QString::number(value, 'f', precision));
                _reportSettings->setNomTEWidth(value);
                break;
            case Operation::RadiusLE:
                _ui->nominalRadiusLE->setText(QString::number(value, 'f', precision));
                _reportSettings->setNomLERadius(value);
                break;
            case Operation::RadiusTE:
                _ui->nominalRadiusTE->setText(QString::number(value, 'f', precision));
                _reportSettings->setNomTERadius(value);
                break;
        }
    }
}

void TurbineWindow::setMeasuredParams(const QMap<Operation, double>& params) {
    for(auto [operation, value] : params.asKeyValueRange()) {
        switch(operation) {
            case Operation::MaxWidth:
                _reportSettings->setMeasMaxWidth(value);
                break;
            case Operation::MaxWidthX:
                _reportSettings->setMeasXMaxWidth(value);
                break;
            case Operation::MaxWidthY:
                _reportSettings->setMeasYMaxWidth(value);
                break;
            case Operation::ContactLineLength:
                _reportSettings->setMeasContactLineLength(value);
                break;
            case Operation::WidthLE:
                _reportSettings->setMeasLEWidth(value);
                break;
            case Operation::WidthTE:
                _reportSettings->setMeasTEWidth(value);
                break;
            case Operation::RadiusLE:
                _reportSettings->setMeasLERadius(value);
                break;
            case Operation::RadiusTE:
                _reportSettings->setMeasTERadius(value);
                break;
        }
    }
}

void TurbineWindow::setSettings(const QString &filePath) {
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
        _reportSettings->setRadiusCompensation(_ui->upperValue->text().toDouble());
    }

    _reportSettings->setProfileType(static_cast<ReportSettings::Profile>(_ui->profileType->currentIndex()));
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
    _reportSettings->setNeedAdditionalFigures(_ui->needMaxDiam->isChecked(), _ui->needMCL->isChecked(), _ui->needContactLine->isChecked());
    _reportSettings->setVisibilityEdges(_ui->needFormLE->isChecked(), _ui->needFormTE->isChecked());
    _reportSettings->setTypeOfShowDevs(
        static_cast<ReportSettings::TypeOfShowDevs>(_ui->showNumDevLE->currentIndex()),
        static_cast<ReportSettings::TypeOfShowDevs>(_ui->showNumDevTE->currentIndex()));
    _reportSettings->setValueOfSetShowDevs(_ui->valueOfSetLE->text().toInt(), _ui->valueOfSetTE->text().toInt());

    _reportSettings->setTypesOfTableValue(QMap<DimFigure::ValueType, bool>{
        { static_cast<DimFigure::ValueType>(18), _ui->minMax->isChecked() },
        { static_cast<DimFigure::ValueType>(19), _ui->form->isChecked() },
        { static_cast<DimFigure::ValueType>(20), _ui->min->isChecked() },
        { static_cast<DimFigure::ValueType>(21), _ui->max->isChecked() },
        { static_cast<DimFigure::ValueType>(22), _ui->maxAbs->isChecked() },
        { static_cast<DimFigure::ValueType>(23), _ui->supUT->isChecked() },
        { static_cast<DimFigure::ValueType>(24), _ui->infLT->isChecked() },
    });
    auto evaluationPlace = _ui->markPlaceNominal->isChecked() ? ReportSettings::Evaluation::Nominal : ReportSettings::Evaluation::Measured;
    auto evaluationDirection = _ui->markDirectionNominal->isChecked() ? ReportSettings::Evaluation::Nominal : ReportSettings::Evaluation::Measured;
    _reportSettings->setEvaluation(evaluationPlace, evaluationDirection);


    for(auto i = 0; i < _ui->paramList->count(); i++) {
        auto item = _ui->paramList->item(i);
        if(item->checkState() == Qt::Checked) {
            auto operation = static_cast<Operation>(item->data(Qt::UserRole).toInt());
            setTolerances(operation);
        }
    }

    if(_ui->needPrintTemplate->isChecked()) {
        _reportSettings->setNeedPrintWithTemplate(true);
        _reportSettings->setReportTemplate(static_cast<ReportSettings::Template>(_ui->reportList->currentRow()));
        _reportSettings->setComment(_ui->comment->text());
        _reportSettings->setReportPath(filePath);
    }
}

void TurbineWindow::setTolerances(Operation operation) {
    switch(operation) {
        case Operation::MaxWidth:
        {
            auto upperTolerance = _ui->upperTolMaxWidth->text().toDouble();
            auto lowerTolerance = _ui->lowerTolMaxWidth->text().toDouble();
            _reportSettings->setMaxWidthTolerances(upperTolerance, lowerTolerance);
            break;
        }
        case Operation::MaxWidthX:
        {
            auto upperTolerance = _ui->upperTolMaxWidthX->text().toDouble();
            auto lowerTolerance = _ui->lowerTolMaxWidthX->text().toDouble();
            _reportSettings->setXMaxWidthTolerances(upperTolerance, lowerTolerance);
            break;
        }
        case Operation::MaxWidthY:
        {
            auto upperTolerance = _ui->upperTolMaxWidthY->text().toDouble();
            auto lowerTolerance = _ui->lowerTolMaxWidthY->text().toDouble();
            _reportSettings->setYMaxWidthTolerances(upperTolerance, lowerTolerance);
            break;
        }
        case Operation::ContactLineLength:
        {
            auto upperTolerance = _ui->upperTolChordLength->text().toDouble();
            auto lowerTolerance = _ui->lowerTolChordLength->text().toDouble();
            _reportSettings->setContactLineLengthTolerances(upperTolerance, lowerTolerance);
            break;
        }
        case Operation::WidthLE:
        {
            auto upperTolerance = _ui->upperTolWidthLE->text().toDouble();
            auto lowerTolerance = _ui->lowerTolWidthLE->text().toDouble();
            _reportSettings->setLEWidthTolerances(upperTolerance, lowerTolerance);
            break;
        }
        case Operation::WidthTE:
        {
            auto upperTolerance = _ui->upperTolWidthTE->text().toDouble();
            auto lowerTolerance = _ui->lowerTolWidthTE->text().toDouble();
            _reportSettings->setTEWidthTolerances(upperTolerance, lowerTolerance);
            break;
        }
        case Operation::RadiusLE:
        {
            auto upperTolerance = _ui->upperTolRadiusLE->text().toDouble();
            auto lowerTolerance = _ui->lowerTolRadiusLE->text().toDouble();
            _reportSettings->setLERadiusTolerances(upperTolerance, lowerTolerance);
            break;
        }
        case Operation::RadiusTE:
        {
            auto upperTolerance = _ui->upperTolRadiusTE->text().toDouble();
            auto lowerTolerance = _ui->lowerTolRadiusTE->text().toDouble();
            _reportSettings->setTERadiusTolerances(upperTolerance, lowerTolerance);
            break;
        }
    }
}

void TurbineWindow::updateParamOutputView() {
    auto currentParam = _ui->paramList->currentItem();

    if(currentParam) {
        auto currentIndex = _ui->paramList->currentItem()->data(Qt::UserRole).toInt();
        _ui->paramsTab->setCurrentIndex(currentIndex);
    }
}

void TurbineWindow::changeItemOfList() {
    auto selectedItemsOfListNominal = _ui->listNominal->selectedItems();
    auto selectedItemsOfListMeasured = _ui->listMeasured->selectedItems();
    auto currentNominalItem = selectedItemsOfListNominal.length() == 1 ? selectedItemsOfListNominal[0] : nullptr;
    auto currentMeasuredItem = selectedItemsOfListMeasured.length() == 1 ? selectedItemsOfListMeasured[0] : nullptr;

    if(currentNominalItem && !currentMeasuredItem) {
        auto nominalFigure = _project->findFigure(currentNominalItem->text());
        auto nominalCurve = dynamic_cast<const CurveFigure*>(nominalFigure);
        assert(nominalCurve);
        _curveGraphics->drawCurve(nominalCurve, Qt::green, 0.1);
    } else if(!currentNominalItem && currentMeasuredItem) {
        auto measuredFigure = _project->findFigure(currentMeasuredItem->text());
        auto measuredCurve = dynamic_cast<const CurveFigure*>(measuredFigure);
        assert(measuredCurve);
        _curveGraphics->drawCurve(measuredCurve, Qt::blue, 0.1);
    } else if(currentNominalItem && currentMeasuredItem) {
        auto nominalFigure = _project->findFigure(currentNominalItem->text());
        auto nominalCurve = dynamic_cast<const CurveFigure*>(nominalFigure);
        auto measuredFigure = _project->findFigure(currentMeasuredItem->text());
        auto measuredCurve = dynamic_cast<const CurveFigure*>(measuredFigure);
        assert(nominalCurve && measuredCurve);
        _curveGraphics->drawCurve(nominalCurve, Qt::green, 0.1, measuredCurve, Qt::blue, 0.1);
    }
}

void TurbineWindow::onNoBestFitLEClick() {
    auto edgeBestFit = static_cast<ReportSettings::EdgeBestFit>(_ui->bestFitLE->currentIndex());
    if(edgeBestFit == ReportSettings::EdgeBestFit::NoFit) {
        _ui->toliranceLE->show();
    } else {
        _ui->toliranceLE->hide();
    }
}

void TurbineWindow::onNoBestFitTEClick() {
    auto edgeBestFit = static_cast<ReportSettings::EdgeBestFit>(_ui->bestFitTE->currentIndex());
    if(edgeBestFit == ReportSettings::EdgeBestFit::NoFit) {
        _ui->toliranceTE->show();
    } else {
        _ui->toliranceTE->hide();
    }
}

void TurbineWindow::onShowNumDevLEClick() {
    auto typeOfShowDevs = static_cast<ReportSettings::TypeOfShowDevs>(_ui->showNumDevLE->currentIndex());
    if(typeOfShowDevs == ReportSettings::TypeOfShowDevs::Set) {
        _ui->valueOfSetLE->show();
    } else {
        _ui->valueOfSetLE->hide();
    }
}

void TurbineWindow::onShowNumDevTEClick() {
    auto typeOfShowDevs = static_cast<ReportSettings::TypeOfShowDevs>(_ui->showNumDevTE->currentIndex());
    if(typeOfShowDevs == ReportSettings::TypeOfShowDevs::Set) {
        _ui->valueOfSetTE->show();
    } else {
        _ui->valueOfSetTE->hide();
    }
}

void TurbineWindow::onPreparePointsClick() {
    auto isChecked = _ui->needPreparePoints->isChecked();
    if(isChecked) {
        _ui->needSort->show();
        _ui->needRemoveSimilar->show();
        _ui->needRadiusCorrection->show();
        _ui->needUse3DVectors->show();
    } else {
        _ui->needSort->hide();
        _ui->needRemoveSimilar->hide();
        _ui->needRadiusCorrection->hide();
        _ui->needUse3DVectors->hide();
    }
}

void TurbineWindow::onDeleteSimilarClick() {
    auto isChecked = _ui->needRemoveSimilar->isChecked();
    if(isChecked) {
        _ui->limitForEqualPoints->show();
    } else {
        _ui->limitForEqualPoints->hide();
    }
}

void TurbineWindow::onRadiusCorrectionClick() {
    auto isChecked = _ui->needRadiusCorrection->isChecked();
    if(isChecked) {
        _ui->upperValue->show();
        _ui->lowerValue->show();
    } else {
        _ui->upperValue->hide();
        _ui->lowerValue->hide();
    }
}

void TurbineWindow::onUpperValueChange() {
    auto upperValue = _ui->upperValue->text();
    _ui->lowerValue->setText(upperValue);
}

void TurbineWindow::onNeedPrintTemplateClick() {
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

void TurbineWindow::closeEvent(QCloseEvent * event) {
    closeWindow();
}

void TurbineWindow::closeWindow() {
    close();
    _ui->listMeasured->clear();
    _ui->listNominal->clear();
    _reportSettings->clear();

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
    _ui->profileType->setCurrentIndex(0);
    _ui->globalBestFit->setCurrentIndex(0);
    _ui->bestFitType->setCurrentIndex(0);
    _ui->averageDeviation->setChecked(false);
    _ui->stretchLE->setChecked(false);
    _ui->stretchTE->setChecked(false);
    _ui->globalApmlification->setText("5");
    _ui->globalZoom->setText("0");
    _ui->globalAxis->setCurrentIndex(0);
    _ui->needMCL->setChecked(false);
    _ui->needMaxDiam->setChecked(false);
    _ui->needContactLine->setChecked(false);

    _ui->needFormLE->setChecked(true);
    _ui->bestFitLE->setCurrentIndex(0);
    _ui->showNumDevLE->setCurrentIndex(0);
    _ui->amplificationLE->setText("1");
    _ui->zoomLE->setText("0");
    _ui->axisLE->setCurrentIndex(0);

    _ui->needFormTE->setChecked(true);
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

TurbineWindow::~TurbineWindow() {
    delete _ui;
}
