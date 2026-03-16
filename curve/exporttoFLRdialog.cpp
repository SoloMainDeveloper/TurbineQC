#include "curve/pch.h"

#include "exporttoFLRdialog.h"
#include "ui_exporttoFLRdialog.h"
#include "filesystem.h"

ExportToFLRDialog::ExportToFLRDialog() : _ui(new Ui::ExportToFLRDialog) {
    _ui->setupUi(this);

    setFixedSize(350, 420);

    connect(_ui->choosePathButton, &QPushButton::clicked, this, &ExportToFLRDialog::chooseFilePath);
    connect(_ui->chooseAllCheckbox, &QCheckBox::stateChanged, this, &ExportToFLRDialog::onChooseAllStateChanged);
    connect(_ui->cancelButton, &QPushButton::clicked, this, &ExportToFLRDialog::closeWindow);
    connect(_ui->exportButton, &QPushButton::clicked, this, &ExportToFLRDialog::exportToFLR);
}

void ExportToFLRDialog::initialize() {
    auto curves = Project::instance().curveFigures();
    std::sort(curves.begin(), curves.end(), [](const auto &a, const auto &b) { return a->index() < b->index(); });
    for(auto curve : curves) {
        auto item = new QListWidgetItem(curve->name());
        item->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        item->setCheckState(Qt::Unchecked);
        _ui->curveList->addItem(item);
    }
    exec();

}

void ExportToFLRDialog::exportToFLR() {
    QStringList figuresToTake;
    for(auto i = 0; i < _ui->curveList->count(); i++) {
        auto item = _ui->curveList->item(i);
        if(item->checkState() == Qt::Checked) {
            figuresToTake.append(item->text());
        }
    }
    try {
        FileSystem::exportToFLR(_ui->filePathLineEdit->text(), &figuresToTake);
        close();
    } catch(...) {
    }
}

void ExportToFLRDialog::chooseFilePath() {
    QString fileName = QFileDialog::getSaveFileName(nullptr, tr("Choose filepath"), "", "FLR Files (*.flr)");
    if(!fileName.isEmpty()) {
        _ui->filePathLineEdit->setText(fileName);
    }
}

void ExportToFLRDialog::closeWindow() {
    close();
    _ui->filePathLineEdit->clear();
    _ui->curveList->clear();
}

void ExportToFLRDialog::closeEvent(QCloseEvent *event) {
    closeWindow();
}

void ExportToFLRDialog::onChooseAllStateChanged() {
    for(auto i = 0; i < _ui->curveList->count(); i++) {
        _ui->curveList->item(i)->setCheckState(_ui->chooseAllCheckbox->isChecked() ? Qt::Checked : Qt::Unchecked);
    }
}

ExportToFLRDialog::~ExportToFLRDialog() {
    delete _ui;
}
