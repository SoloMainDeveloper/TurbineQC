#include "curve/pch.h"

#include "algorithms.h"
#include "mergepointcloudsdialog.h"
#include "ui_mergepointcloudsdialog.h"

MergePointCloudsDialog::MergePointCloudsDialog()
    : _ui(new Ui::MergePointCloudsDialog())
{
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
    connect(_ui->firstListWidgetOfCurves, &QListWidget::currentItemChanged, this, &MergePointCloudsDialog::changeItemOfFirstList);
    connect(_ui->secondListWidgetOfCurves, &QListWidget::currentItemChanged, this, &MergePointCloudsDialog::changeItemOfSecondList);
    connect(_ui->needSortedCheckBox, &QCheckBox::clicked, this, &MergePointCloudsDialog::sort);
    connect(_ui->mergeBtn, &QPushButton::clicked, this, &MergePointCloudsDialog::merge);
    connect(_ui->closeBtn, &QPushButton::clicked, this, &MergePointCloudsDialog::closeWindow);
}

void MergePointCloudsDialog::initialize()
{
    _curveGraphics->initialization();

    updateFigures();

    this->exec();
}

void MergePointCloudsDialog::updateFigures()
{
    auto figures = Project::instance().figures();

    _ui->firstListWidgetOfCurves->clear();
    _ui->secondListWidgetOfCurves->clear();

    if(figures.size() != 0) {
        for(auto figure : figures) {
            if(dynamic_cast<CurveFigure*>(figure)) {
                _ui->firstListWidgetOfCurves->addItem(figure->name());
                _ui->secondListWidgetOfCurves->addItem(figure->name());
            }
        }
    }
}

void MergePointCloudsDialog::changeItemOfFirstList(QListWidgetItem* currentFirstScan)
{
    if(currentFirstScan) {
        autoSelectSimilarInSecondList(currentFirstScan->text());
    }
}

void MergePointCloudsDialog::changeItemOfSecondList(QListWidgetItem* currentSecondScan)
{
    auto selectedItemsOfFirstList = _ui->firstListWidgetOfCurves->currentItem();
    if(selectedItemsOfFirstList && currentSecondScan) {
        auto firstFigure = _project->findFigure(selectedItemsOfFirstList->text());
        auto firstCurve = dynamic_cast<const CurveFigure*>(firstFigure);

        auto secondFigure = _project->findFigure(currentSecondScan->text());
        auto secondCurve = dynamic_cast<const CurveFigure*>(secondFigure);

        _curveGraphics->drawCurve(firstCurve, Qt::green, 0.1, secondCurve, Qt::blue, 0.1);
    }
}

void MergePointCloudsDialog::sort()
{
    if(_ui->needSortedCheckBox->checkState() == Qt::Checked) {
        _ui->labelThresholdForEqualPoints->show();
        _ui->threshold->show();
    }
    else {
        _ui->labelThresholdForEqualPoints->hide();
        _ui->threshold->hide();
    }
}

void MergePointCloudsDialog::merge()
{
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
            auto status = BladeGeometryService::tryMergePointClouds(nameOfFirstCurve, nameOfSecondCurve, resultName,
                threshold, needSorted);

            if(!status) {
                _message->setText("No intersection points found. Check selected curves!");
                _message->exec();
            }

            updateFigures();
        }
        else {
            _message->setText("Empty line. Write result name!");
            _message->exec();
        }
    }
    else {
        _message->setText("Not enough curves. Select two curves!");
        _message->exec();
    }
}

void MergePointCloudsDialog::closeEvent(QCloseEvent* event)
{
    closeWindow();
}

void MergePointCloudsDialog::closeWindow()
{
    this->close();
    _ui->resultName->clear();
    _ui->firstListWidgetOfCurves->clear();
    _ui->secondListWidgetOfCurves->clear();
    _ui->needSortedCheckBox->setChecked(true);
    _ui->threshold->setText("0.02");
    _ui->labelThresholdForEqualPoints->show();
    _ui->threshold->show();
}

int MergePointCloudsDialog::calculatePrefixMatchLength(const QString& str1, const QString& str2)
{
    int minLength = qMin(str1.length(), str2.length());
    int matchLength = 0;

    for(int i = 0; i < minLength; ++i) {
        if(str1[i] == str2[i]) {
            matchLength++;
        }
        else {
            break;
        }
    }

    return matchLength;
}

QString MergePointCloudsDialog::findMostSimilarByPrefix(const QString& selectedItem, const QStringList& items)
{
    int bestMatchLength = -1;
    QString bestMatch;

    for(const QString& item : items) {
        if(item == selectedItem) {
            continue;
        }

        int matchLength = calculatePrefixMatchLength(selectedItem, item);

        if(matchLength > bestMatchLength) {
            bestMatchLength = matchLength;
            bestMatch = item;
        }
    }

    return bestMatch;
}

void MergePointCloudsDialog::autoSelectSimilarInSecondList(const QString& selectedFromFirst)
{
    if(selectedFromFirst.isEmpty()) {
        return;
    }

    QStringList itemsInSecondList;
    for(int i = 0; i < _ui->secondListWidgetOfCurves->count(); ++i) {
        itemsInSecondList.append(_ui->secondListWidgetOfCurves->item(i)->text());
    }

    QString bestMatch = findMostSimilarByPrefix(selectedFromFirst, itemsInSecondList);

    if(!bestMatch.isEmpty()) {
        QList<QListWidgetItem*> foundItems = _ui->secondListWidgetOfCurves->findItems(bestMatch, Qt::MatchExactly);
        if(!foundItems.isEmpty()) {
            _ui->secondListWidgetOfCurves->setCurrentItem(foundItems.first());
        }
    }
}

MergePointCloudsDialog::~MergePointCloudsDialog()
{
    delete _ui;
}
