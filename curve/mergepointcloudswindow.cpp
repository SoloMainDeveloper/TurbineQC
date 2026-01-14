#include "curve/pch.h"
#include "mergepointcloudswindow.h"

MergePointCloudsWindow::MergePointCloudsWindow(Project *project, QWidget *parent) : _project(project), QDialog(parent), _ui(new Ui::MergePointCloudsWindow()) {
    _ui->setupUi(this);

    _curveGraphics = new CurveGraphics(_project, _ui->graphicsView);
    _message = new QMessageBox(this);
    _message->setWindowTitle("Error");

    this->setFixedSize(650, 440);
    _ui->graphicsView->setFixedSize(200, 240);
    _ui->groupBoxOptions->setMinimumHeight(67);

    connect(_ui->firstListWidgetOfCurves, &QListWidget::itemClicked, this, &MergePointCloudsWindow::changeItemOfList);
    connect(_ui->secondListWidgetOfCurves, &QListWidget::itemClicked, this, &MergePointCloudsWindow::changeItemOfList);
    connect(_ui->needSortedCheckBox, &QCheckBox::clicked, this, &MergePointCloudsWindow::sort);
    connect(_ui->mergeBtn, &QPushButton::clicked, this, &MergePointCloudsWindow::merge);
    connect(_ui->closeBtn, &QPushButton::clicked, this, &MergePointCloudsWindow::closeWindow);
}

void MergePointCloudsWindow::windowInitialization() {
    auto figures = _project->figures();
    _curveGraphics = new CurveGraphics(_project, _ui->graphicsView);

    if(figures.length() != 0) {
        for(auto figure : figures) {
            if(dynamic_cast<CurveFigure*>(figure)) {
                _ui->firstListWidgetOfCurves->addItem(figure->name());
                _ui->secondListWidgetOfCurves->addItem(figure->name());
            }
        }
    }
    this->exec();
}

void MergePointCloudsWindow::changeItemOfList() {
    auto selectedItemsOfFirstList = _ui->firstListWidgetOfCurves->selectedItems();
    auto selectedItemsOfSecondList = _ui->secondListWidgetOfCurves->selectedItems();
    auto currentItemOfFirstList = selectedItemsOfFirstList.length() == 1 ? selectedItemsOfFirstList[0] : nullptr;
    auto currentItemOfSecondList = selectedItemsOfSecondList.length() == 1 ? selectedItemsOfSecondList[0] : nullptr;
    _curveGraphics->drawCurve(currentItemOfFirstList, Qt::green, 0.1, currentItemOfSecondList, Qt::blue, 0.1);
}

void MergePointCloudsWindow::sort() {
    if(_ui->needSortedCheckBox->checkState() == Qt::Checked) {
        _ui->labelThresholdForEqualPoints->show();
        _ui->threshold->show();
    } else {
        _ui->labelThresholdForEqualPoints->hide();
        _ui->threshold->hide();
    }
}

void MergePointCloudsWindow::merge() {
    _name = _ui->resultName->text();
    auto currentItemOfFirstList = _ui->firstListWidgetOfCurves->currentItem();
    auto currentItemOfSecondList = _ui->secondListWidgetOfCurves->currentItem();
    if(currentItemOfFirstList != nullptr && currentItemOfSecondList != nullptr) {
        if(_name.length() != 0) {
            auto nameOfFirstCurve = currentItemOfFirstList->text();
            auto nameOfSecondCurve = currentItemOfSecondList->text();
            auto needSorted = _ui->needSortedCheckBox->checkState() == Qt::Checked;
            auto threshold = _ui->threshold->text().toDouble();
            auto result = Algorithms::mergePointClouds(nameOfFirstCurve, nameOfSecondCurve, threshold, needSorted, _project);

            if(result.length() != 0) {
                auto newName = _ui->resultName->text();
                auto curve = new CurveFigure(newName, result);
                if(_project->containsFigure(newName)) {
                    _project->removeFigure(newName);
                }
                _project->insertFigure(curve);
                auto firstFigure = _project->findFigure(nameOfFirstCurve);
            } else {
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

void MergePointCloudsWindow::closeEvent(QCloseEvent *event) {
    closeWindow();
}

void MergePointCloudsWindow::closeWindow() {
    this->close();
    _ui->resultName->clear();
    _ui->firstListWidgetOfCurves->clear();
    _ui->secondListWidgetOfCurves->clear();
    _ui->needSortedCheckBox->setChecked(true);
    _ui->threshold->setText("0.02");
    _ui->labelThresholdForEqualPoints->show();
    _ui->threshold->show();
}

MergePointCloudsWindow::~MergePointCloudsWindow() {
    delete _ui;
}
