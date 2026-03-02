#include "curve/pch.h"
#include "exporttoFLRdialog.h"

ExportToFLRDialog::ExportToFLRDialog(Project *project, QWidget *parent) : QDialog(parent), _ui(new Ui::ExportToFLRDialog) {
    _ui->setupUi(this);
    _project = project;

    setWindowTitle("Export to FLR");
    setFixedSize(350, 420);

    connect(_ui->choosePathButton, &QPushButton::clicked, this, &ExportToFLRDialog::chooseFilePath);
    connect(_ui->chooseAllCheckbox, &QCheckBox::stateChanged, this, &ExportToFLRDialog::onChooseAllStateChanged);
    connect(_ui->cancelButton, &QPushButton::clicked, this, &ExportToFLRDialog::closeWindow);
    connect(_ui->exportButton, &QPushButton::clicked, this, &ExportToFLRDialog::exportToFLR);
}

void ExportToFLRDialog::initialization() {
    for(auto curve : _project->curveFigures()) {
        auto item = new QListWidgetItem(curve->name());
        item->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        item->setCheckState(Qt::Unchecked);
        _ui->curveList->addItem(item);
    }
    exec();

}

void ExportToFLRDialog::exportToFLR() {
    auto figuresToTake = new QStringList();
    for(auto i = 0; i < _ui->curveList->count(); i++) {
        auto item = _ui->curveList->item(i);
        if(item->checkState() == Qt::Checked) {
            figuresToTake->append(item->text());
        }
    }
    FileSystem::exportToFLR(_project, _ui->filePathLineEdit->text(), figuresToTake);
    close();
}

void ExportToFLRDialog::chooseFilePath() {
    QString fileName = QFileDialog::getSaveFileName(nullptr, "Choose filepath", "", "FLR Files (*.flr)");
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
