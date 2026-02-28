#include "curve/pch.h"
#include "loadingcloudwindow.h"

LoadingCloudWindow::LoadingCloudWindow(Project *project, QWidget *parent) : QDialog(parent), _ui(new Ui::LoadingCloudWindow()) {
    _ui->setupUi(this);

    _project = project;
    _message = new QMessageBox(this);
    _message->setWindowTitle("Warning");

    this->setWindowTitle("Load file");
    this->setFixedSize(750, 500);

    _groupOfBtnWidgets = new QGroupBox("From clipboard", this);
    _btnWidgetsLayout = new QHBoxLayout(_groupOfBtnWidgets);
    _ui->gridLayout->addWidget(_groupOfBtnWidgets, 0, 2, 1, 7);

    _pasteDataBtn = new QPushButton("Paste from clipboard");
    _pasteDataBtn->setFocusPolicy(Qt::NoFocus);
    _btnWidgetsLayout->addItem(new QSpacerItem(160, 20, QSizePolicy::Minimum));
    _btnWidgetsLayout->addWidget(_pasteDataBtn);
    _btnWidgetsLayout->addItem(new QSpacerItem(160, 20, QSizePolicy::Minimum));

    _groupOfBtnWidgets->hide();

    _ui->columnNumbersBox->addItem("1,2");
    _ui->columnNumbersBox->addItem("1,2,3");
    _ui->columnNumbersBox->addItem("1,2,3,4,5,6");
    _ui->columnNumbersBox->addItem("1,2,3,4,5,6,7");
    _ui->columnNumbersBox->addItem("1,2,3,4,5,6,7,8,9");
    _ui->columnNumbersBox->setCurrentIndex(0);
    _ui->columnNumbersBox->setFixedSize(QSize(120, 22));

    _ui->columnNamesBox->addItem("X,Y");
    _ui->columnNamesBox->addItem("X,Y,Z");
    _ui->columnNamesBox->addItem("X,Y,Z,I,J,K");
    _ui->columnNamesBox->addItem("X,Y,Z,I,J,K,D");
    _ui->columnNamesBox->addItem("X,Y,Z,I,J,K,D,UT,LT");
    _ui->columnNamesBox->setCurrentIndex(0);
    _ui->columnNamesBox->setFixedSize(QSize(120, 22));

    _ui->decimalBox->addItem(".");
    _ui->decimalBox->addItem(",");
    _ui->decimalBox->setCurrentIndex(0);
    _ui->decimalBox->setFixedSize(QSize(80, 22));

    _ui->separatorBox->addItem(";");
    _ui->separatorBox->addItem(",");
    _ui->separatorBox->addItem(" ");
    _ui->separatorBox->setCurrentIndex(2);
    _ui->separatorBox->setFixedSize(QSize(80, 22));

    connect(_ui->fileBtn, &QRadioButton::clicked, this, &LoadingCloudWindow::selectFile);
    connect(_ui->clipboardBtn, &QRadioButton::clicked, this, &LoadingCloudWindow::selectClipboard);
    connect(_ui->findFileBtn, &QPushButton::clicked, this, &LoadingCloudWindow::findFile);
    connect(_ui->openFileBtn, &QPushButton::clicked, this, &LoadingCloudWindow::openFile);
    connect(_ui->loadCloudBtn, &QPushButton::clicked, this, &LoadingCloudWindow::loadCloud);
    connect(_ui->closeWindowBtn, &QPushButton::clicked, this, &LoadingCloudWindow::closeWindow);
    connect(_pasteDataBtn, &QPushButton::clicked, this, &LoadingCloudWindow::pasteData);
}

void LoadingCloudWindow::selectFile() {
    _ui->groupOfFileWidgets->show();
    _groupOfBtnWidgets->hide();
}

void LoadingCloudWindow::selectClipboard() {
    _groupOfBtnWidgets->show();
    _ui->groupOfFileWidgets->hide();
}

void LoadingCloudWindow::findFile() {
    auto filePath = _ui->filePathLineEdit->text();
    QFile file(filePath);

    if(!file.exists()) {
        _message->setText("File not found! Check the path.");
        _message->exec();
        return;
    }
    if(!isCorrectFileExtension(filePath)) {
        _message->setText("Incorrect file extension!");
        _message->exec();
        return;
    }

    QApplication::setOverrideCursor(Qt::WaitCursor);
    auto name = filePath.replace("\\", "/").split("/").last().split('.')[0];
    _fileData = FileSystem::readFile(filePath).split("\n");
    _ui->inputData->clear();
    _ui->notifyLabel->clear();
    _ui->nameFigureLineEdit->setText(name);
    for(auto i = 0; i < _fileData.length(); i++) {
        _ui->inputData->append(QString("%1 -> %2").arg(i + 1, 4, 10, QChar('0')).arg(_fileData[i]));
    }
    _ui->startLineEdit->setText("1");
    _ui->finishLineEdit->setText(QString::number(_fileData.length()));
    QApplication::restoreOverrideCursor();
}

