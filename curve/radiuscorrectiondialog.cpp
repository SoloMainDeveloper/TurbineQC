#include "curve/pch.h"

#include "radiuscorrectiondialog.h"
#include "ui_radiuscorrectiondialog.h"
#include "algorithms.h"

RadiusCorrectionDialog::RadiusCorrectionDialog() : _ui(new Ui::RadiusCorrectionDialog()) {
    _ui->setupUi(this);
    _project = &Project::instance();

    connect(_ui->buttonBox, &QDialogButtonBox::accepted, this, &RadiusCorrectionDialog::calculateOffsetCurve);
    connect(_ui->buttonBox, &QDialogButtonBox::rejected, this, &RadiusCorrectionDialog::resetDialog);
    connect(_ui->buttonBox, &QDialogButtonBox::rejected, this, &RadiusCorrectionDialog::reject);
    connect(_ui->equalPushButton, &QPushButton::clicked, this, &RadiusCorrectionDialog::equalizeResultToInitial);
    connect(_ui->divOffsetBy2PushButton, &QPushButton::clicked, this, &RadiusCorrectionDialog::devOffsetByTwo);
    connect(_ui->closedRadioButton, &QRadioButton::toggled, this, &RadiusCorrectionDialog::changeCurveType);
    connect(_ui->curvesComboBox, &QComboBox::textActivated, this, &RadiusCorrectionDialog::updateResultNameAndClosed);

    _ui->offsetLineEdit->setValidator(new QDoubleValidator(0, INFINITY, -1));
}

void RadiusCorrectionDialog::initialize() {
    auto figures = _project->figures();
    for(auto figure : figures) {
        if(dynamic_cast<CurveFigure*>(figure)) {
            _ui->curvesComboBox->addItem(figure->name());
        }
    }
    if(_ui->curvesComboBox->findText(_project->currentFigureName(), Qt::MatchExactly) != -1) {
        _ui->curvesComboBox->setCurrentIndex(_ui->curvesComboBox->findText(_project->currentFigureName(), Qt::MatchExactly));
        updateResultNameAndClosed(_project->currentFigureName());
    } else {
        _ui->curvesComboBox->setCurrentIndex(-1);
    }

    adjustSize();
    exec();
}

void RadiusCorrectionDialog::equalizeResultToInitial() {
    _ui->resultCurveLineEdit->setText(_ui->curvesComboBox->currentText());
}

void RadiusCorrectionDialog::devOffsetByTwo() {
    _ui->offsetLineEdit->setText(QString::number((_ui->offsetLineEdit->text()).toDouble() / 2));
}

void RadiusCorrectionDialog::changeCurveType() {
    if(_ui->closedRadioButton->isChecked()) {
        _ui->direction1RadioButton->setText("External");
        _ui->direction2RadioButton->setText("Internal");
    } else {
        _ui->direction1RadioButton->setText("Right");
        _ui->direction2RadioButton->setText("Left");
    }
}

void RadiusCorrectionDialog::calculateOffsetCurve() {
    if(_ui->curvesComboBox->currentText().isEmpty()) {
        auto text = QMessageBox::warning(this, "No initial curve selected", "Select initial curve", "Ok");
        return;
    }
    auto curveName = _ui->curvesComboBox->currentText();
    auto newCurveName = _ui->resultCurveLineEdit->text();
    if(newCurveName.isEmpty()) {
        newCurveName = curveName + "_Radius_Corrected";
    }
    auto offset = _ui->offsetLineEdit->text().toDouble();
    auto isClosed = _ui->closedRadioButton->isChecked();
    auto isExternal = _ui->direction1RadioButton->isChecked();
    auto needSort = _ui->sortCheckBox->isChecked();
    FunctionParams::Direction direction = FunctionParams::Direction::Right;
    if(_ui->direction2RadioButton->isChecked()) {
        direction = FunctionParams::Direction::Left;
    }

    const Function3Params* params = new Function3Params(offset, isClosed, isExternal, direction, needSort);
    Algorithms::makeRadiusCorrection(curveName, newCurveName, params);

    resetDialog();
    accept();

}

void RadiusCorrectionDialog::updateResultNameAndClosed(QString curveName) {
    if(!curveName.isEmpty()) {
        _ui->resultCurveLineEdit->setText(curveName + "_Radius_Corrected");
        auto isClosed = dynamic_cast<const CurveFigure*>(_project->findFigure(curveName))->isClosed();
        if(isClosed) {
            _ui->closedRadioButton->setChecked(true);
        } else {
            _ui->openedRadioButton->setChecked(true);
        }
    } else {
        _ui->resultCurveLineEdit->clear();
    }
}

void RadiusCorrectionDialog::resetDialog() {
    _ui->curvesComboBox->clear();
    _ui->resultCurveLineEdit->clear();
    _ui->offsetLineEdit->setText("0.5");
    _ui->closedRadioButton->setChecked(true);
    _ui->direction1RadioButton->setChecked(true);
    _ui->sortCheckBox->setChecked(false);
}

void RadiusCorrectionDialog::closeEvent(QCloseEvent* event) {
    resetDialog();
    reject();
}

RadiusCorrectionDialog::~RadiusCorrectionDialog() {
    delete _ui;
}
