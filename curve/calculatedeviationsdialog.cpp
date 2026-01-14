#include "curve/pch.h"
#include "calculatedeviationsdialog.h"

CalculateDeviationsDialog::CalculateDeviationsDialog(Project *project, QWidget *parent)
    : QDialog(parent)
    , _ui(new Ui::CalculateDeviationsDialogClass())
{
    _ui->setupUi(this);
    _project = project;

    connect(_ui->saveDevInNomCheckBox, &QCheckBox::stateChanged, this, &CalculateDeviationsDialog::switchResultNameLineEdit);
    connect(_ui->advSettingsCheckBox, &QCheckBox::stateChanged, this, &CalculateDeviationsDialog::switchAdvancedSettings);
    connect(_ui->closedRadioButton, &QRadioButton::toggled, this, &CalculateDeviationsDialog::changeCurveType);
    connect(_ui->buttonBox, &QDialogButtonBox::rejected, this, &CalculateDeviationsDialog::resetDialog);
    connect(_ui->buttonBox, &QDialogButtonBox::rejected, this, &CalculateDeviationsDialog::reject);
    connect(_ui->buttonBox, &QDialogButtonBox::accepted, this, &CalculateDeviationsDialog::calculateDeviations);
    connect(_ui->measCurvesComboBox, &QComboBox::currentIndexChanged, this, &CalculateDeviationsDialog::changeResultName);

    _ui->nomTolLineEdit->setValidator(new QIntValidator);
}

void CalculateDeviationsDialog::initialization() {
    auto figures = _project->figures();
    for(auto figure : figures) {
        if(dynamic_cast<CurveFigure*>(figure)) {
            _ui->measCurvesComboBox->addItem(figure->name());
            _ui->nomCurvesComboBox->addItem(figure->name());
        }
    }
    _ui->measCurvesComboBox->setCurrentIndex(_ui->measCurvesComboBox->findText(_project->currentFigureName(), Qt::MatchExactly));
    _ui->nomCurvesComboBox->setCurrentIndex(-1);

    adjustSize();
    exec();
}

void CalculateDeviationsDialog::calculateDeviations() {
    if(_ui->measCurvesComboBox->currentText().isEmpty()) {
        auto text = QMessageBox::warning(this, "No measured curve selected", "Select measured curve", "Ok");
    } else if(_ui->nomCurvesComboBox->currentText().isEmpty()) {
        auto text = QMessageBox::warning(this, "No nominal curve selected", "Select nominal curve", "Ok");
    } else {
        auto measFigure = _project->findFigure(_ui->measCurvesComboBox->currentText());
        auto measCurve = dynamic_cast<const CurveFigure*>(measFigure);
        auto nomFigure = _project->findFigure(_ui->nomCurvesComboBox->currentText());
        auto nomCurve = dynamic_cast<const CurveFigure*>(nomFigure);

        QString resultName;
        if(_ui->saveDevInNomCheckBox->isChecked()) {
            resultName = _ui->nomCurvesComboBox->currentText();
        } else {
            resultName = _ui->resultNameLineEdit->text();
            if(resultName.isEmpty()) {
                resultName = _ui->measCurvesComboBox->currentText() + "_Deviations";
            }
        }

        Function4Params params;
        if(_ui->advSettingsCheckBox->isChecked()) {
            auto nomTol = _ui->nomTolLineEdit->text().toDouble();
            auto needSort = _ui->sortCheckBox->isChecked();
            auto evaPlace = 1;
            if(_ui->measPlaceRadioButton->isChecked()) {
                evaPlace = 2;
            }
            auto evaDir = 1;
            if(_ui->measDirRadioButton->isChecked()) {
                evaDir = 2;
            }
            auto isClosed = true;
            if(_ui->openedRadioButton->isChecked()) {
                isClosed = false;
            }
            auto isExternal = true;
            auto measSide = FunctionParams::Direction::Left;
            if(_ui->side2RadioButton->isChecked()) {
                isExternal = false;
                measSide = FunctionParams::Direction::Right;
            }
            params = Function4Params(nomTol, evaPlace, evaDir, isClosed, isExternal, measSide, needSort);
        } else {
            params = Function4Params(0, 1, 1, true, true, FunctionParams::Direction::Left, false);
        }

        Algorithms::calculateDeviations(nomCurve->name(), measCurve->name(), resultName, &params, _project);

        resetDialog();
        accept();
    }
}

void CalculateDeviationsDialog::switchResultNameLineEdit() {
    if(_ui->saveDevInNomCheckBox->isChecked()) {
        _ui->resultNameLineEdit->setEnabled(false);
    } else {
        _ui->resultNameLineEdit->setEnabled(true);
    }
}

void CalculateDeviationsDialog::switchAdvancedSettings() {
    if(_ui->advSettingsCheckBox->isChecked()) {
        _ui->AdvancedSettingsGroupBox->setEnabled(true);
    } else {
        _ui->AdvancedSettingsGroupBox->setEnabled(false);
    }
}

void CalculateDeviationsDialog::changeCurveType() {
    if(_ui->closedRadioButton->isChecked()) {
        _ui->side1RadioButton->setText("External");
        _ui->side2RadioButton->setText("Internal");
    } else {
        _ui->side1RadioButton->setText("Left");
        _ui->side2RadioButton->setText("Right");
    }
}

void CalculateDeviationsDialog::changeResultName() {
    if(!_ui->measCurvesComboBox->currentText().isEmpty()) {
        _ui->resultNameLineEdit->setText(_ui->measCurvesComboBox->currentText() + "_Deviations");
    } else {
        _ui->resultNameLineEdit->setText(QString());
    }
}

void CalculateDeviationsDialog::resetDialog() {
    _ui->measCurvesComboBox->clear();
    _ui->nomCurvesComboBox->clear();
    _ui->resultNameLineEdit->clear();
    _ui->saveDevInNomCheckBox->setChecked(false);
    _ui->advSettingsCheckBox->setChecked(false);
    _ui->closedRadioButton->setChecked(true);
    _ui->side1RadioButton->setChecked(true);
    _ui->nomPlaceRadioButton->setChecked(true);
    _ui->nomDirRadioButton->setChecked(true);
    _ui->nomTolLineEdit->setText("0");
    _ui->sortCheckBox->setChecked(false);
}

void CalculateDeviationsDialog::closeEvent(QCloseEvent *event) {
    resetDialog();
    reject();
}

CalculateDeviationsDialog::~CalculateDeviationsDialog()
{
    delete _ui;
}
