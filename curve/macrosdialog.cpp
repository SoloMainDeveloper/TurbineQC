#include "curve/pch.h"

#include "macrosdialog.h"
#include "ui_macrosdialog.h"
#include "macrostranslator.h"

MacrosDialog::MacrosDialog() : _ui(new Ui::MacrosDialog()) {
    _ui->setupUi(this);

    _macrosManager = &MacrosManager::instance();
    _operationList = _ui->operationList;
    _operationList->setHeaderHidden(true);
    _ui->buttonsList->setCurrentIndex(0);
    _isDebugging = false;

    editItem = new QAction(QIcon("icons/edit.ico"), "Редактировать");
    removeItem = new QAction(QIcon("icons/delete.ico"), "Удалить");
    executeCurrentItem = new QAction(QIcon("icons/debug.ico"), "Выполнить выбранную команду");
    moveOperationUp = new QAction(QIcon("icons/arrowUp.ico"), "Сдвинуть выше");
    moveOperationDown = new QAction(QIcon("icons/arrowDown.ico"), "Сдвинуть ниже");

    connect(removeItem, &QAction::triggered, this, &MacrosDialog::onRemoveItemTriggered);
    connect(editItem, &QAction::triggered, this, &MacrosDialog::onEditItemTriggered);
    connect(executeCurrentItem, &QAction::triggered, this, &MacrosDialog::onExecuteCurrentItemTriggered);
    connect(moveOperationUp, &QAction::triggered, this, &MacrosDialog::onMoveOperationUpItemTriggered);
    connect(moveOperationDown, &QAction::triggered, this, &MacrosDialog::onMoveOperationDownItemTriggered);

    connect(_macrosManager, &MacrosManager::operationLogged, this, &MacrosDialog::addOperation);
    connect(_macrosManager, &MacrosManager::recordingToggled, this, &MacrosDialog::updateRecordingButton);
    connect(_macrosManager, &MacrosManager::recordIndexChanged, this, &MacrosDialog::onRecordIndexChanged);
    connect(_macrosManager, &MacrosManager::operationExecuted, this, &MacrosDialog::onOperationExecuted);
    connect(_macrosManager, &MacrosManager::operationSkipped, this, &MacrosDialog::onOperationSkipped);

    connect(_ui->toggleRecordButton, &QPushButton::clicked, _macrosManager, &MacrosManager::toggleRecording);
    connect(_ui->clearButton, &QPushButton::clicked, this, &MacrosDialog::clear);
    connect(_ui->runButton, &QPushButton::clicked, this, &MacrosDialog::run);
    connect(_ui->saveButton, &QPushButton::clicked, this, &MacrosDialog::save);
    connect(_ui->loadButton, &QPushButton::clicked, this, &MacrosDialog::load);
    connect(_ui->moveRecordButton, &QPushButton::clicked, this, &MacrosDialog::onMoveRecordButtonClicked);
    connect(_ui->startDebugButton, &QPushButton::clicked, this, &MacrosDialog::startDebug);
    connect(_ui->stopDebugButton, &QPushButton::clicked, this, &MacrosDialog::stopDebug);
    connect(_ui->nextDebugButton, &QPushButton::clicked, this, &MacrosDialog::debugNext);
    connect(_ui->skipButton, &QPushButton::clicked, _macrosManager, &MacrosManager::skipOne);
}

void MacrosDialog::initialize() {
    setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
    onRecordIndexChanged(_macrosManager->recordIndex());

    QScreen* screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    int x = screenGeometry.width() - width() - 20;
    int y = 50;

    move(x, y);
    show();
}

void MacrosDialog::addOperation(QString operation, QString comment) {
    auto recordIndex = _macrosManager->recordIndex();
    auto item = createOperationItem(recordIndex, operation, comment);
    insert(recordIndex, item);
    reindex(recordIndex);
}

void MacrosDialog::insert(int index, QTreeWidgetItem* item) {
    auto items = QList<QTreeWidgetItem*>();
    while(index != _operationList->topLevelItemCount()) {
        auto itemi = _operationList->takeTopLevelItem(index - 1);
        auto text = itemi->text(0);
        items.append(itemi);
    }
    items.append(item);
    _operationList->insertTopLevelItems(index, items);
}

void MacrosDialog::clear() {
    _macrosManager->clear();
    _operationList->clear();
}

void MacrosDialog::save() {
    QString fileName = QFileDialog::getSaveFileName(nullptr, "Сохранить макрос", "", "Macros (*.json)");
    if(!fileName.isEmpty()) {
        QFile file(fileName);
        if(!file.open(QIODevice::WriteOnly)) {
            qWarning() << "Cannot open file for writing:" << fileName;
            return;
        }
        QJsonDocument doc(_macrosManager->toJson());
        file.write(doc.toJson());
        file.close();
    } else {
        QMessageBox::critical(nullptr, "Error", "Макрос не был сохранен");
    }
}

