#include "curve/pch.h"

#include "bestfitdialog.h"

BestFitDialog::BestFitDialog() : _ui(new Ui::BestFitDialog)
{
    _ui->setupUi(this);
    _errorMessage = new QMessageBox(this);
    _errorMessage->setWindowTitle("Error");
    _questionMessage = new QMessageBox(this);
    _questionMessage->setText("A curve with that name already exists.\nDo you want to reassign the name?");
    _questionMessage->setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    _algorithm = Function6Params::Method::Curve;
    _isClosed = false;
    _needXShift = true;
    _needYShift = true;
    _needRotation = true;
    _needHConstraint = false;
    _needVConstraint = false;
    _needRConstraint = false;
    this->setWindowTitle("Best-fit");
    this->setFixedSize(440, 580);
    _ui->groupBoxConstraints->setFixedHeight(195);
    _ui->comboBoxMethods->addItem("LSQ (points-to-curve)");
    _ui->comboBoxMethods->addItem("LSQ (point-to-point)");
    //_ui->comboBoxMethods->addItem("Fit in tolerance band");
    //_ui->comboBoxMethods->addItem("Min. form fit");
    _ui->comboBoxMethods->setCurrentIndex(0);
    _ui->groupBoxConstraints->setEnabled(false);

    connect(_ui->listOfMeasuredCurves, &QListWidget::itemClicked, this, &BestFitDialog::changelistOfMeasuredCurves);
    connect(_ui->comboBoxMethods, &QComboBox::currentTextChanged, this, &BestFitDialog::changeMethod);
    connect(_ui->checkBoxClosed, &QCheckBox::clicked, this, &BestFitDialog::onClosed);
    connect(_ui->checkBoxXShift, &QCheckBox::clicked, this, &BestFitDialog::onXShift);
    connect(_ui->checkBoxYShift, &QCheckBox::clicked, this, &BestFitDialog::onYShift);
    connect(_ui->checkBoxRotation, &QCheckBox::clicked, this, &BestFitDialog::onRotation);
    connect(_ui->checkBoxConstraints, &QCheckBox::clicked, this, &BestFitDialog::onConstraints);
    connect(_ui->checkBoxHConstraint, &QCheckBox::clicked, this, &BestFitDialog::onHorizontalConstraint);
    connect(_ui->checkBoxVConstraint, &QCheckBox::clicked, this, &BestFitDialog::onVerticalConstraint);
    connect(_ui->checkBoxRConstraint, &QCheckBox::clicked, this, &BestFitDialog::onRotationConstraint);
    connect(_ui->calculateBtn, &QPushButton::clicked, this, &BestFitDialog::checkSettings);
    connect(_ui->closeBtn, &QPushButton::clicked, this, &BestFitDialog::closeDialog);
}

void BestFitDialog::initialize()
{
    auto figures = Project::instance().figures();

    for(auto figure : figures) {
        if(dynamic_cast<CurveFigure*>(figure)) {
            _ui->listOfMeasuredCurves->addItem(figure->name());
            _ui->listOfNominalCurves->addItem(figure->name());
        }
    }
    this->exec();
}

void BestFitDialog::changelistOfMeasuredCurves()
{
    auto selectedItems = _ui->listOfMeasuredCurves->selectedItems();
    auto currentItem = selectedItems.length() == 1 ? selectedItems[0] : nullptr;
    if(currentItem != nullptr) {
        _ui->lineEditResultName->setText(currentItem->text() + "_best-fit");
    }
}

void BestFitDialog::changeMethod()
{
    auto method = _ui->comboBoxMethods->currentText();
    Function6Params::Method algorithm;
    if(method == "LSQ (points-to-curve)") {
        _ui->checkBoxClosed->show();
        _algorithm = Function6Params::Method::Curve;
    }
    if(method == "LSQ (point-to-point)") {
        _ui->checkBoxClosed->hide();
        _algorithm = Function6Params::Method::Point;
    }
}

void BestFitDialog::onClosed()
{
    _isClosed = _ui->checkBoxClosed->checkState() == Qt::Checked ? true : false;
}

void BestFitDialog::onXShift()
{
    _needXShift = _ui->checkBoxXShift->checkState() == Qt::Checked ? true : false;
}

void BestFitDialog::onYShift()
{
    _needYShift = _ui->checkBoxYShift->checkState() == Qt::Checked ? true : false;
}

void BestFitDialog::onRotation()
{
    _needRotation = _ui->checkBoxRotation->checkState() == Qt::Checked ? true : false;
}

