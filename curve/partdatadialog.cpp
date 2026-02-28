#include "curve/pch.h"
#include "partdatadialog.h"

PartDataDialog::PartDataDialog(Project *project, QWidget *parent)
    : QDialog(parent)
    , _ui(new Ui::PartDataDialogClass())
{
    _ui->setupUi(this);
    _project = project;

    connect(_ui->buttonBox, &QDialogButtonBox::accepted, this, &PartDataDialog::applyPartDataChanges);
    connect(_ui->buttonBox, &QDialogButtonBox::rejected, this, &PartDataDialog::reject);

    connect(this, &PartDataDialog::partDataChangeRequested, _project, &Project::changePartData);
}

PartDataDialog::~PartDataDialog()
{
    delete _ui;
}

void PartDataDialog::initializationByMacros(QString reportTitle, QString description, QString drawing, QString orderNumber, QString partNumber, QString projectOperator,
    QString note, QString machine, QString tool, QString fixturing, QString batch, QString supplier, QString revision) {
    QStringList hint;
    for(auto &figure : _project->figures()) {
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


    if(MacrosManager::isRecording()) {
        _ui->macrosShowCB->show();
        _ui->macrosShowCB->setChecked(false);
    } else {
        _ui->macrosShowCB->hide();
    }

    exec();

    
}
void PartDataDialog::initialization() {
    QStringList hint;
    for(auto &figure : _project->figures()) {
        if(dynamic_cast<const TextFigure*>(figure)) {
            hint.append("TXT:" + figure->name());
        }
    }

    _ui->reportTitleCB->clear();
    _ui->reportTitleCB->addItems(hint);
    _ui->reportTitleCB->setCurrentText(_project->reportTitle());
    _ui->descriptionCB->clear();
    _ui->descriptionCB->addItems(hint);
    _ui->descriptionCB->setCurrentText(_project->description());
    _ui->drawingCB->clear();
    _ui->drawingCB->addItems(hint);
    _ui->drawingCB->setCurrentText(_project->drawing());
    _ui->orderNumberCB->clear();
    _ui->orderNumberCB->addItems(hint);
    _ui->orderNumberCB->setCurrentText(_project->orderNumber());
    _ui->partNumberCB->clear();
    _ui->partNumberCB->addItems(hint);
    _ui->partNumberCB->setCurrentText(_project->partNumber());
    _ui->operatorCB->clear();
    _ui->operatorCB->addItems(hint);
    _ui->operatorCB->setCurrentText(_project->projectOperator());
    _ui->noteCB->clear();
    _ui->noteCB->addItems(hint);
    _ui->noteCB->setCurrentText(_project->note());

    _ui->machineCB->clear();
    _ui->machineCB->addItems(hint);
    _ui->machineCB->setCurrentText(_project->machine());
    _ui->toolCB->clear();
    _ui->toolCB->addItems(hint);
    _ui->toolCB->setCurrentText(_project->tool());
    _ui->fixturingCB->clear();
    _ui->fixturingCB->addItems(hint);
    _ui->fixturingCB->setCurrentText(_project->fixturing());
    _ui->batchCB->clear();
    _ui->batchCB->addItems(hint);
    _ui->batchCB->setCurrentText(_project->batch());
    _ui->supplierCB->clear();
    _ui->supplierCB->addItems(hint);
    _ui->supplierCB->setCurrentText(_project->supplier());
    _ui->revisionCB->clear();
    _ui->revisionCB->addItems(hint);
    _ui->revisionCB->setCurrentText(_project->revision());

    if(MacrosManager::isRecording()) {
        _ui->macrosShowCB->show();
        _ui->macrosShowCB->setChecked(false);
    } else {
        _ui->macrosShowCB->hide();
    }

    show();
}
void PartDataDialog::applyPartDataChanges(){
    emit partDataChangeRequested(_ui->reportTitleCB->currentText(), _ui->descriptionCB->currentText(), _ui->drawingCB->currentText(), _ui->orderNumberCB->currentText(),
        _ui->partNumberCB->currentText(), _ui->operatorCB->currentText(), _ui->noteCB->currentText(), _ui->machineCB->currentText(), _ui->toolCB->currentText(),
        _ui->fixturingCB->currentText(), _ui->batchCB->currentText(), _ui->supplierCB->currentText(), _ui->revisionCB->currentText(), _ui->macrosShowCB->isChecked());
    accept();
}