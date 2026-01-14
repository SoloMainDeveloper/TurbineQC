#include "curve/pch.h"
#include "airfoiltolerancesdialog.h"

AirfoilTolerancesDialog::AirfoilTolerancesDialog(Project* mainProject, QWidget *parent)
    : QDialog(parent)
    , _ui(new Ui::AirfoilTolerancesDialogClass())
{
    _ui->setupUi(this);

    widgetsInitializaion();
    
    setWindowTitle("Assign Tolerances to Nominal");

    _project = mainProject;
    connect(this, &AirfoilTolerancesDialog::curveChangeToleranceRequested, _project, &Project::changeCurveTolerance);

    _ui->listWidgetCurves->setFixedWidth(150);
    connect(_ui->listWidgetCurves, &QListWidget::currentItemChanged, this, &AirfoilTolerancesDialog::changeCurrentMode);

    _curveGraphics = new CurveGraphicsWidget();
    _containerLayout = new QGridLayout(_ui->container);
    _containerLayout->setContentsMargins(0, 0, 0, 0);
    _containerLayout->addWidget(_curveGraphics, 0, 0);
    connect(_ui->listWidgetCurves, &QListWidget::itemClicked, this, &AirfoilTolerancesDialog::changeItemOfList);

    _ui->comboBoxMode->addItem("Constant tolerance");
    _ui->comboBoxMode->addItem("Edges");
    connect(_ui->comboBoxMode, &QComboBox::currentTextChanged, this, &AirfoilTolerancesDialog::changeCurrentMode);

    _ui->horizontalLayout->addLayout(_stackedLayoutModes);
    _stackedLayoutModes->addWidget(_constantTolerancesWidget);
    _stackedLayoutModes->addWidget(_edgesWidget);

    _constantTolerancesWidget->setLayout(_constantTolerancesLayout);
    _constantTolerancesLayout->addStretch();
    addToleranceBlock(_constantTolerancesLayout, "Tolerance (Upper / Lower)", _toleranceLabel, _upperToleranceLineEdit, _lowerToleranceLineEdit);
    _constantTolerancesLayout->addStretch();
    addButtonBox(_constantTolerancesLayout, _constantToleranceButtonBox);

    _edgesWidget->setLayout(_edgesLayout);
    _edgesLayout->addStretch();
    addToleranceBlock(_edgesLayout, "Leading edge tolerance (Upper / Lower)", _leadingEdgeToleranceLabel,
        _leadingEdgeUpperToleranceLineEdit, _leadingEdgeLowerToleranceLineEdit);
    addToleranceBlock(_edgesLayout, "Trailing edge tolerance (Upper / Lower)", _trailingEdgeToleranceLabel,
        _trailingEdgeUpperToleranceLineEdit, _trailingEdgeLowerToleranceLineEdit);
    addToleranceBlock(_edgesLayout, "High edge tolerance (Upper / Lower)", _highEdgeToleranceLabel,
        _highEdgeUpperToleranceLineEdit , _highEdgeLowerToleranceLineEdit);
    addToleranceBlock(_edgesLayout, "Low edge tolerance (Upper / Lower)", _lowEdgeToleranceLabel,
        _lowEdgeUpperToleranceLineEdit, _lowEdgeLowerToleranceLineEdit);
    _edgesLayout->addStretch();
    _edgesLayout->addWidget(_edgesGroupBox);
    _edgesLayout->addStretch();
    addButtonBox(_edgesLayout, _edgesButtonBox);

    _edgesGroupBox->setLayout(_edgesGroupBoxLayout);
    _leadingEdgeDirectionComboBox->addItem("Auto");
    _leadingEdgeDirectionComboBox->addItem("+X");
    _leadingEdgeDirectionComboBox->addItem("-X");
    _leadingEdgeDirectionComboBox->addItem("+Y");
    _leadingEdgeDirectionComboBox->addItem("-Y");
    _leadingEdgePercentLineEdit->setValidator(new QDoubleValidator);
    _trailingEdgePercentLineEdit->setValidator(new QDoubleValidator);
    _edgesGroupBoxLayout->addWidget(_leadingEdgeDirectionLabel, 0, 0);
    _edgesGroupBoxLayout->addWidget(_leadingEdgeDirectionComboBox, 0, 1);
    _edgesGroupBoxLayout->addWidget(_edgesPercentsLabel, 1, 0);
    _edgesGroupBoxLayout->addWidget(_leadingEdgePercentLineEdit, 1, 1);
    _edgesGroupBoxLayout->addWidget(_trailingEdgePercentLineEdit, 1, 2);
}