void BestFitDialog::onConstraints()
{
    if(_ui->checkBoxConstraints->checkState() == Qt::Checked) {
        _ui->groupBoxConstraints->setEnabled(true);
        _ui->checkBoxHConstraint->setChecked(true);
        _ui->checkBoxVConstraint->setChecked(true);
        onHorizontalConstraint();
        onVerticalConstraint();
        onRotationConstraint();
    }
    else {
        _ui->groupBoxConstraints->setEnabled(false);
        _ui->checkBoxHConstraint->setChecked(false);
        _ui->checkBoxVConstraint->setChecked(false);
        _ui->checkBoxRConstraint->setChecked(false);
        onHorizontalConstraint();
        onVerticalConstraint();
        onRotationConstraint();
    }
}

void BestFitDialog::onHorizontalConstraint()
{
    _needHConstraint = _ui->checkBoxHConstraint->checkState() == Qt::Checked ? true : false;
    if(_needHConstraint) {
        _ui->labelXShiftFrom->setEnabled(true);
        _ui->labelXShiftTo->setEnabled(true);
        _ui->lineEditXShiftFrom->setEnabled(true);
        _ui->lineEditXShiftTo->setEnabled(true);
        _ui->lineEditXShiftFrom->setText("-0.1");
        _ui->lineEditXShiftTo->setText("0.1");
    }
    else {
        _ui->labelXShiftFrom->setEnabled(false);
        _ui->labelXShiftTo->setEnabled(false);
        _ui->lineEditXShiftFrom->setEnabled(false);
        _ui->lineEditXShiftTo->setEnabled(false);
        _ui->lineEditXShiftFrom->clear();
        _ui->lineEditXShiftTo->clear();
    }
}

void BestFitDialog::onVerticalConstraint()
{
    _needVConstraint = _ui->checkBoxVConstraint->checkState() == Qt::Checked ? true : false;
    if(_needVConstraint) {
        _ui->labelYShiftFrom->setEnabled(true);
        _ui->labelYShiftTo->setEnabled(true);
        _ui->lineEditYShiftFrom->setEnabled(true);
        _ui->lineEditYShiftTo->setEnabled(true);
        _ui->lineEditYShiftFrom->setText("-0.1");
        _ui->lineEditYShiftTo->setText("0.1");
    }
    else {
        _ui->labelYShiftFrom->setEnabled(false);
        _ui->labelYShiftTo->setEnabled(false);
        _ui->lineEditYShiftFrom->setEnabled(false);
        _ui->lineEditYShiftTo->setEnabled(false);
        _ui->lineEditYShiftFrom->clear();
        _ui->lineEditYShiftTo->clear();
    }
}

void BestFitDialog::onRotationConstraint()
{
    _needRConstraint = _ui->checkBoxRConstraint->checkState() == Qt::Checked ? true : false;
    if(_needRConstraint) {
        _ui->labelRotationFrom->setEnabled(true);
        _ui->labelRotationTo->setEnabled(true);
        _ui->lineEditRotationFrom->setEnabled(true);
        _ui->lineEditRotationTo->setEnabled(true);
        _ui->lineEditRotationFrom->setText("-1");
        _ui->lineEditRotationTo->setText("1");
    }
    else {
        _ui->labelRotationFrom->setEnabled(false);
        _ui->labelRotationTo->setEnabled(false);
        _ui->lineEditRotationFrom->setEnabled(false);
        _ui->lineEditRotationTo->setEnabled(false);
        _ui->lineEditRotationFrom->clear();
        _ui->lineEditRotationTo->clear();
    }
}

void BestFitDialog::checkSettings()
{
    auto resultCurveName = _ui->lineEditResultName->text();
    auto selectedItemsOfListMeasured = _ui->listOfMeasuredCurves->selectedItems();
    auto selectedItemsOfListNominal = _ui->listOfNominalCurves->selectedItems();
    auto currentItemOfListMeasured = selectedItemsOfListMeasured.length() == 1 ? selectedItemsOfListMeasured[0] : nullptr;
    auto currentItemOfListNominal = selectedItemsOfListNominal.length() == 1 ? selectedItemsOfListNominal[0] : nullptr;

    if(currentItemOfListMeasured == nullptr || currentItemOfListNominal == nullptr) {
        _errorMessage->setText("Not enough curves. Select two curves!");
        _errorMessage->exec();
        return;
    }

    if(resultCurveName.length() == 0) {
        _errorMessage->setText("Empty line. Write result name!");
        _errorMessage->exec();
        return;
    }

    if(!isCorrectNumericLines()) {
        _errorMessage->setText("Incorrect numeric line. Check constraints!");
        _errorMessage->exec();
        return;
    }

    auto measuredCurveName = currentItemOfListMeasured->text();
    auto nominalCurveName = currentItemOfListNominal->text();
    auto project = &Project::instance();

    if(_algorithm == Function6Params::Method::Point) {
        auto nominalFigure = project->findFigure(nominalCurveName);
        auto measuredFigure = project->findFigure(measuredCurveName);
        auto nominalCurve = dynamic_cast<const CurveFigure*>(nominalFigure);
        auto measuredCurve = dynamic_cast<const CurveFigure*>(measuredFigure);

        assert(nominalCurve && measuredCurve);

        auto numberOfPointsOfNomCurve = nominalCurve->points().length();
        auto numberOfPointsOfMeasCurve = measuredCurve->points().length();
        if(numberOfPointsOfNomCurve != numberOfPointsOfMeasCurve) {
            _errorMessage->setText(QString("Point-to-point best-fit requires two curves with the same\nnumber of points(%1<>%2)! Check selected curves!")
                    .arg(numberOfPointsOfMeasCurve)
                    .arg(numberOfPointsOfNomCurve));
            _errorMessage->exec();
            return;
        }
    }

    if(project->findFigure(resultCurveName)) {
        _questionMessage->exec();
        if(_questionMessage->result() == QMessageBox::Yes) {
            calculateBestFit(nominalCurveName, measuredCurveName, resultCurveName);
        }
        else {
            return;
        }
    }
    else {
        calculateBestFit(nominalCurveName, measuredCurveName, resultCurveName);
    }
}

