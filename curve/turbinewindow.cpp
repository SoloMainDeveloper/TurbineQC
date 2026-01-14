#include "curve/pch.h"
#include "turbinewindow.h"

TurbineParams::TurbineParams() {
    nominal = 0;
    toleranceTop = 0;
    toleranceBottom = 0;
    atWidth = 1;
}

TurbineParams::~TurbineParams() {
}

TurbineWindow::TurbineWindow(Project *project, Plot *plot) : _ui(new Ui::TurbineWindow()) {
    _ui->setupUi(this);
    _project = project;
    _plot = plot;
    _reportData = new ReportData();
    _curveGraphics = new CurveGraphicsWidget();
    _containerLayout = new QGridLayout(_ui->container);
    _containerLayout->setContentsMargins(0, 0, 0, 0);
    _containerLayout->addWidget(_curveGraphics, 0, 0);
    _message = new QMessageBox(this);
    _message->setWindowTitle("Error");
    this->setWindowTitle("Calculate turbine");
    setupWindow();
    connect(_ui->closeButton, &QPushButton::clicked, this, &TurbineWindow::closeWindow);
    connect(_ui->calculateButton, &QPushButton::clicked, this, &TurbineWindow::calculateNominals);
    connect(_ui->runButton, &QPushButton::clicked, this, &TurbineWindow::run);
    connect(_ui->preparePointsCheckBox, &QCheckBox::clicked, this, &TurbineWindow::onPreparePointsClick);
    connect(_ui->deleteSimilarCheckBox, &QCheckBox::clicked, this, &TurbineWindow::onDeleteSimilarClick);
    connect(_ui->radiusCorrectionCheckBox, &QCheckBox::clicked, this, &TurbineWindow::onRadiusCorrectionClick);
    connect(showNumLE, &QComboBox::currentIndexChanged, this, &TurbineWindow::onShowNumLEClick);
    connect(showNumTE, &QComboBox::currentIndexChanged, this, &TurbineWindow::onShowNumTEClick);
    connect(nominals, &QListWidget::currentItemChanged, this, &TurbineWindow::updateParamOutputView);
    connect(paramList, &QListWidget::currentItemChanged, this, &TurbineWindow::updateParamOutputView);
    connect(paramList, &QListWidget::itemChanged, this, &TurbineWindow::showTurbineParams);
    connect(nominals, &QListWidget::itemClicked, this, &TurbineWindow::changeItemOfList);
    connect(measured, &QListWidget::itemClicked, this, &TurbineWindow::changeItemOfList);
    connect(writeWithTemplate, &QCheckBox::clicked, this, &TurbineWindow::onWriteWithTemplateClick);
}

void TurbineWindow::initialization() {
    measured->clear();
    nominals->clear();
    _reportData->clear();

    _ui->bestFitComboBox->setDisabled(true);
    _ui->increaseLineEdit->setDisabled(true);
    _ui->avgDevCheckBox->setDisabled(true);
    _ui->radiusCorrectionCheckBox->setDisabled(true);
    _ui->use3DVectorsCheckBox->setDisabled(true);

    auto model1 = dynamic_cast<QStandardItemModel*>(_ui->axisComboBox->model());
    auto item1 = model1->item(1);
    item1->setEnabled(false);

    auto model2 = dynamic_cast<QStandardItemModel*>(_ui->axisComboBox_2->model());
    auto item2 = model2->item(1);
    item2->setEnabled(false);

    auto model3 = dynamic_cast<QStandardItemModel*>(_ui->axisComboBox_3->model());
    auto item3 = model3->item(1);
    item3->setEnabled(false);


    _ui->limitForEqualPoints->hide();
    _ui->upperValue->hide();
    _ui->lowerValue->hide();
    _ui->valueOfSetLE->hide();
    _ui->valueOfSetTE->hide();

    _ui->reportList->setCurrentRow(0);
    _ui->commentLabel->show();
    _ui->commentLineEdit->show();
    _ui->reportList->show();

    auto figures = _project->figures();
    _curveGraphics->initialization();

    for(auto item : figures) {
        if(dynamic_cast<CurveFigure*>(item)) {
            auto name = item->name();
            measured->addItem(new QListWidgetItem(name));
            nominals->addItem(new QListWidgetItem(name));
        }
    }


    if(paramList->item(0)->data(Qt::UserRole).isNull()) {
        for(auto i = 0; i < paramList->count(); i++) {
            paramList->item(i)->setData(Qt::UserRole, i);
        }
    }
    paramList->setCurrentItem(nullptr);

    onPreparePointsClick();
    exec();
}

