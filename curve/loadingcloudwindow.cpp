#include "curve/pch.h"
#include "loadingcloudwindow.h"

LoadingCloudWindow::LoadingCloudWindow(Project *project, QWidget *parent) : QDialog(parent), _ui(new Ui::LoadingCloudWindow()) {
    _ui->setupUi(this);

    _project = project;

    this->setWindowTitle("Load cloud");
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
    _ui->columnNumbersBox->setCurrentIndex(2);
    _ui->columnNumbersBox->setFixedSize(QSize(77, 22));

    _ui->columnNamesBox->addItem("X,Y");
    _ui->columnNamesBox->addItem("X,Y,Z");
    _ui->columnNamesBox->addItem("X,Y,Z,I,J,K");
    _ui->columnNamesBox->setCurrentIndex(2);
    _ui->columnNamesBox->setFixedSize(QSize(77, 22));

    _ui->decimalBox->addItem(".");
    _ui->decimalBox->addItem(",");
    _ui->decimalBox->setCurrentIndex(0);
    _ui->decimalBox->setFixedSize(QSize(77, 22));

    _ui->separatorBox->addItem(";");
    _ui->separatorBox->addItem(",");
    _ui->separatorBox->addItem(" ");
    _ui->separatorBox->setCurrentIndex(2);
    _ui->separatorBox->setFixedSize(QSize(77, 22));

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

    if(file.exists()) {
        _fileData = FileSystem::readFile(filePath).split("\n");
        _name = filePath.replace("\\", "/").split("/").last().split('.')[0];
        _ui->inputData->clear();
        _ui->notifyLabel->clear();
        _ui->nameFigureLineEdit->setText(_name);
        if(_fileData.length() > 0) {
            for(auto i = 0; i < _fileData.length(); i++) {
                _ui->inputData->append(QString("%1 -> %2").arg(i + 1, 4, 10, QChar('0')).arg(_fileData[i]));
            }
            _ui->startLineEdit->setText("1");
            _ui->finishLineEdit->setText(QString::number(_fileData.length()));
        }
    }
}

void LoadingCloudWindow::openFile() {
    auto filePath = QFileDialog::getOpenFileName(nullptr, "Open file", "", "(*.txt)");

    if(!filePath.isEmpty()) {
        _ui->filePathLineEdit->setText(filePath);
        _fileData = FileSystem::readFile(filePath).split("\n");
        _name = filePath.replace("\\", "/").split('/').last().split('.')[0];
        _ui->inputData->clear();
        _ui->notifyLabel->clear();
        _ui->nameFigureLineEdit->setText(_name);
        if(_fileData.length() > 0) {
            for(auto i = 0; i < _fileData.length(); i++) {
                _ui->inputData->append(QString("%1 -> %2").arg(i + 1, 4, 10, QChar('0')).arg(_fileData[i]));
            }
            _ui->startLineEdit->setText("1");
            _ui->finishLineEdit->setText(QString::number(_fileData.length()));
        }
    }
}

void LoadingCloudWindow::pasteData() {
    QClipboard *clipboard = QApplication::clipboard();
    _fileData = clipboard->text().split("\n");
    _ui->inputData->clear();
    if(_fileData.length() > 0) {
        for(auto i = 0; i < _fileData.length(); i++) {
            _ui->inputData->append(QString("%1 -> %2").arg(i + 1, 4, 10, QChar('0')).arg(_fileData[i]));
        }
        _ui->startLineEdit->setText("1");
        _ui->finishLineEdit->setText(QString::number(_fileData.length()));
        _ui->filePathLineEdit->clear();
        _ui->nameFigureLineEdit->clear();
        _ui->notifyLabel->clear();
    }
}

void LoadingCloudWindow::loadCloud() {
    _name = _ui->nameFigureLineEdit->text();

    if(_name != "" && _ui->inputData->toPlainText() != "") {
        auto separator = _ui->separatorBox->currentText();
        auto startLineToSkip = _ui->startLineEdit->text().toInt() - 1;
        auto finishLineToSkip = _fileData.length() - _ui->finishLineEdit->text().toInt();
        auto points = FileSystem::parsePointsFromElement(_fileData, separator, startLineToSkip, finishLineToSkip, Order::Default);
        if(!_project->containsFigure(_name)) {
            if(points.length() > 0) {
                _ui->notifyLabel->setText(QString("Interpreted lines %1!").arg(points.length()));
                auto curve = new CurveFigure(_name, points, 1);
                _project->insertFigure(curve);
            } else {
                _ui->notifyLabel->setText("Error in interpretation settings!");
            }
        } else {
            _ui->notifyLabel->setText("This name already exists!");
        }
    }
}

void LoadingCloudWindow::closeEvent(QCloseEvent *event) {
    closeWindow();
}

// For the CURVE-56 test
QString LoadingCloudWindow::name() {
    return _name;
}

void LoadingCloudWindow::closeWindow() {
    this->close();
    _ui->fileBtn->setChecked(true);
    _ui->groupOfFileWidgets->show();
    _groupOfBtnWidgets->hide();
    _ui->filePathLineEdit->clear();
    _ui->inputData->clear();
    _ui->startLineEdit->clear();
    _ui->finishLineEdit->clear();
    _ui->columnNumbersBox->setCurrentIndex(2);
    _ui->columnNamesBox->setCurrentIndex(2);
    _ui->decimalBox->setCurrentIndex(0);
    _ui->separatorBox->setCurrentIndex(2);
    _ui->notifyLabel->clear();
    _ui->nameFigureLineEdit->clear();
}

LoadingCloudWindow::~LoadingCloudWindow() {
    delete _ui;
}
