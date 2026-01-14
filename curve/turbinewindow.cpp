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

TurbineWindow::TurbineWindow(Project *project, QWidget *parent) : QDialog(parent), _ui(new Ui::TurbineWindow()) {
    _ui->setupUi(this);
    _project = project;
    _curveGraphics = new CurveGraphics(_project, _ui->graphicsView);
    this->setWindowTitle("Calculate turbine");
    setupWindow();
    connect(_ui->closeButton, &QPushButton::clicked, this, &TurbineWindow::closeWindow);
    connect(_ui->calculateButton, &QPushButton::clicked, this, &TurbineWindow::calculateNominals);
    connect(_ui->runButton, &QPushButton::clicked, this, &TurbineWindow::run);
    connect(_ui->preparePointsCheckBox, &QCheckBox::clicked, this, &TurbineWindow::onPreparePointsClick);
    connect(nominals, &QListWidget::currentItemChanged, this, &TurbineWindow::updateParamOutputView);
    connect(_ui->paramList, &QListWidget::currentItemChanged, this, &TurbineWindow::updateParamOutputView);
    connect(paramList, &QListWidget::itemChanged, this, &TurbineWindow::showTurbineParams);
    connect(_ui->listNominal, &QListWidget::itemClicked, this, &TurbineWindow::changeItemOfList);
    connect(_ui->listMeasured, &QListWidget::itemClicked, this, &TurbineWindow::changeItemOfList);
}

void TurbineWindow::initialization() {
    measured->clear();
    nominals->clear();

    auto figures = _project->figures();
    _curveGraphics = new CurveGraphics(_project, _ui->graphicsView);

    for(auto item : figures) {
        if(dynamic_cast<CurveFigure*>(item)) {
            auto name = item->name();
            measured->addItem(new QListWidgetItem(name));
            nominals->addItem(new QListWidgetItem(name));
        }
    }

    if(paramList->item(0)->data(Qt::UserRole).isNull()) {
        for(auto i = 0; i < paramList->count(); i++) {
            paramList->item(i)->setData(Qt::UserRole, static_cast<Operation>(i));
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
    dimensionType->setCurrentIndex(0);

    if(needPreparePoints->isChecked()) {
        needPreparePoints->click();
    }
    _ui->needSortCheckBox->setChecked(false);
    _ui->deleteSimilarCheckBox->setChecked(false);
    _ui->radiusCorrectionCheckBox->setChecked(false);
    _ui->use3DVectorsCheckBox->setChecked(false);

    formTab->setCurrentIndex(0);
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
    writeWithTemplate->setChecked(false);
    reportComment->clear();
    allProfilesToReport->clear();
}

void TurbineWindow::setupWindow() {
    //validator
    _doubleValidator = new QDoubleValidator();
    _doubleValidator->setDecimals(9);
    _doubleValidator->setLocale(QLocale::C);
    _doubleValidator->setNotation(QDoubleValidator::StandardNotation);

    //profiles
    nominals = _ui->listNominal;
    measured = _ui->listMeasured;
    directionLE = _ui->directionLEComboBox;
    zoneScaleLE = _ui->scaleLE;
    zoneScaleLE->setValidator(_doubleValidator);
    zoneScaleTE = _ui->scaleTE;
    zoneScaleTE->setValidator(_doubleValidator);
    dimensionType = _ui->measureTypeComboBox;

    //prepare points
    needPreparePoints = _ui->preparePointsCheckBox;

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

    //TE form
    formTE = _ui->formTECheckBox;
    bestFitTE = _ui->bestFitComboBox_3;
    showNumTE = _ui->showComboBox_3;
    scaleTE = _ui->scaleLineEdit_3;
    scaleTE->setValidator(_doubleValidator);
    increaseTE = _ui->increaseLineEdit_3;
    increaseTE->setValidator(_doubleValidator);
    axisTE = _ui->axisComboBox_3;

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
    if(nominals->currentItem() != nullptr) {
        auto figureName = nominals->currentItem()->text();
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
                        Algorithms::getMaxWidthCircle(figureName, new Function18Params(/*0, scaleLEValue, scaleTEValue*/), _project);
                        auto maxCircle = static_cast<const CircleFigure*>(_project->findFigure(figureName + "_MaxDia"));
                        nominalTurbineParams[figureName][Operation::MaxWidth]->nominal = maxCircle->radius() * 2;
                        nominalTurbineParams[figureName][Operation::MaxWidthX]->nominal = maxCircle->center().x;
                        nominalTurbineParams[figureName][Operation::MaxWidthY]->nominal = maxCircle->center().y;
                        break;
                    }
                    case Operation::ChordLength:
                    {
                        auto result = Algorithms::getChordLength(figureName, new Function18Params(/*0, scaleLEValue, scaleTEValue*/), _project);
                        nominalTurbineParams[figureName][operation]->nominal = result;
                        break;
                    }
                    case Operation::WidthLE:
                    case Operation::WidthTE:
                    {
                        auto result = Algorithms::getWidthOfEdges(figureName, nominalTurbineParams[figureName][WidthLE]->atWidth,
                            nominalTurbineParams[figureName][WidthTE]->atWidth, _project, operation == Operation::WidthLE, operation == Operation::WidthTE);
                        nominalTurbineParams[figureName][Operation::WidthLE]->nominal = result[0];
                        nominalTurbineParams[figureName][Operation::WidthTE]->nominal = result[1];
                        break;
                    }
                    case Operation::RadiusLE:
                    case Operation::RadiusTE:
                    {
                        auto result = Algorithms::getRadiusOfEdges(figureName, new Function18Params(0, scaleLEValue, scaleTEValue), _project);
                        nominalTurbineParams[figureName][Operation::RadiusTE]->nominal = result[1];
                        nominalTurbineParams[figureName][Operation::RadiusLE]->nominal = result[0];
                        break;
                    }
                }
            }
        }
        updateParamOutputView();
    }
}