void TurbineWindow::closeWindow() {    //set default for everything
    close();
    measured->setCurrentItem(nullptr);
    nominals->setCurrentItem(nullptr);
    directionLE->setCurrentIndex(0);
    zoneScaleLE->setText("5");
    zoneScaleTE->setText("5");
    dimension->setCurrentIndex(0);

    if(needPreparePoints->isChecked()) {
        needPreparePoints->click();
    }
    _ui->needSortCheckBox->setChecked(false);
    _ui->deleteSimilarCheckBox->setChecked(false);
    _ui->radiusCorrectionCheckBox->setChecked(false);
    _ui->use3DVectorsCheckBox->setChecked(false);

    formTab->setCurrentIndex(0);
    profile->setCurrentIndex(0);
    globalBestFit->setCurrentIndex(0);
    globalBestFitType->setCurrentIndex(0);
    showMCL->setChecked(false);
    showMaxDiam->setChecked(false);
    showContactLines->setChecked(false);
    avgDeviation->setChecked(false);
    stretchLE->setChecked(false);
    stretchTE->setChecked(false);
    globalScale->setText("5");
    globalIncrease->setText("0");
    globalAxis->setCurrentIndex(0);

    formLE->setChecked(true);
    bestFitLE->setCurrentIndex(0);
    showNumLE->setCurrentIndex(0);
    scaleLE->setText("1");
    increaseLE->setText("0");
    axisLE->setCurrentIndex(0);

    formTE->setChecked(true);
    bestFitTE->setCurrentIndex(0);
    showNumTE->setCurrentIndex(0);
    scaleTE->setText("1");
    increaseTE->setText("0");
    axisTE->setCurrentIndex(0);

    minMax->setChecked(true);
    form->setChecked(false);
    min->setChecked(true);
    max->setChecked(true);
    maxAbs->setChecked(false);
    supUT->setChecked(true);
    infLT->setChecked(true);
    markPlaceNominal->setChecked(true);
    markDirectionNominal->setChecked(true);

    peakAnalysis->setCurrentIndex(0);

    paramList->blockSignals(true);
    for(auto i = 0; i < paramList->count(); i++) {
        paramList->item(i)->setCheckState(Qt::Unchecked);
    }
    paramList->blockSignals(false);


    reportTab->setCurrentIndex(0);
    writeWithTemplate->setChecked(true);
    reportComment->clear();
    allProfilesToReport->clear();
}