void AirfoilTolerancesDialog::dialogInitialization() {
    _ui->listWidgetCurves->clear(); 
    _ui->listWidgetCurves->setCurrentItem(nullptr);
    _currentCurveName = nullptr;
    _currentCurve = nullptr;

    auto projectCurrentCurveName = _project->currentFigureName();
    auto figures = _project->figures();
    _curveGraphics->initialization();

    for(auto item : figures) {
        if(dynamic_cast<CurveFigure*>(item)) {
            auto name = item->name();
            auto listItem = new QListWidgetItem(name);
            _ui->listWidgetCurves->addItem(listItem);
        }
    }
    if(_ui->listWidgetCurves->findItems(projectCurrentCurveName, Qt::MatchExactly).length() == 1) {
        _ui->listWidgetCurves->setCurrentItem(_ui->listWidgetCurves->findItems(projectCurrentCurveName, Qt::MatchExactly)[0]);
    } else {
        changeCurrentMode();
    }
    exec();
}

void AirfoilTolerancesDialog::changeCurrentMode() {
    auto currentMode = _ui->comboBoxMode->currentText();

    if(currentMode == "Constant tolerance") {
        _stackedLayoutModes->setCurrentWidget(_constantTolerancesWidget);

        checkCurrentItem(_constantToleranceButtonBox);

        if(_currentCurve) {
            _upperToleranceLineEdit->setEnabled(true);
            _lowerToleranceLineEdit->setEnabled(true);
        } else {
            _upperToleranceLineEdit->setEnabled(false);
            _lowerToleranceLineEdit->setEnabled(false);
        }
        _upperToleranceLineEdit->setText("0.1");
        _lowerToleranceLineEdit->setText("-0.1");
    } else if(currentMode == "Edges") {
        _stackedLayoutModes->setCurrentWidget(_edgesWidget);

        checkCurrentItem(_edgesButtonBox);

        if(_currentCurve) {
            edgesLayoutSetEnabled(true);
        } else {
            edgesLayoutSetEnabled(false);
        }

        _leadingEdgeDirectionComboBox->setCurrentIndex(0);
        _leadingEdgePercentLineEdit->setText("5");
        _trailingEdgePercentLineEdit->setText("5");

    }
    adjustSize();

}

void AirfoilTolerancesDialog::changeItemOfList() {
    auto selectedItemsOfCurves = _ui->listWidgetCurves->selectedItems();
    auto currFigure = selectedItemsOfCurves.length() == 1 ? selectedItemsOfCurves[0] : nullptr;
    _curveGraphics->drawCurve(_project, currFigure, Qt::blue, 0.1);
}