void TurbineWindow::run() {
    if(nominals->currentItem() != nullptr) {
        auto figureName = nominals->currentItem()->text();
        auto scaleLEValue = zoneScaleLE->text().toDouble();
        auto scaleTEValue = zoneScaleTE->text().toDouble();
        auto needMaxDia = _ui->maxDiamCheckBox->isChecked();
        auto needMCL = _ui->mclCheckBox->isChecked();
        auto needChord = _ui->contactCheckBox->isChecked();
        Algorithms::turbineRun(figureName, new Function18Params(0, scaleLEValue, scaleTEValue), _project,
            needMaxDia, needMCL, needChord);
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

void TurbineWindow::showTurbineParams(QListWidgetItem *currParam) {
    paramList->blockSignals(true);
    paramList->setCurrentItem(currParam); //check also changes currentItem
    paramList->blockSignals(false);
    updateParamOutputView();
}

void TurbineWindow::clearLayout(QLayout *layout) {
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
        auto operation = static_cast<Operation>(currentParam->data(Qt::UserRole).toInt());
        if(!nominalTurbineParams[figureName].contains(operation)) {
            for(auto i = 0; i < Operation::OperationsCount; i++) {
                nominalTurbineParams[figureName].insert(static_cast<Operation>(i), new TurbineParams());
            }
        }
        if(currentParam->checkState() == Qt::Checked) {
            auto currTurbineParams = nominalTurbineParams[figureName][operation];
            labelLayout->addWidget(new QLabel("Nominal"));
            auto nomEdit = new QLineEdit(QString::number(currTurbineParams->nominal));
            nomEdit->setReadOnly(true);
            lineEditLayout->addWidget(nomEdit);
            labelLayout->addWidget(new QLabel("Tolerance top"));
            lineEditLayout->addWidget(new QLineEdit(QString::number(currTurbineParams->toleranceTop)));
            labelLayout->addWidget(new QLabel("Tolerance bottom"));
            lineEditLayout->addWidget(new QLineEdit(QString::number(currTurbineParams->toleranceBottom)));
            switch(operation) {
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
                {
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
}

void TurbineWindow::changeItemOfList() {
    auto selectedItemsOfListNominal = _ui->listNominal->selectedItems();
    auto selectedItemsOfListMeasured = _ui->listMeasured->selectedItems();
    auto currentItemOfListNominal = selectedItemsOfListNominal.length() == 1 ? selectedItemsOfListNominal[0] : nullptr;
    auto currentItemOfListMeasured = selectedItemsOfListMeasured.length() == 1 ? selectedItemsOfListMeasured[0] : nullptr;
    _curveGraphics->drawCurve(currentItemOfListNominal, Qt::green, 0.1, currentItemOfListMeasured, Qt::blue, 0.1);
}

void TurbineWindow::closeEvent(QCloseEvent *event) {
    closeWindow();
}

TurbineWindow::~TurbineWindow() {
    delete _ui;
}