void TurbineWindow::setupWindow() {
    //validator
    _doubleValidator = new QDoubleValidator();
    _doubleValidator->setDecimals(9);
    _doubleValidator->setLocale(QLocale::C);
    _doubleValidator->setNotation(QDoubleValidator::StandardNotation);

    _intValidator = new QIntValidator();
    _intValidator->setLocale(QLocale::C);
    _intValidator->setRange(0, 1000);

    //profiles
    nominals = _ui->listNominal;
    measured = _ui->listMeasured;
    directionLE = _ui->directionLEComboBox;
    zoneScaleLE = _ui->scaleLE;
    zoneScaleLE->setValidator(_doubleValidator);
    zoneScaleTE = _ui->scaleTE;
    zoneScaleTE->setValidator(_doubleValidator);
    dimension = _ui->measureTypeComboBox;

    //prepare points
    needPreparePoints = _ui->preparePointsCheckBox;
    _ui->limitForEqualPoints->setValidator(_doubleValidator);
    _ui->upperValue->setValidator(_doubleValidator);
    _ui->lowerValue->setValidator(_doubleValidator);

    formTab = _ui->baseParamTabWidget;
    //global form
    profile = _ui->profileTypeComboBox;
    globalBestFit = _ui->bestFitComboBox;
    globalBestFitType = _ui->typeBFComboBox;
    showMCL = _ui->mclCheckBox;
    showMaxDiam = _ui->maxDiamCheckBox;
    showContactLines = _ui->contactCheckBox;
    avgDeviation = _ui->avgDevCheckBox;
    stretchLE = _ui->stretchLECheckBox;
    stretchTE = _ui->stretchTECheckBox;
    globalScale = _ui->scaleLineEdit;
    globalScale->setValidator(_doubleValidator);
    globalIncrease = _ui->increaseLineEdit;
    globalIncrease->setValidator(_doubleValidator);
    globalAxis = _ui->axisComboBox;

    //LE form
    formLE = _ui->formLECheckBox;
    bestFitLE = _ui->bestFitComboBox_2;
    showNumLE = _ui->showComboBox_2;
    scaleLE = _ui->scaleLineEdit_2;
    scaleLE->setValidator(_doubleValidator);
    increaseLE = _ui->increaseLineEdit_2;
    increaseLE->setValidator(_doubleValidator);
    axisLE = _ui->axisComboBox_2;
    _ui->valueOfSetLE->setValidator(_intValidator);

    //TE form
    formTE = _ui->formTECheckBox;
    bestFitTE = _ui->bestFitComboBox_3;
    showNumTE = _ui->showComboBox_3;
    scaleTE = _ui->scaleLineEdit_3;
    scaleTE->setValidator(_doubleValidator);
    increaseTE = _ui->increaseLineEdit_3;
    increaseTE->setValidator(_doubleValidator);
    axisTE = _ui->axisComboBox_3;
    _ui->valueOfSetTE->setValidator(_intValidator);

    //output form
    minMax = _ui->minMaxCheckBox;
    form = _ui->formCheckBox;
    min = _ui->minCheckBox;
    max = _ui->maxCheckBox;
    maxAbs = _ui->maxAbsCheckBox;
    supUT = _ui->supUTCheckBox;
    infLT = _ui->infLTCheckBox;
    markPlaceNominal = _ui->markPlaceNominal;
    markPlaceMeasured = _ui->markPlaceMeasured;
    markDirectionNominal = _ui->markDirectionNominal;
    markDirectionMeasured = _ui->markDirectionMeasured;

    //peak analysis
    peakAnalysis = _ui->peakAnalysisComboBox;

    //parameters
    paramList = _ui->paramList;
    labelLayout = _ui->vLayoutLabels;
    lineEditLayout = _ui->vLayoutLineEdits;

    //report
    reportTab = _ui->reportTabWidget;
    writeWithTemplate = _ui->printTemplateCheckBox;
    reportComment = _ui->commentLineEdit;
    allProfilesToReport = _ui->profileNames;
}

