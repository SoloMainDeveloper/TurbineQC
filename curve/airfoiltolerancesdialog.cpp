#include "curve/pch.h"
#include "airfoiltolerancesdialog.h"

AirfoilTolerancesDialog::AirfoilTolerancesDialog(Project* mainProject, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AirfoilTolerancesDialogClass())
{
    ui->setupUi(this);
    
    setWindowTitle("Assign Tolerances to Nominal");

    _project = mainProject;
    connect(this, &AirfoilTolerancesDialog::curveChangeToleranceRequested, _project, &Project::changeCurveTolerance);

    connect(ui->listWidgetCurves, &QListWidget::currentItemChanged, this, &AirfoilTolerancesDialog::changeCurrentMode);

    ui->comboBoxMode->addItem("Constant tolerance");
    connect(ui->comboBoxMode, &QComboBox::currentTextChanged, this, &AirfoilTolerancesDialog::changeCurrentMode);

    _buttonBox = new QDialogButtonBox;
    _buttonBox->setCenterButtons(true);
    _buttonBox->addButton(QDialogButtonBox::Ok);
    _buttonBox->addButton(QDialogButtonBox::Cancel);
    connect(_buttonBox, &QDialogButtonBox::accepted, this, &AirfoilTolerancesDialog::assignTolerances);
    connect(_buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

void AirfoilTolerancesDialog::dialogInitialization() {
    ui->listWidgetCurves->clear();

    auto currentCurveName = _project->currentFigureName();
    auto figures = _project->figures();
    for(auto item : figures) {
        if(dynamic_cast<CurveFigure*>(item)) {
            auto name = item->name();
            auto listItem = new QListWidgetItem(name);
            ui->listWidgetCurves->addItem(listItem);
            if(name == currentCurveName) {
                ui->listWidgetCurves->blockSignals(true);
                ui->listWidgetCurves->setCurrentItem(listItem);
                ui->listWidgetCurves->blockSignals(false);
            }
        }
    }
    changeCurrentMode();
    exec();
}

void AirfoilTolerancesDialog::changeCurrentMode() {
    while(ui->assignLayout->itemAt(1)) {
        auto item = ui->assignLayout->takeAt(0);
        delete item->widget();
        delete item;
    }

    auto currentMode = ui->comboBoxMode->currentText();

    QString currentCurveName;
    const CurveFigure* currentCurve = nullptr;

    if(auto currentCurveItem = ui->listWidgetCurves->currentItem()) {
        currentCurveName = currentCurveItem->text();
        auto currentFigure = _project->findFigure(currentCurveName);
        currentCurve = dynamic_cast<const CurveFigure*>(currentFigure);
        auto button = _buttonBox->button(QDialogButtonBox::Ok);
        button->setEnabled(true);
    } else {
        auto button = _buttonBox->button(QDialogButtonBox::Ok);
        button->setEnabled(false);
    }

    if(currentMode == "Constant tolerance") {
        QLabel* upperToleranceLabel = new QLabel("Upper tolerance");
        QLabel* lowerToleranceLabel = new QLabel("Lower tolerance");
        upperToleranceLE = new QLineEdit;
        upperToleranceLE->setValidator(new QDoubleValidator);
        lowerToleranceLE = new QLineEdit;
        lowerToleranceLE->setValidator(new QDoubleValidator);
        
        if(currentCurve) {
            auto currentCurvePoints = currentCurve->points();
            auto currentCurveUpperTolerance = currentCurvePoints[0].ut;
            upperToleranceLE->setEnabled(true);
            upperToleranceLE->setText(QString::number(currentCurveUpperTolerance));
            auto currentCurveLowerTolerance = currentCurvePoints[0].lt;
            lowerToleranceLE->setEnabled(true);
            lowerToleranceLE->setText(QString::number(currentCurveLowerTolerance));
        } else {
            upperToleranceLE->setEnabled(false);
            lowerToleranceLE->setEnabled(false);
        }

        ui->assignLayout->addStretch();
        ui->assignLayout->addWidget(upperToleranceLabel);
        ui->assignLayout->addWidget(upperToleranceLE);
        ui->assignLayout->addWidget(lowerToleranceLabel);
        ui->assignLayout->addWidget(lowerToleranceLE);
        ui->assignLayout->addStretch();
        ui->assignLayout->addWidget(_buttonBox);
    }
}

void AirfoilTolerancesDialog::assignTolerances() {
    auto currentMode = ui->comboBoxMode->currentText();

    auto currentCurveItem = ui->listWidgetCurves->currentItem();
    auto currentCurveName = currentCurveItem->text();
    auto currentCurve = _project->findFigure(currentCurveName);

    if(currentMode == "Constant tolerance") {
        auto newUpperTolerance = upperToleranceLE->text().toDouble();
        auto newLowerTolerance = lowerToleranceLE->text().toDouble();
        emit curveChangeToleranceRequested(currentCurveName, newUpperTolerance, newLowerTolerance);
    }

    accept();
}

AirfoilTolerancesDialog::~AirfoilTolerancesDialog() {
    delete ui;
}