bool LoadingCloudWindow::isCorrectFileExtension(const QString &filePath) {
    auto extension = QFileInfo(filePath).suffix().toLower();
    return extension == "txt" || extension == "nom";
}

void LoadingCloudWindow::openFile() {
    auto filePath = QFileDialog::getOpenFileName(nullptr, "Open file", "", "(*.txt);; (*.nom)");

    if(filePath.isEmpty()) {
        return;
    }

    QApplication::setOverrideCursor(Qt::WaitCursor);
    _ui->filePathLineEdit->setText(filePath);
    _fileData = FileSystem::readFile(filePath).split("\n");
    auto name = filePath.replace("\\", "/").split('/').last().split('.')[0];
    _ui->inputData->clear();
    _ui->notifyLabel->clear();
    _ui->nameFigureLineEdit->setText(name);
    for(auto i = 0; i < _fileData.length(); i++) {
        _ui->inputData->append(QString("%1 -> %2").arg(i + 1, 4, 10, QChar('0')).arg(_fileData[i]));
    }
    _ui->startLineEdit->setText("1");
    _ui->finishLineEdit->setText(QString::number(_fileData.length()));
    QApplication::restoreOverrideCursor();
}

void LoadingCloudWindow::pasteData() {
    QApplication::setOverrideCursor(Qt::WaitCursor);
    auto clipboard = QApplication::clipboard();
    _fileData = clipboard->text().split("\n");
    _ui->inputData->clear();
    for(auto i = 0; i < _fileData.length(); i++) {
        _ui->inputData->append(QString("%1 -> %2").arg(i + 1, 4, 10, QChar('0')).arg(_fileData[i]));
    }
    _ui->startLineEdit->setText("1");
    _ui->finishLineEdit->setText(QString::number(_fileData.length()));
    _ui->filePathLineEdit->clear();
    _ui->nameFigureLineEdit->clear();
    _ui->notifyLabel->clear();
    QApplication::restoreOverrideCursor();
}

void LoadingCloudWindow::loadCloud() {
    auto name = _ui->nameFigureLineEdit->text();

    if(name.isEmpty()) {
        _message->setText("Write a name for the curve!");
        _message->exec();
        return;
    }
    if(_ui->inputData->toPlainText().isEmpty()) {
        _message->setText("There is no data to load!");
        _message->exec();
        return;
    }
    if(_project->containsFigure(name)) {
        _message->setText("This name already exists!\nDo you want to overwrite the curve?");
        _message->setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        _message->exec();
        auto button = _message->clickedButton()->text();
        _message->setStandardButtons(QMessageBox::Ok);
        if(button == "Cancel") {
            return;
        }
        _project->removeFigure(name);
    }

    auto columnNames = _ui->columnNamesBox->currentText();
    auto columnNumbers = _ui->columnNumbersBox->currentText();
    auto decimal = _ui->decimalBox->currentText();
    auto separator = _ui->separatorBox->currentText();
    auto skipStart = _ui->startLineEdit->text().toInt() - 1;
    auto skipAfter = _fileData.length() - _ui->finishLineEdit->text().toInt();
    try {
        FileSystem::loadCloud(_project, _ui->filePathLineEdit->text(), name, separator, skipStart,
            skipAfter, columnNames, columnNumbers, decimal);
        _ui->notifyLabel->setText(QString("Interpreted completed!"));
    } catch(...) {
    }
}

void LoadingCloudWindow::closeEvent(QCloseEvent * event) {
    closeWindow();
}

void LoadingCloudWindow::closeWindow() {
    this->close();
    _ui->fileBtn->setChecked(true);
    _ui->groupOfFileWidgets->show();
    _groupOfBtnWidgets->hide();
    _ui->filePathLineEdit->clearFocus();
    _ui->filePathLineEdit->clear();
    _ui->inputData->clear();
    _ui->startLineEdit->clearFocus();
    _ui->startLineEdit->clear();
    _ui->finishLineEdit->clearFocus();
    _ui->finishLineEdit->clear();
    _ui->columnNumbersBox->clearFocus();
    _ui->columnNumbersBox->setCurrentIndex(0);
    _ui->columnNamesBox->clearFocus();
    _ui->columnNamesBox->setCurrentIndex(0);
    _ui->decimalBox->clearFocus();
    _ui->decimalBox->setCurrentIndex(0);
    _ui->separatorBox->clearFocus();
    _ui->separatorBox->setCurrentIndex(2);
    _ui->notifyLabel->clear();
    _ui->nameFigureLineEdit->clearFocus();
    _ui->nameFigureLineEdit->clear();
}

LoadingCloudWindow::~LoadingCloudWindow() {
    delete _ui;
}