void TurbineWindow::calculateNominals() {
    auto selectedNominalItems = nominals->selectedItems();
    auto currentNominal = nominals->currentItem();
    if(currentNominal == nullptr) {
        _message->setText("Please select the nominal curve!");
        _message->exec();
        return;
    }

    auto nomFigureName = nominals->currentItem()->text();
    auto scaleLEValue = zoneScaleLE->text().toDouble();
    auto scaleTEValue = zoneScaleTE->text().toDouble();

    for(auto i = 0; i < paramList->count(); i++) {
        auto item = paramList->item(i);
        if(item->checkState() == Qt::Checked) {
            auto operation = static_cast<Operation>(item->data(Qt::UserRole).toInt());
            switch(operation) {
                case Operation::MaxWidth:
                case Operation::MaxWidthX:
                case Operation::MaxWidthY:
                {
                    Algorithms::getMaxWidthCircle(nomFigureName, new Function18Params(/*0, scaleLEValue, scaleTEValue*/), _project);
                    auto nomMaxCircle = static_cast<const CircleFigure*>(_project->findFigure(nomFigureName + "_MaxDia"));
                    auto nomDiameter = nomMaxCircle->radius() * 2;
                    auto nomXCenter = nomMaxCircle->center().x;
                    auto nomYCenter = nomMaxCircle->center().y;

                    if(operation == Operation::MaxWidth) {
                        nominalTurbineParams[nomFigureName][Operation::MaxWidth]->nominal = nomDiameter;
                        _reportData->setNomMaxWidth(nomDiameter);
                    }
                    if(operation == Operation::MaxWidthX) {
                        nominalTurbineParams[nomFigureName][Operation::MaxWidthX]->nominal = nomXCenter;
                        _reportData->setNomXMaxWidth(nomXCenter);
                    }
                    if(operation == Operation::MaxWidthY) {
                        nominalTurbineParams[nomFigureName][Operation::MaxWidthY]->nominal = nomYCenter;
                        _reportData->setNomYMaxWidth(nomYCenter);
                    }
                    break;
                }
                case Operation::ChordLength:
                {
                    auto nomLength = Algorithms::getChordLength(nomFigureName, new Function18Params(/*0, scaleLEValue, scaleTEValue*/), _project);
                    nominalTurbineParams[nomFigureName][operation]->nominal = nomLength;
                    _reportData->setNomChordLength(nomLength);
                    break;
                }
                case Operation::WidthLE:
                case Operation::WidthTE:
                {
                    auto nomResult = Algorithms::getWidthOfEdges(nomFigureName, nominalTurbineParams[nomFigureName][Operation::WidthLE]->atWidth,
                        nominalTurbineParams[nomFigureName][Operation::WidthTE]->atWidth, _project, operation == Operation::WidthLE, operation == Operation::WidthTE);
                    auto nomLEWidth = nomResult[0];
                    auto nomTEWidth = nomResult[1];

                    if(operation == Operation::WidthLE) {
                        nominalTurbineParams[nomFigureName][Operation::WidthLE]->nominal = nomLEWidth;
                        _reportData->setNomLEWidth(nomLEWidth);
                    }
                    if(operation == Operation::WidthTE) {
                        nominalTurbineParams[nomFigureName][Operation::WidthTE]->nominal = nomTEWidth;
                        _reportData->setNomTEWidth(nomTEWidth);
                    }
                    break;
                }
                case Operation::RadiusLE:
                case Operation::RadiusTE:
                {
                    auto nomResult = Algorithms::getRadiusOfEdges(nomFigureName, new Function18Params(0, scaleLEValue, scaleTEValue), _project);
                    auto nomLERadius = nomResult[0];
                    auto nomTERadius = nomResult[1];

                    if(operation == Operation::RadiusLE) {
                        nominalTurbineParams[nomFigureName][Operation::RadiusTE]->nominal = nomLERadius;
                        _reportData->setNomLERadius(nomLERadius);
                    }
                    if(operation == Operation::RadiusTE) {
                        nominalTurbineParams[nomFigureName][Operation::RadiusLE]->nominal = nomTERadius;
                        _reportData->setNomTERadius(nomTERadius);
                    }
                    break;
                }
            }
        }
    }
    updateParamOutputView();
}