void BestFitDialog::calculateBestFit(const QString& nominalCurveName, const QString& measuredCurveName, const QString& resultCurveName)
{
    QApplication::setOverrideCursor(Qt::WaitCursor);

    auto xShiftFrom = _ui->lineEditXShiftFrom->text().toDouble();
    auto xShiftTo = _ui->lineEditXShiftTo->text().toDouble();
    auto yShiftFrom = _ui->lineEditYShiftFrom->text().toDouble();
    auto yShiftTo = _ui->lineEditYShiftTo->text().toDouble();
    auto rotationFrom = _ui->lineEditRotationFrom->text().toDouble();
    auto rotationTo = _ui->lineEditRotationTo->text().toDouble();
    auto params = Function6Params(true, _algorithm, _isClosed, _needXShift, _needYShift, _needRotation);

    if(_needHConstraint) {
        params.setHorizontalConstraint(xShiftFrom, xShiftTo);
    }

    if(_needVConstraint) {
        params.setVerticalConstraint(yShiftFrom, yShiftTo);
    }

    if(_needRConstraint) {
        params.setRotationConstraint(rotationFrom, rotationTo);
    }

    Algorithms::calculateBestFit(nominalCurveName, measuredCurveName, resultCurveName, resultCurveName + "_BF", &params);

    QApplication::restoreOverrideCursor();
}

bool BestFitDialog::isCorrectNumericLines()
{
    auto isCorrect = true;

    if(_ui->checkBoxConstraints->checkState() == Qt::Checked) {
        if(_needHConstraint) {
            if(!_ui->lineEditXShiftFrom->text().toDouble() || !_ui->lineEditXShiftTo->text().toDouble()) {
                isCorrect = false;
            }
        }
        if(_needVConstraint) {
            if(!_ui->lineEditYShiftFrom->text().toDouble() || !_ui->lineEditYShiftTo->text().toDouble()) {
                isCorrect = false;
            }
        }
        if(_needRConstraint) {
            if(!_ui->lineEditRotationFrom->text().toDouble() || !_ui->lineEditRotationTo->text().toDouble()) {
                isCorrect = false;
            }
        }
    }
    return isCorrect;
}

void BestFitDialog::closeEvent(QCloseEvent* event)
{
    closeDialog();
}

void BestFitDialog::closeDialog()
{
    this->close();
    _ui->listOfMeasuredCurves->clear();
    _ui->listOfNominalCurves->clear();
    _ui->lineEditResultName->clear();
    _ui->lineEditXShiftFrom->clear();
    _ui->lineEditXShiftTo->clear();
    _ui->lineEditYShiftFrom->clear();
    _ui->lineEditYShiftTo->clear();
    _ui->lineEditRotationFrom->clear();
    _ui->lineEditRotationTo->clear();
    _ui->comboBoxMethods->setCurrentIndex(0);
    _ui->checkBoxClosed->setChecked(false);
    _ui->checkBoxXShift->setChecked(true);
    _ui->checkBoxYShift->setChecked(true);
    _ui->checkBoxRotation->setChecked(true);
    _ui->checkBoxConstraints->setChecked(false);
    _ui->checkBoxHConstraint->setChecked(false);
    _ui->checkBoxVConstraint->setChecked(false);
    _ui->checkBoxRConstraint->setChecked(false);
    _ui->groupBoxConstraints->setEnabled(false);
    _ui->lineEditResultName->clearFocus();
}

BestFitDialog::~BestFitDialog()
{
    delete _ui;
}