void MacrosDialog::load() {
    auto path = QFileDialog::getOpenFileName(nullptr, "Загрузить макрос", "", "Macros (*.json *.crm)");
    if(!path.isEmpty()) {
        QFileInfo fileInfo(path);
        QString extension = fileInfo.suffix().toLower();

        if(extension == "crm") {
            QFile file(path);
            if(!file.open(QIODevice::ReadOnly)) {
                qWarning() << "Cannot open file for reading:" << path;
                return;
            }

            QByteArray data = file.readAll();
            QString operationText = QString::fromUtf8(data);
            operationText.remove('\r');

            QList<std::shared_ptr<ICommand>>* commands = MacrosTranslator::translateCRM(operationText);

            // Здесь нужно обработать полученные команды
            for(auto command : *commands) {
                _macrosManager->log(command);
            }

            file.close();
        } else if(extension == "json") {
            QFile file(path);
            if(!file.open(QIODevice::ReadOnly)) {
                qWarning() << "Cannot open file for reading:" << path;
                return;
            }

            QString content = QString::fromUtf8(file.readAll());
            content.remove('\r');

            QJsonDocument doc = QJsonDocument::fromJson(content.toUtf8());

            if(doc.isArray()) {
                _macrosManager->fromJson(doc.array());
            }

            file.close();
        }
    }
}

void MacrosDialog::startDebug() {
    _ui->buttonsList->setCurrentIndex(1);
    _isDebugging = true;
}

void MacrosDialog::stopDebug() {
    _macrosManager->stopDebug();
    _ui->buttonsList->setCurrentIndex(0);
    _ui->nextDebugButton->setEnabled(true);
    _ui->skipButton->setEnabled(true);
    _isDebugging = false;
    for(auto i = 0; i < _operationList->topLevelItemCount(); i++) {
        _operationList->topLevelItem(i)->setBackground(0, QBrush(QColor(255, 255, 255)));
    }
}

void MacrosDialog::debugNext() {
    _macrosManager->debugNext();
}

void MacrosDialog::updateRecordingButton() {
    if(_macrosManager->isRecording()) {
        _ui->toggleRecordButton->setText("Стоп");
    } else {
        _ui->toggleRecordButton->setText("Начать запись");
    }
}

void MacrosDialog::run() {
    _macrosManager->run();
}

void MacrosDialog::contextMenuEvent(QContextMenuEvent * event) {
    auto current = _operationList->currentItem();
    auto index = current->text(0).split('.')[0].toInt() - 1;
    if(current && !current->parent()) {
        QMenu menu;
        menu.addAction(removeItem);
        menu.addAction(editItem);
        if(!_isDebugging) {
            moveOperationUp->setEnabled(index != 0);
            moveOperationDown->setEnabled(index != _operationList->topLevelItemCount() - 1);
            menu.addAction(executeCurrentItem);
            menu.addAction(moveOperationUp);
            menu.addAction(moveOperationDown);
        }
        menu.exec(event->globalPos());
    }
}

void MacrosDialog::onRemoveItemTriggered() {
    QMessageBox mBox;
    mBox.setText("Удалить выбранную команду?");
    mBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    auto dialogWindow = mBox.exec();
    switch(dialogWindow) {
        case QMessageBox::Ok:
        {
            auto index = _operationList->currentItem()->text(0).split('.')[0].toInt() - 1;
            removeOperation(index);
            break;
        }
        case QMessageBox::Cancel:
            break;
    }
}

void MacrosDialog::onExecuteCurrentItemTriggered() {
    auto index = _operationList->currentItem()->text(0).split('.')[0].toInt() - 1;
    _macrosManager->executeOne(index);
}

void MacrosDialog::onMoveOperationUpItemTriggered() {
    auto index = _operationList->currentItem()->text(0).split('.')[0].toInt() - 1;
    if(index == 0) return;
    _operationList->insertTopLevelItem(index - 1, _operationList->takeTopLevelItem(index));
    _operationList->setCurrentItem(_operationList->topLevelItem(index - 1));
    reindex(index - 1, index);
    _macrosManager->swapOperations(index - 1, index);
}

void MacrosDialog::onMoveOperationDownItemTriggered() {
    auto index = _operationList->currentItem()->text(0).split('.')[0].toInt() - 1;
    if(index == _operationList->topLevelItemCount() - 1) return;
    _operationList->insertTopLevelItem(index + 1, _operationList->takeTopLevelItem(index));
    _operationList->setCurrentItem(_operationList->topLevelItem(index + 1));
    reindex(index, index + 1);
    _macrosManager->swapOperations(index, index + 1);
}