void TurbineWindow::calculateMeasured() {
    auto selectedNominalItems = nominals->selectedItems();
    auto currentNominal = nominals->currentItem();
    auto selectedMeasuredItems = measured->selectedItems();
    auto currentMeasured = measured->currentItem();

    if(currentNominal != nullptr && currentMeasured != nullptr) {
        auto nomFigureName = nominals->currentItem()->text();
        auto measFigureName = measured->currentItem()->text();

        for(auto i = 0; i < paramList->count(); i++) {
            auto item = paramList->item(i);
            if(item->checkState() == Qt::Checked) {
                auto operation = static_cast<Operation>(item->data(Qt::UserRole).toInt());
                switch(operation) {
                    case Operation::MaxWidth:
                    case Operation::MaxWidthX:
                    case Operation::MaxWidthY:
                    {
                        auto measFigureName = measured->currentItem()->text();
                        Algorithms::getMaxWidthCircle(measFigureName, new Function18Params(), _project);

                        auto measMaxCircle = static_cast<const CircleFigure*>(_project->findFigure(measFigureName + "_MaxDia"));
                        auto measDiameter = measMaxCircle->radius() * 2;
                        auto measXCenter = measMaxCircle->center().x;
                        auto measYCenter = measMaxCircle->center().y;

                        if(operation == Operation::MaxWidth) {
                            _reportData->setMeasMaxWidth(measDiameter);
                        }
                        if(operation == Operation::MaxWidthX) {
                            _reportData->setMeasXMaxWidth(measXCenter);
                        }
                        if(operation == Operation::MaxWidthY) {
                            _reportData->setMeasYMaxWidth(measYCenter);
                        }
                        break;
                    }
                    case Operation::ChordLength:
                    {
                        auto measFigureName = measured->currentItem()->text();
                        auto measLength = Algorithms::getChordLength(measFigureName, new Function18Params(), _project);
                        _reportData->setMeasChordLength(measLength);
                        break;
                    }
                    case Operation::WidthLE:
                    case Operation::WidthTE:
                    {
                        auto measResult = Algorithms::getWidthOfEdges(measFigureName, nominalTurbineParams[nomFigureName][Operation::WidthLE]->atWidth,
                            nominalTurbineParams[nomFigureName][Operation::WidthTE]->atWidth, _project, operation == Operation::WidthLE, operation == Operation::WidthTE);
                        auto measLEWidth = measResult[0];
                        auto measTEWidth = measResult[1];

                        if(operation == Operation::WidthLE) {
                            _reportData->setMeasLEWidth(measLEWidth);
                        }
                        if(operation == Operation::WidthTE) {
                            _reportData->setMeasTEWidth(measTEWidth);
                        }
                        break;
                    }
                    case Operation::RadiusLE:
                    case Operation::RadiusTE:
                    {
                        auto measResult = Algorithms::getRadiusOfEdges(measFigureName, new Function18Params(), _project);
                        auto measLERadius = measResult[0];
                        auto measTERadius = measResult[1];

                        if(operation == Operation::RadiusLE) {
                            _reportData->setMeasLERadius(measLERadius);
                        }
                        if(operation == Operation::RadiusTE) {
                            _reportData->setMeasTERadius(measTERadius);
                        }
                        break;
                    }
                }
            }
        }
    }
}

void TurbineWindow::run() {
    auto selectedNominalItems = nominals->selectedItems();
    auto currentNominal = nominals->currentItem();
    auto selectedMeasuredItems = measured->selectedItems();
    auto currentMeasured = measured->currentItem();

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
    if(writeWithTemplate->isChecked()) {
        filePath = QFileDialog::getSaveFileName(nullptr, "Save report", "", "(*.html)");
    }
    if(writeWithTemplate->isChecked() && filePath != nullptr || !writeWithTemplate->isChecked()) {
        calculateMeasured();

        auto nominalFigure = _project->findFigure(currentNominal->text());
        auto nominalCurve = dynamic_cast<const CurveFigure*>(nominalFigure);
        auto measuredFigure = _project->findFigure(currentMeasured->text());
        auto measuredCurve = dynamic_cast<const CurveFigure*>(measuredFigure);

        assert(nominalCurve && measuredCurve);

        QString fileName = QFileInfo(filePath).fileName();
        setSettings(fileName);

        auto reportGenerator = new ReportGenerator(_project, _plot, _reportData, filePath);
        reportGenerator->createReport(nominalCurve, measuredCurve);

        delete reportGenerator;
    }
}