void AirfoilTolerancesDialog::assignTolerances() {
    if(!_currentCurve) {
        auto text = QMessageBox::warning(this, "Curve not selected", "Select curve", "Ok");
    } else {
        auto currentMode = _ui->comboBoxMode->currentText();
        if(currentMode == "Constant tolerance") {
            auto newUpperTolerance = _upperToleranceLineEdit->text().toDouble();
            auto newLowerTolerance = _lowerToleranceLineEdit->text().toDouble();
            auto curveWithTolerances = *_currentCurve;
            curveWithTolerances.assignToleranceToSegment(newUpperTolerance, newLowerTolerance);

            auto f1params = Function1Params(0, 0, 0, true, true, FunctionParams::Direction::Left, true);
            auto f1result = CurveLibrary::function1(curveWithTolerances.points(), f1params);

            emit curveChangeToleranceRequested(_currentCurveName, f1result.curve.points());

        } else if(currentMode == "Edges") {
            auto lEDir = 0;
            auto lEDirText = _leadingEdgeDirectionComboBox->currentText();
            if(lEDirText == "Auto") {
                lEDir = 0;
            } else if(lEDirText == "+X") {
                lEDir = 1;
            } else if(lEDirText == "-X") {
                lEDir = 3;
            } else if(lEDirText == "+Y") {
                lEDir = 2;
            } else if(lEDirText == "-Y") {
                lEDir = 4;
            }
            auto lEPercent = _leadingEdgePercentLineEdit->text().toDouble();
            auto tEPercent = _trailingEdgePercentLineEdit->text().toDouble();
            auto const f18params = Function18Params(lEDir, lEPercent, tEPercent);
            auto f18result = CurveLibrary::function18(_currentCurve->points(), f18params);

            auto lEUpper = _leadingEdgeUpperToleranceLineEdit->text().toDouble();
            auto lELower = _leadingEdgeLowerToleranceLineEdit->text().toDouble();
            auto tEUpper = _trailingEdgeUpperToleranceLineEdit->text().toDouble();
            auto tELower = _trailingEdgeLowerToleranceLineEdit->text().toDouble();
            auto highEUpper = _highEdgeUpperToleranceLineEdit->text().toDouble();
            auto highELower = _highEdgeLowerToleranceLineEdit->text().toDouble();
            auto lowEUpper = _lowEdgeUpperToleranceLineEdit->text().toDouble();
            auto lowELower = _lowEdgeLowerToleranceLineEdit->text().toDouble();

            f18result.curveLE.assignToleranceToSegment(lEUpper, lELower);
            f18result.curveTE.assignToleranceToSegment(tEUpper, tELower);
            f18result.curveHigh.assignToleranceToSegment(highEUpper, highELower);
            f18result.curveLow.assignToleranceToSegment(lowEUpper, lowELower);

            QVector<CurvePoint> pointsWithTolerances;
            if(f18result.curveLE.points().length() + f18result.curveHigh.points().length() +
                f18result.curveTE.points().length() + f18result.curveLow.points().length() == _currentCurve->points().length() + 8) {
                auto curveLEPoints = f18result.curveLE.points();
                curveLEPoints.pop_back();
                curveLEPoints.pop_front();
                auto curveTEPoints = f18result.curveTE.points();
                curveTEPoints.pop_back();
                curveTEPoints.pop_front();
                auto curveHighPoints = f18result.curveHigh.points();
                curveHighPoints.pop_back();
                curveHighPoints.pop_front();
                auto curveLowPoints = f18result.curveLow.points();
                curveLowPoints.pop_back();
                curveLowPoints.pop_front();

                pointsWithTolerances = curveLEPoints + curveTEPoints + curveHighPoints + curveLowPoints;
            } else {
                pointsWithTolerances = f18result.curveLE.points() + f18result.curveHigh.points() +
                    f18result.curveTE.points() + f18result.curveLow.points();
            }

            auto f1params = Function1Params(0, 0, 0, true, true, FunctionParams::Direction::Left, true);
            auto f1result = CurveLibrary::function1(pointsWithTolerances, f1params);

            emit curveChangeToleranceRequested(_currentCurveName, f1result.curve.points());
        }
        accept();
    }
}

void AirfoilTolerancesDialog::addToleranceBlock(QVBoxLayout* layout, QString blockName, QLabel* label, QLineEdit* upperLineEdit, QLineEdit* lowerLineEdit) {
    label->setText(blockName);
    layout->addWidget(label);
    QHBoxLayout* blockLayout = new QHBoxLayout;
    blockLayout->addWidget(upperLineEdit);
    blockLayout->addWidget(lowerLineEdit);
    upperLineEdit->setValidator(new QDoubleValidator);
    lowerLineEdit->setValidator(new QDoubleValidator);
    layout->addLayout(blockLayout);
}

void AirfoilTolerancesDialog::addButtonBox(QVBoxLayout* layout, QDialogButtonBox* buttonBox) {
    buttonBox->setCenterButtons(true);
    buttonBox->addButton(QDialogButtonBox::Ok);
    buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    buttonBox->addButton(QDialogButtonBox::Cancel);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &AirfoilTolerancesDialog::assignTolerances);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    layout->addWidget(buttonBox);
}

