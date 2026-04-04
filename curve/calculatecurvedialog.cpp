#include "curve/pch.h"

#include "algorithms.h"
#include "calculatecurvedialog.h"
#include "ui_calculatecurvedialog.h"

CalculateCurveDialog::CalculateCurveDialog() : _ui(new Ui::CalculateCurveDialog())
{
    _ui->setupUi(this);

    _ui->valueLE->setValidator(new QIntValidator);

    _ui->eliminateSB->setSingleStep(0.1);
    _ui->minlineSB->setSingleStep(0.1);

    connect(_ui->commonRB, &QRadioButton::toggled, [&](bool checked) {
        if(checked) {
            _ui->stackedWidget->setCurrentWidget(_ui->page1);
        }
    });
    connect(_ui->regenerateRB, &QRadioButton::toggled, [&](bool checked) {
        if(checked) {
            _ui->stackedWidget->setCurrentWidget(_ui->page2);
        }
    });
    connect(_ui->equalizePB, &QPushButton::clicked, [&]() {
        _ui->resultCurveLE->setText(_ui->initialCurveCBox->currentText());
    });
    connect(_ui->closedRB, &QRadioButton::toggled, [&](bool checked) {
        if(checked) {
            _ui->materialRB1->setText("External");
            _ui->materialRB2->setText("Internal");
        }
        else {
            _ui->materialRB1->setText("Right");
            _ui->materialRB2->setText("Left");
        }
    });
    connect(_ui->initialCurveCBox, &QComboBox::textActivated, this, &CalculateCurveDialog::updateResultNameAndClosed);
    connect(_ui->buttonBox, &QDialogButtonBox::accepted, this, &CalculateCurveDialog::calculateCurve);
    connect(_ui->buttonBox, &QDialogButtonBox::rejected, this, &CalculateCurveDialog::close);
}

void CalculateCurveDialog::initialize()
{
    _ui->initialCurveCBox->clear();
    _ui->resultCurveLE->setText("");
    _ui->closedRB->setChecked(true);
    _ui->materialRB1->setChecked(true);
    _ui->commonRB->setChecked(true);
    _ui->sortCB->setChecked(false);
    _ui->eliminateSB->setValue(0.0);
    _ui->intermediateSB->setValue(0);
    _ui->minlineSB->setValue(0.0);
    _ui->numberRB->setChecked(true);
    _ui->valueLE->setText("10");
    // setWindowTitle("Calculate 2D Curve");

    auto project = &Project::instance();
    auto figures = project->figures();
    for(auto& figure : figures) {
        if(dynamic_cast<CurveFigure*>(figure)) {
            _ui->initialCurveCBox->addItem(figure->name());
        }
    }
    if(_ui->initialCurveCBox->findText(project->currentFigureName(), Qt::MatchExactly) != -1) {
        _ui->initialCurveCBox->setCurrentIndex(_ui->initialCurveCBox->findText(project->currentFigureName(), Qt::MatchExactly));
        updateResultNameAndClosed(project->currentFigureName());
    }
    else {
        _ui->initialCurveCBox->setCurrentIndex(-1);
    }

    show();
}

void CalculateCurveDialog::updateResultNameAndClosed(QString curveName)
{
    if(!curveName.isEmpty()) {
        _ui->resultCurveLE->setText(curveName + "_Recalculated");
        setWindowTitle("Calculate " + curveName);
        auto isClosed = dynamic_cast<const CurveFigure*>(Project::instance().findFigure(curveName))->isClosed();
        if(isClosed) {
            _ui->closedRB->setChecked(true);
        }
        else {
            _ui->openedRB->setChecked(true);
        }
    }
    else {
        _ui->resultCurveLE->clear();
    }
}

void CalculateCurveDialog::calculateCurve()
{
    if(_ui->initialCurveCBox->currentText().isEmpty()) {
        auto text = QMessageBox::warning(this, "No initial curve selected", "Select initial curve", "Ok");
        return;
    }

    auto curveName = _ui->initialCurveCBox->currentText();
    auto newCurveName = _ui->resultCurveLE->text();
    if(newCurveName.isEmpty()) {
        newCurveName = curveName + "_Recalculated";
    }

    auto isClosed = _ui->closedRB->isChecked();
    auto isExternal = _ui->materialRB1->isChecked();
    FunctionParams::Direction material = FunctionParams::Direction::Right;
    if(_ui->materialRB2->isChecked()) {
        material = FunctionParams::Direction::Left;
    }

    if(_ui->commonRB->isChecked()) {
        auto needSort = _ui->sortCB->isChecked();
        auto eliminate = _ui->eliminateSB->value();
        auto minline = _ui->minlineSB->value();
        auto intermediate = _ui->intermediateSB->value();

        const Function1Params* params = new Function1Params(intermediate, eliminate, minline, isClosed, isExternal, material, needSort);
        Algorithms::calculateCurve(curveName, newCurveName, params, &Project::instance());
    }
    else if(_ui->regenerateRB->isChecked()) {
        auto value = _ui->valueLE->text().toInt();
        auto mode = _ui->numberRB->isChecked() ? "number" : "density";

        const Function19Params* params = new Function19Params(isClosed, isExternal, material, mode, value);
        Algorithms::regenerateCurve(curveName, newCurveName, params);
    }

    accept();
}

CalculateCurveDialog::~CalculateCurveDialog()
{
    delete _ui;
}