void TurbineWindow::setSettings(const QString &fileName) {
    _reportData->setLEDirection(static_cast<ReportData::LEDirection>(directionLE->currentIndex()));
    _reportData->setZone(zoneScaleLE->text().toInt(), zoneScaleTE->text().toInt(),
        static_cast<ReportData::Dimension>(dimension->currentIndex()));

    _reportData->setPreProcessSettings(_ui->needSortCheckBox->isChecked(), _ui->deleteSimilarCheckBox->isChecked(),
        _ui->radiusCorrectionCheckBox->isChecked(), _ui->use3DVectorsCheckBox->isChecked());
    if(_ui->deleteSimilarCheckBox->isChecked()) {
        _reportData->setLimitForEqualPoints(_ui->limitForEqualPoints->text().toDouble());
    }

    _reportData->setProfileType(static_cast<ReportData::Profile>(profile->currentIndex()));
    _reportData->setBestFitType(static_cast<ReportData::BestFitType>(globalBestFitType->currentIndex()));
    _reportData->setStretch(stretchLE->isChecked(), stretchTE->isChecked());
    _reportData->setAmplification(globalScale->text().toDouble(), scaleLE->text().toDouble(), scaleTE->text().toDouble());
    _reportData->setAxisDisplayType(
        static_cast<ReportData::Axis>(globalAxis->currentIndex()),
        static_cast<ReportData::Axis>(axisLE->currentIndex()),
        static_cast<ReportData::Axis>(axisTE->currentIndex()));
    _reportData->setNeedFigures(showMaxDiam->isChecked(), showMCL->isChecked(), showContactLines->isChecked());
    _reportData->setVisibilityEdges(formLE->isChecked(), formTE->isChecked());
    _reportData->setTypeOfShowDevs(
        static_cast<ReportData::TypeOfShowDevs>(showNumLE->currentIndex()),
        static_cast<ReportData::TypeOfShowDevs>(showNumTE->currentIndex()));
    _reportData->setValueOfSetShowDevs(_ui->valueOfSetLE->text().toInt(), _ui->valueOfSetTE->text().toInt());

    _reportData->setTypesOfTableValue(QMap<ReportData::TypeOfTableValue, bool>{
        { static_cast<ReportData::TypeOfTableValue>(0), minMax->isChecked() },
        { static_cast<ReportData::TypeOfTableValue>(1), form->isChecked() },
        { static_cast<ReportData::TypeOfTableValue>(2), min->isChecked() },
        { static_cast<ReportData::TypeOfTableValue>(3), max->isChecked() },
        { static_cast<ReportData::TypeOfTableValue>(4), maxAbs->isChecked() },
        { static_cast<ReportData::TypeOfTableValue>(5), supUT->isChecked() },
        { static_cast<ReportData::TypeOfTableValue>(6), infLT->isChecked() },
    });
    auto evaluationPlace = _ui->markPlaceNominal->isChecked() ? ReportData::Evaluation::Nominal : ReportData::Evaluation::Measured;
    auto evaluationDirection = _ui->markDirectionNominal->isChecked() ? ReportData::Evaluation::Nominal : ReportData::Evaluation::Measured;
    _reportData->setEvaluation(evaluationPlace, evaluationDirection);

    if(writeWithTemplate->isChecked()) {
        _reportData->setNeedPrintWithTemplate(true);
        _reportData->setReportTemplate(static_cast<ReportData::Template>(_ui->reportList->currentRow()));
        _reportData->setComment(reportComment->text());
        _reportData->setReportName(fileName);
    }
}

void TurbineWindow::onPreparePointsClick() {
    auto isChecked = _ui->preparePointsCheckBox->isChecked();
    if(isChecked) {
        _ui->needSortCheckBox->show();
        _ui->deleteSimilarCheckBox->show();
        _ui->radiusCorrectionCheckBox->show();
        _ui->use3DVectorsCheckBox->show();
    } else {
        _ui->needSortCheckBox->hide();
        _ui->deleteSimilarCheckBox->hide();
        _ui->radiusCorrectionCheckBox->hide();
        _ui->use3DVectorsCheckBox->hide();
    }
}

void TurbineWindow::onDeleteSimilarClick() {
    auto isChecked = _ui->deleteSimilarCheckBox->isChecked();
    if(isChecked) {
        _ui->limitForEqualPoints->show();
    } else {
        _ui->limitForEqualPoints->hide();
    }
}