void AirfoilTolerancesDialog::widgetsInitializaion() {
    _stackedLayoutModes = new QStackedLayout;
    _constantToleranceButtonBox = new QDialogButtonBox;
    _edgesButtonBox = new QDialogButtonBox;
    _constantTolerancesWidget = new QWidget;
    _constantTolerancesLayout = new QVBoxLayout;
    _toleranceLabel = new QLabel;
    _upperToleranceLineEdit = new QLineEdit;
    _lowerToleranceLineEdit = new QLineEdit;
    _edgesWidget = new QWidget;
    _edgesLayout = new QVBoxLayout;
    _leadingEdgeToleranceLabel = new QLabel;
    _trailingEdgeToleranceLabel = new QLabel;
    _highEdgeToleranceLabel = new QLabel;
    _lowEdgeToleranceLabel = new QLabel;
    _leadingEdgeUpperToleranceLineEdit = new QLineEdit;
    _leadingEdgeLowerToleranceLineEdit = new QLineEdit;
    _trailingEdgeUpperToleranceLineEdit = new QLineEdit;
    _trailingEdgeLowerToleranceLineEdit = new QLineEdit;
    _highEdgeUpperToleranceLineEdit = new QLineEdit;
    _highEdgeLowerToleranceLineEdit = new QLineEdit;
    _lowEdgeUpperToleranceLineEdit = new QLineEdit;
    _lowEdgeLowerToleranceLineEdit = new QLineEdit;
    _edgesGroupBox = new QGroupBox("Edges");
    _leadingEdgeDirectionLabel = new QLabel("Leading edge direction");
    _edgesPercentsLabel = new QLabel("LE / TE, %");
    _leadingEdgeDirectionComboBox = new QComboBox;
    _leadingEdgePercentLineEdit = new QLineEdit;
    _trailingEdgePercentLineEdit = new QLineEdit;
    _edgesGroupBoxLayout = new QGridLayout;
}

void AirfoilTolerancesDialog::checkCurrentItem(QDialogButtonBox* buttonBox) {
    if(auto currentCurveItem = _ui->listWidgetCurves->currentItem()) {
        _currentCurveName = currentCurveItem->text();
        auto currentFigure = _project->findFigure(_currentCurveName);
        if(currentFigure) {
            _currentCurve = dynamic_cast<const CurveFigure*>(currentFigure);
            buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
        } else {
            _currentCurve = nullptr;
            buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
        }      
    } else {
        _currentCurve = nullptr;
        buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    }
}

void AirfoilTolerancesDialog::edgesLayoutSetEnabled(bool state) {
    _leadingEdgeUpperToleranceLineEdit->setEnabled(state);
    _leadingEdgeLowerToleranceLineEdit->setEnabled(state);
    _trailingEdgeUpperToleranceLineEdit->setEnabled(state);
    _trailingEdgeLowerToleranceLineEdit->setEnabled(state);
    _highEdgeUpperToleranceLineEdit->setEnabled(state);
    _highEdgeLowerToleranceLineEdit->setEnabled(state);
    _lowEdgeUpperToleranceLineEdit->setEnabled(state);
    _lowEdgeLowerToleranceLineEdit->setEnabled(state);
    _leadingEdgeDirectionComboBox->setEnabled(state);
    _leadingEdgePercentLineEdit->setEnabled(state);
    _trailingEdgePercentLineEdit->setEnabled(state);

    _leadingEdgeUpperToleranceLineEdit->setText("0.1");
    _leadingEdgeLowerToleranceLineEdit->setText("-0.1");
    _trailingEdgeUpperToleranceLineEdit->setText("0.1");
    _trailingEdgeLowerToleranceLineEdit->setText("-0.1");
    _highEdgeUpperToleranceLineEdit->setText("0.1");
    _highEdgeLowerToleranceLineEdit->setText("-0.1");
    _lowEdgeUpperToleranceLineEdit->setText("0.1");
    _lowEdgeLowerToleranceLineEdit->setText("-0.1");
}

AirfoilTolerancesDialog::~AirfoilTolerancesDialog() {
    delete _ui;
}