void MacrosDialog::onEditItemTriggered() {
    auto* dialog = new QDialog(this, Qt::WindowStaysOnTopHint);
    dialog->setWindowTitle("Редактировать команду");
    dialog->setFixedSize(420, 470);

    int index = _operationList->currentItem()->text(0).split(".")[0].toInt() - 1;
    auto currentCommand = _macrosManager->getCommand(index);
    QJsonObject commandJson = currentCommand->toJson();
    auto paramsJson = QJsonDocument(commandJson["parameters"].toObject());

    auto* edit = new QTextEdit(dialog);
    edit->setText(QString::fromUtf8(paramsJson.toJson(QJsonDocument::Indented)));
    edit->setGeometry(10, 10, 400, 400);

    auto* cancelBtn = new QPushButton("Отмена", dialog);
    cancelBtn->setGeometry(10, 420, 195, 40);
    connect(cancelBtn, &QPushButton::clicked, dialog, &QDialog::close);

    auto* acceptBtn = new QPushButton("Применить", dialog);
    acceptBtn->setGeometry(215, 420, 195, 40);
    connect(acceptBtn, &QPushButton::clicked, this, [&]() {
        commandJson["parameters"] = QJsonDocument::fromJson(edit->toPlainText().toUtf8()).object();
        currentCommand->fromJson(commandJson);
        dialog->close();
    });

    dialog->exec();
}

void MacrosDialog::onMoveRecordButtonClicked() {
    auto* dialog = new QDialog(this, Qt::WindowStaysOnTopHint);
    dialog->setWindowTitle("Передвинуть каретку записи");
    auto* label = new QLabel(dialog);
    label->setText("Поставить запись после какой операции?");
    label->setGeometry(10, 10, 180, 30);

    auto* edit = new QLineEdit(dialog);
    edit->setValidator(new QIntValidator(0, _operationList->topLevelItemCount()));
    edit->setText(0);
    edit->setGeometry(10, 45, 180, 30);
    edit->show();

    auto* cancelBtn = new QPushButton("Отмена", dialog);
    cancelBtn->setGeometry(10, 80, 85, 30);
    connect(cancelBtn, &QPushButton::clicked, dialog, &QDialog::close);
    cancelBtn->show();

    auto* acceptBtn = new QPushButton("Применить", dialog);
    acceptBtn->setGeometry(105, 80, 85, 30);
    connect(acceptBtn, &QPushButton::clicked, this, [&]() {
        auto index = edit->text().toInt();
        auto count = _operationList->topLevelItemCount();
        if(index >= 0 && index <= count) {
            _macrosManager->setRecordIndex(index);
            dialog->close();
        } else {
            edit->clear();
            edit->setPlaceholderText("Неверный индекс. Попробуйте снова");
        }
    });
    acceptBtn->show();

    dialog->setFixedSize(200, 120);
    dialog->exec();
}

void MacrosDialog::onRecordIndexChanged(int index) {
    _ui->indexLineEdit->setText(QString::number(index));
}

void MacrosDialog::onOperationExecuted(int index, bool isSuccessful) {
    if(isSuccessful) {
        _operationList->topLevelItem(index)->setBackground(0, QBrush(QColor(0, 255, 0)));
    } else {
        _operationList->topLevelItem(index)->setBackground(0, QBrush(QColor(255, 0, 0)));
        _ui->nextDebugButton->setEnabled(false);
        _ui->skipButton->setEnabled(false);
    }
}

void MacrosDialog::onOperationSkipped(int index) {
    _operationList->topLevelItem(index)->setBackground(0, QBrush(QColor(150, 150, 150)));
}

void MacrosDialog::keyPressEvent(QKeyEvent * event) {
    if(event->key() == Qt::Key_Delete && _ui->operationList->currentItem()) {
        QMessageBox mBox;
        QString name = _ui->operationList->currentItem()->text(0);

        mBox.setText(QString("Удалить %1?").arg(name));
        mBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        int dialogResponse = mBox.exec();
        if(dialogResponse == QMessageBox::Ok) {
            auto index = _operationList->currentItem()->text(0).split('.')[0].toInt() - 1;
            removeOperation(index);
        }
    }
}

void MacrosDialog::removeOperation(int index) {
    _ui->operationList->takeTopLevelItem(index);
    _macrosManager->remove(index);
    reindex(index);
}

void MacrosDialog::reindex(int indexFrom, int indexTo) {
    for(auto i = indexFrom; i <= indexTo; i++) { //check
        auto text = _operationList->topLevelItem(i)->text(0).split(". ");
        _operationList->topLevelItem(i)->setText(0, QString("%1. %2").arg(i + 1).arg(text[1]));
    }
}

void MacrosDialog::reindex(int indexFrom) {
    reindex(indexFrom, _operationList->topLevelItemCount() - 1);
}

QTreeWidgetItem* MacrosDialog::createOperationItem(int index, QString operation, QString comment) {
    auto root = new QTreeWidgetItem(_operationList);
    root->setText(0, QString("%1. %2").arg(index).arg(operation));
    //auto commentItem = new QTreeWidgetItem(root);
    //auto commentList = comment.trimmed();
    //commentItem->setText(0, commentList);
    return root;
}

MacrosDialog::~MacrosDialog() {
    delete _ui;
}