void TurbineWindow::onRadiusCorrectionClick() {
    auto isChecked = _ui->radiusCorrectionCheckBox->isChecked();
    if(isChecked) {
        _ui->upperValue->show();
        _ui->lowerValue->show();
    } else {
        _ui->upperValue->hide();
        _ui->lowerValue->hide();
    }
}

void TurbineWindow::onShowNumLEClick() {
    if(showNumLE->currentText() == "Set") {
        _ui->valueOfSetLE->show();
    } else {
        _ui->valueOfSetLE->hide();
    }
}

void TurbineWindow::onShowNumTEClick() {
    if(showNumTE->currentText() == "Set") {
        _ui->valueOfSetTE->show();
    } else {
        _ui->valueOfSetTE->hide();
    }
}

void TurbineWindow::onWriteWithTemplateClick() {
    if(writeWithTemplate->isChecked()) {
        _ui->commentLabel->show();
        _ui->commentLineEdit->show();
        _ui->reportList->show();
    } else {
        _ui->commentLabel->hide();
        _ui->commentLineEdit->hide();
        _ui->reportList->hide();
    }

}

void TurbineWindow::showTurbineParams(QListWidgetItem * currParam) {
    paramList->blockSignals(true);
    paramList->setCurrentItem(currParam); //check also changes currentItem
    paramList->blockSignals(false);
    updateParamOutputView();
}

void TurbineWindow::clearLayout(QLayout * layout) {
    while(QLayoutItem* item = layout->takeAt(0)) {
        delete item->widget();
        delete item;
    }
}

void TurbineWindow::updateParamOutputView() {
    auto currentFigure = nominals->currentItem();
    auto currentParam = paramList->currentItem();
    clearLayout(labelLayout);
    clearLayout(lineEditLayout);
    if(currentParam != nullptr && currentFigure != nullptr) {
        auto text = currentParam->text();

        auto figureName = currentFigure->text();
        auto currentOperation = static_cast<Operation>(currentParam->data(Qt::UserRole).toInt());
        if(!nominalTurbineParams[figureName].contains(currentOperation)) {
            for(auto i = 0; i < static_cast<int>(Operation::OperationsCount); i++) {
                nominalTurbineParams[figureName].insert(static_cast<Operation>(i), new TurbineParams());
            }
        }
        if(currentParam->checkState() == Qt::Checked) {
            auto currTurbineParams = nominalTurbineParams[figureName][currentOperation];
            labelLayout->addWidget(new QLabel("Nominal"));
            auto nomEdit = new QLineEdit(QString::number(currTurbineParams->nominal));
            nomEdit->setReadOnly(true);
            lineEditLayout->addWidget(nomEdit);

            labelLayout->addWidget(new QLabel("UT"));
            editToleranceTop = new QLineEdit(QString::number(currTurbineParams->toleranceTop));
            editToleranceTop->setValidator(_doubleValidator);
            connect(editToleranceTop, &QLineEdit::textChanged, this, [&]() {
                auto operation = static_cast<Operation>(paramList->currentItem()->data(Qt::UserRole).toInt());
                toleranceTopValue = editToleranceTop->text().toDouble() ? editToleranceTop->text().toDouble() : 0;
                toleranceBottomValue = editToleranceBottom->text().toDouble() ? editToleranceBottom->text().toDouble() : 0;
                nominalTurbineParams[nominals->currentItem()->text()][operation]->toleranceTop = toleranceTopValue;
                setTolerances(operation, toleranceTopValue, toleranceBottomValue);
            });
            lineEditLayout->addWidget(editToleranceTop);

            labelLayout->addWidget(new QLabel("LT"));
            editToleranceBottom = new QLineEdit(QString::number(currTurbineParams->toleranceBottom));
            editToleranceBottom->setValidator(_doubleValidator);
            connect(editToleranceBottom, &QLineEdit::textChanged, this, [&]() {
                auto operation = static_cast<Operation>(paramList->currentItem()->data(Qt::UserRole).toInt());
                toleranceBottomValue = editToleranceBottom->text().toDouble() ? editToleranceBottom->text().toDouble() : 0;
                toleranceTopValue = editToleranceTop->text().toDouble() ? editToleranceTop->text().toDouble() : 0;
                nominalTurbineParams[nominals->currentItem()->text()][operation]->toleranceBottom = toleranceBottomValue;
                setTolerances(operation, toleranceTopValue, toleranceBottomValue);
            });
            lineEditLayout->addWidget(editToleranceBottom);

            switch(currentOperation) {
                case Operation::WidthLE:
                    labelLayout->addWidget(new QLabel("At (mm)"));
                    atEditLE = new QLineEdit(QString::number(nominalTurbineParams[figureName][Operation::WidthLE]->atWidth));
                    atEditLE->setValidator(_doubleValidator);
                    connect(atEditLE, &QLineEdit::textChanged, this, [&]() {
                        atLEValue = atEditLE->text().toDouble() ? atEditLE->text().toDouble() : 1;
                        nominalTurbineParams[nominals->currentItem()->text()][Operation::WidthLE]->atWidth = atLEValue;
                    });
                    lineEditLayout->addWidget(atEditLE);
                    break;
                case Operation::WidthTE:
                    labelLayout->addWidget(new QLabel("At (mm)"));
                    atEditTE = new QLineEdit(QString::number(nominalTurbineParams[figureName][Operation::WidthTE]->atWidth));
                    atEditTE->setValidator(_doubleValidator);
                    connect(atEditTE, &QLineEdit::textChanged, this, [&]() {
                        atTEValue = atEditTE->text().toDouble() ? atEditTE->text().toDouble() : 1;
                        nominalTurbineParams[nominals->currentItem()->text()][Operation::WidthTE]->atWidth = atTEValue;
                    });
                    lineEditLayout->addWidget(atEditTE);
                    break;
            }
        }
    }
}

