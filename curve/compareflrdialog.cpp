#include "curve/pch.h"

#include "algorithms.h"
#include "compareflrdialog.h"
#include "ui_compareflrdialog.h"

CompareFLRDialog::CompareFLRDialog() : _ui(new Ui::CompareFLRDialog())
{
    _ui->setupUi(this);
    setWindowTitle("Compare FLR");

    connect(_ui->compareButton, &QPushButton::clicked, this, &CompareFLRDialog::compare);

    connect(_ui->openFirstFLRButton, &QPushButton::clicked, this, [&]() {
        _ui->filepathFLR1->setText(QFileDialog::getOpenFileName(nullptr, "Open first FLR", "", "(*.flr *.txt *.TXT)"));
    });

    connect(_ui->openSecondFLRButton, &QPushButton::clicked, this, [&]() {
        _ui->filepathFLR2->setText(QFileDialog::getOpenFileName(nullptr, "Open second FLR", "", "(*.flr *.txt *.TXT)"));
    });
}

void CompareFLRDialog::initialize()
{
    _ui->tabWidget->setCurrentIndex(0);
    adjustSize();
    show();
}

void CompareFLRDialog::compare()
{
    try {
        QString resultPath;
        if(_ui->needSaveToFile->isChecked()) {
            resultPath = QFileDialog::getSaveFileName(nullptr, "Save report", "", "(*.txt)");
        }
        auto precision = _ui->precision->text().toDouble();
        auto pointsStartIndex = _ui->pointsStartIndex->text().toInt();
        auto result = Algorithms::compareFLR(_ui->filepathFLR1->text(), _ui->filepathFLR2->text(), resultPath, pointsStartIndex, precision);

        _ui->totalParams->setText(QString::number(result->paramsTotal()));
        _ui->totalFails->setText(QString::number(result->failsTotal()));
        _ui->maxFail->setText(QString::number(result->maxDeviationFail()));

        _ui->dimsTotalFails->setText(QString::number(result->dimsFailsTotal()));
        _ui->dimsFailSum->setText(QString::number(result->dimsFailsDevitationSum()));
        _ui->dimsAverageFail->setText(QString::number(result->getAverageDimDevFail()));

        _ui->pointsTotalFails->setText(QString::number(result->pointsFailsTotal()));
        _ui->pointsFailSum->setText(QString::number(result->pointsFailsDevitationSum()));
        _ui->pointsAverageFail->setText(QString::number(result->getAveragePointDevFail()));

        _ui->tabWidget->setCurrentIndex(1);
    }
    catch(...) {
    }
}

CompareFLRDialog::~CompareFLRDialog()
{
}
