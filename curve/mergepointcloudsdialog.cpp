#include "curve/pch.h"

#include "mergepointcloudsdialog.h"
#include "ui_mergepointcloudsdialog.h"
#include "algorithms.h"

MergePointCloudsDialog::MergePointCloudsDialog() : _ui(new Ui::MergePointCloudsDialog()) {
    _ui->setupUi(this);

    _project = &Project::instance();
    _curveGraphics = new CurveGraphicsWidget();
    _containerLayout = new QGridLayout(_ui->container);
    _containerLayout->setContentsMargins(0, 0, 0, 0);
    _containerLayout->addWidget(_curveGraphics, 0, 0);
    _message = new QMessageBox(this);
    _message->setWindowTitle("Error");
    this->setFixedSize(650, 440);
    _ui->groupBoxOptions->setMinimumHeight(67);
    connect(_ui->firstListWidgetOfCurves, &QListWidget::itemClicked, this, &MergePointCloudsDialog::changeItemOfList);
    connect(_ui->secondListWidgetOfCurves, &QListWidget::itemClicked, this, &MergePointCloudsDialog::changeItemOfList);
    connect(_ui->needSortedCheckBox, &QCheckBox::clicked, this, &MergePointCloudsDialog::sort);
    connect(_ui->mergeBtn, &QPushButton::clicked, this, &MergePointCloudsDialog::merge);
    connect(_ui->closeBtn, &QPushButton::clicked, this, &MergePointCloudsDialog::closeWindow);
}

void MergePointCloudsDialog::initialize() {
    auto figures = Project::instance().figures();
    _curveGraphics->initialization();

    if(figures.size() != 0) {
        for(auto figure : figures) {
            if(dynamic_cast<CurveFigure*>(figure)) {
                _ui->firstListWidgetOfCurves->addItem(figure->name());
                _ui->secondListWidgetOfCurves->addItem(figure->name());
            }
        }
    }
    this->exec();
}

void MergePointCloudsDialog::changeItemOfList() {
    auto selectedItemsOfFirstList = _ui->firstListWidgetOfCurves->selectedItems();
    auto selectedItemsOfSecondList = _ui->secondListWidgetOfCurves->selectedItems();
    auto currentItemOfFirstList = selectedItemsOfFirstList.length() == 1 ? selectedItemsOfFirstList[0] : nullptr;
    auto currentItemOfSecondList = selectedItemsOfSecondList.length() == 1 ? selectedItemsOfSecondList[0] : nullptr;
    if(currentItemOfFirstList && !currentItemOfSecondList) {
        auto figure = _project->findFigure(currentItemOfFirstList->text());
        auto curve = dynamic_cast<const CurveFigure*>(figure);
        assert(curve);
        _curveGraphics->drawCurve(curve, Qt::green, 0.1);
    } else if(!currentItemOfFirstList && currentItemOfSecondList) {
        auto figure = _project->findFigure(currentItemOfSecondList->text());
        auto curve = dynamic_cast<const CurveFigure*>(figure);
        assert(curve);
        _curveGraphics->drawCurve(curve, Qt::blue, 0.1);
    } else if(currentItemOfFirstList && currentItemOfSecondList) {
        auto firstFigure = _project->findFigure(currentItemOfFirstList->text());
        auto firstCurve = dynamic_cast<const CurveFigure*>(firstFigure);
        auto secondFigure = _project->findFigure(currentItemOfSecondList->text());
        auto secondCurve = dynamic_cast<const CurveFigure*>(secondFigure);
        assert(firstCurve && secondCurve);
        _curveGraphics->drawCurve(firstCurve, Qt::green, 0.1, secondCurve, Qt::blue, 0.1);
    }
}

void MergePointCloudsDialog::sort() {
    if(_ui->needSortedCheckBox->checkState() == Qt::Checked) {
        _ui->labelThresholdForEqualPoints->show();
        _ui->threshold->show();
    } else {
        _ui->labelThresholdForEqualPoints->hide();
        _ui->threshold->hide();
    }
}

void MergePointCloudsDialog::merge() {
    _name = _ui->resultName->text();
    auto currentItemOfFirstList = _ui->firstListWidgetOfCurves->currentItem();
    auto currentItemOfSecondList = _ui->secondListWidgetOfCurves->currentItem();
    if(currentItemOfFirstList != nullptr && currentItemOfSecondList != nullptr) {
        if(_name.length() != 0) {
            auto nameOfFirstCurve = currentItemOfFirstList->text();
            auto nameOfSecondCurve = currentItemOfSecondList->text();
            auto needSorted = _ui->needSortedCheckBox->checkState() == Qt::Checked;
            auto threshold = _ui->threshold->text().toDouble();
            auto resultName = _ui->resultName->text();
            auto status = Algorithms::tryMergePointClouds(nameOfFirstCurve, nameOfSecondCurve, resultName,
                threshold, needSorted);
            if(!status) {
                _message->setText("No intersection points found. Check selected curves!");
                _message->exec();
            }
        } else {
            _message->setText("Empty line. Write result name!");
            _message->exec();
        }
    } else {
        _message->setText("Not enough curves. Select two curves!");
        _message->exec();
    }
}

void MergePointCloudsDialog::closeEvent(QCloseEvent *event) {
    closeWindow();
}

void MergePointCloudsDialog::closeWindow() {
    this->close();
    _ui->resultName->clear();
    _ui->firstListWidgetOfCurves->clear();
    _ui->secondListWidgetOfCurves->clear();
    _ui->needSortedCheckBox->setChecked(true);
    _ui->threshold->setText("0.02");
    _ui->labelThresholdForEqualPoints->show();
    _ui->threshold->show();
}

MergePointCloudsDialog::~MergePointCloudsDialog() {
    delete _ui;
}