void TurbineWindow::setTolerances(Operation operation, double toleranceTopValue, double toleranceBottomValue) {
    switch(operation) {
        case Operation::MaxWidth:
            _reportData->setMaxWidthTolerances(toleranceTopValue, toleranceBottomValue);
            break;
        case Operation::MaxWidthX:
            _reportData->setXMaxWidthTolerances(toleranceTopValue, toleranceBottomValue);
            break;
        case Operation::MaxWidthY:
            _reportData->setYMaxWidthTolerances(toleranceTopValue, toleranceBottomValue);
            break;
        case Operation::ChordLength:
            _reportData->setChordLengthTolerances(toleranceTopValue, toleranceBottomValue);
            break;
        case Operation::WidthLE:
            _reportData->setLEWidthTolerances(toleranceTopValue, toleranceBottomValue);
            break;
        case Operation::WidthTE:
            _reportData->setTEWidthTolerances(toleranceTopValue, toleranceBottomValue);
            break;
        case Operation::RadiusLE:
            _reportData->setLERadiusTolerances(toleranceTopValue, toleranceBottomValue);
            break;
        case Operation::RadiusTE:
            _reportData->setTERadiusTolerances(toleranceTopValue, toleranceBottomValue);
            break;
    }
}

void TurbineWindow::changeItemOfList() {
    auto selectedItemsOfListNominal = nominals->selectedItems();
    auto selectedItemsOfListMeasured = measured->selectedItems();
    auto currentItemOfListNominal = selectedItemsOfListNominal.length() == 1 ? selectedItemsOfListNominal[0] : nullptr;
    auto currentItemOfListMeasured = selectedItemsOfListMeasured.length() == 1 ? selectedItemsOfListMeasured[0] : nullptr;
    _curveGraphics->drawCurve(_project, currentItemOfListNominal, Qt::green, 0.1, currentItemOfListMeasured, Qt::blue, 0.1);
}

void TurbineWindow::closeEvent(QCloseEvent * event) {
    closeWindow();
}

TurbineWindow::~TurbineWindow() {
    delete _ui;
}
