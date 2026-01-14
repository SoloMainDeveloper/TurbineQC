#include "curve/pch.h"
#include "radiuscorrectiondialog.h"

RadiusCorrectionDialog::RadiusCorrectionDialog(Project *project, QWidget *parent)
    : QDialog(parent)
    , _ui(new Ui::RadiusCorrectionDialogClass())
{
    _ui->setupUi(this);
    _project = project;

    connect(_ui->buttonBox, &QDialogButtonBox::accepted, this, &RadiusCorrectionDialog::calculateOffsetCurve);
    connect(_ui->buttonBox, &QDialogButtonBox::rejected, this, &RadiusCorrectionDialog::resetDialog);
    connect(_ui->buttonBox, &QDialogButtonBox::rejected, this, &RadiusCorrectionDialog::reject);
    connect(_ui->equalPushButton, &QPushButton::clicked, this, &RadiusCorrectionDialog::equalizeResultToInitial);
    connect(_ui->divOffsetBy2PushButton, &QPushButton::clicked, this, &RadiusCorrectionDialog::devOffsetByTwo);
    connect(_ui->closedRadioButton, &QRadioButton::toggled, this, &RadiusCorrectionDialog::changeCurveType);
    connect(_ui->curvesComboBox, &QComboBox::currentTextChanged, this, &RadiusCorrectionDialog::changeResultCurveName);

    _ui->offsetLineEdit->setValidator(new QDoubleValidator(0, INFINITY, -1));
}

void RadiusCorrectionDialog::initialization() {
    auto figures = _project->figures();
    for(auto figure : figures) {
        if(dynamic_cast<CurveFigure*>(figure)) {
            _ui->curvesComboBox->addItem(figure->name());
        }
    }
    _ui->curvesComboBox->setCurrentIndex(_ui->curvesComboBox->findText(_project->currentFigureName(), Qt::MatchExactly));
   
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
    if (_ui->closedRadioButton->isChecked()) {
        _ui->direction1RadioButton->setText("Internal");
        _ui->direction2RadioButton->setText("External");
    } else {
        _ui->direction1RadioButton->setText("Right");
        _ui->direction2RadioButton->setText("Left");
    }
}

void RadiusCorrectionDialog::calculateOffsetCurve() {
    if(_ui->curvesComboBox->currentText().isEmpty()) {
        auto text = QMessageBox::warning(this, "No initial curve selected", "Select initial curve", "Ok");
    } else {
        auto curveName = _ui->curvesComboBox->currentText();
        auto newCurveName = _ui->resultCurveLineEdit->text();
        if(newCurveName.isEmpty()) {
            newCurveName = curveName + "_Radius_Corrected";
        }
        auto offset = _ui->offsetLineEdit->text().toDouble();
        auto isClosed = _ui->closedRadioButton->isChecked();
        auto isExternal = _ui->direction2RadioButton->isChecked();
        auto needSort = _ui->sortCheckBox->isChecked();
        FunctionParams::Direction direction = FunctionParams::Direction::Right;
        if(_ui->direction2RadioButton->isChecked()) {
            direction = FunctionParams::Direction::Left;
        }

        const Function3Params *params = new Function3Params(offset, isClosed, isExternal, direction, needSort);
        Algorithms::makeRadiusCorrection(curveName, newCurveName, params, _project);

        resetDialog();
        accept();
    }  
}

void RadiusCorrectionDialog::changeResultCurveName(QString curveName) {
    if(!curveName.isEmpty()) {
        _ui->resultCurveLineEdit->setText(curveName + "_Radius_Corrected");
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

void RadiusCorrectionDialog::closeEvent(QCloseEvent *event) {
    resetDialog();
    reject();
}

RadiusCorrectionDialog::~RadiusCorrectionDialog()
{
    delete _ui;
}
