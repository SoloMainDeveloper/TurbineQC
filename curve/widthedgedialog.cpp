#include "curve/pch.h"

#include "widthedgedialog.h"
#include "ui_widthedgedialog.h"

WidthEdgeDialog::WidthEdgeDialog() : _ui(new Ui::WidthEdgeDialog()) {
    _ui->setupUi(this);
    _curveGraphics = new CurveGraphicsWidget();
    _containerLayout = new QGridLayout(_ui->container);
    _containerLayout->setContentsMargins(0, 0, 0, 0);
    _containerLayout->addWidget(_curveGraphics, 0, 0);
    setupWindow();
    connect(_ui->closeButton, &QPushButton::clicked, this, &WidthEdgeDialog::closeWindow);
    connect(_ui->calculateButton, &QPushButton::clicked, this, &WidthEdgeDialog::calculateWidthEdge);
    connect(checkBoxLE, &QCheckBox::clicked, this, &WidthEdgeDialog::checkBoxLEStateChange);
    connect(checkBoxTE, &QCheckBox::clicked, this, &WidthEdgeDialog::checkBoxTEStateChange);
    connect(curves, &QListWidget::itemClicked, this, &WidthEdgeDialog::updateAnswerView);
}

void WidthEdgeDialog::initialize() {
    auto figures = Project::instance().figures();
    _curveGraphics->initialization();

    for(auto item : figures) {
        if(dynamic_cast<CurveFigure*>(item)) {
            auto name = item->name();
            curves->addItem(new QListWidgetItem(name));
        }
    }
    exec();
}

void WidthEdgeDialog::setupWindow() {
    _doubleValidator = new QDoubleValidator(); // TODO: memory leak
    _doubleValidator->setDecimals(9);
    _doubleValidator->setLocale(QLocale::C);
    _doubleValidator->setNotation(QDoubleValidator::StandardNotation);

    curves = _ui->curves;
    directionLE = _ui->directionLEComboBox;
    checkBoxLE = _ui->checkBoxLE;
    checkBoxTE = _ui->checkBoxTE;
    distanceLE = _ui->distanceLE;
    distanceLE->setValidator(_doubleValidator);
    distanceTE = _ui->distanceTE;
    distanceTE->setValidator(_doubleValidator);
    groupBoxLE = _ui->groupBoxLE;
    groupBoxTE = _ui->groupBoxTE;
    answerLE = _ui->answerLE;
    answerTE = _ui->answerTE;
}

void WidthEdgeDialog::closeWindow() {
    close();
    curves->clear();
    directionLE->setCurrentIndex(0);
    if(!checkBoxLE->isChecked()) {
        checkBoxLE->click();
    }
    if(!checkBoxTE->isChecked()) {
        checkBoxTE->click();
    }
    distanceLE->setText("1");
    distanceTE->setText("1");
    answerLE->clear();
    answerTE->clear();
    _ui->createDistanceCheckBoxLE->setChecked(false);
    _ui->createDistanceCheckBoxTE->setChecked(false);
    _ui->modeLE->setCurrentIndex(0);
    _ui->modeTE->setCurrentIndex(0);
}

void WidthEdgeDialog::calculateWidthEdge() {
    auto selectedItemsOfCurves = curves->selectedItems();
    auto currFigure = selectedItemsOfCurves.length() == 1 ? selectedItemsOfCurves[0] : nullptr;
    if(currFigure != nullptr) {
        auto distanceLEValue = distanceLE->text().toDouble() ? distanceLE->text().toDouble() : 1;
        auto distanceTEValue = distanceTE->text().toDouble() ? distanceTE->text().toDouble() : 1;
        auto widthLE = 0.0;
        auto widthTE = 0.0;
        auto currentFigure = currFigure->text();
        auto params18 = new Function18Params();
        if(_ui->checkBoxLE) {
            if(_ui->createDistanceCheckBoxLE->isChecked()) {
                auto param = WidthLE(0, 0, 0, QString::number(distanceLEValue));
                param.createMeasured(currentFigure, currentFigure, Function18Params(), &Project::instance());
                widthLE = param.measured;
            } else {
                auto result = Algorithms::getWidthOfLeadingEdge(currentFigure, params18, distanceLEValue, &Project::instance());
                auto [firstPoint, secondPoint] = result;
                widthLE = Algorithms::getDistanceBetweenPoints(Point(firstPoint), Point(secondPoint));
            }
        }
        if(_ui->checkBoxTE) {
            if(_ui->createDistanceCheckBoxTE->isChecked()) {
                auto param = WidthTE(0, 0, 0, QString::number(distanceTEValue));
                param.createMeasured(currentFigure, currentFigure, Function18Params(), &Project::instance());
                widthTE = param.measured;
            } else {
                auto result = Algorithms::getWidthOfTrailingEdge(currentFigure, params18, distanceTEValue, &Project::instance());
                auto [firstPoint, secondPoint] = result;
                widthTE = Algorithms::getDistanceBetweenPoints(Point(firstPoint), Point(secondPoint));
            }
        }
        _widths[currFigure->text()] = new EdgeWidth(widthLE, distanceLEValue, widthTE, distanceTEValue);
        updateAnswerView();
    }
}

void WidthEdgeDialog::updateAnswerView() {
    auto selectedItemsOfCurves = curves->selectedItems();
    auto currentItem = selectedItemsOfCurves.length() == 1 ? selectedItemsOfCurves[0] : nullptr;
    if(currentItem) {
        auto figure = Project::instance().findFigure(currentItem->text());
        auto curve = dynamic_cast<const CurveFigure*>(figure);
        assert(curve);
        _curveGraphics->drawCurve(curve, Qt::blue, 0.1);
        if(_widths.contains(currentItem->text())) {
            auto figureName = currentItem->text();
            answerLE->setText(QString::number(_widths[figureName]->widthLE));
            distanceLE->setText(QString::number(_widths[figureName]->distanceLE));
            answerTE->setText(QString::number(_widths[figureName]->widthTE));
            distanceTE->setText(QString::number(_widths[figureName]->distanceTE));
        } else {
            answerLE->clear();
            answerTE->clear();
        }
    }
}

void WidthEdgeDialog::closeEvent(QCloseEvent *event) {
    closeWindow();
}

void WidthEdgeDialog::checkBoxLEStateChange() {
    auto state = checkBoxLE->isChecked();
    if(state) {
        groupBoxLE->show();
    } else {
        groupBoxLE->hide();
    }
}

void WidthEdgeDialog::checkBoxTEStateChange() {
    auto state = checkBoxTE->isChecked();
    if(state) {
        groupBoxTE->show();
    } else {
        groupBoxTE->hide();
    }
}

WidthEdgeDialog::~WidthEdgeDialog() {
    delete _ui;
}

EdgeWidth::EdgeWidth(double widthLeadingEdge, double distLE, double widthTrailingEdge, double distTE) {
    widthLE = widthLeadingEdge;
    distanceLE = distLE;
    widthTE = widthTrailingEdge;
    distanceTE = distTE;
}

EdgeWidth::~EdgeWidth() {
}
