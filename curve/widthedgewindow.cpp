#include "curve/pch.h"
#include "widthedgewindow.h"

WidthEdgeWindow::WidthEdgeWindow(Project *project, QWidget *parent) : QDialog(parent), _ui(new Ui::WidthEdgeWindow()) {
    _ui->setupUi(this);
    _project = project;
    _curveGraphics = new CurveGraphicsWidget();
    _containerLayout = new QGridLayout(_ui->container);
    _containerLayout->setContentsMargins(0, 0, 0, 0);
    _containerLayout->addWidget(_curveGraphics, 0, 0);
    setupWindow();
    connect(_ui->closeButton, &QPushButton::clicked, this, &WidthEdgeWindow::closeWindow);
    connect(_ui->calculateButton, &QPushButton::clicked, this, &WidthEdgeWindow::calculateWidthEdge);
    connect(checkBoxLE, &QCheckBox::clicked, this, &WidthEdgeWindow::checkBoxLEStateChange);
    connect(checkBoxTE, &QCheckBox::clicked, this, &WidthEdgeWindow::checkBoxTEStateChange);
    connect(curves, &QListWidget::itemClicked, this, &WidthEdgeWindow::updateAnswerView);
}

void WidthEdgeWindow::initialization() {
    auto figures = _project->figures();
    _curveGraphics->initialization();

    for(auto item : figures) {
        if(dynamic_cast<CurveFigure*>(item)) {
            auto name = item->name();
            curves->addItem(new QListWidgetItem(name));
        }
    }
    exec();
}

void WidthEdgeWindow::setupWindow() {
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

void WidthEdgeWindow::closeWindow() {
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

void WidthEdgeWindow::calculateWidthEdge() {
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
                param.createMeasured(currentFigure, currentFigure, Function18Params(), _project);
                widthLE = param.measured;
            } else {
                auto result = Algorithms::getWidthOfLeadingEdge(currentFigure, params18, distanceLEValue, _project);
                auto [firstPoint, secondPoint] = result;
                widthLE = Algorithms::getDistanceBetweenPoints(Point(firstPoint), Point(secondPoint));
            }
        }
        if(_ui->checkBoxTE) {
            if(_ui->createDistanceCheckBoxTE->isChecked()) {
                auto param = WidthTE(0, 0, 0, QString::number(distanceTEValue));
                param.createMeasured(currentFigure, currentFigure, Function18Params(), _project);
                widthTE = param.measured;
            } else {
                auto result = Algorithms::getWidthOfTrailingEdge(currentFigure, params18, distanceTEValue, _project);
                auto [firstPoint, secondPoint] = result;
                widthTE = Algorithms::getDistanceBetweenPoints(Point(firstPoint), Point(secondPoint));
            }
        }
        _widths[currFigure->text()] = new EdgeWidth(widthLE, distanceLEValue, widthTE, distanceTEValue);
        updateAnswerView();
    }
}

void WidthEdgeWindow::updateAnswerView() {
    auto selectedItemsOfCurves = curves->selectedItems();
    auto currentItem = selectedItemsOfCurves.length() == 1 ? selectedItemsOfCurves[0] : nullptr;
    if(currentItem) {
        auto figure = _project->findFigure(currentItem->text());
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

void WidthEdgeWindow::closeEvent(QCloseEvent *event) {
    closeWindow();
}

void WidthEdgeWindow::checkBoxLEStateChange() {
    auto state = checkBoxLE->isChecked();
    if(state) {
        groupBoxLE->show();
    } else {
        groupBoxLE->hide();
    }
}

void WidthEdgeWindow::checkBoxTEStateChange() {
    auto state = checkBoxTE->isChecked();
    if(state) {
        groupBoxTE->show();
    } else {
        groupBoxTE->hide();
    }
}

WidthEdgeWindow::~WidthEdgeWindow() {
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
