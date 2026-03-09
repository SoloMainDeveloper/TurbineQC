#include "curve/pch.h"

#include "saveprojectdialog.h"
#include "ui_saveprojectdialog.h"
#include "project.h"
#include "filesystem.h"

SaveProjectDialog::SaveProjectDialog() : _ui(new Ui::SaveProjectDialog()) {
    _ui->setupUi(this);
    this->setWindowTitle("Save project");
    connect(_ui->createCRV, &QCheckBox::clicked, this, &SaveProjectDialog::showBoxCRV);
    connect(_ui->saveButton, &QPushButton::clicked, this, &SaveProjectDialog::saveProject);
    connect(_ui->openButton, &QPushButton::clicked, this, &SaveProjectDialog::openExplorer);
    connect(_ui->projectNameEdit, &QLineEdit::textChanged, this, &SaveProjectDialog::projectNameChanged);
}

void SaveProjectDialog::initialize() {
    exec();
}

void SaveProjectDialog::closeWindow() {
    close();
    _ui->directoryPathEdit->setText("");
    _ui->projectNameEdit->setText("");
    _ui->createCRV->click();
    _ui->projectNameEdit->setPlaceholderText("Write project name (without file extension)");
    _ui->directoryPathEdit->setPlaceholderText("Choose directory path...");
    _ui->resultNameCRV->setPlaceholderText("");
}

void SaveProjectDialog::closeEvent(QCloseEvent *event) {
    closeWindow();
}

void SaveProjectDialog::showBoxCRV() {
    auto boxCRV = _ui->boxCRV;
    if(_ui->createCRV->isChecked()) {
        boxCRV->show();
    } else {
        boxCRV->hide();
    }
}

void SaveProjectDialog::saveProject() {
    auto dir = _ui->directoryPathEdit->text();
    auto name = _ui->projectNameEdit->text();
    auto needCreateCRV = _ui->createCRV->isChecked();
    if(!FileSystem::isValidFileName(name)) {
        _ui->projectNameEdit->clear();
        _ui->projectNameEdit->setPlaceholderText("Invalid name");
        _ui->projectNameEdit->setFocus();
        return;
    }
    if(dir == "") {
        _ui->directoryPathEdit->setPlaceholderText("Invalid path");
        return;
    }
    if(QFile(dir + "/" + name + ".txt").exists()) {
        _ui->projectNameEdit->clear();
        _ui->projectNameEdit->setPlaceholderText("File already exists");
        _ui->projectNameEdit->setFocus();
        return;
    }
    if(_ui->createCRV->isChecked() && QFile(dir + "/" + name + ".crv").exists()) {
        _ui->resultNameCRV->clear();
        _ui->resultNameCRV->setPlaceholderText("File already exists");
        return;
    }
    try {
        FileSystem::saveProject(dir, name, needCreateCRV);
    } catch(...) {
    }
    closeWindow();
}

void SaveProjectDialog::openExplorer() {
    auto path = QFileDialog::getExistingDirectory(this, tr("Open Directory"), "",
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if(!path.isEmpty()) {
        _ui->directoryPathEdit->setText(path);
    }
}

void SaveProjectDialog::projectNameChanged() {
    auto name = _ui->projectNameEdit->text();
    if(name != "") {
        _ui->resultNameCRV->setText(name + ".crv");
    } else {
        _ui->resultNameCRV->setText("");
    }
}


SaveProjectDialog::~SaveProjectDialog() {
    delete _ui;
}
