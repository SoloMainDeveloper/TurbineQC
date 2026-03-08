#include "curve/pch.h"

#include "partdatadialog.h"
#include "ui_partdatadialog.h"
#include "project.h"

PartDataDialog::PartDataDialog() : _ui(new Ui::PartDataDialog()) {
    _ui->setupUi(this);

    connect(_ui->buttonBox, &QDialogButtonBox::accepted, this, &PartDataDialog::applyPartDataChanges);
    connect(_ui->buttonBox, &QDialogButtonBox::rejected, this, &PartDataDialog::reject);

    connect(this, &PartDataDialog::partDataChangeRequested, &Project::instance(), &Project::changePartData);
}

PartDataDialog::~PartDataDialog() {
    delete _ui;
}

void PartDataDialog::initializationByMacros(QString reportTitle, QString description, QString drawing, QString orderNumber, QString partNumber, QString projectOperator,
    QString note, QString machine, QString tool, QString fixturing, QString batch, QString supplier, QString revision) {
    QStringList hint;
    for(auto &figure : Project::instance().figures()) {
        if(dynamic_cast<const TextFigure*>(figure)) {
            hint.append("TXT:" + figure->name());
        }
    }

    _ui->reportTitleCB->clear();
    _ui->reportTitleCB->addItems(hint);
    _ui->reportTitleCB->setCurrentText(reportTitle);
    _ui->descriptionCB->clear();
    _ui->descriptionCB->addItems(hint);
    _ui->descriptionCB->setCurrentText(description);
    _ui->drawingCB->clear();
    _ui->drawingCB->addItems(hint);
    _ui->drawingCB->setCurrentText(drawing);
    _ui->orderNumberCB->clear();
    _ui->orderNumberCB->addItems(hint);
    _ui->orderNumberCB->setCurrentText(orderNumber);
    _ui->partNumberCB->clear();
    _ui->partNumberCB->addItems(hint);
    _ui->partNumberCB->setCurrentText(partNumber);
    _ui->operatorCB->clear();
    _ui->operatorCB->addItems(hint);
    _ui->operatorCB->setCurrentText(projectOperator);
    _ui->noteCB->clear();
    _ui->noteCB->addItems(hint);
    _ui->noteCB->setCurrentText(note);

    _ui->machineCB->clear();
    _ui->machineCB->addItems(hint);
    _ui->machineCB->setCurrentText(machine);
    _ui->toolCB->clear();
    _ui->toolCB->addItems(hint);
    _ui->toolCB->setCurrentText(tool);
    _ui->fixturingCB->clear();
    _ui->fixturingCB->addItems(hint);
    _ui->fixturingCB->setCurrentText(fixturing);
    _ui->batchCB->clear();
    _ui->batchCB->addItems(hint);
    _ui->batchCB->setCurrentText(batch);
    _ui->supplierCB->clear();
    _ui->supplierCB->addItems(hint);
    _ui->supplierCB->setCurrentText(supplier);
    _ui->revisionCB->clear();
    _ui->revisionCB->addItems(hint);
    _ui->revisionCB->setCurrentText(revision);


    if(MacrosManager::instance().isRecording()) {
        _ui->macrosShowCB->show();
        _ui->macrosShowCB->setChecked(false);
    } else {
        _ui->macrosShowCB->hide();
    }

    exec();


}
void PartDataDialog::initialize() {
    QStringList hint;
    auto project = &Project::instance();
    for(auto &figure : project->figures()) {
        if(dynamic_cast<const TextFigure*>(figure)) {
            hint.append("TXT:" + figure->name());
        }
    }

    _ui->reportTitleCB->clear();
    _ui->reportTitleCB->addItems(hint);
    _ui->reportTitleCB->setCurrentText(Project::instance().reportTitle());
    _ui->descriptionCB->clear();
    _ui->descriptionCB->addItems(hint);
    _ui->descriptionCB->setCurrentText(project->description());
    _ui->drawingCB->clear();
    _ui->drawingCB->addItems(hint);
    _ui->drawingCB->setCurrentText(project->drawing());
    _ui->orderNumberCB->clear();
    _ui->orderNumberCB->addItems(hint);
    _ui->orderNumberCB->setCurrentText(project->orderNumber());
    _ui->partNumberCB->clear();
    _ui->partNumberCB->addItems(hint);
    _ui->partNumberCB->setCurrentText(project->partNumber());
    _ui->operatorCB->clear();
    _ui->operatorCB->addItems(hint);
    _ui->operatorCB->setCurrentText(project->projectOperator());
    _ui->noteCB->clear();
    _ui->noteCB->addItems(hint);
    _ui->noteCB->setCurrentText(project->note());

    _ui->machineCB->clear();
    _ui->machineCB->addItems(hint);
    _ui->machineCB->setCurrentText(project->machine());
    _ui->toolCB->clear();
    _ui->toolCB->addItems(hint);
    _ui->toolCB->setCurrentText(project->tool());
    _ui->fixturingCB->clear();
    _ui->fixturingCB->addItems(hint);
    _ui->fixturingCB->setCurrentText(project->fixturing());
    _ui->batchCB->clear();
    _ui->batchCB->addItems(hint);
    _ui->batchCB->setCurrentText(project->batch());
    _ui->supplierCB->clear();
    _ui->supplierCB->addItems(hint);
    _ui->supplierCB->setCurrentText(project->supplier());
    _ui->revisionCB->clear();
    _ui->revisionCB->addItems(hint);
    _ui->revisionCB->setCurrentText(project->revision());

    if(MacrosManager::instance().isRecording()) {
        _ui->macrosShowCB->show();
        _ui->macrosShowCB->setChecked(false);
    } else {
        _ui->macrosShowCB->hide();
    }

    show();
}

void PartDataDialog::applyPartDataChanges() {
    emit partDataChangeRequested(_ui->reportTitleCB->currentText(), _ui->descriptionCB->currentText(), _ui->drawingCB->currentText(), _ui->orderNumberCB->currentText(),
        _ui->partNumberCB->currentText(), _ui->operatorCB->currentText(), _ui->noteCB->currentText(), _ui->machineCB->currentText(), _ui->toolCB->currentText(),
        _ui->fixturingCB->currentText(), _ui->batchCB->currentText(), _ui->supplierCB->currentText(), _ui->revisionCB->currentText(), _ui->macrosShowCB->isChecked());
    accept();
}
